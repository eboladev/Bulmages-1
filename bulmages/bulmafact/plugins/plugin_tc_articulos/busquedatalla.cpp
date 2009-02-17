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
#include "busquedatalla.h"
#include "bfcompany.h"
#include "blfunctions.h"


/** Constructor del Widget conecta las se&ntilde;ales y prepara los componentes.
*/
/**
\param parent
**/
BusquedaTalla::BusquedaTalla ( QWidget *parent, const char * )
        : BlComboBox ( parent )
{
    _depura ( "BusquedaTalla::BusquedaTalla", 0 );
    m_cursorcombo = NULL;
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    _depura ( "END BusquedaTalla::BusquedaTalla", 0 );
}


/** El destructor no precisa de operaciones especiales
*/
/**
**/
BusquedaTalla::~BusquedaTalla()
{
    _depura ( "BusquedaTalla::~BusquedaTalla", 0 );
    _depura ( "END BusquedaTalla::~BusquedaTalla", 0 );
}


/** Mediante este metodo establecemos el almacen que debe presentar el Widget
    como seleccionado.
    Recarga el cursor y presenta en el combo aquel que se ajusta al id pasado.
*/
/**
\param idtc_talla
\return
**/
void BusquedaTalla::setidtc_talla ( QString idtc_talla )
{
    _depura ( "BusquedaTalla::setidtc_talla", 0, idtc_talla );
    if ( m_cursorcombo != NULL ) {
        delete m_cursorcombo;
    } // end if
    m_cursorcombo = empresaBase() ->cargacursor ( "SELECT * FROM tc_talla ORDER BY nomtc_talla" );
    if ( !m_cursorcombo ) return;
    int i = 0;
    int i1 = 0;
    int i2 = 0;
    clear();
    addItem ( "--" );
    while ( !m_cursorcombo->eof() ) {
        i++;
        if ( m_cursorcombo->valor ( "idtc_talla" ) == idtc_talla )
            i1 = i;
        addItem ( m_cursorcombo->valor ( "nomtc_talla" ) );
        m_cursorcombo->siguienteregistro();
    } //end while
    if ( i1 != 0 ) {
        setCurrentIndex ( i1 );
    } else {
        setCurrentIndex ( i2 );
    } // end if
    _depura ( "END BusquedaTalla::setidtc_talla", 0, idtc_talla );
}


///
/**
\param idtc_talla
**/
void BusquedaTalla::setValorCampo ( QString idtc_talla )
{
    _depura ( "BusquedaTalla::setValorCampo", 0 );
    setidtc_talla ( idtc_talla );
    _depura ( "END BusquedaTalla::setValorCampo", 0 );
}


///
/**
\return
**/
QString BusquedaTalla::idtc_talla()
{
    _depura ( "BusquedaTalla::idtc_talla", 0 );
    int index = currentIndex();
    if ( index > 0 ) {
        _depura ( "END BusquedaTalla::idtc_talla", 0, m_cursorcombo->valor ( "idtc_talla", index - 1 ) );
        return ( m_cursorcombo->valor ( "idtc_talla", index - 1 ) );
    } else {
        _depura ( "END BusquedaTalla::idtc_talla", 0 );
        return "";
    } // end if
}


///
/**
\return
**/
QString BusquedaTalla::valorCampo()
{
    _depura ( "BusquedaTalla::valorCampo", 0 );
    _depura ( "END BusquedaTalla::valorCampo", 0 );
    return idtc_talla();
}


///
/**
\param comp
**/
void BusquedaTalla::setMainCompany ( BfCompany *comp )
{
    _depura ( "BusquedaTalla::setMainCompany", 0 );
    BlMainCompanyPointer::setMainCompany ( comp );
    _depura ( "END BusquedaTalla::setMainCompany", 0 );
}


///
/**
\param index
**/
void BusquedaTalla::m_activated ( int index )
{
    _depura ( "BusquedaTalla::m_activated", 0 );
    if ( index > 0 ) {
        emit ( valueChanged ( m_cursorcombo->valor ( "idtc_talla", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    } // end if
    _depura ( "END BusquedaTalla::m_activated", 0 );
}

/// ========================= ITEM DELEGATE ===============================0

/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el SIGNAL activated() con m_activated() para tratarlo.
*/
/**
\param parent
**/
BusquedaTallaDelegate::BusquedaTallaDelegate ( QWidget *parent )
        : BlComboBox ( parent )
{
    _depura ( "BusquedaTallaDelegate::BusquedaTallaDelegate", 0 );
    m_cursorcombo = NULL;
    setSizeAdjustPolicy ( QComboBox::AdjustToContents );
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    _depura ( "END BusquedaTallaDelegate::BusquedaTallaDelegate", 0 );
}


/** Libera la memoria reservada.
*/
/**
**/
BusquedaTallaDelegate::~BusquedaTallaDelegate()
{
    _depura ( "BusquedaTallaDelegate::~BusquedaTallaDelegate", 0 );
    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    _depura ( "END BusquedaTallaDelegate::~BusquedaTallaDelegate", 0 );
}


/** Permite indicar al Widget cual es la serie de factura seleccionada por defecto.
    Recarga cursor de serie_factura y cuando encuentra un registro cuyo codigoserie_factura coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
/**
\param cod
**/
void BusquedaTallaDelegate::set ( const QString &cod )
{
    _depura ( "BusquedaTallaDelegate::set", 0 );
    int index = 0;
    QString codigo = cod;

    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;

    m_cursorcombo = empresaBase() ->cargacursor ( "SELECT idtc_talla, nomtc_talla FROM tc_talla ORDER BY nomtc_talla" );
    clear();
    while ( !m_cursorcombo->eof() ) {
        addItem ( m_cursorcombo->valor ( "nomtc_talla" ) );
        m_cursorcombo->siguienteregistro();
        if ( m_cursorcombo->valor ( "nomtc_talla" )  == cod )
            index = m_cursorcombo->regactual();
    }// end while
    setEditText ( cod );
    setCurrentIndex ( index );

    _depura ( "END BusquedaTallaDelegate::set", 0 );
}



///
/**
\return
**/
QString BusquedaTallaDelegate::id()
{
    _depura ( "BusquedaColorDelegate::id", 0 );
    _depura ( "END BusquedaColorDelegate::id", 0 );
    return m_cursorcombo->valor ( "idtc_talla", currentIndex() );
}
