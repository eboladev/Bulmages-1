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

#ifdef Q_WS_WIN
# define MY_EXPORT __declspec(dllexport)
#else
# define MY_EXPORT
#endif

#include "btbulmatpv.h"
#include "articulolist.h"
#include "btcompany.h"
#include "btticket.h"


//extern "C" MY_EXPORT int entryPoint ( BulmaTPV * );
extern "C" MY_EXPORT int Ticket_insertarArticulo_Post ( BtTicket * );
extern "C" MY_EXPORT int Ticket_agregarLinea_Post ( BtTicket *);
extern "C" MY_EXPORT int Ticket_ponerPrecio_Post ( BtTicket * );
extern "C" MY_EXPORT int Ticket_imprimir ( BtTicket * );
extern "C" MY_EXPORT int ArticuloListSubForm_ArticuloListSubForm_Post(ArticuloListSubForm *);
extern "C" MY_EXPORT int BtCompany_x ( BtCompany * );
extern "C" MY_EXPORT int BtCompany_z ( BtCompany * );

