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

#include "busquedaformapago.h"


/** Inicializa todos los componenetes a NULL para que no se produzcan confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el activar un tipo con m_activated.
*/
/// \TODO: No deberia usarse m_activated como signal ya que confunde con una variable de clase.
BusquedaFormaPago::BusquedaFormaPago(QWidget *parent)
        : QComboBox(parent), PEmpresaBase() {
    _depura("BusquedaFormaPago::BusquedaFormaPago", 0);
    m_cursorcombo = NULL;
    connect(this, SIGNAL(activated(int)), this, SLOT(m_activated(int)));
    _depura("END BusquedaFormaPago::BusquedaFormaPago", 0);
}


/** Se libera memoria destruyendo el cursor que almacena los tipos de IVA si es que esta creado.
*/
BusquedaFormaPago::~BusquedaFormaPago() {
    _depura("BusquedaFormaPago::~BusquedaFormaPago", 0);
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    _depura("END BusquedaFormaPago::~BusquedaFormaPago", 0);
}


/** Mediante este metodo podemos indicar al Widget cual es la forma de pago que esta seleccionada
    pasandole como parametro el idform_pago.
    Busca en la tabla forma_pago de la base de datos aquel registro cuyo idforma_pago coincide con el pasado.
    Rellena el comboBox haciendo que el seleccionado sea el visible.
*/
void BusquedaFormaPago::setidforma_pago(QString idforma_pago) {
    _depura("BusquedaFormaPago::setidforma_pago", 0);
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = empresaBase()->cargacursor("SELECT * FROM forma_pago");
    if (!m_cursorcombo) return;
    int i = 0;
    int i1 = 0;
    clear();
    addItem("--");
    while (!m_cursorcombo->eof()) {
        i ++;
        if (m_cursorcombo->valor("idforma_pago") == idforma_pago)
            i1 = i;
        addItem(m_cursorcombo->valor("descforma_pago"));
        m_cursorcombo->siguienteregistro();
    } // end while
    setCurrentIndex(i1);
    _depura("END BusquedaFormaPago::setidforma_pago", 0);
}


void BusquedaFormaPago::setIdCliente(QString idcliente) {
    _depura("BusquedaFormaPago::setIdCliente", 0, idcliente);


    /// Si el idcliente no existe salimos.
    if (idcliente == "") {
        _depura("END BusquedaFormaPago::setIdCliente", 0, "idcliente invalido");
        return;
    } // end if

    cursor2 *cur = empresaBase()->cargacursor("SELECT idforma_pago FROM cliente WHERE idcliente = " + idcliente);

    if (!cur->eof()) {
        setidforma_pago(cur->valor("idforma_pago"));
    } // end if
    delete cur;
    _depura("END BusquedaFormaPago::setIdCliente", 0);
}


void BusquedaFormaPago::setIdProveedor(QString idproveedor) {
    _depura("BusquedaFormaPago::setIdProveedor", 0, idproveedor);

    /// Si no es un proveedor valido salimos.
    if (idproveedor == "")
	return;

    cursor2 * cur = empresaBase()->cargacursor("SELECT idforma_pago FROM proveedor WHERE idproveedor="+idproveedor);

    if (!cur->eof()) {
        setidforma_pago(cur->valor("idforma_pago"));
    } // end if
    delete cur;
    _depura("END BusquedaFormaPago::setIdProveedor", 0);
}


void BusquedaFormaPago::m_activated(int index) {
    _depura("BusquedaFormaPago::m_activated", 0);
    if (index > 0) {
        emit(valueChanged(m_cursorcombo->valor("idforma_pago", index - 1)));
    } else {
        emit(valueChanged(""));
    } // end if
    _depura("END BusquedaFormaPago::m_activated", 0);
}


/** Devuelve el identificador de forma de pago indicado
**/
QString BusquedaFormaPago::idforma_pago() {
    _depura("BusquedaFormaPago::idforma_pago", 0);
    _depura("END BusquedaFormaPago::idforma_pago", 0);
    if (!m_cursorcombo) return "0";
    return (m_cursorcombo->valor("idforma_pago", currentIndex() - 1));
}

