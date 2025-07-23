#pragma once

#include <QAbstractTableModel>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QIntValidator>
#include <QTimer>
#include <QFile>
#include <QPoint>
#include <QList>

#include "Cell.h"

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

class Sudoku : public QObject
{
	Q_OBJECT
public:
	Sudoku(QObject* parent = nullptr);

	Q_INVOKABLE void generate(int open_slots_count);
	Q_INVOKABLE bool solve();
	Q_INVOKABLE QPoint help() const;
	Q_INVOKABLE bool check() const;

	Q_INVOKABLE SudokuBlockModel* blockModel(int index) const;

	Q_INVOKABLE void setDigit(int x, int y, int digit);
	int getDigit(int x, int y) const;
	int isLocked(int x, int y) const;

private:
	friend class SudokuBlockModel;

	std::pair<int,int> findError() const;
	int getBlockIndex(int x, int y) const;
	void updateBlockModels() const;
	std::array<std::unique_ptr<SudokuBlockModel>, 9> _blockModels;
	std::array<std::array<Cell, 9>, 9> _cells;
	int _open_slots_count;
};
