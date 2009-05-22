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

#include "bfbuscartipoiva.h"
#include "bfcompany.h"
#include "blfunctions.h"


/** Inicializa todos los componentes a NULL para que no haya confusiones sobre
    si un elemento ha sido inicializado o no.
    Hace la conexion del SIGNAL activated con m_activated para tratar el evento.
*/
/**
\param parent
**/
BfBuscarTipoIVA::BfBuscarTipoIVA ( QWidget *parent )
        : BlComboBox ( parent )
{
    _depura ( "BfBuscarTipoIVA::BfBuscarTipoIVA", 0 );
    m_comboRecordSet = NULL;
    m_table = "tipo_iva";
    _depura ( "END BfBuscarTipoIVA::BfBuscarTipoIVA", 0 );
}


/** Libera la memoria dinamica que se estaba utilizando.
*/
/**
**/
BfBuscarTipoIVA::~BfBuscarTipoIVA()
{
    _depura ( "BfBuscarTipoIVA::~BfBuscarTipoIVA", 0 );
    _depura ( "END BfBuscarTipoIVA::~BfBuscarTipoIVA", 0 );
}


/** Con este metodo se puede indicar al Widget cual es el tipo_iva seleccionado.
    Recarga el cursor de tipo_iva y cuando encuentra uno con el mismo identificador
    que el que se ha pasado por parametro lo establece como elemento seleccionado.
*/
/**
\param idtipo_iva
\returns
**/
void BfBuscarTipoIVA::setId ( QString idtipo_iva )
{
    _depura ( "BfBuscarTipoIVA::setidtipo_iva", 0 );

    if ( m_comboRecordSet != NULL ) {
        delete m_comboRecordSet;
    } // end if

    m_comboRecordSet = mainCompany() ->loadQuery ( "SELECT * FROM tipo_iva" );
    /// Tratamos el caso en que no se haya devuelto nada.

    if ( m_comboRecordSet == NULL ) return;

    int i = 0;
    int i1 = 0;
    clear();
    addItem ( "--" );

    while ( !m_comboRecordSet->eof() ) {
        i ++;

        if ( m_comboRecordSet->valor ( "idtipo_iva" ) == idtipo_iva ) {
            i1 = i;
        } // end if

        addItem ( m_comboRecordSet->valor ( "desctipo_iva" ) );
        m_comboRecordSet->nextRecord();
    } // end while

    setCurrentIndex ( i1 );

    _depura ( "END BfBuscarTipoIVA::setidtipo_iva", 0 );
}



/** Devuelve el identificador del tipo_iva seleccionado
**/
/**
\return
**/
QString BfBuscarTipoIVA::id()
{
    _depura ( "BfBuscarTipoIVA::idtipo_iva", 0 );
    _depura ( "END BfBuscarTipoIVA::idtipo_iva", 0 );

    /// Como puede haber habido un error con la base de datos debemos tratar dicho caso.
    if ( !m_comboRecordSet ) return "0";

    return m_comboRecordSet->valor ( "idtipo_iva", currentIndex() - 1 );
}


/** SLOT que responde a la activacion de un elemento en el QComboBox y que hace que se emita el SIGNAL valueChanged
**/
/**
\param index
**/
void BfBuscarTipoIVA::m_activated ( int index )
{
    _depura ( "BfBuscarTipoIVA::m_activated", 0 );

    if ( index > 0 ) {
        emit ( valueChanged ( m_comboRecordSet->valor ( "idtipo_iva", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    } // end if

    _depura ( "END BfBuscarTipoIVA::m_activated", 0 );
}



/// ==================================================================

/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el SIGNAL activated() con m_activated() para tratarlo.
*/
/**
\param parent
**/
BfBuscarTipoIVADelegate::BfBuscarTipoIVADelegate ( QWidget *parent )
        : BlComboBox ( parent )
{
    _depura ( "BfBuscarTipoIVADelegate::BfBuscarTipoIVADelegate", 0 );
    m_comboRecordSet = NULL;
    setSizeAdjustPolicy ( QComboBox::AdjustToContents );
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    _depura ( "END BfBuscarTipoIVADelegate::BfBuscarTipoIVADelegate", 0 );
}


/** Libera la memoria reservada.
*/
/**
**/
BfBuscarTipoIVADelegate::~BfBuscarTipoIVADelegate()
{
    _depura ( "BfBuscarTipoIVADelegate::~BfBuscarTipoIVADelegate", 0 );
    _depura ( "END BfBuscarTipoIVADelegate::~BfBuscarTipoIVADelegate", 0 );
}


/** Permite indicar al Widget cual es la serie de factura seleccionada por defecto.
    Recarga cursor de serie_factura y cuando encuentra un registro cuyo codigoserie_factura coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
/**
\param code
**/
void BfBuscarTipoIVADelegate::set ( const QString &cod )
{
    _depura ( "BfBuscarTipoIVADelegate::set", 0 );

    int index = 0;
    QString codigo = cod;

    if ( m_comboRecordSet != NULL ) {
        delete m_comboRecordSet;
    } // end if

    m_comboRecordSet = mainCompany() ->loadQuery ( "SELECT desctipo_iva FROM tipo_iva " );

    /// Comprobamos que no haya habido errores
    if ( !m_comboRecordSet ) return;

    clear();

    while ( !m_comboRecordSet->eof() ) {
        addItem ( m_comboRecordSet->valor ( "desctipo_iva" ) );
        m_comboRecordSet->nextRecord();
        if ( m_comboRecordSet->valor ( "desctipo_iva" ) == cod )
            index = m_comboRecordSet->currentRecord();
    } // end while

    setEditText ( cod );
    setCurrentIndex ( index );

    _depura ( "END BfBuscarTipoIVADelegate::set", 0 );
}

