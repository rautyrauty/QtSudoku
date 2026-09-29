#include <QtTest>

#include "Sudoku.h"
#include "testutils.h"

using namespace testutils;

class TestSudoku : public QObject
{
	Q_OBJECT
private slots:
	void initTestCase()
	{
		QStandardPaths::setTestModeEnabled(true);
	}

	void init()
	{
		QFile::remove(Sudoku::historyFilePath());
	}

	void generateLocksRequestedCount_data()
	{
		QTest::addColumn<int>("open");
		for (int open : {0, 1, 17, 30, 45, 80, 81}) {
			QTest::addRow("%d", open) << open;
		}
	}

	void generateLocksRequestedCount()
	{
		QFETCH(int, open);
		Sudoku sdk;
		sdk.generate(open);
		QCOMPARE(lockedCount(sdk), open);

		const Grid grid = snapshot(sdk);
		QVERIFY(noConflicts(grid));
		for (int y = 0; y < 9; ++y) {
			for (int x = 0; x < 9; ++x) {
				if (sdk.isLocked(x, y)) {
					QVERIFY(grid[y][x] >= 1 and grid[y][x] <= 9);
				} else {
					QCOMPARE(grid[y][x], 0);
				}
			}
		}
	}

	void regenerateResetsPreviousBoard()
	{
		Sudoku sdk;
		sdk.generate(81);
		sdk.generate(10);
		QCOMPARE(lockedCount(sdk), 10);
	}

	void fullyGivenBoardIsValid()
	{
		Sudoku sdk;
		sdk.generate(81);
		QVERIFY(isComplete(snapshot(sdk)));
		QCOMPARE(sdk.help(), QPoint(-1, -1));
	}

	void solveProducesValidBoardKeepingGivens_data()
	{
		QTest::addColumn<int>("open");
		for (int open : {0, 5, 30, 35, 40, 60, 81}) {
			QTest::addRow("%d", open) << open;
		}
	}

	void solveProducesValidBoardKeepingGivens()
	{
		QFETCH(int, open);
		for (int round = 0; round < 25; ++round) {
			Sudoku sdk;
			sdk.generate(open);
			const Grid givens = snapshot(sdk);
			QVERIFY(sdk.solve());
			const Grid solved = snapshot(sdk);
			QVERIFY2(isComplete(solved), qPrintable(QString("round %1").arg(round)));
			for (int y = 0; y < 9; ++y) {
				for (int x = 0; x < 9; ++x) {
					if (sdk.isLocked(x, y)) {
						QCOMPARE(solved[y][x], givens[y][x]);
					}
				}
			}
			QCOMPARE(sdk.help(), QPoint(-1, -1));
		}
	}

	void helpPointsAtFirstEmptyCellColumnMajor()
	{
		Sudoku sdk;
		sdk.generate(30);
		QPoint expected(-1, -1);
		for (int x = 0; x < 9 and expected.x() < 0; ++x) {
			for (int y = 0; y < 9; ++y) {
				if (not sdk.isLocked(x, y)) {
					expected = QPoint(x, y);
					break;
				}
			}
		}
		QCOMPARE(sdk.help(), expected);
	}

	void helpPointsAtConflictingDigit()
	{
		Sudoku sdk;
		sdk.generate(30);
		QVERIFY(sdk.solve());

		for (int y = 0; y < 9; ++y) {
			int openX = -1;
			int lockedDigit = 0;
			for (int x = 0; x < 9; ++x) {
				if (sdk.isLocked(x, y)) {
					lockedDigit = sdk.getDigit(x, y);
				} else if (openX < 0) {
					openX = x;
				}
			}
			if (openX >= 0 and lockedDigit != 0) {
				sdk.setDigit(openX, y, lockedDigit);
				QCOMPARE(sdk.help(), QPoint(openX, y));
				QVERIFY(not sdk.check());
				return;
			}
		}
		QSKIP("no row with both open and locked cells");
	}

	void setDigitIgnoresLockedCells()
	{
		Sudoku sdk;
		sdk.generate(81);
		const int before = sdk.getDigit(4, 4);
		sdk.setDigit(4, 4, before % 9 + 1);
		QCOMPARE(sdk.getDigit(4, 4), before);
	}

	void setDigitUsesXYOrder()
	{
		Sudoku sdk;
		sdk.generate(0);
		sdk.setDigit(2, 7, 5);
		QCOMPARE(sdk.getDigit(2, 7), 5);
		QCOMPARE(sdk.getDigit(7, 2), 0);
	}

	void checkFailsOnIncompleteBoard()
	{
		Sudoku sdk;
		sdk.generate(30);
		QVERIFY(not sdk.check());
		QVERIFY(not QFile::exists(Sudoku::historyFilePath()));
	}

	void checkRecordsVictory()
	{
		Sudoku sdk;
		sdk.generate(30);
		QVERIFY(sdk.solve());
		QVERIFY(sdk.check());

		QFile file(Sudoku::historyFilePath());
		QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
		QCOMPARE(QString::fromUtf8(file.readAll()), QString("Won with 30 open cells\n"));
	}

	void checkAppendsToHistory()
	{
		Sudoku sdk;
		sdk.generate(81);
		QVERIFY(sdk.check());
		QVERIFY(sdk.check());

		QFile file(Sudoku::historyFilePath());
		QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
		QCOMPARE(file.readAll().count('\n'), 2);
	}
};

QTEST_GUILESS_MAIN(TestSudoku)
#include "tst_sudoku.moc"
