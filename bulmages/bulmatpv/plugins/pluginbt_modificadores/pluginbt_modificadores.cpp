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

#include <QToolButton>

#include "pluginbt_modificadores.h"
#include "blconfiguration.h"
#include "btcompany.h"
#include "blplugins.h"
#include "btticket.h"
#include "bldockwidget.h"
#include "blapplication.h"
#include "modificadoresqtoolbutton.h"

#include "tabletcanvas.h"

/*
Admin *g_admin;
BlDockWidget *g_admin1;
*/
TabletCanvas *g_tablet;
BlDockWidget *g_doc1 = NULL;

///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    blDebug ( "entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbt_modificadores", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    /// Vamos a probar con un docwindow.
    g_doc1 = new BlDockWidget ( _ ( "Tablet" ), tpv, "pluginmodificadoresdock" );
    g_doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );
    g_doc1->setGeometry ( 100, 100, 100, 500 );
    g_doc1->resize ( 330, 400 );
    tpv->addDockWidget ( Qt::LeftDockWidgetArea, g_doc1 );
    g_doc1->cargaconf();
    g_doc1->show();    
    
    g_tablet = new TabletCanvas();
    
    blDebug ( "END entryPoint", 0 );
    return 0;
}


int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{

    g_doc1->setWidget ( g_tablet );

    return 0;
}


int MTicketIVAInc_MTicketIVAInc_Post (MTicketIVAInc *tick) {
    ModificadoresQToolButton *sel = new ModificadoresQToolButton (  (BtCompany *)tick->mainCompany(), tick );
    sel->setFixedSize (48, 48);
    sel->setObjectName ( QString::fromUtf8 ( "exporta" ) );
    sel->setStatusTip ( "Modificadores" );
    sel->setToolTip ( "Establecer los modificadores de producto" );
    sel->setMinimumSize ( QSize ( 32, 32 ) );
    sel->setIcon ( QIcon ( g_confpr->valor ( CONF_PROGDATA ) + "icons/q19.png"  ) );
    sel->setIconSize ( QSize ( 32, 32 ) );    
    
    QHBoxLayout *m_hboxLayout1 = tick->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( tick->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addWidget ( sel );
}



int MTicket_MTicket_Post (MTicket *tick) {
    ModificadoresQToolButton *sel = new ModificadoresQToolButton ( (BtCompany *)tick->mainCompany(), tick );
    sel->setFixedSize (48, 48);
    sel->setObjectName ( QString::fromUtf8 ( "exporta" ) );
    sel->setStatusTip ( "Modificadores" );
    sel->setToolTip ( "Establecer los modificadores de producto" );
    sel->setMinimumSize ( QSize ( 32, 32 ) );
    sel->setIcon ( QIcon ( g_confpr->valor ( CONF_PROGDATA ) + "icons/q19.png"  ) );
    sel->setIconSize ( QSize ( 32, 32 ) );    
    
    QFrame *frame = tick->findChild<QFrame *>("mui_plugbotones");
//    QHBoxLayout *m_hboxLayout1 = tick->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if (frame) {
      QHBoxLayout *m_hboxLayout1 = frame->findChild<QHBoxLayout *> ( "hboxLayout1" );
      if ( !m_hboxLayout1 ) {
//	  m_hboxLayout1 = new QHBoxLayout ( tick->mui_plugbotones );
	  m_hboxLayout1 = new QHBoxLayout ( frame );
	  m_hboxLayout1->setSpacing ( 5 );
	  m_hboxLayout1->setMargin ( 0 );
	  m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
      } // end if
      m_hboxLayout1->addWidget ( sel );
    } else {
      blMsgInfo("No existe el mui_plugbotones");
    } // end if
}


/*
int BtTicket_imprimir_Post(BtTicket *tick)
{
    blDebug ( "PluginBt_PrinterCocina::BtTicket_imprimir", 0 );
    impresionCocina(tick);
    blDebug ( "END PluginBt_PrinterCocina::BtTicket_imprimir", 0 );
    return 1;
}
*/

int BtTicket_exportXML_Post( BtTicket *tick)
{
    blDebug ( "PluginBt_Modificadores::BtTicket_exportXML_Post", 0 );
    QString filename ("/tmp/guardado_"+tick->dbValue("nomticket")+".jpg");
    filename.remove(' ');
//    g_tablet->saveImage(filename);
    blDebug ( "END PluginBt_Modificadores::BtTicket_exportXML_Post", 0 );
    return 0;
}


int BtCompany_setTicketActual(BtCompany *comp) {
    if (comp->ticketActual()) {
      QString filename ("/tmp/guardado_"+comp->ticketActual()->dbValue("nomticket")+".jpg");
      filename.remove(' ');
      g_tablet->saveImage(filename);
    } // end if
        return 0;
}

int BtCompany_setTicketActual_Post(BtCompany *comp) {
    if (comp->ticketActual()) {
      g_tablet->erasePixmap();
      QString filename ("/tmp/guardado_"+comp->ticketActual()->dbValue("nomticket")+".jpg");
      filename.remove(' ');
      if (QFile::exists(filename)) {
	g_tablet->loadImage(filename);
      } // end if
    } // end if
        return 0;
}


int BtCompany_cobrar_1(BtCompany *comp) {
      QString filename ("/tmp/guardado_"+comp->ticketActual()->dbValue("nomticket")+".jpg");
      filename.remove(' ');
      QString cadena = "rm " + filename;
      system (cadena.toAscii());
      g_tablet->erasePixmap();
      return 0;
}

/*
int BtCompany_cobrar_1(BtCompany *comp) {
    blDebug ( "PluginBt_PrinterCocina::BtCompany_cobrar_1", 0 );
    impresionCocina(comp->ticketActual());
    blDebug ( "END PluginBt_PrinterCocina::BtCompany_cobrar_1", 0 );
    return 0;
}
*/