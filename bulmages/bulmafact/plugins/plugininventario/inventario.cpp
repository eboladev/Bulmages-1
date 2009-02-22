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


///
/**
\param comp
\param parent
**/
Inventario::Inventario ( BfCompany *comp, QWidget *parent ) : FichaBf ( comp, parent )
{
    _depura ( "Inventario::Inventario", 0 );
    companyact = comp;
    setTitleName ( _( "Inventario" ) );
    setDbTableName ( "inventario" );
    setDbFieldId ( "idinventario" );
    addDbField ( "idinventario", BlDbField::DBint, BlDbField::DBPrimaryKey, _( "Identificador inventario" ) );
    addDbField ( "fechainventario", BlDbField::DBdate, BlDbField::DBNothing, _( "Fecha inventario" ) );
    addDbField ( "nominventario", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Nombre inventario" ) );
    _depura ( "END Inventario::Inventario", 0 );
}


///
/**
**/
Inventario::~Inventario()
{
    _depura ( "Inventario::~Inventario", 0 );
    _depura ( "END Inventario::~Inventario", 0 );
}


///
/**
\return
**/
int Inventario::borrar()
{
    if ( DBvalue ( "idinventario" ) != "" ) {
        _depura ( "vamos a borrar las lineas del inventario", 0 );
        companyact->begin();

        listalineas->borrar();
        _depura ( "Vamos a borrar el resto", 0 );
        int error = companyact->runQuery ( "DELETE FROM inventario WHERE idinventario = " + DBvalue ( "idinventario" ) );
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
    _depura ( "Inventario::vaciaInventario", 0 );
    DBclear();
    _depura ( "END Inventario::vaciaInventario", 0 );
}


///
/**
**/
void Inventario::pintaInventario()
{
    _depura ( "pintaInventario\n", 0 );
    pintaidinventario ( DBvalue ( "idinventario" ) );
    pintanominventario ( DBvalue ( "nominventario" ) );
    pintafechainventario ( DBvalue ( "fechainventario" ) );
    /// Pinta el subformulario de detalle del Inventario.
    //listalineas->pintaListControlStock();
}





///
/**
\return
**/
int Inventario::guardar()
{
    _depura ( "Inventario::guardar()", 0 );
    companyact->begin();
    try {
        QString id;
        DBsave ( id );
        setidinventario ( id );
        listalineas->guardar();
        companyact->commit();
        cargar ( id );
        _depura ( "END Inventario::guardar()", 0 );
        return 0;
    } catch ( ... ) {
        _depura ( "Error guardando el inventario", 2 );
        companyact->rollback();
        _depura ( "END Inventario::guardar()", 0 );
        return -1;
    } // end try
}


///
/**
**/
void Inventario::pregenerar()
{
    _depura ( "Inventario::pregenerar", 0 );
    listalineas->pregenerar();
    _depura ( "END Inventario::pregenerar", 0 );
}


///
/**
\param a
**/
void Inventario::setListControlStock ( ListControlStockView *a )
{
    _depura ( "Inventario::setListControlStock", 0 );
    listalineas = a;
    listalineas->setMainCompany ( companyact );
    _depura ( "END Inventario::setListControlStock", 0 );
}


///
/**
\return
**/
ListControlStockView *Inventario::getlistalineas()
{
    _depura ( "Inventario::getlistalineas", 0 );
    _depura ( "Inventario::getlistalineas", 0 );
    return listalineas;
}


///
/**
**/
void Inventario::pintaidinventario ( QString )
{
    _depura ( "Inventario::pintaidinventario", 0 );
    _depura ( "END Inventario::pintaidinventario", 0 );
}


///
/**
**/
void Inventario::pintafechainventario ( QString )
{
    _depura ( "Inventario::pintafechainventario", 0 );
    _depura ( "END Inventario::pintafechainventario", 0 );
}


///
/**
**/
void Inventario::pintanominventario ( QString )
{
    _depura ( "Inventario::pintanominventario", 0 );
    _depura ( "END Inventario::pintanominventario", 0 );
}


///
/**
\param val
**/
void Inventario::setidinventario ( QString val )
{
    _depura ( "Inventario::setidinventario", 0 );
    setDbValue ( "idinventario", val );
    listalineas->setColumnValue ( "idinventario", val );
    _depura ( "END Inventario::setidinventario", 0 );
}


///
/**
\param val
**/
void Inventario::setfechainventario ( QString val )
{
    _depura ( "Inventario::setfechainventario", 0 );
    setDbValue ( "fechainventario", val );
    _depura ( "END Inventario::setfechainventario", 0 );
}


///
/**
\param val
**/
void Inventario::setnominventario ( QString val )
{
    _depura ( "Inventario::setnominventario", 0 );
    setDbValue ( "nominventario", val );
    _depura ( "END Inventario::setnominventario", 0 );
}


///
/**
**/
void Inventario::imprimirInventario()
{
    _depura ( "Inventario::imprimirInventario", 0 );
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

    _depura ( txt, 2 );
    _depura ( "END Inventario::imprimirInventario", 0 );
}

