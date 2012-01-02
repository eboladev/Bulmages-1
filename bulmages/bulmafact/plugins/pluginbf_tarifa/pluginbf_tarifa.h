/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifndef PLUGINBF_TARIFA_H
#define PLUGINBF_TARIFA_H

#include "blpostgresqlclient.h"
#include "clienteview.h"
#include "articuloview.h"
#include "bfbulmafact.h"
#include "pdefs_pluginbf_tarifa.h"
#include "blaction.h"

extern "C" PLUGINBF_TARIFA_EXPORT int entryPoint ( BfBulmaFact * );
extern "C" PLUGINBF_TARIFA_EXPORT int ClienteView_ClienteView ( ClienteView * );
extern "C" PLUGINBF_TARIFA_EXPORT int ArticuloView_ArticuloView ( ArticuloView * );
extern "C" PLUGINBF_TARIFA_EXPORT int ArticuloView_load ( ArticuloView * );
extern "C" PLUGINBF_TARIFA_EXPORT int ArticuloView_guardar_post ( ArticuloView * );
extern "C" PLUGINBF_TARIFA_EXPORT int ArticuloView_borrar ( ArticuloView * );
extern "C" PLUGINBF_TARIFA_EXPORT int BfSubForm_BfSubForm ( BfSubForm * );
extern "C" PLUGINBF_TARIFA_EXPORT int BfSubForm_calculaPVP ( BfSubForm * );
extern "C" PLUGINBF_TARIFA_EXPORT int BlAction_actionTriggered(BlAction *);

#endif

