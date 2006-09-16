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

#include "busquedafecha.h"
#include "calendario.h"
#include "funcaux.h"


BusquedaFecha::BusquedaFecha(QWidget *parent, const char *name)
        : QWidget(parent, name) {
    setupUi(this);
    QObject::connect(m_searchcliente, SIGNAL(clicked(bool)), this, SLOT(s_searchFecha()));
    QObject::connect(m_fecha, SIGNAL(returnPressed()), this, SLOT(s_returnPressed()));
    QObject::connect(m_fecha, SIGNAL(editingFinished()), this, SLOT(s_fechalostFocus()));
}


BusquedaFecha::~BusquedaFecha() {}


void BusquedaFecha::s_searchFecha() {
    Q3PtrList<QDate> a;
    calendario *cal = new calendario(0);
    cal->exec();
    a = cal->dn->selectedDates();
    m_fecha->setText(a.first()->toString("dd/MM/yyyy"));
    delete cal;
    emit(valueChanged(m_fecha->text()));
}


void BusquedaFecha::s_fechatextChanged(const QString &texto) {
    _depura("BusquedaFecha::s_fechatextChanged", 0);
    if (texto == "+")
        s_searchFecha();
    if (texto == "*")
        m_fecha->setText(QDate::currentDate().toString("dd/MM/yyyy"));
    m_fecha->setText(normalizafecha(texto).toString("dd/MM/yyyy"));
    if (texto == "") {
        m_fecha->setText("");
        return;
    } // end if
    emit(valueChanged(m_fecha->text()));
    _depura("END BusquedaFecha::s_fechatextChanged", 0);
}


void BusquedaFecha::s_fechalostFocus() {
    _depura("BusquedaFecha::s_fechalostFocus", 0);
    QString fech = m_fecha->text();
    if (fech != "")
        s_fechatextChanged(fech);
    _depura("END BusquedaFecha::s_fechalostFocus", 0);
}

