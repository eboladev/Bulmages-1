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

#include <iostream>
#include <sstream>
#include <fstream>

#include "base64.h"

#include "bndsmodifier.h"
#include "bndsfunctions.h"
#include "bndsvideo.h"


BndsModifier::BndsModifier()
{
    m_nombreArchivo = "";
}


BndsModifier::~BndsModifier()
{
    if (m_nombreArchivo != "") {
  
	/// Antes de destruirse borra el archivo de imagen asociado si existe.
	ifstream ifile(m_nombreArchivo.c_str(), ifstream::out);
	
	if (ifile) {
	  /// El archivo existe se intenta borrar.
	  remove(m_nombreArchivo.c_str());
	} // end if

    } // end if

}


/// Para un origen 8 bits por pixel.
void BndsModifier::screenshotToBmp1BitBase64(const char* filename) {

	FILE* file=fopen(filename, "wb");
	
	int imageWidth = 256; // 215
	int imageHeight = 192; // 170
	int imageXOffset = 0; // 41
	int imageYOffset = 0;
	int imageSize = imageWidth*imageHeight/8;
	u16 offset = sizeof(INFOHEADER)+sizeof(HEADER)+(sizeof(INFOPALETTE)*2);

	
	u8* temp=(u8*)malloc(imageSize+offset);

	HEADER* header=(HEADER*)temp;
	INFOHEADER* infoheader=(INFOHEADER*)(temp+sizeof(HEADER));
	INFOPALETTE* palette0=(INFOPALETTE*)(temp+sizeof(HEADER)+sizeof(INFOHEADER));
	INFOPALETTE* palette1=(INFOPALETTE*)(temp+sizeof(HEADER)+sizeof(INFOHEADER)+sizeof(INFOPALETTE));
	
	write16(&header->type, 0x4D42);
	write32(&header->size, imageSize+offset);
	write32(&header->offset, offset);
	write16(&header->reserved1, 0);
	write16(&header->reserved2, 0);

	write16(&infoheader->bits, 1);
	write32(&infoheader->size, sizeof(INFOHEADER));
	write32(&infoheader->compression, 0);
	write32(&infoheader->width, imageWidth);
	write32(&infoheader->height, imageHeight);
	write16(&infoheader->planes, 1);
	write32(&infoheader->imagesize, imageSize);
	write32(&infoheader->xresolution, 0);
	write32(&infoheader->yresolution, 0);
	write32(&infoheader->importantcolours, 0);
	write32(&infoheader->ncolours, 2);

	write8(&palette0->blue, 0x00);
	write8(&palette0->green, 0x00);
	write8(&palette0->red, 0x00);
	write8(&palette0->reserved, 0x00);

	write8(&palette1->blue, 0xFF);
	write8(&palette1->green, 0xFF);
	write8(&palette1->red, 0xFF);
	write8(&palette1->reserved, 0x00);

	for (int y = imageYOffset; y < imageHeight; y++) {
	  
		for (int x = imageXOffset; x < imageWidth; x++) {
			u8 pixel = getPixel8bpp ( x, y, (u16*)bgGetGfxPtr(g_video->backgroundMain()) );
			drawPixel1bpp ( x - imageXOffset, (192 - y) - imageYOffset, pixel, temp, offset );
		} // end for

	} // end for


	DC_FlushAll();
	
	/// Codifica el archivo BMP en Base64.
	string encoded = base64_encode(reinterpret_cast<const unsigned char*>(temp), imageSize+offset);
	
	//fwrite(temp, 1, imageSize+offset, file);
	fwrite(encoded.data(), 1, encoded.size(), file);
	fclose(file);
	free(temp);
}



void BndsModifier::clearCanvas()
{
      swiWaitForVBlank();
      u16* buffer = (u16*) bgGetGfxPtr ( g_video->backgroundMain() );
      /// Borra la pantalla.
      drawRectangle8bpp ( 41, 0, 255, 170, 6, buffer );

}


/// Dibuja el fondo.
void BndsModifier::show()
{
      PrintConsole con = g_video->consoleMain();
      consoleSelect( &con );

      clearCanvas();
      u16* buffer = (u16*) bgGetGfxPtr ( g_video->backgroundMain() );
  
      /// Barra lateral izquierda.
      drawRectangle8bpp ( 0, 0, 40, 191, 0, buffer );
      
      /// Barra inferior.
      drawRectangle8bpp ( 0, 171, 255, 192, 0, buffer );

      /// Boton cancelar.
      drawRectangle8bpp ( 0, 175, 70, 192, 2, buffer );
      iprintf("\x1b[23;0HCancelar");

      /// Boton aceptar.
      drawRectangle8bpp ( 180, 175, 255, 192, 2, buffer );
      iprintf("\x1b[23;24HAceptar");
      
}


int BndsModifier::exec()
{
	/// Inicializa las 2 pantallas a negro.
	g_video->resetAllMain();
	g_video->resetAllSub();
	
	show();

	touchPosition touch;
	int pulsado;
	int pulsadoClick;
	int grosorX = 1;
	int grosorY = 2;
	bool tactil = false;
	int origenX = 0;
	int origenY = 0;
	
	/// Limites donde dibujar.
	int limXmin = 40 + 2;
	int limXmax = 256;
	int limYmin = 0;
	int limYmax = 171 - 2;
	
	u16* buffer = (u16*) bgGetGfxPtr ( g_video->backgroundMain() );
	
	/// Espera hasta que no se este pulsando en la pantalla. Nos aseguramos de
	/// no estar pintando antes de tiempo.
	while (1) {
	    scanKeys();
	    touchRead(&touch);
	    pulsado = keysHeld();
	    if (pulsado & KEY_TOUCH) {
	    } else {
		break;
	    } // end if
	} // end while


	while (1) {
	    scanKeys();
	    touchRead(&touch);
	    pulsado = keysHeld();
	    pulsadoClick = keysDown();
	    
	
	    if ((pulsado & KEY_TOUCH) && ((touch.px >= limXmin) && (touch.px <= limXmax) && (touch.py >= limYmin) && (touch.py <= limYmax)) ) {
		/// Se esta pulsando en la pantalla tactil.
		if (tactil == false) {
		    /// Se acaba de pulsar en la pantalla tactil. Se registra la posicion inicial.
		    origenX = touch.px;
		    origenY = touch.py;
		    tactil = true;
		} // end if
		
		//drawRectangle8bpp ( touch.px - (grosorX / 2), touch.py - (grosorY / 2), touch.px + (grosorX / 2), touch.py + (grosorY / 2), 1, buffer);
		drawLine(origenX, origenY, touch.px, touch.py, grosorX, grosorY, 0, buffer);
		
		/// Registra para el proximo dibujado.
		origenX = touch.px;
		origenY = touch.py;
		
	    } else {
		/// No se esta pulsando la pantalla tactil.
		tactil = false;
	    } // end if


	    if ( (pulsado & KEY_L) && (pulsado & KEY_R)) {
		/// Resetea la pantalla
		clearCanvas();
	    } // end if

	    if (pulsado & KEY_A) {
		/// Se pulsa la tecla A (aceptar).
		return 1;
	    } // end if

	    /// Si se pulsa en el boton 'aceptar'.
	    if ( (pulsadoClick & KEY_TOUCH) &&
		  (touch.px >= 180) && (touch.px <= 255 ) &&
		  (touch.py >= 175) && (touch.py <= 192 ) ) {

		  /// Se guarda la imagen en disco
		  time_t seconds;
		  seconds = time (NULL);
	    
		  string sec;
		  stringstream out;
		  out << seconds;
		  sec = out.str();
	    
		  string nombreArchivo = "tmp_ndsbulmatpv_" + sec + ".bmp.base64";
		  screenshotToBmp1BitBase64(nombreArchivo.c_str());
		  m_nombreArchivo = nombreArchivo;
	      
		  return 1;
	    } // end if

	    /// Si se pulsa en el boton 'cancelar'.
	    if ( (pulsadoClick & KEY_TOUCH) &&
		  (touch.px >= 0) && (touch.px <= 70 ) &&
		  (touch.py >= 175) && (touch.py <= 192 ) ) {
		    return -1;
	    } // end if

	    swiWaitForVBlank();
	
	} // end while

	return 0;
}


string BndsModifier::getNombreArchivo()
{
    return m_nombreArchivo;
}
