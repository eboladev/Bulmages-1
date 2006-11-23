/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QObject>
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QVBoxLayout>

#include <map>

#include "listventanas.h"
#include "funcaux.h"


using namespace std;


listventanas::listventanas(QWidget *a) : QDockWidget(a) {
    _depura("listventanas::INIT_listventanas()\n", 0);
    m_listBox = new QListWidget1(this);
    m_listBox->setIconSize(QSize(32, 32));
    m_listBox->setContentsMargins(0, 0, 0, 0);
    m_listBox->setSpacing(0);
    m_listBox->setWindowTitle(tr("Indexador"));
    setAccessibleName(tr("Indexador"));
    setAccessibleDescription(tr("Indexador"));
    setWindowTitle(tr("Indexador"));
    setFocusPolicy(Qt::StrongFocus);
    setWidget(m_listBox);
    connect(m_listBox, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(dclicked()));
    connect(m_listBox, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(clicked()));
    _depura("listventanas::END_listventanas()\n", 0);
}


void listventanas::dclicked() {
    _depura("listventanas::dclicked()\n", 0);
    QWidget *widget = (QWidget *)((QListWidgetItem1 *) m_listBox->currentItem())->object();
    if (widget != NULL) {
        if (widget->isMaximized() == TRUE)
            widget->showNormal();
        else
            widget->showMaximized();
    } // end if
    _depura("END listventanas::dclicked()\n", 0);
}


void listventanas::clicked() {
    _depura("listventanas::clicked()\n", 0);
    QWidget *widget = (QWidget *)((QListWidgetItem1 *) m_listBox->currentItem())->object();
    if (widget != NULL) {
        widget->show();
        widget->parentWidget()->raise();
        m_pWorkspace->setActiveWindow(widget);
    } // end if
    _depura("END listventanas::clicked()\n", 0);
}


listventanas::~listventanas() {
    _depura("listventanas::~listventanas\n", 0);
    delete m_listBox;
    _depura("END listventanas::~listventanas\n", 0);
}


void listventanas::vaciar() {
    _depura("listventanas::vaciar", 0);
    /// Buscamos la ventana correspondiente y la borramos.
    int i = 0;
    while (i < m_listBox->count()) {
        QListWidgetItem1 *m = (QListWidgetItem1 *)m_listBox->item(i);
        _depura("Vamos a destruir la ventana", 0, m->nombre());
        if (((QWidget *)m->object())->testAttribute(Qt::WA_DeleteOnClose)) {
            delete m->object();
        } else {
            i++;
        } // end if
    } // end while
    _depura("END listventanas::vaciar", 0);
}


/// This function provides the ability of push a window in the dock window
/// nombre This QString is the name of the window that was shown in the listbox
/// obj This QObject * contains the pointer of the window for furtner reference.
int listventanas::meteWindow(QString nombre, QObject *obj, bool compdup) {
    _depura("listventanas::meteWindow()", 0);
    try {
        int i = 0;
        while (i < m_listBox->count()) {
            QListWidgetItem1 *m = (QListWidgetItem1 *)m_listBox->item(i);
            /// Si la ventana ya esta en la lista.
            if (m->object() == obj) {
                _depura("listventanas::La ventana ya existe", 0, nombre);
                m->setNombre(nombre);
                //m_listBox->setCurrentItem(m);
                return 0;
            } // end if

            /// Comprobamos ventanas duplicadas.
            if (m->nombre() == nombre && compdup) {
                _depura("listventanas::hay una duplicada y la cerramos()\n", 0);
                _depura("listventanas::mostramos la original()\n", 0);
                ((QWidget *)m->object())->hide();
                ((QWidget *)m->object())->show();
                _depura("listventanas::Establecemos la nueva primaria()\n", 0);
                //m_listBox->setCurrentItem(m);
                throw -1;
            } // end if
            i++;
        } // end while
        if (i >= m_listBox->count()) {
            _depura("insercion", 0);
            QPixmap icon = ((QWidget *)obj)->windowIcon().pixmap(32, 32);
            QListWidgetItem1 *m = new QListWidgetItem1(m_listBox, icon);
            m->setObject(obj);
            m->setNombre(nombre);
            //m_listBox->setCurrentItem(m);
        } // end if
    } catch (...) {
        mensajeInfo(tr("listventanas::meteWindow ventana duplicada"));
        throw -1;
    } // end try
    _depura("END listventanas::meteWindow()\n", 0);
    return 0;
}


/// Sirve para seleccionar una ventana listada en el Indexador.
int listventanas::seleccionaWindow(QString nombre, QObject *obj) {
    _depura("listventanas::seleccionaWindow()", 0);
    try {
        int i = 0;
        while (i < m_listBox->count()) {
            QListWidgetItem1 *m = (QListWidgetItem1 *)m_listBox->item(i);
            /// Encuentra la ventana en la lista.
            if (m->object() == obj) {
                _depura("listventanas::Se ha encontrado la ventana", 0, nombre);
                m_listBox->setCurrentItem(m);
                return 0;
            } // end if
            i++;
        } // end while
    } catch (...) {
        throw -1;
    } // end try
    _depura("END listventanas::seleccionaWindow()\n", 0);
    return 0;
}


/// Deselecciona todas las entradas del Indexador.
int listventanas::deSeleccionaWindow() {
    _depura("listventanas::deSeleccionaWindow()", 0);
    try {
        m_listBox->clearSelection();
        return 0;
    } catch (...) {
        throw -1;
    } // end try
    _depura("END listventanas::deSeleccionaWindow()\n", 0);
    return 0;
}


void listventanas::sacaWindow(QObject *obj) {
    _depura("listventanas::sacaWindow()\n", 0);
    /// Buscamos la entrada correspondiente dentro del Indexador y la borramos.
    int i = 0;
    while (i < m_listBox->count()) {
        QListWidgetItem1 *m = (QListWidgetItem1 *)m_listBox->item(i);
        if (m->object() == obj) {
            _depura("listventanas::sacaWindow ventana encontrada y vamos a sacarla", 0, m->nombre());
            m_listBox->takeItem(i);
            delete m;
            break;
        } // end if
        i++;
    } // end while
    _depura("END listventanas::sacaWindow()\n", 0);
}


void listventanas::cambiaVisible() {
    if (this->isVisible() == TRUE) {
        this->hide();
    } else {
        this->show();
    } // end if
}

