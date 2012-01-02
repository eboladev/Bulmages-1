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

#ifndef PLUGINBF_ACTIVIDAD_H
#define PLUGINBF_ACTIVIDAD_H

#include "blpostgresqlclient.h"
#include "bfbulmafact.h"
#include "blwidget.h"
#include "bfsubform.h"
#include "bfbuscarreferencia.h"
#include "blsearchwidget.h"
#include "pdefs_pluginbf_actividad.h"
#include "blaction.h"

extern "C" PLUGINBF_ACTIVIDAD_EXPORT int entryPoint ( BfBulmaFact * );
extern "C" PLUGINBF_ACTIVIDAD_EXPORT int BfCompany_createMainWindows_Post(BfCompany *);
extern "C" PLUGINBF_ACTIVIDAD_EXPORT int BlSubFormDelegate_createEditor(BlSubFormDelegate *);
extern "C" PLUGINBF_ACTIVIDAD_EXPORT int BlSubFormDelegate_setModelData(BlSubFormDelegate *);
extern "C" PLUGINBF_ACTIVIDAD_EXPORT int BlSubFormDelegate_setEditorData(BlSubFormDelegate *);
extern "C" PLUGINBF_ACTIVIDAD_EXPORT int BlSubForm_editFinished(BlSubForm *);
extern "C" PLUGINBF_ACTIVIDAD_EXPORT int BlSubForm_BlSubForm_Post ( BlSubForm * );
extern "C" PLUGINBF_ACTIVIDAD_EXPORT int Busqueda_on_mui_buscar_clicked ( BlSearchWidget * );
extern "C" PLUGINBF_ACTIVIDAD_EXPORT int BlAction_actionTriggered(BlAction *);


class SubForm_Actividad : public QObject
{
    Q_OBJECT

public:
    SubForm_Actividad ( BlSubForm * );
    ~SubForm_Actividad();
    void editarActividad( QString);
    void nuevoActividad();
    void seleccionarActividad(BfSubForm *);

public slots:
    virtual void s_pintaMenu ( QMenu * );
    virtual void s_trataMenu ( QAction * );
};


#endif

