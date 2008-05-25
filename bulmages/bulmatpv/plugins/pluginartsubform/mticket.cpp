
#include <QLabel>
#include "mticket.h"
#include "dbrecord.h"
#include "bulmatpv.h"
#include "subform2bt.h"
#include <QTextBrowser>



//extern QTextBrowser *g_browser;

/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
//typedef QMap<QString, Fixed> base;


///
/**
**/
myplugin::myplugin()
{
    _depura ( "myplugin::myplugin", 0 );
    _depura ( "END myplugin::myplugin", 0 );
}


///
/**
**/
myplugin::~myplugin()
{
    _depura ( "myplugin::~myplugin", 0 );
    _depura ( "END myplugin::~myplugin", 0 );
}


///
/**
**/
void myplugin::elslot ( QString item )
{
    _depura ( "myplugin::elslot", 0 );
    mensajeInfo ( "Seleccionado" );
    QString idarticulo =  m_lan->mui_list->DBvalue ( "idarticulo" );
    m_etpv->ticketActual() ->insertarArticulo ( idarticulo, Fixed ( "1" ) );
    _depura ( "END myplugin::elslot", 0 );
}


void myplugin::elslot1 ( QTableWidgetItem *item )
{
    _depura ( "myplugin::elslot", 0 );
    mensajeInfo ( "Seleccionado" );
    QString idarticulo =  m_lan->mui_list->DBvalue ( "idarticulo" );
    m_etpv->ticketActual() ->insertarArticulo ( idarticulo, Fixed ( "1" ) );
    _depura ( "END myplugin::elslot", 0 );
}

///
/**
\param bges
**/
void myplugin::inicializa ( EmpresaTPV *etpv )
{
    _depura ( "myplugin::inicializa", 0 );

    m_etpv = etpv;
    /*
        m_lan = new ArticuloList1((Company *) etpv, NULL, 0, Listado::SelectMode);

        g_main->setCentralWidget ( m_lan );
        connect ( m_lan, SIGNAL ( selected ( QString  ) ), this, SLOT ( elslot ( QString ) ) );

        connect ( m_lan->mui_list, SIGNAL ( itemDoubleClicked(QTableWidgetItem *)), SLOT ( elslot1 (QTableWidgetItem *)));
    */
    _depura ( "END myplugin::inicializa", 0 );
}
