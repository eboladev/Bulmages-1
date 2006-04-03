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
#include "busquedaformapago.h"
#include "company.h"
#include <QComboBox>

BusquedaFormaPago::BusquedaFormaPago(QWidget *parent)
: QComboBox(parent) {
    companyact=NULL;
    m_cursorcombo=NULL;
    
    connect(this,SIGNAL(activated(int)), this, SLOT(m_activated(int)));
}


BusquedaFormaPago::~BusquedaFormaPago() {
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
}

void BusquedaFormaPago::setidforma_pago(QString idforma_pago) {
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = companyact->cargacursor("SELECT * FROM forma_pago");
    int i = 0;
    int i1 = 0;
    clear();
    insertItem("--");
    while (!m_cursorcombo->eof()) {
        i ++;
        if (m_cursorcombo->valor("idforma_pago") == idforma_pago)
            i1 = i;
        insertItem(m_cursorcombo->valor("descforma_pago"));
        m_cursorcombo->siguienteregistro();
    }
    setCurrentItem(i1);
}
