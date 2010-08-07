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

#ifndef BNDSVIDEO_H
#define BNDSVIDEO_H

#include <nds.h>
#include <stdio.h>
#include <string>


using namespace std;


class BndsVideo
{
  private:
      int m_totalItemsTemplateListA;
      touchPosition m_touch;

  protected:
      int m_bgMain;
      int m_bgSub;
      PrintConsole m_conMain;
      PrintConsole m_conSub;
  
  public:
      BndsVideo();
      ~BndsVideo();
      void ndsInit();
      void ndsInitSplash();
      void ndsInitConfig();
      int backgroundMain();
      int backgroundSub();
      
      PrintConsole consoleMain();
      PrintConsole consoleSub();
      void resetBgMain();
      void resetBgSub();
      
      void showTemplateListA(int totalItems, u16 indexPalette = 1, int offset = 0, bool previousPage = false);
      int eventTemplateListA(bool previousPage = false);

      void showTemplateListB(int totalItems, u16 indexPalette = 1);
      int eventTemplateListB();

      void showTemplateListC(int lineSelected, u16 indexPalette = 1);
      int eventTemplateListC();
};


extern BndsVideo *g_video;

#endif

