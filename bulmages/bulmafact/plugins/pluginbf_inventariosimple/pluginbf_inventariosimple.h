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

#ifndef PLUGINBF_INVENTARIOSIMPLE_H
#define PLUGINBF_INVENTARIOSIMPLE_H

#include "blpostgresqlclient.h"
#include "bfbulmafact.h"
#include "blwidget.h"
#include "actividadview.h"
#include "pdefs_pluginbf_inventariosimple.h"
#include "blaction.h"

extern "C" PLUGINBF_INVENTARIOSIMPLE_EXPORT int entryPoint ( BfBulmaFact * );
extern "C" PLUGINBF_INVENTARIOSIMPLE_EXPORT int ActividadView_ActividadView(ActividadView *);
extern "C" PLUGINBF_INVENTARIOSIMPLE_EXPORT int BlForm_guardar_Post(BlForm *);
extern "C" PLUGINBF_INVENTARIOSIMPLE_EXPORT int BlForm_load(BlForm *);
extern "C" PLUGINBF_INVENTARIOSIMPLE_EXPORT int BfSubForm_pressedAsterisk ( BfSubForm * );
extern "C" PLUGINBF_INVENTARIOSIMPLE_EXPORT int BlSubForm_BlSubForm_Post ( BlSubForm * );
extern "C" PLUGINBF_INVENTARIOSIMPLE_EXPORT int BlSubForm_preparaMenu ( BlSubForm * );
extern "C" PLUGINBF_INVENTARIOSIMPLE_EXPORT int BlAction_actionTriggered ( BlAction * );

class SubForm_InventarioSimple : public QObject
{
    Q_OBJECT

public:
    SubForm_InventarioSimple ( BlSubForm * );
    ~SubForm_InventarioSimple();


public slots:
    void seleccionarMaterial(BfSubForm *sub = NULL);
    virtual void s_pintaMenu ( QMenu * );
    virtual void s_trataMenu ( QAction * );
};




#endif

