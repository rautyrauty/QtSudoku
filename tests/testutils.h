#pragma once

#include <array>

#include "Sudoku.h"

namespace testutils {

using Grid = std::array<std::array<int, 9>, 9>;

inline Grid snapshot(const Sudoku& sdk)
{
	Grid grid{};
	for (int y = 0; y < 9; ++y) {
		for (int x = 0; x < 9; ++x) {
			grid[y][x] = sdk.getDigit(x, y);
		}
	}
	return grid;
}

inline int lockedCount(const Sudoku& sdk)
{
	int count = 0;
	for (int y = 0; y < 9; ++y) {
		for (int x = 0; x < 9; ++x) {
			count += sdk.isLocked(x, y) ? 1 : 0;
		}
	}
	return count;
}

inline bool noConflicts(const Grid& grid)
{
	for (int i = 0; i < 9; ++i) {
		std::array<bool, 10> row{}, column{}, block{};
		for (int j = 0; j < 9; ++j) {
			const int r = grid[i][j];
			const int c = grid[j][i];
			const int b = grid[i / 3 * 3 + j / 3][i % 3 * 3 + j % 3];
			if ((r and row[r]) or (c and column[c]) or (b and block[b])) {
				return false;
			}
			row[r] = column[c] = block[b] = true;
		}
	}
	return true;
}

inline bool isComplete(const Grid& grid)
{
	for (const auto& row : grid) {
		for (int digit : row) {
			if (digit < 1 or digit > 9) {
				return false;
			}
		}
	}
	return noConflicts(grid);
}

} // namespace testutils
