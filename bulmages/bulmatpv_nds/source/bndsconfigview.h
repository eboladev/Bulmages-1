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

#ifndef BNDSCONFIGVIEW_H
#define BNDSCONFIGVIEW_H

#include <nds.h>
#include <stdio.h>
#include <string>

using namespace std;

#include "bndsvideo.h"
#include "bndsdb.h"


class BndsConfigView
{
  private:
     BndsConfig* m_config;
     
  public:
    BndsConfigView(BndsConfig* config);
    ~BndsConfigView();
    void showServerIp();
    void showPosUser();
    void showApSSID();
    void showApWep128Key();
    int showMenu();
    int exec();
};

#endif


