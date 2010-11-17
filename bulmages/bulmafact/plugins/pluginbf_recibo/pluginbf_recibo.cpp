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

///
/**
**/
MyPlugProf::MyPlugProf()
{
    blDebug ( "MyPlugProf::MyPlugProf", 0 );
    blDebug ( "END MyPlugProf::MyPlugProf", 0 );
}

///
/**
**/
MyPlugProf::~MyPlugProf()
{
    blDebug ( "MyPlugProf::~MyPlugProf", 0 );
    blDebug ( "END MyPlugProf::~MyPlugProf", 0 );
}

///
/**
**/
void MyPlugProf::elslot()
{
    blDebug ( "MyPlugProf::elslot", 0 );
    
    if ( g_recibosList ) {
        g_recibosList->hide();
        g_recibosList->show();
    } // end if
    
    blDebug ( "END MyPlugProf::elslot", 0 );
}

///
/**
**/
void MyPlugProf::elslot1()
{
    blDebug ( "MyPlugProf::elslot1", 0 );
    
    ReciboView * bud = new ReciboView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    
    blDebug ( "END MyPlugProf::elslot1", 0 );
}


///
/**
**/
void MyPlugProf::elslot2()
{
    blDebug ( "MyPlugProf::elslot2", 0 );
    
    EmitirRecibosView * bud = new EmitirRecibosView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    
    blDebug ( "END MyPlugProf::elslot2", 0 );
}

///
/**
\param bges
**/
void MyPlugProf::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "MyPlugProf::inicializa", 0 );

    if ( bges->company()->hasTablePrivilege ( "cobro", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( "&Economia", "menuEconomia", "menuMaestro" );
        pPluginMenu->addSeparator();

        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        m_bges = bges;
        setMainCompany ( bges->company() );
        QAction *listRecibos = new QAction ( _ ( "&Recibos" ), 0 );
        listRecibos->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/receive-list.png" ) ) );
        listRecibos->setStatusTip ( _ ( "Recibos" ) );
        listRecibos->setWhatsThis ( _ ( "Recibos" ) );
        pPluginMenu->addAction ( listRecibos );
        bges->Listados->addAction ( listRecibos );
        connect ( listRecibos, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        QAction *nuevoRecibo = new QAction ( _ ( "&Nuevo recibo" ), 0 );
        nuevoRecibo->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/receive.png" ) ) );
        nuevoRecibo->setStatusTip ( _ ( "Nuevo recibo" ) );
        nuevoRecibo->setWhatsThis ( _ ( "Nuevo recibo" ) );
        pPluginMenu->addAction ( nuevoRecibo );
        bges->Fichas->addAction ( nuevoRecibo );
        connect ( nuevoRecibo, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );


        QAction *emitirRecibos = new QAction ( _ ( "Recibos &Automaticos" ), 0 );
        emitirRecibos->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-delivery-note-to-invoice.png" ) ) );
        emitirRecibos->setStatusTip ( _ ( "Emitir recibos" ) );
        emitirRecibos->setWhatsThis ( _ ( "Emitir recibos" ) );
        pPluginMenu->addAction ( emitirRecibos );
        bges->Fichas->addAction ( emitirRecibos );
        connect ( emitirRecibos, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );

    } // end if
    
    blDebug ( "END MyPlugProf::inicializa", 0 );
}

///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de entrada del plugin de recibos\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_recibo", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    MyPlugProf *plug = new MyPlugProf();
    plug->inicializa ( bges );
    
    return 0;
}

int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "recibo", "SELECT" ) ) {
        g_recibosList = new RecibosList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_recibosList );
        g_recibosList->hide();
    }// end if
    
    return 0;
}


///
/**
\param l
\return
**/
int ActividadView_ActividadView ( ActividadView *l )
{

    blDebug ( "PluginRecibo_ActividadView_ActividadView", 0 );

    QToolButton *mui_generar_recibos = new QToolButton ( l->mui_plugbotones );
    mui_generar_recibos->setObjectName ( QString::fromUtf8 ( "genrecibo" ) );
    mui_generar_recibos->setStatusTip ( "Generar Pedido" );
    mui_generar_recibos->setToolTip ( "Generar Pedido" );
    mui_generar_recibos->setMinimumSize ( QSize ( 32, 32 ) );
    mui_generar_recibos->setMaximumSize ( QSize ( 32, 32 ) );
    mui_generar_recibos->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-delivery-note-to-invoice.png" ) ) );
    mui_generar_recibos->setIconSize ( QSize ( 32, 32 ) );
    mui_generar_recibos->setContentsMargins ( 0, 0, 0, 0 );



    MyPlugRecibo1 *p = new MyPlugRecibo1(l->mainCompany());

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }// end if
    p->m_actividad = l;
    m_hboxLayout1->addWidget ( mui_generar_recibos );
    p->connect(mui_generar_recibos, SIGNAL(released()), p, SLOT(elslot()));
    blDebug ( "END PluginRecibo_ActividadView_ActividadView", 0 );

    return 0;
}



// =====================


///
/**
**/
MyPlugRecibo1::MyPlugRecibo1(BlMainCompany *comp) : BlMainCompanyPointer(comp)
{
    blDebug ( "MyPlugRecibo1::MyPlugRecibo1", 0 );
    blDebug ( "END MyPlugRecibo1::MyPlugRecibo1", 0 );
}

///
/**
**/
MyPlugRecibo1::~MyPlugRecibo1()
{
    blDebug ( "MyPlugRecibo1::~MyPlugRecibo1", 0 );
    blDebug ( "END MyPlugRecibo1::~MyPlugRecibo1", 0 );
}

///
/**
**/
void MyPlugRecibo1::elslot()
{
    blDebug ( "MyPlugRecibo1::elslot", 0 );
    
    EmitirRecibosView * bud = new EmitirRecibosView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->mui_actividad->setChecked(TRUE);
    bud->mui_idactividad->setId(m_actividad->dbValue("idactividad"));
    bud->show();
    
    blDebug ( "END MyPlugRecibo1::elslot", 0 );
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
