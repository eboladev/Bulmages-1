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

#ifndef PLUGINBT_ALIASTALLASYCOLORES_H
#define PLUGINBT_ALIASTALLASYCOLORES_H

#include "btbulmatpv.h"
#include "btcompany.h"
#include "btticket.h"
#include "mticket.h"
#include "mticketivainc.h"
#include "pdefs_pluginbt_aliastallasycolores.h"


extern "C" PLUGINBT_ALIASTALLASYCOLORES_EXPORT int BtTicket_insertarArticuloCodigo_Post ( BtTicket * );
extern "C" PLUGINBT_ALIASTALLASYCOLORES_EXPORT int BtTicket_insertarArticuloCodigoNL_Post ( BtTicket * );
extern "C" PLUGINBT_ALIASTALLASYCOLORES_EXPORT int BtTicket_agregarLinea_Post ( BtTicket * );
extern "C" PLUGINBT_ALIASTALLASYCOLORES_EXPORT int MTicket_pintar ( MTicket * );
extern "C" PLUGINBT_ALIASTALLASYCOLORES_EXPORT int MTicketIVAInc_pintar ( MTicketIVAInc * );
extern "C" PLUGINBT_ALIASTALLASYCOLORES_EXPORT int BtTicket_imprimir(BtTicket *);
extern "C" PLUGINBT_ALIASTALLASYCOLORES_EXPORT int BtTicket_imprimirIVAInc(BtTicket *);


#endif

