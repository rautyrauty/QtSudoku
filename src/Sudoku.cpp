#include "Sudoku.h"

Sudoku::Sudoku(QObject* parent) :
	QObject(parent)
{
	for (int i = 0; i < 9; ++i) {
		_blockModels[i] = std::make_unique<SudokuBlockModel>(this, i, this);
	}
}

void Sudoku::generate(int open_slots_count)
{
	std::array<std::array<Cell, 9>, 9> sdk{};

	for (int row = 0; row < 9; )
	{
		for (int column = 0; column < 9; )
		{
			for (int tmp_column = 0; tmp_column < column; tmp_column += 1)
			{
				sdk[row][column].RemoveFD(sdk[row][tmp_column].GetDigit());
			}
			for (int tmp_row = 0; tmp_row < row; tmp_row += 1)
			{
				sdk[row][column].RemoveFD(sdk[tmp_row][column].GetDigit());
			}

			{
				int tmp_row = row / 3 * 3;
				int tmp_column = column / 3 * 3;

				while (not ((tmp_row == row) && (tmp_column == column)))
				{
					sdk[row][column].RemoveFD(sdk[tmp_row][tmp_column].GetDigit());
					tmp_column += 1;
					if (tmp_column >= column / 3 * 3 + 3)
					{
						tmp_row += 1;
						tmp_column = column / 3 * 3;
					}
				}
			}


			if (not sdk[row][column].GenerateDigit())
			{
				if ((row == 0) and (column == 0)) throw std::runtime_error("Generation failed");;
				sdk[row][column].Reset();
				if (column == 0)
				{
					row -= 1;
					column = 8;
				}
				else column -= 1;

				continue;
			}
			column += 1;
		}
		row += 1;
	}

	std::array<bool, 9 * 9> opened{};

	std::random_device rd;
	std::mt19937 gen(rd());
	for (int i = 0; i < open_slots_count; i += 1)
	{
		std::uniform_int_distribution<int> dist(0, 9*9 - i - 1);
		int tmp = dist(gen);
		int true_num = 0;
		while (true)
		{
			if (not opened[true_num])
			{
				if (tmp == 0)
				{
					break;
				}
				tmp -= 1;
			}
			true_num += 1;
		}
		opened[true_num] = true;
	}
	for (int row = 0; row < 9; row += 1)
	{
		for (int column = 0; column < 9; column += 1)
		{
			if (opened[column + row * 9]) {
				_cells[row][column].SetDigit(sdk[row][column].GetDigit());
				_cells[row][column].Lock();
				} else {
				_cells[row][column].SetDigit(0);
				_cells[row][column].Open();
			}
		}
	}

	updateBlockModels();
}

bool Sudoku::solve()
{
	std::array<std::array<bool, 9>, 9> columns{};
	std::array<std::array<bool, 9>, 9> rows{};
	std::array<std::array<bool, 9>, 9> squares{};
	for (auto& col : columns) col.fill(true);
	for (auto& row : rows) row.fill(true);
	for (auto& sq : squares) sq.fill(true);

	for (int column = 0; column < 9; column += 1)
	{
		for (int row = 0; row < 9; row += 1)
		{
			if (not _cells[row][column].IsLocked())
			{
				continue;
			}

			if ((columns[column][_cells[row][column].GetDigit()-1]) and (rows[row][_cells[row][column].GetDigit()-1])
					and (squares[row / 3 + column / 3 * 3][_cells[row][column].GetDigit() - 1]))
			{
				columns[column][_cells[row][column].GetDigit() - 1] = false;
				rows[row][_cells[row][column].GetDigit() - 1] = false;
				squares[row / 3 + column / 3 * 3][_cells[row][column].GetDigit() - 1] = false;
			}
			else
			{
				return false;
			}
		}
	}

	std::array<std::array<Cell, 9>, 9> sdk{};

	for (int row = 0; row < 9; )
	{
		for (int column = 0; column < 9; )
		{
			if (_cells[row][column].IsLocked())
			{
				sdk[row][column].SetDigit(_cells[row][column].GetDigit());
				column += 1;
				continue;
			}
			for (int tmp_column = 0; tmp_column < column; tmp_column += 1)
			{
				sdk[row][column].RemoveFD(sdk[row][tmp_column].GetDigit());
			}
			for (int tmp_row = 0; tmp_row < row; tmp_row += 1)
			{
				sdk[row][column].RemoveFD(sdk[tmp_row][column].GetDigit());
			}
			for (int tmp_column = column+1; tmp_column < 9; tmp_column += 1)
			{
				if (_cells[row][tmp_column].IsLocked())
				{
					sdk[row][column].RemoveFD(_cells[row][tmp_column].GetDigit());
				}
			}
			for (int tmp_row = row+1; tmp_row < 9; tmp_row += 1)
			{
				if (_cells[tmp_row][column].IsLocked())
				{
					sdk[row][column].RemoveFD(_cells[tmp_row][column].GetDigit());
				}
			}

			{
				int tmp_row = row / 3 * 3;
				int tmp_column = column / 3 * 3;
				while (not ((tmp_row == row) and (tmp_column == column)))
				{
					sdk[row][column].RemoveFD(sdk[tmp_row][tmp_column].GetDigit());
					tmp_column += 1;
					if (tmp_column >= column / 3 * 3 + 3)
					{
						tmp_row += 1;
						tmp_column = column / 3 * 3;
					}
				}
			}

			{
				int tmp_row = row / 3 * 3 + 2;
				int tmp_column = column / 3 * 3 + 2;
				while (not ((tmp_row == row) and (tmp_column == column)))
				{
					if (_cells[tmp_row][tmp_column].IsLocked())
					{
						sdk[row][column].RemoveFD(_cells[tmp_row][tmp_column].GetDigit());
					}
					tmp_column -= 1;
					if (tmp_column < column / 3 * 3)
					{
						tmp_row -= 1;
						tmp_column = column / 3 * 3 + 2;
					}
				}
			}

			if (not sdk[row][column].GenerateDigit())
			{
				sdk[row][column].Reset();

				if (column == 0)
				{
					row -= 1;
					column = 9 -1;
				}
				else column -= 1;

				if (row<0)
				{
					return false;
				}

				while (_cells[row][column].IsLocked())
				{

					if (column == 0)
					{
						row -= 1;
						column = 9 - 1;
					}
					else column -= 1;

					if (row<0)
					{
						return false;
					}
				}

				continue;
			}
			column += 1;
		}
		row += 1;
	}

	for (int row = 0; row < 9; row += 1)
	{
		for (int column = 0; column < 9; column += 1)
		{
			_cells[row][column].SetDigit(sdk[row][column].GetDigit());
		}
	}

	return true;
}

int Sudoku::getDigit(int x, int y) const
{
	return _cells[y][x].GetDigit();
}

int Sudoku::isLocked(int x, int y) const
{
	return _cells[y][x].IsLocked();
}

QPoint Sudoku::help() const
{
	const auto [row, col] = findError();
	if (row == -1 && col == -1) {
		return QPoint{-1,-1};
	}
	else
	{
		return QPoint{col,row};
	}
}

bool Sudoku::check() const
{
	const auto [row, col] = findError();
	if (row == -1 && col == -1) {
		if (QFile file("base.txt"); file.open(QIODevice::Append)) {
			QString result = "Won with " + QString::number(_open_slots_count) +  " open cells";
			QTextStream out(&file);
			out << result << Qt::endl;
		}
		return true;
	}
	return false;
}

SudokuBlockModel* Sudoku::blockModel(int index) const {
	if (index >= 0 && index < 9) {
		return _blockModels[index].get();
	}
	return nullptr;
}

void Sudoku::setDigit(int x, int y, int digit)
{
	if (isLocked(x,y)) {
		return;
	}

	_cells[y][x].SetDigit(digit);
	int block_index = getBlockIndex(x,y);
	QModelIndex changed = _blockModels[block_index]->index(x%3 + y%3*3, 0);
	emit _blockModels[block_index]->dataChanged(changed, changed);
}

std::pair<int, int> Sudoku::findError() const
{
	std::array<std::array<bool, 9>, 9> columns{};
	std::array<std::array<bool, 9>, 9> rows{};
	std::array<std::array<bool, 9>, 9> squares{};
	for (auto& col : columns) col.fill(true);
	for (auto& row : rows) row.fill(true);
	for (auto& sq : squares) sq.fill(true);

	for (int column = 0; column < 9; column += 1)
	{
		for (int row = 0; row < 9; row += 1)
		{
			if (_cells[row][column].IsLocked()) {
				columns[column][_cells[row][column].GetDigit() - 1] = false;
				rows[row][_cells[row][column].GetDigit() - 1] = false;
				squares[row / 3 + column / 3 * 3][_cells[row][column].GetDigit() - 1] = false;
			}
		}
	}

	for (int column = 0; column < 9; column += 1)
	{
		for (int row = 0; row < 9; row += 1)
		{
			if (_cells[row][column].IsLocked()) {
				continue;
			}
			if (_cells[row][column].GetDigit() == 0)
			{
				return {row, column};
			}

			if ((columns[column][_cells[row][column].GetDigit()-1]) and (rows[row][_cells[row][column].GetDigit()-1])
					and (squares[row / 3 + column / 3 * 3][_cells[row][column].GetDigit() - 1]))
			{
				columns[column][_cells[row][column].GetDigit() - 1] = false;
				rows[row][_cells[row][column].GetDigit() - 1] = false;
				squares[row / 3 + column / 3 * 3][_cells[row][column].GetDigit() - 1] = false;
			}
			else
			{
				return {row, column};
			}
		}
	}

	return {-1,-1};
}

int Sudoku::getBlockIndex(int x, int y) const
{
	return x / 3 + y / 3 * 3;
}

void Sudoku::updateBlockModels() const
{
	for (int i = 0; i < 9; ++i) {
		QModelIndex topLeft = _blockModels[i]->index(0, 0);
		QModelIndex bottomRight = _blockModels[i]->index(8, 0);
		emit _blockModels[i]->dataChanged(topLeft, bottomRight);
	}
}
