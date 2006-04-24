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
 : QWidget(parent, name) {
  setupUi(this);
  QObject::connect(m_searchcliente,SIGNAL(clicked(bool)),this,SLOT(s_searchFecha()));
  QObject::connect(m_fecha,SIGNAL(returnPressed()),this,SLOT(s_returnPressed()));
//  QObject::connect(m_fecha,SIGNAL(textChanged(const QString &)),this,SLOT(s_fechatextChanged(const QString &)));
  QObject::connect(m_fecha,SIGNAL(editingFinished()),this,SLOT(s_fechalostFocus()));
}


BusquedaFecha::~BusquedaFecha() {
}

void BusquedaFecha::s_searchFecha() {
        Q3PtrList<QDate> a;
        calendario *cal = new calendario(0,0);
        cal->exec();
        a = cal->dn->selectedDates();
        m_fecha->setText(a.first()->toString("dd/MM/yyyy"));
        delete cal;
    emit(valueChanged(m_fecha->text()));	
}


void BusquedaFecha::s_fechatextChanged(const QString &texto) {
	_depura("BusquedaFecha::s_fechatextChanged",0);
    if (texto=="+") {
       s_searchFecha();
    }
    if (texto=="*")
        m_fecha->setText(QDate::currentDate().toString("dd/MM/yyyy") );
    m_fecha->setText(normalizafecha(texto).toString("dd/MM/yyyy"));
    if (texto == "") {
	m_fecha->setText("");
	return;
    }// end if
    emit(valueChanged(m_fecha->text()));
	_depura("END BusquedaFecha::s_fechatextChanged",0);
}


void BusquedaFecha::s_fechalostFocus() {
	_depura("BusquedaFecha::s_fechalostFocus",0);
	QString fech = m_fecha->text();
	if (fech != "")
		s_fechatextChanged(fech);
	_depura("END BusquedaFecha::s_fechalostFocus",0);
}// end lostFocus
