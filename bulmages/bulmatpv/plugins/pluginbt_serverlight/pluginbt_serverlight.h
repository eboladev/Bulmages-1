/***************************************************************************
 *   Copyright (C) 2010 by Fco. Javier M. C.                               *
 *   comercial@todo-redes.com                                              *
 *   http://www.todo-redes.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef PLUGINBT_SERVERLIGHT_H
#define PLUGINBT_SERVERLIGHT_H

#include "btbulmatpv.h"
#include "btticket.h"
#include "btcompany.h"
#include "pdefs_pluginbt_serverlight.h"


extern "C" PLUGINBT_SERVERLIGHT_EXPORT int entryPoint ( BtBulmaTPV * );
extern "C" PLUGINBT_SERVERLIGHT_EXPORT int BtCompany_createMainWindows_Post ( BtCompany * );


#endif

