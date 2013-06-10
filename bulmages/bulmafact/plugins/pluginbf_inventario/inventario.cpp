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

#include "blfile.h"
#include <QtCore/QTextStream>

#include "inventario.h"
#include "bfcompany.h"
#include "blfunctions.h"

#include "inventariosview.h"

///
/**
\param comp
\param parent
**/
Inventario::Inventario ( BfCompany *comp, QWidget *parent ) : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    companyact = comp;
    setTitleName ( _ ( "Inventario" ) );
    setDbTableName ( "inventario" );
    setDbFieldId ( "idinventario" );
    addDbField ( "idinventario", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Identificador inventario" ) );
    addDbField ( "fechainventario", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha inventario" ) );
    addDbField ( "nominventario", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre inventario" ) );
    blScript(this);
    
}


///
/**
**/
Inventario::~Inventario()
{
    BL_FUNC_DEBUG
    
}


///
/**
\return
**/
int Inventario::remove()
{
    BL_FUNC_DEBUG
    if ( dbValue ( "idinventario" ) != "" ) 
    {
	/// Vamos a borrar las lineas del inventario.
	companyact->begin();

	listalineas->remove();

	/// Vamos a borrar el resto.
	int error = companyact->runQuery ( "DELETE FROM inventario WHERE idinventario = " + dbValue ( "idinventario" ) );
        if ( error ) {
    	    companyact->rollback();
	    
            return -1;
        } // end if
        companyact->commit();
    } // end if
    
    return 0;
}



///
/**
**/
void Inventario::vaciaInventario()
{
    BL_FUNC_DEBUG
    DBclear();
    
}


///
/**
**/
void Inventario::pintaInventario()
{
    BL_FUNC_DEBUG
    pintaIdInventario ( dbValue ( "idinventario" ) );
    pintaNomInventario ( dbValue ( "nominventario" ) );
    pintaFechaInventario ( dbValue ( "fechainventario" ) );   
}


///
/**
\return
**/
int Inventario::save()
{
    BL_FUNC_DEBUG
    companyact->begin();
    try {
        QString id;
        dbSave ( id );
        setIdInventario ( id );
        listalineas->save();
        companyact->commit();
        load ( id );
        
        return 0;
    } catch ( ... ) {
	blMsgError(_("Error guardando el inventario."));
        companyact->rollback();
	
        return -1;
    } // end try
}


///
/**
\param a
**/
void Inventario::setListControlStock ( ListControlStockView *a )
{
    BL_FUNC_DEBUG
    listalineas = a;
    listalineas->setMainCompany ( companyact );
    
}


///
/**
\return
**/
ListControlStockView *Inventario::getlistalineas()
{
    BL_FUNC_DEBUG
    
    return listalineas;
}


///
/**
**/
void Inventario::pintaIdInventario ( QString )
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void Inventario::pintaFechaInventario ( QString )
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void Inventario::pintaNomInventario ( QString )
{
    BL_FUNC_DEBUG
    
}


///
/**
\param val
**/
void Inventario::setIdInventario ( QString val )
{
    BL_FUNC_DEBUG
    setDbValue ( "idinventario", val );
    listalineas->setColumnValue ( "idinventario", val );
    
}


///
/**
\param val
**/
void Inventario::setFechaInventario ( QString val )
{
    BL_FUNC_DEBUG
    setDbValue ( "fechainventario", val );
    
}


///
/**
\param val
**/
void Inventario::setNomInventario ( QString val )
{
    BL_FUNC_DEBUG
    setDbValue ( "nominventario", val );
    
}


///
/**
**/
void Inventario::imprimirInventario()
{
    BL_FUNC_DEBUG
    QString txt = "<blockTable>\n";
    txt += "<tr><td></td>\n";

    QString query = "SELECT idarticulo, codigocompeltoarticulo, nomarticulo FROM articulo ";
    BlDbRecordSet *almacenes = companyact->loadQuery ( "SELECT * FROM almacen" );
    while ( !almacenes->eof() ) {
        QString idalmacen = almacenes->value( "idalmacen" );
        query += " LEFT JOIN ( SELECT stock, idarticulo FROM stock_almacen WHERE idalmacen=" + almacenes->value( "idalmacen" ) + ") AS t" + idalmacen + " ON " + " t" + idalmacen + ".idarticulo = articulo.idarticulo";
        txt += "<td>" + almacenes->value( "nomalmacen" ) + "</td>";
        almacenes->nextRecord();
    } // end while
    delete almacenes;
    txt += "</tr>\n";

    BlDbRecordSet *cstock = companyact->loadQuery ( query );
    while ( !cstock->eof() ) {
        txt += "<tr>\n";
        txt += "<td>" + cstock->value( "nomarticulo" );
        for ( int i = 0; i < cstock->numcampos(); i++ ) {
            txt += "<td>" + cstock->value( i ) + "</td>";
        } // end for
        cstock->nextRecord();
        txt += "\n";
        txt += "</tr>\n";
    } // end while
    delete cstock;
}

