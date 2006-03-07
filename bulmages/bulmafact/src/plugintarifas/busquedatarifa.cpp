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
#include "busquedatarifa.h"
#include "company.h"
#include <Q3ComboBox>

BusquedaTarifa::BusquedaTarifa(QWidget *parent, const char *name)
: Q3ComboBox(parent, name) {
    companyact=NULL;
    m_cursorcombo=NULL;
    
    connect(this,SIGNAL(activated(int)), this, SLOT(m_activated(int)));
}


BusquedaTarifa::~BusquedaTarifa() {}

void BusquedaTarifa::setidtarifa(QString idtarifa) {
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = companyact->cargacursor("SELECT * FROM tarifa");
    int i = 0;
    int i1 = 0;
    clear();
    insertItem("--");
    while (!m_cursorcombo->eof()) {
        i ++;
        if (m_cursorcombo->valor("idtarifa") == idtarifa)
            i1 = i;
        insertItem(m_cursorcombo->valor("nomtarifa"));
        m_cursorcombo->siguienteregistro();
    }
    setCurrentItem(i1);
}
