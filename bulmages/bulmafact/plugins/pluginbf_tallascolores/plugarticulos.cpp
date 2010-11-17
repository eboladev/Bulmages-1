#include "plugarticulos.h"
#include "listcoloresview.h"
#include "listtallasview.h"



///
/**
**/
plugin_tc_articulos::plugin_tc_articulos()
{
    blDebug ( "plugin_tc_articulos::plugin_tc_articulos", 0 );
    blDebug ( "END plugin_tc_articulos::plugin_tc_articulos", 0 );
}


///
/**
**/

plugin_tc_articulos::~plugin_tc_articulos()
{
    blDebug ( "plugin_tc_articulos::~plugin_tc_articulos", 0 );
    blDebug ( "END plugin_tc_articulos::~plugin_tc_articulos", 0 );
}


///
/**
**/

void plugin_tc_articulos::tallas()
{
    blDebug ( "plugin_tc_articulos::tallas", 0 );
    ListTallasView *tallas = new ListTallasView ( m_bulmafact->company(), 0 );
    m_bulmafact->company()->pWorkspace()->addSubWindow ( tallas );
    tallas->show();
    blDebug ( "END plugin_tc_articulos::tallas", 0 );
}


///
/**
**/

void plugin_tc_articulos::colores()
{
    blDebug ( "plugin_tc_articulos::colores", 0 );
    ListColoresView *colores = new ListColoresView ( m_bulmafact->company(), 0 );
    m_bulmafact->company()->pWorkspace()->addSubWindow ( colores );
    colores->show();
    blDebug ( "END plugin_tc_articulos::colores", 0 );
}


///
/**
\param bges
**/

void plugin_tc_articulos::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "plugin_tc_articulos::inicializa", 0 );

    m_bulmafact = bges;
    QMenu *pPluginMenuTallasColores;
    /// Miramos si existe un menu Herramientas
    pPluginMenuTallasColores = bges->menuBar() ->findChild<QMenu *> ( "Tallas y colores" );

    /// Creamos el men&uacute;.
    if ( !pPluginMenuTallasColores ) {
        pPluginMenuTallasColores = new QMenu ( _ ( "&Tallas y colores" ), bges->menuBar() );
        pPluginMenuTallasColores->setObjectName ( QString::fromUtf8 ( "Tallas y colores" ) );
    } // end if
    /// Creamos el men&uacute;.

    QAction *acciontallas = new QAction ( _ ( "&Tallas" ), 0 );
    acciontallas->setStatusTip ( _ ( "Tallas" ) );
    acciontallas->setWhatsThis ( _ ( "Tallas" ) );
    connect ( acciontallas, SIGNAL ( activated() ), this, SLOT ( tallas() ) );
    pPluginMenuTallasColores->addAction ( acciontallas );

    QAction *accioncolores = new QAction ( _ ( "&Colores" ), 0 );
    accioncolores->setStatusTip ( _ ( "Colores" ) );
    accioncolores->setWhatsThis ( _ ( "Colores" ) );
    connect ( accioncolores, SIGNAL ( activated() ), this, SLOT ( colores() ) );
    pPluginMenuTallasColores->addAction ( accioncolores );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar() ->insertMenu ( bges->menuVentana->menuAction(), pPluginMenuTallasColores );
    blDebug ( "END plugin_tc_articulos::inicializa", 0 );
}


