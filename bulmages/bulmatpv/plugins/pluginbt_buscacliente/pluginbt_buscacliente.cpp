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

#include "pluginbt_buscacliente.h"
#include "blfunctions.h"
#include "btcompany.h"
#include "blplugins.h"
#include "btticket.h"
#include "blapplication.h"
#include "bldockwidget.h"
#include "blformlist.h"

/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;
BlSearchWidget *g_busc;
BlDockWidget *g_doc1;
BtCompany * g_pluginbt_buscacliente_emp;


///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbt_buscacliente", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    g_pluginbt_buscacliente_emp = tpv->company();

    /// Vamos a probar con un docwindow.
    g_doc1 = new BlDockWidget ( _ ( "Cliente" ), tpv, "buscaclientedock" );
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
    g_busc = new BlSearchWidget ( 0 );
    g_busc->setMainCompany ( etpv );
    /// Establecemos los parametros de busqueda del Cliente
    g_busc->setLabel ( _ ( "Cliente:" ) );
    g_busc->setTableName ( "cliente" );
    g_busc->m_valores["cifcliente"] = "";
    g_busc->m_valores["nomcliente"] = "";
    g_doc1->setWidget ( g_busc );

    return 0;
}

int Busqueda_on_m_inputBusqueda_editingFinished_Post ( BlSearchWidget *busc )
{
    if ( busc->id() != "" && busc == g_busc ) {
        g_pluginbt_buscacliente_emp->ticketActual() ->setDbValue ( "idcliente", busc->id() );
        g_pluginbt_buscacliente_emp->ticketActual() ->pintar();
        g_pluginbt_buscacliente_emp->setValorBtInput ( "" );
        g_pluginbt_buscacliente_emp->pulsaTecla ( 0, "" );
    } // end if
    return 0;
}




