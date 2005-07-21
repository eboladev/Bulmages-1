// (C) Tomeu Borr� Riera, Febrero 2005

#ifndef __LISTVENTANAS__
#define __LISTVENTANAS__
#include <qdockwindow.h>
#include <qlistbox.h>


/** \brief Provides the dock window to list all windows in BulmaFact
 ** \param m_listBox this variable is the listBox that contains all titles of the diferent windows.
*/
class listventanas : public QDockWindow {
Q_OBJECT
	private:
	QListBox *m_listBox ;
	public:
	listventanas(QWidget *, const char *);
	~listventanas();
	public:
	void meteWindow(QString, QObject *);
	void sacaWindow(QObject *);
public slots:
	virtual void dclicked();
	virtual void clicked();
};

#endif

