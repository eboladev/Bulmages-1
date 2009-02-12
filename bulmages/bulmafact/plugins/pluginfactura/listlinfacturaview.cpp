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

#include <QMessageBox>
#include <QKeyEvent>
#include <QEvent>

#include "listlinfacturaview.h"
#include "funcaux.h"
#include "blfixed.h"
#include "plugins.h"


///
/**
\param parent
\returns
**/
ListLinFacturaView::ListLinFacturaView ( QWidget *parent ) : SubForm2Bf ( parent )
{
    _depura ( "ListLinFacturaView::ListLinFacturaView", 0 );
    setDBTableName ( "lfactura" );
    setDBCampoId ( "idlfactura" );

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ListLinFacturaView_ListLinFacturaView", this );
    if ( res != 0 )
        return;

    addSHeader ( "idarticulo", DBCampo::DBint, DBCampo::DBNotNull, BlSubFormHeader::DBNoView, _( "Id articulo" ) );
    addSHeader ( "codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone, _( "Codigo completo articulo" ) );
    addSHeader ( "nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Nombre articulo" ) );
    addSHeader ( "idlfactura", DBCampo::DBint, DBCampo::DBPrimaryKey, BlSubFormHeader::DBNoView, _( "Idl factura" ) );
    addSHeader ( "desclfactura", DBCampo::DBvarchar, DBCampo::DBNotNull, BlSubFormHeader::DBNone, _( "Descripcion" ) );
    addSHeader ( "cantlfactura", DBCampo::DBnumeric, DBCampo::DBNotNull, BlSubFormHeader::DBNone, _( "Cantidad" ) );
    addSHeader ( "pvplfactura", DBCampo::DBnumeric, DBCampo::DBNotNull, BlSubFormHeader::DBNone, _( "P.V.P." ) );
    addSHeader ( "ivalfactura", DBCampo::DBnumeric, DBCampo::DBNotNull, BlSubFormHeader::DBNone, _( "I.V.A." ) );
    addSHeader ( "reqeqlfactura", DBCampo::DBnumeric, DBCampo::DBNothing, BlSubFormHeader::DBNone, _( "% Recargo E.Q." ) );
    addSHeader ( "descuentolfactura", DBCampo::DBnumeric, DBCampo::DBNotNull, BlSubFormHeader::DBNone, _( "% Descuento" ) );
    addSHeader ( "idfactura", DBCampo::DBint, DBCampo::DBNotNull, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Id factura" ) );
    addSHeader ( "ordenlfactura", DBCampo::DBint, DBCampo::DBNotNull, BlSubFormHeader::DBNoView, _( "Orden" ) );
    setinsercion ( TRUE );
    setOrdenEnabled ( TRUE );
    /// Disparamos los plugins.
    g_plugins->lanza ( "ListLinFacturaView_ListLinFacturaView_Post", this );

    _depura ( "END ListLinFacturaView::ListLinFacturaView", 0 );
}


///
/**
\param idfactura
**/
void ListLinFacturaView::cargar ( QString idfactura )
{
    _depura ( "ListLinFacturaView::cargar", 0 );
    mdb_idfactura = idfactura;
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ListLinFacturaView_cargar", this );
    if ( res != 0 )
        return;
    BlSubForm::cargar ( "SELECT * FROM lfactura LEFT JOIN articulo ON lfactura.idarticulo = articulo.idarticulo WHERE idfactura=" + mdb_idfactura + " ORDER BY ordenlfactura" );
    _depura ( "END ListLinFacturaView::cargar", 0 );
}



