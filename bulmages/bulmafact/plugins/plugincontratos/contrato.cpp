/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#include <QFile>
#include <QTextStream>

#include "contrato.h"
#include "company.h"
#include "blfunctions.h"
#include "plugins.h"


/** PReparamos la clase BlDbRecord para funcionar con la tabla contrato.
*/
/**
\param comp
\param parent
**/
Contrato::Contrato ( Company *comp, QWidget *parent ) : FichaBf ( comp, parent )
{
    _depura ( "Contrato::Contrato", 0 );
    setTitleName ( _( "Contrato" ) );
    setDBTableName ( "contrato" );
    setDBCampoId ( "idcontrato" );
    addDBCampo ( "idcontrato",  BlDbField::DBint, BlDbField::DBPrimaryKey, _( "Id contrato" ) );
    addDBCampo ( "idcliente",   BlDbField::DBint, BlDbField::DBNotNull, _( "Id cliente" ) );
    addDBCampo ( "refcontrato", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Referencia contrato" ) );
    addDBCampo ( "descontrato", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Descripcion contrato" ) );
    addDBCampo ( "nomcontrato", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Descripcion contrato" ) );
    addDBCampo ( "loccontrato", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Descripcion contrato" ) );
    addDBCampo ( "periodicidadcontrato", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Descripcion contrato" ) );
    addDBCampo ( "fincontrato",  BlDbField::DBdate, BlDbField::DBNothing, _( "Descripcion contrato" ) );
    addDBCampo ( "ffincontrato", BlDbField::DBdate, BlDbField::DBNothing, _( "Descripcion contrato" ) );
    _depura ( "END Contrato::Contrato", 0 );
}


/** No requiere de acciones especiales en el destructor.
*/
/**
**/
Contrato::~Contrato()
{
    _depura ( "Contrato::~Contrato", 0 );
    _depura ( "END Contrato::~Contrato", 0 );
}


/** Se encarga del borrado de una contrato.
    Para ello primero borra las lineas y descuentos de esta. y luego
    Borra la ficha de la contrato.
    Sit odo va bien devuelve 0, sino devuelve -1.
*/
/// \TODO: Deberia contener un bloque try{} catch{}
/**
\return
**/
int Contrato::borrar()
{
    _depura ( "Contrato::borrar", 0 );
    if ( DBvalue ( "idcontrato" ) != "" ) {
        empresaBase() ->begin();
        int error = m_listalineas->borrar();
        if ( error ) {
            empresaBase() ->rollback();
            return -1;
        } // end if
        error = BlDbRecord::borrar();
        if ( error ) {
            empresaBase() ->rollback();
            return -1;
        } // end if
        empresaBase() ->commit();
    } // end if
    _depura ( "END Contrato::borrar", 0 );
    return 0;
}


/** Vacia la memoria que pudiera tener BlDbRecord
*/
/**
**/
void Contrato::vaciaContrato()
{
    _depura ( "Contrato::vaciaContrato", 0 );
    DBclear();
    _depura ( "END Contrato::vaciaContrato", 0 );
}


/** Pinta una contrato.
    Para ello utiliza los metodos sobrecargables pintaXXX de esta forma se podria generar una segunda interficie reutilizando esta clase.
*/
/// \TODO: Deberia llamarse pintar.
/**
**/
void Contrato::pintar()
{
    _depura ( "pintaContrato", 0 );
    pintaidcliente ( DBvalue ( "idcliente" ) );
    pintafincontrato ( DBvalue ( "fincontrato" ) );
    pintaffincontrato ( DBvalue ( "ffincontrato" ) );
    pintaperiodicidadcontrato ( DBvalue ( "periodicidadcontrato" ) );
    pintarefcontrato ( DBvalue ( "refcontrato" ) );
    pintadescontrato ( DBvalue ( "descontrato" ) );
    pintanomcontrato ( DBvalue ( "nomcontrato" ) );
    pintaloccontrato ( DBvalue ( "loccontrato" ) );
    /// Pintamoslas lineas
    m_listalineas->pintar();
    _depura ( "END pintaContrato", 0 );
}


/// Esta funcion carga un contrato.
/** Carga una contrato.
    Crea el query de carga y delega el llenodo de registros a BlDbRecord.
    Una vez cargado BlDbRecord hace una llamada a la carga de lineas y descuentos.
    Tras el cargado se hace un pintado para que se vea bien todo.
    Si todo va bien devuelve 0.
*/
/// \TODO: Deberia utilizar un bloque try{} catch{}
/**
\param idbudget
\return
**/
int Contrato::cargar ( QString idbudget )
{
    _depura ( "Contrato::cargar", 0 );
    inicialize();
    QString query = "SELECT * FROM contrato WHERE idcontrato = " + idbudget;
    BlDbRecordSet * cur = empresaBase() ->cargacursor ( query );
    if ( !cur->eof() ) {
        DBload ( cur );
    } // end if
    delete cur;
    m_listalineas->cargar ( idbudget );
    pintar();
    _depura ( "END Contrato::cargar", 0 );
    return 0;
}


/** Se encarga del guardado de una contrato.
    Primero busca el numero de contrato que hace falta.
    Luego guarda el registro.
    Despues guarda las lineas y descuentos.
    Al finalizar el guardado hace un cargar para repintar todos los elementos.
    Si todo va bien devuelve 0
    Si se produce un error genera una excepcion -1
*/
/**
\return
**/
int Contrato::guardar()
{
    _depura ( "Contrato::guardar", 0 );
    QString fecha;
    try {
        /// Calculamos el proximo numero de contrato para poder insertarlo en caso de que este sea nulo.
        QString id;
        empresaBase() ->begin();
        DBsave ( id );
        setDBvalue ( "idcontrato", id );
        m_listalineas->setColumnValue ( "idcontrato", id );
        m_listalineas->guardar();
        empresaBase() ->commit();

        /// Hacemos una carga para recuperar datos como la referencia
        cargar ( id );

        _depura ( "END Contrato::guardar", 0 );
        return 0;
    } catch ( ... ) {
        _depura ( "Contrato::guardar() se produjo un error guardando la contrato", 0 );
        empresaBase() ->rollback();
        throw  - 1;
    } // end try
}


/** Devuelve el puntero a la clase company con la que esta trabajando la contrato
**/
/**
\return
**/
Company * Contrato::_company()
{
    _depura ( "Contrato::_company", 0 );
    _depura ( "END Contrato::_company", 0 );
    return empresaBase();
}


/** Funciones para ser derivadas en las clases de pintado
**/


///
/**
**/
void Contrato::pintaidcliente ( QString )
{
    _depura ( "Contrato::pintaidcliente", 0 );
    _depura ( "END Contrato::pintaidcliente", 0 );
}


///
/**
**/
void Contrato::pintarefcontrato ( QString )
{
    _depura ( "Contrato::pintarefcontrato", 0 );
    _depura ( "Contrato::pintarefcontrato", 0 );
}


///
/**
**/
void Contrato::pintafincontrato ( QString )
{
    _depura ( "Contrato::pintafincontrato", 0 );
    _depura ( "END Contrato::pintafincontrato", 0 );
}


///
/**
**/
void Contrato::pintadescontrato ( QString )
{
    _depura ( "Contrato::pintadescontrato", 0 );
    _depura ( "END Contrato::pintadescontrato", 0 );
}


///
/**
**/
void Contrato::pintanomcontrato ( QString )
{
    _depura ( "Contrato::pintanomcontrato", 0 );
    _depura ( "END Contrato::pintanomcontrato", 0 );
}


///
/**
**/
void Contrato::pintaffincontrato ( QString )
{
    _depura ( "Contrato::pintaffincontrato", 0 );
    _depura ( "END Contrato::pintaffincontrato", 0 );
}


///
/**
**/
void Contrato::pintaloccontrato ( QString )
{
    _depura ( "Contrato::pintaloccontrato", 0 );
    _depura ( "END Contrato::pintaloccontrato", 0 );
}


///
/**
**/
void Contrato::pintaperiodicidadcontrato ( QString )
{
    _depura ( "Contrato::pintaperiodicidadcontrato", 0 );
    _depura ( "END Contrato::pintaperiodicidadcontrato", 0 );
}


///
/**
**/
void Contrato::inicialize()
{
    _depura ( "Contrato::inicialize", 0 );
    _depura ( "END Contrato::inicialize", 0 );
}


///
/**
\return
**/
QString Contrato::calculateValues()
{
    _depura ( "Contrato::calculateValues", 0 );
    _depura ( "END Contrato::calculateValues", 0 );
    return "";
}



