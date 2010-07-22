/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef PLUGINBT_IVAINCLUIDO_H
#define PLUGINBT_IVAINCLUIDO_H

#include "btbulmatpv.h"
#include "articulolist.h"
#include "btcompany.h"
#include "btticket.h"
#include "pdefs_pluginbt_ivaincluido.h"


//extern "C" PLUGINBT_IVAINCLUIDO_EXPORT int entryPoint ( BulmaTPV * );
extern "C" PLUGINBT_IVAINCLUIDO_EXPORT int BtTicket_insertarArticulo_Post ( BtTicket * );
extern "C" PLUGINBT_IVAINCLUIDO_EXPORT int BtTicket_agregarLinea_Post ( BtTicket *);
extern "C" PLUGINBT_IVAINCLUIDO_EXPORT int BtTicket_ponerPrecio_Post ( BtTicket * );
// extern "C" PLUGINBT_IVAINCLUIDO_EXPORT int BtTicket_imprimir ( BtTicket * );
extern "C" PLUGINBT_IVAINCLUIDO_EXPORT int ArticuloListSubForm_ArticuloListSubForm_Post(ArticuloListSubForm *);
//extern "C" PLUGINBT_IVAINCLUIDO_EXPORT int BtCompany_x ( BtCompany * );
extern "C" PLUGINBT_IVAINCLUIDO_EXPORT int BtCompany_z ( BtCompany * );


#endif

