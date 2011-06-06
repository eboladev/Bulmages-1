/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#include "bfbuscarregimenfiscal.h"

/** Inicializa todos los componenetes a NULL para que no se produzcan confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el activar un tipo con m_activated.
*/
/// \TODO: No deberia usarse m_activated como signal ya que confunde con una variable de clase.
/**
\param parent
**/
BfBuscarRegimenFiscal::BfBuscarRegimenFiscal ( QWidget *parent )
        : BlComboBox ( parent )
{
    blDebug ( Q_FUNC_INFO, 0 );
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    addItem ( "Normal" );
    addItem ( "Intracomunitario" );
    addItem ( "Extracomunitario" );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


/** Se libera memoria destruyendo el cursor que almacena los tipos de IVA si es que esta creado.
*/
/**
**/
BfBuscarRegimenFiscal::~BfBuscarRegimenFiscal()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


/** Mediante este metodo podemos indicar al Widget cual es la forma de pago que esta seleccionada
    pasandole como parametro el idform_pago.
    Busca en la tabla forma_pago de la base de datos aquel registro cuyo idforma_pago coincide con el pasado.
    Rellena el comboBox haciendo que el seleccionado sea el visible.
*/
/**
\param regimen
**/
void BfBuscarRegimenFiscal::setRegimenFiscal ( QString regimen )
{
    blDebug ( Q_FUNC_INFO, 0 );
    if ( regimen == "Normal" ) {
        setCurrentIndex ( 0 );
    } else if ( regimen == "Intracomunitario" ) {
        setCurrentIndex ( 1 );
    } else if ( regimen == "Extracomunitario" ) {
        setCurrentIndex ( 2 );
    } // end if
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param regimen
**/
void BfBuscarRegimenFiscal::setFieldValue ( QString regimen )
{
    blDebug ( Q_FUNC_INFO, 0 );
    setRegimenFiscal ( regimen );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param index
**/
void BfBuscarRegimenFiscal::m_activated ( int index )
{
    blDebug ( Q_FUNC_INFO, 0 );
    if ( index > 0 ) {
        emit ( valueChanged ( currentText() ) );
    } else {
        emit ( valueChanged ( "" ) );
    } // end if
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


/** Devuelve el regimen fiscal seleccionado
**/
/**
\return
**/
QString BfBuscarRegimenFiscal::regimenFiscal()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return ( currentText() );
}


/** Devuelve el regimen fiscal seleccionado
**/
/**
\return
**/
QString BfBuscarRegimenFiscal::fieldValue()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return ( currentText() );
}

