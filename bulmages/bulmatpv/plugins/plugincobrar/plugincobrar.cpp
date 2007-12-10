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


#include "plugincobrar.h"
#include "funcaux.h"
#include "empresatpv.h"
#include "cobrar.h"
#include "plugins.h"
#include "ticket.h"
#include "qapplication2.h"

/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, Fixed> base;


Cobrar *g_cobrar;
QDockWidget *g_doc1;
///
/**
\return
**/
int entryPoint(BulmaTPV *tpv) {
    _depura("entryPoint", 0);

    /// Vamos a probar con un docwindow.
    g_doc1 = new QDockWidget("Cobrar", tpv);
    g_doc1->setFeatures(QDockWidget::AllDockWidgetFeatures);

    g_doc1->setGeometry(100, 100, 100, 500);
    g_doc1->resize(330, 400);
    tpv->addDockWidget(Qt::LeftDockWidgetArea, g_doc1);
    g_doc1->show();

    _depura("END entryPoint", 0);
    return 0;
}

int EmpresaTPV_createMainWindows_Post(EmpresaTPV *etpv) {
	g_cobrar = new Cobrar(etpv, g_doc1);
//	etpv->pWorkspace()->addWindow(g_cobrar);
        g_doc1->setWidget(g_cobrar);

//        ((BulmaTPV *)etpv->parent())->addDockWidget(Qt::LeftDockWidgetArea, g_cobrar);
	return 0;
}



