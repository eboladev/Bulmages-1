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

#include "listlinalbaranclienteview.h"
#include "articulolist.h"
#include "funcaux.h"
#include "blfixed.h"
#include "plugins.h"


///
/**
\param parent
**/
ListLinAlbaranClienteView::ListLinAlbaranClienteView ( QWidget *parent )
        : SubForm2Bf ( parent )
{
    _depura ( "ListLinAlbaranClienteView::ListLinAlbaranClienteView", 0 );
    setDBTableName ( "lalbaran" );
    setDBCampoId ( "numlalbaran" );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ListLinAlbaranClienteView_ListLinAlbaranClienteView", this );
    if ( res != 0 )
        return;
    addSHeader ( "idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, _( "Id articulo" ) );
    addSHeader ( "codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, _( "Codigo completo articulo" ) );
    addSHeader ( "nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "Nombre articulo" ) );
    addSHeader ( "numlalbaran", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, _( "Nº linea" ) );
    addSHeader ( "desclalbaran", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, _( "Descripcion" ) );
    addSHeader ( "cantlalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, _( "Cantidad" ) );
    addSHeader ( "pvplalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, _( "P.V.P." ) );
    addSHeader ( "ivalalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, _( "I.V.A." ) );
    addSHeader ( "reqeqlalbaran", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone, _( "% Recargo E.Q." ) );
    addSHeader ( "descuentolalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, _( "% Descuento" ) );
    addSHeader ( "idalbaran", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView | SHeader::DBNoWrite, _( "Id albaran" ) );
    addSHeader ( "ordenlalbaran", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, _( "Orden" ) );
    setinsercion ( TRUE );
    setOrdenEnabled ( TRUE );
    /// Disparamos los plugins.
    g_plugins->lanza ( "ListLinAlbaranClienteView_ListLinAlbaranClienteView_Post", this );
    _depura ( "END ListLinAlbaranClienteView::ListLinAlbaranClienteView", 0 );

}


///
/**
\param idalbaran
**/
void ListLinAlbaranClienteView::cargar ( QString idalbaran )
{
    _depura ( "ListLinAlbaranClienteView::cargar", 0 );
    mdb_idalbaran = idalbaran;
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ListLinAlbaranClienteView_cargar", this );
    if ( res != 0 )
        return;

    SubForm3::cargar ( "SELECT * FROM lalbaran LEFT JOIN articulo ON lalbaran.idarticulo = articulo.idarticulo WHERE idalbaran=" + mdb_idalbaran + "   ORDER BY ordenlalbaran" );
    _depura ( "END ListLinAlbaranClienteView::cargar", 0 );
}


