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


#include  "busquedacanal.h"
#include  "empresa.h"
#include  "funcaux.h"


///
/**
**/
BusquedaCanal::BusquedaCanal(QWidget *parent)
        : QComboBox2(parent) {
    _depura("BusquedaCanal::BusquedaCanal", 0);
    m_cursorcombo = NULL;
    connect(this, SIGNAL(activated(int)), this, SLOT(m_activated(int)));
    _depura("END BusquedaCanal::BusquedaCanal", 0);
}


///
/**
**/
BusquedaCanal::~BusquedaCanal() {
    _depura("BusquedaCanal::~BusquedaCanal", 0);
    _depura("END BusquedaCanal::~BusquedaCanal", 0);
}


///
/**
\param idcanal
**/
void BusquedaCanal::setidcanal(QString idcanal) {
    _depura("BusquedaCanal::setidcanal", 0, idcanal);
    if (m_cursorcombo != NULL) {
        delete m_cursorcombo;
    } // end if
    m_cursorcombo = empresaBase()->cargacursor("SELECT * FROM canal");
    int i = 0;
    int i1 = 0;
    clear();
    addItem("--");
    while (!m_cursorcombo->eof()) {
        i ++;
        if (m_cursorcombo->valor("idcanal") == idcanal) {
            i1 = i;
        } // end if
        addItem(m_cursorcombo->valor("nombre"));
        m_cursorcombo->siguienteregistro();
    } //end while
    setCurrentIndex(i1);
    _depura("END BusquedaCanal::setidcanal", 0, idcanal);
}


///
/**
\param idcanal
**/
void BusquedaCanal::setValorCampo(QString idcanal) {
        _depura("BusquedaCanal::setValorCampo", 0);
	setidcanal(idcanal);
        _depura("END BusquedaCanal::setValorCampo", 0);
}


///
/**
\param index
**/
void BusquedaCanal::m_activated(int index) {
    _depura("BusquedaCanal::m_activated", 0);
    if (index > 0) {
        emit(valueChanged(m_cursorcombo->valor("idcanal", index - 1)));
    } else {
        emit(valueChanged(""));
    } // end if
    _depura("END BusquedaCanal::m_activated", 0);
}


///
/**
\return
**/
QString BusquedaCanal::idcanal() {
    _depura("BusquedaCanal::idcanal", 0);
    int index= currentIndex();
    if (index > 0) {
        return(m_cursorcombo->valor("idcanal", index - 1));
    } else {
        return "";
    } // end if
    _depura("END BusquedaCanal::idcanal", 0);
}


///
/**
\return
**/
QString BusquedaCanal::valorCampo() {
	return idcanal();
}

/// ===================================================================
/// Busqueda Cuenta Delegate para usar con los subforms
/// ===================================================================
/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el SIGNAL activated() con m_activated() para tratarlo. */
/**
\param parent
**/
BusquedaCanalDelegate::BusquedaCanalDelegate(QWidget *parent)
        : QComboBox2(parent) {
    _depura("BusquedaCanalDelegate::BusquedaCanalDelegate", 10);
    setEditable(true);
    connect(this, SIGNAL(editTextChanged(const QString &)), this, SLOT(s_editTextChanged(const QString &)));
    _depura("END BusquedaCanalDelegate::BusquedaCanalDelegate", 0);
}



/// Libera la memoria reservada.
/**
**/
BusquedaCanalDelegate::~BusquedaCanalDelegate() {
    _depura("BusquedaCanalDelegate::~BusquedaCanalDelegate", 10);
    _depura("END BusquedaCanalDelegate::~BusquedaCanalDelegate", 0);
}


/** Permite indicar al Widget cual es la serie de factura seleccionada por defecto.
    Recarga cursor de serie_factura y cuando encuentra un registro cuyo codigoserie_factura coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
/**
\param cod
\return
**/
void BusquedaCanalDelegate::set(const QString &cod) {
    _depura("BusquedaCanalDelegate::s_editTextChanged", 0);
    static bool semaforo = FALSE;
    QString codigo = cod;

    if (semaforo) {
        return;
    } else {
        semaforo = TRUE;
    } // end if

    m_cursorcombo = empresaBase()->cargacursor("SELECT nombre FROM canal WHERE nombre LIKE '" + codigo + "%' ORDER BY nombre LIMIT 25");
    clear();

    ///TODO: La idea es que salga en el desplegable del combobox el listado de cuentas que
    /// coincidan con el texto escrito para poder elegirlo.
    while (!m_cursorcombo->eof()) {
        //addItem(m_cursorcombo->valor("codigo") + ".-" + m_cursorcombo->valor("descripcion"));
        addItem(m_cursorcombo->valor("nombre"));
        m_cursorcombo->siguienteregistro();
    }
    delete m_cursorcombo;
    setEditText(cod);

    semaforo = FALSE;
    _depura("END BusquedaCanalDelegate::s_editTextChanged", 0);
}

