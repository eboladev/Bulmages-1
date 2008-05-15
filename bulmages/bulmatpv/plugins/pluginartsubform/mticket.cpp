
#include <QLabel>
#include "mticket.h"
#include "dbrecord.h"
#include "bulmatpv.h"
#include "subform2bt.h"
#include <QTextBrowser>

#include "articulolisttpv.h"

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
void myplugin::elslot ( QTableWidgetItem * )
{
    _depura ( "myplugin::elslot", 0 );
    QString idarticulo =  m_lan->DBvalue ( "idarticulo" );
    m_tpv->empresaTPV() ->ticketActual() ->insertarArticulo ( idarticulo, Fixed ( "1" ) );
    _depura ( "END myplugin::elslot", 0 );
}


///
/**
\param bges
**/
void myplugin::inicializa ( BulmaTPV *tpv )
{
    _depura ( "myplugin::inicializa", 0 );
    m_tpv = tpv;

    ArticuloList1 *art = new ArticuloList1((Company *) tpv->empresaTPV());
    tpv->workspace() ->addWindow ( art );
    art->showFullScreen();

/*
    m_lan = new SubForm2Bt ( NULL );
    m_lan->setEmpresaBase ( tpv->empresaTPV() );
    m_lan->setDBTableName ( "articulo" );
    m_lan->setDBCampoId ( "idarticulo" );
    m_lan->addSHeader ( "idarticulo", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, QApplication::translate ( "ticketbasico", "ID articulo" ) );
    m_lan->addSHeader ( "codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, QApplication::translate ( "ticketbasico", "Codigo completo del articulo" ) );
    m_lan->addSHeader ( "nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, QApplication::translate ( "ticketbasico", "Nombre del articulo" ) );
    m_lan->addSHeader ( "abrevarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, QApplication::translate ( "ticketbasico", "Descripcion abreviada del articulo" ) );
    m_lan->addSHeader ( "obserarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, QApplication::translate ( "ticketbasico", "Observaciones sobre el articulo" ) );
    m_lan->addSHeader ( "desctipo_articulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, QApplication::translate ( "ticketbasico", "Descripcion del tipo de articulo" ) );
    m_lan->addSHeader ( "desctipo_iva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, QApplication::translate ( "ticketbasico", "Descripcion tipo de I.V.A." ) );
    m_lan->addSHeader ( "pvparticulo", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, QApplication::translate ( "ticketbasico", "P.V.P. articulo" ) );
    m_lan->addSHeader ( "stockarticulo", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, QApplication::translate ( "ticketbasico", "Disponible en stock" ) );
    m_lan->setinsercion ( FALSE );
    m_lan->setDelete ( FALSE );
    m_lan->setSortingEnabled ( TRUE );
    tpv->workspace() ->addWindow ( m_lan );
    m_lan->showFullScreen();
    m_lan->cargar ( "SELECT * FROM articulo" );

    connect ( m_lan, SIGNAL ( itemDoubleClicked ( QTableWidgetItem * ) ), this, SLOT ( elslot ( QTableWidgetItem * ) ) );
*/
    _depura ( "END myplugin::inicializa", 0 );
}
