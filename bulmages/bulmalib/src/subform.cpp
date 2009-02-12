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

#include "subform.h"
#include "blsubform.h"


/// Inicia el constructor
/**
Mantiene un contador de elementos creados para depurar la liberacion de memoria.
\param con Empresa con la que va a trabajar
\todo Se debe eliminar el contador una vez testeado el consumo de memoria
**/
SDBRecord::SDBRecord ( EmpresaBase *con ) : BlDbRecord ( con )
{
    _depura ( "SDBRecord::SDBRecord", 0 );
    static int creaciones = 0;
    creaciones++;
    _depura ( "END SDBRecord::SDBRecord", 0, "Creaciones" + QString::number ( creaciones ) );
}


/// Destruye la clase
/**
Mantiene un contador de elementos destruidos para depurar la liberacion de memoria
\todo Se debe eliminar el contador una vez testeado la correcta liberacion de memoria
**/
SDBRecord::~SDBRecord()
{
    static int destrucciones = 0;
    _depura ( "SDBRecord::~SDBRecord", 0 );
    destrucciones++;
    _depura ( "END SDBRecord::~SDBRecord", 0, "Destrucciones " + QString::number ( destrucciones ) );
}


///
/**
\param id Devuelve el identificador del registro guardado
\return Si todo ha ido bien devuelve 0
**/
int SDBRecord::DBsave ( QString &id )
{
    _depura ( "SDBRecord::DBsave", 0 );
    refresh();
    _depura ( "END SDBRecord::DBsave", 0 );
    return BlDbRecord::DBsave ( id );
}


/// Actualiza los contenidos del subformulario acorde a los valores de la tabla
/**
Actualiza los valores del registro acorde a los valores que tiene la tabla que lo contiene
**/
void SDBRecord::refresh()
{
    _depura ( "SDBRecord::refresh", 0 );
    SDBCampo *camp;
    for ( int i = 0; i < m_lista.size(); ++i ) {
        camp = ( SDBCampo * ) m_lista.at ( i );
        camp->refresh();
    } // end for
    _depura ( "END SDBRecord::refresh", 0 );
}


/// Agrega un campo al registro
/**
\param nom Nombre del campo
\param typ Tipo del campo
\param res Restricciones a aplicar en el campo
\param nomp Nombre a mostrar en caso de error
\return
**/
int SDBRecord::addDBCampo ( QString nom, BlDbField::dbtype typ, int res, QString nomp )
{
    _depura ( "SDBRecord::addDBCampo", 0 );
    SDBCampo *camp = new SDBCampo ( this, m_conexionbase, nom, typ, res, nomp );
    camp->set ( "" );
    m_lista.append ( camp );
    _depura ( "END SDBRecord::addDBCampo", 0 );
    return 0;
}


/// Constructor de SDBCampo. Representa, maneja y alberga un campo de un registro de un recordset.
/**
\param par El registro padre del campo
\param com La base de datos con la que esta trabajando
\param nom El nombre del campo
\param typ El tipo de campo
\param res Las restricciones del campo
\param nomp el nombre a presentar en caso de error.
**/
SDBCampo::SDBCampo ( SDBRecord *par, BlPostgreSqlClient *com, QString nom, dbtype typ, int res, QString nomp )
        : QTableWidgetItem2(), BlDbField ( com, nom, typ, res, nomp )
{
    _depura ( "SDBCampo::SDBCampo", 0 );
    m_pare = par;
    _depura ( "END SDBCampo::SDBCampo", 0 );
}


///
/**
**/
SDBCampo::~SDBCampo()
{
    _depura ( "SDBCampo::~SDBCampo()", 0 );
    _depura ( "END SDBCampo::~SDBCampo()", 0 );
}


/// Refresco de los contenidos de un campo con lo que se esta visualizando
/**
Tiene especial intereses con los campos checkables ya que su tratamiento no es directo
**/
void SDBCampo::refresh()
{
    _depura ( "SDBCampo::refresh", 0 );
    if ( this->tipo() == BlDbField::DBboolean )
        BlDbField::set ( checkState() == Qt::Checked ? "TRUE" : "FALSE" );
    else
        BlDbField::set ( text() );
    // end if
    _depura ( "END SDBCampo::refresh", 0 );
}


/// Establece el valor que debe tener un campo
/**
Al introducir un valor en el campo ya se hacen las posibles conversiones que puedan interesar
para la presentacion de valores corregidos a un estandar.
\param val Valor que toma el campo
\return Si no hay errores devuelve 0
**/
int SDBCampo::set ( QString val )
{
    _depura ( "SDBCampo::set", 0, nomcampo() + " = " + val );
    BlDbField::set ( val );
    QRegExp importe ( "^\\d*\\.\\d{2}$" ); ///< Para emparejar los valores numericos con decimales
    if ( tipo() == BlDbField::DBboolean ) {
        if ( restrictcampo() == BlSubFormHeader::DBNoWrite ) {
            setFlags ( this->flags() & ( ~Qt::ItemIsUserCheckable ) );
        } // end if
        if ( val == "TRUE" || val == "t" ) {
            setCheckState ( Qt::Checked );
        } else {
            setCheckState ( Qt::Unchecked );
        } // end if
    } else if ( tipo() == BlDbField::DBnumeric && importe.exactMatch ( val ) ) {
        setText ( valorcampo() );
    } else if ( tipo() == BlDbField::DBdate ) {
        setText ( val.left ( 10 ) );
    } else {
        setText ( valorcampo() );
    } // end if

    _depura ( "END SDBCampo::set", 0, val );
    return 0;
}


/// Comparacion entre campos
/**
\param other El campo con el que hay que comprarar
\return TRUE si se considere este el campo mas grande, FALSE en caso contrario.
**/
bool SDBCampo::operator< ( const QTableWidgetItem &other )
{
    _depura ( "SDBCampo::operator <", 0, text() );
    SDBCampo *ot = ( SDBCampo * ) & other;
    dbtype tip = ot->tipo();
    if ( tip == this->tipo() ) {
        QString val = ot->valorcampo();

        if ( this->tipo() == BlDbField::DBnumeric || this->tipo() == BlDbField::DBint ) {
            _depura ( "SDBCampo::operator < es del tipo numerico:", 0, this->nomcampo() + QString::number ( this->tipo() ) );
            double db1 = this->valorcampo().toDouble();
            double db2 = val.toDouble();
            return ( db1 < db2 );
        } // end if

        if ( this->tipo() == BlDbField::DBdate ) {
            _depura ( "SDBCampo::operator < es del tipo fecha:", 0, this->nomcampo() + QString::number ( this->tipo() ) );
            QDate fech = normalizafecha ( this->valorcampo() );
            QString db1 = fech.toString ( Qt::ISODate );
            QDate fech1 = normalizafecha ( val );
            QString db2 = fech1.toString ( Qt::ISODate );
            return ( db1 < db2 );
        } // end if

        if ( this->tipo() == BlDbField::DBvarchar ) {
            _depura ( "SDBCampo::operator < es del tipo varchar:", 0, this->nomcampo() + QString::number ( this->tipo() ) );
            return ( this->valorcampo() < val );
        } // end if
        _depura ( "tipo desconocido", 0 );
    }
    _depura ( "END SDBCampo::operator <", 0, text() );
    return FALSE;
}


/// Devuelve el registro que alberga este campo
/**
Es util porque se activan signals sobre campos determinados en los que no sabremos a que registro
pertencecen
\return Puntero al registro
**/
SDBRecord *SDBCampo::pare()
{
    _depura ( "SDBCampo::pare", 0 );
    _depura ( "END SDBCampo::pare", 0 );
    return m_pare;
}


