#include <QtTest>

#include "Sudoku.h"

class TestBlockModel : public QObject
{
	Q_OBJECT
private slots:
	void blockModelRange()
	{
		Sudoku sdk;
		QVERIFY(sdk.blockModel(-1) == nullptr);
		QVERIFY(sdk.blockModel(9) == nullptr);
		for (int i = 0; i < 9; ++i) {
			QVERIFY(sdk.blockModel(i) != nullptr);
			QCOMPARE(sdk.blockModel(i)->rowCount(), 9);
		}
	}

	void roleNames()
	{
		Sudoku sdk;
		const auto names = sdk.blockModel(0)->roleNames();
		QCOMPARE(names.value(SudokuBlockModel::DigitRole), QByteArray("digit"));
		QCOMPARE(names.value(SudokuBlockModel::IsLockedRole), QByteArray("isLocked"));
		QCOMPARE(names.value(SudokuBlockModel::IndexRole), QByteArray("blockIndex"));
	}

	void dataMatchesGrid()
	{
		Sudoku sdk;
		sdk.generate(40);
		for (int y = 0; y < 9; ++y) {
			for (int x = 0; x < 9; ++x) {
				SudokuBlockModel* model = sdk.blockModel(x / 3 + y / 3 * 3);
				const QModelIndex index = model->index(x % 3 + y % 3 * 3, 0);
				QCOMPARE(model->data(index, SudokuBlockModel::DigitRole).toInt(), sdk.getDigit(x, y));
				QCOMPARE(model->data(index, SudokuBlockModel::IsLockedRole).toBool(), bool(sdk.isLocked(x, y)));
				QCOMPARE(model->data(index, SudokuBlockModel::IndexRole).toInt(), x / 3 + y / 3 * 3);
			}
		}
	}

	void dataRejectsBadIndexAndRole()
	{
		Sudoku sdk;
		SudokuBlockModel* model = sdk.blockModel(4);
		QVERIFY(not model->data(QModelIndex(), SudokuBlockModel::DigitRole).isValid());
		QVERIFY(not model->data(model->index(9, 0), SudokuBlockModel::DigitRole).isValid());
		QVERIFY(not model->data(model->index(0, 0), Qt::DisplayRole).isValid());
	}

	void setDigitNotifiesOwningBlockOnly()
	{
		Sudoku sdk;
		sdk.generate(0);

		std::vector<std::unique_ptr<QSignalSpy>> spies;
		for (int i = 0; i < 9; ++i) {
			spies.push_back(std::make_unique<QSignalSpy>(sdk.blockModel(i), &QAbstractItemModel::dataChanged));
		}

		sdk.setDigit(7, 4, 3);
		for (int i = 0; i < 9; ++i) {
			QCOMPARE(spies[i]->count(), i == 5 ? 1 : 0);
		}
		const auto args = spies[5]->takeFirst();
		QCOMPARE(args.at(0).value<QModelIndex>().row(), 4);
		QCOMPARE(args.at(1).value<QModelIndex>().row(), 4);
	}

	void generateNotifiesAllBlocks()
	{
		Sudoku sdk;
		std::vector<std::unique_ptr<QSignalSpy>> spies;
		for (int i = 0; i < 9; ++i) {
			spies.push_back(std::make_unique<QSignalSpy>(sdk.blockModel(i), &QAbstractItemModel::dataChanged));
		}
		sdk.generate(20);
		for (const auto& spy : spies) {
			QCOMPARE(spy->count(), 1);
			const auto args = spy->takeFirst();
			QCOMPARE(args.at(0).value<QModelIndex>().row(), 0);
			QCOMPARE(args.at(1).value<QModelIndex>().row(), 8);
		}
	}
};

QTEST_GUILESS_MAIN(TestBlockModel)
#include "tst_blockmodel.moc"
