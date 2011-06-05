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

#include "bfbuscaralmacen.h"
#include "bfcompany.h"
#include "blfunctions.h"


/** Constructor del Widget conecta las se&ntilde;ales y prepara los componentes.
*/
/**
\param parent
**/
BfBuscarAlmacen::BfBuscarAlmacen ( QWidget *parent, const char * )
        : BlComboBox ( parent )
{
    blDebug ( "BfBuscarAlmacen::BfBuscarAlmacen", 0 );
    m_comboRecordSet = NULL;
    m_codigoalmacen = "";
    m_table = "almacen";
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


/** El destructor no precisa de operaciones especiales
*/
/**
**/
BfBuscarAlmacen::~BfBuscarAlmacen()
{
    blDebug ( "BfBuscarAlmacen::~BfBuscarAlmacen", 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


/** Mediante este metodo establecemos el almacen que debe presentar el Widget
    como seleccionado.
    Recarga el cursor y presenta en el combo aquel que se ajusta al id pasado.
*/
/**
\param idalmacen
\return
**/
void BfBuscarAlmacen::setId ( QString idalmacen )
{
    blDebug ( "BfBuscarAlmacen::setidalmacen", 0, idalmacen );

    try {
        /// Si no existe la mainCompany() pq aun no ha sido establecida salimos
        if ( !mainCompany() ) return;

        /// Si lo que se pasa como serie es un valor malo cogemos la serie de factura por defecto.
        if ( idalmacen.isEmpty() || idalmacen == "0" ) {
            idalmacen = g_confpr->value( CONF_IDALMACEN_DEFECTO );
        } // end if

        /// Si ya esta creado el cursorcombo lo borramos.
        if ( m_comboRecordSet != NULL ) {
            delete m_comboRecordSet;
        } // end if

        m_comboRecordSet = mainCompany() ->loadQuery ( "SELECT * FROM almacen ORDER BY nomalmacen" );
        if ( !m_comboRecordSet ) return;
        int i = 0;
        int i1 = 0;
        int i2 = 0;
        clear();
        addItem ( "--" );
        while ( !m_comboRecordSet->eof() ) {
            i++;
            if ( m_comboRecordSet->value( "codigoalmacen" ) == m_codigoalmacen )
                i2 = i;
            if ( m_comboRecordSet->value( "idalmacen" ) == idalmacen )
                i1 = i;
            addItem ( m_comboRecordSet->value( "nomalmacen" ) );
            m_comboRecordSet->nextRecord();
        } //end while
        if ( i1 != 0 ) {
            setCurrentIndex ( i1 );
        } else {
            setCurrentIndex ( i2 );
        } // end if
    } catch ( ... ) {
        blDebug ( "Error en BfBuscarAlmacen::setidalmacen", 2 );
    } // end try
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}



///
/**
\return
**/
QString BfBuscarAlmacen::id()
{
    blDebug ( "BfBuscarAlmacen::idalmacen", 0 );
    int index = currentIndex();

    if ( index > 0 ) {
        blDebug ( ("END ", Q_FUNC_INFO), 0, _("'index' > 0") );
        return ( m_comboRecordSet->value( "idalmacen", index - 1 ) );
    } else {
        blDebug ( ("END ", Q_FUNC_INFO), 0 );
        return "";
    } // end if
}

///
/**
\param comp
**/
void BfBuscarAlmacen::setMainCompany ( BfCompany *comp )
{
    blDebug ( "BfBuscarAlmacen::setMainCompany", 0 );
    BlMainCompanyPointer::setMainCompany ( comp );
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre ='AlmacenDefecto'" );
    if ( cur ) {
        if ( !cur->eof() ) {
            m_codigoalmacen = cur->value( "valor" );
        } // end if
        delete cur;
    } // end if
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param index
**/
void BfBuscarAlmacen::m_activated ( int index )
{
    blDebug ( "BfBuscarAlmacen::m_activated", 0 );

    if ( index > 0 ) {
        emit ( valueChanged ( m_comboRecordSet->value( "idalmacen", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    } // end if

    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


/// ========================= ITEM DELEGATE ===============================0

/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el SIGNAL activated() con m_activated() para tratarlo.
*/
/**
\param parent
**/
BfBuscarAlmacenDelegate::BfBuscarAlmacenDelegate ( QWidget *parent )
        : BlComboBox ( parent )
{
    blDebug ( "BfBuscarAlmacenDelegate::BfBuscarAlmacenDelegate", 0 );
    m_comboRecordSet = NULL;
    setSizeAdjustPolicy ( QComboBox::AdjustToContents );
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


/** Libera la memoria reservada.
*/
/**
**/
BfBuscarAlmacenDelegate::~BfBuscarAlmacenDelegate()
{
    blDebug ( "BfBuscarAlmacenDelegate::~BfBuscarAlmacenDelegate", 0 );
    if ( m_comboRecordSet != NULL )
        delete m_comboRecordSet;
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


/** Permite indicar al Widget cual es la serie de factura seleccionada por defecto.
    Recarga cursor de serie_factura y cuando encuentra un registro cuyo codigoserie_factura coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
/**
\param cod
**/
void BfBuscarAlmacenDelegate::set ( const QString &cod )
{
    blDebug ( "BfBuscarAlmacenDelegate::set", 0 );
    int index = 0;
    QString codigo = cod;

    if ( m_comboRecordSet != NULL ) {
        delete m_comboRecordSet;
    } // end if

    m_comboRecordSet = mainCompany() ->loadQuery ( "SELECT codigoalmacen, nomalmacen FROM almacen ORDER BY nomalmacen" );
    clear();

    while ( !m_comboRecordSet->eof() ) {
        addItem ( m_comboRecordSet->value( "nomalmacen" ) + ", " + m_comboRecordSet->value( "codigoalmacen" ) );
        m_comboRecordSet->nextRecord();

        if ( m_comboRecordSet->value( "nomalmacen" ) + ", " + m_comboRecordSet->value( "codigoalmacen" ) == cod ) {
            index = m_comboRecordSet->currentRecord();
        } // end if

    }// end while

    setEditText ( cod );
    setCurrentIndex ( index );

    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}

