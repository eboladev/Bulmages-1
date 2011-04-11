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
    blDebug ( "Inventario::Inventario", 0 );
    companyact = comp;
    setTitleName ( _ ( "Inventario" ) );
    setDbTableName ( "inventario" );
    setDbFieldId ( "idinventario" );
    addDbField ( "idinventario", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Identificador inventario" ) );
    addDbField ( "fechainventario", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha inventario" ) );
    addDbField ( "nominventario", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre inventario" ) );
    blScript(this);
    blDebug ( "END Inventario::Inventario", 0 );
}


///
/**
**/
Inventario::~Inventario()
{
    blDebug ( "Inventario::~Inventario", 0 );
    blDebug ( "END Inventario::~Inventario", 0 );
}


///
/**
\return
**/
int Inventario::borrar()
{
    blDebug ( "Inventario::borrar", 0 );
    if ( dbValue ( "idinventario" ) != "" ) 
    {
	blDebug ( "vamos a borrar las lineas del inventario", 0 );
	companyact->begin();

	listalineas->borrar();
	blDebug ( "Vamos a borrar el resto", 0 );
	int error = companyact->runQuery ( "DELETE FROM inventario WHERE idinventario = " + dbValue ( "idinventario" ) );
        if ( error ) {
    	    companyact->rollback();
            return -1;
        } // end if
        companyact->commit();
    } // end if
    return 0;
    presentar();
    blDebug ( "END Inventario::borrar", 0 );
}



///
/**
**/
void Inventario::vaciaInventario()
{
    blDebug ( "Inventario::vaciaInventario", 0 );
    DBclear();
    blDebug ( "END Inventario::vaciaInventario", 0 );
}


///
/**
**/
void Inventario::pintaInventario()
{
    blDebug ( "pintaInventario\n", 0 );
    pintaidinventario ( dbValue ( "idinventario" ) );
    pintanominventario ( dbValue ( "nominventario" ) );
    pintafechainventario ( dbValue ( "fechainventario" ) );
}





///
/**
\return
**/
int Inventario::guardar()
{
    blDebug ( "Inventario::guardar()", 0 );
    companyact->begin();
    try {
        QString id;
        DBsave ( id );
        setidinventario ( id );
        listalineas->guardar();
        companyact->commit();
        cargar ( id );
        blDebug ( "END Inventario::guardar()", 0 );
        return 0;
    } catch ( ... ) {
        blDebug ( "Error guardando el inventario", 2 );
        companyact->rollback();
        blDebug ( "END Inventario::guardar()", 0 );
        return -1;
    } // end try
}


///
/**
**/
void Inventario::pregenerar()
{
    blDebug ( "Inventario::pregenerar", 0 );
    listalineas->pregenerar();
    blDebug ( "END Inventario::pregenerar", 0 );
}


///
/**
\param a
**/
void Inventario::setListControlStock ( ListControlStockView *a )
{
    blDebug ( "Inventario::setListControlStock", 0 );
    listalineas = a;
    listalineas->setMainCompany ( companyact );
    blDebug ( "END Inventario::setListControlStock", 0 );
}


///
/**
\return
**/
ListControlStockView *Inventario::getlistalineas()
{
    blDebug ( "Inventario::getlistalineas", 0 );
    blDebug ( "Inventario::getlistalineas", 0 );
    return listalineas;
}


///
/**
**/
void Inventario::pintaidinventario ( QString )
{
    blDebug ( "Inventario::pintaidinventario", 0 );
    blDebug ( "END Inventario::pintaidinventario", 0 );
}


///
/**
**/
void Inventario::pintafechainventario ( QString )
{
    blDebug ( "Inventario::pintafechainventario", 0 );
    blDebug ( "END Inventario::pintafechainventario", 0 );
}


///
/**
**/
void Inventario::pintanominventario ( QString )
{
    blDebug ( "Inventario::pintanominventario", 0 );
    blDebug ( "END Inventario::pintanominventario", 0 );
}


///
/**
\param val
**/
void Inventario::setidinventario ( QString val )
{
    blDebug ( "Inventario::setidinventario", 0 );
    setDbValue ( "idinventario", val );
    listalineas->setColumnValue ( "idinventario", val );
    blDebug ( "END Inventario::setidinventario", 0 );
}


///
/**
\param val
**/
void Inventario::setfechainventario ( QString val )
{
    blDebug ( "Inventario::setfechainventario", 0 );
    setDbValue ( "fechainventario", val );
    blDebug ( "END Inventario::setfechainventario", 0 );
}


///
/**
\param val
**/
void Inventario::setnominventario ( QString val )
{
    blDebug ( "Inventario::setnominventario", 0 );
    setDbValue ( "nominventario", val );
    blDebug ( "END Inventario::setnominventario", 0 );
}


///
/**
**/
void Inventario::imprimirInventario()
{
    blDebug ( "Inventario::imprimirInventario", 0 );
    QString txt = "<blockTable>\n";
    txt += "<tr><td></td>\n";

    QString query = "SELECT idarticulo, codigocompeltoarticulo, nomarticulo FROM articulo ";
    BlDbRecordSet *almacenes = companyact->loadQuery ( "SELECT * FROM almacen" );
    while ( !almacenes->eof() ) {
        QString idalmacen = almacenes->valor ( "idalmacen" );
        query += " LEFT JOIN ( SELECT stock, idarticulo FROM stock_almacen WHERE idalmacen=" + almacenes->valor ( "idalmacen" ) + ") AS t" + idalmacen + " ON " + " t" + idalmacen + ".idarticulo = articulo.idarticulo";
        txt += "<td>" + almacenes->valor ( "nomalmacen" ) + "</td>";
        almacenes->nextRecord();
    } // end while
    delete almacenes;
    txt += "</tr>\n";

    BlDbRecordSet *cstock = companyact->loadQuery ( query );
    while ( !cstock->eof() ) {
        txt += "<tr>\n";
        txt += "<td>" + cstock->valor ( "nomarticulo" );
        for ( int i = 0; i < cstock->numcampos(); i++ ) {
            txt += "<td>" + cstock->valor ( i ) + "</td>";
        } // end for
        cstock->nextRecord();
        txt += "\n";
        txt += "</tr>\n";
    } // end while
    delete cstock;

    blDebug ( txt, 2 );
    blDebug ( "END Inventario::imprimirInventario", 0 );
}

