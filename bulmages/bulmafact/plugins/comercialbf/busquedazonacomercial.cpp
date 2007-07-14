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

#include "busquedazonacomercial.h"
#include "company.h"


BusquedaZonaComercial::BusquedaZonaComercial(QWidget *parent)
        : QComboBox(parent), PEmpresaBase() {
    _depura("BusquedaZonaComercial::BusquedaZonaComercial", 0);
    m_cursorcombo = NULL;
    connect(this, SIGNAL(activated(int)), this, SLOT(m_activated(int)));
    _depura("END BusquedaZonaComercial::BusquedaZonaComercial", 0);
}


BusquedaZonaComercial::~BusquedaZonaComercial() {
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
}


void BusquedaZonaComercial::setidzonacomercial(QString idzonacomercial) {
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = empresaBase()->cargacursor("SELECT * FROM zonacomercial");
    int i = 0;
    int i1 = 0;
    clear();
    addItem("--");
    while (!m_cursorcombo->eof()) {
        i++;
        if (m_cursorcombo->valor("idzonacomercial") == idzonacomercial)
            i1 = i;
        addItem(m_cursorcombo->valor("nomzonacomercial"));
        m_cursorcombo->siguienteregistro();
    } // end while
    setCurrentIndex(i1);
}

