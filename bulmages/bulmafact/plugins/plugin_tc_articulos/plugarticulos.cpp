#include "plugarticulos.h"
#include "listcoloresview.h"
#include "listtallasview.h"



///
/**
**/
plugin_tc_articulos::plugin_tc_articulos()
{
    _depura ( "plugin_tc_articulos::plugin_tc_articulos", 0 );
    _depura ( "END plugin_tc_articulos::plugin_tc_articulos", 0 );
}


///
/**
**/

plugin_tc_articulos::~plugin_tc_articulos()
{
    _depura ( "plugin_tc_articulos::~plugin_tc_articulos", 0 );
    _depura ( "END plugin_tc_articulos::~plugin_tc_articulos", 0 );
}


///
/**
**/

void plugin_tc_articulos::tallas()
{
    _depura ( "plugin_tc_articulos::tallas", 0 );
    ListTallasView *tallas = new ListTallasView(m_bulmafact->getcompany(), 0);
    m_bulmafact->getcompany()->pWorkspace()->addWindow ( tallas );
    tallas->show();
    _depura ( "END plugin_tc_articulos::tallas", 0 );
}


///
/**
**/

void plugin_tc_articulos::colores()
{
    _depura ( "plugin_tc_articulos::colores", 0 );
    ListColoresView *colores = new ListColoresView(m_bulmafact->getcompany(), 0);
    m_bulmafact->getcompany()->pWorkspace()->addWindow ( colores );
    colores->show();
    _depura ( "END plugin_tc_articulos::colores", 0 );
}


///
/**
\param bges
**/

void plugin_tc_articulos::inicializa ( Bulmafact *bges )
{
    _depura ( "plugin_tc_articulos::inicializa", 0 );
    m_bulmafact = bges;
    QMenu *pPluginMenuTallasColores;
    /// Miramos si existe un menu Herramientas
    pPluginMenuTallasColores = bges->menuBar() ->findChild<QMenu *> ( "Tallas y colores" );

    /// Creamos el men&uacute;.
    if ( !pPluginMenuTallasColores ) {
        pPluginMenuTallasColores = new QMenu ( "&Tallas y colores", bges->menuBar() );
        pPluginMenuTallasColores->setObjectName ( QString::fromUtf8 ( "Tallas y colores" ) );
    } // end if
    /// Creamos el men&uacute;.

    QAction *acciontallas = new QAction ( "&Tallas", 0 );
    acciontallas->setStatusTip ( "Tallas" );
    acciontallas->setWhatsThis ( "Tallas" );
    connect ( acciontallas, SIGNAL ( activated() ), this, SLOT ( tallas() ) );
    pPluginMenuTallasColores->addAction ( acciontallas );

    QAction *accioncolores = new QAction ( "&Colores", 0 );
    accioncolores->setStatusTip ( "Colores" );
    accioncolores->setWhatsThis ( "Colores" );
    connect ( accioncolores, SIGNAL ( activated() ), this, SLOT ( colores() ) );
    pPluginMenuTallasColores->addAction ( accioncolores );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar() ->insertMenu ( bges->menuVentana->menuAction(), pPluginMenuTallasColores );
    _depura ( "END plugin_tc_articulos::inicializa", 0 );
}




