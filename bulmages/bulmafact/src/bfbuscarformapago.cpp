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
#include "bfbuscarformapago.h"


/** Inicializa todos los componenetes a NULL para que no se produzcan confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el activar un tipo con m_activated.
*/
/// \TODO: No deberia usarse m_activated como signal ya que confunde con una variable de clase.
/**
\param parent
**/
BfBuscarFormaPago::BfBuscarFormaPago ( QWidget *parent )
        : BlComboBox ( parent )
{
    _depura ( "BfBuscarFormaPago::BfBuscarFormaPago", 0 );

    m_comboRecordSet = NULL;
    m_table = "forma_pago";
    m_null = TRUE;

    _depura ( "END BfBuscarFormaPago::BfBuscarFormaPago", 0 );
}


/** Se libera memoria destruyendo el cursor que almacena los tipos de IVA si es que esta creado.
*/
/**
**/
BfBuscarFormaPago::~BfBuscarFormaPago()
{
    _depura ( "BfBuscarFormaPago::~BfBuscarFormaPago", 0 );
    _depura ( "END BfBuscarFormaPago::~BfBuscarFormaPago", 0 );
}


/** Mediante este metodo podemos indicar al Widget cual es la forma de pago que esta seleccionada
    pasandole como parametro el idform_pago.
    Busca en la tabla forma_pago de la base de datos aquel registro cuyo idforma_pago coincide con el pasado.
    Rellena el comboBox haciendo que el seleccionado sea el visible.
*/
///
/**
\param idforma_pago
\return
**/
void BfBuscarFormaPago::setId ( QString idforma_pago )
{
    _depura ( "BfBuscarFormaPago::setidforma_pago", 0 );

    /// Si lo que se pasa como forma de pago es un valor malo cogemos la forma de pago por defecto.
    if ( idforma_pago.isEmpty() || idforma_pago == "0" ) {
        idforma_pago = g_confpr->valor ( CONF_IDFORMA_PAGO_DEFECTO );
    } // end if

    if ( m_comboRecordSet != NULL ) {
        delete m_comboRecordSet;
    } // end if

    m_comboRecordSet = mainCompany() ->loadQuery ( "SELECT * FROM forma_pago" );

    if ( !m_comboRecordSet ) return;

    int i = 0;
    int i1 = 0;
    clear();
    addItem ( "--" );

    while ( !m_comboRecordSet->eof() ) {
        i ++;

        if ( m_comboRecordSet->valor ( "idforma_pago" ) == idforma_pago ) {
            i1 = i;
        } // end if

        addItem ( m_comboRecordSet->valor ( "descforma_pago" ) );
        m_comboRecordSet->nextRecord();
    } // end while

    setCurrentIndex ( i1 );

    _depura ( "END BfBuscarFormaPago::setidforma_pago", 0 );
}


///
/**
\param idcliente
\return
**/
void BfBuscarFormaPago::setIdCliente ( QString idcliente )
{
    _depura ( "BfBuscarFormaPago::setIdCliente", 0, idcliente );

    /// Si el idcliente no existe salimos.
    if ( idcliente == "" ) {
        _depura ( "END BfBuscarFormaPago::setIdCliente", 0, "idcliente invalido" );
        return;
    } // end if

    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT idforma_pago FROM cliente WHERE idcliente = " + idcliente );

    if ( !cur->eof() ) {
        setId ( cur->valor ( "idforma_pago" ) );
    } // end if

    delete cur;

    _depura ( "END BfBuscarFormaPago::setIdCliente", 0 );
}


///
/**
\param idproveedor
\return
**/
void BfBuscarFormaPago::setIdProveedor ( QString idproveedor )
{
    _depura ( "BfBuscarFormaPago::setIdProveedor", 0, idproveedor );

    /// Si no es un proveedor valido salimos.
    if ( idproveedor == "" )
        return;

    BlDbRecordSet * cur = mainCompany() ->loadQuery ( "SELECT idforma_pago FROM proveedor WHERE idproveedor=" + idproveedor );

    if ( !cur->eof() ) {
        setId ( cur->valor ( "idforma_pago" ) );
    } // end if
    delete cur;
    _depura ( "END BfBuscarFormaPago::setIdProveedor", 0 );
}


///
/**
\param index
**/
void BfBuscarFormaPago::m_activated ( int index )
{
    _depura ( "BfBuscarFormaPago::m_activated", 0 );

    if ( index > 0 ) {
        emit ( valueChanged ( m_comboRecordSet->valor ( "idforma_pago", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    } // end if

    _depura ( "END BfBuscarFormaPago::m_activated", 0 );
}


/** Devuelve el identificador de forma de pago indicado
**/
/**
\return
**/
QString BfBuscarFormaPago::id()
{
    _depura ( "BfBuscarFormaPago::idforma_pago", 0 );
    _depura ( "END BfBuscarFormaPago::idforma_pago", 0 );

    if ( !m_comboRecordSet ) return "0";

    if ( currentIndex() > 0 ) {
        return ( m_comboRecordSet->valor ( "idforma_pago", currentIndex() - 1 ) );
    } // end if

    return "";
}



