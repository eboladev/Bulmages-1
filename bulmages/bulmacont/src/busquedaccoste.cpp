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
#include  "busquedaccoste.h"
#include  "funcaux.h"
#include  "empresa.h"


///
/**
\param parent
**/
BusquedaCCoste::BusquedaCCoste(QWidget *parent, const char *)
        : QComboBox2(parent) {
    _depura("BusquedaCCoste::BusquedaCCoste", 0);
    m_cursorcombo = NULL;
    connect(this, SIGNAL(activated(int)), this, SLOT(m_activated(int)));
    _depura("END BusquedaCCoste::BusquedaCCoste", 0);
}


///
/**
**/
BusquedaCCoste::~BusquedaCCoste() {
    _depura("BusquedaCCoste::~BusquedaCCoste", 0);
    _depura("END BusquedaCCoste::~BusquedaCCoste", 0);
}


///
/**
\return
**/
QString BusquedaCCoste::idc_coste() {
    _depura("BusquedaCCoste::idc_coste", 0);
    int index= currentIndex();
    if (index > 0) {
        return(m_cursorcombo->valor("idc_coste", index - 1));
    } else {
        _depura("END BusquedaCCoste::idc_coste", 0);
        return "";
    } // end if
}


///
/**
\return
**/
QString BusquedaCCoste::valorCampo() {
    _depura("BusquedaCCoste::valorCampo", 0);
    _depura("END BusquedaCCoste::valorCampo", 0);
    return idc_coste();
}


///
/**
\param index
**/
void BusquedaCCoste::m_activated(int index) {
    _depura("BusquedaCCoste::m_activated", 0);
    if (index > 0) {
        emit(valueChanged(m_cursorcombo->valor("idc_coste", index - 1)));
    } else {
        emit(valueChanged(""));
        _depura("END BusquedaCCoste::m_activated", 0);
    } // end if
}


///
/**
\param idc_coste
**/
void BusquedaCCoste::setidc_coste(QString idc_coste) {
    _depura("BusquedaCCoste::setidc_coste", 0, idc_coste);
    if (m_cursorcombo != NULL) {
        delete m_cursorcombo;
    } // end if
    m_cursorcombo = empresaBase()->cargacursor("SELECT * FROM c_coste");
    int i = 0;
    int i1 = 0;
    clear();
    addItem("--");
    while (!m_cursorcombo->eof()) {
        i ++;
        if (m_cursorcombo->valor("idc_coste") == idc_coste) {
            i1 = i;
        } // end if
        addItem(m_cursorcombo->valor("nombre"));
        m_cursorcombo->siguienteregistro();
    } //end while
    setCurrentIndex(i1);
    _depura("END BusquedaCCoste::setidc_coste", 0, idc_coste);
}


///
/**
\param idc_coste
**/
void BusquedaCCoste::setValorCampo(QString idc_coste) {
    _depura("BusquedaCCoste::setValorCampo", 0);
    setidc_coste(idc_coste);
    _depura("END BusquedaCCoste::setValorCampo", 0);
}

/// ===================================================================
/// Busqueda Cuenta Delegate para usar con los subforms
/// ===================================================================
/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el SIGNAL activated() con m_activated() para tratarlo.
*/
/**
\param parent
**/
BusquedaCCosteDelegate::BusquedaCCosteDelegate(QWidget *parent)
        : QComboBox2(parent) {
    _depura("BusquedaCCosteDelegate::BusquedaCCosteDelegate", 0);
    setEditable(false);
    _depura("END BusquedaCCosteDelegate::BusquedaCCosteDelegate", 0);
}


/// Libera la memoria reservada.
/**
**/
BusquedaCCosteDelegate::~BusquedaCCosteDelegate() {
    _depura("BusquedaCCosteDelegate::~BusquedaCCosteDelegate", 10);
    _depura("END BusquedaCCosteDelegate::~BusquedaCCosteDelegate", 0);
}


/** Permite indicar al Widget cual es la serie de factura seleccionada por defecto.
    Recarga cursor de serie_factura y cuando encuentra un registro cuyo codigoserie_factura coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
/**
\param cod
\return
**/
void BusquedaCCosteDelegate::set(const QString &cod) {
    _depura("BusquedaCCosteDelegate::set", 0);

    int index = 0;

    m_cursorcombo = empresaBase()->cargacursor("SELECT nombre FROM c_coste ORDER BY nombre ");
    clear();

    addItem("--");
    while (!m_cursorcombo->eof()) {
        addItem(m_cursorcombo->valor("nombre"));
        if (m_cursorcombo->valor("nombre") == cod)
            index = m_cursorcombo->regactual() + 1;
        m_cursorcombo->siguienteregistro();
    } // end while
    delete m_cursorcombo;
    setEditText(cod);
    setCurrentIndex(index);
    _depura("END BusquedaCCosteDelegate::set", 0);
}



