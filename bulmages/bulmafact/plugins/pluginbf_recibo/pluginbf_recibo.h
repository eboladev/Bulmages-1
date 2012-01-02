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

#ifndef PLUGINBF_RECIBO_H
#define PLUGINBF_RECIBO_H

#include "blpostgresqlclient.h"
#include "bfbulmafact.h"
#include "blwidget.h"
#include "bfbuscarreferencia.h"
#include "actividadview.h"
#include "pdefs_pluginbf_recibo.h"
#include "blaction.h"

extern "C" PLUGINBF_RECIBO_EXPORT int SNewReciboView (BfCompany *);
extern "C" PLUGINBF_RECIBO_EXPORT int entryPoint ( BfBulmaFact * );
extern "C" PLUGINBF_RECIBO_EXPORT int BfCompany_createMainWindows_Post(BfCompany *);
extern "C" PLUGINBF_RECIBO_EXPORT int ActividadView_ActividadView(ActividadView *);
extern "C" PLUGINBF_RECIBO_EXPORT  int BlAction_actionTriggered(BlAction *);


class EmitirRecibos : public QObject, BlMainCompanyPointer
{
    Q_OBJECT

public:
    BfBulmaFact *m_bges;
    ActividadView *m_actividad;

public:
    EmitirRecibos(BlMainCompany *);
    ~EmitirRecibos();
public slots:
    void elslot();
};


#endif

