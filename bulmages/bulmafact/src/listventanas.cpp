#include <listventanas.h>
#include <qobject.h>

#include <map>

using namespace std;
typedef QObject *pQObject;

typedef map<int, pQObject> mapa;

mapa elmapa;

listventanas::listventanas(QWidget *a, const char *b): QDockWindow (a,b) {
    m_listBox = new QListBox( this ,0 );
    m_listBox->setCaption( tr( "Ventanas Abiertas") );
    setGeometry(100,100,100,500);
    setFixedExtentWidth(200);
    setWidget(m_listBox);
    setResizeEnabled(TRUE);
    setMovingEnabled(TRUE);
    connect(m_listBox, SIGNAL(doubleClicked(QListBoxItem *)), this, SLOT(clicked()));
}// end listventanas

void listventanas::clicked() {
    fprintf(stderr,"Se ha pulsado\n");
    int item = m_listBox->currentItem();
    QWidget *widget = (QWidget *)elmapa[item];
    //	widget->hide();
    widget->showMaximized();
    widget->setActiveWindow();
}// end clicked


listventanas::~listventanas() {
    delete m_listBox;
}// end ~listventanas

/** \brief This function provides the ability of push a window in the dock window
 ** \param  nombre This QStrin is the name of the window that was shown in the listbox
 ** \param obj This QObject * contains the pointer of the window for furtner reference.
 */
void  listventanas::meteWindow(QString nombre, QObject *obj) {
    mapa::iterator iterador = elmapa.begin();
    while (iterador != elmapa.end()) {
        if ((*iterador).second == obj) {
             m_listBox->removeItem((*iterador).first);
            elmapa.erase((*iterador).first);
        }// end if
        iterador++;
    }// end while
    if( iterador == elmapa.end()) {
        m_listBox->insertItem(nombre,-1);
        elmapa[m_listBox->count()-1]=obj;
    }// end if
}// end meteWindow


void listventanas::sacaWindow(QObject *obj) {
    mapa::iterator iterador = elmapa.begin();
    while (iterador != elmapa.end()) {
        if ((*iterador).second == obj) {
            m_listBox->removeItem((*iterador).first);
            elmapa.erase((*iterador).first);
        }
        iterador++;
    }// end while
}// end sacaWindow

