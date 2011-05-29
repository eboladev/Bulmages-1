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


#include "busquedacolor.h"
#include "blfunctions.h"


/** Constructor del Widget conecta las se&ntilde;ales y prepara los componentes.
*/
/**
\param parent
**/
BusquedaColor::BusquedaColor ( QWidget *parent, const char * )
        : BlComboBox ( parent )
{
    blDebug ( "BusquedaColor::BusquedaColor", 0 );
    m_comboRecordSet = NULL;
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    blDebug ( "END BusquedaColor::BusquedaColor", 0 );
}


/** El destructor no precisa de operaciones especiales
*/
/**
**/
BusquedaColor::~BusquedaColor()
{
    blDebug ( "BusquedaColor::~BusquedaColor", 0 );
    blDebug ( "END BusquedaColor::~BusquedaColor", 0 );
}


/** Mediante este metodo establecemos el color que debe presentar el Widget
    como seleccionado.
    Recarga el cursor y presenta en el combo aquel que se ajusta al id pasado.
*/
/**
\param idtc_color
\return
**/
void BusquedaColor::setidtc_color ( QString idtc_color )
{
    blDebug ( "BusquedaColor::setidtc_color", 0, idtc_color );
    if ( m_comboRecordSet != NULL ) {
        delete m_comboRecordSet;
    } // end if
    m_comboRecordSet = mainCompany() ->loadQuery ( "SELECT * FROM tc_color ORDER BY nomtc_color" );
    if ( !m_comboRecordSet ) return;
    int i = 0;
    int i1 = 0;
    int i2 = 0;
    clear();
    addItem ( "--" );
    while ( !m_comboRecordSet->eof() ) {
        i++;
        if ( m_comboRecordSet->value( "idtc_color" ) == idtc_color )
            i1 = i;
        addItem ( m_comboRecordSet->value( "nomtc_color" ) );
        m_comboRecordSet->nextRecord();
    } //end while
    if ( i1 != 0 ) {
        setCurrentIndex ( i1 );
    } else {
        setCurrentIndex ( i2 );
    } // end if
    blDebug ( "END BusquedaColor::setidtc_color", 0, idtc_color );
}


///
/**
\param idtc_color
**/
void BusquedaColor::setFieldValue ( QString idtc_color )
{
    blDebug ( "BusquedaColor::setFieldValue", 0 );
    setidtc_color ( idtc_color );
    blDebug ( "END BusquedaColor::setFieldValue", 0 );
}


///
/**
\return
**/
QString BusquedaColor::idtc_color()
{
    blDebug ( "BusquedaColor::idtc_color", 0 );
    int index = currentIndex();
    if ( index > 0 ) {
        blDebug ( "END BusquedaColor::idtc_color", 0, m_comboRecordSet->value( "idtc_color", index - 1 ) );
        return ( m_comboRecordSet->value( "idtc_color", index - 1 ) );
    } else {
        blDebug ( "END BusquedaColor::idtc_color", 0 );
        return "";
    } // end if
}


///
/**
\return
**/
QString BusquedaColor::fieldValue()
{
    blDebug ( "BusquedaColor::fieldValue", 0 );
    blDebug ( "END BusquedaColor::fieldValue", 0 );
    return idtc_color();
}


///
/**
\param comp
**/
void BusquedaColor::setMainCompany ( BfCompany *comp )
{
    blDebug ( "BusquedaColor::setMainCompany", 0 );
    BlMainCompanyPointer::setMainCompany ( comp );
    blDebug ( "END BusquedaColor::setMainCompany", 0 );
}


///
/**
\param index
**/
void BusquedaColor::m_activated ( int index )
{
    blDebug ( "BusquedaColor::m_activated", 0 );
    if ( index > 0 ) {
        emit ( valueChanged ( m_comboRecordSet->value( "idtc_color", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    } // end if
    blDebug ( "END BusquedaColor::m_activated", 0 );
}

/// ========================= ITEM DELEGATE ===============================0

/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el SIGNAL activated() con m_activated() para tratarlo.
*/
/**
\param parent
**/
BusquedaColorDelegate::BusquedaColorDelegate ( QWidget *parent )
        : BlComboBoxDelegate ( parent )
{
    blDebug ( "BusquedaColorDelegate::BusquedaColorDelegate", 0 );
    setSizeAdjustPolicy ( QComboBox::AdjustToContents );
    setQuery ( "SELECT idtc_color, nomtc_color FROM tc_color ORDER BY nomtc_color" );
    setTableName ( "tc_color" );
    setFieldId ( "idtc_color" );
    m_valores["nomtc_color"] = "";
    setAllowNull(FALSE);
    blDebug ( "END BusquedaColorDelegate::BusquedaColorDelegate", 0 );
}


/** Libera la memoria reservada.
*/
/**
**/
BusquedaColorDelegate::~BusquedaColorDelegate()
{
    blDebug ( "BusquedaColorDelegate::~BusquedaColorDelegate", 0 );
    blDebug ( "END BusquedaColorDelegate::~BusquedaColorDelegate", 0 );
}

