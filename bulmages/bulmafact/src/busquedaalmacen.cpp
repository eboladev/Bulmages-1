/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include "busquedaalmacen.h"
#include "company.h"
#include "funcaux.h"


BusquedaAlmacen::BusquedaAlmacen(QWidget *parent, const char *) : QComboBox(parent) {
    _depura("BusquedaAlmacen::BusquedaAlmacen",0);
    companyact = NULL;
    m_cursorcombo = NULL;
    connect(this, SIGNAL(activated(int)), this, SLOT(m_activated(int)));
    _depura("END BusquedaAlmacen::BusquedaAlmacen",0);
}


BusquedaAlmacen::~BusquedaAlmacen() {
	_depura("BusquedaAlmacen::~BusquedaAlmacen",0);
}


void BusquedaAlmacen::setidalmacen(QString idalmacen) {
    _depura("BusquedaAlmacen::setidalmacen", 0, idalmacen);
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
    } //end while
    setCurrentItem(i1);
    _depura("END BusquedaAlmacen::setidalmacen", 0, idalmacen);

}

