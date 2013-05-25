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

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "contrato.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "blplugins.h"

#ifdef __NOCOMPILE


/** PReparamos la clase BlDbRecord para funcionar con la tabla contrato.
*/
/**
\param comp
\param parent
**/
Contrato::Contrato ( BfCompany *comp, QWidget *parent ) : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG

    
}


/** No requiere de acciones especiales en el destructor.
*/
/**
**/
Contrato::~Contrato()
{
    BL_FUNC_DEBUG
    
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
int Contrato::remove()
{
    BL_FUNC_DEBUG
    if ( dbValue ( "idcontrato" ) != "" ) {
        mainCompany() ->begin();
        int error = m_listalineas->remove();
        if ( error ) {
            mainCompany() ->rollback();
            return -1;
        } // end if
        error = BlDbRecord::remove();
        if ( error ) {
            mainCompany() ->rollback();
            return -1;
        } // end if
        mainCompany() ->commit();
    } // end if
    
    return 0;
}


/** Vacia la memoria que pudiera tener BlDbRecord
*/
/**
**/
void Contrato::vaciaContrato()
{
    BL_FUNC_DEBUG
    DBclear();
    
}


/** Pinta una contrato.
    Para ello utiliza los metodos sobrecargables pintaXXX de esta forma se podria generar una segunda interficie reutilizando esta clase.
*/
/// \TODO: Deberia llamarse pintar.
/**
**/
void Contrato::pintar()
{
    BL_FUNC_DEBUG
    pintaidcliente ( dbValue ( "idcliente" ) );
    pintafincontrato ( dbValue ( "fincontrato" ) );
    pintaffincontrato ( dbValue ( "ffincontrato" ) );
    pintaperiodicidadcontrato ( dbValue ( "periodicidadcontrato" ) );
    pintarefcontrato ( dbValue ( "refcontrato" ) );
    pintadescontrato ( dbValue ( "descontrato" ) );
    pintanomcontrato ( dbValue ( "nomcontrato" ) );
    pintaloccontrato ( dbValue ( "loccontrato" ) );
    /// Pintamoslas lineas
    m_listalineas->pintar();
    
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
int Contrato::load ( QString idbudget )
{
    BL_FUNC_DEBUG
    inicialize();
    QString query = "SELECT * FROM contrato WHERE idcontrato = " + idbudget;
    BlDbRecordSet * cur = mainCompany() ->loadQuery ( query );
    if ( !cur->eof() ) {
        DBload ( cur );
    } // end if
    delete cur;
    m_listalineas->load ( idbudget );
    pintar();
    
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
int Contrato::save()
{
    BL_FUNC_DEBUG
    QString fecha;
    try {
        /// Calculamos el proximo numero de contrato para poder insertarlo en caso de que este sea nulo.
        QString id;
        mainCompany() ->begin();
        dbSave ( id );
        setDbValue ( "idcontrato", id );
        m_listalineas->setColumnValue ( "idcontrato", id );
        m_listalineas->save();
        mainCompany() ->commit();

        /// Hacemos una carga para recuperar datos como la referencia
        load ( id );

        
        return 0;
    } catch ( ... ) {
        mainCompany() ->rollback();
	
        throw  - 1;
    } // end try
}


/** Funciones para ser derivadas en las clases de pintado
**/


///
/**
**/
void Contrato::pintaidcliente ( QString )
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void Contrato::pintarefcontrato ( QString )
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void Contrato::pintafincontrato ( QString )
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void Contrato::pintadescontrato ( QString )
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void Contrato::pintanomcontrato ( QString )
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void Contrato::pintaffincontrato ( QString )
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void Contrato::pintaloccontrato ( QString )
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void Contrato::pintaperiodicidadcontrato ( QString )
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void Contrato::inicialize()
{
    BL_FUNC_DEBUG
    
}


///
/**
\return
**/
QString Contrato::calculateValues()
{
    BL_FUNC_DEBUG
    
    return "";
}


#endif
