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
    BL_FUNC_DEBUG
    m_comboRecordSet = NULL;
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    
}


/** El destructor no precisa de operaciones especiales
*/
/**
**/
BusquedaColor::~BusquedaColor()
{
    BL_FUNC_DEBUG
    
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
    BL_FUNC_DEBUG
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
    
}


///
/**
\param idtc_color
**/
void BusquedaColor::setFieldValue ( QString idtc_color )
{
    BL_FUNC_DEBUG
    setidtc_color ( idtc_color );
    
}


///
/**
\return
**/
QString BusquedaColor::idtc_color()
{
    BL_FUNC_DEBUG
    int index = currentIndex();
    if ( index > 0 ) {
        
        return ( m_comboRecordSet->value( "idtc_color", index - 1 ) );
    } else {
        
        return "";
    } // end if
}


///
/**
\return
**/
QString BusquedaColor::fieldValue()
{
    BL_FUNC_DEBUG
    
    return idtc_color();
}


///
/**
\param comp
**/
void BusquedaColor::setMainCompany ( BfCompany *comp )
{
    BL_FUNC_DEBUG
    BlMainCompanyPointer::setMainCompany ( comp );
    
}


///
/**
\param index
**/
void BusquedaColor::m_activated ( int index )
{
    BL_FUNC_DEBUG
    if ( index > 0 ) {
        emit ( valueChanged ( m_comboRecordSet->value( "idtc_color", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    } // end if
    
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
    BL_FUNC_DEBUG
    setSizeAdjustPolicy ( QComboBox::AdjustToContents );
    setQuery ( "SELECT idtc_color, nomtc_color FROM tc_color ORDER BY nomtc_color" );
    setTableName ( "tc_color" );
    setFieldId ( "idtc_color" );
    m_valores["nomtc_color"] = "";
    setAllowNull(false);
    
}


/** Libera la memoria reservada.
*/
/**
**/
BusquedaColorDelegate::~BusquedaColorDelegate()
{
    BL_FUNC_DEBUG
    
}

