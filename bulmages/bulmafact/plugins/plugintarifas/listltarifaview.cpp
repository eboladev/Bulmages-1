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

#include "listltarifaview.h"
#include "blfunctions.h"


///
/**
\param parent
**/
ListLTarifaView::ListLTarifaView ( QWidget *parent ) : SubForm2Bf ( parent )
{
    _depura ( "ListLTarifaView::ListLTarifaView", 0 );
    setDBTableName ( "ltarifa" );
    setDBCampoId ( "idltarifa" );
    addSHeader ( "nomtarifa", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Nombre tarifa" ) );
    addSHeader ( "nomalmacen", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Nombre almacen" ) );
    addSHeader ( "pvpltarifa", BlDbField::DBnumeric, BlDbField::DBRequired, BlSubFormHeader::DBNone, _( "Precio de venta" ) );
    addSHeader ( "idarticulo", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "ID articulo" ) );
    addSHeader ( "idalmacen", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "ID almacen" ) );
    addSHeader ( "idtarifa", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "ID tarifa" ) );
    addSHeader ( "idltarifa", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView, _( "Num de linea" ) );
    addSHeader ( "codigocompletoarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoView, _( "Codigo completo del articulo" ) );
    addSHeader ( "nomarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoView, _( "Nombre del articulo" ) );
    setinsercion ( FALSE );
    _depura ( "END ListLTarifaView::ListLTarifaView", 0 );
}


///
/**
\param parent
**/
ListLTarifaView1::ListLTarifaView1 ( QWidget *parent ) : SubForm2Bf ( parent )
{
    _depura ( "ListLTarifaView1::ListLTarifaView1", 0 );
    setDBTableName ( "ltarifa" );
    setDBCampoId ( "idltarifa" );
    addSHeader ( "nomalmacen", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite , _( "Nombre almacen" ) );
    addSHeader ( "codigocompletoarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Codigo completo del articulo" ) );
    addSHeader ( "nomarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite , _( "Nombre articulo" ) );
    addSHeader ( "pvpltarifa", BlDbField::DBnumeric, BlDbField::DBRequired, BlSubFormHeader::DBNone, _( "Precio de venta" ) );
    addSHeader ( "idarticulo", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "ID articulo" ) );
    addSHeader ( "idalmacen", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "ID almacen" ) );
    addSHeader ( "idtarifa", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "ID tarifa" ) );
    addSHeader ( "idltarifa", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView, _( "Num de linea" ) );
    addSHeader ( "nomtarifa", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoView, _( "Nombre de la tarifa" ) );
    setinsercion ( FALSE );
    _depura ( "ListLTarifaView1::ListLTarifaView1", 0 );
}

