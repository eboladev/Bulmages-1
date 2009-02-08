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

#include <QDockWidget>

#include "pluginbuscaarticulo.h"
#include "funcaux.h"
#include "empresatpv.h"
#include "plugins.h"
#include "ticket.h"
#include "blapplication.h"
#include "busquedaarticulo.h"
#include "blfixed.h"
#include "bdockwidget.h"

/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;


BusquedaArticulo *g_busc;
BDockWidget *g_doc1;
EmpresaTPV * g_emp;
///
/**
\return
**/
int entryPoint ( BulmaTPV *tpv )
{
    _depura ( "pluginbuscaarticulo::entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginbusquedaarticulo", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    g_emp = tpv->empresaTPV();

    /// Vamos a probar con un docwindow.
    g_doc1 = new BDockWidget ( _("Articulo"), tpv, "articulodock" );
    g_doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );
    g_doc1->setGeometry ( 100, 100, 100, 500 );
    g_doc1->resize ( 330, 400 );
    tpv->addDockWidget ( Qt::TopDockWidgetArea, g_doc1 );
    g_doc1->show();
    g_doc1->cargaconf();
    _depura ( "END pluginbuscaarticulo::entryPoint", 0 );
    return 0;
}

int EmpresaTPV_createMainWindows_Post ( EmpresaTPV *etpv )
{
    g_busc = new BusquedaArticulo ( 0 );
    g_busc->setEmpresaBase ( etpv );
    g_doc1->setWidget ( g_busc );

    return 0;
}

int BusquedaArticulo_on_m_codigocompletoarticulo_editingFinished_Post ( BusquedaArticulo *busc )
{
    if ( busc->idarticulo() != "" )
        g_emp->ticketActual() ->insertarArticulo (  busc->idarticulo(), BlFixed ( "1" ) );
    return 0;
}

