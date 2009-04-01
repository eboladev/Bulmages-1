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
    setDbTableName ( "controlstock" );
    setDbFieldId ( "idarticulo" );
    addSubFormHeader ( "punteocontrolstock", BlDbField::DbBoolean, BlDbField::DbNothing, BlSubFormHeader::DbNone, _( "Punteado" ) );
    addSubFormHeader ( "codigoalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _( "Codigo almacen" ) );
    addSubFormHeader ( "nomalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Nombre almacen" ) );
    addSubFormHeader ( "codigocompletoarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Codigo completo articulo" ) );
    addSubFormHeader ( "nomarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Nombre articulo" ) );
    addSubFormHeader ( "stockantcontrolstock", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite, _( "Stock anterior" ) );
    addSubFormHeader ( "stocknewcontrolstock", BlDbField::DbNumeric, BlDbField::DbRequired, BlSubFormHeader::DbNone, _( "Stock revisado" ) );
    addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNone | BlSubFormHeader::DbHideView, _( "Id articulo" ) );
    addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNone | BlSubFormHeader::DbHideView, "Id almacen" );
    addSubFormHeader ( "idinventario", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNone | BlSubFormHeader::DbHideView, "Id inventario" );
    addSubFormHeader ( "idarticulopk", BlDbField::DbInt, BlDbField::DbNoSave | BlDbField::DbDupPrimaryKey, BlSubFormHeader::DbNone | BlSubFormHeader::DbHideView | BlSubFormHeader::DbDisableView, "idarticulo" );
    addSubFormHeader ( "idalmacenpk", BlDbField::DbInt,  BlDbField::DbNoSave | BlDbField::DbDupPrimaryKey, BlSubFormHeader::DbNone | BlSubFormHeader::DbHideView | BlSubFormHeader::DbDisableView, "idalmacen" );
//    addSubFormHeader("idinventariopk", BlDbField::DbInt,  BlDbField::DbNoSave | BlDbField::DbDupPrimaryKey, BlSubFormHeader::DbNone | BlSubFormHeader::DbHideView | BlSubFormHeader::DbDisableView, "idinventario");
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
    mainCompany() ->runQuery ( "DELETE FROM controlstock WHERE idinventario = " + mdb_idinventario );
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

