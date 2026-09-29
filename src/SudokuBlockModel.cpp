#include "SudokuBlockModel.h"
#include "Sudoku.h"

SudokuBlockModel::SudokuBlockModel(Sudoku* sudoku, int blockIndex, QObject* parent) :
	QAbstractListModel(parent),
	_sudoku(sudoku),
	_blockIndex(blockIndex)
{}

int SudokuBlockModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return 9;
}

QVariant SudokuBlockModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() || index.row() >= 9) {
		return QVariant();
	}

	int x = _blockIndex % 3 * 3 + index.row() % 3;
	int y = _blockIndex / 3 * 3 + index.row() / 3;

	switch (role) {
		case DigitRole:
			return _sudoku->getDigit(x, y);
		case IsLockedRole:
			return _sudoku->isLocked(x, y);
		case IndexRole:
			return _blockIndex;
		default:
			return QVariant();
	}
}

QHash<int, QByteArray> SudokuBlockModel::roleNames() const
{
	return {
		{DigitRole, "digit"},
		{IsLockedRole, "isLocked"},
		{IndexRole, "blockIndex"},
	};
}
