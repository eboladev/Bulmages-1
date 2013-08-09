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
