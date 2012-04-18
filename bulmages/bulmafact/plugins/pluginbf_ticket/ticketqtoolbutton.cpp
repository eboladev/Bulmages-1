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
#include "facturaview.h"
#include "pedidoclienteview.h"
#include "ticketclienteview.h"
#include "albaranclienteview.h"

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
TicketQToolButton::TicketQToolButton ( PresupuestoView *pres, PedidoClienteView *ped, TicketClienteView *alb, FacturaView *fac , AlbaranClienteView *alb1, QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    m_presupuestoView = pres;
    m_pedidoClienteView = ped;
    m_albaranClienteView = alb;
    m_facturaView = fac;
    m_albaran1ClienteView = alb1;
    setBoton();
    
}


///
/**
**/
TicketQToolButton::~TicketQToolButton()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void TicketQToolButton::setBoton()
{
    BL_FUNC_DEBUG
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _ ( "Imprimir Ticket" ) );
    setToolTip ( _ ( "Imprimir Ticket" ) );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/ticket.png"  ) );
    setIconSize ( QSize ( 22, 22 ) );
}


///
/**
**/
void TicketQToolButton::click()
{
    BL_FUNC_DEBUG

       if ( m_presupuestoView != NULL ) {
    
	  m_presupuestoView->generateRML("presupuesto_ticket.txt");
	  
	  if (!g_confpr->value( CONF_TICKET_PRINTER_FILE).isEmpty() && g_confpr->value( CONF_TICKET_PRINTER_FILE) != "/dev/null") {
	      QString comando = "cat " + g_confpr->value(CONF_DIR_USER) + "presupuesto_ticket.txt" + "  > " + g_confpr->value( CONF_TICKET_PRINTER_FILE );
	      system ( comando.toAscii().data() );
	  } else if (!g_confpr->value( CONF_CASHBOX_FILE).isEmpty() && g_confpr->value( CONF_CASHBOX_FILE) != "/dev/null") {
	      QString comando = "cat " + g_confpr->value(CONF_DIR_USER) + "presupuesto_ticket.txt" + "  > " + g_confpr->value( CONF_CASHBOX_FILE );
	      system ( comando.toAscii().data() );
	  } else if (!g_confpr->value(CONF_CUPS_TICKET_PRINTER).isEmpty() && g_confpr->value(CONF_CUPS_TICKET_PRINTER) != "None") {
		      blRawPrint( "presupuesto_ticket.txt", TRUE, g_confpr->value( CONF_TICKET_PRINTER_FILE));
	  } else if (g_confpr->value(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->value(CONF_CUPS_DEFAULT_PRINTER) == "None") {
	      blMsgError(_("Debe establecer el parametro 'CONF_CUPS_DEFAULT_PRINTER' o 'CONF_CASHBOX_FILE' para abrir el cajon."));
	  } else {
		      blRawPrint( "presupuesto_ticket.txt");
	  } // end if  
	  
    } else   if ( m_pedidoClienteView != NULL ) {
    
	  m_pedidoClienteView->generateRML("pedido_ticket.txt");
	  
	  if (!g_confpr->value( CONF_TICKET_PRINTER_FILE).isEmpty() && g_confpr->value( CONF_TICKET_PRINTER_FILE) != "/dev/null") {
	      QString comando = "cat " + g_confpr->value(CONF_DIR_USER) + "pedido_ticket.txt" + "  > " + g_confpr->value( CONF_TICKET_PRINTER_FILE );
	      system ( comando.toAscii().data() );
	  } else if (!g_confpr->value( CONF_CASHBOX_FILE).isEmpty() && g_confpr->value( CONF_CASHBOX_FILE) != "/dev/null") {
	      QString comando = "cat " + g_confpr->value(CONF_DIR_USER) + "pedido_ticket.txt" + "  > " + g_confpr->value( CONF_CASHBOX_FILE );
	      system ( comando.toAscii().data() );
	  } else if (!g_confpr->value(CONF_CUPS_TICKET_PRINTER).isEmpty() && g_confpr->value(CONF_CUPS_TICKET_PRINTER) != "None") {
		      blRawPrint( "pedido_ticket.txt", TRUE, g_confpr->value( CONF_TICKET_PRINTER_FILE));
	  } else if (g_confpr->value(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->value(CONF_CUPS_DEFAULT_PRINTER) == "None") {
	      blMsgError(_("Debe establecer el parametro 'CONF_CUPS_DEFAULT_PRINTER' o 'CONF_CASHBOX_FILE' para abrir el cajon."));
	  } else {
		      blRawPrint( "pedido_ticket.txt");
	  } // end if  
	  
    } else   if ( m_albaranClienteView != NULL ) {
    
	  m_albaranClienteView->generateRML("ticket_normal.txt");
	  
	  if (!g_confpr->value( CONF_TICKET_PRINTER_FILE).isEmpty() && g_confpr->value( CONF_TICKET_PRINTER_FILE) != "/dev/null") {
	      QString comando = "cat " + g_confpr->value(CONF_DIR_USER) + "ticket_normal.txt" + "  > " + g_confpr->value( CONF_TICKET_PRINTER_FILE );
	      system ( comando.toAscii().data() );
	  } else if (!g_confpr->value( CONF_CASHBOX_FILE).isEmpty() && g_confpr->value( CONF_CASHBOX_FILE) != "/dev/null") {
	      QString comando = "cat " + g_confpr->value(CONF_DIR_USER) + "ticket_normal.txt" + "  > " + g_confpr->value( CONF_CASHBOX_FILE );
	      system ( comando.toAscii().data() );
	  } else if (!g_confpr->value(CONF_CUPS_TICKET_PRINTER).isEmpty() && g_confpr->value(CONF_CUPS_TICKET_PRINTER) != "None") {
		      blRawPrint( "ticket_normal.txt", TRUE, g_confpr->value( CONF_TICKET_PRINTER_FILE));
	  } else if (g_confpr->value(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->value(CONF_CUPS_DEFAULT_PRINTER) == "None") {
	      blMsgError(_("Debe establecer el parametro 'CONF_CUPS_DEFAULT_PRINTER' o 'CONF_CASHBOX_FILE' para abrir el cajon."));
	  } else {
		      blRawPrint( "ticket_normal.txt");
	  } // end if  
	  
    } else     if ( m_albaran1ClienteView != NULL ) {
    
	  m_albaran1ClienteView->generateRML("albaran_ticket.txt");
	  
	  if (!g_confpr->value( CONF_TICKET_PRINTER_FILE).isEmpty() && g_confpr->value( CONF_TICKET_PRINTER_FILE) != "/dev/null") {
	      QString comando = "cat " + g_confpr->value(CONF_DIR_USER) + "albaran_ticket.txt" + "  > " + g_confpr->value( CONF_TICKET_PRINTER_FILE );
	      system ( comando.toAscii().data() );
	  } else if (!g_confpr->value( CONF_CASHBOX_FILE).isEmpty() && g_confpr->value( CONF_CASHBOX_FILE) != "/dev/null") {
	      QString comando = "cat " + g_confpr->value(CONF_DIR_USER) + "albaran_ticket.txt" + "  > " + g_confpr->value( CONF_CASHBOX_FILE );
	      system ( comando.toAscii().data() );
	  } else if (!g_confpr->value(CONF_CUPS_TICKET_PRINTER).isEmpty() && g_confpr->value(CONF_CUPS_TICKET_PRINTER) != "None") {
		      blRawPrint( "albaran_ticket.txt", TRUE, g_confpr->value( CONF_TICKET_PRINTER_FILE));
	  } else if (g_confpr->value(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->value(CONF_CUPS_DEFAULT_PRINTER) == "None") {
	      blMsgError(_("Debe establecer el parametro 'CONF_CUPS_DEFAULT_PRINTER' o 'CONF_CASHBOX_FILE' para abrir el cajon."));
	  } else {
		      blRawPrint( "albaran_ticket.txt");
	  } // end if  
    } else     if ( m_facturaView != NULL ) {
    
	  m_facturaView->generateRML("factura_ticket.txt");
	  
	  if (!g_confpr->value( CONF_TICKET_PRINTER_FILE).isEmpty() && g_confpr->value( CONF_TICKET_PRINTER_FILE) != "/dev/null") {
	      QString comando = "cat " + g_confpr->value(CONF_DIR_USER) + "factura_ticket.txt" + "  > " + g_confpr->value( CONF_TICKET_PRINTER_FILE );
	      system ( comando.toAscii().data() );
	  } else if (!g_confpr->value( CONF_CASHBOX_FILE).isEmpty() && g_confpr->value( CONF_CASHBOX_FILE) != "/dev/null") {
	      QString comando = "cat " + g_confpr->value(CONF_DIR_USER) + "factura_ticket.txt" + "  > " + g_confpr->value( CONF_CASHBOX_FILE );
	      system ( comando.toAscii().data() );
	  } else if (!g_confpr->value(CONF_CUPS_TICKET_PRINTER).isEmpty() && g_confpr->value(CONF_CUPS_TICKET_PRINTER) != "None") {
		      blRawPrint( "factura_ticket.txt", TRUE, g_confpr->value( CONF_TICKET_PRINTER_FILE));
	  } else if (g_confpr->value(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->value(CONF_CUPS_DEFAULT_PRINTER) == "None") {
	      blMsgError(_("Debe establecer el parametro 'CONF_CUPS_DEFAULT_PRINTER' o 'CONF_CASHBOX_FILE' para abrir el cajon."));
	  } else {
		      blRawPrint( "factura_ticket.txt");
	  } // end if  
    }
    
}


