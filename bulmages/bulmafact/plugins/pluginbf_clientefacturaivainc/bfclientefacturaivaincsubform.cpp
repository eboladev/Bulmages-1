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

#include <QtWidgets/QMessageBox>
#include <QtGui/QKeyEvent>
#include <QtCore/QEvent>

#include "bfclientefacturaivaincsubform.h"
#include "blfunctions.h"
#include "blfixed.h"
#include "blplugins.h"


///
/**
\param parent
**/
BfClienteFacturaIVAIncSubForm::BfClienteFacturaIVAIncSubForm ( QWidget *parent )
        : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    setDbTableName ( "lfactura" );
    setDbFieldId ( "idlfactura" );
    /// Disparamos los plugins.
    int res = g_plugins->run ( "BfClienteFacturaIVAIncSubForm_BfClienteFacturaIVAIncSubForm", this );
    if ( res != 0 )
        return;
    addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView, _ ( "Id articulo" ) );
    addSubFormHeader ( "codigocompletoarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Codigo completo articulo" ) );
    addSubFormHeader ( "nomarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "Nombre articulo" ) );
    addSubFormHeader ( "idlfactura", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView, _ ( "No linea" ) );
    addSubFormHeader ( "desclfactura", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Descripcion" ) );
    addSubFormHeader ( "cantlfactura", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Cantidad" ) );
//    addSubFormHeader ( "pvplfactura", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "P.V.P." ) );
    addSubFormHeader ( "pvpivainclfactura", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "P.V.P." ) );
    addSubFormHeader ( "totallfactura", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "Total Linea" ) );
    addSubFormHeader ( "ivalfactura", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "I.V.A." ) );
    addSubFormHeader ( "reqeqlfactura", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "% Recargo E.Q." ) );
    addSubFormHeader ( "descuentolfactura", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "% Descuento" ) );
    addSubFormHeader ( "idfactura", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id factura" ) );
    addSubFormHeader ( "ordenlfactura", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView, _ ( "Orden" ) );
    setInsert ( true );
    setOrdenEnabled ( true );
    /// Disparamos los plugins.
    g_plugins->run ( "BfClienteFacturaIVAIncSubForm_BfClienteFacturaIVAIncSubForm_Post", this );
    

}


///
/**
\param idfactura
**/
void BfClienteFacturaIVAIncSubForm::load ( QString idfactura )
{
    BL_FUNC_DEBUG
    mdb_idfactura = idfactura;
    /// Disparamos los plugins.
    int res = g_plugins->run ( "BfClienteFacturaIVAIncSubForm_cargar", this );
    if ( res != 0 )
        return;

    BlSubForm::load ( "SELECT *, (cantlfactura * pvpivainclfactura)::NUMERIC(12,2) AS totallfactura FROM lfactura LEFT JOIN articulo ON lfactura.idarticulo = articulo.idarticulo WHERE idfactura=" + mdb_idfactura + "   ORDER BY ordenlfactura" );
    
}


