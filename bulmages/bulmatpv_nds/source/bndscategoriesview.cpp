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

#include "bndscategoriesview.h"


BndsCategoriesView::BndsCategoriesView() : BndsListUi()
{
	/// Recorre todas las categorias.
	m_listaCateg = g_db->categoryArticleList();

	/// Establece el numero de items a mostrar en la lista.
	setTotalItems( m_listaCateg.size() );
}


BndsCategoriesView::~BndsCategoriesView()
{
}


void BndsCategoriesView::show()
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
	iprintf("\x1b[23;0HPag: %i / %i - CATEGORIAS -", m_currentPage, m_totalPages);
	

	int primerElemento = (m_currentPage * m_maxItemsPerPage) - m_maxItemsPerPage;
	int ultimoElemento = primerElemento + m_maxItemsPerPage;
	int totalElementosPaginaActual = ((int) m_listaCateg.size() - (m_maxItemsPerPage * (m_currentPage - 1))) >= m_maxItemsPerPage ? m_maxItemsPerPage : ((int) m_listaCateg.size() - (m_maxItemsPerPage * (m_currentPage - 1)));
	
	/// Dibuja la plantilla tactil en la pantalla principal.
	g_video->showTemplateListA( totalElementosPaginaActual, 1, primerElemento );
	
	int contador = 1;
	for (list<BndsCategoryArticle*>::iterator itCateg = m_listaCateg.begin(); itCateg != m_listaCateg.end(); itCateg++) {

	  /// Muestra los items en pantalla.
	  if ((contador > primerElemento) && (contador <= ultimoElemento) ) {

	    consoleSelect( &conSub );
	    iprintf("\x1b[%i;0H%2i %-21.21s", lineaInicio, lineaInicio - 1 + primerElemento, (*itCateg)->nomCategoryArticle().c_str() );

	    lineaInicio++;
	    
	  } // end if
	  
	  contador++;

	} // end for

}


int BndsCategoriesView::exec()
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
	  } else if (keysDown() & KEY_START) {
	      /// Regresa a la pantalla inicial y reinicia el programa.
	      return -100;
	  } // end if

	  /// Procesa los eventos de la pantalla tactil.
	  int itemSelected = g_video->eventTemplateListA();

	  if (itemSelected == -1) {
	      /// Necesita repintarse.
	      show();
	  } // end if

	  if (itemSelected == -2) {
	      previousPage();
	  } // end if

	  if (itemSelected == -3) {
	      nextPage();
	  } // end if

	  /// itemSelected = 0 => No se ha pulsado ningun recuadro. No se hace nada.
	  if (itemSelected > 0) {
	      /// Devuelve la posicion de la categoria dentro de la lista de categorias.
	      /// El primer elemento es el 1.
	      return itemSelected + ((m_currentPage - 1) * m_maxItemsPerPage);
	  } // end if

      } // end while

    return 0;
}
  