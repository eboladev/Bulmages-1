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
#include "funcaux.h"


///
/**
\param parent
**/
ListLResArticulo::ListLResArticulo ( QWidget *parent ) : SubForm2Bf ( parent )
{
    _depura ( "ListLResArticulo::ListLResArticulo", 0 );
    setDBTableName ( "lfactura" );
    setDBCampoId ( "idlfactura" );

    addSHeader ( "cifcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "CIF" ) );

    addSHeader ( "nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Cliente" ) );

    addSHeader ( "codigoserie_factura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Serie Factura" ) );

    addSHeader ( "numfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Num. Factura" ) );

    addSHeader ( "ffactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Fecha" ) );

    addSHeader ( "desclfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Descripcion" ) );

    addSHeader ( "cantlfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Cantidad" ) );

    addSHeader ( "pvplfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Precio Venta" ) );

    addSHeader ( "ivalfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "IVA" ) );

    addSHeader ( "descuentolfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Descuento" ) );

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
    SubForm2Bf::cargar ( SQLQuery );
    _depura ( "END ListLResArticulo::cargar\n", 0 );
}


// =================================================

///
/**
\param parent
**/
ListEntregas::ListEntregas ( QWidget *parent ) : SubForm2Bf ( parent )
{
    _depura ( "ListEntregas::ListEntregas", 0 );
    setDBTableName ( "lalbaran" );
    setDBCampoId ( "idlalbaran" );

    addSHeader ( "cifcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "CIF" ) );

    addSHeader ( "nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Cliente" ) );

    addSHeader ( "numalbaran", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Num. Albaran" ) );

    addSHeader ( "falbaran", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Fecha" ) );

    addSHeader ( "desclalbaran", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Descripcion" ) );

    addSHeader ( "cantlalbaran", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Cantidad" ) );

    addSHeader ( "pvplalbaran", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Precio" ) );

    addSHeader ( "ivalalbaran", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "IVA" ) );

    addSHeader ( "descuentolalbaran", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Descuento" ) );

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
    SubForm2Bf::cargar ( SQLQuery );
    _depura ( "END ListEntregas::cargar\n", 0 );
}


// =================================================

///
/**
\param parent
**/
ListCompras::ListCompras ( QWidget *parent ) : SubForm2Bf ( parent )
{
    _depura ( "ListCompras::ListCompras", 0 );
    setDBTableName ( "lfacturap" );
    setDBCampoId ( "idlfacturap" );

    addSHeader ( "cifproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "CIF" ) );

    addSHeader ( "nomproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Proveedor" ) );

    addSHeader ( "numfacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Num. Factura" ) );

    addSHeader ( "ffacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Fecha" ) );

    addSHeader ( "desclfacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Descripcion" ) );

    addSHeader ( "cantlfacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Cantidad" ) );

    addSHeader ( "pvplfacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Precio" ) );

    addSHeader ( "ivalfacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "IVA" ) );

    addSHeader ( "descuentolfacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Descuento" ) );

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
    SubForm2Bf::cargar ( SQLQuery );
    _depura ( "END ListCompras::cargar\n", 0 );
}



// =================================================

///
/**
\param parent
**/
ListRecogidas::ListRecogidas ( QWidget *parent ) : SubForm2Bf ( parent )
{
    _depura ( "ListRecogidas::ListRecogidas", 0 );
    setDBTableName ( "lalbaranp" );
    setDBCampoId ( "idlalbaranp" );

    addSHeader ( "cifproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "CIF" ) );

    addSHeader ( "nomproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Proveedor" ) );

    addSHeader ( "numalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Num. Albaran" ) );

    addSHeader ( "falbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Fecha" ) );

    addSHeader ( "desclalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Descripcion" ) );

    addSHeader ( "cantlalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Cantidad" ) );

    addSHeader ( "pvplalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Precio" ) );

    addSHeader ( "ivalalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "IVA" ) );

    addSHeader ( "descuentolalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Descuento" ) );

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
    SubForm2Bf::cargar ( SQLQuery );
    _depura ( "END ListRecogidas::cargar\n", 0 );
}