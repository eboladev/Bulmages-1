#include <listventanas.h>
#include <qobject.h>

#include <map>

#include "bulmafact.h"

extern bulmafact *bges;

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
    connect(m_listBox, SIGNAL(doubleClicked(QListBoxItem *)), this, SLOT(dclicked()));
    connect(m_listBox, SIGNAL(clicked(QListBoxItem *)), this, SLOT(clicked()));
}// end listventanas

void listventanas::dclicked() {
    fprintf(stderr,"listventanas::clicked()\n");
    int item = m_listBox->currentItem();
    QWidget *widget = (QWidget *)elmapa[item];
    if (widget != NULL) {
	fprintf(stderr,"El estado de la ventana es: %d, deberia ser %d\n",widget->windowState(),Qt::WindowMaximized);
	QWidget *punt = bges->workspace()->activeWindow();
	if (widget != punt) {
		widget->showMaximized();
	} else {
		if (!(widget->windowState() & Qt::WindowMaximized)) {
			widget->showMaximized();
		} else {
			widget->hide();
		}// end if
	}// end if
    }// end if
    fprintf(stderr,"END listventanas::clicked()\n");
}// end clicked



void listventanas::clicked() {
    fprintf(stderr,"listventanas::clicked()\n");
    int item = m_listBox->currentItem();
    QWidget *widget = (QWidget *)elmapa[item];
    if (widget != NULL) {
	fprintf(stderr,"El estado de la ventana es: %d, deberia ser %d\n",widget->windowState(),Qt::WindowMaximized);
	QWidget *punt = bges->workspace()->activeWindow();
	if (widget != punt) {
		widget->showNormal();
	} else {
		widget->hide();
	}// end if
    }// end if
    fprintf(stderr,"END listventanas::clicked()\n");
}// end clicked

listventanas::~listventanas() {
    delete m_listBox;
}// end ~listventanas

#include <qimage.h>

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
	if (((QWidget *)obj)->icon()) {
		QPixmap icon = *((QWidget *)obj)->icon();
		QImage imgicon = icon.convertToImage();
		imgicon = imgicon.scale(32,32);
		icon = imgicon;
       		m_listBox->insertItem(icon,nombre,-1);
	} else {
		m_listBox->insertItem(nombre,-1);
	}
        elmapa[m_listBox->count()-1]=obj;
    }// end if
}// end meteWindow


void listventanas::sacaWindow(QObject *obj) {
    fprintf(stderr,"listventanas::sacaWindow()\n");
    mapa::iterator iterador = elmapa.begin();
    while (iterador != elmapa.end()) {
        if ((*iterador).second == obj) {
            m_listBox->removeItem((*iterador).first);
            elmapa.erase((*iterador).first);
        }
        iterador++;
    }// end while
}// end sacaWindow

