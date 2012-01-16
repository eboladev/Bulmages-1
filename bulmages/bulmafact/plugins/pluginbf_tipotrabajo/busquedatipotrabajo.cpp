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

#include <QComboBox>

#include "busquedatipotrabajo.h"
#include "bfcompany.h"
#include "blfunctions.h"


/// Constructor del Widget conecta las se&ntilde;ales y prepara los componentes.
/**
\param parent
**/
BusquedaTipoTrabajo::BusquedaTipoTrabajo ( QWidget *parent, const char * )
        : BlComboBox ( parent )
{
    BL_FUNC_DEBUG
    m_cursorcombo = NULL;
    setTableName("tipotrabajo");
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    m_idtipotrabajo = "";
    
}


/// El destructor no precisa de operaciones especiales
/**
**/
BusquedaTipoTrabajo::~BusquedaTipoTrabajo()
{
    BL_FUNC_DEBUG
    
}


/** Mediante este metodo establecemos el tipo de trabajo que debe presentar el Widget
    como seleccionado.
    Recarga el cursor y presenta en el combo aquel que se ajusta al id pasado.
*/
/**
\param idtipotrabajo
**/
void BusquedaTipoTrabajo::setId ( QString idtipotrabajo )
{
    BL_FUNC_DEBUG
    if ( m_cursorcombo != NULL ) {
        delete m_cursorcombo;
    } // end if
    m_cursorcombo = mainCompany() ->loadQuery ( "SELECT * FROM tipotrabajo ORDER BY nomtipotrabajo" );
    if ( !m_cursorcombo ) return;
    int i = 0;
    int i1 = 0;
    clear();
    addItem ( "--" );
    while ( !m_cursorcombo->eof() ) {
        i++;
        if ( m_cursorcombo->value( "idtipotrabajo" ) == idtipotrabajo )
            i1 = i;
        addItem ( m_cursorcombo->value( "nomtipotrabajo" ) );
        m_cursorcombo->nextRecord();
    } //end while
    if ( i1 != 0 ) {
        setCurrentIndex ( i1 );
    } // end if
    
}


///
/**
\return
**/
QString BusquedaTipoTrabajo::id()
{
    BL_FUNC_DEBUG
    int index = currentIndex();
    if ( index > 0 ) {
        
        return ( m_cursorcombo->value( "idtipotrabajo", index - 1 ) );
    } else {
        
        return "";
    } // end if
}


///
/**
\param index
**/
void BusquedaTipoTrabajo::m_activated ( int index )
{
    BL_FUNC_DEBUG
    if ( index > 0 ) {
        emit ( valueChanged ( m_cursorcombo->value( "idtipotrabajo", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    } // end if
    
}



/// Busqueda Tipo Trabajo Delegate
/// ==============================
/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el SIGNAL activated() con m_activated() para tratarlo.
*/
/**
\param parent
**/
BusquedaTipoTrabajoDelegate::BusquedaTipoTrabajoDelegate ( QWidget *parent )
        : BlComboBox ( parent )
{
    BL_FUNC_DEBUG
    m_cursorcombo = NULL;
    setSizeAdjustPolicy ( BlComboBox::AdjustToContents );
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    
}


/** Libera la memoria reservada.
*/
/**
**/
BusquedaTipoTrabajoDelegate::~BusquedaTipoTrabajoDelegate()
{
    BL_FUNC_DEBUG
    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    
}


/** Permite indicar al Widget cual es el tipo de trabajo seleccionado por defecto.
    Recarga cursor de nomtipotrabajo y cuando encuentra un registro cuyo idtrabajo coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
/**
\param cod
**/
void BusquedaTipoTrabajoDelegate::set ( const QString &cod )
{
    BL_FUNC_DEBUG
    int index = 0;
    QString codigo = cod;

    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;

    m_cursorcombo = mainCompany() ->loadQuery ( "SELECT idtipotrabajo, nomtipotrabajo FROM tipotrabajo " );
    clear();
    while ( !m_cursorcombo->eof() ) {
        addItem ( m_cursorcombo->value( "nomtipotrabajo" ) );
        m_cursorcombo->nextRecord();
        if ( m_cursorcombo->value( "nomtipotrabajo" ) == cod )
            index = m_cursorcombo->currentRecord();
    } // end while
    setEditText ( cod );
    setCurrentIndex ( index );

    
}


///
/**
\return
**/
QString BusquedaTipoTrabajoDelegate::id()
{
    BL_FUNC_DEBUG
    
    return m_cursorcombo->value( "idtipotrabajo", currentIndex() );
}

