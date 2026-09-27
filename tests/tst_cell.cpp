#include <QtTest>

#include <set>

#include "Cell.h"

class TestCell : public QObject
{
	Q_OBJECT
private slots:
	void defaultsToEmptyAndOpen()
	{
		Cell cell;
		QCOMPARE(cell.GetDigit(), 0);
		QVERIFY(not cell.IsLocked());
	}

	void constructorStoresDigit()
	{
		Cell cell(7);
		QCOMPARE(cell.GetDigit(), 7);
	}

	void lockAndOpenToggleState()
	{
		Cell cell;
		cell.Lock();
		QVERIFY(cell.IsLocked());
		cell.Lock();
		QVERIFY(cell.IsLocked());
		cell.Open();
		QVERIFY(not cell.IsLocked());
		cell.Open();
		QVERIFY(not cell.IsLocked());
	}

	void generateDigitExhaustsAllNine()
	{
		Cell cell;
		std::set<int> seen;
		for (int i = 0; i < 9; ++i) {
			QVERIFY(cell.GenerateDigit());
			const int digit = cell.GetDigit();
			QVERIFY(digit >= 1 and digit <= 9);
			QVERIFY(seen.insert(digit).second);
		}
		QVERIFY(not cell.GenerateDigit());
	}

	void removeFDExcludesDigits()
	{
		for (int kept = 1; kept <= 9; ++kept) {
			Cell cell;
			for (int digit = 1; digit <= 9; ++digit) {
				if (digit != kept) {
					cell.RemoveFD(digit);
				}
			}
			QVERIFY(cell.GenerateDigit());
			QCOMPARE(cell.GetDigit(), kept);
			QVERIFY(not cell.GenerateDigit());
		}
	}

	void resetRestoresEverything()
	{
		Cell cell;
		for (int digit = 1; digit <= 9; ++digit) {
			cell.RemoveFD(digit);
		}
		cell.SetDigit(5);
		cell.Lock();
		cell.Reset();
		QCOMPARE(cell.GetDigit(), 0);
		QVERIFY(not cell.IsLocked());
		int generated = 0;
		while (cell.GenerateDigit()) {
			++generated;
		}
		QCOMPARE(generated, 9);
	}
};

QTEST_GUILESS_MAIN(TestCell)
#include "tst_cell.moc"
