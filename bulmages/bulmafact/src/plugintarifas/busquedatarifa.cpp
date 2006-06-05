/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <Q3ComboBox>

#include "busquedatarifa.h"
#include "company.h"
#include "funcaux.h"


BusquedaTarifa::BusquedaTarifa(QWidget *parent, const char *name)
        : Q3ComboBox(parent, name) {
    companyact = NULL;
    m_cursorcombo = NULL;
    connect(this, SIGNAL(activated(int)), this, SLOT(m_activated(int)));
}


BusquedaTarifa::~BusquedaTarifa() {}


void BusquedaTarifa::setidtarifa(QString idtarifa) {
    _depura("BusquedaTarifa::setidtarifa", 0);
    mdb_idtarifa = idtarifa;
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

