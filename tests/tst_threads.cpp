#include <QtTest>

#include <atomic>
#include <thread>

#include "Sudoku.h"
#include "testutils.h"

using namespace testutils;

class TestThreads : public QObject
{
	Q_OBJECT
private slots:
	void parallelGenerateAndSolve()
	{
		constexpr int threads = 4;
		constexpr int rounds = 20;
		std::atomic<int> failures{0};

		std::vector<std::thread> workers;
		for (int t = 0; t < threads; ++t) {
			workers.emplace_back([&failures, t] {
				for (int round = 0; round < rounds; ++round) {
					Sudoku sdk;
					const int open = 30 + (t * rounds + round) % 52;
					sdk.generate(open);
					if (lockedCount(sdk) != open or not sdk.solve() or not isComplete(snapshot(sdk))) {
						failures.fetch_add(1);
					}
				}
			});
		}
		for (auto& worker : workers) {
			worker.join();
		}
		QCOMPARE(failures.load(), 0);
	}
};

QTEST_GUILESS_MAIN(TestThreads)
#include "tst_threads.moc"
