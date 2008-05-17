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
#include <QToolButton>
#include <QLineEdit>
#include <QTableWidget>

#include "pluginartsubform.h"
#include "funcaux.h"
#include "ticket.h"
#include "empresatpv.h"
#include "mticket.h"
#include "busquedaarticulo.h"


///
/**
\return
**/
int entryPoint ( BulmaTPV *tpv )
{
    _depura ( "entryPoint", 0 );
    _depura ( "END entryPoint", 0 );
    return 0;
}


int EmpresaTPV_createMainWindows_Post ( EmpresaTPV *etpv )
{


    ArticuloList1 *lan = new ArticuloList1((Company *) etpv, NULL, 0, ArticuloList1::SelectMode);
    g_main->setCentralWidget( lan );

/*
    BusquedaArticulo *busc = new BusquedaArticulo(0);
    busc->setEmpresaBase(etpv);
    g_main->setCentralWidget ( busc );
*/
//    lan->connect ( lan, SIGNAL ( selected ( QString  ) ), this, SLOT ( elslot ( QString ) ) );

    return 0;
}
