#include "test_initial.h"

void Initial::toUpper() {
	QString str = "Hello";
	QCOMPARE(str.toUpper(), QString("HELLO"));
}

QTEST_MAIN(Initial)
#include "test_initial.moc"
