# Development

## Updating the demo

Regenerate the animated demo from the current QML interface. Python 3 and Pillow
are required in addition to the application's Qt 6 Quick dependencies.

Run these commands from the repository root:

```sh
python3 -m venv build/media-venv
build/media-venv/bin/pip install Pillow
cmake -S . -B build -DSUDOKU_MEDIA_PYTHON="$PWD/build/media-venv/bin/python3"
cmake --build build --target screenshots
```

After setup, only the last command is needed. It replaces `screenshots/demo.gif`
with a looping demo: menu, entering digits, Help highlighting a mistake, completing
the puzzle, and victory confetti. It uses Qt's offscreen platform and software
renderer, so no desktop session or external screenshot utility is needed.
The animation is 800×800 without window borders. The puzzle is generated afresh
on each run, and no game history is written. Temporary frames are removed
automatically.

## TODO

- Options page with graphics settings, such as a color theme and fonts.
- About page with information about authors, application version, and Qt version.
- Restore the timer.
- Replace the text file leaderboard with a proper leaderboard.
