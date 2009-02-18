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

#include "listcontrolstockview.h"


///
/**
\param parent
**/
ListControlStockView::ListControlStockView ( QWidget *parent, const char * )
        : BfSubForm ( parent )
{
    _depura ( "ListControlStockView::ListControlStockView", 0 );
    setDBTableName ( "controlstock" );
    setDBCampoId ( "idarticulo" );
    addSubFormHeader ( "punteocontrolstock", BlDbField::DBboolean, BlDbField::DBNothing, BlSubFormHeader::DBNone, _( "Punteado" ) );
    addSubFormHeader ( "codigoalmacen", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Codigo almacen" ) );
    addSubFormHeader ( "nomalmacen", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Nombre almacen" ) );
    addSubFormHeader ( "codigocompletoarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Codigo completo articulo" ) );
    addSubFormHeader ( "nomarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Nombre articulo" ) );
    addSubFormHeader ( "stockantcontrolstock", BlDbField::DBnumeric, BlDbField::DBNothing, BlSubFormHeader::DBNoWrite, _( "Stock anterior" ) );
    addSubFormHeader ( "stocknewcontrolstock", BlDbField::DBnumeric, BlDbField::DBRequired, BlSubFormHeader::DBNone, _( "Stock revisado" ) );
    addSubFormHeader ( "idarticulo", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoView, _( "Id articulo" ) );
    addSubFormHeader ( "idalmacen", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoView, "Id almacen" );
    addSubFormHeader ( "idinventario", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoView, "Id inventario" );
    addSubFormHeader ( "idarticulopk", BlDbField::DBint, BlDbField::DBNoSave | BlDbField::DBDupPrimaryKey, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoView | BlSubFormHeader::DBBlockView, "idarticulo" );
    addSubFormHeader ( "idalmacenpk", BlDbField::DBint,  BlDbField::DBNoSave | BlDbField::DBDupPrimaryKey, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoView | BlSubFormHeader::DBBlockView, "idalmacen" );
//    addSubFormHeader("idinventariopk", BlDbField::DBint,  BlDbField::DBNoSave | BlDbField::DBDupPrimaryKey, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoView | BlSubFormHeader::DBBlockView, "idinventario");
    setinsercion ( FALSE );
    _depura ( "END ListControlStockView::ListControlStockView", 0 );
}


///
/**
\param idinventario
**/
/*
void ListControlStockView::cargar ( int idinventario )
{
    _depura ( "ListControlStockView::cargar", 0 );
    mdb_idinventario = QString::number(idinventario);
    QString SQLQuery = "SELECT * FROM ";
    SQLQuery += " (SELECT idarticulo, idalmacen, nomarticulo, nomalmacen, codigocompletoarticulo, codigoalmacen FROM articulo, almacen) AS t1 ";
    SQLQuery += " LEFT JOIN (SELECT punteocontrolstock,stockantcontrolstock, stocknewcontrolstock, idarticulo AS idarticulopk, idalmacen AS idalmacenpk, idinventario FROM controlstock WHERE idinventario = " + mdb_idinventario + ") AS t2 ON t1.idarticulo = t2.idarticulopk AND t1.idalmacen = t2.idalmacenpk ";

    SQLQuery += " ORDER BY codigoalmacen, codigocompletoarticulo";
    BfSubForm::cargar ( SQLQuery );
    _depura ( "END ListControlStockView::cargar", 0 );
}
*/

///
/**
\return
**/
int ListControlStockView::borrar()
{
    _depura ( "ListControlStockView::borrar", 0 );
    empresaBase() ->ejecuta ( "DELETE FROM controlstock WHERE idinventario = " + mdb_idinventario );
    _depura ( "END ListControlStockView::borrar", 0 );
    return 0;
}


///
/**
**/
void ListControlStockView::pregenerar()
{
    _depura ( "ListControlStockView::pregenerar", 0 );
    QString query;
    query = "SELECT * FROM (SELECT * FROM articulo, almacen) AS t1 LEFT JOIN (SELECT stocknewcontrolstock, idarticulo AS idarticulopk, idalmacen AS idalmacenpk, idinventario AS idinventariopk FROM controlstock WHERE idinventario = 1) AS t2 ON t1.idarticulo = t2.idarticulopk AND t1.idalmacen = t2.idalmacenpk ORDER BY codigoalmacen, codigocompletoarticulo;";
//    BlSubForm::cargar ( query );
    _depura ( "END ListControlStockView::pregenerar", 0 );
}


///
/**
**/
ListControlStockView::~ListControlStockView()
{
    _depura ( "ListControlStockView::~ListControlStockView", 0 );
    _depura ( "END ListControlStockView::~ListControlStockView", 0 );
}


///
/**
\return
**/
int ListControlStockView::guardar()
{
    _depura ( "ListControlStockView::guardar", 0 );
    BfSubForm::guardar();
    _depura ( "END ListControlStockView::guardar", 0 );
    return 0;
}

