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

#ifndef PLUGINBF_CLIENTECOBRO_H
#define PLUGINBF_CLIENTECOBRO_H

#include "blpostgresqlclient.h"
#include "bfbulmafact.h"
#include "blwidget.h"
#include "clienteview.h"
#include "bfbuscarreferencia.h"
#include "facturaview.h"
#include "facturaivaincclienteview.h"
#include "albaranclienteview.h"
#include "pedidoclienteview.h"
#include "pdefs_pluginbf_clientecobro.h"
#include "blaction.h"

extern "C" PLUGINBF_CLIENTECOBRO_EXPORT int entryPoint ( BfBulmaFact * );
extern "C" PLUGINBF_CLIENTECOBRO_EXPORT int SNewCobroView (BfCompany *);
extern "C" PLUGINBF_CLIENTECOBRO_EXPORT int BfCompany_createMainWindows_Post(BfCompany *);
extern "C" PLUGINBF_CLIENTECOBRO_EXPORT int ClienteView_ClienteView_Post (ClienteView *);
extern "C" PLUGINBF_CLIENTECOBRO_EXPORT int ClienteView_cargarPost_Post (ClienteView *);
extern "C" PLUGINBF_CLIENTECOBRO_EXPORT int BfBuscarReferencia_on_mui_abrirtodo_clicked_Post (BfBuscarReferencia *);
extern "C" PLUGINBF_CLIENTECOBRO_EXPORT int FacturaView_FacturaView ( FacturaView * );
extern "C" PLUGINBF_CLIENTECOBRO_EXPORT int AlbaranClienteView_AlbaranClienteView ( AlbaranClienteView * );
extern "C" PLUGINBF_CLIENTECOBRO_EXPORT int PedidoClienteView_PedidoClienteView ( PedidoClienteView * );
extern "C" PLUGINBF_CLIENTECOBRO_EXPORT int BlAction_actionTriggered(BlAction *);
extern "C" PLUGINBF_CLIENTECOBRO_EXPORT int FacturaIVAIncClienteView_FacturaIVAIncClienteView ( FacturaIVAIncClienteView * );
#endif

