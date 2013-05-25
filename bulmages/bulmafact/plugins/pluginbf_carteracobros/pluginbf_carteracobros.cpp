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
#include <QtCore/QTime>
#include <QtWidgets/QWidget>

#include "pluginbf_carteracobros.h"
#include "blmaincompany.h"
#include "blfunctions.h"
#include "carteracobroslist.h"


BfBulmaFact *g_pluginbf_carteracobros = NULL;



///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("DBRev-CarteraCobros", "0.11.1-0001");

    g_pluginbf_carteracobros = bges;

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_carteracobros", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );
 
    /// Miramos si existe un menu Ventas
    QMenu *pPluginMenu = bges->newMenu( _("&Compras"), "menuVentas", "menuMaestro");

    BlAction *accionA = new BlAction ( _("&Cartera de cobros"), 0 );
    accionA->setStatusTip ( _("Cartera de cobros") );
    accionA->setWhatsThis ( _("Cartera de cobros") );
    accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/receive-list.png" ) ) );
    accionA->setObjectName("mui_actionCarteraCobros");
    
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accionA );
    bges->Listados->addAction (accionA);

    
    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionCarteraCobros") {
        CarteraCobrosList *carteracobrosview = new CarteraCobrosList ( g_pluginbf_carteracobros->company() );
        g_pluginbf_carteracobros->company()->m_pWorkspace->addSubWindow ( carteracobrosview );
        carteracobrosview->show();
    } // end if 
    return 0;
}
 

///
/**
\param art
\return
**/
int ClienteView_ClienteView_Post ( ClienteView *art )
{
    BL_FUNC_DEBUG

    /// Agregamos el subformulario de validaciones.
    BfSubForm *l = new BfSubForm ( art );
    l->setObjectName ( QString::fromUtf8 ( "vencimientocliente" ) );
    l->setMainCompany ( art->mainCompany() );
    l->setDbTableName ( "vencimientocliente" );
    l->setDbFieldId ( "idvencimientocliente" );
    l->addSubFormHeader ( "idvencimientocliente", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView , QApplication::translate("TrabajadorView", "Id vencimiento"));
    l->addSubFormHeader ( "diasvencimientocliente", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNone , QApplication::translate ( "TrabajadorView", "Dias tras factura" ) );
    l->addSubFormHeader ( "porcentajevencimientocliente", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, QApplication::translate ( "TrabajadorView", "Porcentaje" ) );
    l->addSubFormHeader ( "descforma_pago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, QApplication::translate ( "TrabajadorView", "Forma de pago" ) );
    l->addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite , QApplication::translate ( "TrabajadorView", "Id cliente" ) );
    l->addSubFormHeader ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNone, QApplication::translate ( "TrabajadorView", "Id forma de pago" ) );
    l->setInsert ( true );
    l->setDelete ( true );
    l->setSortingEnabled ( false );
    art->dialogChanges_setExcludedObject ( l->mui_list );

    art->mui_tab->addTab ( l, "Vencimientos" );
    l->load("SELECT * FROM vencimientocliente WHERE idcliente IS NULL");
    
    return 0;
}


///
/**
\param art
\return
**/
int ClienteView_cargarPost_Post ( ClienteView *art )
{
    BL_FUNC_DEBUG
    BfSubForm *l = art->findChild<BfSubForm *> ( "vencimientocliente" );
    if ( l ) {
        l->load ( "SELECT * FROM vencimientocliente LEFT JOIN forma_pago ON vencimientocliente.idforma_pago = forma_pago.idforma_pago WHERE idcliente =  " + art->dbValue("idcliente"));
    } // end if
    
    return 0;
}


void generarVencimientos (FacturaView *art) {
			BlDbRecordSet *cur1 = art->mainCompany()->loadQuery("SELECT totalfactura FROM factura WHERE idfactura = " + art->dbValue("idfactura"));
			blMsgInfo( _("El total de la factura es :") + cur1->value("totalfactura"));
			BlFixed contado("0.00");

			BlDbRecordSet *cur = art->mainCompany()->loadQuery("SELECT * FROM vencimientocliente WHERE idcliente = " + art->dbValue("idcliente"));
			while (!cur->eof())  {
				QString query = "SELECT ffactura + " + cur->value("diasvencimientocliente") + " AS fechav, totalfactura / 100 * "+cur->value("porcentajevencimientocliente")+" AS porcent FROM factura WHERE idfactura = " + art->dbValue("idfactura");
				if (cur->isLastRecord()) {
					query = "SELECT ffactura + " + cur->value("diasvencimientocliente") + " AS fechav, totalfactura - "+ contado.toQString('.') +" AS porcent FROM factura WHERE idfactura = " + art->dbValue("idfactura");
				} //end if
				BlDbRecordSet *cur2 = art->mainCompany()->loadQuery(query);
				/// REALIZAMOS EL QUERY
				query = "INSERT INTO vencimientoc (idfactura, fechavencimientoc, cantvencimientoc, idforma_pago, refvencimientoc, idcliente) VALUES ("+art->dbValue("idfactura")+",'"+cur2->value("fechav")+"',"+cur2->value("porcent")+","+cur->value("idforma_pago")+",'"+art->dbValue("reffactura")+"',"+art->dbValue("idcliente")+")";
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

        BfSubForm *l = art->findChild<BfSubForm *> ( "vencimientocliente" );
	if (l) {
		l->setColumnValue ( "idcliente", art->dbValue ( "idcliente" ) );
		l->save();
        } // end if

	
	/* ES EL GUARDADO DE LA FACTURA */
        BfSubForm *l1 = art->findChild<BfSubForm *> ( "vencimientoc" );
	if (l1) {
		if (l1->rowCount() > 1) {

			QMessageBox msgBox;
			msgBox.setText("Hay vencimientos generados para esta factura. Que desea hacer?");
			QPushButton *guardarButton = msgBox.addButton("Guardar", QMessageBox::ActionRole);
			QPushButton *regenerarButton = msgBox.addButton("Regenerar", QMessageBox::ActionRole);
			QPushButton *nadaButton = msgBox.addButton("Dejar Actuales", QMessageBox::ActionRole);
			msgBox.exec();
			if (msgBox.clickedButton() == guardarButton) {
				l1->setColumnValue ( "idfactura", art->dbValue ( "idfactura" ) );
				l1->setColumnValue ( "idcliente", art->dbValue ( "idcliente" ) );
				l1->save();
			} else if (msgBox.clickedButton() == regenerarButton) {
				QString query = "DELETE FROM vencimientoc WHERE idfactura="+ art->dbValue("idfactura");
				art->mainCompany()->runQuery(query);
				generarVencimientos( (FacturaView *) art);
			} // end if
		} else {
			generarVencimientos((FacturaView *) art);
		} // end if
        } // end if
        return 0;
    } catch ( ... ) {
        
        return 1;
    } // end try
}




int FacturaView_FacturaView (FacturaView *factp) {
    BL_FUNC_DEBUG
    /// Agregamos el subformulario de validaciones.
    BfSubForm *l = new BfSubForm ( factp );
    l->setObjectName ( QString::fromUtf8 ( "vencimientoc" ) );
    l->setMainCompany ( factp->mainCompany() );
    l->setDbTableName ( "vencimientoc" );
    l->setDbFieldId ( "idvencimientoc" );
    l->addSubFormHeader ( "idvencimientoc", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView , QApplication::translate("TrabajadorView", "Id vencimiento"));
    l->addSubFormHeader ( "fechavencimientoc", BlDbField::DbDate, BlDbField::DbNotNull, BlSubFormHeader::DbNone , QApplication::translate ( "TrabajadorView", "Fecha" ) );
    l->addSubFormHeader ( "cantvencimientoc", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, QApplication::translate ( "TrabajadorView", "Cantidad" ) );
    l->addSubFormHeader ( "descforma_pago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, QApplication::translate ( "TrabajadorView", "Forma de pago" ) );
    l->addSubFormHeader ( "procesadovencimientoc", BlDbField::DbBoolean, BlDbField::DbNothing, BlSubFormHeader::DbNone, QApplication::translate ( "TrabajadorView", "Procesado" ) );
    l->addSubFormHeader ( "idfactura", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite , QApplication::translate ( "TrabajadorView", "Id factura" ) );
    l->addSubFormHeader ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, QApplication::translate ( "TrabajadorView", "Id forma de pago" ) );
    l->addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite , QApplication::translate ( "Cliente", "Id cliente" ) );
    l->setInsert ( true );
    l->setDelete ( true );
    l->setSortingEnabled ( false );
    factp->dialogChanges_setExcludedObject ( l->mui_list );

    factp->m_desdfgs->addTab ( l, "Vencimientos" );
    l->load("SELECT * FROM vencimientoc WHERE idfactura IS NULL");
    
    return 0;
}

///
/**
\param art
\return
**/
int FacturaView_cargarPost_Post ( FacturaView *art )
{
    BL_FUNC_DEBUG

    /// Para que funcione el identificador de factura debe ser valido
    if (art->dbValue("idfactura") != "") {

	BfSubForm *l = art->findChild<BfSubForm *> ( "vencimientoc" );
	if ( l ) {
		l->load ( "SELECT * FROM vencimientoc LEFT JOIN forma_pago ON vencimientoc.idforma_pago = forma_pago.idforma_pago WHERE idfactura =  " + art->dbValue("idfactura"));
	} // end if
    } // end if
    
    return 0;
}


///
/**
\param art
\return
**/
int FacturaView_beforeDelete ( FacturaView *art )
{
    BL_FUNC_DEBUG

    /// Para que funcione el identificador de factura debe ser valido
    if (art->dbValue("idfactura") != "") {

	BfSubForm *l = art->findChild<BfSubForm *> ( "vencimientoc" );
	if ( l ) {
		l->mainCompany()->runQuery ( "DELETE FROM vencimientoc WHERE idfactura =  " + art->dbValue("idfactura"));
	} // end if
    } // end if
    
    return 0;
}
