/***************************************************************************
 *   Copyright (C) 2008 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#include "blfunctions.h"
#include "blpostgresqlclient.h"
#include "bccompany.h"
#include "bfcompany.h"
#include "pdefs_pluginbl_exaro.h"
#include "blaction.h"


extern "C" PLUGINBL_EXARO_EXPORT int entryPoint ( QMainWindow * );
extern "C" PLUGINBL_EXARO_EXPORT int BfCompany_createMainWindows_Post ( BfCompany * );
extern "C" PLUGINBL_EXARO_EXPORT int BcCompany_createMainWindows_Post ( BcCompany * );
extern "C" PLUGINBL_EXARO_EXPORT int BlAction_actionTriggered(BlAction *);
extern "C" PLUGINBL_EXARO_EXPORT int init();

