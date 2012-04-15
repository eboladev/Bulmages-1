#ifndef __INITIAL__
#define __INITIAL__
#include<QtCore>
#include<QtGui>
#include<QtTest>

class Initial: public QObject {
	Q_OBJECT
	private slots:

	// Basic string testing
	void toUpper_data();
	void toUpper();

	// Basic UI testing
	void testGui_data();
	void testGui();
};
#endif //__INITIAL__
