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

#include <QtWidgets/QDockWidget>

#include "pluginbt_buscaarticulo.h"
#include "btcompany.h"
#include "blplugins.h"
#include "btticket.h"
#include "blapplication.h"
#include "bfbuscararticulo.h"
#include "blfixed.h"
#include "bldockwidget.h"


/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;
BfBuscarArticulo *g_busc;
BlDockWidget *g_doc1;
BtCompany * g_pluginbt_buscaarticulo_emp;


///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbt_buscaarticulo", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    g_pluginbt_buscaarticulo_emp = tpv->company();

    /// Vamos a probar con un docwindow.
    g_doc1 = new BlDockWidget ( _ ( "Articulo" ), tpv, "articulodock" );
    g_doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );
    g_doc1->setGeometry ( 100, 100, 100, 500 );
    g_doc1->resize ( 330, 400 );
    tpv->addDockWidget ( Qt::TopDockWidgetArea, g_doc1 );
    g_doc1->show();
    g_doc1->loadConfig();
    
    return 0;
}

int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{
    g_busc = new BfBuscarArticulo ( 0 );
    g_busc->setMainCompany ( etpv );
    g_doc1->setWidget ( g_busc );

    return 0;
}

int BfBuscarArticulo_on_m_codigocompletoarticulo_editingFinished_Post ( BfBuscarArticulo *busc )
{
    if ( busc->idarticulo() != "" )
        g_pluginbt_buscaarticulo_emp->ticketActual() ->insertarArticulo (  busc->idarticulo(), BlFixed ( "1" ) );
    return 0;
}

