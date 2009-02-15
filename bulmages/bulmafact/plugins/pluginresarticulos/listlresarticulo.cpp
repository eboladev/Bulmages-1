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

#include "listlresarticulo.h"
#include "blfunctions.h"


///
/**
\param parent
**/
ListLResArticulo::ListLResArticulo ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListLResArticulo::ListLResArticulo", 0 );
    setDBTableName ( "lfactura" );
    setDBCampoId ( "idlfactura" );

    addSHeader ( "cifcliente", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "CIF" ) );
    addSHeader ( "nomcliente", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Cliente" ) );
    addSHeader ( "codigoserie_factura", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Serie Factura" ) );
    addSHeader ( "numfactura", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Num. Factura" ) );
    addSHeader ( "ffactura", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Fecha" ) );
    addSHeader ( "desclfactura", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descripcion" ) );
    addSHeader ( "cantlfactura", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Cantidad" ) );
    addSHeader ( "pvplfactura", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Precio Venta" ) );
    addSHeader ( "ivalfactura", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "IVA" ) );
    addSHeader ( "descuentolfactura", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descuento" ) );

    setinsercion ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    _depura ( "END ListLResArticulo::ListLResArticulo", 0 );
}


///
/**
\param idarticulo
**/
void ListLResArticulo::cargar ( QString idarticulo )
{
    _depura ( "ListLResArticulo::cargar\n", 0 );
    mdb_idarticulo = idarticulo;
    QString SQLQuery = "SELECT * FROM lfactura LEFT JOIN factura ON factura.idfactura= lfactura.idfactura LEFT JOIN cliente ON cliente.idcliente = factura.idcliente WHERE idarticulo =" + mdb_idarticulo;
    BfSubForm::cargar ( SQLQuery );
    _depura ( "END ListLResArticulo::cargar\n", 0 );
}


// =================================================

///
/**
\param parent
**/
ListEntregas::ListEntregas ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListEntregas::ListEntregas", 0 );
    setDBTableName ( "lalbaran" );
    setDBCampoId ( "idlalbaran" );

    addSHeader ( "cifcliente", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "CIF" ) );
    addSHeader ( "nomcliente", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Cliente" ) );
    addSHeader ( "numalbaran", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Num. albaran" ) );
    addSHeader ( "falbaran", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Fecha" ) );
    addSHeader ( "desclalbaran", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descripcion" ) );
    addSHeader ( "cantlalbaran", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Cantidad" ) );
    addSHeader ( "pvplalbaran", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Precio" ) );
    addSHeader ( "ivalalbaran", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "IVA" ) );
    addSHeader ( "descuentolalbaran", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descuento" ) );

    setinsercion ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    _depura ( "END ListEntregas::ListEntregas", 0 );
}


///
/**
\param idarticulo
**/
void ListEntregas::cargar ( QString idarticulo )
{
    _depura ( "ListEntregas::cargar\n", 0 );
    mdb_idarticulo = idarticulo;
    QString SQLQuery = "SELECT * FROM lalbaran LEFT JOIN albaran ON albaran.idalbaran= lalbaran.idalbaran LEFT JOIN cliente ON cliente.idcliente = albaran.idcliente WHERE idarticulo =" + mdb_idarticulo;
    BfSubForm::cargar ( SQLQuery );
    _depura ( "END ListEntregas::cargar\n", 0 );
}


// =================================================

///
/**
\param parent
**/
ListCompras::ListCompras ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListCompras::ListCompras", 0 );
    setDBTableName ( "lfacturap" );
    setDBCampoId ( "idlfacturap" );

    addSHeader ( "cifproveedor", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "CIF" ) );
    addSHeader ( "nomproveedor", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Proveedor" ) );
    addSHeader ( "numfacturap", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Num. factura" ) );
    addSHeader ( "ffacturap", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Fecha" ) );
    addSHeader ( "desclfacturap", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descripcion" ) );
    addSHeader ( "cantlfacturap", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Cantidad" ) );
    addSHeader ( "pvplfacturap", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Precio" ) );
    addSHeader ( "ivalfacturap", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "IVA" ) );
    addSHeader ( "descuentolfacturap", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descuento" ) );

    setinsercion ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    _depura ( "END ListCompras::ListCompras", 0 );
}


///
/**
\param idarticulo
**/
void ListCompras::cargar ( QString idarticulo )
{
    _depura ( "ListCompras::cargar\n", 0 );
    mdb_idarticulo = idarticulo;
    QString SQLQuery = "SELECT * FROM lfacturap LEFT JOIN facturap ON facturap.idfacturap= lfacturap.idfacturap LEFT JOIN proveedor ON proveedor.idproveedor = facturap.idproveedor WHERE idarticulo =" + mdb_idarticulo;
    BfSubForm::cargar ( SQLQuery );
    _depura ( "END ListCompras::cargar\n", 0 );
}



// =================================================

///
/**
\param parent
**/
ListRecogidas::ListRecogidas ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListRecogidas::ListRecogidas", 0 );
    setDBTableName ( "lalbaranp" );
    setDBCampoId ( "idlalbaranp" );

    addSHeader ( "cifproveedor", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "CIF" ) );
    addSHeader ( "nomproveedor", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Proveedor" ) );
    addSHeader ( "numalbaranp", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Num. albaran" ) );
    addSHeader ( "falbaranp", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Fecha" ) );
    addSHeader ( "desclalbaranp", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descripcion" ) );
    addSHeader ( "cantlalbaranp", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Cantidad" ) );
    addSHeader ( "pvplalbaranp", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Precio" ) );
    addSHeader ( "ivalalbaranp", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "IVA" ) );
    addSHeader ( "descuentolalbaranp", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descuento" ) );

    setinsercion ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    _depura ( "END ListRecogidas::ListRecogidas", 0 );
}


///
/**
\param idarticulo
**/
void ListRecogidas::cargar ( QString idarticulo )
{
    _depura ( "ListRecogidas::cargar\n", 0 );
    mdb_idarticulo = idarticulo;
    QString SQLQuery = "SELECT * FROM lalbaranp LEFT JOIN albaranp ON albaranp.idalbaranp= lalbaranp.idalbaranp LEFT JOIN proveedor ON proveedor.idproveedor = albaranp.idproveedor WHERE idarticulo =" + mdb_idarticulo;
    BfSubForm::cargar ( SQLQuery );
    _depura ( "END ListRecogidas::cargar\n", 0 );
}

