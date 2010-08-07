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

#include "bndsconfigview.h"
#include "bndsfunctions.h"


BndsConfigView::BndsConfigView()
{
}


BndsConfigView::~BndsConfigView()
{
}


int BndsConfigView::showMenu()
{
    static int option = 0;
    int noptions = 4;

    PrintConsole conSub = g_video->consoleSub();
    consoleSelect( &conSub );
    
    while (1) {
      	  consoleClear();

	  iprintf("\x1b[0;2HMenu de configuracion:");
	  iprintf("\x1b[1;2H----------------------");
	  
          iprintf("\x1b[3;2HAccess Point SSID");
          iprintf("\x1b[4;2HClave Wep AP");
          iprintf("\x1b[5;2HIP servidor");
          iprintf("\x1b[6;2HTrabajador TPV");	  
	  
  	  iprintf("\x1b[10;2H(A) Editar dato.");
  	  iprintf("\x1b[11;2H(L) Volver al programa.");

	  
	  iprintf("\x1b[%i;0H->", 3 + option);
	  
	  swiWaitForVBlank();

	  scanKeys();
	  
	  if (keysDown() & KEY_UP) {
	  
	      option--;
	      if (option < 0) option = noptions - 1;
	    
	  } else if (keysDown() & KEY_DOWN) {

	      option++;
	      if (option > noptions - 1) option = 0;

	  } else if (keysDown() & KEY_L) {
	    
	      return -1;
	    
	  } else if (keysDown() & KEY_A) {
	    
	      switch(option) {
                case 0:
                    showApSSID();
                    break;
                case 1:
                    showApWep128Key();
                    break;
                case 2:
                    showServerIp();
                    return 0;
                    break;
                case 3:
                    showPosUser();
                    break;
	      } // end switch
		

	  } // end if

    } // end while

    return 0;
}


void BndsConfigView::showApWep128Key()
{
  PrintConsole conSub = g_video->consoleSub();
  consoleSelect( &conSub );
  
  Keyboard *defaultKeyboard = keyboardGetDefault();

  Keyboard kbd;
  keyboardInit(&kbd, 3, BgType_Text4bpp, BgSize_T_256x512, defaultKeyboard->mapBase, defaultKeyboard->tileBase, false, true);
  //kbd.OnKeyPressed = callback_keyboardOnKeyPressed;


  string cadena;
  int key;

    while (1) {

      consoleClear();
      iprintf("\x1b[1;0HClave WEP del access point:\n");

      keyboardShow();
      cadena = accessPointWep128Key();

      while(1) {
          key = keyboardUpdate();

          if (key > 0) {

              if (key == 8) {
                  if (cadena.size() >= 1) {
                    cadena.resize(cadena.size()-1);
                  } // end if
	      } else if (key == 10) {
		  // Intro
		  break;
              } else if (key >= 11) {
                    cadena.push_back(key);
              } // end if

            } // end if

          iprintf("\x1b[2;0H%s_ ", cadena.c_str());

          swiWaitForVBlank();
      } // end while

      keyboardHide();

      iprintf("\n\n");
      iprintf("(L) Aceptar.\n");
      iprintf("(R) Cancelar.\n");
      iprintf("(A) Repetir.\n");

      while (1) {

        scanKeys();

        if (keysDown() & KEY_L) {
            /// Aceptar.
	    setAccessPointWep128Key(cadena);
            return;
        } else if (keysDown() & KEY_R) {
            /// Cancelar.
            return;
        } else if (keysDown() & KEY_A) {
            /// Repetir.
            break;
        } // end if

        swiWaitForVBlank();

      } // end while

    } // end while

}


void BndsConfigView::showPosUser()
{
}


void BndsConfigView::showApSSID()
{
    /// 1) Muestra el AP SSID guardado.
    /// 2) Muestra opción para buscar y seleccionar otro AP SSID.
  PrintConsole conSub = g_video->consoleSub();
  consoleSelect( &conSub );
  
  string cadena;
  int key;

    while (1) {

      consoleClear();
      iprintf("\x1b[1;0HAP SSID actual:\n");

      cadena = accessPointSsid();
      
      /// Muestra el AP SSID actual.
      if (cadena == "") {
	iprintf("\x1b[2;0H\x1b[32;1m<SIN SSID ASIGNADO>\x1b[39m");
      } else {
	iprintf("\x1b[2;0H%s_ ", cadena.c_str());
      } // end if

      
      swiWaitForVBlank();


      iprintf("\n\n");
      iprintf("(A) Buscar Access Point.\n");
      

      

      iprintf("\n\n");
      iprintf("(L) Aceptar.\n");
      iprintf("(R) Cancelar.\n");
      iprintf("(A) Repetir.\n");

      while (1) {

        scanKeys();

        if (keysDown() & KEY_L) {
            /// Aceptar.
            setAccessPointSsid(cadena);
            return;
        } else if (keysDown() & KEY_R) {
            /// Cancelar.
            return;
        } else if (keysDown() & KEY_A) {
            /// Repetir.
            break;
        } // end if

        swiWaitForVBlank();

      } // end while

    } // end while

    
}


void BndsConfigView::showServerIp()
{
  PrintConsole conSub = g_video->consoleSub();
  consoleSelect( &conSub );
  
  Keyboard *defaultKeyboard = keyboardGetDefault();

  Keyboard kbd;
  keyboardInit(&kbd, 3, BgType_Text4bpp, BgSize_T_256x512, defaultKeyboard->mapBase, defaultKeyboard->tileBase, false, true);
  //kbd.OnKeyPressed = callback_keyboardOnKeyPressed;


  string cadena;
  int key;

    while (1) {

      consoleClear();
      iprintf("\x1b[1;0HIP del servidor BulmaTPV:\n");

      keyboardShow();
      cadena = ipServer();

      while(1) {
          key = keyboardUpdate();

          if (key > 0) {

              if (key == 8) {
                  if (cadena.size() >= 1) {
                    cadena.resize(cadena.size()-1);
                    } // end if
              } else if (((key >= 48) && (key <=57)) || (key == 46)) {
                    // Solo deja que se escriban numeros (0-9) y punto (.)
                    cadena.push_back(key);
                } else if (key == 10) {
                    // Intro
                    break;

                }

            } // end if

          iprintf("\x1b[2;0H%s_ ", cadena.c_str());

          swiWaitForVBlank();
      } // end while

      keyboardHide();

      iprintf("\n\n");
      iprintf("(L) Aceptar.\n");
      iprintf("(R) Cancelar.\n");
      iprintf("(A) Repetir.\n");

      while (1) {

        scanKeys();

        if (keysDown() & KEY_L) {
            /// Aceptar.
            setIpServer(cadena);
            return;
        } else if (keysDown() & KEY_R) {
            /// Cancelar.
            return;
        } else if (keysDown() & KEY_A) {
            /// Repetir.
            break;
        } // end if

        swiWaitForVBlank();

      } // end while

    } // end while

}


int BndsConfigView::exec()
{  
      while(1) {
	  int res = showMenu();

	  swiWaitForVBlank();

	  scanKeys();
	  
	  if (res == -1) {
	      break;
	  } // end if

      } // end while

    return 0;
}

