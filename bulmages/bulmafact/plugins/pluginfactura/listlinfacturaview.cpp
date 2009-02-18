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
#include "blfunctions.h"
#include "blfixed.h"
#include "plugins.h"


///
/**
\param parent
\returns
**/
ListLinFacturaView::ListLinFacturaView ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListLinFacturaView::ListLinFacturaView", 0 );
    setDbTableName ( "lfactura" );
    setDbFieldId ( "idlfactura" );

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ListLinFacturaView_ListLinFacturaView", this );
    if ( res != 0 )
        return;

    addSubFormHeader ( "idarticulo", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "Id articulo" ) );
    addSubFormHeader ( "codigocompletoarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Codigo completo articulo" ) );
    addSubFormHeader ( "nomarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Nombre articulo" ) );
    addSubFormHeader ( "idlfactura", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView, _( "Idl factura" ) );
    addSubFormHeader ( "desclfactura", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Descripcion" ) );
    addSubFormHeader ( "cantlfactura", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Cantidad" ) );
    addSubFormHeader ( "pvplfactura", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "P.V.P." ) );
    addSubFormHeader ( "ivalfactura", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "I.V.A." ) );
    addSubFormHeader ( "reqeqlfactura", BlDbField::DBnumeric, BlDbField::DBNothing, BlSubFormHeader::DBNone, _( "% Recargo E.Q." ) );
    addSubFormHeader ( "descuentolfactura", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "% Descuento" ) );
    addSubFormHeader ( "idfactura", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Id factura" ) );
    addSubFormHeader ( "ordenlfactura", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "Orden" ) );
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



