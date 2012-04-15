#include "test_initial.h"

void Initial::toUpper_data()
{
	QTest::addColumn<QString>("string");
	QTest::addColumn<QString>("result");

	QTest::newRow("all lower") << "hello" << "HELLO";
	QTest::newRow("mixed")     << "Hello" << "HELLO";
	QTest::newRow("all upper") << "HELLO" << "HELLO";
}

void Initial::toUpper() {
	QFETCH(QString, string);
	QFETCH(QString, result);
	QCOMPARE(string.toUpper(), result);

	QBENCHMARK {
		int res = result.compare(string.toUpper());
	}
}

void Initial::testGui_data() {
	QTest::addColumn<QTestEventList>("events");
	QTest::addColumn<QString>("expected");

	QTestEventList list1;
	list1.addKeyClick('a');
	QTest::newRow("char") << list1 << "a";

	QTestEventList list2;
	list2.addKeyClick('a');
	list2.addKeyClick(Qt::Key_Backspace);
	QTest::newRow("there and back again") << list2 << "";

	list1.addKeyClick('y');
	QTest::newRow("char") << list1 << "ay";

	list2.addKeyClick('a');
	list2.addKeyClick(Qt::Key_Backspace);
	QTest::newRow("there and back again") << list2 << "";
}

void Initial::testGui() {
	QFETCH(QTestEventList, events);
	QFETCH(QString, expected);

	QLineEdit lineEdit;

	events.simulate(&lineEdit);

	QCOMPARE(lineEdit.text(), expected);

	QBENCHMARK {
		int result = (lineEdit.text() == expected);
	}
}

QTEST_MAIN(Initial)
#include "test_initial.moc"
