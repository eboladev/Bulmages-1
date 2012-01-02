/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#ifndef PLUGINBF_CUADRANTE_H
#define PLUGINBF_CUADRANTE_H

#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QObject>

#include "blapplication.h"
#include "bfbulmafact.h"
#include "blpostgresqlclient.h"
#include "blwidget.h"
#include "almacenview.h"
#include "trabajadorview.h"
#include "pdefs_pluginbf_cuadrante.h"
#include "blaction.h"

extern "C" PLUGINBF_CUADRANTE_EXPORT int entryPoint ( BfBulmaFact * );
extern "C" PLUGINBF_CUADRANTE_EXPORT int AlmacenView_AlmacenView ( AlmacenView * );
extern "C" PLUGINBF_CUADRANTE_EXPORT int TrabajadorView_TrabajadorView ( TrabajadorView * );
extern "C" PLUGINBF_CUADRANTE_EXPORT int TrabajadorView_on_mui_lista_currentItemChanged_Post ( TrabajadorView *trab );
extern "C" PLUGINBF_CUADRANTE_EXPORT int TrabajadorView_on_mui_guardar_clicked ( TrabajadorView *trab );
extern "C" PLUGINBF_CUADRANTE_EXPORT int BlAction_actionTriggered(BlAction *);

#endif

