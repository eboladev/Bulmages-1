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


/*
Admin *g_admin;
BlDockWidget *g_admin1;
*/

///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    _depura ( "entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbt_modificadores", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    _depura ( "END entryPoint", 0 );
    return 0;
}


int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{
//    g_admin = new Admin ( etpv, g_admin1 );
//    g_admin1->setWidget ( g_admin );
    // ============ Pruebas con abrevs
/*
    QFrame *fr = g_main->findChild<QFrame *> ( "mui_frameabrevs" );
    if ( fr ) {
        QHBoxLayout *m_hboxLayout1 = fr->findChild<QHBoxLayout *> ( "hboxLayout1" );
        if ( !m_hboxLayout1 ) {
            m_hboxLayout1 = new QHBoxLayout ( fr );
            m_hboxLayout1->setSpacing ( 5 );
            m_hboxLayout1->setMargin ( 5 );
            m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
        } // end if
        m_hboxLayout1->addWidget ( g_admin );
    } // end if
*/
    return 0;
}


int MTicketIVAInc_MTicketIVAInc_Post (MTicketIVAInc *tick) {
    QToolButton *sel = new QToolButton ( tick );
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
    QToolButton *sel = new QToolButton ( tick );
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
      mensajeInfo("No existe el mui_plugbotones");
    } // end if
}


