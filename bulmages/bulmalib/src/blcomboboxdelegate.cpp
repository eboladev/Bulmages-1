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

#include "blcomboboxdelegate.h"


/// Existe una instancia del objeto global g_theApp.



/** Inicializa todos los componenetes a NULL para que no se produzcan confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el activar un tipo con m_activated.
*/
/**
\param parent
**/
BlComboBoxDelegate::BlComboBoxDelegate ( QWidget *parent  )
        : QComboBox ( parent ), BlMainCompanyPointer()
{
    _depura ( "BlComboBoxDelegate::BlComboBoxDelegate", 0 );
    m_comboRecordSet = NULL;
    m_null = TRUE;

    _depura ( "END BlComboBoxDelegate::BlComboBoxDelegate", 0 );
}


///
/**
**/
BlComboBoxDelegate::~BlComboBoxDelegate()
{
    _depura ( "BlComboBoxDelegate::~BlComboBoxDelegate", 0 );
    if ( m_comboRecordSet != NULL )
        delete m_comboRecordSet;
    _depura ( "END BlComboBoxDelegate::~BlComboBoxDelegate", 0 );
}



/** Este metodo sirve para indicar al Widget cual es la item por defecto.
    Recarga todo el query y compone de nuevo el comboBox estableciendo el item
    por defecto aquella cuyo identificador coincide con el que se ha pasado por parametro.
*/
/**
\param id
**/
void BlComboBoxDelegate::setId ( QString id )
{
    _depura ( "BlComboBoxDelegate::setId", 0, id );
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

        if ( m_comboRecordSet->valor ( m_fieldId ) == id ) {
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
            cad = cad  + m_comboRecordSet->valor ( it.key() );
        } // end while

        addItem ( cad );
        i ++;
        m_comboRecordSet->nextRecord();
    } // end while

    setCurrentIndex ( i1 );
    _depura ( "END BlComboBoxDelegate::setIdProvincia", 0 );
}


///
/**
\param id
**/
void BlComboBoxDelegate::setFieldValue ( QString id )
{
    _depura ( "BlComboBoxDelegate::setFieldValue", 0 );
    setId ( id );
    _depura ( "END BlComboBoxDelegate::setFieldValue", 0 );
}





///
/**
\return
**/
QString BlComboBoxDelegate::id()
{
    _depura ( "BlComboBoxDelegate::id", 0 );
    if ( currentIndex() >= 0 ) {

        /// Si el campo tiene un elemento nulo, al seleccionarlo hay que devolver una cadena vac&iacute;a,
        /// y si se selecciona otro se retorna el valor del n&uacute;mero de elemento menos uno (sobra el valor nulo)
        if ( m_null ) {
            if ( currentIndex() == 0 ) {
                return "";
            } else {
                return m_comboRecordSet->valor ( m_fieldId, currentIndex() - 1 );
            } // end if

        /// Si el campo no tiene valor nulo, el orden coincide con el n&uacute;mero de &iacute;ndice
        } else {
            return m_comboRecordSet->valor ( m_fieldId, currentIndex() );
        } // end if

    } // end if
    _depura ( "END BlComboBoxDelegate::id", 0 );
    return "";
}


///
/**
\return
**/
QString BlComboBoxDelegate::id(QString value)
{
    _depura ( "BlComboBoxDelegate::id", 0, value );
    m_comboRecordSet->firstRecord();
    while (!m_comboRecordSet->eof()) {
      
        /// Inicializamos los valores de vuelta a ""
        QMapIterator<QString, QString> it ( m_valores );
        QString cad = "";
        while ( it.hasNext() ) {
            it.next();
	    if (cad != "") {
	        cad = cad + " ";
	    } // end if
            cad = cad  + m_comboRecordSet->valor ( it.key() );
        } // end while      
      
       if (cad == value) {
	  _depura ( "END BlComboBoxDelegate::id", 0, value );
	  return m_comboRecordSet->valor ( m_fieldId );
       } // end if
       m_comboRecordSet->nextRecord();
    } // end while
    

    _depura ( "END BlComboBoxDelegate::id", 0, value );
    return "";
}

///
/**
\return
**/
QString BlComboBoxDelegate::fieldValue()
{
    _depura ( "BlComboBoxDelegate::fieldValue", 0 );
    _depura ( "END BlComboBoxDelegate::fieldValue", 0 );
    return id();
}


///
/**
**/
void BlComboBoxDelegate::setQuery ( QString q )
{
    m_query = q;
}


///
/**
**/
void BlComboBoxDelegate::setTableName ( QString tableName )
{
    m_table = tableName;
}


///
/**
**/
void BlComboBoxDelegate::setFieldId ( QString fieldId )
{
    m_fieldId = fieldId;
}


///
/**
**/
void BlComboBoxDelegate::setAllowNull ( bool v )
{
    m_null = v;
}


