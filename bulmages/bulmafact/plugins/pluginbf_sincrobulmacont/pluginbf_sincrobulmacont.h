/***************************************************************************
 *   Copyright (C) 2010 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#ifndef PLUGINBF_SINCROBULMACONT_H
#define PLUGINBF_SINCROBULMACONT_H

#include "bfbulmafact.h"
#include "bcbulmacont.h"
#include "familiasview.h"
#include "pdefs_pluginbf_sincrobulmacont.h"


extern "C" PLUGINBF_SINCROBULMACONT_EXPORT int entryPoint ( BfBulmaFact * );
//extern "C" PLUGINBF_SINCROBULMACONT_EXPORT int BfCompany_createMainWindows_Post(BfCompany *);

extern "C" PLUGINBF_SINCROBULMACONT_EXPORT int FamiliasView_FamiliasView ( FamiliasView * );


#endif

