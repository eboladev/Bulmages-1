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

#include "pluginbf_carteracobros.h"
#include "blmaincompany.h"
#include "blfunctions.h"
#include "carteracobroslist.h"

///
/**
**/
PluginBf_CarteraCobros::PluginBf_CarteraCobros()
{
    blDebug ( "PluginBf_CarteraCobros::PluginBf_CarteraCobros", 0 );
    blDebug ( "END PluginBf_CarteraCobros::PluginBf_CarteraCobros", 0 );
}

///
/**
**/
PluginBf_CarteraCobros::~PluginBf_CarteraCobros()
{
    blDebug ( "PluginBf_CarteraCobros::~PluginBf_CarteraCobros", 0 );
    blDebug ( "END PluginBf_CarteraCobros::~PluginBf_CarteraCobros", 0 );
}


///
/**
**/
void PluginBf_CarteraCobros::elslot()
{
    blDebug ( "PluginBf_CarteraCobros::elslot", 0 );
    CarteraCobrosList *carteracobrosview = new CarteraCobrosList ( ( BfCompany * ) m_conexionbase );
    m_bulmafact->workspace() ->addSubWindow ( carteracobrosview );
    carteracobrosview->show();
    blDebug ( "END PluginBf_CarteraCobros::elslot", 0 );
}


///
/**
\param bges
**/
void PluginBf_CarteraCobros::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "PluginBf_CarteraCobros::inicializa", 0 );
    /// Creamos el men&uacute;.
    m_conexionbase = bges->company();
    m_bulmafact = bges;

    /// Miramos si existe un menu Ventas
	QMenu *pPluginMenu = bges->newMenu("&Compras", "menuVentas", "menuMaestro");

    QAction *accion = new QAction ( "&Cartera de cobros", 0 );
    accion->setStatusTip ( "Cartera de cobros" );
    accion->setWhatsThis ( "Cartera de cobros" );
    accion->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/receive-list.png" ) ) );
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accion );
	bges->Listados->addAction (accion);
    blDebug ( "END PluginBf_CarteraCobros::inicializa", 0 );
}






///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de Entrada del plugin de cartera de cobros\n", 0 );

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("DBRev-CarteraCobros", "0.11.1-0001");

    PluginBf_CarteraCobros *plug = new PluginBf_CarteraCobros();
    plug->inicializa ( bges );
    return 0;
}


///
/**
\param art
\return
**/
int ClienteView_ClienteView_Post ( ClienteView *art )
{
    blDebug ( "ClienteView_ClienteView_Post", 0 );

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
    l->setInsert ( TRUE );
    l->setDelete ( TRUE );
    l->setSortingEnabled ( FALSE );
    art->dialogChanges_setQObjectExcluido ( l->mui_list );

    art->mui_tab->addTab ( l, "Vencimientos" );
    l->cargar("SELECT * FROM vencimientocliente WHERE idcliente IS NULL");
    blDebug ( "END ClienteView_ClienteView_Post", 0 );
    return 0;
}


///
/**
\param art
\return
**/
int ClienteView_cargarPost_Post ( ClienteView *art )
{
    blDebug ( "ArticuloView_cargar", 0 );
    BfSubForm *l = art->findChild<BfSubForm *> ( "vencimientocliente" );
    if ( l ) {
        l->cargar ( "SELECT * FROM vencimientocliente LEFT JOIN forma_pago ON vencimientocliente.idforma_pago = forma_pago.idforma_pago WHERE idcliente =  " + art->dbValue("idcliente"));
    } // end if
    blDebug ( "END ArticuloView_cargar", 0 );
    return 0;
}


void generarVencimientos (FacturaView *art) {
			BlDbRecordSet *cur1 = art->mainCompany()->loadQuery("SELECT totalfactura FROM factura WHERE idfactura = " + art->dbValue("idfactura"));
			blMsgInfo( "El total de la factura es :" + cur1->valor("totalfactura"));
			BlFixed contado("0.00");

			BlDbRecordSet *cur = art->mainCompany()->loadQuery("SELECT * FROM vencimientocliente WHERE idcliente = " + art->dbValue("idcliente"));
			while (!cur->eof())  {
				QString query = "SELECT ffactura + " + cur->valor("diasvencimientocliente") + " AS fechav, totalfactura / 100 * "+cur->valor("porcentajevencimientocliente")+" AS porcent FROM factura WHERE idfactura = " + art->dbValue("idfactura");
				if (cur->isLastRecord()) {
					query = "SELECT ffactura + " + cur->valor("diasvencimientocliente") + " AS fechav, totalfactura - "+ contado.toQString('.') +" AS porcent FROM factura WHERE idfactura = " + art->dbValue("idfactura");
				} //end if
				BlDbRecordSet *cur2 = art->mainCompany()->loadQuery(query);
				/// REALIZAMOS EL QUERY
				query = "INSERT INTO vencimientoc (idfactura, fechavencimientoc, cantvencimientoc, idforma_pago, refvencimientoc, idcliente) VALUES ("+art->dbValue("idfactura")+",'"+cur2->valor("fechav")+"',"+cur2->valor("porcent")+","+cur->valor("idforma_pago")+",'"+art->dbValue("reffactura")+"',"+art->dbValue("idcliente")+")";
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
    blDebug ( "BlForm_guardar_Post", 0 );
    try {

        BfSubForm *l = art->findChild<BfSubForm *> ( "vencimientocliente" );
	if (l) {
		l->setColumnValue ( "idcliente", art->dbValue ( "idcliente" ) );
		l->guardar();
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
				l1->guardar();
			} else if (msgBox.clickedButton() == regenerarButton) {
				QString query = "DELETE FROM vencimientoc WHERE idfactura="+ art->dbValue("idfactura");
				art->mainCompany()->runQuery(query);
				generarVencimientos( (FacturaView *)art);
			} // end if
		} else {
			generarVencimientos((FacturaView *)art);
		} // end if
        } // end if
            blDebug ( "END BlForm_guardar_Post", 0 );
        return 0;
    } catch ( ... ) {
        blDebug ( "Hubo un error al guardar los vencimientos", 2 );
        return 0;
    } // end try
}




int FacturaView_FacturaView (FacturaView *factp) {
    blDebug ( "FacturaView_FacturaView", 0 );
    /// Agregamos el subformulario de validaciones.
    BfSubForm *l = new BfSubForm ( factp );
    l->setObjectName ( QString::fromUtf8 ( "vencimientoc" ) );
    l->setMainCompany ( factp->mainCompany() );
    l->setDbTableName ( "vencimientoc" );
    l->setDbFieldId ( "idvencimientoc" );
    l->addSubFormHeader ( "idvencimientoc", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView , QApplication::translate("TrabajadorView", "Id vencimiento"));
    l->addSubFormHeader ( "fechavencimientoc", BlDbField::DbDate, BlDbField::DbNotNull, BlSubFormHeader::DbNone , QApplication::translate ( "TrabajadorView", "Fecha" ) );
    l->addSubFormHeader ( "cantvencimientoc", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNone, QApplication::translate ( "TrabajadorView", "Cantidad" ) );
    l->addSubFormHeader ( "descforma_pago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, QApplication::translate ( "TrabajadorView", "Forma de pago" ) );
    l->addSubFormHeader ( "procesadovencimientoc", BlDbField::DbBoolean, BlDbField::DbNothing, BlSubFormHeader::DbNone, QApplication::translate ( "TrabajadorView", "Procesado" ) );
    l->addSubFormHeader ( "idfactura", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite , QApplication::translate ( "TrabajadorView", "Id factura" ) );
    l->addSubFormHeader ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, QApplication::translate ( "TrabajadorView", "Id forma de pago" ) );
    l->addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite , QApplication::translate ( "Cliente", "Id cliente" ) );
    l->setInsert ( TRUE );
    l->setDelete ( TRUE );
    l->setSortingEnabled ( FALSE );
    factp->dialogChanges_setQObjectExcluido ( l->mui_list );

    factp->m_desdfgs->addTab ( l, "Vencimientos" );
    l->cargar("SELECT * FROM vencimientoc WHERE idfactura IS NULL");
    blDebug ( "END FacturaView_FacturaView", 0 );
    return 0;
}

///
/**
\param art
\return
**/
int FacturaView_cargarPost_Post ( FacturaView *art )
{
    blDebug ( "FacturaView_cargarPost_Post", 0 );

    /// Para que funcione el identificador de factura debe ser valido
    if (art->dbValue("idfactura") != "") {

	BfSubForm *l = art->findChild<BfSubForm *> ( "vencimientoc" );
	if ( l ) {
		l->cargar ( "SELECT * FROM vencimientoc LEFT JOIN forma_pago ON vencimientoc.idforma_pago = forma_pago.idforma_pago WHERE idfactura =  " + art->dbValue("idfactura"));
	} // end if
    } // end if
    blDebug ( "END FacturaView_cargarPost_Post", 0 );
    return 0;
}


///
/**
\param art
\return
**/
int FacturaView_borrarPre ( FacturaView *art )
{
    blDebug ( "FacturaView_borrarPre", 0 );

    /// Para que funcione el identificador de factura debe ser valido
    if (art->dbValue("idfactura") != "") {

	BfSubForm *l = art->findChild<BfSubForm *> ( "vencimientoc" );
	if ( l ) {
		l->mainCompany()->runQuery ( "DELETE FROM vencimientoc WHERE idfactura =  " + art->dbValue("idfactura"));
	} // end if
    } // end if
    blDebug ( "END FacturaView_borrarPre", 0 );
    return 0;
}
