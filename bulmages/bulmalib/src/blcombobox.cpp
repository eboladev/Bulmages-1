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


/// Existe una instancia del objeto global g_theApp.



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
    blDebug ( "BlComboBox::BlComboBox", 0 );
    m_comboRecordSet = NULL;
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    connect ( g_theApp, SIGNAL ( dbTableChanged ( const QString & ) ), this, SLOT ( onDbTableChanged ( const QString & ) ) );
    m_null = TRUE;

    blDebug ( "END BlComboBox::BlComboBox", 0 );
}


///
/**
**/
BlComboBox::~BlComboBox()
{
    blDebug ( "BlComboBox::~BlComboBox", 0 );
    if ( m_comboRecordSet != NULL )
        delete m_comboRecordSet;
    blDebug ( "END BlComboBox::~BlComboBox", 0 );
}


///
/**
**/
void BlComboBox::onDbTableChanged ( const QString &t )
{
    blDebug ( "BlComboBox::onDbTableChanged", 0 );
    if ( m_table == t ) {
        setId ( id() );
    } // end if
    blDebug ( "END BlComboBox::onDbTableChanged", 0 );
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
    blDebug ( "BlComboBox::setId", 0, id );
    if ( m_comboRecordSet != NULL ) {
        delete m_comboRecordSet;
    } // end if

    m_comboRecordSet = mainCompany() ->loadQuery ( m_query );
    /// Si ha habido un problema con la base de datos salimos.
    if ( !m_comboRecordSet ) return;

    int i = 0;
    int i1 = 0;
    clear();
    if ( m_null ) {
        addItem ( "--" );
        i++;
    } // end if

    while ( !m_comboRecordSet->eof() ) {

        if ( m_comboRecordSet->value( m_fieldId ) == id ) {
            i1 = i;
        } // end if

        /// Inicializamos los valores de vuelta a ""
        QMapIterator<QString, QString> it ( m_valores );
        QString cad = "";
        while ( it.hasNext() ) {
            it.next();
	    if (cad != "") {
	        cad = cad + " ";
	    } // end if
            cad = cad  + m_comboRecordSet->value( it.key() );
        } // end while

        addItem ( cad );
        i ++;
        m_comboRecordSet->nextRecord();
    } // end while

    setCurrentIndex ( i1 );
    blDebug ( "END BlComboBox::setIdProvincia", 0 );
}


///
/**
\param id
**/
void BlComboBox::setFieldValue ( QString id )
{
    blDebug ( "BlComboBox::setFieldValue", 0 );
    setId ( id );
    blDebug ( "END BlComboBox::setFieldValue", 0 );
}


/// SLOT que se activa al activar un elemento del desplegable
/**
\param index
**/
void BlComboBox::m_activated ( int index )
{
    blDebug ( "BlComboBox::m_activated", 0 );
    if ( index > 0 ) {
        if ( m_null ) {
            emit ( valueChanged ( m_comboRecordSet->value( m_fieldId, index - 1 ) ) );
        } else {
            emit ( valueChanged ( m_comboRecordSet->value( m_fieldId, index ) ) );
        } // end if
    } else {
        emit ( valueChanged ( "" ) );
    }
    blDebug ( "END BlComboBox::m_activated", 0 );
}


///
/**
\return
**/
QString BlComboBox::id()
{
    blDebug ( "BlComboBox::id", 0 );
    if ( currentIndex() >= 0 ) {

        /// Si el campo tiene un elemento nulo, al seleccionarlo hay que devolver una cadena vac&iacute;a,
        /// y si se selecciona otro se retorna el valor del n&uacute;mero de elemento menos uno (sobra el valor nulo)
        if ( m_null ) {
            if ( currentIndex() == 0 ) {
                return "";
            } else {
                return m_comboRecordSet->value( m_fieldId, currentIndex() - 1 );
            } // end if

        /// Si el campo no tiene valor nulo, el orden coincide con el n&uacute;mero de &iacute;ndice
        } else {
            return m_comboRecordSet->value( m_fieldId, currentIndex() );
        } // end if

    } // end if
    blDebug ( "END BlComboBox::id", 0 );
    return "";
}


///
/**
\return
**/
QString BlComboBox::fieldValue()
{
    blDebug ( "BlComboBox::fieldValue", 0 );
    blDebug ( "END BlComboBox::fieldValue", 0 );
    return id();
}


///
/**
**/
void BlComboBox::setQuery ( QString q )
{
    m_query = q;
}


///
/**
**/
void BlComboBox::setTableName ( QString tableName )
{
    m_table = tableName;
}


///
/**
**/
void BlComboBox::setFieldId ( QString fieldId )
{
    m_fieldId = fieldId;
}


///
/**
**/
void BlComboBox::setAllowNull ( bool v )
{
    m_null = v;
}


