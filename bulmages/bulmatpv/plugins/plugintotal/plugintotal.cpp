/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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


#include "plugintotal.h"
#include "funcaux.h"
#include "empresatpv.h"
#include "total.h"
#include "plugins.h"
#include "ticket.h"


Total *g_tot;
///
/**
\return
**/
int entryPoint(BulmaTPV *tpv) {
    _depura("entryPoint", 0);
    _depura("END entryPoint", 0);
    return 0;
}

int EmpresaTPV_createMainWindows_Post(EmpresaTPV *etpv) {
	g_tot = new Total(NULL);
	etpv->pWorkspace()->addWindow(g_tot);
	return 0;
}

int Ticket_pintar(Ticket *tick) {
    DBRecord *item;
    Fixed total("0");
    for (int i = 0; i < tick->listaLineas()->size(); ++i) {
        item = tick->listaLineas()->at(i);
	Fixed totalLinea("0.00");
	totalLinea = Fixed(item->DBvalue("cantlticket")) * Fixed(item->DBvalue("pvplticket"));
	total = total + totalLinea;
    }// end for
    /// Pintamos el total
    g_tot->mui_display->display(total.toQString());
}


