
#include <QObject>
#include <QPixmap>
#include <map>
#include <QImage>
#include <QPainter>
#include <QVBoxLayout>

#include "listventanas.h"
#include "bulmafact.h"
#include "funcaux.h"

extern bulmafact *bges;
using namespace std;

listventanas::listventanas( QWidget *a): QDockWidget (a) {
    _depura("listventanas::INIT_listventanas()\n",0);
    m_listBox = new QListWidget1(this);
    m_listBox->setIconSize(QSize(32,32));
    m_listBox->setContentsMargins(0,0,0,0);
    m_listBox->setSpacing(0);
    m_listBox->setCaption( tr( "Indexador") );
    setAccessibleName(tr("Indexador"));
    setAccessibleDescription(tr("Indexador"));
    setWindowTitle(tr("Indexador"));
    setWidget(m_listBox);
    connect(m_listBox, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(dclicked()));
    connect(m_listBox, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(clicked()));
    _depura("listventanas::END_listventanas()\n",0);
}// end listventanas


void listventanas::dclicked() {
    _depura("listventanas::clicked()\n",0);
    QWidget *widget = (QWidget *)((QListWidgetItem1 *) m_listBox->currentItem())->object();

    if (widget != NULL) {
        widget->hide();
        widget->showMaximized();
    }// end if
    _depura("END listventanas::clicked()\n",0);
}// end clicked

void listventanas::clicked() {
    _depura("listventanas::clicked()\n",0);

    QWidget *widget = (QWidget *)((QListWidgetItem1 *) m_listBox->currentItem())->object();
    if (widget != NULL) {
        QWidget *punt = bges->workspace()->activeWindow();
        if (widget != punt) {
            widget->hide();
            widget->showNormal();
        }
    }// end if
    _depura("END listventanas::clicked()\n",0);
}

listventanas::~listventanas() {
    _depura("listventanas::destructor()\n",0);
    delete m_listBox;
}



/** \brief This function provides the ability of push a window in the dock window
 ** \param  nombre This QStrin is the name of the window that was shown in the listbox
 ** \param obj This QObject * contains the pointer of the window for furtner reference.
 */
int  listventanas::meteWindow(QString nombre, QObject *obj) {
    _depura("listventanas::INIT_meteWindow()",0);
    uint i=0;
    while (i < m_listBox->count())  {
        QListWidgetItem1 *m = (QListWidgetItem1 *)m_listBox->item(i);
        /// si la ventana ya esta en la lista
        if (m->object() == obj) {
            _depura("Ventana ya existente "+nombre,0);
            m->setNombre(nombre);
            return 0;
        }// end if

        /// Comprobamos ventanas duplicadas.
        if (m->nombre() == nombre) {
            _depura("hay una duplicada y la cerramos()\n",0);
            ((QWidget *)obj)->close();
            _depura("mostramos la original()\n",0);
            ((QWidget *)m->object())->hide();
            ((QWidget *)m->object())->show();
            _depura("Establecemos la nueva primaria()\n",0);
            m_listBox->setCurrentItem(m);
            return 1;
        }// end if
        i++;
    }// end while

    if( i >= m_listBox->count())  {
        _depura("insercion",0);
        // Solo se insertan ventanas con icono. Sino no se insertan.
        if (((QWidget *)obj)->icon())  {
            _depura("es posible insertar"+nombre,0);
            QPixmap icon = *((QWidget *)obj)->icon();
            QImage imgicon = icon.convertToImage();
            imgicon = imgicon.scaled(32,32);
            icon = imgicon;
            QListWidgetItem1 *m = new QListWidgetItem1(m_listBox,icon);
            m->setObject(obj);
            m->setNombre(nombre);
        }// end if
    }// end if
    _depura("listventanas::END_meteWindow()\n",0);
    return 0;
}


void listventanas::sacaWindow(QObject *obj) {
    _depura("listventanas::INIT_sacaWindow()\n",0);
    /// Buscamos la ventana correspondiente y la borramos.
    uint i=0;
    while (i < m_listBox->count())  {
        QListWidgetItem1 *m = (QListWidgetItem1 *)m_listBox->item(i);
        if (m->object() == obj) {
            m_listBox->takeItem(i);
            break;
        }
        i++;
    }// end while
    _depura("listventanas::END_sacaWindow()\n",0);
}

