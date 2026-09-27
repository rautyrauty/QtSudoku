#pragma once

#include <QAbstractTableModel>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QIntValidator>
#include <QTimer>
#include <QFile>
#include <QString>
#include <QPoint>
#include <QList>

#include "Cell.h"
#include "SudokuBlockModel.h"

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

	static QString historyFilePath();

private:
	friend class SudokuBlockModel;

	std::pair<int,int> findError() const;
	int getBlockIndex(int x, int y) const;
	void updateBlockModels() const;
	std::array<std::unique_ptr<SudokuBlockModel>, 9> _blockModels;
	std::array<std::array<Cell, 9>, 9> _cells;
	int _open_slots_count = 0;
};
