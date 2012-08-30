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

#include <stdio.h>
#include <QTime>

#include "pluginbf_carterapagos.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "carterapagoslist.h"


BfBulmaFact *g_pluginbf_carterapagos = NULL;


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("DBRev-CarteraPagos", "0.11.1-0001");

    g_pluginbf_carterapagos = bges;
 
    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_carteracobros", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    /// Creamos el men&uacute;.
    /// Miramos si existe un menu Ventas
	QMenu *pPluginMenu = bges->newMenu( _("&Compras"), "menuCompras", "menuMaestro");

    BlAction *accion = new BlAction ( _("&Cartera de Pagos"), 0 );
    accion->setStatusTip ( _("Cartera de Pagos" ));
    accion->setWhatsThis ( _("Cartera de Pagos" ) );
    accion->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/pay-list.png" ) ) );
    accion->setObjectName("mui_actionCarteraPagos");

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accion );
	g_pluginbf_carterapagos->Listados->addAction (accion);

    return 0;
}

int BlAction_actionTriggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionCarteraPagos") {
        CarteraPagosList *vehiculoview = new CarteraPagosList ( g_pluginbf_carterapagos->company() );
        g_pluginbf_carterapagos->company()->m_pWorkspace ->addSubWindow ( vehiculoview );
        //m_bulmafact->workspace() ->addSubWindow ( vehiculoview );
        vehiculoview->show();
    } // endi if
    return 0;
}

///
/**
\param art
\return
**/
int ProveedorView_ProveedorView_Post ( ProveedorView *art )
{
    BL_FUNC_DEBUG

    /// Agregamos el subformulario de validaciones.
    BfSubForm *l = new BfSubForm ( art );
    l->setObjectName ( QString::fromUtf8 ( "vencimientoproveedor" ) );
    l->setMainCompany ( art->mainCompany() );
    l->setDbTableName ( "vencimientoproveedor" );
    l->setDbFieldId ( "idvencimientoproveedor" );
    l->addSubFormHeader ( "idvencimientoproveedor", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView , QApplication::translate("TrabajadorView", "ID Vencimiento"));
    l->addSubFormHeader ( "diasvencimientoproveedor", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNone , QApplication::translate ( "TrabajadorView", "Dias tras Factura" ) );
    l->addSubFormHeader ( "porcentajevencimientoproveedor", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, QApplication::translate ( "TrabajadorView", "Porcentaje" ) );
    l->addSubFormHeader ( "descforma_pago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, QApplication::translate ( "TrabajadorView", "Forma de Pago" ) );
    l->addSubFormHeader ( "idproveedor", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite , QApplication::translate ( "TrabajadorView", "Id Articulo" ) );
    l->addSubFormHeader ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNone, QApplication::translate ( "TrabajadorView", "ID Forma de Pago" ) );
    l->setInsert ( TRUE );
    l->setDelete ( TRUE );
    l->setSortingEnabled ( FALSE );
    art->dialogChanges_setExcludedObject ( l->mui_list );

    art->mui_tab->addTab ( l, "Vencimientos" );
    l->load("SELECT * FROM vencimientoproveedor WHERE idproveedor IS NULL");
    
    return 0;
}


///
/**
\param art
\return
**/
int ProveedorView_cargarPost_Post ( ProveedorView *art )
{
    BL_FUNC_DEBUG
    BfSubForm *l = art->findChild<BfSubForm *> ( "vencimientoproveedor" );
    if ( l ) {
        l->load ( "SELECT * FROM vencimientoproveedor LEFT JOIN forma_pago ON vencimientoproveedor.idforma_pago = forma_pago.idforma_pago WHERE idproveedor =  " + art->dbValue("idproveedor"));
    } // end if
    
    return 0;
}


void generarVencimientos (FacturaProveedorView *art) {
			BlDbRecordSet *cur1 = art->mainCompany()->loadQuery("SELECT totalfacturap FROM facturap WHERE idfacturap = " + art->dbValue("idfacturap"));
			blMsgInfo( _("El total de la factura es :") + cur1->value("totalfacturap"));
			BlFixed contado("0.00");

			BlDbRecordSet *cur = art->mainCompany()->loadQuery("SELECT * FROM vencimientoproveedor WHERE idproveedor = " + art->dbValue("idproveedor"));
			while (!cur->eof())  {
				QString query = "SELECT ffacturap + " + cur->value("diasvencimientoproveedor") + " AS fechav, totalfacturap / 100 * "+cur->value("porcentajevencimientoproveedor")+" AS porcent FROM facturap WHERE idfacturap = " + art->dbValue("idfacturap");
				if (cur->isLastRecord()) {
					query = "SELECT ffacturap + " + cur->value("diasvencimientoproveedor") + " AS fechav, totalfacturap - "+ contado.toQString('.') +" AS porcent FROM facturap WHERE idfacturap = " + art->dbValue("idfacturap");
				} //end if
				BlDbRecordSet *cur2 = art->mainCompany()->loadQuery(query);
				/// REALIZAMOS EL QUERY
				query = "INSERT INTO vencimientop (idfacturap, fechavencimientop, cantvencimientop, idforma_pago, refvencimientop, idproveedor) VALUES ("+art->dbValue("idfacturap")+",'"+cur2->value("fechav")+"',"+cur2->value("porcent")+","+cur->value("idforma_pago")+",'"+art->dbValue("reffacturap")+"',"+art->dbValue("idproveedor")+")";
				contado = contado + BlFixed(cur2->value("porcent"));
				art->mainCompany()->runQuery(query);
				delete cur2;
				cur->nextRecord();
			} // end while
			delete cur;
			delete cur1;
}


///
/**
\param art
\return
**/
int BlForm_afterSave_Post ( BlForm *art )
{
    BL_FUNC_DEBUG
    
    try {

        BfSubForm *l = art->findChild<BfSubForm *> ( "vencimientoproveedor" );
	if (l) {
		l->setColumnValue ( "idproveedor", art->dbValue ( "idproveedor" ) );
		l->save();
        }

	
	/* ES EL GUARDADO DE LA FACTURA DE PROVEEDOR*/
        BfSubForm *l1 = art->findChild<BfSubForm *> ( "vencimientop" );
	if (l1) {
		if (l1->rowCount() > 1) {

			QMessageBox msgBox;
			msgBox.setText(_("Hay Vencimientos generados para esta factura. Que desea hacer?"));
			QPushButton *guardarButton = msgBox.addButton("Guardar", QMessageBox::ActionRole);
			QPushButton *regenerarButton = msgBox.addButton("Regenerar", QMessageBox::ActionRole);
			QPushButton *nadaButton = msgBox.addButton("Dejar Actuales", QMessageBox::ActionRole);
			msgBox.exec();
			if (msgBox.clickedButton() == guardarButton) {
				l1->setColumnValue ( "idfacturap", art->dbValue ( "idfacturap" ) );
				l1->setColumnValue ( "idproveedor", art->dbValue ( "idproveedor" ) );
				l1->save();
			} else if (msgBox.clickedButton() == regenerarButton) {
				QString query = "DELETE FROM vencimientop WHERE idfacturap="+ art->dbValue("idfacturap");
				art->mainCompany()->runQuery(query);
				generarVencimientos( (FacturaProveedorView *) art);
			} // end if
		} else {
			generarVencimientos( (FacturaProveedorView *) art);
		} // end if
        } // end if
        return 0;
    } catch ( ... ) {
	
        return 0;
    } // end try
}




int FacturaProveedorView_FacturaProveedorView (FacturaProveedorView *factp) {
    BL_FUNC_DEBUG
    /// Agregamos el subformulario de validaciones.
    BfSubForm *l = new BfSubForm ( factp );
    l->setObjectName ( QString::fromUtf8 ( "vencimientop" ) );
    l->setMainCompany ( factp->mainCompany() );
    l->setDbTableName ( "vencimientop" );
    l->setDbFieldId ( "idvencimientop" );
    l->addSubFormHeader ( "idvencimientop", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView , QApplication::translate("TrabajadorView", "ID Vencimiento"));
    l->addSubFormHeader ( "fechavencimientop", BlDbField::DbDate, BlDbField::DbNotNull, BlSubFormHeader::DbNone , QApplication::translate ( "TrabajadorView", "Fecha" ) );
    l->addSubFormHeader ( "cantvencimientop", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, QApplication::translate ( "TrabajadorView", "Cantidad" ) );
    l->addSubFormHeader ( "descforma_pago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, QApplication::translate ( "TrabajadorView", "Forma de Pago" ) );
    l->addSubFormHeader ( "procesadovencimientop", BlDbField::DbBoolean, BlDbField::DbNothing, BlSubFormHeader::DbNone, QApplication::translate ( "TrabajadorView", "Procesado" ) );
    l->addSubFormHeader ( "idfacturap", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite , QApplication::translate ( "TrabajadorView", "Id Articulo" ) );
    l->addSubFormHeader ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, QApplication::translate ( "TrabajadorView", "ID Forma de Pago" ) );
    l->addSubFormHeader ( "idproveedor", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite , QApplication::translate ( "Proveedor", "Id Proveedor" ) );
    l->setInsert ( TRUE );
    l->setDelete ( TRUE );
    l->setSortingEnabled ( FALSE );
    factp->dialogChanges_setExcludedObject ( l->mui_list );

    factp->discounts->addTab ( l, "Vencimientos" );
    l->load("SELECT * FROM vencimientop WHERE idfacturap IS NULL");
    
    return 0;
}

///
/**
\param art
\return
**/
int FacturaProveedorView_cargarPost_Post ( FacturaProveedorView *art )
{
    BL_FUNC_DEBUG
    BfSubForm *l = art->findChild<BfSubForm *> ( "vencimientop" );
    if ( l ) {
        l->load ( "SELECT * FROM vencimientop LEFT JOIN forma_pago ON vencimientop.idforma_pago = forma_pago.idforma_pago WHERE idfacturap =  " + art->dbValue("idfacturap"));
    } // end if
    
    return 0;
}



