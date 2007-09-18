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


#include "busquedafecha2.h"
#include "funcaux.h"


/// Inicializa el objeto y hace todas las conexiones necesarias.
/**
\param parent
**/
BusquedaFecha2::BusquedaFecha2(QWidget *parent) : QLineEdit(parent) {
    _depura("BusquedaFecha2::BusquedaFecha2", 0);
    installEventFilter(this);
    QObject::connect(this, SIGNAL(editingFinished()), this, SLOT(on_mui_editingFinished()));
    _depura("END BusquedaFecha2::BusquedaFecha2", 0);
}


///No requiere de acciones especiales en el destructor.
/**
**/
BusquedaFecha2::~BusquedaFecha2() {
    _depura("BusquedaFecha2::~BusquedaFecha2", 0);
    _depura("END BusquedaFecha2::~BusquedaFecha2", 0);
}


///
/**
\param val
**/
void BusquedaFecha2::setText(QString val) {
    _depura("BusquedaFecha2::setText", 0);
    QLineEdit::setText(val);
    _depura("END BusquedaFecha2::setText", 0);
}


///
/**
\param val
**/
void BusquedaFecha2::setValorCampo(QString val) {
    _depura("BusquedaFecha2::setValorCampo", 0);
    QLineEdit::setText(val);
    _depura("END BusquedaFecha2::setValorCampo", 0);
}


///
/**
**/
void BusquedaFecha2::selectAll() {
    _depura("BusquedaFecha2::selectAll", 0);
    _depura("END BusquedaFecha2::selectAll", 0);
}


///
/**
\return
**/
QString BusquedaFecha2::text() {
    _depura("BusquedaFecha2::text", 0);
    return QLineEdit::text();
    _depura("END BusquedaFecha2::text", 0);
}


///
/**
\return
**/
QString BusquedaFecha2::valorCampo() {
    _depura("BusquedaFecha2::valorCampo", 0);
    return QLineEdit::text();
    _depura("END BusquedaFecha2::valorCampo", 0);
}


///
/**
**/
void BusquedaFecha2::on_mui_editingFinished() {
    _depura("BusquedaFecha2::on_mui_editingFinished", 0);
    setText(normalizafecha(text()).toString("dd/MM/yyyy"));
    _depura("END BusquedaFecha2::on_mui_editingFinished", 0);
}


///
/**
\param obj
\param event
\return
**/
bool BusquedaFecha2::eventFilter(QObject *obj, QEvent *event) {
    _depura("BusquedaFecha2::eventFilter", 0);
/*
    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        switch (key) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            setText(normalizafecha(text()).toString());
            break;
      } // end switch
    } // end if
*/
/*
    if (event->type() == QEvent::FocusOut) {
        setText("1111");
        return TRUE;
    } // end if
*/
    return QLineEdit::eventFilter(obj, event);
    _depura("END BusquedaFecha2::eventFilter", 0);
}

