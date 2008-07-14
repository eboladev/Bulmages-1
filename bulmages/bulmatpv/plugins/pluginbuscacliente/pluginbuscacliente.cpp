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

#include "pluginbuscacliente.h"
#include "funcaux.h"
#include "empresatpv.h"
#include "plugins.h"
#include "ticket.h"
#include "qapplication2.h"
#include "busquedacliente.h"
#include "bdockwidget.h"

/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, Fixed> base;


BusquedaCliente *g_busc;
BDockWidget *g_doc1;
EmpresaTPV * g_emp;
///
/**
\return
**/
int entryPoint ( BulmaTPV *tpv )
{
    _depura ( "entryPoint", 0 );
    g_emp = tpv->empresaTPV();

    /// Vamos a probar con un docwindow.
    g_doc1 = new BDockWidget ( "Cliente", tpv, "buscaclientedock" );
    g_doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );
    g_doc1->setGeometry ( 100, 100, 100, 500 );
    g_doc1->resize ( 330, 400 );
    tpv->addDockWidget ( Qt::TopDockWidgetArea, g_doc1 );
    g_doc1->show();
    g_doc1->cargaconf();
    _depura ( "END entryPoint", 0 );
    return 0;
}

int EmpresaTPV_createMainWindows_Post ( EmpresaTPV *etpv )
{
    g_busc = new BusquedaCliente ( 0 );
    g_busc->setEmpresaBase ( etpv );
    g_doc1->setWidget ( g_busc );

    return 0;
}

int BusquedaCliente_on_m_cifcliente_editingFinished_Post ( BusquedaCliente *busc )
{
    if ( busc->idcliente() != "" ) {
        g_emp->ticketActual() ->setDBvalue ( "idcliente", busc->idcliente() );
        g_emp->ticketActual() ->pintar();
        g_emp->setValorInput ( "" );
        g_emp->pulsaTecla ( 0, "" );
    } // end if
    return 0;
}

