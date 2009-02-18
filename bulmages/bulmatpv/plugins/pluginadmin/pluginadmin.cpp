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

#include "pluginadmin.h"
#include "btcompany.h"
#include "admin.h"
#include "plugins.h"
#include "ticket.h"
#include "bldockwidget.h"
#include "blapplication.h"


Admin *g_admin;
BlDockWidget *g_admin1;


///
/**
\return
**/
int entryPoint ( BulmaTPV *tpv )
{
    _depura ( "entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginadmin", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    /// Vamos a probar con un docwindow.
    /*
        g_admin1 = new BlDockWidget ( "Administracion", tpv, "pluginadministracion" );
        g_admin1->setFeatures ( QDockWidget::AllDockWidgetFeatures );
        g_admin1->setGeometry ( 100, 100, 100, 500 );
        g_admin1->resize ( 330, 400 );
        tpv->addDockWidget ( Qt::BottomDockWidgetArea, g_admin1 );
        g_admin1->show();
        g_admin1->cargaconf();
    */


    _depura ( "END entryPoint", 0 );
    return 0;
}

///
/**
\return
**/
int exitPoint ( BulmaTPV *tpv )
{
    _depura ( "pluginadmin::exitPoint", 0 );
    delete g_admin;
    _depura ( "END pluginadmin::exitPoint", 0 );
    return 0;
}


int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{
    g_admin = new Admin ( etpv, g_admin1 );
//    g_admin1->setWidget ( g_admin );



    // ============ Pruebas con abrevs
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

    return 0;
}
