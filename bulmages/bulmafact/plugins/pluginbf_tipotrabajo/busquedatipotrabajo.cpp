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
        : QComboBox ( parent ), BlMainCompanyPointer()
{
    blDebug ( "BusquedaTipoTrabajo::BusquedaTipoTrabajo", 0 );
    m_cursorcombo = NULL;
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    m_idtipotrabajo = "";
    blDebug ( "END BusquedaTipoTrabajo::BusquedaTipoTrabajo", 0 );
}


/// El destructor no precisa de operaciones especiales
/**
**/
BusquedaTipoTrabajo::~BusquedaTipoTrabajo()
{
    blDebug ( "BusquedaTipoTrabajo::~BusquedaTipoTrabajo", 0 );
    blDebug ( "END BusquedaTipoTrabajo::~BusquedaTipoTrabajo", 0 );
}


/** Mediante este metodo establecemos el almacen que debe presentar el Widget
    como seleccionado.
    Recarga el cursor y presenta en el combo aquel que se ajusta al id pasado.
*/
/**
\param idtipotrabajo
**/
void BusquedaTipoTrabajo::setidtipotrabajo ( QString idtipotrabajo )
{
    blDebug ( "BusquedaTipoTrabajo::setidtipotrabajo", 0, idtipotrabajo );
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
        if ( m_cursorcombo->valor ( "idtipotrabajo" ) == idtipotrabajo )
            i1 = i;
        addItem ( m_cursorcombo->valor ( "nomtipotrabajo" ) );
        m_cursorcombo->nextRecord();
    } //end while
    if ( i1 != 0 ) {
        setCurrentIndex ( i1 );
    } // end if
    blDebug ( "END BusquedaTipoTrabajo::setidtipotrabajo", 0, idtipotrabajo );
}


///
/**
\return
**/
QString BusquedaTipoTrabajo::idtipotrabajo()
{
    blDebug ( "BusquedaTipoTrabajo::idtipotrabajo", 0 );
    int index = currentIndex();
    if ( index > 0 ) {
        blDebug ( "END BusquedaTipoTrabajo::idtipotrabajo", 0 );
        return ( m_cursorcombo->valor ( "idtipotrabajo", index - 1 ) );
    } else {
        blDebug ( "END BusquedaTipoTrabajo::idtipotrabajo", 0 );
        return "";
    } // end if
}


///
/**
\param index
**/
void BusquedaTipoTrabajo::m_activated ( int index )
{
    blDebug ( "BusquedaTipoTrabajo::m_activated", 0 );
    if ( index > 0 ) {
        emit ( valueChanged ( m_cursorcombo->valor ( "idtipotrabajo", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    } // end if
    blDebug ( "END BusquedaTipoTrabajo::m_activated", 0 );
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
    blDebug ( "BusquedaTipoTrabajoDelegate::BusquedaTipoTrabajoDelegate", 0 );
    m_cursorcombo = NULL;
    setSizeAdjustPolicy ( QComboBox::AdjustToContents );
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    blDebug ( "END BusquedaTipoTrabajoDelegate::BusquedaTipoTrabajoDelegate", 0 );
}


/** Libera la memoria reservada.
*/
/**
**/
BusquedaTipoTrabajoDelegate::~BusquedaTipoTrabajoDelegate()
{
    blDebug ( "BusquedaTipoTrabajoDelegate::~BusquedaTipoTrabajoDelegate", 0 );
    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    blDebug ( "END BusquedaTipoTrabajoDelegate::~BusquedaTipoTrabajoDelegate", 0 );
}


/** Permite indicar al Widget cual es la serie de factura seleccionada por defecto.
    Recarga cursor de serie_factura y cuando encuentra un registro cuyo codigoserie_factura coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
/**
\param cod
**/
void BusquedaTipoTrabajoDelegate::set ( const QString &cod )
{
    blDebug ( "BusquedaTipoTrabajoDelegate::set", 0 );
    int index = 0;
    QString codigo = cod;

    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;

    m_cursorcombo = mainCompany() ->loadQuery ( "SELECT idtipotrabajo, nomtipotrabajo FROM tipotrabajo " );
    clear();
    while ( !m_cursorcombo->eof() ) {
        addItem ( m_cursorcombo->valor ( "nomtipotrabajo" ) );
        m_cursorcombo->nextRecord();
        if ( m_cursorcombo->valor ( "nomtipotrabajo" ) == cod )
            index = m_cursorcombo->currentRecord();
    }// end while
    setEditText ( cod );
    setCurrentIndex ( index );

    blDebug ( "END BusquedaTipoTrabajoDelegate::set", 0 );
}


///
/**
\return
**/
QString BusquedaTipoTrabajoDelegate::id()
{
    blDebug ( "BusquedaTipoTrabajoDelegate::id", 0 );
    blDebug ( "END BusquedaTipoTrabajoDelegate::id", 0 );
    return m_cursorcombo->valor ( "idtipotrabajo", currentIndex() );
}

