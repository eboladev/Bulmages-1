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

#include "bndsticketview.h"
#include "bndsfunctions.h"
#include "bndsticketslistview.h"


BndsTicketView::BndsTicketView(BndsTicket *ticket) : BndsListUi()
{
    m_ticket = ticket;
    
    /// Establece el numero de items a mostrar en la lista.
    setTotalItems( m_ticket->ticketLines().size() );

    list<BndsTicketLine*> listaTicketLines = m_ticket->ticketLines();
    if (listaTicketLines.size() > 0) {
	m_currentLine = 1;
    } // end if
}


BndsTicketView::~BndsTicketView()
{
}


void BndsTicketView::show()
{
  	/// Inicia la consola.
	PrintConsole conSub = g_video->consoleSub();
	consoleSelect( &conSub );

	iprintf("\x1b[0;0H");
	iprintf("\x1b[2J");
	printf("\x1b[0;0H%-15.15s - Total: %.2f", m_ticket->nomTicket().c_str(), m_ticket->total() );

	/// Borra las lineas de texto donde hay que escribir.
	int lineaInicio = 2;
	for (int i = lineaInicio; i < m_maxItemsPerPage + lineaInicio; i++) {
		iprintf("\x1b[%i;0H", i);
	        iprintf("\x1b[2K");
	} // end for

	/// Muestra la paginacion.
	iprintf("\x1b[23;0H");
	iprintf("\x1b[2K");
	iprintf("\x1b[23;0HPag: %i / %i", m_currentPage, m_totalPages);

	int primerElemento = (m_currentPage * m_maxItemsPerPage) - m_maxItemsPerPage;
	int ultimoElemento = primerElemento + m_maxItemsPerPage;

	/// Dibuja la plantilla tactil en la pantalla principal.
	g_video->showTemplateListB( m_totalItemsCurrentPage, 4 );
	
	list<BndsTicketLine*> listaTicketLines = m_ticket->ticketLines();

	consoleSelect( &conSub );

	int contador = 1;
	for (list<BndsTicketLine*>::iterator itTicketLine = listaTicketLines.begin(); itTicketLine != listaTicketLines.end(); itTicketLine++) {

	  /// Muestra los items en pantalla.
	  if ((contador > primerElemento) && (contador <= ultimoElemento) ) {

	    if ( contador == currentLine() + primerElemento ) {
		printf("\x1b[%i;0H\x1b[30m%4i %-20.20s %3.2f", lineaInicio, (*itTicketLine)->quantityArticle() , (*itTicketLine)->article()->nomArticle().c_str(), (*itTicketLine)->subTotalLine() );
		m_currentTicketLine = *itTicketLine;
	    } else {
		printf("\x1b[%i;0H\x1b[39m%4i %-20.20s %3.2f", lineaInicio, (*itTicketLine)->quantityArticle() , (*itTicketLine)->article()->nomArticle().c_str(), (*itTicketLine)->subTotalLine() );
	    } // end if
	    
	    lineaInicio++;
	    
	  } // end if
	  
	  contador++;

	} // end for
	
	/// En la pantalla secundaria se marca el fondo de la linea de ticket seleccionada.
	if (currentLine() > 0) {
	  g_video->showTemplateListC(currentLine(), 1);
	} // end if

}


int BndsTicketView::exec()
{
      string t;
      char str [50];
      int res;
      BndsTicketsListView *btlv;
      
      /// Muestra la pantalla.
      show();
  
      while(1) {

	  swiWaitForVBlank();

	  scanKeys();

  	  /// Procesa los eventos de la pantalla tactil.
	  int itemSelected = g_video->eventTemplateListB();

	  
	  if (keysDown() & KEY_LEFT) {
	      previousPage();
	  } else if (keysDown() & KEY_RIGHT) {
	      nextPage();
	  } else if (keysDown() & KEY_L) {
	      /// Vuelta a la pantalla anterior.
	      g_video->resetBgSub();
	      break;
	  } else if (keysDown() & KEY_R) {
	      /// Seleccionar mesa y enviar.
	      itemSelected = -10;	    
	  } else if (keysDown() & KEY_UP) {
	      itemSelected = -6;
	  } else if (keysDown() & KEY_DOWN) {
	      itemSelected = -7;
	  } // end if


  
	  switch (itemSelected) {
	    case -1:
		/// Necesita repintarse.
		show();
		break;
	    case -2:
		if (m_currentLine == 0) break;
		
		/// Boton - cantidad.
		m_currentTicketLine->setQuantityArticle(m_currentTicketLine->quantityArticle() - 1);

		/// Si la cantidad es <= 0 entonces se borra la linea de ticket.
		if (m_currentTicketLine->quantityArticle() <= 0) {

		  m_ticket->removeTicketLine( m_currentTicketLine );
		  /// Establece el numero de items a mostrar en la lista.
		  setTotalItems( m_ticket->ticketLines().size() );
		  
		} // end if

		m_ticket->recalculeTotal();
		show();
		break;
	    case -3:
		if (m_currentLine == 0) break;
		
		/// Boton + cantidad.
		m_currentTicketLine->setQuantityArticle(m_currentTicketLine->quantityArticle() + 1);
		m_ticket->recalculeTotal();
		show();
		break;
	    case -4:
		if (m_currentLine == 0) break;
		
		/// Boton borrar linea
		m_ticket->removeTicketLine( m_currentTicketLine );
		/// Establece el numero de items a mostrar en la lista.
		setTotalItems( m_ticket->ticketLines().size() );
		m_ticket->recalculeTotal();
		show();
		break;
	    case -5:
		/// Boton anterior.
		/// Vuelta a la pantalla anterior.
		g_video->resetBgSub();
		return 0;
		break;
	    case -6:
		/// Dibuja boton 'Sube linea'.
		lineUp();
		break;
	    case -7:
		/// Dibuja boton 'Baja linea'.
		lineDown();
		break;
	    case -10:
		/// Seleccionar mesa y enviar.
		g_video->resetBgMain();
		g_video->resetBgSub();

		/// Primero cambiar el nombre al ticket. Despues enviar los datos.
		
		/// TODO: Provisionalmente genera 40 mesas (40 nombres de tickets).
		/// luego comprueba que el nombre no exista en la base de datos (se esta usando).
		/// muestra la lista completa, pero solo deja elegir los que esten disponibles.
		btlv = new BndsTicketsListView();
		res = btlv->exec();
		delete btlv;
		
		/// TODO: manera nada adecuada de conseguir el nombre del ticket seleccionado.
		sprintf(str,"%i", res);
		t = string("MESA ") + string(str);
		
		m_ticket->setNomTicket(t);
		
		/// TODO: tecla provisional. Enviar ticket.
		g_db->saveTicket();
		m_ticket->clearTicket(); 
		setTotalItems( m_ticket->ticketLines().size() );
		m_ticket->setNomTicket(m_ticket->defaultTicketName());
		return 0;
		break;
	    case 2:
		break;
	  } // end switch

	  
      } // end while

    return 0;
}


void BndsTicketView::calculateTotalItemsCurrentPage()
{
      int totalElementosPaginaActual = ((int) m_ticket->ticketLines().size() - (m_maxItemsPerPage * (m_currentPage - 1))) >= m_maxItemsPerPage ? m_maxItemsPerPage : ((int) m_ticket->ticketLines().size() - (m_maxItemsPerPage * (m_currentPage - 1)));
      /// Establece el numero de elementos en la pagina actual para poder moverse por ella.
      setTotalItemsCurrentPage(totalElementosPaginaActual);
}

