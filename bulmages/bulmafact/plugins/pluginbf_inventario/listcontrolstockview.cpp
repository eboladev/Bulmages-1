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
    blDebug ( Q_FUNC_INFO, 0 );
    setDbTableName ( "controlstock" );
    setDbFieldId ( "idarticulo" );
    addSubFormHeader ( "punteocontrolstock", BlDbField::DbBoolean, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Punteado" ) );
    addSubFormHeader ( "codigoalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "Codigo almacen" ) );
    addSubFormHeader ( "nomalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre almacen" ) );
    addSubFormHeader ( "codigocompletoarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Codigo completo articulo" ) );
    addSubFormHeader ( "nomarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre articulo" ) );
    addSubFormHeader ( "stockantcontrolstock", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Stock anterior" ) );
    addSubFormHeader ( "stocknewcontrolstock", BlDbField::DbNumeric, BlDbField::DbRequired, BlSubFormHeader::DbNone, _ ( "Stock revisado" ) );
    addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Id articulo" ) );
    addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, "Id almacen" );
    addSubFormHeader ( "idinventario", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, "Id inventario" );
    addSubFormHeader ( "idarticulopk", BlDbField::DbInt, BlDbField::DbNoSave | BlDbField::DbDupPrimaryKey, BlSubFormHeader::DbNone | BlSubFormHeader::DbHideView | BlSubFormHeader::DbDisableView, "idarticulo" );
    addSubFormHeader ( "idalmacenpk", BlDbField::DbInt,  BlDbField::DbNoSave | BlDbField::DbDupPrimaryKey, BlSubFormHeader::DbNone | BlSubFormHeader::DbHideView | BlSubFormHeader::DbDisableView, "idalmacen" );
    setInsert ( FALSE );
    mdb_idinventario = "";
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\return
**/
/*
int ListControlStockView::remove()
{
//    blDebug ( Q_FUNC_INFO, 0 );
    fprintf(stderr, "f");
    mdb_idinventario = dbValue("idinventario");
///zhixiong    mainCompany() ->runQuery ( "DELETE FROM inventario WHERE idinventario = " +  dbValue ( "idinventario" ) );
    QString temp = QString("--") + dbValue("idinventario") + " - " + dbValue("idarticulo") + QString("--");
    fprintf(stderr, temp.toAscii());
//    mainCompany() ->runQuery ( "DELETE FROM controlstock WHERE idinventario = " + mdb_idinventario );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return 0;
}
*/

///
/**
**/
void ListControlStockView::pregenerar()
{
    blDebug ( Q_FUNC_INFO, 0 );
    QString query;
    query = "SELECT * FROM (SELECT * FROM articulo, almacen) AS t1 LEFT JOIN (SELECT stocknewcontrolstock, idarticulo AS idarticulopk, idalmacen AS idalmacenpk, idinventario AS idinventariopk FROM controlstock WHERE idinventario = 1) AS t2 ON t1.idarticulo = t2.idarticulopk AND t1.idalmacen = t2.idalmacenpk ORDER BY codigoalmacen, codigocompletoarticulo;";
//    BlSubForm::load ( query );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
ListControlStockView::~ListControlStockView()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\return
**/
/*
int ListControlStockView::save()
{
    blDebug ( Q_FUNC_INFO, 0 );
    BfSubForm::save();
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return 0;
}
*/
