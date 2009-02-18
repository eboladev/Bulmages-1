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
#include "blfunctions.h"
#include "blfixed.h"


///
/**
\param parent
\return
**/
ListLinPresupuestoView::ListLinPresupuestoView ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListLinPresupuestoView::ListLinPresupuestoView", 0 );
    setDbTableName ( "lpresupuesto" );
    setDbFieldId ( "idlpresupuesto" );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ListLinPresupuestoView_ListLinPresupuestoView", this );
    if ( res != 0 ) {
        return;
    } // end if
    addSubFormHeader ( "idarticulo", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "Id articulo" ) );
    addSubFormHeader ( "codigocompletoarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Codigo completo" ) );
    addSubFormHeader ( "nomarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Nombre del articulo" ) );
    addSubFormHeader ( "idlpresupuesto", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView, _( "No de linea" ) );
    addSubFormHeader ( "desclpresupuesto", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Descripcion" ) );
    addSubFormHeader ( "cantlpresupuesto", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Cantidad" ) );
    addSubFormHeader ( "pvplpresupuesto", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Precio de venta s/IVA" ) );
    addSubFormHeader ( "ivalpresupuesto", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "% I.V.A." ) );
    addSubFormHeader ( "reqeqlpresupuesto", BlDbField::DBnumeric, BlDbField::DBNothing, BlSubFormHeader::DBNone, _( "% Recargo E.Q." ) );
    addSubFormHeader ( "descuentolpresupuesto", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "% Descuento" ) );
    addSubFormHeader ( "idpresupuesto", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Id presupuesto" ) );
    addSubFormHeader ( "ordenlpresupuesto", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "Orden" ) );
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



