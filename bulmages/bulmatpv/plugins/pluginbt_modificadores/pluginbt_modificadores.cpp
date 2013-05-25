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

#include <QtWidgets/QToolButton>
#include <QtCore/QBuffer>

#include "pluginbt_modificadores.h"
#include "blconfiguration.h"
#include "btcompany.h"
#include "blplugins.h"
#include "btticket.h"
#include "bldockwidget.h"
#include "blapplication.h"
#include "modificadoresqtoolbutton.h"
#include "editardescripcionqtoolbutton.h"
#include "blfunctions.h"

#include "tabletcanvas.h"

//TabletCanvas *g_tablet = NULL;
BlDockWidget *g_doc1 = NULL;

int BtTicket_agregarLinea_Post ( BtTicket *tick )
{
    BL_FUNC_DEBUG

    BlDbRecord *item = (BlDbRecord *) g_plugParams;

    item->addDbField ( "imglalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _( "Imagen." ) );

    item->addDbField ( "textolibremodificador", BlDbField::DbVarChar, BlDbField::DbNothing, _( "Texto." ) );

    item->addDbField ( "idmodificador3", BlDbField::DbInt, BlDbField::DbNothing, _( "Imagen." ) );
    item->addDbField ( "idmodificador4", BlDbField::DbInt, BlDbField::DbNothing, _( "Imagen." ) );
    item->addDbField ( "idmodificador5", BlDbField::DbInt, BlDbField::DbNothing, _( "Imagen." ) );
    item->addDbField ( "idmodificador6", BlDbField::DbInt, BlDbField::DbNothing, _( "Imagen." ) );
    item->addDbField ( "idmodificador7", BlDbField::DbInt, BlDbField::DbNothing, _( "Imagen." ) );
    item->addDbField ( "idmodificador8", BlDbField::DbInt, BlDbField::DbNothing, _( "Imagen." ) );
    item->addDbField ( "idmodificador9", BlDbField::DbInt, BlDbField::DbNothing, _( "Imagen." ) );
    

    return 0;
}


///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbt_modificadores", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    /// Vamos a probar con un docwindow.
    /*
    g_doc1 = new BlDockWidget ( _ ( "Tablet" ), tpv, "pluginmodificadoresdock" );
    g_doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );
    g_doc1->setGeometry ( 100, 100, 100, 500 );
    g_doc1->resize ( 250, 200 );
    g_doc1->setWindowIcon(QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/modificadores.png"  ));
    tpv->addDockWidget ( Qt::LeftDockWidgetArea, g_doc1 );
    g_doc1->loadConfig();
    g_doc1->show();

    g_tablet = new TabletCanvas();
    g_tablet->setWindowIcon(QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/modificadoresg.png"  ));
    */
    
    return 0;
}


int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{

//    g_doc1->setWidget ( g_tablet );

    return 0;
}


int MTicketIVAInc_MTicketIVAInc_Post (MTicketIVAInc *tick) {
    BL_FUNC_DEBUG

    ModificadoresQToolButton *modificadores = new ModificadoresQToolButton (  (BtCompany *)tick->mainCompany(), tick );
    modificadores->setFixedSize (48, 48);
    modificadores->setObjectName ( QString::fromUtf8 ( "modificadoresqtoolbutton" ) );
    modificadores->setStatusTip ( _("Modificadores") );
    modificadores->setToolTip ( _("Establecer los modificadores de producto") );
    modificadores->setMinimumSize ( QSize ( 32, 32 ) );
    modificadores->setIcon ( QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/modificadores.png"  ) );
    modificadores->setIconSize ( QSize ( 32, 32 ) );    

    EditarDescripcionQToolButton *editarDescripcion = new EditarDescripcionQToolButton (  (BtCompany *)tick->mainCompany(), tick );
    editarDescripcion->setFixedSize (48, 48);
    editarDescripcion->setObjectName ( QString::fromUtf8 ( "editardescripcionqtoolbutton" ) );
    editarDescripcion->setStatusTip ( _("Editar descripcion") );
    editarDescripcion->setToolTip ( _("Edita la descripcion del articulo") );
    editarDescripcion->setMinimumSize ( QSize ( 32, 32 ) );
    editarDescripcion->setIcon ( QIcon ( ":/Images/edit_edit.png" ) );
    editarDescripcion->setIconSize ( QSize ( 32, 32 ) );    
    

    if ( g_confpr->value(CONF_PLUGINBT_MODIFICADORES_EDIT_MODIFICADORES).isEmpty() || (g_confpr->value(CONF_PLUGINBT_MODIFICADORES_EDIT_MODIFICADORES) == "false") ) {
	modificadores->hide();
    } else {
	modificadores->show();
    } // end if
    
    if ( (g_confpr->value(CONF_PLUGINBT_MODIFICADORES_EDIT_ARTICLE_DESCRIPTION).isEmpty()) || (g_confpr->value(CONF_PLUGINBT_MODIFICADORES_EDIT_ARTICLE_DESCRIPTION) == "false") ) {
	editarDescripcion->hide();
    } else {
	editarDescripcion->show();
    } // end if
    
    
    QFrame *frame = tick->findChild<QFrame *>("mui_plugbotones");

    if (frame) {
    
	QHBoxLayout *m_hboxLayout1 = frame->findChild<QHBoxLayout *> ( "hboxLayout1" );
	if ( !m_hboxLayout1 ) {
	    m_hboxLayout1 = new QHBoxLayout ( frame );
	    m_hboxLayout1->setSpacing ( 5 );
	    m_hboxLayout1->setMargin ( 0 );
	    m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	} // end if
	
	m_hboxLayout1->addWidget ( modificadores );
	m_hboxLayout1->addWidget ( editarDescripcion );

    } else {
      
	blMsgInfo("No existe el mui_plugbotones");
	
    } // end if
  
}



int MTicket_MTicket_Post (MTicket *tick) {
    BL_FUNC_DEBUG
  
    ModificadoresQToolButton *modificadores = new ModificadoresQToolButton (  (BtCompany *)tick->mainCompany(), tick );
    modificadores->setFixedSize (48, 48);
    modificadores->setObjectName ( QString::fromUtf8 ( "modificadoresqtoolbutton" ) );
    modificadores->setStatusTip ( _("Modificadores") );
    modificadores->setToolTip ( _("Establecer los modificadores de producto") );
    modificadores->setMinimumSize ( QSize ( 32, 32 ) );
    modificadores->setIcon ( QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/modificadores.png"  ) );
    modificadores->setIconSize ( QSize ( 32, 32 ) );    

    
    EditarDescripcionQToolButton *editarDescripcion = new EditarDescripcionQToolButton (  (BtCompany *)tick->mainCompany(), tick );
    editarDescripcion->setFixedSize (48, 48);
    editarDescripcion->setObjectName ( QString::fromUtf8 ( "editardescripcionqtoolbutton" ) );
    editarDescripcion->setStatusTip ( _("Editar descripcion") );
    editarDescripcion->setToolTip ( _("Edita la descripcion del articulo") );
    editarDescripcion->setMinimumSize ( QSize ( 32, 32 ) );
    editarDescripcion->setIcon ( QIcon ( ":/Images/edit_edit.png" ) );
    editarDescripcion->setIconSize ( QSize ( 32, 32 ) );    

    
    if ( (g_confpr->value(CONF_PLUGINBT_MODIFICADORES_EDIT_MODIFICADORES).isEmpty()) || (g_confpr->value(CONF_PLUGINBT_MODIFICADORES_EDIT_MODIFICADORES) == "false") ) {
	modificadores->hide();
    } else {
	modificadores->show();
    } // end if
    
    if ( (g_confpr->value(CONF_PLUGINBT_MODIFICADORES_EDIT_ARTICLE_DESCRIPTION).isEmpty()) || (g_confpr->value(CONF_PLUGINBT_MODIFICADORES_EDIT_ARTICLE_DESCRIPTION) == "false") ) {
	editarDescripcion->hide();
    } else {
	editarDescripcion->show();
    } // end if


    QFrame *frame = tick->findChild<QFrame *>("mui_plugbotones");

    if (frame) {
    
	QHBoxLayout *m_hboxLayout1 = frame->findChild<QHBoxLayout *> ( "hboxLayout1" );
	if ( !m_hboxLayout1 ) {
	    m_hboxLayout1 = new QHBoxLayout ( frame );
	    m_hboxLayout1->setSpacing ( 5 );
	    m_hboxLayout1->setMargin ( 0 );
	    m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	} // end if
	
	m_hboxLayout1->addWidget ( modificadores );
	m_hboxLayout1->addWidget ( editarDescripcion );

    } else {
      
	blMsgInfo("No existe el mui_plugbotones");
	
    } // end if
}


/*
int BtTicket_imprimir_Post(BtTicket *tick)
{
    BL_FUNC_DEBUG
    impresionCocina(tick);
    
    return 1;
}
*/


/*
int BtTicket_exportXML_Post( BtTicket *tick)
{
    BL_FUNC_DEBUG
    QString filename ("/tmp/guardado_"+tick->dbValue("nomticket")+".jpg");
    filename.remove(' ');
    
    return 0;
}


int BtCompany_setTicketActual(BtCompany *comp) {
    BL_FUNC_DEBUG
    if (comp->ticketActual()) {
    if (comp->ticketActual()->exists("nomticket")) {
      QString filename ("/tmp/guardado_"+comp->ticketActual()->dbValue("nomticket")+".jpg");
      filename.remove(' ');
      if (g_tablet) {
	g_tablet->saveImage(filename);
      } // end if
    } // end if
    } // end if
    
    return 0;
}

int BtCompany_setTicketActual_Post(BtCompany *comp) {
    BL_FUNC_DEBUG
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
    
    return 0;
}


int BtCompany_cobrar_1(BtCompany *comp) {
      QString filename ("/tmp/guardado_"+comp->ticketActual()->dbValue("nomticket")+".jpg");
      filename.remove(' ');
      QString cadena = "rm " + filename;
      system (cadena.toLatin1());
      g_tablet->erasePixmap();
      return 0;
}

*/
int BtTicket_insertarArticulo_Post ( BtTicket *tick )
{

    BL_FUNC_DEBUG
    int valor = 0;
    /*
    static int semaforo = 0;
    
    if ( semaforo == 0 ) {
        semaforo = 1;
	  if (g_tablet->m_vacio == true) {
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
    

    return valor;
}


