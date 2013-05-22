/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <fstream>

#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>
#include <QCloseEvent>

#include "pagoview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"
#include "blcombobox.h"


///
/**
\param comp
\param parent
**/
PagoView::PagoView ( BfCompany *comp, QWidget *parent )
        :  BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    try {
        setAttribute ( Qt::WA_DeleteOnClose );
        setupUi ( this );
        blCenterOnScreen ( this );
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        mui_idproveedor->setMainCompany ( comp );
        mui_refpago->setMainCompany ( comp );
        mui_idbanco->setMainCompany ( comp );

        setTitleName ( _ ( "Pago" ) );
        setDbTableName ( "pago" );
        setDbFieldId ( "idpago" );
        addDbField ( "idpago", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Id pago" ) );
        addDbField ( "idproveedor", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id proveedor" ) );
        addDbField ( "previsionpago", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Previcion de pago" ) );
        addDbField ( "fechapago", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha de pago" ) );
        addDbField ( "refpago", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia de pago" ) );
        addDbField ( "cantpago", BlDbField::DbNumeric, BlDbField::DbNotNull, _ ( "Cantidad" ) );
        addDbField ( "comentpago", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentario del pago" ) );
        addDbField ( "idbanco", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Banco" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Forma de pago" ) );
	
        /// Disparamos los plugins.
        int res = g_plugins->run ( "PagoView_PagoView", this );
        if ( res != 0 ) {
            return;
        } // end if
	
        /// Establecemos los parametros de busqueda del Cliente
        mui_idproveedor->setLabel ( _ ( "Proveedor:" ) );
        mui_idproveedor->setTableName ( "proveedor" );
        mui_idproveedor->m_valores["cifproveedor"] = "";
        mui_idproveedor->m_valores["nomproveedor"] = "";

	mui_idforma_pago->setMainCompany( comp );
        mui_idforma_pago->setQuery ( "SELECT * FROM forma_pago ORDER BY descforma_pago" );
        mui_idforma_pago->setTableName ( "forma_pago" );
        mui_idforma_pago->setFieldId ( "idforma_pago" );
        mui_idforma_pago->m_valores["descforma_pago"] = "";
        mui_idforma_pago->setAllowNull ( FALSE );
        mui_idforma_pago->setId ( "" );
	
	
        insertWindow ( windowTitle(), this, FALSE );
        pintar();
        dialogChanges_readValues();
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el pago" ), this );
    } // end try

    
}



///
/**
**/
PagoView::~PagoView()
{
    BL_FUNC_DEBUG
    
}

/** Pintar un albaran de proveedor
*/
void PagoView::pintarPost ( )
{
    BL_FUNC_DEBUG

    /// Escribimos como descripcion el nombre del proveedor para que aparezca en el titulo y en el dockwidget
    setDescripcion( mui_idproveedor->fieldValue("nomproveedor") + "\n" + mui_fechapago->text());
}



/** Cuando hacemos un cobro y el valor del cobro es igual por referencia que todas las facturas
     con la misma referencia se procesan dichas facturas **/
int PagoView::afterSave()
{
    BL_FUNC_DEBUG

    QString query1 = "SELECT COALESCE(sum (cantpago), 0) AS totalp FROM pago WHERE refpago='" + dbValue ( "refpago" ) + "'";
    BlDbRecordSet *cur1 = mainCompany()->loadQuery ( query1 );

    QString query = "SELECT COALESCE(sum(totalfacturap), 0) AS total FROM facturap WHERE reffacturap='" + dbValue ( "refpago" ) + "'";
    BlDbRecordSet *cur = mainCompany()->loadQuery ( query );

    if ( cur->value( "total" ) == cur1->value( "totalp" ) ) {
        blMsgInfo ( _("Toda la referencia esta pagada. Se procesaran todos los documentos con esta referencia") );
        QString query2 = "UPDATE facturap set procesadafacturap = TRUE WHERE reffacturap='" + dbValue ( "refpago" ) + "'";
        mainCompany()->runQuery ( query2 );
    } // end if
    delete cur;
    delete cur1;

    /// Disparamos los plugins.
    g_plugins->run ( "PagoView_afterSave_Post", this );  
    
    return 0;
}