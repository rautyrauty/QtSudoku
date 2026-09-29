# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Run

CMake-based Qt6 Quick application. Requires `Qt6::Quick`.

```bash
cmake -S . -B build
cmake --build build
./build/SudokuApp
```

`CMakeLists.txt` uses `GLOB_RECURSE` over `src/*.cpp src/*.h`, so new C++ sources under `src/` are picked up automatically on reconfigure. Everything except `src/main.cpp` goes into the `sudoku_core` static library, which both `SudokuApp` and the tests link. QML and font resources must be registered in `src/qml.qrc` — they are compiled in via `AUTORCC`.

## Tests

Qt Test based unit tests live in `tests/` (`tst_cell`, `tst_sudoku`, `tst_blockmodel`, `tst_threads`), built only with `-DBUILD_TESTING=ON`. `CMakePresets.json` has one preset per sanitizer setup:

```bash
cmake --preset asan-ubsan && cmake --build --preset asan-ubsan && ctest --preset asan-ubsan
```

Presets: `tests` (no sanitizers), `asan-ubsan`, `lsan`, `tsan`, `valgrind` (memcheck stands in for MSan, which false-positives on the uninstrumented system Qt). Sanitizers are set through the `SUDOKU_SANITIZERS` cache variable (e.g. `address;undefined`), which also turns on `_GLIBCXX_ASSERTIONS`. Sanitizer runtime options are passed to the tests from `tests/CMakeLists.txt`.

`solve()` is a naive randomized backtracker: boards with 10..25 givens can take seconds each, so tests avoid that range.

No linters configured.

## Architecture

Two-layer split: a C++ model exposed to a pure-QML frontend via `qmlRegisterType` in `src/main.cpp`.

**C++ model (`src/Sudoku.{h,cpp}`, `src/Cell.{h,cpp}`)**
- `Cell` is the generation/solving primitive. It tracks a 9-slot `_free_digits` bitset of still-possible values; `GenerateDigit()` picks one uniformly from the remaining free set. `RemoveFD`/`Reset` drive backtracking.
- `Sudoku` owns the 9×9 `_cells` grid and implements three backtracking passes over `Cell`:
  - `generate(open_slots_count)` — fills the grid, then randomly locks `open_slots_count` cells as the puzzle givens.
  - `solve()` — backtracking solver that respects locked cells.
  - `findError()` — used by both `check()` (victory detection, appends a line to `base.txt`) and `help()` (returns `QPoint` of first offending cell).
- The QML view does not see the raw grid. Instead, `Sudoku` owns nine `SudokuBlockModel` instances (one per 3×3 block), each a `QAbstractListModel` with roles `digit` / `isLocked` / `blockIndex`. `blockModel(i)` is `Q_INVOKABLE`; the QML side iterates 0..8 and binds each block to its own `Repeater`. After mutating cells, C++ must emit `dataChanged` on the affected block model — `setDigit` does this for a single cell, `updateBlockModels()` does it for all nine (used after `generate`/`solve`).
- Coordinate convention: C++ internals use `_cells[row][column]` = `_cells[y][x]`, but the `Q_INVOKABLE` surface (`setDigit`, `getDigit`, `help()` returning `QPoint`) uses `(x, y)` order. Block index is `x/3 + y/3*3`; within a block, the list row is `x%3 + y%3*3`.

**QML frontend (`src/qml/`)**
- `main.qml` — `ApplicationWindow` with a `StackView` switching between `startScreen` (menu + open-slots input) and `sudokuScreen` (`SudokuPage`). Instantiates one `SudokuModel { id: sdk }` shared across screens. Also hosts the `ConfettiCanvas` overlay fired on victory.
- `SudokuPage.qml` / `Sudoku.qml` / `SudokuCell.qml` — the board view. Renders nine blocks, each backed by `sdk.blockModel(i)`.
- `MenuBtn.qml` — shared styled button.
- `ConfettiCanvas.qml` — vendored from [MeldStudio/canvas-confetti-qml](https://github.com/MeldStudio/canvas-confetti-qml) (see README credits); do not rewrite from scratch.
- Fonts: `RussoOne-Regular.ttf` is loaded via `FontLoader` in `main.qml` and referenced by `russoFontLoader.name`.

**Persistence**
- `Sudoku::check()` appends `"Won with N open cells"` to `Sudoku::historyFilePath()` (`base.txt` under `QStandardPaths::AppDataLocation`, e.g. `~/.local/share/qtsudoku/`) on victory. This is the only persistent state; the README flags replacing this flat file with a real leaderboard as a TODO.

## Conventions

- C++17, tabs for indentation (see commit `6cf4493`). Qt6 only.
- Recent direction (commit `7480513`): the UI is QML; do not add `QtWidgets` back. The `QPainter`/`QPaintEvent`/`QIntValidator` includes still in `Sudoku.h` are legacy leftovers.
- When adding cell mutation paths on the C++ side, remember to emit `dataChanged` on the right `SudokuBlockModel` — the view will not otherwise refresh.
