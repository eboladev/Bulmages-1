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
#include "busquedaalmacen.h"
#include "company.h"
#include <qcombobox.h>

BusquedaAlmacen::BusquedaAlmacen(QWidget *parent, const char *name)
: QComboBox(parent, name) {
    companyact=NULL;
    m_cursorcombo=NULL;
    
    connect(this,SIGNAL(activated(int)), this, SLOT(m_activated(int)));
}


BusquedaAlmacen::~BusquedaAlmacen() {}

void BusquedaAlmacen::setidalmacen(QString idalmacen) {
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = companyact->cargacursor("SELECT * FROM almacen");
    int i = 0;
    int i1 = 0;
    clear();
    insertItem("--");
    while (!m_cursorcombo->eof()) {
        i ++;
        if (m_cursorcombo->valor("idalmacen") == idalmacen)
            i1 = i;
        insertItem(m_cursorcombo->valor("nomalmacen"));
        m_cursorcombo->siguienteregistro();
    }
    setCurrentItem(i1);
}
