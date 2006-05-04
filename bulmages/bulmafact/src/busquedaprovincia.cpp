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

#include <QComboBox>

#include "busquedaprovincia.h"
#include "company.h"
#include "funcaux.h"

BusquedaProvincia::BusquedaProvincia(QWidget *parent, const char *name)
        : QComboBox(parent, name) {
    _depura("BusquedaProvincia::BusquedaProvincia",0);
    companyact=NULL;
    m_cursorcombo=NULL;
    connect(this,SIGNAL(activated(int)), this, SLOT(m_activated(int)));
    _depura("END BusquedaProvincia::BusquedaProvincia",0);
}


BusquedaProvincia::~BusquedaProvincia() {}

void BusquedaProvincia::setProvincia(QString provincia) {
    _depura("BusquedaProvincia::setProvincia",0);
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = companyact->cargacursor("SELECT * FROM provincia");

    int i = 0;
    int i1 = 0;
    clear();
    insertItem("--");
    while (!m_cursorcombo->eof()) {
        i ++;
        if (m_cursorcombo->valor("provincia") == provincia)
            i1 = i;
        insertItem(m_cursorcombo->valor("provincia"));
        m_cursorcombo->siguienteregistro();
    }// end while
    setCurrentItem(i1);
    _depura("END BusquedaProvincia::setProvincia",0);

}
