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
#include "busquedaperiodo.h"
#include <QComboBox>

BusquedaPeriodo::BusquedaPeriodo(QWidget *parent)
        : QComboBox(parent) {
    _depura("BusquedaPeriodo::BusquedaPeriodo",0);
    m_textos[0] = "Semanal";
    m_textos[1] = "Bisemanal";
    m_textos[2] = "Quinzenal";
    m_textos[3] = "Mensual";
    m_textos[4] = "Trimestral";
    m_textos[5] = "Semestral";

    m_valores[0] = "@ 7 days";
    m_valores[1] = "@ 14 days";
    m_valores[2] = "@ 15 days";
    m_valores[3] = "@ 1 mon";
    m_valores[4] = "@ 3 mons";
    m_valores[5] = "@ 6 mons";

    int i = 0;
    while (i <= 5) {
        insertItem(m_textos[i]);
        i++;
    }// end while


    connect(this,SIGNAL(activated(int)), this, SLOT(s_activated(int)));
    _depura("END BusquedaPeriodo::BusquedaPeriodo",0);
}


BusquedaPeriodo::~BusquedaPeriodo() {}

void BusquedaPeriodo::setperiodo(QString periodo) {
    _depura("BusquedaPeriodo::setperiodo",0);
    int i = 0;
    while (m_valores[i] != periodo && i<=5) {
        i ++;
    }
    if (i <= 5)
        setCurrentItem(i);
    else
        setCurrentItem(0);
    _depura("END BusquedaPeriodo::setperiodo",0);
}
