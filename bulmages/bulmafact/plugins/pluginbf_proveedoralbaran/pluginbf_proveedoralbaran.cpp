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

#include "pluginbf_proveedoralbaran.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "albaranproveedorview.h"
#include "albaranesproveedor.h"
#include "genalbpqtoolbutton.h"
#include "bltoolbutton.h"


AlbaranesProveedor *g_albaranesProveedor = NULL;
BfBulmaFact *g_pluginbf_proveedoralbaran = NULL;

///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_proveedoralbaran", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );
    g_pluginbf_proveedoralbaran = bges;


    if ( bges->company()->hasTablePrivilege ( "albaranp", "SELECT" ) ) {

        /// Miramos si existe un menu Compras
        QMenu *pPluginMenu = bges->newMenu ( _("&Compras"), "menuCompras", "menuMaestro" );
        pPluginMenu->addSeparator();

        BlAction *accionA = new BlAction ( _ ( "&Albaranes de proveedores" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-delivery-note-list.png" ) ) );
        accionA->setStatusTip ( _ ( "Albaranes de proveedores" ) );
        accionA->setWhatsThis ( _ ( "Albaranes de proveedores" ) );
        accionA->setObjectName("mui_actionProveedoresAlbaran");
        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );

        BlAction *accionB = new BlAction ( _ ( "&Nuevo albaran de proveedor" ), 0 );
        accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-delivery-note.png" ) ) );
        accionB->setStatusTip ( _ ( "Nuevo albaran de proveedor" ) );
        accionB->setWhatsThis ( _ ( "Nuevo albaran de proveedor" ) );
        accionB->setObjectName("mui_actionProveedAlbaranNuevo");
        pPluginMenu->addAction ( accionB );
        bges->Fichas->addAction ( accionB );


    } // end if


    return 0;
}

int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, "PluginBf_ProveedorAlbaran" );
    if (accion->objectName() == "mui_actionProveedoresAlbaran") {
        if ( g_albaranesProveedor ) {
            g_albaranesProveedor->hide();
            g_albaranesProveedor->show();
        } // end if
    } // end if

    if (accion->objectName() == "mui_actionProveedAlbaranNuevo") {
        AlbaranProveedorView * bud = new AlbaranProveedorView ( g_pluginbf_proveedoralbaran->company(), NULL );
        g_pluginbf_proveedoralbaran->company()->m_pWorkspace->addSubWindow ( bud );
        bud->inicializar();
        bud->show();
    } // end if
    return 0;
}

int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "albaranp", "SELECT" ) ) {
        g_albaranesProveedor = new AlbaranesProveedor ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_albaranesProveedor );
        g_albaranesProveedor->hide();
    } // end if
    return 0;
}


int ProveedorView_ProveedorView_Post ( ProveedorView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "albaranp", "SELECT" ) ) {
        AlbaranesProveedor *albaranesProveedor = new AlbaranesProveedor ( ( BfCompany * ) prov->mainCompany(), NULL, 0, BL_SELECT_MODE );
        albaranesProveedor->setEditMode();
        albaranesProveedor->setObjectName ( "listalbaranesproveedor" );
        albaranesProveedor->hideBusqueda();
        prov->mui_tab->addTab ( albaranesProveedor, "Albaranes" );
    }// end if
    return 0;
}

int ProveedorView_cargarPost_Post ( ProveedorView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "albaranp", "SELECT" ) ) {
        AlbaranesProveedor *albaranesProveedor = prov->findChild<AlbaranesProveedor *> ( "listalbaranesproveedor" );
        albaranesProveedor->setidproveedor ( prov->dbValue ( "idproveedor" ) );
        albaranesProveedor->presentar();
    }// end if
    return 0;
}// end if


int BfBuscarReferencia_on_mui_abrirtodo_clicked_Post ( BfBuscarReferencia *ref )
{
    QString SQLQuery = "SELECT * FROM albaranp WHERE refalbaranp = '" + ref->mui_referencia->text() + "'";
    BlDbRecordSet *cur = ref->mainCompany() ->loadQuery ( SQLQuery );
    while ( !cur->eof() ) {
        AlbaranProveedorView * bud = new AlbaranProveedorView ( ( BfCompany * ) ref->mainCompany(), NULL );
        ref->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
        bud->load ( cur->value( "idalbaranp" ) );
        bud->show();
        cur->nextRecord();
    } // end while
    delete cur;
    return 0;
}// end if



///
/**
\param l
\return
**/
int PedidoProveedorView_PedidoProveedorView ( PedidoProveedorView *l )
{
    BL_FUNC_DEBUG

    GenAlbProQToolButton *mui_exporta_efactura2 = new GenAlbProQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 4 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }// end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );

    
    return 0;
}


/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewAlbaranProveedorView ( BfCompany *v )
{
    AlbaranProveedorView *h = new AlbaranProveedorView ( v, 0 );
    g_plugParams = h;
    return 1;
}

/// Apertura de un elemento controlado a partir del parametro g_plugParams tabla_identificador
int Plugin_open(BfCompany * comp) {
  BL_FUNC_DEBUG
  QString cad = *((QString*)g_plugParams);
  QStringList args = cad.split("_");
  if (args[0] == "actividad") {
	AlbaranProveedorView * bud = new AlbaranProveedorView ( comp, 0 );
        comp->m_pWorkspace->addSubWindow ( bud );
	QString id =  args[1];
	bud->load(id);
        bud->show();

  } // end if
  return 0;
}




///
/**
\param l
\return
**/
int FacturaProveedorView_FacturaProveedorView ( FacturaProveedorView *l )
{

    BL_FUNC_DEBUG

    BlToolButton *agalbaran = new BlToolButton ( l );
    agalbaran->setObjectName("mui_agalbaranp12");
    
    agalbaran->setStatusTip ( "Agregar albaran" );
    agalbaran->setToolTip ( "Agregar albaran" );
    agalbaran->setMinimumSize ( QSize ( 32, 32 ) );
    agalbaran->setMaximumSize ( QSize ( 32, 32 ) );
    agalbaran->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client-delivery-note-add-to-invoice.png" ) ) );
    agalbaran->setIconSize ( QSize ( 22, 22 ) );
    agalbaran->setContentsMargins ( 0, 0, 0, 0 );
    
    
    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }// end if
    
    m_hboxLayout1->addWidget ( agalbaran );

    

    return 0;
}


int BlToolButton_released(BlToolButton *bot) {
    BL_FUNC_DEBUG
  if (bot->objectName() == "mui_agalbaranp12") {
    

      FacturaProveedorView *fact1 = (FacturaProveedorView *) bot->parent()->parent()->parent();

      /// AGREGAR ALBARAN A LA FACTURA
    
        QDialog *diag = new QDialog ( 0 );
	diag->setModal ( true );
	AlbaranesProveedor *fac = new AlbaranesProveedor ( fact1->mainCompany(), diag, 0, BL_SELECT_MODE );
	

	QObject::connect ( fac, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

	/// Hacemos que las opciones de filtrado del listado ya esten bien.
	fac->m_proveedor->setId ( fact1->dbValue ( "idproveedor" ) );
	fac->on_mui_actualizar_clicked();

	/// Lanzamos el dialogo.
	diag->exec();
	QString idalbaran = fac->idalbaranp();
	delete diag;

	/// Si no hay idfactura es que hemos abortado y por tanto cancelamos la operacion.
	if ( idalbaran == "" )
	    return 1;

	/// Creamos la factura.
	AlbaranProveedorView *bud = new AlbaranProveedorView ( fact1->mainCompany(), NULL );
	bud->load ( idalbaran );

	/// Agregamos a comentarios que albaran se corresponde.
	QString comm = fact1->dbValue ( "comentfacturap" ) + "(" + _ ( "ALBARAN: Num " ) + bud->dbValue ( "numalbaranp" ) + _ ( "Ref:" ) + " " + bud->dbValue ( "refalbaranp" ) + _ ( "Fecha:" ) + " " + bud->dbValue ( "fechaalbaranp" ) + ")\n";

	fact1->setDbValue ( "comentfacturap", comm );
	fact1->pintar();

	/// EN TEORIA SE DEBERIA COMPROBAR QUE LA FACTURA Y EL ALBARAN SON DEL MISMO CLIENTE, pero por ahora no lo hacemos.
	BlDbSubFormRecord *linea, *linea1;
	for ( int i = 0; i < bud->getlistalineas() ->rowCount(); ++i ) {
	    linea = bud->getlistalineas() ->lineaat ( i );
	    /// Los registros vacios no se tienen en cuenta.
	    if ( linea->dbValue ( "idarticulo" ) != "" ) {
		linea1 = fact1->getlistalineas() ->lineaat ( fact1->getlistalineas() ->rowCount() - 1 );
		linea1->setDbValue ( "codigocompletoarticulo", linea->dbValue ( "codigocompletoarticulo" ) );
		linea1->setDbValue ( "desclfacturap", linea->dbValue ( "desclalbaranp" ) );
		linea1->setDbValue ( "cantlfacturap", linea->dbValue ( "cantlalbaranp" ) );
		linea1->setDbValue ( "pvplfacturap", linea->dbValue ( "pvplalbaranp" ) );
		linea1->setDbValue ( "descuentolfacturap", linea->dbValue ( "descuentolalbaranp" ) );
		linea1->setDbValue ( "idarticulo", linea->dbValue ( "idarticulo" ) );
		linea1->setDbValue ( "nomarticulo", linea->dbValue ( "nomarticulo" ) );
		linea1->setDbValue ( "ivalfacturap", linea->dbValue ( "ivalalbaranp" ) );
		fact1->getlistalineas() ->newRecord();
	    } // end if
	} // end for

	/// Procesamos el albaran.
	bud->mui_procesadoalbaranp->setChecked ( true );
	bud->save();
	delete bud;

	/// Pintamos los totales.
	fact1->calculaypintatotales();
    
      /// AGREGAR ALBARAN A LA FACTURA

  } //end if
  return 0;
}
