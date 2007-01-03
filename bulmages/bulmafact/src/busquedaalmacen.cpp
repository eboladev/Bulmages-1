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
    m_codigoalmacen = "";
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
    if (m_cursorcombo != NULL) {
        delete m_cursorcombo;
    } // end if
    m_cursorcombo = companyact->cargacursor("SELECT * FROM almacen ORDER BY nomalmacen");
    int i = 0;
    int i1 = 0;
    int i2 = 0;
    clear();
    addItem("--");
    while (!m_cursorcombo->eof()) {
        i ++;
	if (m_cursorcombo->valor("codigoalmacen") == m_codigoalmacen)
	    i2 = i;
        if (m_cursorcombo->valor("idalmacen") == idalmacen)
            i1 = i;
        addItem(m_cursorcombo->valor("nomalmacen"));
        m_cursorcombo->siguienteregistro();
    } //end while
    if (i1 != 0) {
    	setCurrentIndex(i1);
    } else {
	setCurrentIndex(i2);
    } // end if
    _depura("END BusquedaAlmacen::setidalmacen", 0, idalmacen);
}

QString BusquedaAlmacen::idalmacen() {
	_depura("BusquedaAlmacen::idalmacen", 0);
        int index= currentIndex();
        if (index > 0) {
	    _depura("END BusquedaAlmacen::idalmacen", 0);
            return(m_cursorcombo->valor("idalmacen", index - 1));
        } else {
	    _depura("END BusquedaAlmacen::idalmacen", 0);
            return "";
        } // end if
}

void BusquedaAlmacen::setcompany(company *comp) {
	_depura("BusquedaAlmacen::setcompany", 0);
	companyact = comp;
	cursor2 *cur = companyact->cargacursor("SELECT * FROM configuracion WHERE nombre ='AlmacenDefecto'");
	if (!cur->eof()) {
		m_codigoalmacen = cur->valor("valor");
	} // end if
	delete cur;
	_depura("END BusquedaAlmacen::setcompany", 0);
}


void BusquedaAlmacen::m_activated(int index) {
	_depura("BusquedaAlmacen::m_activated", 0);
        if (index > 0) {
            emit(valueChanged(m_cursorcombo->valor("idalmacen", index - 1)));
        } else {
            emit(valueChanged(""));
        } // end if
	_depura("END BusquedaAlmacen::m_activated", 0);
}



