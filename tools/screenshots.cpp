#include "Sudoku.h"

#include <QDir>
#include <QEventLoop>
#include <QGuiApplication>
#include <QImage>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlExpression>
#include <QQuickWindow>
#include <QQuickItem>
#include <QTimer>

#include <stdexcept>
#include <vector>

static void settle(int milliseconds = 700)
{
	QEventLoop loop;
	QTimer::singleShot(milliseconds, &loop, &QEventLoop::quit);
	loop.exec();
}

static QVariant evaluate(QObject *root, const QString &code)
{
	QQmlExpression expression(qmlContext(root), root, code);
	const QVariant result = expression.evaluate();
	if (expression.hasError())
		throw std::runtime_error(expression.error().toString().toStdString());
	return result;
}

static void record(QQuickWindow *window, const QDir &directory, int count)
{
	static int frame = 0;
	for (int i = 0; i < count; ++i)
	{
		settle(100);
		const QImage image = window->grabWindow();
		const QString path = directory.filePath(QString("%1.png").arg(frame++, 4, 10, QChar('0')));
		if (image.isNull() || image.size() != QSize(800, 800) || !image.save(path))
			throw std::runtime_error(QString("Cannot capture %1").arg(path).toStdString());
	}
}

static QQuickItem *button(QQuickItem *item, const QString &text)
{
	if (item->property("text").toString() == text)
		return item;
	for (auto *child : item->childItems())
		if (auto *found = button(child, text))
			return found;
	return nullptr;
}

static void click(QQuickWindow *window, const QString &text)
{
	auto *item = button(window->contentItem(), text);
	if (!item || !QMetaObject::invokeMethod(item, "clicked"))
		throw std::runtime_error(QString("Cannot activate %1").arg(text).toStdString());
}

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	QGuiApplication::setApplicationName(QStringLiteral("qtsudoku-screenshots"));
	if (app.arguments().size() != 2)
	{
		qCritical("Usage: sudoku_screenshots OUTPUT_DIRECTORY");
		return 1;
	}

	qmlRegisterType<Sudoku>("SudokuModel", 1, 0, "SudokuModel");
	qmlRegisterUncreatableType<SudokuBlockModel>("SudokuModel", 1, 0,
		"SudokuBlockModel", "Created by Sudoku model");
	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
	if (engine.rootObjects().isEmpty())
		return 1;
	auto *window = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
	if (!window)
		return 1;

	try
	{
		const QDir directory(app.arguments().at(1));
		if (!QDir().mkpath(directory.absolutePath()))
			throw std::runtime_error("Cannot create output directory");
		settle();
		record(window, directory, 15);

		auto *sudoku = evaluate(window, "sdk").value<Sudoku *>();
		if (!sudoku)
			throw std::runtime_error("Cannot find the Sudoku model");
		sudoku->generate(40);
		evaluate(window, "stackView.push(sudokuScreen, StackView.Immediate)");
		record(window, directory, 15);

		if (!sudoku->solve() || sudoku->help() != QPoint(-1, -1))
			throw std::runtime_error("Cannot solve the screenshot puzzle");
		struct Entry { int x; int y; int digit; };
		std::vector<Entry> entries;
		for (int x = 0; x < 9; ++x)
		{
			for (int y = 0; y < 9; ++y)
			{
				if (!sudoku->isLocked(x, y))
				{
					entries.push_back({x, y, sudoku->getDigit(x, y)});
					sudoku->setDigit(x, y, 0);
				}
			}
		}
		for (int i = 0; i < 5; ++i)
		{
			const auto &entry = entries.at(i);
			sudoku->setDigit(entry.x, entry.y, entry.digit);
			record(window, directory, 4);
		}
		const auto &mistake = entries.at(5);
		int wrongDigit = mistake.digit % 9 + 1;
		for (int x = 0; x < 9; ++x)
			if (sudoku->isLocked(x, mistake.y))
			{
				wrongDigit = sudoku->getDigit(x, mistake.y);
				break;
			}
		sudoku->setDigit(mistake.x, mistake.y, wrongDigit);
		record(window, directory, 8);
		click(window, "Help");
		record(window, directory, 51);
		for (size_t i = 5; i < entries.size(); ++i)
		{
			const auto &entry = entries[i];
			sudoku->setDigit(entry.x, entry.y, entry.digit);
			if (i % 4 == 0)
				record(window, directory, 2);
		}
		if (sudoku->help() != QPoint(-1, -1))
			throw std::runtime_error("Expected a solved puzzle");
		click(window, "Check");
		record(window, directory, 35);
	}
	catch (const std::exception &error)
	{
		qCritical("%s", error.what());
		return 1;
	}
	return 0;
}
