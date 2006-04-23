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

    m_textos[0] = "Semanal";
    m_textos[1] = "Bisemanal";
    m_textos[2] = "Quinzenal";
    m_textos[3] = "Mensual";
    m_textos[4] = "Trimestral";
    m_textos[5] = "Semestral";

    m_valores[0] = "--";
    m_valores[1] = "--";
    m_valores[2] = "--";
    m_valores[3] = "--";
    m_valores[4] = "--";
    m_valores[5] = "--";

    int i = 0;
    while (i <= 5)
        insertItem(m_textos[i]);


    connect(this,SIGNAL(activated(int)), this, SLOT(s_activated(int)));
}


BusquedaPeriodo::~BusquedaPeriodo() {}

void BusquedaPeriodo::setperiodo(QString periodo) {
    int i = 0;
    clear();
    while (m_valores[i] != periodo && i<=5)
        i ++;
    if (i <= 5)
        setCurrentItem(i);
    else
        setCurrentItem(0);
}
