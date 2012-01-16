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

#include <stdio.h>

#include "pluginbf_seriesfactura.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "bfseriefacturaview.h"


BfBulmaFact *g_pluginbf_seriesfactura = NULL;


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_seriesfactura", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_pluginbf_seriesfactura = bges;

    if ( bges->company()->hasTablePrivilege ( "serie_factura", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->menuMaestro;
        pPluginMenu->addSeparator();
        
        BlAction *accionA = new BlAction ( _ ( "&Series de factura" ), 0 );
        accionA->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/client-invoice-series.png" ) ));
        accionA->setStatusTip ( _ ( "Series de factura" ) );
        accionA->setWhatsThis ( _ ( "Series de factura" ) );
        accionA->setObjectName("mui_actionSeriesFactura");
        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );

    } // end if
    

    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, "PluginBf_SeriesFactura" );

    if (accion->objectName() == "mui_actionSeriesFactura") {
        BfSerieFacturaView * bud = new BfSerieFacturaView ( g_pluginbf_seriesfactura->company(), NULL );
        g_pluginbf_seriesfactura->company() ->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if
    
    return 0;
}


/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewSerieFacturaView ( BfCompany *v )
{
    BfSerieFacturaView *h = new BfSerieFacturaView ( v, 0 );
    g_plugParams = h;
    return 1;
}
