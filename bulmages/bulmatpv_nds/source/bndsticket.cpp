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
#include "bndsfunctions.h"
#include "bndsdb.h"


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


void BndsTicket::addArticle(BndsArticle* article, BndsModifier *modifier)
{

    list<BndsTicketLine*> ticketLinesList = ticketLines();
    bool previousExist = false;

    
    /// Si existe modificador siempre se crea una linea de ticket nueva.
    if (modifier == ( BndsModifier* ) 0) {
      
	for (list<BndsTicketLine*>::iterator itTicketLine = ticketLinesList.begin(); itTicketLine != ticketLinesList.end(); itTicketLine++) {

	    if ( (*itTicketLine)->article()->idArticle() == article->idArticle() ) {
		/// Comprueba que esa linea no tenga modificadores.
		if ( (*itTicketLine)->getModifier() == ( BndsModifier* ) 0 ) {
		    (*itTicketLine)->setQuantityArticle( (*itTicketLine)->quantityArticle() + 1 );
		    previousExist = true;
		} // end if

	    } // end if

	} // end for  
  
    } // end if
    

    /// Si no existia antes entonces se crea entrada en la lista de articulos en el ticket.
    if (previousExist == false) {
     
	BndsTicketLine *ticketLine = new BndsTicketLine();
	ticketLine->setArticle(article);
	ticketLine->setQuantityArticle(1);

	if (modifier != ( BndsModifier* ) 0) {
	    /// Si dispone de modificadores se establece.
	    ticketLine->setModifier(modifier);
	} else {
	    ticketLine->setModifier(0);
	} // end if
	
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
	  
	    /// Para borrar la una lineas de ticket hay que borrar tambien el
	    /// modificador si existen.
	    (*itTicketLine)->removeModifier();

	    m_ticketLines.erase (itTicketLine);
	    break;
	    
	} // end if
	
    } // end for

    /// Recalcula el total del ticket;
    recalculeTotal();
}


void BndsTicket::clearTicket()
{
    
    for (list<BndsTicketLine*>::iterator itTicketLine = m_ticketLines.begin(); itTicketLine != m_ticketLines.end(); itTicketLine++) {

	  /// Para borrar una lineas de ticket hay que borrar tambien el
	  /// modificador si existen.
	  (*itTicketLine)->removeModifier();

    } // end for

    /// Se borran todas las lineas de ticket.
    m_ticketLines.clear();

    /// Recalcula el total del ticket;
    recalculeTotal();
}


/// Genera el archivo XML y lo envia por el socket abierto.
string BndsTicket::ticket2xml(bool envia)
{
	string xml = "<DOCUMENT><PUTCOMMAND>ticket_data</PUTCOMMAND><TICKET>";
	
	/// TODO:Nombre del usuario.
	
	/// Nombre del ticket.
	xml += "<NOMTICKET>";
	xml += nomTicket().c_str();
	xml += "</NOMTICKET>";

	/// Nombre del trabajador.
	xml += "<NOMTRABAJADOR>";
	xml += "";
	xml += "</NOMTRABAJADOR>";

	/// Total ticket.
	xml += "<TOTALTICKET>";
	xml += "";
	xml += "</TOTALTICKET>";
	
	/// Envia.
	if (envia) { g_db->linkSocket()->sendDataToServer(xml); xml= ""; }
  
	/// Recorre todos los elementos del ticket y genera un string XML con los datos.
	list<BndsTicketLine*> listaTicketLines = ticketLines();
	for (list<BndsTicketLine*>::iterator itTicketLine = listaTicketLines.begin(); itTicketLine != listaTicketLines.end(); itTicketLine++) {

	    xml += "<LINEATICKET>";
	    
	      xml += "<IDARTICULO>";
	      xml += (*itTicketLine)->article()->idArticle().c_str();
	      xml += "</IDARTICULO>";

	      xml += "<CANTARTICULO>";
	      xml += float2string3x2 ( (*itTicketLine)->quantityArticle() ).c_str();
	      xml += "</CANTARTICULO>";

	      xml += "<PVPARTICULO>";
	      xml += float2string3x2 ( (*itTicketLine)->article()->pvpArticle() ).c_str();
	      xml += "</PVPARTICULO>";

	      xml += "<SUBTOTAL>";
	      xml += "";
	      xml += "</SUBTOTAL>";
	      
	      /// Sigue enviando.
	      if (envia) { g_db->linkSocket()->sendDataToServer(xml); xml= ""; }

	      if ( (*itTicketLine)->getModifier() != ( BndsModifier* ) 0 ) {
		  /// Esta linea de ticket tiene modificadores.

		  xml += "<MODIFICADORES>";
		    xml += "<IMAGEN>";
		    
			BndsReadFileContent *fc = new BndsReadFileContent();
			bool r = fc->openFileForRead( (*itTicketLine)->getModifier()->getNombreArchivo() );
			
			if (r == false) {
			  ///TODO:
			  /// Error al abrir el archivo.
			} else {

			    while( !fc->isEof() ) {
				  xml += fc->readContent() ;
				  /// Sigue enviando.
				  if (envia) { g_db->linkSocket()->sendDataToServer(xml); xml= ""; }
			    } // end while
			
			} // end if
    
			delete fc;
    
		    xml += "</IMAGEN>";
		  xml += "</MODIFICADORES>";

	      } // end if
	      
	    xml += "</LINEATICKET>";

	    /// Sigue enviando.
	    if (envia) { g_db->linkSocket()->sendDataToServer(xml); xml= ""; }

	} // end for

	xml += "</TICKET></DOCUMENT>";

	/// Sigue enviando.
	if (envia) { g_db->linkSocket()->sendDataToServer(xml); xml= ""; }

	return xml;
}
