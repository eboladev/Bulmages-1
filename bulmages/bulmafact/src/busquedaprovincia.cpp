/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include <QComboBox>

#include "busquedaprovincia.h"
#include "company.h"
#include "funcaux.h"


BusquedaProvincia::BusquedaProvincia(QWidget *parent)
        : QComboBox(parent) {
    _depura("BusquedaProvincia::BusquedaProvincia", 0);
    companyact = NULL;
    m_cursorcombo = NULL;
    connect(this, SIGNAL(activated(int)), this, SLOT(m_activated(int)));
    _depura("END BusquedaProvincia::BusquedaProvincia", 0);
}


BusquedaProvincia::~BusquedaProvincia() {}


void BusquedaProvincia::setProvincia(QString provincia) {
    _depura("BusquedaProvincia::setProvincia", 0);
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = companyact->cargacursor("SELECT * FROM provincia ORDER BY provincia");

    int i = 0;
    int i1 = 0;
    clear();
    addItem("--");

    while (!m_cursorcombo->eof()) {
        i ++;
        if (m_cursorcombo->valor("provincia") == provincia)
            i1 = i;
        addItem(m_cursorcombo->valor("provincia"));
        m_cursorcombo->siguienteregistro();
    } // end while

    setCurrentItem(i1);
    _depura("END BusquedaProvincia::setProvincia", 0);
}

