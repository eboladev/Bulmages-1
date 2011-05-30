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

#include <QKeyEvent>
#include <QEvent>
#include <QFile>
#include <QLocale>
#include <QRegExp>

#include "bldbsubform.h"
#include "blsubform.h"


/// Inicia el constructor
/**
Mantiene un contador de elementos creados para depurar la liberacion de memoria.
\param con Empresa con la que va a trabajar
\todo Se debe eliminar el contador una vez testeado el consumo de memoria
**/
BlDbSubFormRecord::BlDbSubFormRecord ( BlMainCompany *con ) : BlDbRecord ( con )
{
    blDebug ( "BlDbSubFormRecord::BlDbSubFormRecord", 0 );
    static int creaciones = 0;
    creaciones++;
    blDebug ( "END BlDbSubFormRecord::BlDbSubFormRecord", 0, "Creaciones" + QString::number ( creaciones ) );
}


/// Destruye la clase
/**
Mantiene un contador de elementos destruidos para depurar la liberacion de memoria
\todo Se debe eliminar el contador una vez testeado la correcta liberacion de memoria
**/
BlDbSubFormRecord::~BlDbSubFormRecord()
{
    static int destrucciones = 0;
    blDebug ( "BlDbSubFormRecord::~BlDbSubFormRecord", 0 );
    destrucciones++;
    blDebug ( "END BlDbSubFormRecord::~BlDbSubFormRecord", 0, "Destrucciones " + QString::number ( destrucciones ) );
}


///
/**
\param id Devuelve el identificador del registro guardado
\return Si todo ha ido bien devuelve 0
**/
int BlDbSubFormRecord::dbSave ( QString &id )
{
    blDebug ( "BlDbSubFormRecord::dbSave", 0 );
    refresh();
    blDebug ( "END BlDbSubFormRecord::dbSave", 0 );
    return BlDbRecord::dbSave ( id );
}


/// Actualiza los contenidos del subformulario acorde a los valores de la tabla
/**
Actualiza los valores del registro acorde a los valores que tiene la tabla que lo contiene
**/
void BlDbSubFormRecord::refresh()
{
    blDebug ( "BlDbSubFormRecord::refresh", 0 );
    BlDbSubFormField *camp;
    for ( int i = 0; i < m_lista.size(); ++i ) {
        camp = ( BlDbSubFormField * ) m_lista.at ( i );
        camp->refresh();
    } // end for
    blDebug ( "END BlDbSubFormRecord::refresh", 0 );
}


/// Agrega un campo al registro
/**
\param nom Nombre del campo
\param typ Tipo del campo
\param res Restricciones a aplicar en el campo
\param nomp Nombre a mostrar en caso de error
\return
**/
int BlDbSubFormRecord::addDbField ( QString nom, BlDbField::DbType typ, int res, QString nomp )
{
    blDebug ( "BlDbSubFormRecord::addDbField", 0 );
    BlDbSubFormField *camp = new BlDbSubFormField ( this, m_dbConnection, nom, typ, res, nomp );
    camp->set ( "" );
    m_lista.append ( camp );
    blDebug ( "END BlDbSubFormRecord::addDbField", 0 );
    return 0;
}


/// Constructor de BlDbSubFormField. Representa, maneja y alberga un campo de un registro de un recordset.
/**
\param par El registro padre del campo
\param com La base de datos con la que esta trabajando
\param nom El nombre del campo
\param typ El tipo de campo
\param res Las restricciones del campo
\param nomp el nombre a presentar en caso de error.
**/
BlDbSubFormField::BlDbSubFormField ( BlDbSubFormRecord *par, BlPostgreSqlClient *com, QString nom, DbType typ, int res, QString nomp )
        : BlTableWidgetItem(), BlDbField ( com, nom, typ, res, nomp )
{
    blDebug ( "BlDbSubFormField::BlDbSubFormField", 0 );
    m_pare = par;
    blDebug ( "END BlDbSubFormField::BlDbSubFormField", 0 );
}


///
/**
**/
BlDbSubFormField::~BlDbSubFormField()
{
    blDebug ( "BlDbSubFormField::~BlDbSubFormField()", 0 );
    blDebug ( "END BlDbSubFormField::~BlDbSubFormField()", 0 );
}


/// Refresco de los contenidos de un campo con lo que se esta visualizando
/**
Tiene especial intereses con los campos checkables ya que su tratamiento no es directo
**/
void BlDbSubFormField::refresh()
{
    blDebug ( "BlDbSubFormField::refresh", 0 );
    if ( this->dbFieldType() == BlDbField::DbBoolean ) {
        switch ( checkState() ) {
            case Qt::Checked:
                BlDbField::set ( "TRUE" );
                break;
            case Qt::Unchecked:
                BlDbField::set ( "FALSE" );
                break;
            case Qt::PartiallyChecked:
            default:
                /// Esta opci&oacute;n se usa si el campo SQL admite el valor nulo y por tanto tenemos un CheckBox triestado
                BlDbField::set ( "" );
                break;
        } // end switch
     } else {
        BlDbField::set ( text() );
     } // end if
    blDebug ( "END BlDbSubFormField::refresh", 0 );
}


/// Establece el valor que debe tener un campo
/**
Al introducir un valor en el campo ya se hacen las posibles conversiones que puedan interesar
para la presentacion de valores corregidos a un estandar.
\param val Valor que toma el campo
\return Si no hay errores devuelve 0
**/
int BlDbSubFormField::set ( QString val )
{
    blDebug ( "BlDbSubFormField::set", 0, fieldName() + " = " + val );

    BlDbField::set ( val );
    QRegExp importe ( "^\\d*\\.\\d{2}$" ); ///< Para emparejar los valores numericos con decimales


    if ( dbFieldType() == BlDbField::DbBoolean ) {
        if ( restrictcampo() == BlSubFormHeader::DbNoWrite ) {
            setFlags ( this->flags() & ( ~Qt::ItemIsUserCheckable ) );
        } // end if

        if ( val == "TRUE" || val == "t" ) {
            setCheckState ( Qt::Checked );
        } else if ( val == "FALSE" || val == "f" ) {
            setCheckState ( Qt::Unchecked );
        } else {
            /// El triestado si usa si el campo booleano admite el valor nulo.
            setCheckState ( Qt::PartiallyChecked );
        } // end if
    } else if ( dbFieldType() == BlDbField::DbNumeric && importe.exactMatch ( val ) ) {
        setText ( valorcampo() );
    } else if ( dbFieldType() == BlDbField::DbDate ) {
        setText ( val.left ( 10 ) );
    } else {
	  setText ( valorcampo() );

	  if ( g_confpr->value( CONF_CENTER_TEXT_FIELDS ) == "TRUE"
	  && dbFieldType() == BlDbField::DbVarChar )
	  {
		setTextAlignment(Qt::AlignCenter);
	  }
    } // end if

    blDebug ( "END BlDbSubFormField::set", 0, val );
    return 0;
}


/// Comparacion entre campos
/**
\param other El campo con el que hay que comprarar
\return TRUE si se considere este el campo mas grande, FALSE en caso contrario.
**/
bool BlDbSubFormField::operator< ( const QTableWidgetItem &other )
{
    blDebug ( "BlDbSubFormField::operator <", 0, text() );
    BlDbSubFormField *ot = ( BlDbSubFormField * ) & other;
    DbType tip = ot->dbFieldType();
    if ( tip == this->dbFieldType() ) {
        QString val = ot->valorcampo();

        if ( this->dbFieldType() == BlDbField::DbNumeric || this->dbFieldType() == BlDbField::DbInt ) {
            blDebug ( "BlDbSubFormField::operator < es del tipo numerico:", 0, this->fieldName() + QString::number ( this->dbFieldType() ) );
            double db1 = this->valorcampo().toDouble();
            double db2 = val.toDouble();
            return ( db1 < db2 );
        } // end if

        if ( this->dbFieldType() == BlDbField::DbDate ) {
            blDebug ( "BlDbSubFormField::operator < es del tipo fecha:", 0, this->fieldName() + QString::number ( this->dbFieldType() ) );
		QDate fech = blNormalizeDate ( this->valorcampo() );
            QString db1 = fech.toString ( Qt::ISODate );
		QDate fech1 = blNormalizeDate ( val );
            QString db2 = fech1.toString ( Qt::ISODate );
            return ( db1 < db2 );
        } // end if

        if ( this->dbFieldType() == BlDbField::DbVarChar ) {
            blDebug ( "BlDbSubFormField::operator < es del tipo varchar:", 0, this->fieldName() + QString::number ( this->dbFieldType() ) );
            return ( this->valorcampo() < val );
        } // end if
        blDebug ( "tipo desconocido", 0 );
    }
    blDebug ( "END BlDbSubFormField::operator <", 0, text() );
    return FALSE;
}


/// Devuelve el registro que alberga este campo
/**
Es util porque se activan signals sobre campos determinados en los que no sabremos a que registro
pertencecen
\return Puntero al registro
**/
BlDbSubFormRecord *BlDbSubFormField::pare()
{
    blDebug ( "BlDbSubFormField::pare", 0 );
    blDebug ( "END BlDbSubFormField::pare", 0 );
    return m_pare;
}

