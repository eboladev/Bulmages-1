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
#include <QBuffer>

#include "pluginbt_modificadores.h"
#include "blconfiguration.h"
#include "btcompany.h"
#include "blplugins.h"
#include "btticket.h"
#include "bldockwidget.h"
#include "blapplication.h"
#include "modificadoresqtoolbutton.h"

#include "tabletcanvas.h"

//TabletCanvas *g_tablet = NULL;
BlDockWidget *g_doc1 = NULL;


int BtTicket_agregarLinea_Post ( BtTicket *tick )
{
    blDebug ( "PluginBt_Modificadores::BtTicket_agregarLinea_Post", 0 );

    BlDbRecord *item = (BlDbRecord *) g_plugParams;

    item->addDbField ( "imglalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _( "Imagen." ) );

    item->addDbField ( "idmodificador3", BlDbField::DbInt, BlDbField::DbNothing, _( "Imagen." ) );
    item->addDbField ( "idmodificador4", BlDbField::DbInt, BlDbField::DbNothing, _( "Imagen." ) );
    item->addDbField ( "idmodificador5", BlDbField::DbInt, BlDbField::DbNothing, _( "Imagen." ) );
    item->addDbField ( "idmodificador6", BlDbField::DbInt, BlDbField::DbNothing, _( "Imagen." ) );
    item->addDbField ( "idmodificador7", BlDbField::DbInt, BlDbField::DbNothing, _( "Imagen." ) );
    item->addDbField ( "idmodificador8", BlDbField::DbInt, BlDbField::DbNothing, _( "Imagen." ) );
    item->addDbField ( "idmodificador9", BlDbField::DbInt, BlDbField::DbNothing, _( "Imagen." ) );

    blDebug ( "END PluginBt_Modificadores::BtTicket_agregarLinea_Post", 0 );

    return 0;
}


///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    blDebug ( "entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbt_modificadores", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    /// Vamos a probar con un docwindow.
    /*
    g_doc1 = new BlDockWidget ( _ ( "Tablet" ), tpv, "pluginmodificadoresdock" );
    g_doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );
    g_doc1->setGeometry ( 100, 100, 100, 500 );
    g_doc1->resize ( 250, 200 );
    g_doc1->setWindowIcon(QIcon ( g_confpr->valor ( CONF_PROGDATA ) + "icons/modificadores.png"  ));
    tpv->addDockWidget ( Qt::LeftDockWidgetArea, g_doc1 );
    g_doc1->cargaconf();
    g_doc1->show();

    g_tablet = new TabletCanvas();
    g_tablet->setWindowIcon(QIcon ( g_confpr->valor ( CONF_PROGDATA ) + "icons/modificadoresg.png"  ));
    */
    
    blDebug ( "END entryPoint", 0 );
    return 0;
}


int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{

//    g_doc1->setWidget ( g_tablet );

    return 0;
}


int MTicketIVAInc_MTicketIVAInc_Post (MTicketIVAInc *tick) {
    ModificadoresQToolButton *sel = new ModificadoresQToolButton (  (BtCompany *)tick->mainCompany(), tick );
    sel->setFixedSize (48, 48);
    sel->setObjectName ( QString::fromUtf8 ( "exporta" ) );
    sel->setStatusTip ( _("Modificadores") );
    sel->setToolTip ( _("Establecer los modificadores de producto") );
    sel->setMinimumSize ( QSize ( 32, 32 ) );
    sel->setIcon ( QIcon ( g_confpr->valor ( CONF_PROGDATA ) + "icons/modificadores.png"  ) );
    sel->setIconSize ( QSize ( 32, 32 ) );    

/*    
    MTabletQToolButton *sel1 = new MTabletQToolButton (  (BtCompany *)tick->mainCompany(), tick );
    sel1->setFixedSize (48, 48);
    sel1->setObjectName ( QString::fromUtf8 ( "Captura Tablet" ) );
    sel1->setStatusTip ( _("Captura tablet") );
    sel1->setToolTip ( _("Captura tablet") );
    sel1->setMinimumSize ( QSize ( 32, 32 ) );
    sel1->setIcon ( QIcon ( g_confpr->valor ( CONF_PROGDATA ) + "icons/modificadoresg.png"  ) );
    sel1->setIconSize ( QSize ( 32, 32 ) );
*/
    QHBoxLayout *m_hboxLayout1 = tick->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( tick->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addWidget ( sel );
//    m_hboxLayout1->addWidget ( sel1 );
}



int MTicket_MTicket_Post (MTicket *tick) {
    ModificadoresQToolButton *sel = new ModificadoresQToolButton ( (BtCompany *)tick->mainCompany(), tick );
    sel->setFixedSize (48, 48);
    sel->setObjectName ( QString::fromUtf8 ( "exporta" ) );
    sel->setStatusTip ( _("Modificadores") );
    sel->setToolTip ( _("Establecer los modificadores de producto") );
    sel->setMinimumSize ( QSize ( 32, 32 ) );
    sel->setIcon ( QIcon ( g_confpr->valor ( CONF_PROGDATA ) + "icons/modificadores.png"  ) );
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


/*
int BtTicket_exportXML_Post( BtTicket *tick)
{
    blDebug ( "PluginBt_Modificadores::BtTicket_exportXML_Post", 0 );
    QString filename ("/tmp/guardado_"+tick->dbValue("nomticket")+".jpg");
    filename.remove(' ');
    blDebug ( "END PluginBt_Modificadores::BtTicket_exportXML_Post", 0 );
    return 0;
}


int BtCompany_setTicketActual(BtCompany *comp) {
    blDebug ( "PluginBt_Modificadores::BtCompany_setTicketActual", 0 );
    if (comp->ticketActual()) {
    if (comp->ticketActual()->exists("nomticket")) {
      QString filename ("/tmp/guardado_"+comp->ticketActual()->dbValue("nomticket")+".jpg");
      filename.remove(' ');
      if (g_tablet) {
	g_tablet->saveImage(filename);
      } // end if
    } // end if
    } // end if
    blDebug ( "END PluginBt_Modificadores::BtCompany_setTicketActual", 0 );
    return 0;
}

int BtCompany_setTicketActual_Post(BtCompany *comp) {
    blDebug ( "PluginBt_Modificadores::BtCompany_setTicketActual_Post", 0 );
    if (comp->ticketActual()) {
      g_tablet->erasePixmap();
      QString filename ("/tmp/guardado_"+comp->ticketActual()->dbValue("nomticket")+".jpg");
      filename.remove(' ');
      if (QFile::exists(filename)) {
	if (g_tablet) {
	  g_tablet->loadImage(filename);
	} // end if
      } // end if
    } // end if
    blDebug ( "END PluginBt_Modificadores::BtCompany_setTicketActual_Post", 0 );
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

*/
int BtTicket_insertarArticulo_Post ( BtTicket *tick )
{

    blDebug ( "PluginBt_Modificadores::BtTicket_insertarArticulo_Post", 0 );
    int valor = 0;
    /*
    static int semaforo = 0;
    
    if ( semaforo == 0 ) {
        semaforo = 1;
	  if (g_tablet->m_vacio == TRUE) {
	      tick->lineaActBtTicket()->setDbValue ( "imglalbaran", "" );
	  } else {
	      QByteArray bytes;
	      QBuffer buffer(&bytes);
	      buffer.open(QIODevice::WriteOnly);
	      g_tablet->pixmap.save(&buffer, "PNG");
	      QString text = bytes.toBase64();
	      tick->lineaActBtTicket()->setDbValue ( "imglalbaran", text );
	  } // end if 
        semaforo = 0;
    } // end if
    */
    blDebug ( "END PluginBt_Modificadores::BtTicket_insertarArticulo_Post", 0 );

    return valor;
}


