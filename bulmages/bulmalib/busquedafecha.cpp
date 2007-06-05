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
#include "funcaux.h"
 #include <QCalendarWidget>

void BusquedaFecha::setfecha(QString val) {
    m_fecha->setText(val);
}


void BusquedaFecha::setText(QString val) {
    m_fecha->setText(val);
}


QString BusquedaFecha::fecha() {
    return m_fecha->text();
}


QString BusquedaFecha::text() {
    return m_fecha->text();
}


void BusquedaFecha::s_returnPressed() {
    s_fechalostFocus();
    emit returnPressed();
}


void BusquedaFecha::selectAll() {
    m_fecha->selectAll();
}


void BusquedaFecha::setFocus() {
    m_fecha->setFocus();
}


/**  Inicializa el objeto y hace todas las conexiones necesarias.
*/
BusquedaFecha::BusquedaFecha(QWidget *parent) : QWidget(parent) {
    _depura("BusquedaFecha::BusquedaFecha", 0);
    setupUi(this);
    QObject::connect(m_searchcliente, SIGNAL(clicked(bool)), this, SLOT(s_searchFecha()));
    QObject::connect(m_fecha, SIGNAL(returnPressed()), this, SLOT(s_returnPressed()));
    QObject::connect(m_fecha, SIGNAL(editingFinished()), this, SLOT(s_fechalostFocus()));
    QObject::connect(m_fecha, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
    _depura("BusquedaFecha::BusquedaFecha", 0);
}

/** No requiere de acciones especiales en el destructor.
*/
BusquedaFecha::~BusquedaFecha() {
    _depura("BusquedaFecha::~BusquedaFecha", 0);
    _depura("END BusquedaFecha::~BusquedaFecha", 0);
}


void BusquedaFecha::s_searchFecha() {
    _depura("BusquedaFecha::s_searchFecha", 0);

    QDialog *diag = new QDialog(0);
    diag->setModal(true);
    QCalendarWidget *calend = new QCalendarWidget( diag);
    connect(calend, SIGNAL(activated ( const QDate & )), diag, SLOT(accept()));

    /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
    /// para que sea redimensionable y aparezca el titulo de la ventana.
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(calend);
    layout->setMargin(0);
    layout->setSpacing(0);
    diag->setLayout(layout);
    diag->setWindowTitle("Seleccione Fecha");
    diag->exec();

    m_fecha->setText(calend->selectedDate().toString("dd/MM/yyyy"));

    delete calend;
    delete diag;

    emit(valueChanged(m_fecha->text()));
    _depura("END BusquedaFecha::s_searchFecha", 0);
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

