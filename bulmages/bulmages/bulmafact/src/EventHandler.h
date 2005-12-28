#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <qobject.h>
#include <q3table.h>
//Added by qt3to4:
#include <QEvent>

#define QUANTITY 1
#define DATE 2


class EventHandler : public QObject {
Q_OBJECT
public:
	EventHandler(QWidget *, const int);
	EventHandler(QWidget *, Q3Table *);
	~EventHandler();
	bool eventFilter( QObject *, QEvent *);
//	int const QUANTITY=1;
//	int const DATE=2;
	

private:
	void nextCell(QObject *);
	void duplicateCell(QObject *);
	int m_TextEditType;
	void manageDate(QObject *);
	void manageQuantity(QObject *);
	
	//QString * columnFunctions[];
	
protected:
	//bool eventFilter( QObject *obj, QEvent *ev );
};

#endif
