/***************************************************************************
 *                                                                         *
 *   BulmaTPV remote client for Nintendo DS console.                       *
 *   -----------------------------------------------                       *
 *                                                                         *
 *   Copyright (C) 2010 by Fco. Javier M. C.                               *
 *   comercial@todo-redes.com                                              *
 *   http://www.todo-redes.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#include "bndsticket.h"


BndsTicket::BndsTicket()
{
    m_nomTicket = defaultTicketName();
    m_total = 0.0;
}


BndsTicket::~BndsTicket()
{
}


string BndsTicket::defaultTicketName()
{
    return string("Ticket Actual");
}


void BndsTicket::addArticle(BndsArticle* article)
{

    /// Recorre los articulos del ticket para ver si ya existe. Si existe suma 1 unidad a la cantidad.
    list<BndsTicketLine*> ticketLinesList = ticketLines();
    bool previousExist = FALSE;
    
    for (list<BndsTicketLine*>::iterator itTicketLine = ticketLinesList.begin(); itTicketLine != ticketLinesList.end(); itTicketLine++) {

	if ( (*itTicketLine)->idArticle() == article->idArticle() ) {
	    (*itTicketLine)->setQuantityArticle( (*itTicketLine)->quantityArticle() + 1 );
	    previousExist = TRUE;
	} // end if
      
    } // end for
  
    /// Si no existia antes entonces se crea entrada en la lista de articulos en el ticket.
    if (previousExist == FALSE) {

	BndsTicketLine *ticketLine = (BndsTicketLine*) article;
	ticketLine->setQuantityArticle(1);
      
	m_ticketLines.push_back( ticketLine );
    
    } // end if
	
    /// Recalcula el total del ticket;
    recalculeTotal();
}



void BndsTicket::setNomTicket(string nombre)
{
    m_nomTicket = nombre;
}


string BndsTicket::nomTicket()
{
    return m_nomTicket;
}


float BndsTicket::total()
{
    return m_total;
}


void BndsTicket::recalculeTotal()
{
     float total = 0.0;
  
     /// Recorre todas las lineas del ticket sumando subtotales;
     for (list<BndsTicketLine*>::iterator itArticLine = m_ticketLines.begin(); itArticLine != m_ticketLines.end(); itArticLine++) {
	  total += (*itArticLine)->subTotalLine();
     } // end for
     
     m_total = total;
}



list<BndsTicketLine*> BndsTicket::ticketLines()
{
    return m_ticketLines;
}


void BndsTicket::removeTicketLine(BndsTicketLine *ticketLine)
{
    for (list<BndsTicketLine*>::iterator itTicketLine = m_ticketLines.begin(); itTicketLine != m_ticketLines.end(); itTicketLine++) {

	if ( (*itTicketLine) == ticketLine ) {
	  
	    m_ticketLines.erase (itTicketLine);
	    break;
	    
	} // end if
	
    } // end for

    /// Recalcula el total del ticket;
    recalculeTotal();
}


void BndsTicket::cleanTicket()
{
    m_ticketLines.clear ();

    /// Recalcula el total del ticket;
    recalculeTotal();
}

