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

#include "busquedaseriefactura.h"
#include "company.h"
#include "funcaux.h"



/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no. 
    Conecta el SIGNAL activated() con m_activated() para tratarlo.
*/
BusquedaSerieFactura::BusquedaSerieFactura(QWidget *parent)
        : QComboBox(parent) {
    _depura("BusquedaSerieFactura::BusquedaSerieFactura", 0);
    companyact = NULL;
    m_cursorcombo = NULL;
    connect(this, SIGNAL(activated(int)), this, SLOT(m_activated(int)));
    _depura("END BusquedaSerieFactura::BusquedaSerieFactura", 0);
}


/** Libera la memoria reservada.
*/
BusquedaSerieFactura::~BusquedaSerieFactura() {
    _depura("BusquedaSerieFactura::~BusquedaSerieFactura", 0);
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    _depura("END BusquedaSerieFactura::~BusquedaSerieFactura", 0);
}


/** Permite indicar al Widget cual es la serie de factura seleccionada por defecto.
    Recarga cursor de serie_factura y cuando encuentra un registro cuyo codigoserie_factura coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
void BusquedaSerieFactura::setcodigoserie_factura(QString codigo) {
    _depura("BusquedaSerieFactura::setcodigoserie_factura", 0);
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = companyact->cargacursor("SELECT * FROM serie_factura");
    int i = 0;
    int i1 = 0;
    clear();
    addItem("--");
    while (!m_cursorcombo->eof()) {
        i ++;
        if (m_cursorcombo->valor("codigoserie_factura") == codigo)
            i1 = i;
        addItem(m_cursorcombo->valor("codigoserie_factura") + ".-" + m_cursorcombo->valor("descserie_factura"));
        m_cursorcombo->siguienteregistro();
    }
    setCurrentIndex(i1);
    _depura("END BusquedaSerieFactura::setcodigoserie_factura", 0);
}

