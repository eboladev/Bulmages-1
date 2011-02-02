/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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


#include <cstdio>
#include <cstdlib>

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QList>
#include <QWidget>

#include "blfunctions.h"
#include "ticketqtoolbutton.h"
#include "presupuestoview.h"
#include "blfixed.h"
#include "facturaview.h"
#include "bfcompany.h"
#include "bldb.h"


///
/**
\param pres
\param ped
\param alb
\param fac
\param parent
**/
TicketQToolButton::TicketQToolButton ( PresupuestoView *pres, PedidoClienteView *ped, TicketClienteView *alb, FacturaView *fac , QWidget *parent ) : QToolButton ( parent )
{
    blDebug ( "TicketQToolButton::TicketQToolButton", 0 );
    m_presupuestoView = pres;
    m_pedidoClienteView = ped;
    m_albaranClienteView = alb;
    m_facturaView = fac;
    setBoton();
    blDebug ( "END TicketQToolButton::TicketQToolButton", 0 );
}


///
/**
**/
TicketQToolButton::~TicketQToolButton()
{
    blDebug ( "TicketQToolButton::~TicketQToolButton", 0 );
    blDebug ( "END TicketQToolButton::~TicketQToolButton", 0 );
}


///
/**
**/
void TicketQToolButton::setBoton()
{
    blDebug ( "TicketQToolButton::setBoton", 0 );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _ ( "Imprimir Ticket" ) );
    setToolTip ( _ ( "Imprimir Ticket" ) );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( g_confpr->valor ( CONF_PROGDATA ) + "icons/ticket.png"  ) );
    setIconSize ( QSize ( 22, 22 ) );
    blDebug ( "END TicketQToolButton::setBoton", 0 );
}


///
/**
**/
void TicketQToolButton::click()
{
    blDebug ( "ImpQToolButton::click", 0 );



    if ( m_albaranClienteView != NULL ) {


    
    m_albaranClienteView->generaRML("ticket_normal.txt");


    if (!g_confpr->valor ( CONF_TICKET_PRINTER_FILE).isEmpty() && g_confpr->valor ( CONF_TICKET_PRINTER_FILE) != "/dev/null") {
        QString comando = "cat " + g_confpr->valor(CONF_DIR_USER) + "ticket_normal.txt" + "  > " + g_confpr->valor ( CONF_TICKET_PRINTER_FILE );
        system ( comando.toAscii().data() );
    } else if (g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER) == "None") {
        blDebug("Debe establecer el parametro CONF_CUPS_DEFAULT_PRINTER o CONF_CASHBOX_FILE para abrir el cajon " , 2);
    } else {
        QString comando = "lp -d" + g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER) + " " + g_confpr->valor(CONF_DIR_USER) + "ticket_normal.txt";
        system ( comando.toAscii().data() );
    } // end if    

    
    } // end if
    blDebug ( "END ImpQToolButton::click", 0 );
}


