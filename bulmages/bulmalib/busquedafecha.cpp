//
// C++ Implementation: busquedafecha
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "busquedafecha.h"
#include "calendario.h"
#include "funcaux.h"

BusquedaFecha::BusquedaFecha(QWidget *parent, const char *name)
 : BusquedaFechaBase(parent, name) {
}


BusquedaFecha::~BusquedaFecha() {
}

void BusquedaFecha::s_searchFecha() {
        QList<QDate> a;
        calendario *cal = new calendario(0,0);
        cal->exec();
        a = cal->dn->selectedDates();
        m_fecha->setText(a.first()->toString("dd/MM/yyyy"));
        delete cal;
    emit(valueChanged(m_fecha->text()));	
}


void BusquedaFecha::s_fechatextChanged(const QString &texto) {
    if (texto=="+") {
       s_searchFecha();
    }
    if (texto=="*")
        m_fecha->setText(QDate::currentDate().toString("dd/MM/yyyy") );
    m_fecha->setText(normalizafecha(texto).toString("dd/MM/yyyy"));
    emit(valueChanged(m_fecha->text()));
}

void BusquedaFecha::s_fechalostFocus() {
	QString fech = m_fecha->text();
	s_fechatextChanged(fech);
}// end lostFocus
