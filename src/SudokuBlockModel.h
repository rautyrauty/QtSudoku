#pragma once

#include <QAbstractListModel>

class Sudoku;

class SudokuBlockModel : public QAbstractListModel
{
	Q_OBJECT
public:
	SudokuBlockModel(Sudoku* sudoku, int blockIndex, QObject* parent = nullptr);
	enum Roles {
		DigitRole = Qt::UserRole,
		IsLockedRole = Qt::UserRole + 1,
		IndexRole = Qt::UserRole + 2
	};

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QHash<int, QByteArray> roleNames() const override;

private:
	Sudoku* _sudoku;
	int _blockIndex;
};
