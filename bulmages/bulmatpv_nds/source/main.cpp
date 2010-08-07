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

#include <nds.h>
#include <stdio.h>
#include <string>
#include <list>

using namespace std;

#include "bndsdb.h"
#include "bndssocket.h"
#include "bndsarticle.h"
#include "bndsvideo.h"
#include "bndscategoriesview.h"
#include "bndsarticlesview.h"
#include "bndsticket.h"

/// VERSION = 1.5 Beta

int main(void) {

	int res;
	BndsTicket *ticket;
	BndsCategoriesView *categView = NULL;
	BndsArticlesView *articView = NULL;
	PrintConsole conSub;
	
    while(1) {

      	/// Inicializa el motor grafico y los modos de video.
	g_video = new BndsVideo();
	g_video->ndsInitSplash();
      
	/// Muestra el splash screen.
	while(1) {

          swiWaitForVBlank();

          scanKeys();
          
          if (keysDown() & KEY_A) {
		break;
	  } // end if

          if (keysDown() & KEY_L) {
		BndsConfig *config = new BndsConfig();
		config->editConfig();
		delete config;
		break;
	  } // end if
	
	} // end while
	
	
	/// Inicializa el modo normal de video para el programa.
	g_video->ndsInit();

	/// Inicializa la base de datos.
	g_db = new BndsDb();
	res = g_db->initConnection();
	if (res) {
		iprintf("ERROR FATAL: No se han podido conectar con el servidor.");
		return -1;
	} // end if

	res = g_db->loadData();
	if (res) {
		iprintf("ERROR FATAL: No se han podido cargar los datos.");
		return -1;
	} // end if
	

	conSub = g_video->consoleSub();
	consoleSelect( &conSub );

	ticket = new BndsTicket();
	g_db->addTicket(ticket);
	
	while(1) {
	  
	    categView = new BndsCategoriesView();
	    /// Muestra la pantalla de categorias y ejecuta el bucle de ejecucion de eventos.
	    int itemSelected = categView->exec();
	    
	    if (itemSelected > 0) {
		/// Se ha seleccionado una categoria, por tanto hay que mostrar
		/// la lista de articulos de esa categoria.
		articView = new BndsArticlesView(itemSelected - 1);
		articView->exec();
		delete articView;
	    } else {
		/// regresamos a la pantalla inicial y reiniciamos el programa.
		break;
	    } // end if

	    swiWaitForVBlank();
	
	} // end while

	delete categView;
	delete ticket;
	delete g_db;
	delete g_video;

    } // end while
    
    return 0;
}
 
