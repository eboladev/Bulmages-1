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

#include "busquedaalmacen.h"
#include "company.h"
#include "funcaux.h"


/** Constructor del Widget conecta las se&ntilde;ales y prepara los componentes.
*/
BusquedaAlmacen::BusquedaAlmacen(QWidget *parent, const char *)
        : QComboBox(parent) {
    _depura("BusquedaAlmacen::BusquedaAlmacen", 0);
    companyact = NULL;
    m_cursorcombo = NULL;
    connect(this, SIGNAL(activated(int)), this, SLOT(m_activated(int)));
    _depura("END BusquedaAlmacen::BusquedaAlmacen", 0);
}

/** El destructor no precisa de operaciones especiales
*/
BusquedaAlmacen::~BusquedaAlmacen() {
    _depura("BusquedaAlmacen::~BusquedaAlmacen", 0);
    _depura("END BusquedaAlmacen::~BusquedaAlmacen", 0);
}


/** Mediante este metodo establecemos el almacen que debe presentar el Widget
    como seleccionado.
    Recarga el cursor y presenta en el combo aquel que se ajusta al id pasado.
*/
void BusquedaAlmacen::setidalmacen(QString idalmacen) {
    _depura("BusquedaAlmacen::setidalmacen", 0, idalmacen);
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = companyact->cargacursor("SELECT * FROM almacen");
    int i = 0;
    int i1 = 0;
    clear();
    addItem("--");
    while (!m_cursorcombo->eof()) {
        i ++;
        if (m_cursorcombo->valor("idalmacen") == idalmacen)
            i1 = i;
        addItem(m_cursorcombo->valor("nomalmacen"));
        m_cursorcombo->siguienteregistro();
    } //end while
    setCurrentIndex(i1);
    _depura("END BusquedaAlmacen::setidalmacen", 0, idalmacen);

}

