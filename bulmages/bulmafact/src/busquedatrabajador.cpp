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

#include "blcombobox.h"

#include "busquedatrabajador.h"
#include "company.h"


/** Inicializa todos los componentes a NULL para que no haya confusiones sobre
    si un elemento ha sido inicializado o no.
    Hace la conexion del SIGNAL activated con m_activated para tratar el evento.
*/
/**
\param parent
**/
BusquedaTrabajador::BusquedaTrabajador ( QWidget *parent )
        : BlComboBox ( parent )
{
    _depura ( "BusquedaTrabajador::BusquedaTrabajador", 0 );
    m_cursorcombo = NULL;
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    _depura ( "END BusquedaTrabajador::BusquedaTrabajador", 0 );
}


/** Libera la memoria dinamica que se estaba utilizando.
*/
/**
**/
BusquedaTrabajador::~BusquedaTrabajador()
{
    _depura ( "BusquedaTrabajador::~BusquedaTrabajador", 0 );
    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    _depura ( "END BusquedaTrabajador::~BusquedaTrabajador", 0 );
}


/** Con este metodo se puede indicar al Widget cual es el trabajador seleccionado.
    Recarga el cursor de trabajadores y cuando encuentra uno con el mismo identificador
    que el que se ha pasado por parametro lo establece como elemento seleccionado.
*/
/**
\param idtrabajador
\return
**/
void BusquedaTrabajador::setidtrabajador ( QString idtrabajador )
{
    _depura ( "BusquedaTrabajador::setidtrabajador", 0 );

	/// Si lo que se pasa como forma de pago es un valor malo cogemos la forma de pago por defecto.
	if (idtrabajador.isEmpty() || idtrabajador == "0")
		idtrabajador = confpr->valor(CONF_IDTRABAJADOR_DEFECTO);

    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    m_cursorcombo = empresaBase() ->cargacursor ( "SELECT * FROM trabajador" );
    if ( !m_cursorcombo ) return;
    int i = 0;
    int i1 = 0;
    clear();
    addItem ( "--" );
    while ( !m_cursorcombo->eof() ) {
        i ++;
        if ( m_cursorcombo->valor ( "idtrabajador" ) == idtrabajador )
            i1 = i;
        addItem ( m_cursorcombo->valor ( "apellidostrabajador" ) + ", " + m_cursorcombo->valor ( "nomtrabajador" ) );
        m_cursorcombo->siguienteregistro();
    } // end while
    setCurrentIndex ( i1 );
    _depura ( "END BusquedaTrabajador::setidtrabajador", 0 );
}


///
/**
\param idtrabajador
**/
void BusquedaTrabajador::setValorCampo ( QString idtrabajador )
{
    _depura ( "BusquedaTrabajador::setValorCampo", 0 );
    setidtrabajador ( idtrabajador );
    _depura ( "END BusquedaTrabajador::setValorCampo", 0 );
}


/** Devuelve el identificador del trabajador seleccionado
**/
/**
\return
**/
QString BusquedaTrabajador::idtrabajador()
{
    _depura ( "BusquedaTrabajador::idtrabajador", 0 );
    _depura ( "END BusquedaTrabajador::idtrabajador", 0 );
    if ( !m_cursorcombo ) return "0";
    return m_cursorcombo->valor ( "idtrabajador", currentIndex() - 1 );
}


///
/**
\return
**/
QString BusquedaTrabajador::valorCampo()
{
    _depura ( "BusquedaTrabajador::valorCampo", 0 );
    _depura ( "END BusquedaTrabajador::valorCampo", 0 );
    return idtrabajador();
}


/** SLOT que responde a la activacion de un elemento en el QComboBox y que hace que se emita el SIGNAL valueChanged
**/
/**
\param index
**/
void BusquedaTrabajador::m_activated ( int index )
{
    _depura ( "BusquedaTrabajador::m_activated", 0 );
    if ( index > 0 ) {
        emit ( valueChanged ( m_cursorcombo->valor ( "idtrabajador", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    }
    _depura ( "END BusquedaTrabajador::m_activated", 0 );
}

/// ========================= ITEM DELEGATE ===============================0

/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el SIGNAL activated() con m_activated() para tratarlo.
*/
/**
\param parent
**/
BusquedaTrabajadorDelegate::BusquedaTrabajadorDelegate ( QWidget *parent )
        : BlComboBox ( parent )
{
    _depura ( "BusquedaTrabajadorDelegate::BusquedaTrabajadorDelegate", 0 );
    m_cursorcombo = NULL;
    setSizeAdjustPolicy ( QComboBox::AdjustToContents );
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    _depura ( "END BusquedaTrabajadorDelegate::BusquedaTrabajadorDelegate", 0 );
}


/** Libera la memoria reservada.
*/
/**
**/
BusquedaTrabajadorDelegate::~BusquedaTrabajadorDelegate()
{
    _depura ( "BusquedaTrabajadorDelegate::~BusquedaTrabajadorDelegate", 0 );
    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    _depura ( "END BusquedaTrabajadorDelegate::~BusquedaTrabajadorDelegate", 0 );
}


/** Permite indicar al Widget cual es la serie de factura seleccionada por defecto.
    Recarga cursor de serie_factura y cuando encuentra un registro cuyo codigoserie_factura coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
/**
\param cod
**/
void BusquedaTrabajadorDelegate::set ( const QString &cod )
{
    _depura ( "BusquedaTrabajadorDelegate::set", 0 );
    int index = 0;
    QString codigo = cod;

    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;

    m_cursorcombo = empresaBase() ->cargacursor ( "SELECT nomtrabajador, apellidostrabajador FROM trabajador " );
    clear();
    while ( !m_cursorcombo->eof() ) {
        addItem ( m_cursorcombo->valor ( "apellidostrabajador" ) + ", " + m_cursorcombo->valor ( "nomtrabajador" ) );
        m_cursorcombo->siguienteregistro();
        if ( m_cursorcombo->valor ( "apellidostrabajador" ) + ", " + m_cursorcombo->valor ( "nomtrabajador" ) == cod )
            index = m_cursorcombo->regactual();
    }// end while
    setEditText ( cod );
    setCurrentIndex ( index );

    _depura ( "END BusquedaTrabajadorDelegate::set", 0 );
}



