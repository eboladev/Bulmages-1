// (C) Tomeu Borr� Riera, Febrero 2005

#ifndef __LISTVENTANAS__
#define __LISTVENTANAS__
#include <q3dockwindow.h>
//#include <QDockWidget>

#include <Q3ListBox>


/** \brief Provides the dock window to list all windows in BulmaFact
 ** \param m_listBox this variable is the listBox that contains all titles of the diferent windows.
*/
class listventanas : public Q3DockWindow {
//class listventanas : public QDockWidget {
Q_OBJECT
private:
	Q3ListBox *m_listBox ;
public:
	listventanas(const QString & title, QWidget *a=0, Qt::WFlags b=0);
	~listventanas();
	public:
	int meteWindow(QString, QObject *);
	void sacaWindow(QObject *);
public slots:
	virtual void dclicked();
	virtual void clicked();
//	virtual bool close() {hide();return FALSE ;};
};

#endif

