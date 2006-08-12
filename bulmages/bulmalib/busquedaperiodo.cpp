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

#include "busquedaperiodo.h"
#include <QComboBox>


BusquedaPeriodo::BusquedaPeriodo(QWidget *parent)
        : QComboBox(parent) {
    _depura("BusquedaPeriodo::BusquedaPeriodo", 0);
    m_textos[0] = "Semanal";
    m_textos[1] = "Bisemanal";
    m_textos[2] = "Quinzenal";
    m_textos[3] = "Mensual";
    m_textos[4] = "Trimestral";
    m_textos[5] = "Semestral";

    m_valores[0] = "@ 7 days";
    m_valores[1] = "@ 14 days";
    m_valores[2] = "@ 15 days";
    m_valores[3] = "@ 1 mon";
    m_valores[4] = "@ 3 mons";
    m_valores[5] = "@ 6 mons";

    int i = 0;
    while (i <= 5) {
        insertItem(m_textos[i]);
        i++;
    } // end while

    connect(this,SIGNAL(activated(int)), this, SLOT(s_activated(int)));
    _depura("END BusquedaPeriodo::BusquedaPeriodo", 0);
}


BusquedaPeriodo::~BusquedaPeriodo() {}


void BusquedaPeriodo::setperiodo(QString periodo) {
    _depura("BusquedaPeriodo::setperiodo", 0);
    int i = 0;
    while (m_valores[i] != periodo && i <= 5) {
        i ++;
    } // end while
    if (i <= 5)
        setCurrentItem(i);
    else
        setCurrentItem(0);
    _depura("END BusquedaPeriodo::setperiodo", 0);
}

