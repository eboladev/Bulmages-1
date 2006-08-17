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

#include <QComboBox>

#include "busquedacanal.h"
#include "empresa.h"
#include "funcaux.h"


BusquedaCanal::BusquedaCanal(QWidget *parent)
        : QComboBox(parent) {
    _depura("BusquedaCanal::BusquedaCanal", 0);
    companyact = NULL;
    m_cursorcombo = NULL;
    connect(this, SIGNAL(activated(int)), this, SLOT(m_activated(int)));
    _depura("END BusquedaCanal::BusquedaCanal", 0);
}


BusquedaCanal::~BusquedaCanal() {
    _depura("BusquedaCanal::~BusquedaCanal", 0);
}


void BusquedaCanal::setidcanal(QString idcanal) {
    _depura("BusquedaCanal::setidcanal", 0, idcanal);
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = companyact->cargacursor("SELECT * FROM canal");
    int i = 0;
    int i1 = 0;
    clear();
    addItem("--");
    while (!m_cursorcombo->eof()) {
        i ++;
        if (m_cursorcombo->valor("idcanal") == idcanal)
            i1 = i;
        addItem(m_cursorcombo->valor("nombre"));
        m_cursorcombo->siguienteregistro();
    } //end while
    setCurrentIndex(i1);
    _depura("END BusquedaCanal::setidcanal", 0, idcanal);
}

void BusquedaCanal::m_activated(int index) {
        if (index > 0) {
            emit(valueChanged(m_cursorcombo->valor("idcanal", index - 1)));
        } else {
            emit(valueChanged(""));
        }
}

QString BusquedaCanal::idcanal() {
        int index= currentIndex();
        if (index > 0) {
            return(m_cursorcombo->valor("idcanal", index - 1));
        } else {
            return "";
        }
}
