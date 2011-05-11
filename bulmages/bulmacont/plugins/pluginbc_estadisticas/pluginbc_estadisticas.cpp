#include <stdio.h>
#include "pluginbc_estadisticas.h"
#include "bccompany.h"
#include "resmensualview.h"
#include "estadisticasview.h"

#include "estadisticas.xpm"

#include <qpopupmenu.h>
#include <qaction.h>
#include <qobject.h>
#include <qmessagebox.h>


PluginBc_Estadisticas:: PluginBc_Estadisticas()
{
    blDebug ( "PluginBc_Estadisticas::PluginBc_Estadisticas", 0 );
    blDebug ( "END PluginBc_Estadisticas::PluginBc_Estadisticas", 0 );
}
PluginBc_Estadisticas::~PluginBc_Estadisticas()
{
    blDebug ( "PluginBc_Estadisticas::~PluginBc_Estadisticas", 0 );
    blDebug ( "END PluginBc_Estadisticas::~PluginBc_Estadisticas", 0 );
}


void PluginBc_Estadisticas::SegCuentas()
{
// Aprovechamos para empezar a trabajar con estadisticas a ver que tal van.
    resmensualview * rmen = new resmensualview ( empresaactual, 0, 0 );
    rmen->inicializa();
    rmen->exec();
    delete rmen;
}// end SegCuentas

void PluginBc_Estadisticas::BalanceGrafico()
{
    // Aprovechamos para empezar a trabajar con estadisticas a ver que tal van.
    estadisticasview * est = new estadisticasview ( 0, 0 );
    est->inicializa ( dbConnection );
    est->exec();
    delete est;
}// end BalanceGrafico

void PluginBc_Estadisticas::inicializa ( Bulmages01 *bges )
{

    QPixmap * estadisticasIcon = new QPixmap ( estadisticas );

    empresaactual = & ( bges->empresaactual );
    dbConnection = empresaactual->bdempresa();
    //El menu de empresa
    QPopupMenu *pPluginMenu = new QPopupMenu();
    pPluginMenu->setCheckable ( true );
    bges->menuBar() ->insertItem ( "&Estadisticas", pPluginMenu, -1, 3 );
    QAction *planCuentas = new QAction ( "Resumen Mensual", *estadisticasIcon, "&Resumen Mensual", 0, 0 );
    planCuentas->setStatusTip ( "Resumen Mensual" );
    planCuentas->setWhatsThis ( "Resumen Mensual" );
    planCuentas->addTo ( pPluginMenu );

    connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( SegCuentas() ) );

    QAction *planCuentas1 = new QAction ( "Balance Gráfico", "&Balance Gráfico", 0, 0 );
    planCuentas1->setStatusTip ( "Balance Gráfico" );
    planCuentas1->setWhatsThis ( "Balance Gráfico" );
    planCuentas1->addTo ( pPluginMenu );

    connect ( planCuentas1, SIGNAL ( activated() ), this, SLOT ( BalanceGrafico() ) );
}// end inicializa


int entryPoint ( Bulmages01 *bges )
{
    fprintf ( stderr, "Estoy dentro del plugin\n" );
    PluginBc_Estadisticas *plug = new PluginBc_Estadisticas( );
    plug->inicializa ( bges );
    bges->setCaption ( "Prueba de plugin." );

}


