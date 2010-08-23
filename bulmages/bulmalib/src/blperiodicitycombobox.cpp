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

#include "blperiodicitycombobox.h"
#include "local_BlI18n.h"

///
/**
\return
**/
QString BlPeriodicityComboBox::periodo()
{
    blDebug ( "BlPeriodicityComboBox::periodo", 0 );
    blDebug ( "END BlPeriodicityComboBox::periodo", 0 );
    return m_valores[currentIndex() ];
}


///
/**
\return
**/
QString BlPeriodicityComboBox::fieldValue()
{
    blDebug ( "BlPeriodicityComboBox::fieldValue", 0 );
    blDebug ( "END BlPeriodicityComboBox::fieldValue", 0 );
    return m_valores[currentIndex() ];
}


///
/**
\param index
**/
void BlPeriodicityComboBox::s_activated ( int index )
{
    blDebug ( "Activado el combo box.", 0 );
    if ( index > 0 ) {
        emit ( valueChanged ( m_valores[currentIndex() ] ) );
    } else {
        emit ( valueChanged ( "" ) );
    } // end if
}


///
/**
\param parent
**/
BlPeriodicityComboBox::BlPeriodicityComboBox ( QWidget *parent ) : QComboBox ( parent )
{
    blDebug ( "BlPeriodicityComboBox::BlPeriodicityComboBox", 0 );
    m_textos[0] = _ ( "Semanal" );
    m_textos[1] = _ ( "Quincenal" );
    m_textos[2] = _ ( "Mensual" );
    m_textos[3] = _ ( "Trimestral" );
    m_textos[4] = _ ( "Semestral" );
    m_textos[5] = _ ( "Anual" );
    m_textos[6] = _ ( "Bienal" );
    m_textos[7] = "";

    m_valores[0] = "@ 7 days";
    m_valores[1] = "@ 15 days";
    m_valores[2] = "@ 1 mon";
    m_valores[3] = "@ 3 mons";
    m_valores[4] = "@ 6 mons";
    m_valores[5] = "@ 1 year";
    m_valores[6] = "@ 2 year";
    m_valores[7] = "";

    int i = 0;
    while ( m_textos[i] != "" ) {
        /// Anyade un nuevo elemento al final del QComboBox.
        insertItem ( this->count() + 1, m_textos[i] );
        i++;
    } // end while

    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( s_activated ( int ) ) );
    blDebug ( "END BlPeriodicityComboBox::BlPeriodicityComboBox", 0 );
}


///
/**
**/
BlPeriodicityComboBox::~BlPeriodicityComboBox()
{
    blDebug ( "BlPeriodicityComboBox::~BlPeriodicityComboBox", 0 );
    blDebug ( "END BlPeriodicityComboBox::~BlPeriodicityComboBox", 0 );
}


///
/**
Establece cual es el periodo en el combobox y lo pone.
\param periodo QString en sintaxis de PostgreSQL (@ 1 year, 3 mons, etc)
**/
void BlPeriodicityComboBox::setperiodo ( QString periodo )
{
    blDebug ( "BlPeriodicityComboBox::setperiodo", 0, periodo );
    int i = 0;
    /// Buscamos un campo que se corresponda con el periodo.
    /// El problema es que postgres acepta como sintaxis de entrada la arroba
    /// y como sintaxis de salida no la devuelve.
    while (  (m_valores[i] != periodo)   && (m_valores[i] != "@ "+ periodo) && (m_valores[i] != "") ) {
        i++;
    } // end while
    if ( m_valores[i] != "" ) {
        setCurrentIndex ( i );
    } else {
        setCurrentIndex ( 0 );
    } // end if
    blDebug ( "END BlPeriodicityComboBox::setperiodo", 0, periodo );
}


///
/**
\param periodo
**/
void BlPeriodicityComboBox::setFieldValue ( QString periodo )
{
    blDebug ( "BlPeriodicityComboBox::setFieldValue", 0 );
    setperiodo ( periodo );
    blDebug ( "END BlPeriodicityComboBox::setFieldValue", 0 );
}




