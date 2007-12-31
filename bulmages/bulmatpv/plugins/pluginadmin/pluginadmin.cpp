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


#include "pluginadmin.h"
#include "funcaux.h"
#include "empresatpv.h"
#include "admin.h"
#include "plugins.h"
#include "ticket.h"
#include "qapplication2.h"



Admin *g_admin;
QDockWidget *g_admin1;
///
/**
\return
**/
int entryPoint ( BulmaTPV *tpv )
{
    _depura ( "entryPoint", 0 );

    /// Vamos a probar con un docwindow.
    g_admin1 = new QDockWidget ( "Administracion", tpv );
    g_admin1->setFeatures ( QDockWidget::AllDockWidgetFeatures );

    g_admin1->setGeometry ( 100, 100, 100, 500 );
    g_admin1->resize ( 330, 400 );
    tpv->addDockWidget ( Qt::BottomDockWidgetArea, g_admin1 );
    g_admin1->show();

    _depura ( "END entryPoint", 0 );
    return 0;
}

int EmpresaTPV_createMainWindows_Post ( EmpresaTPV *etpv )
{
    g_admin = new Admin ( etpv, g_admin1 );
    g_admin1->setWidget ( g_admin );
    return 0;
}
