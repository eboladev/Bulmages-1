/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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


/// Existe una instancia del objeto global theApp.
extern BlApplication *theApp;


/** Inicializa todos los componenetes a NULL para que no se produzcan confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el activar un tipo con m_activated.
*/
/// \TODO: No deberia usarse m_activated como signal ya que confunde con una variable de clase.
/**
\param parent
**/
BlComboBox::BlComboBox ( QWidget *parent )
        : QComboBox ( parent ), BlMainCompanyPointer()
{
    _depura ( "BlComboBox::BlComboBox", 0 );
    m_cursorcombo = NULL;
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    connect(theApp, SIGNAL(tablaCambiada(const QString &)), this, SLOT(onTablaCambiada(const QString &)));
    m_null = TRUE;

    _depura ( "END BlComboBox::BlComboBox", 0 );
}


///
/**
**/
BlComboBox::~BlComboBox()
{
    _depura ( "BlComboBox::~BlComboBox", 0 );
    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    _depura ( "END BlComboBox::~BlComboBox", 0 );
}


///
/**
**/
void BlComboBox::onTablaCambiada(const QString &t) {
	if (m_tabla == t) {
		setId(id());
	} // end if
}


/** Este metodo sirve para indicar al Widget cual es la item por defecto.
    Recarga todo el query y compone de nuevo el comboBox estableciendo el item
    por defecto aquella cuyo identificador coincide con el que se ha pasado por parametro.
*/
/**
\param id
**/
void BlComboBox::setId ( QString id )
{
    _depura ( "BlComboBox::setId", 0, id );
    if ( m_cursorcombo != NULL ) {
        delete m_cursorcombo;
    } // end if

    m_cursorcombo = mainCompany() ->loadQuery ( m_query );
    /// Si ha habido un problema con la base de datos salimos.
    if ( !m_cursorcombo ) return;

    int i = 0;
    int i1 = 0;
    clear();
    if (m_null) {
	addItem ( "--" );
    } // end if

    while ( !m_cursorcombo->eof() ) {
        i ++;
        if ( m_cursorcombo->valor ( m_id ) == id ) {
            i1 = i;
	} // end if

	/// Inicializamos los valores de vuelta a ""
	QMapIterator<QString, QString> it(m_valores);
	QString cad;
	while (it.hasNext()) {
		it.next();
		cad = cad +" "+ m_cursorcombo->valor(it.key());
	} // end while

        addItem ( cad );
        m_cursorcombo->siguienteregistro();
    } // end while

    setCurrentIndex ( i1 );
    _depura ( "END BusquedaProvincia::setIdProvincia", 0 );
}


///
/**
\param idprovincia
**/
void BlComboBox::setValorCampo ( QString id )
{
    _depura ( "BlComboBox::setValorCampo", 0 );
    setId ( id );
    _depura ( "END BlComboBox::setValorCampo", 0 );
}


/// SLOT que se activa al activar un elemento del desplegable
/**
\param index
**/
void BlComboBox::m_activated ( int index )
{
    _depura ( "BlComboBox::m_activated", 0 );
    if ( index > 0 ) {
	if (m_null) {
	        emit ( valueChanged ( m_cursorcombo->valor ( m_id, index - 1 ) ) );
	} else {
	        emit ( valueChanged ( m_cursorcombo->valor ( m_id, index ) ) );
	} // end if
    } else {
        emit ( valueChanged ( "" ) );
    }
    _depura ( "END BlComboBox::m_activated", 0 );
}


///
/**
\return
**/
QString BlComboBox::id()
{
    _depura ( "BlComboBox::id", 0 );
    if ( currentIndex() > 0 ) {
	if (m_null) {
        return m_cursorcombo->valor ( m_id, currentIndex() - 1 );
	} else {
        return m_cursorcombo->valor ( m_id, currentIndex() );
	} // end if
    } else {
        return "";
    } // end if
    _depura ( "END BlComboBox::id", 0 );
}


///
/**
\return
**/
QString BlComboBox::valorCampo()
{
    _depura ( "BlComboBox::valorCampo", 0 );
    _depura ( "END BlComboBox::valorCampo", 0 );
    return id();
}


///
/**
**/
void BlComboBox::setQuery(QString q) {
	m_query = q;
}


///
/**
**/
void BlComboBox::setTableName(QString tableName) {
	m_tabla = tableName;
}


///
/**
**/
void BlComboBox::setCampoId(QString cid) {
	m_id = cid;
}


///
/**
**/
void BlComboBox::allowNull(bool v) {
	m_null = v;
}

