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

#include "busquedazonacomercial.h"
#include "company.h"
#include <QComboBox>

BusquedaZonaComercial::BusquedaZonaComercial(QWidget *parent)
: QComboBox(parent) {
	_depura("BusquedaZonaComercial::BusquedaZonaComercial",0);
    companyact=NULL;
    m_cursorcombo=NULL;
    connect(this,SIGNAL(activated(int)), this, SLOT(m_activated(int)));
	_depura("END BusquedaZonaComercial::BusquedaZonaComercial",0);
}


BusquedaZonaComercial::~BusquedaZonaComercial() {
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
}

void BusquedaZonaComercial::setidzonacomercial(QString idzonacomercial) {
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = companyact->cargacursor("SELECT * FROM zonacomercial");
    int i = 0;
    int i1 = 0;
    clear();
    insertItem("--");
    while (!m_cursorcombo->eof()) {
        i ++;
        if (m_cursorcombo->valor("idzonacomercial") == idzonacomercial)
            i1 = i;
        insertItem(m_cursorcombo->valor("nomzonacomercial"));
        m_cursorcombo->siguienteregistro();
    }
    setCurrentItem(i1);
}



