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

#include "blcombobox.h"
#include "busquedacolor.h"
#include "bfcompany.h"
#include "blfunctions.h"


/** Constructor del Widget conecta las se&ntilde;ales y prepara los componentes.
*/
/**
\param parent
**/
BusquedaColor::BusquedaColor ( QWidget *parent, const char * )
        : BlComboBox ( parent )
{
    _depura ( "BusquedaColor::BusquedaColor", 0 );
    m_cursorcombo = NULL;
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    _depura ( "END BusquedaColor::BusquedaColor", 0 );
}


/** El destructor no precisa de operaciones especiales
*/
/**
**/
BusquedaColor::~BusquedaColor()
{
    _depura ( "BusquedaColor::~BusquedaColor", 0 );
    _depura ( "END BusquedaColor::~BusquedaColor", 0 );
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
    _depura ( "BusquedaColor::setidtc_color", 0, idtc_color );
    if ( m_cursorcombo != NULL ) {
        delete m_cursorcombo;
    } // end if
    m_cursorcombo = mainCompany() ->loadQuery ( "SELECT * FROM tc_color ORDER BY nomtc_color" );
    if ( !m_cursorcombo ) return;
    int i = 0;
    int i1 = 0;
    int i2 = 0;
    clear();
    addItem ( "--" );
    while ( !m_cursorcombo->eof() ) {
        i++;
        if ( m_cursorcombo->valor ( "idtc_color" ) == idtc_color )
            i1 = i;
        addItem ( m_cursorcombo->valor ( "nomtc_color" ) );
        m_cursorcombo->nextRecord();
    } //end while
    if ( i1 != 0 ) {
        setCurrentIndex ( i1 );
    } else {
        setCurrentIndex ( i2 );
    } // end if
    _depura ( "END BusquedaColor::setidtc_color", 0, idtc_color );
}


///
/**
\param idtc_color
**/
void BusquedaColor::setFieldValue ( QString idtc_color )
{
    _depura ( "BusquedaColor::setFieldValue", 0 );
    setidtc_color ( idtc_color );
    _depura ( "END BusquedaColor::setFieldValue", 0 );
}


///
/**
\return
**/
QString BusquedaColor::idtc_color()
{
    _depura ( "BusquedaColor::idtc_color", 0 );
    int index = currentIndex();
    if ( index > 0 ) {
        _depura ( "END BusquedaColor::idtc_color", 0, m_cursorcombo->valor ( "idtc_color", index - 1 ) );
        return ( m_cursorcombo->valor ( "idtc_color", index - 1 ) );
    } else {
        _depura ( "END BusquedaColor::idtc_color", 0 );
        return "";
    } // end if
}


///
/**
\return
**/
QString BusquedaColor::fieldValue()
{
    _depura ( "BusquedaColor::fieldValue", 0 );
    _depura ( "END BusquedaColor::fieldValue", 0 );
    return idtc_color();
}


///
/**
\param comp
**/
void BusquedaColor::setMainCompany ( BfCompany *comp )
{
    _depura ( "BusquedaColor::setMainCompany", 0 );
    BlMainCompanyPointer::setMainCompany ( comp );
    _depura ( "END BusquedaColor::setMainCompany", 0 );
}


///
/**
\param index
**/
void BusquedaColor::m_activated ( int index )
{
    _depura ( "BusquedaColor::m_activated", 0 );
    if ( index > 0 ) {
        emit ( valueChanged ( m_cursorcombo->valor ( "idtc_color", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    } // end if
    _depura ( "END BusquedaColor::m_activated", 0 );
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
        : BlComboBox ( parent )
{
    _depura ( "BusquedaColorDelegate::BusquedaColorDelegate", 0 );
    m_cursorcombo = NULL;
    setSizeAdjustPolicy ( QComboBox::AdjustToContents );
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    _depura ( "END BusquedaColorDelegate::BusquedaColorDelegate", 0 );
}


/** Libera la memoria reservada.
*/
/**
**/
BusquedaColorDelegate::~BusquedaColorDelegate()
{
    _depura ( "BusquedaColorDelegate::~BusquedaColorDelegate", 0 );
    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    _depura ( "END BusquedaColorDelegate::~BusquedaColorDelegate", 0 );
}


/** Permite indicar al Widget cual es el color seleccionado por defecto.
    Recarga cursor de color y cuando encuentra un registro cuyo nomtc_color coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
/**
\param cod
**/
void BusquedaColorDelegate::set ( const QString &cod )
{
    _depura ( "BusquedaColorDelegate::set", 0 );
    int index = 0;
    QString codigo = cod;

    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;

    m_cursorcombo = mainCompany() ->loadQuery ( "SELECT idtc_color, nomtc_color FROM tc_color ORDER BY nomtc_color" );
    clear();
//    addItem("--");
    while ( !m_cursorcombo->eof() ) {
        addItem ( m_cursorcombo->valor ( "nomtc_color" ) );
        m_cursorcombo->nextRecord();
        if ( m_cursorcombo->valor ( "nomtc_color" ) == cod )
            index = m_cursorcombo->currentRecord();
    }// end while
    setEditText ( cod );
    setCurrentIndex ( index );

    _depura ( "END BusquedaColorDelegate::set", 0 );
}


///
/**
\return
**/
QString BusquedaColorDelegate::id()
{
    _depura ( "BusquedaColorDelegate::id", 0 );
    _depura ( "END BusquedaColorDelegate::id", 0 );
    return m_cursorcombo->valor ( "idtc_color", currentIndex() );
}
