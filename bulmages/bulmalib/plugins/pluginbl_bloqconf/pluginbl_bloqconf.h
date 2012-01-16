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


#include "QObject"
#include "QMenu"
#include "QAction"


#include "blmainwindow.h"
#include "blfunctions.h"
#include "bccompany.h"
#include "bfcompany.h"
#include "blsubform.h"
#include "pdefs_pluginbl_bloqconf.h"
#include "blaction.h"

extern "C" PLUGINBL_BLOQCONF_EXPORT int entryPoint ( BlMainWindow * );
extern "C" PLUGINBL_BLOQCONF_EXPORT int BfCompany_createMainWindows_Post ( BfCompany * );
extern "C" PLUGINBL_BLOQCONF_EXPORT int BcCompany_createMainWindows_Post ( BcCompany * );
extern "C" PLUGINBL_BLOQCONF_EXPORT int BlSubForm_BlSubForm_Post ( BlSubForm * );
extern "C" PLUGINBL_BLOQCONF_EXPORT int BlAction_actionTriggered (BlAction *);
