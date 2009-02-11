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
#include <QWaitCondition>

#include "listlinpresupuestoview.h"
#include "funcaux.h"
#include "blfixed.h"


///
/**
\param parent
\return
**/
ListLinPresupuestoView::ListLinPresupuestoView ( QWidget *parent ) : SubForm2Bf ( parent )
{
    _depura ( "ListLinPresupuestoView::ListLinPresupuestoView", 0 );
    setDBTableName ( "lpresupuesto" );
    setDBCampoId ( "idlpresupuesto" );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ListLinPresupuestoView_ListLinPresupuestoView", this );
    if ( res != 0 ) {
        return;
    } // end if
    addSHeader ( "idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, _( "Id articulo" ) );
    addSHeader ( "codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, _( "Codigo completo" ) );
    addSHeader ( "nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "Nombre del articulo" ) );
    addSHeader ( "idlpresupuesto", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, _( "No de linea" ) );
    addSHeader ( "desclpresupuesto", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, _( "Descripcion" ) );
    addSHeader ( "cantlpresupuesto", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, _( "Cantidad" ) );
    addSHeader ( "pvplpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, _( "Precio de venta s/IVA" ) );
    addSHeader ( "ivalpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, _( "% I.V.A." ) );
    addSHeader ( "reqeqlpresupuesto", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone, _( "% Recargo E.Q." ) );
    addSHeader ( "descuentolpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, _( "% Descuento" ) );
    addSHeader ( "idpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView | SHeader::DBNoWrite, _( "Id presupuesto" ) );
    addSHeader ( "ordenlpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, _( "Orden" ) );
    setinsercion ( TRUE );
    setOrdenEnabled ( TRUE );
    setOrdenPorQuery ( FALSE );

    /// Lanzamos los plugins
    g_plugins->lanza ( "ListLinPresupuestoView_ListLinPresupuestoView_Post", this );
    _depura ( "END ListLinPresupuestoView::ListLinPresupuestoView", 0 );
}


///
/**
\param idpresupuesto
**/
void ListLinPresupuestoView::cargar ( QString idpresupuesto )
{
    _depura ( "ListLinPresupuestoView::cargar", 0 );
    mdb_idpresupuesto = idpresupuesto;
    /// Al hacer la carge usamos el ordenlpresupuesto para indicar que el campo de ordenacion es el que toca.
    BlSubForm::cargar ( "SELECT * FROM lpresupuesto LEFT JOIN articulo ON lpresupuesto.idarticulo = articulo.idarticulo WHERE idpresupuesto = " + mdb_idpresupuesto + " ORDER BY ordenlpresupuesto" );
    _depura ( "END ListLinPresupuestoView::cargar", 0 );
}



