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

#include "pluginbt_totalivaincluido.h"
#include "btcompany.h"
#include "total.h"
#include "blplugins.h"
#include "btticket.h"
#include "blapplication.h"
#include "bldockwidget.h"


/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;
Total *g_tot = NULL;
BlDockWidget *g_doc1 = NULL;


///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "plugintotalivainc", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    /// Vamos a probar con un docwindow.
    g_doc1 = new BlDockWidget ( _ ( "Total" ), tpv, "plugintotaldock" );
    g_doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );
    g_doc1->setGeometry ( 100, 100, 100, 500 );
    g_doc1->resize ( 330, 400 );
    tpv->addDockWidget ( Qt::LeftDockWidgetArea, g_doc1 );
    g_doc1->loadConfig();
    g_doc1->show();

    
    return 0;
}

int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{
    BL_FUNC_DEBUG

    g_tot = new Total ( etpv, g_doc1 );
    g_doc1->setWidget ( g_tot );
    

    return 0;
}


///
/**
\return
**/
int exitPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG
    delete g_doc1;
    
    return 0;
}

int BtTicket_pintar ( BtTicket *tick )
{
    BL_FUNC_DEBUG

    base basesimp;
    base basesimpreqeq;
    BlDbRecord *linea;
    int precision = 0;
    int maxprecision = 0;
    
    /// Impresion de los contenidos.
    QString l;
    BlFixed total ( "0.00" );
    BlFixed descuentolinea ( "0.00" );
    for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
        if (linea = tick->listaLineas() ->at ( i ) ) {
            BlFixed cant ( linea->dbValue ( "cantlalbaran" ) );
	    cant.setPrecision(((BtCompany *)tick->mainCompany())->decimalesCantidad());
            BlFixed pvpund ( linea->dbValue ( "pvpivainclalbaran" ) );
	    pvpund.setPrecision(((BtCompany *)tick->mainCompany())->decimalesPrecio());
            BlFixed cantpvp = cant * pvpund;
            precision = cant.precision() > pvpund.precision() ? cant.precision() : pvpund.precision();
            maxprecision = precision > maxprecision ? precision : maxprecision;
            total = total + cantpvp;
        } // end if
    } // end for

    /// Pintamos el total
    if ( g_tot ) {
        maxprecision = 2;
        g_tot->mui_total->setText ( total.toQString('0', maxprecision) );
    } // end if
    
    

    return 0;

}


