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

#include "bndsfunctions.h"
#include "bndsdb.h"
#include "bndscategoryarticle.h"


void callback_keyboardOnKeyPressed(int key)
{
       if(key > 0)
      iprintf("%c", key);
}


unsigned long ipToLong(string ip)
{
      string result = "";
      size_t found = 0;
      size_t prevfound = 0;
      int counter = 0;
      unsigned long iplong = 0;

      while (found != string::npos) {

	found = ip.find_first_of(".", found + 1);
	result = ip.substr(0 + prevfound, found - prevfound);
	
	switch (counter) {
	  case 0:
	    iplong += atoi(result.c_str()) * 0x1;
	    counter++;
	    break;
	  case 1:
	    iplong += atoi(result.c_str()) * 0x100;
	    counter++;
	    break;
	  case 2:
	    iplong += atoi(result.c_str()) * 0x10000;
	    counter++;
	    break;
	  case 3:
	    iplong += atoi(result.c_str()) * 0x1000000;
	    counter++;
	    break;
	} // end switch

	prevfound = found + 1;

      } // end while

      return iplong;
}


int callback_processConfigFile(int event, const char* txt, int len, void* tuser) {
  
	/*
	    Para poner el tratamiento de una nueva etiqueta hay que crear codigo en cada apartado (A,B,C,...)
	*/

	/// A
	static bool tag_ipserver = false;
	static bool tag_posuser = false;
	static bool tag_accesspointssid = false;
	static bool tag_accesspointwep128key = false;

	/// B
	static string ipserver = "";
	static string posuser = "";
	static string accesspointssid = "";
	static string accesspointwep128key = "";

	string strTxt;
	strTxt.assign(txt);

	/// TAGS de inicio
	if (tinyxml_event_str(event) == (const char*) "start tag") {

	    if (strTxt == "CONFIG") {

		/// Estamos al inicio de una nueva configuracion. Inicializamos todas las variables de configuracion.
		ipserver = "";
		posuser = "";
		accesspointssid = "";
		accesspointwep128key = "";
		
		tag_ipserver = false;
		tag_posuser = false;
		tag_accesspointssid = false;
		tag_accesspointwep128key = false;
		
	    /// C
	    } else if (strTxt == "IPSERVER") {
	    
		tag_ipserver = true;

	    } else if (strTxt == "POSUSER") {
	    
		tag_posuser = true;

	    } else if (strTxt == "ACCESSPOINTSSID") {
	    
		tag_accesspointssid = true;

	    } else if (strTxt == "ACCESSPOINTWEP128KEY") {
	    
		tag_accesspointwep128key = true;

	    } // end if

	} // end if

	/// TAGS de texto
	if (tinyxml_event_str(event) == (const char*) "text") {

	    /// D
	    if (tag_ipserver) g_db->linkSocket()->setIpServer(txt);
	    if (tag_posuser) g_db->linkSocket()->setPosUser(txt);
	    if (tag_accesspointssid) g_db->linkSocket()->setAccessPointSsid(txt);
	    if (tag_accesspointwep128key) g_db->linkSocket()->setAccessPointWep128Key(txt);

	} // end if


	/// TAGS de fin
	if (tinyxml_event_str(event) == (const char*) "end tag") {

	    if (strTxt == "/CONFIG") {

		/// No se hace nada.
	      
	    /// E
	    } else if (strTxt == "/IPSERVER") {
	    
		tag_ipserver = false;

	    } else if (strTxt == "/POSUSER") {
	    
		tag_posuser = false;

	    } else if (strTxt == "/ACCESSPOINTSSID") {

		tag_accesspointssid = false;
	      
	    } else if (strTxt == "/ACCESSPOINTWEP128KEY") {

		tag_accesspointwep128key = false;
	      
	    } // end if

	} // end if


	return 1;
}



int callback_processCategoryArticles(int event, const char* txt, int len, void* tuser) {

	/*
	    Para poner el tratamiento de una nueva etiqueta hay que crear codigo en cada apartado (A,B,C,...)
	*/

	static BndsCategoryArticle *categArt = NULL;
	static BndsArticle *articulo = NULL;
	
	/// A
	static bool tag_codcategoriaarticulo = false;
	static bool tag_nomcategoriaarticulo = false;
	static bool tag_idarticulo = false;
	static bool tag_nomarticulo = false;
	static bool tag_pvparticulo = false;

	/// B
	static string codcategoriaarticulo = "";
	static string nomcategoriaarticulo = "";
	static string idarticulo = "";
	static string nomarticulo = "";
	static string pvparticulo = "";

	string strTxt;
	strTxt.assign(txt);

	/// TAGS de inicio
	if (tinyxml_event_str(event) == (const char*) "start tag") {

	    if (strTxt == "CATEGORIA") {

		/// Estamos al inicio de una nueva categoria. Inicializamos todas las variables de categoria.
		codcategoriaarticulo = "";
		nomcategoriaarticulo = "";

		tag_codcategoriaarticulo = false;
		tag_nomcategoriaarticulo = false;
		
		/// Se crea la nueva categoria sin datos.
		categArt = g_db->newCategoryArticle(codcategoriaarticulo, nomcategoriaarticulo);
		
	    /// C
	    } else if (strTxt == "CODCATEGORIA") {
	    
		tag_codcategoriaarticulo = true;

	    } else if (strTxt == "NOMCATEGORIA") {
	    
		tag_nomcategoriaarticulo = true;

	    } else if (strTxt == "ARTICULO") {
	      
		/// Estamos al inicio de un nuevo articulo. Inicializamos todas las variables de articulo.
		idarticulo = "";
		nomarticulo = "";
		pvparticulo = "";
		
		tag_idarticulo = false;
		tag_nomarticulo = false;
		tag_pvparticulo = false;

		/// Se crea el nuevo articulo sin datos.
		articulo = categArt->newArticle(idarticulo, nomarticulo, pvparticulo);

	    } else if (strTxt == "IDARTICULO") {
	    
		tag_idarticulo = true;
	
	    } else if (strTxt == "NOMARTICULO") {
	    
		tag_nomarticulo = true;
	
	    } else if (strTxt == "PVPARTICULO") {

		tag_pvparticulo = true;
	    
	    } // end if

	} // end if

	/// TAGS de texto
	if (tinyxml_event_str(event) == (const char*) "text") {

	    /// D
	    if (tag_codcategoriaarticulo) categArt->setCodCategoryArticle(txt);
	    if (tag_nomcategoriaarticulo) categArt->setNomCategoryArticle(txt);
	    if (tag_idarticulo) articulo->setIdArticle(txt);
	    if (tag_nomarticulo) articulo->setNomArticle(txt);
	    if (tag_pvparticulo) articulo->setPvpArticle(txt);

	} // end if


	/// TAGS de fin
	if (tinyxml_event_str(event) == (const char*) "end tag") {

	    if (strTxt == "/CATEGORIA") {

		/// No se hace nada.
	      
	    /// E
	    } else if (strTxt == "/CODCATEGORIA") {
	    
		tag_codcategoriaarticulo = false;

	    } else if (strTxt == "/NOMCATEGORIA") {
	    
		tag_nomcategoriaarticulo = false;

	    } else if (strTxt == "/ARTICULO") {

      		/// No se hace nada.
	      
	    } else if (strTxt == "/IDARTICULO") {
	    
		tag_idarticulo = false;

	    } else if (strTxt == "/NOMARTICULO") {
	    
		tag_nomarticulo = false;
	
	    } else if (strTxt == "/PVPARTICULO") {

		tag_pvparticulo = false;
	    
	    } // end if

	} // end if


	return 1;
}


void drawPixel8bpp ( int x, int y, u16 paletteIndex, u16* buffer )
{
    buffer[ y * 128 + (x / 2) ] &= 0xFF << ( 8 * !(x % 2) );
    buffer[ y * 128 + (x / 2) ] |= paletteIndex << ( 8 * (x % 2) );
}


void drawRectangle8bpp ( int x1, int y1, int x2, int y2, u16 paletteIndex, u16* buffer)
{
    int cordX1 = x1;
    int cordY1 = y1;
    int cordX2 = x2;
    int cordY2 = y2;

    /// Se asegura que cordX2 siempre sea mayor que cordX1.
    if (x1 > x2) {
	cordX1 = x2;
	cordX2 = x1;
    } // end if

    /// Se asegura que cordY2 siempre sea mayor que cordY1.
    if (y1 > y2) {
	cordY1 = y2;
	cordY2 = y1;
    } // end if


    for (int i = cordY1; i < cordY2; i++) {

          for (int j = cordX1; j < cordX2; j++) {
	    
	      drawPixel8bpp ( j, i, paletteIndex, buffer );
	    
	  } // end for

    } // end for
}

/*
    Convierte un numero float en un string con formato 3 (enteros) . 2 (decimales)
*/
string float2string3x2 (float number)
{
    char strChar [80];
    string str;
    sprintf(strChar, "%3.2f", number );
    str.assign(strChar);

    return str;
}


void debugStop()
{
      while(1) {

	  swiWaitForVBlank();

	  scanKeys();
	  
	  if (keysDown() & KEY_A) {
	    
		return;
		
	  } // end if
	  
      } // end while
}

