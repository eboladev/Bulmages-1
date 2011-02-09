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

#include "bndsarticlesview.h"
#include "bndsmodifier.h"


BndsArticlesView::BndsArticlesView(int category) : BndsListUi()
{
  
	list<BndsCategoryArticle*> listaCateg = g_db->categoryArticleList();
  
	/// Recorremos todas las categorias hasta encontrar la seleccionada.
  	int contador = 0;
	for (list<BndsCategoryArticle*>::iterator itCateg = listaCateg.begin(); itCateg != listaCateg.end(); itCateg++) {

	    if (contador == category) {
		  /// Ya tenemos el puntero a la lista de articulos de la categoria seleccionada.
		  m_categoryArticle = *itCateg;
		  m_listaArtic = (*itCateg)->articleList();
		  break;
	    } // end if
 
	    contador++;

	} // end for
  
	/// Establece el numero de items a mostrar en la lista.
	setTotalItems( m_listaArtic.size() );
}


BndsArticlesView::~BndsArticlesView()
{
}


void BndsArticlesView::show()
{
  
	/// Inicia la consola.
	PrintConsole conSub = g_video->consoleSub();
  	consoleSelect( &conSub );

	iprintf("\x1b[0;0H");
	iprintf("\x1b[2J");
	printf("\x1b[0;0H%-15.15s - Total: %.2f", g_db->currentTicket()->nomTicket().c_str(), g_db->currentTicket()->total() );

	/// Borra las lineas de texto donde hay que escribir.
	int lineaInicio = 2;
	for (int i = lineaInicio; i < m_maxItemsPerPage + lineaInicio; i++) {
		iprintf("\x1b[%i;0H", i);
	        iprintf("\x1b[2K");
	} // end for

	/// Muestra la paginacion.
	iprintf("\x1b[23;0H");
	iprintf("\x1b[2K");
	iprintf("\x1b[23;0HPag: %i / %i - %-15.15s -", m_currentPage, m_totalPages, m_categoryArticle->nomCategoryArticle().c_str());
	

	int primerElemento = (m_currentPage * m_maxItemsPerPage) - m_maxItemsPerPage;
	int ultimoElemento = primerElemento + m_maxItemsPerPage;
	int totalElementosPaginaActual = ((int) m_listaArtic.size() - (m_maxItemsPerPage * (m_currentPage - 1))) >= m_maxItemsPerPage ? m_maxItemsPerPage : ((int) m_listaArtic.size() - (m_maxItemsPerPage * (m_currentPage - 1)));
	
	/// Dibuja la plantilla tactil en la pantalla principal.
	g_video->showTemplateListA( totalElementosPaginaActual, 2, primerElemento, true, true, m_modifierActive );
	
	int contador = 1;
	for (list<BndsArticle*>::iterator itArtic = m_listaArtic.begin(); itArtic != m_listaArtic.end(); itArtic++) {

	  /// Muestra los items en pantalla.
	  if ((contador > primerElemento) && (contador <= ultimoElemento) ) {

	    consoleSelect( &conSub );
	    printf("\x1b[%i;0H%2i %-20.20s %3.2f", lineaInicio, lineaInicio - 1 + primerElemento, (*itArtic)->nomArticle().c_str(), (*itArtic)->pvpArticle() );

	    lineaInicio++;
	    
	  } // end if
	  
	  contador++;

	} // end for

}


int BndsArticlesView::exec()
{
      /// Muestra la pantalla.
      show();
  
      while(1) {

	  swiWaitForVBlank();

	  scanKeys();
	  
	  if (keysDown() & KEY_LEFT) {
	      previousPage();
	  } else if (keysDown() & KEY_RIGHT) {
	      nextPage();
	  } else if (keysDown() & KEY_L) {
	      break;
	  } // end if

	  /// Procesa los eventos de la pantalla tactil.
	  int itemSelected = g_video->eventTemplateListA(true, true);

	  if (itemSelected == -1) {
      	      m_modifierActive = false;
	      /// Necesita repintarse.
	      show();
	  } // end if

	  if (itemSelected == -2) {
	      previousPage();
	  } // end if

	  if (itemSelected == -3) {
	      nextPage();
	  } // end if

	  if (itemSelected == -4) {
	      break;
	  } // end if
	  
	  if (itemSelected == -10) {
	      modifierChangeState();
	  } // end if
	  
	  /// itemSelected = 0 => No se ha pulsado ningun recuadro. No se hace nada.
	  if (itemSelected > 0) {
	      /// Devuelve la posicion de la categoria dentro de la lista de categorias.
	      /// El primer elemento es el 1.
	      /// Inserta el articulo en el ticket.

	      int contador = 0;
	      for (list<BndsArticle*>::iterator itArtic = m_listaArtic.begin(); itArtic != m_listaArtic.end(); itArtic++) {

		if (contador == (itemSelected - 1 + ((m_currentPage - 1) * m_maxItemsPerPage)) ) {
		  
		    if ( m_modifierActive ) {
			/// Se captura la informacion de los modificadores.
			/// 1) Captura info extra.
			/// 2) Guarda info si no se ha cancelado la accion.
			BndsModifier *mod = new BndsModifier();
			int result = mod->exec();
			if (result == 1) {
			    /// Se pulso el boton 'aceptar'. Se guarda la informacion.
			    g_db->currentTicket()->addArticle( *itArtic, mod );
			} else {
			    /// TODO: se pulso otro boton.
			    g_db->currentTicket()->addArticle( *itArtic, 0 );
			} // end if
			m_modifierActive = false;
		    } else {
			g_db->currentTicket()->addArticle( *itArtic, 0 );
		    } // end if
		    
		    /// Refresca la informacion en pantalla.
		    show();
		    /// Sale del for.
		    break;
		    
		} // end if
		
		contador++;

	      } // end for

	  } // end if

      } // end while

    return 0;
}




