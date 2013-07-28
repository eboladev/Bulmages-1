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

#include "bndsvideo.h"
#include "bndsfunctions.h"
#include "bndsticketview.h"
#include "splash.h"


BndsVideo *g_video = NULL;


BndsVideo::BndsVideo()
{
}


BndsVideo::~BndsVideo()
{
}


void BndsVideo::ndsInit()
{
    videoSetMode ( MODE_5_2D );
    videoSetModeSub ( MODE_5_2D );
    vramSetBankA ( VRAM_A_MAIN_BG );
    vramSetBankC ( VRAM_C_SUB_BG );
        
    lcdMainOnBottom();

    consoleInit ( &m_conMain, 0, BgType_Text4bpp, BgSize_T_256x256, 4, 0, true, true );

    consoleInit ( &m_conSub, 0, BgType_Text4bpp, BgSize_T_256x256, 4, 0, false, true );
    
    m_bgMain = bgInit ( 3, BgType_Bmp8, BgSize_B8_256x256, 1, 0);
    m_bgSub = bgInitSub ( 3, BgType_Bmp8, BgSize_B8_256x256, 1, 0);

    resetBgMain();
    resetBgSub();
    
    /// Establece la paleta de colores en la pantalla principal.
    BG_PALETTE[1] = RGB15(31,16,0);
    BG_PALETTE[2] = RGB15(5,31,0);
    BG_PALETTE[3] = RGB15(31,0,0); /// Rojo
    BG_PALETTE[4] = RGB15(0,0,31); /// Azul
    BG_PALETTE[5] = RGB15(0,31,31); /// Cyan
    BG_PALETTE[6] = RGB15(31,31,31); /// Blanco

    /// Establece la paleta de colores en la pantalla secundaria.
    BG_PALETTE_SUB[1] = RGB15(31,16,0);
    BG_PALETTE_SUB[2] = RGB15(5,31,0);
    BG_PALETTE_SUB[3] = RGB15(31,0,0); /// Rojo
    BG_PALETTE_SUB[4] = RGB15(0,0,31); /// Azul
    BG_PALETTE_SUB[5] = RGB15(0,31,31); /// Cyan
    BG_PALETTE_SUB[6] = RGB15(31,31,31); /// Blanco

}


void BndsVideo::ndsInitSplash()
{
  	int bg3_main;
	
  	lcdMainOnTop();
        videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	
	bg3_main = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
        dmaCopy(splashBitmap, bgGetGfxPtr(bg3_main), splashBitmapLen);

   	videoSetModeSub(MODE_0_2D);
	vramSetBankC(VRAM_C_SUB_BG);
	
	PrintConsole *defaultConsole = consoleGetDefault();
	consoleInit(&m_conSub, defaultConsole->bgLayer, BgType_Text4bpp, BgSize_T_256x256, defaultConsole->mapBase, defaultConsole->gfxBase, false, true);

}


void BndsVideo::ndsInitConfig()
{
}


int BndsVideo::backgroundMain()
{
    return m_bgMain;
}


int BndsVideo::backgroundSub()
{
    return m_bgSub;
}


PrintConsole BndsVideo::consoleMain()
{
    return m_conMain;
}


PrintConsole BndsVideo::consoleSub()
{
    return m_conSub;
}


void BndsVideo::resetBgMain()
{
    u16* buffer = (u16*) bgGetGfxPtr ( m_bgMain );

    /// Borra el fondo grafico.
    dmaFillHalfWords ( (u16) 0x0000, buffer, 49152 ); /// 256 * 192 = 49152.
}


void BndsVideo::resetBgSub()
{
    u16* buffer = (u16*) bgGetGfxPtr ( m_bgSub );

    /// Borra el fondo grafico.
    dmaFillHalfWords ( (u16) 0x0000, buffer, 49152 ); /// 256 * 192 = 49152.
}


void BndsVideo::resetAllMain()
{
      /// Resetea la capa de texto.
      PrintConsole conMain = g_video->consoleMain();
      consoleSelect( &conMain );
      iprintf("\x1b[0;0H");
      iprintf("\x1b[2J");
      
      /// Resetea la capa grafica.
      resetBgMain();
}


void BndsVideo::resetAllSub()
{
      /// Resetea la capa de texto.
      PrintConsole conSub = g_video->consoleSub();
      consoleSelect( &conSub );
      iprintf("\x1b[0;0H");
      iprintf("\x1b[2J");
      
      /// Resetea la capa grafica.
      resetBgSub();
}


/*
  Plantilla usada en la lista de categorias y articulos.
*/
void BndsVideo::showTemplateListA(int totalItems, u16 indexPalette, int offset, bool previousPage, bool modificador, bool modificadorActivo)
{

    /// Limite de recuadros en la plantilla.
    if (totalItems > 20) totalItems = 20;

    m_totalItemsTemplateListA = totalItems;

    /// Borra la consola de texto.
    consoleSelect( &m_conMain );
    iprintf("\x1b[2J");

    iprintf("\x1b[23;0HTicket  <--    -->");

    
    resetBgMain();
    
    /// Dibuja boton 'Ver ticket'.
    drawRectangle8bpp ( 0, 176, 52, 192, indexPalette, (u16*) bgGetGfxPtr ( m_bgMain ));
    /// Dibuja boton 'Pagina anterior'.
    drawRectangle8bpp ( 57, 176, 102, 192, indexPalette, (u16*) bgGetGfxPtr ( m_bgMain ));
    /// Dibuja boton 'Pagina siguiente'.
    drawRectangle8bpp ( 107, 176, 150, 192, indexPalette, (u16*) bgGetGfxPtr ( m_bgMain ));

    /// Dibuja boton 'M' (Activa/desactiva modificadores).
    if (modificador) {
      iprintf("\x1b[23;27HMod.");
      if (modificadorActivo) {
	  drawRectangle8bpp ( 205, 176, 256, 192, indexPalette + 1, (u16*) bgGetGfxPtr ( m_bgMain ));
      } else {
	  drawRectangle8bpp ( 205, 176, 256, 192, indexPalette, (u16*) bgGetGfxPtr ( m_bgMain ));
      } // end if
    } // end if
    
    /// Dibuja boton 'Pantalla anterior'.
    if (previousPage) {
      iprintf("\x1b[23;22H^");
      drawRectangle8bpp ( 155, 176, 200, 192, indexPalette, (u16*) bgGetGfxPtr ( m_bgMain ));
    } // end if
    
    
    /// Dibuja la cuadricula de 5 x 4 recuadros maximo.
    int contador = 0;
    for (int i = 0; i < 4; i++) {
      
      for (int j = 0; j < 5; j++) {
	
	/// Dibuja recuadro. (47 px x 39 px; separacion vertical 3 px, separacion horizontal 3 px).
	int x1 = 5 + (50 * j);
	int x2 = 47 + (50 * j);
	int y1 = 4 + (42 * i);
	int y2 = 39 + (42 * i);
	
	drawRectangle8bpp ( x1, y1, x2, y2, indexPalette, (u16*) bgGetGfxPtr ( m_bgMain ));
	
	contador++;

	/// Escribe texto en recuadro.
	iprintf("\x1b[%i;%iH\x1b[30;0m%i", (unsigned int) (y1 / 8) + 1, (unsigned int) (x1 / 8) + 1, contador + offset);	

	/// Si se han dibujado todos los items se sale porque no hay nada nas que dibujar.
	if (contador >= totalItems) return;
	
      } // end for
      
    } // end for
    
}


int BndsVideo::eventTemplateListA(bool previousPage, bool modificador)
{
    /// Procesa los eventos de la pantalla tactil.
    /// En funcion del numero de elementos devuelve que recuadro u opcion se ha seleccionado.
    static bool rebote = true;
    touchRead(&m_touch);

    if ((m_touch.px > 0) && (m_touch.py > 0) && rebote) {
	rebote = false;
	
	/// Coordenadas de otras opciones:
	
	  /// Ver ticket.
	  if ( ((m_touch.px >= 0) && (m_touch.px <= 52)) && ((m_touch.py >= 176) && (m_touch.py <= 192)) ) {
	      BndsTicketView *ticketView = new BndsTicketView( g_db->currentTicket() );
	      ticketView->exec();
	      delete ticketView;
	      return -1;
	  } // end if
	  
	  /// Pagina anterior.
	  if ( ((m_touch.px >= 57) && (m_touch.px <= 102)) && ((m_touch.py >= 176) && (m_touch.py <= 192)) ) {
	      return -2;
	  } // end if

	  /// Pagina siguiente.
	  if ( ((m_touch.px >= 107) && (m_touch.px <= 150)) && ((m_touch.py >= 176) && (m_touch.py <= 192)) ) {
	      return -3;
	  } // end if

    	  /// Boton 'M' (modificadores).
	  if (modificador) {
	      if ( ((m_touch.px >= 205) && (m_touch.px <= 256)) && ((m_touch.py >= 176) && (m_touch.py <= 192)) ) {
		  return -10;
	      } // end if
	  } // end if

	  /// Dibuja boton 'Pantalla anterior'.
	  if (previousPage) {
		if ( ((m_touch.px >= 170) && (m_touch.px <= 217)) && ((m_touch.py >= 176) && (m_touch.py <= 192)) ) {
		    return -4;
		} // end if
	  } // end if
	  	
	/// Coordenadas de los recuadros:
	
	int contador = 0;
	/// filas
	for (int i = 0; i < 4; i++) {
	  /// columnas.
	  for (int j = 0; j < 5; j++) {
	    
	    /// Coordenadas de cada recuadro. (47 px x 39 px; separacion vertical 3 px, separacion horizontal 3 px).
	    int x1 = 5 + (50 * j);
	    int x2 = 47 + (50 * j);
	    int y1 = 4 + (42 * i);
	    int y2 = 39 + (42 * i);
	    
	    if ( ((m_touch.px >= x1) && (m_touch.px <= x2)) && ((m_touch.py >= y1) && (m_touch.py <= y2)) ) {
		/// Se ha pulsado dentro del recuadro.
		/// Devuelve el numero de recuadro pulsado.
		/// El primer elemento es el 1.
		return j + (i * 5) + 1;
		break;
	    } // end if
	    
	    contador++;

	    /// Se tiene en cuenta solo los elementos mostrados en cada caso.
	    if (contador >= m_totalItemsTemplateListA) return 0;
	    
	  } // end for
	  
	} // end for




    } else if ((m_touch.px == 0) && (m_touch.py == 0) && !rebote)  {
	rebote = true;
    } // end if


    return 0;
}


/*
  Plantilla usada en la vista de ticket.
*/
void BndsVideo::showTemplateListB(int totalItems, u16 indexPalette)
{
    resetBgMain();
    resetBgSub();

    PrintConsole conMain = g_video->consoleMain();
    consoleSelect( &conMain );
    consoleClear();

    /// Boton - cantidad
    drawRectangle8bpp ( 5, 4, 47, 39, indexPalette, (u16*) bgGetGfxPtr ( m_bgMain )); // 42 x 35
    iprintf("\x1b[1;1H\x1b[47m- 1"); 
    
    /// Boton + cantidad
    drawRectangle8bpp ( 55, 4, 97, 39, indexPalette, (u16*) bgGetGfxPtr ( m_bgMain )); // 42 x 35
    iprintf("\x1b[1;8H\x1b[47m+ 1");
    
    /// Boton borrar linea
    drawRectangle8bpp ( 115, 4, 170, 39, indexPalette, (u16*) bgGetGfxPtr ( m_bgMain )); // 55 x 35
    iprintf("\x1b[1;15H\x1b[47mBorrar");
    iprintf("\x1b[2;15H\x1b[47mlinea");
    
    /// Dibuja boton 'Anterior'.
    drawRectangle8bpp ( 170, 176, 217, 192, indexPalette, (u16*) bgGetGfxPtr ( m_bgMain ));
    iprintf("\x1b[23;24H^");
    
    /// Dibuja boton 'Sube linea'.
    drawRectangle8bpp ( 188, 4, 230, 39, indexPalette, (u16*) bgGetGfxPtr ( m_bgMain )); // 55 x 35
    iprintf("\x1b[1;25H/|\\");
    iprintf("\x1b[2;26H|");

    /// Dibuja boton 'Baja linea'.
    drawRectangle8bpp ( 188, 47, 230, 74, indexPalette, (u16*) bgGetGfxPtr ( m_bgMain )); // 55 x 35
    iprintf("\x1b[7;26H|");
    iprintf("\x1b[8;25H\\|/");

    /// Dibuja boton 'Seleccionar mesa y enviar'.
    drawRectangle8bpp ( 24, 58, 133, 115, indexPalette, (u16*) bgGetGfxPtr ( m_bgMain ));
    iprintf("\x1b[8;4HSeleccionar");
    iprintf("\x1b[9;4Hmesa y");
    iprintf("\x1b[10;4Henviar");

    
}
  

int BndsVideo::eventTemplateListB()
{
  
    /// Procesa los eventos de la pantalla tactil.
    /// En funcion del numero de elementos devuelve que recuadro u opcion se ha seleccionado.
    static bool rebote = true;
    touchRead(&m_touch);

    if ((m_touch.px > 0) && (m_touch.py > 0) && rebote) {
	rebote = false;

	/// Boton - cantidad.
	if ( ((m_touch.px >= 5) && (m_touch.px <= 47)) && ((m_touch.py >= 4) && (m_touch.py <= 39)) ) {
	    return -2;
	} // end if

	/// Boton + cantidad.
	if ( ((m_touch.px >= 55) && (m_touch.px <= 97)) && ((m_touch.py >= 4) && (m_touch.py <= 39)) ) {
	    return -3;
	} // end if

	/// Boton borrar linea
	if ( ((m_touch.px >= 115) && (m_touch.px <= 170)) && ((m_touch.py >= 4) && (m_touch.py <= 39)) ) {
	    return -4;
	} // end if

	/// Boton anterior.
	if ( ((m_touch.px >= 170) && (m_touch.px <= 217)) && ((m_touch.py >= 176) && (m_touch.py <= 192)) ) {
	    return -5;
	} // end if

	/// Dibuja boton 'Sube linea'.
    	if ( ((m_touch.px >= 188) && (m_touch.px <= 230)) && ((m_touch.py >= 4) && (m_touch.py <= 39)) ) {
	    return -6;
	} // end if

	/// Dibuja boton 'Baja linea'.
    	if ( ((m_touch.px >= 188) && (m_touch.px <= 230)) && ((m_touch.py >= 47) && (m_touch.py <= 74)) ) {
	    return -7;
	} // end if

	/// Dibuja boton 'Seleccionar mesa y enviar'.
    	if ( ((m_touch.px >= 24) && (m_touch.px <= 133)) && ((m_touch.py >= 58) && (m_touch.py <= 115)) ) {
	    return -10;
	} // end if

    } else if ((m_touch.px == 0) && (m_touch.py == 0) && !rebote)  {
	rebote = true;
    } // end if
	
    return 0;
}


/*
  Plantilla usada en la lista de articulos del ticket.
*/
void BndsVideo::showTemplateListC(int lineSelected, u16 indexPalette)
{
  
    resetBgSub();

    /// Dibuja el fondo de la linea seleccionada en el listado.
    if (lineSelected != -1) {
      
	int y1 = (lineSelected * 8) - 8 + 15;
	int y2 = (lineSelected * 8) + 16;
	drawRectangle8bpp ( 0, y1, 255l, y2, indexPalette, (u16*) bgGetGfxPtr ( m_bgSub ));
	
    } // end if

}
  

int BndsVideo::eventTemplateListC()
{
    return 0;
}
