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
#include "busquedaseriefactura.h"
#include "company.h"
#include <QComboBox>

BusquedaSerieFactura::BusquedaSerieFactura(QWidget *parent, const char *name)
: QComboBox(parent, name) {
    companyact=NULL;
    m_cursorcombo=NULL;
    
    connect(this,SIGNAL(activated(int)), this, SLOT(m_activated(int)));
}


BusquedaSerieFactura::~BusquedaSerieFactura() {}

void BusquedaSerieFactura::setcodigoserie_factura(QString codigo) {
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = companyact->cargacursor("SELECT * FROM serie_factura");
    int i = 0;
    int i1 = 0;
    clear();
    insertItem("--");
    while (!m_cursorcombo->eof()) {
        i ++;
        if (m_cursorcombo->valor("codigoserie_factura") == codigo)
            i1 = i;
        insertItem(m_cursorcombo->valor("codigoserie_factura")+".-"+m_cursorcombo->valor("descserie_factura"));
        m_cursorcombo->siguienteregistro();
    }
    setCurrentItem(i1);
}
