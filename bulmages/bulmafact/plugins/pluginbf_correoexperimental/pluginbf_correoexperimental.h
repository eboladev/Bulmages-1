/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
 *   Copyright (C) 2012 by Daniel Ripoll Osma.                             *
 *   info@danielripoll.es                                                  *
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

#ifndef PLUGINBF_CORREOEXPERIMENTAL_H
#define PLUGINBF_CORREOEXPERIMENTAL_H

#include "facturaview.h"
#include "presupuestoview.h"
#include "pedidoclienteview.h"
#include "albaranclienteview.h"
#include "bfbulmafact.h"
#include "pdefs_pluginbf_correoexperimental.h"


extern "C" PLUGINBF_CORREOEXPERIMENTAL_EXPORT int AlbaranClienteView_AlbaranClienteView ( AlbaranClienteView * );
extern "C" PLUGINBF_CORREOEXPERIMENTAL_EXPORT int PedidoClienteView_PedidoClienteView ( PedidoClienteView * );
extern "C" PLUGINBF_CORREOEXPERIMENTAL_EXPORT int PresupuestoView_PresupuestoView ( PresupuestoView * );
extern "C" PLUGINBF_CORREOEXPERIMENTAL_EXPORT int FacturaView_FacturaView ( FacturaView * );

extern "C" PLUGINBF_CORREOEXPERIMENTAL_EXPORT int LaunchChoseMailer ();
extern "C" PLUGINBF_CORREOEXPERIMENTAL_EXPORT int Thunderbird ( QString &recipient, QString &bcc, QString &subject, QString &body, QString &attached );
extern "C" PLUGINBF_CORREOEXPERIMENTAL_EXPORT int Kmail ( QString &recipient, QString &bcc, QString &subject, QString &body, QString &attached );
extern "C" PLUGINBF_CORREOEXPERIMENTAL_EXPORT int Evolution ( QString &recipient, QString &bcc, QString &subject, QString &body, QString &attached );
#ifdef Q_OS_WIN32
extern "C" PLUGINBF_CORREOEXPERIMENTAL_EXPORT int Outlook ( QString &recipient, QString &bcc, QString &subject, QString &body, QString &attached );
 #endif
extern "C" PLUGINBF_CORREOEXPERIMENTAL_EXPORT int bfSendEmail ( QString &recipient, QString &bcc, QString &subject, QString &body, QString &attached );

extern "C" PLUGINBF_CORREOEXPERIMENTAL_EXPORT int entryPoint ( BfBulmaFact * );

#endif

