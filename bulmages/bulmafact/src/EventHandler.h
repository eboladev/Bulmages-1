#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <qobject.h>


class EventHandler : public QObject {

	Q_OBJECT

public:
	EventHandler(QWidget *);
	~EventHandler();
	bool eventFilter( QObject *obj, QEvent *ev );
	

private:
	void nextCell(QObject *obj);
	void duplicateCell(QObject *obj);
	
protected:
	//bool eventFilter( QObject *obj, QEvent *ev );
};

#endif
