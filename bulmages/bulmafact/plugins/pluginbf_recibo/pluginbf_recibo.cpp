/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
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
#include <QToolButton>

#include "pluginbf_recibo.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "reciboview.h"
#include "reciboslist.h"
#include "emitirrecibosview.h"

RecibosList *g_recibosList = NULL;
BfBulmaFact *g_pluginbf_recibo = NULL;


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
    blBindTextDomain ( "pluginbf_recibo", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_pluginbf_recibo = bges;

    if ( bges->company()->hasTablePrivilege ( "cobro", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( _("&Economia"), "menuEconomia", "menuMaestro" );
        pPluginMenu->addSeparator();

        BlAction *accionA = new BlAction ( _ ( "&Recibos" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/receive-list.png" ) ) );
        accionA->setStatusTip ( _ ( "Recibos" ) );
        accionA->setWhatsThis ( _ ( "Recibos" ) );
        accionA->setObjectName("mui_actionRecibos");
        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );

        BlAction *accionB = new BlAction ( _ ( "&Nuevo recibo" ), 0 );
        accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/receive.png" ) ) );
        accionB->setStatusTip ( _ ( "Nuevo recibo" ) );
        accionB->setWhatsThis ( _ ( "Nuevo recibo" ) );
        accionB->setObjectName("mui_actionReciboNuevo");
        pPluginMenu->addAction ( accionB );
        bges->Fichas->addAction ( accionB );


        BlAction *accionC = new BlAction ( _ ( "Recibos &Automaticos" ), 0 );
        accionC->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-delivery-note-to-invoice.png" ) ) );
        accionC->setStatusTip ( _ ( "Emitir recibos" ) );
        accionC->setWhatsThis ( _ ( "Emitir recibos" ) );
        accionC->setObjectName("mui_actionRecibosEmitir");
        pPluginMenu->addAction ( accionC );
        bges->Fichas->addAction ( accionC );

    } // end if

    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, "PluginBf_Recibo" );

    if (accion->objectName() == "mui_actionRecibos") {
        if ( g_recibosList ) {
            g_recibosList->hide();
            g_recibosList->show();
        } // end if
    } // end if

    if (accion->objectName() == "mui_actionReciboNuevo") {
        ReciboView * bud = new ReciboView ( g_pluginbf_recibo->company(), NULL );
        g_pluginbf_recibo->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if

    if (accion->objectName() == "mui_actionRecibosEmitir") {
        EmitirRecibosView * bud = new EmitirRecibosView ( g_pluginbf_recibo->company(), NULL );
        g_pluginbf_recibo->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if

    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "recibo", "SELECT" ) ) {
        g_recibosList = new RecibosList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_recibosList );
        g_recibosList->hide();
    } // end if
    
    return 0;
}


///
/**
\param l
\return
**/
int ActividadView_ActividadView ( ActividadView *l )
{

    BL_FUNC_DEBUG

    QToolButton *mui_generar_recibos = new QToolButton ( l->mui_plugbotones );
    mui_generar_recibos->setObjectName ( QString::fromUtf8 ( "genrecibo" ) );
    mui_generar_recibos->setStatusTip ( _("Generar pedido") );
    mui_generar_recibos->setToolTip ( _("Generar pedido") );
    mui_generar_recibos->setMinimumSize ( QSize ( 32, 32 ) );
    mui_generar_recibos->setMaximumSize ( QSize ( 32, 32 ) );
    mui_generar_recibos->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-delivery-note-to-invoice.png" ) ) );
    mui_generar_recibos->setIconSize ( QSize ( 32, 32 ) );
    mui_generar_recibos->setContentsMargins ( 0, 0, 0, 0 );



    EmitirRecibos *p = new EmitirRecibos(l->mainCompany());

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    p->m_actividad = l;
    m_hboxLayout1->addWidget ( mui_generar_recibos );
    p->connect(mui_generar_recibos, SIGNAL(released()), p, SLOT(elslot()));
    

    return 0;
}



// =====================


///
/**
**/
EmitirRecibos::EmitirRecibos(BlMainCompany *comp) : BlMainCompanyPointer(comp)
{
    BL_FUNC_DEBUG
    
}

///
/**
**/
EmitirRecibos::~EmitirRecibos()
{
    BL_FUNC_DEBUG
    
}

///
/**
**/
void EmitirRecibos::elslot()
{
    BL_FUNC_DEBUG
    
    EmitirRecibosView * bud = new EmitirRecibosView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->mui_actividad->setChecked(TRUE);
    bud->mui_idactividad->setId(m_actividad->dbValue("idactividad"));
    bud->show();
    
    
}

// ==========================





/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewReciboView ( BfCompany *v )
{
    ReciboView *h = new ReciboView ( v, 0 );
    g_plugParams = h;
    return 1;
}
