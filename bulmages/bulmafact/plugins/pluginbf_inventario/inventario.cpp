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

#include <QFile>
#include <QTextStream>

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
    blDebug ( Q_FUNC_INFO, 0 );
    companyact = comp;
    setTitleName ( _ ( "Inventario" ) );
    setDbTableName ( "inventario" );
    setDbFieldId ( "idinventario" );
    addDbField ( "idinventario", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Identificador inventario" ) );
    addDbField ( "fechainventario", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha inventario" ) );
    addDbField ( "nominventario", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre inventario" ) );
    blScript(this);
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
Inventario::~Inventario()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\return
**/
int Inventario::remove()
{
    blDebug ( Q_FUNC_INFO, 0 );
    if ( dbValue ( "idinventario" ) != "" ) 
    {
	/// Vamos a borrar las lineas del inventario.
	companyact->begin();

	listalineas->remove();

	/// Vamos a borrar el resto.
	int error = companyact->runQuery ( "DELETE FROM inventario WHERE idinventario = " + dbValue ( "idinventario" ) );
        if ( error ) {
    	    companyact->rollback();
	    blDebug ( ("END ", Q_FUNC_INFO), 0, _("Error al borrar") );
            return -1;
        } // end if
        companyact->commit();
    } // end if
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return 0;
}



///
/**
**/
void Inventario::vaciaInventario()
{
    blDebug ( Q_FUNC_INFO, 0 );
    DBclear();
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
void Inventario::pintaInventario()
{
    blDebug ( Q_FUNC_INFO, 0 );
    pintaIdInventario ( dbValue ( "idinventario" ) );
    pintaNomInventario ( dbValue ( "nominventario" ) );
    pintaFechaInventario ( dbValue ( "fechainventario" ) );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}





///
/**
\return
**/
int Inventario::save()
{
    blDebug ( Q_FUNC_INFO, 0 );
    companyact->begin();
    try {
        QString id;
        dbSave ( id );
        setIdInventario ( id );
        listalineas->save();
        companyact->commit();
        load ( id );
        blDebug ( ("END ", Q_FUNC_INFO), 0 );
        return 0;
    } catch ( ... ) {
	blMsgError(_("Error guardando el inventario."));
        companyact->rollback();
	blDebug ( ("END ", Q_FUNC_INFO), 0, _("Error guardando el inventario.") );
        return -1;
    } // end try
}


///
/**
**/
void Inventario::pregenerar()
{
    blDebug ( Q_FUNC_INFO, 0 );
    listalineas->pregenerar();
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param a
**/
void Inventario::setListControlStock ( ListControlStockView *a )
{
    blDebug ( Q_FUNC_INFO, 0 );
    listalineas = a;
    listalineas->setMainCompany ( companyact );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\return
**/
ListControlStockView *Inventario::getlistalineas()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return listalineas;
}


///
/**
**/
void Inventario::pintaIdInventario ( QString )
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
void Inventario::pintaFechaInventario ( QString )
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
void Inventario::pintaNomInventario ( QString )
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param val
**/
void Inventario::setIdInventario ( QString val )
{
    blDebug ( Q_FUNC_INFO, 0 );
    setDbValue ( "idinventario", val );
    listalineas->setColumnValue ( "idinventario", val );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param val
**/
void Inventario::setFechaInventario ( QString val )
{
    blDebug ( Q_FUNC_INFO, 0 );
    setDbValue ( "fechainventario", val );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param val
**/
void Inventario::setNomInventario ( QString val )
{
    blDebug ( Q_FUNC_INFO, 0 );
    setDbValue ( "nominventario", val );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
void Inventario::imprimirInventario()
{
    blDebug ( Q_FUNC_INFO, 0 );
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

    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}

