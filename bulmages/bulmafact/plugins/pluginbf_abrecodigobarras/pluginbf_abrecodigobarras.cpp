/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include <QMenu>
#include <QAction>
#include <QObject>
#include <QMessageBox>
#include <QInputDialog>
#include <QStringList>
#include <QTextCodec>
#include <QLocale>

#include "pluginbf_abrecodigobarras.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "facturaview.h"
#include "presupuestoview.h"
#include "pedidoclienteview.h"
#include "albaranclienteview.h"
#include "pedidoproveedorview.h"


///
/**
**/
myplugin::myplugin()
{
    _depura ( "myplugin::myplugin", 0 );
    _depura ( "END myplugin::myplugin", 0 );
}


///
/**
**/
myplugin::~myplugin()
{
    _depura ( "myplugin::~myplugin", 0 );
    _depura ( "END myplugin::~myplugin", 0 );
}


///
/**
\return
**/
void myplugin::elslot()
{
    _depura ( "myplugin::elslot", 0 );
    bool ok;
    QString text = QInputDialog::getText ( 0, _( "QInputDialog::getText()" ),
                                           _( "Introduzca codigo" ), QLineEdit::Normal, "", &ok );
    if ( ok && !text.isEmpty() ) {
        QStringList listaelem = text.split ( " " );
        if ( listaelem.at ( 0 ) == QString ( "FAC" ) ) {
				/// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
				int resur = g_plugins->lanza ("SNewFacturaView", ( BfCompany * ) mainCompany());
				if (!resur) {
					mensajeInfo("no se pudo crear instancia de factura");
					return;
				} // end if
				FacturaView *prov = (FacturaView *) g_plugParams;
            if ( prov->cargar ( listaelem.at ( 1 ) ) ) {
                delete prov;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addWindow ( prov );
            prov->show();
        } else if ( listaelem.at ( 0 ) == QString ( "PRE" ) ) {
				/// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
				int resur = g_plugins->lanza ("SNewPresupuestoView", ( BfCompany * ) mainCompany());
				if (!resur) {
					mensajeInfo("no se pudo crear instancia de pedido cliente");
					return;
				} // end if
            PresupuestoView * prov =  (PresupuestoView *) g_plugParams;
            if ( prov->cargar ( listaelem.at ( 1 ) ) ) {
                delete prov;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addWindow ( prov );
            prov->show();
        } else if ( listaelem.at ( 0 ) == QString ( "PED" ) ) {
				/// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
				int resur = g_plugins->lanza ("SNewPedidoClienteView", ( BfCompany * ) mainCompany());
				if (!resur) {
					mensajeInfo("no se pudo crear instancia de pedido cliente");
					return;
				} // end if
            PedidoClienteView * prov = (PedidoClienteView *) g_plugParams;
            if ( prov->cargar ( listaelem.at ( 1 ) ) ) {
                delete prov;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addWindow ( prov );
            prov->show();
        } else if ( listaelem.at ( 0 ) == QString ( "ALB" ) ) {

				/// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
				int resur = g_plugins->lanza ("SNewAlbaranClienteView", ( BfCompany * ) mainCompany());
				if (!resur) {
					mensajeInfo("no se pudo crear instancia de factura");
					return;
				} // end if
				AlbaranClienteView *prov = (AlbaranClienteView *) g_plugParams;
            if ( prov->cargar ( listaelem.at ( 1 ) ) ) {
                delete prov;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addWindow ( prov );
            prov->show();
        } else if ( listaelem.at ( 0 ) == QString ( "PEDP" ) ) {

            PedidoProveedorView * prov = new PedidoProveedorView ( ( BfCompany * ) mainCompany(), 0 );
            if ( prov->cargar ( listaelem.at ( 1 ) ) ) {
                delete prov;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addWindow ( prov );
            prov->show();
        } // end if
    } // end if
    _depura ( "END myplugin::elslot", 0 );
}


///
/**
\param bges
**/
void myplugin::inicializa ( Bulmafact *bges )
{
    _depura ( "myplugin::inicializa", 0 );
    /// Creamos el men&uacute;.
    setMainCompany ( bges->getcompany() );
    m_bulmafact = bges;

    /// Miramos si existe un menu Herramientas
	QMenu *pPluginMenu = bges->newMenu("&Herramientas", "menuHerramientas", "menuAcerca_de");

    QAction *accion = new QAction ( "&Apertura Rapida", 0 );
    accion->setStatusTip ( "Abre documentos a partir del codigo de barras" );
    accion->setWhatsThis ( "Abre documentos a partir del codigo de barras" );
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    pPluginMenu->addAction ( accion );
    _depura ( "END myplugin::inicializa", 0 );
}


///
/**
\param bges
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginbf_abrecodigobarras", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    myplugin *plug = new myplugin();
    plug->inicializa ( bges );
    _depura ( "END entryPoint", 0 );
    return 0;
}

