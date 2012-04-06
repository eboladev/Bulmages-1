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

#ifndef PLUGINBL_ATTACHDOCUMENT_H
#define PLUGINBL_ATTACHDOCUMENT_H

#include "blfunctions.h"
#include "blpostgresqlclient.h"
#include "blmaincompany.h"
#include "blform.h"
#include "pdefs_pluginbl_attachdocument.h"


extern "C" PLUGINBL_ATTACHDOCUMENT_EXPORT int entryPoint ( QMainWindow * );
extern "C" PLUGINBL_ATTACHDOCUMENT_EXPORT int BlForm_load ( BlForm * );
extern "C" PLUGINBL_ATTACHDOCUMENT_EXPORT int BlForm_DesBlForm ( BlForm * );
extern "C" PLUGINBL_ATTACHDOCUMENT_EXPORT int BlForm_BlForm ( BlForm * );
extern "C" PLUGINBL_ATTACHDOCUMENT_EXPORT int BlForm_loadSpecs ( BlForm * );
extern "C" PLUGINBL_ATTACHDOCUMENT_EXPORT int BlMainCompany_init ( BlMainCompany * );

#endif

