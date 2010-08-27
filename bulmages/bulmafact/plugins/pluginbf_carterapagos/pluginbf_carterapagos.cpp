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
#include <QWidget>

#include "pluginbf_carterapagos.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "carterapagoslist.h"



///
/**
**/
MyPluginCarteraPagos::MyPluginCarteraPagos()
{
    blDebug ( "MyPluginCarteraPagos::MyPluginCarteraPagos", 0 );
    blDebug ( "END MyPluginCarteraPagos::MyPluginCarteraPagos", 0 );
}

///
/**
**/
MyPluginCarteraPagos::~MyPluginCarteraPagos()
{
    blDebug ( "MyPluginCarteraPagos::~MyPluginCarteraPagos", 0 );
    blDebug ( "END MyPluginCarteraPagos::~MyPluginCarteraPagos", 0 );
}


///
/**
**/
void MyPluginCarteraPagos::elslot()
{
    blDebug ( "MyPluginCarteraPagos::elslot", 0 );
    CarteraPagosList *vehiculoview = new CarteraPagosList ( ( BfCompany * ) m_conexionbase );
    m_bulmafact->workspace() ->addWindow ( vehiculoview );
    vehiculoview->show();
    blDebug ( "END MyPluginCarteraPagos::elslot", 0 );
}


///
/**
\param bges
**/
void MyPluginCarteraPagos::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "MyPluginCarteraPagos::inicializa", 0 );
    /// Creamos el men&uacute;.
    m_conexionbase = bges->company();
    m_bulmafact = bges;

    /// Miramos si existe un menu Ventas
	QMenu *pPluginMenu = bges->newMenu("&Compras", "menuCompras", "menuMaestro");

    QAction *accion = new QAction ( "&Cartera de Pagos", 0 );
    accion->setStatusTip ( "Cartera de Pagos" );
    accion->setWhatsThis ( "Cartera de Pagos" );
    accion->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/pay-list.png" ) ) );
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accion );
	bges->Listados->addAction (accion);
    blDebug ( "END MyPluginCarteraPagos::inicializa", 0 );
}






///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de Entrada del plugin de Cartera de Pagos\n", 0 );
    MyPluginCarteraPagos *plug = new MyPluginCarteraPagos();
    plug->inicializa ( bges );
    return 0;
}


///
/**
\param art
\return
**/
int ProveedorView_ProveedorView_Post ( ProveedorView *art )
{
    blDebug ( "ArticuloView_ArticuloView", 0 );

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
    art->dialogChanges_setQObjectExcluido ( l->mui_list );

    art->mui_tab->addTab ( l, "Vencimientos" );
    l->cargar("SELECT * FROM vencimientoproveedor WHERE idproveedor IS NULL");
    blDebug ( "END ArticuloView_ArticuloView", 0 );
    return 0;
}


///
/**
\param art
\return
**/
int ProveedorView_cargarPost_Post ( ProveedorView *art )
{
    blDebug ( "ArticuloView_cargar", 0 );
    BfSubForm *l = art->findChild<BfSubForm *> ( "vencimientoproveedor" );
    if ( l ) {
        l->cargar ( "SELECT * FROM vencimientoproveedor LEFT JOIN forma_pago ON vencimientoproveedor.idforma_pago = forma_pago.idforma_pago WHERE idproveedor =  " + art->dbValue("idproveedor"));
    } // end if
    blDebug ( "END ArticuloView_cargar", 0 );
    return 0;
}


void generarVencimientos (FacturaProveedorView *art) {
			BlDbRecordSet *cur1 = art->mainCompany()->loadQuery("SELECT totalfacturap FROM facturap WHERE idfacturap = " + art->dbValue("idfacturap"));
			blMsgInfo( "El total de la factura es :" + cur1->valor("totalfacturap"));
			BlFixed contado("0.00");

			BlDbRecordSet *cur = art->mainCompany()->loadQuery("SELECT * FROM vencimientoproveedor WHERE idproveedor = " + art->dbValue("idproveedor"));
			while (!cur->eof())  {
				QString query = "SELECT ffacturap + " + cur->valor("diasvencimientoproveedor") + " AS fechav, totalfacturap / 100 * "+cur->valor("porcentajevencimientoproveedor")+" AS porcent FROM facturap WHERE idfacturap = " + art->dbValue("idfacturap");
				if (cur->isLastRecord()) {
					query = "SELECT ffacturap + " + cur->valor("diasvencimientoproveedor") + " AS fechav, totalfacturap - "+ contado.toQString('.') +" AS porcent FROM facturap WHERE idfacturap = " + art->dbValue("idfacturap");
				} //end if
				BlDbRecordSet *cur2 = art->mainCompany()->loadQuery(query);
				/// REALIZAMOS EL QUERY
				query = "INSERT INTO vencimientop (idfacturap, fechavencimientop, cantvencimientop, idforma_pago, refvencimientop, idproveedor) VALUES ("+art->dbValue("idfacturap")+",'"+cur2->valor("fechav")+"',"+cur2->valor("porcent")+","+cur->valor("idforma_pago")+",'"+art->dbValue("reffacturap")+"',"+art->dbValue("idproveedor")+")";
				contado = contado + BlFixed(cur2->valor("porcent"));
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
int BlForm_guardar_Post ( BlForm *art )
{
    blDebug ( "ArticuloView_guardar_post", 0 );
    try {

        BfSubForm *l = art->findChild<BfSubForm *> ( "vencimientoproveedor" );
	if (l) {
		l->setColumnValue ( "idproveedor", art->dbValue ( "idproveedor" ) );
		l->guardar();
        }

	
	/* ES EL GUARDADO DE LA FACTURA */
        BfSubForm *l1 = art->findChild<BfSubForm *> ( "vencimientop" );
	if (l1) {
		if (l1->rowCount() > 1) {

			QMessageBox msgBox;
			msgBox.setText("Hay Vencimientos generados para esta factura. Que desea hacer?");
			QPushButton *guardarButton = msgBox.addButton("Guardar", QMessageBox::ActionRole);
			QPushButton *regenerarButton = msgBox.addButton("Regenerar", QMessageBox::ActionRole);
			QPushButton *nadaButton = msgBox.addButton("Dejar Actuales", QMessageBox::ActionRole);
			msgBox.exec();
			if (msgBox.clickedButton() == guardarButton) {
				l1->setColumnValue ( "idfacturap", art->dbValue ( "idfacturap" ) );
				l1->setColumnValue ( "idproveedor", art->dbValue ( "idproveedor" ) );
				l1->guardar();
			} else if (msgBox.clickedButton() == regenerarButton) {
				QString query = "DELETE FROM vencimientop WHERE idfacturap="+ art->dbValue("idfacturap");
				art->mainCompany()->runQuery(query);
				generarVencimientos( (FacturaProveedorView *)art);
			} // end if
		} else {
			generarVencimientos((FacturaProveedorView *)art);
		} // end if
        } // end if
        return 0;
    } catch ( ... ) {
        blDebug ( "Hubo un error al guardar los vencimientos", 2 );
        return 0;
    } // end try
}




int FacturaProveedorView_FacturaProveedorView (FacturaProveedorView *factp) {
    blDebug ( "ArticuloView_ArticuloView", 0 );
    /// Agregamos el subformulario de validaciones.
    BfSubForm *l = new BfSubForm ( factp );
    l->setObjectName ( QString::fromUtf8 ( "vencimientop" ) );
    l->setMainCompany ( factp->mainCompany() );
    l->setDbTableName ( "vencimientop" );
    l->setDbFieldId ( "idvencimientop" );
    l->addSubFormHeader ( "idvencimientop", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView , QApplication::translate("TrabajadorView", "ID Vencimiento"));
    l->addSubFormHeader ( "fechavencimientop", BlDbField::DbDate, BlDbField::DbNotNull, BlSubFormHeader::DbNone , QApplication::translate ( "TrabajadorView", "Fecha" ) );
    l->addSubFormHeader ( "cantvencimientop", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNone, QApplication::translate ( "TrabajadorView", "Cantidad" ) );
    l->addSubFormHeader ( "descforma_pago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, QApplication::translate ( "TrabajadorView", "Forma de Pago" ) );
    l->addSubFormHeader ( "procesadovencimientop", BlDbField::DbBoolean, BlDbField::DbNothing, BlSubFormHeader::DbNone, QApplication::translate ( "TrabajadorView", "Procesado" ) );
    l->addSubFormHeader ( "idfacturap", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite , QApplication::translate ( "TrabajadorView", "Id Articulo" ) );
    l->addSubFormHeader ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, QApplication::translate ( "TrabajadorView", "ID Forma de Pago" ) );
    l->addSubFormHeader ( "idproveedor", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite , QApplication::translate ( "Proveedor", "Id Proveedor" ) );
    l->setInsert ( TRUE );
    l->setDelete ( TRUE );
    l->setSortingEnabled ( FALSE );
    factp->dialogChanges_setQObjectExcluido ( l->mui_list );

    factp->discounts->addTab ( l, "Vencimientos" );
    l->cargar("SELECT * FROM vencimientop WHERE idfacturap IS NULL");
    blDebug ( "END ArticuloView_ArticuloView", 0 );
    return 0;
}

///
/**
\param art
\return
**/
int FacturaProveedorView_cargarPost_Post ( FacturaProveedorView *art )
{
    blDebug ( "FacturaProveedorView_cargarPost_Post", 0 );
    BfSubForm *l = art->findChild<BfSubForm *> ( "vencimientop" );
    if ( l ) {
        l->cargar ( "SELECT * FROM vencimientop LEFT JOIN forma_pago ON vencimientop.idforma_pago = forma_pago.idforma_pago WHERE idfacturap =  " + art->dbValue("idfacturap"));
    } // end if
    blDebug ( "END FacturaProveedorView_cargarPost_Post", 0 );
    return 0;
}



