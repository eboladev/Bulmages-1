//
// C++ Implementation: busquedacliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "busquedaestadoincidencia.h"
#include <QComboBox>

BusquedaEstadoIncidencia::BusquedaEstadoIncidencia(QWidget *parent)
        : QComboBox(parent) {
    _depura("BusquedaEstadoIncidencia::BusquedaEstadoIncidencia",0);
    m_textos[0] = "Pendiente";
    m_textos[1] = "Terminada";
    m_textos[2] = "No Quiere Nada";
    m_textos[3] = "No Paga";
    m_textos[4] = "Cambio Datos";
    m_textos[5] = "Revisar";

    m_valores[0] = "1";
    m_valores[1] = "2";
    m_valores[2] = "3";
    m_valores[3] = "4";
    m_valores[4] = "5";
    m_valores[5] = "6";

    int i = 0;
    while (i <= 5) {
        insertItem(m_textos[i]);
        i++;
    }// end while


    connect(this,SIGNAL(activated(int)), this, SLOT(s_activated(int)));
    _depura("END BusquedaEstadoIncidencia::BusquedaEstadoIncidencia",0);
}


BusquedaEstadoIncidencia::~BusquedaEstadoIncidencia() {}

void BusquedaEstadoIncidencia::setestado(QString estado) {
    _depura("BusquedaEstadoIncidencia::setestado",0);
    int i = 0;
    while (m_valores[i] != estado && i<=5) {
        i ++;
    }
    if (i <= 5)
        setCurrentItem(i);
    else
        setCurrentItem(0);
    _depura("END BusquedaEstadoIncidencia::setestado",0);
}


