// (C) Tomeu Borr� Riera, Febrero 2005

#ifndef __LISTVENTANAS__
#define __LISTVENTANAS__
#include <q3dockwindow.h>
//#include <QDockWidget>

#include <Q3ListBox>
#include <Q3ListBoxItem>



class Q3ListBox1 : public Q3ListBox {
Q_OBJECT
public:
	Q3ListBox1 ( QWidget * parent = 0, const char * name = 0, Qt::WFlags f = 0 ) : Q3ListBox (parent, name, f) {};
	~Q3ListBox1(){};
	void pintaitem(int i) {updateItem(i);};
	void pintaitem(Q3ListBoxItem *j) {updateItem(j);};
};


class Q3ListBoxItem1 : public Q3ListBoxPixmap {
private:
	QObject *m_obj;
	QString m_nombre;
	Q3ListBox1 *m_list;
public:
	Q3ListBoxItem1(Q3ListBox1 *l,QPixmap &p) : Q3ListBoxPixmap(l,p) {m_list=l;};
	~Q3ListBoxItem1(){};
	void setObject(QObject *m) {m_obj=m;};
	void setNombre(QString m) {m_nombre=m;setText(m);m_list->pintaitem(this);};
	QObject *object() {return m_obj;};
	QString nombre() {return m_nombre;};
	void paint ( QPainter * p ){Q3ListBoxPixmap::paint ( p );};
	int height ( const Q3ListBox * lb ) {return Q3ListBoxPixmap::height ( lb );};
	int width ( const Q3ListBox * lb ) {return Q3ListBoxPixmap::width ( lb );};
};


/** \brief Provides the dock window to list all windows in BulmaFact
 ** \param m_listBox this variable is the listBox that contains all titles of the diferent windows.
*/
class listventanas : public Q3DockWindow {
//class listventanas : public QDockWidget {
Q_OBJECT
private:
	Q3ListBox1 *m_listBox ;
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

