/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *f
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

#include <QtWidgets/QPushButton>

#include "pluginbt_ticketdevuelto.h"
#include "blfunctions.h"
#include "btcompany.h"
#include "blplugins.h"
#include "btticket.h"
#include "blapplication.h"
#include "ticketsdevueltos.h"


MyDevButtonTD * g_plug;
///
/**
**/
MyDevButtonTD::MyDevButtonTD ( const QString & text, QWidget * parent, BtCompany *emp ) : QPushButton ( text,  parent )
{
    BL_FUNC_DEBUG
    m_emp = emp;
    connect ( this, SIGNAL ( released() ), this, SLOT ( on_click() ) );
    
}


///
/**
**/
MyDevButtonTD::~MyDevButtonTD()
{
    BL_FUNC_DEBUG
}


///
/**
\return
**/
void MyDevButtonTD::on_click (  )
{
    BL_FUNC_DEBUG
    QDialog *diag = new QDialog;
    TicketsDevueltos *camb = new TicketsDevueltos ( m_emp, diag );
    diag->setModal ( true );

    /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
    /// para que sea redimensionable y aparezca el titulo de la ventana.
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget ( camb );
    layout->setMargin ( 0 );
    layout->setSpacing ( 0 );
    diag->setLayout ( layout );
    diag->setWindowTitle ( _ ( "Tickets olvidados" ) );

    diag->exec();

    delete diag;

    
}


///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    tpv->company()->dbPatchVersionCheck("PluginBt_TicketDevuelto", "0.11.1-0001");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbt_ticketdevuelto", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    
    return 0;
}

int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{
    // ============ Pruebas con abrevs
    g_plug = new MyDevButtonTD ( "OLVIDADOS", etpv, etpv );
    g_plug->setMinimumSize ( QSize ( 72, 72 ) );
    g_plug->setMaximumSize ( QSize ( 200, 72 ) );


    QFrame *fr = g_main->findChild<QFrame *> ( "mui_frameabrevs" );
    if ( fr ) {
        QHBoxLayout *m_hboxLayout1 = fr->findChild<QHBoxLayout *> ( "hboxLayout1" );
        if ( !m_hboxLayout1 ) {
            m_hboxLayout1 = new QHBoxLayout ( fr );
            m_hboxLayout1->setSpacing ( 5 );
            m_hboxLayout1->setMargin ( 5 );
            m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
        } // end if
        m_hboxLayout1->addWidget ( g_plug );
    } // end if
    return 0;
}



