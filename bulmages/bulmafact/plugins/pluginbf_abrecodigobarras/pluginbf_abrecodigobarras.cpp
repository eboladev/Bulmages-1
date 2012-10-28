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
#include "articuloview.h"
#include "presupuestoview.h"
#include "pedidoclienteview.h"
#include "albaranclienteview.h"
#include "pedidoproveedorview.h"

BfBulmaFact *g_pluginbf_abrecodigobarras = NULL;

///
/**
\param bges
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_abrecodigobarras", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_pluginbf_abrecodigobarras = bges;

    /// Miramos si existe un menu Herramientas
    QMenu *pPluginMenu = bges->newMenu ( _("&Herramientas"), "menuHerramientas", "menuAcerca_de" );

    BlAction *accionA = new BlAction ( _("&Apertura Rapida"), 0 );
    accionA->setStatusTip ( _("Abre documentos a partir del codigo de barras") );
    accionA->setWhatsThis ( _("Abre documentos a partir del codigo de barras") );
    accionA->setObjectName("mui_actionAperturaRapida");

    pPluginMenu->addAction(accionA);

    return 0;
}


int BlAction_actionTriggered(BlAction *accion)
{
    if (accion->objectName() == "mui_actionAperturaRapida") {
        bool ok;
        QString text = QInputDialog::getText ( 0, _ ( "QInputDialog::getText()" ),
                                               _ ( "Introduzca codigo" ), QLineEdit::Normal, "", &ok );
        if ( ok && !text.isEmpty() ) {
            QStringList listaelem = text.split ( " " );
            if ( listaelem.at ( 0 ) == QString ( "FAC" ) ) {
                /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
                int resur = g_plugins->run ( "SNewFacturaView", g_pluginbf_abrecodigobarras->company() );
                if ( !resur ) {
                    blMsgInfo ( _("No se pudo crear instancia de factura") );
                    return 0;
                } // end if
                FacturaView *prov = ( FacturaView * ) g_plugParams;
                if ( prov->load ( listaelem.at ( 1 ) ) ) {
                    delete prov;
                    return 0;
                } // end if
                g_pluginbf_abrecodigobarras->company() ->m_pWorkspace->addSubWindow ( prov );
                prov->show();
            } else if ( listaelem.at ( 0 ) == QString ( "PRE" ) ) {
                /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
                int resur = g_plugins->run ( "SNewPresupuestoView", g_pluginbf_abrecodigobarras->company() );
                if ( !resur ) {
                    blMsgInfo ( _("No se pudo crear instancia de pedido cliente") );
                    return 0;
                } // end if
                PresupuestoView * prov =  ( PresupuestoView * ) g_plugParams;
                if ( prov->load ( listaelem.at ( 1 ) ) ) {
                    delete prov;
                    return 0;
                } // end if
                g_pluginbf_abrecodigobarras->company() ->m_pWorkspace->addSubWindow ( prov );
                prov->show();
            } else if ( listaelem.at ( 0 ) == QString ( "PED" ) ) {
                /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
                int resur = g_plugins->run ( "SNewPedidoClienteView", g_pluginbf_abrecodigobarras->company() );
                if ( !resur ) {
                    blMsgInfo ( _("No se pudo crear instancia de pedido cliente") );
                    return 0;
                } // end if
                PedidoClienteView * prov = ( PedidoClienteView * ) g_plugParams;
                if ( prov->load ( listaelem.at ( 1 ) ) ) {
                    delete prov;
                    return 0;
                } // end if
                g_pluginbf_abrecodigobarras->company() ->m_pWorkspace->addSubWindow ( prov );
                prov->show();
            } else if ( listaelem.at ( 0 ) == QString ( "ALB" ) ) {

                /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
                int resur = g_plugins->run ( "SNewAlbaranClienteView", g_pluginbf_abrecodigobarras->company() );
                if ( !resur ) {
                    blMsgInfo ( _("No se pudo crear instancia de factura") );
                    return 0;
                } // end if
                AlbaranClienteView *prov = ( AlbaranClienteView * ) g_plugParams;
                if ( prov->load ( listaelem.at ( 1 ) ) ) {
                    delete prov;
                    return 0;
                } // end if
                g_pluginbf_abrecodigobarras->company()->m_pWorkspace->addSubWindow ( prov );
                prov->show();
            } else if ( listaelem.at ( 0 ) == QString ( "PEDP" ) ) {
                /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
                int resur = g_plugins->run ( "SNewPedidoProveedorView", g_pluginbf_abrecodigobarras->company() );
                if ( !resur ) {
                    blMsgInfo ( _("No se pudo crear instancia de pedido proveedor") );
                    return 0;
                } // end if
                PedidoProveedorView * prov = ( PedidoProveedorView *) g_plugParams;
                if ( prov->load ( listaelem.at ( 1 ) ) ) {
                    delete prov;
                    return 0;
                } // end if
                g_pluginbf_abrecodigobarras->company()->m_pWorkspace->addSubWindow ( prov );
                prov->show();
            } else  {   /// No es de un tipo conocido asi que buscamos en los articulos.
                QString query = "SELECT idarticulo FROM alias WHERE cadalias = '"+listaelem.at(0)+"'";
                BlDbRecordSet *cur = g_pluginbf_abrecodigobarras->company()->loadQuery ( query );
                while (!cur->eof() ) {
                    /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
                    int resur = g_plugins->run ( "SNewArticuloView", g_pluginbf_abrecodigobarras->company() );
                    if ( !resur ) {
                        blMsgInfo ( _("No se pudo crear instancia de Articulo") );
                        return 0;
                    } // end if
                    ArticuloView * art = ( ArticuloView *) g_plugParams;
                    art->load(cur->value("idarticulo"));
                    g_pluginbf_abrecodigobarras->company()->m_pWorkspace->addSubWindow ( art );
                    art->show();
                    cur->nextRecord();
                } // end while
                delete cur;

                query = "SELECT idarticulo FROM articulo WHERE codigocompletoarticulo = '"+listaelem.at(0)+"'";
                cur = g_pluginbf_abrecodigobarras->company()->loadQuery ( query );
                while (!cur->eof() ) {
                    /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
                    int resur = g_plugins->run ( "SNewArticuloView", g_pluginbf_abrecodigobarras->company() );
                    if ( !resur ) {
                        blMsgInfo ( _("No se pudo crear instancia de Articulo") );
                        return 0;
                    } // end if
                    ArticuloView * art = ( ArticuloView *) g_plugParams;
                    art->load(cur->value("idarticulo"));
                    g_pluginbf_abrecodigobarras->company()->m_pWorkspace->addSubWindow ( art );
                    art->show();
                    cur->nextRecord();
                } // end while
                delete cur;
            } // end if
        } // end if
    } // end if
    return 0;
}
