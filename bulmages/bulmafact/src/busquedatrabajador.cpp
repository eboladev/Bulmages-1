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
#include "busquedatrabajador.h"
#include "company.h"
#include <QComboBox>

BusquedaTrabajador::BusquedaTrabajador(QWidget *parent)
: QComboBox(parent) {
    companyact=NULL;
    m_cursorcombo=NULL;
    
    connect(this,SIGNAL(activated(int)), this, SLOT(m_activated(int)));
}


BusquedaTrabajador::~BusquedaTrabajador() {
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
}

void BusquedaTrabajador::setidtrabajador(QString idtrabajador) {
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = companyact->cargacursor("SELECT * FROM trabajador");
    int i = 0;
    int i1 = 0;
    clear();
    insertItem("--");
    while (!m_cursorcombo->eof()) {
        i ++;
        if (m_cursorcombo->valor("idtrabajador") == idtrabajador)
            i1 = i;
        insertItem(m_cursorcombo->valor("apellidostrabajador")+", "+m_cursorcombo->valor("nomtrabajador"));
        m_cursorcombo->siguienteregistro();
    }
    setCurrentItem(i1);
}
