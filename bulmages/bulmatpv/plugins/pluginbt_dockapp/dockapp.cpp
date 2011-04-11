#include "dockapp.h"
#include "btbulmatpv.h"
#include "blfunctions.h"
#include <QWidget>
#include <QMenu>
#include <QSvgRenderer>
#include <QFileDialog>
#include <QInputDialog>
#include <QTextStream>
#include <QDomDocument>

#include "blmainwindow.h"
#include "blplugins.h"

#ifndef Q_OS_WIN32
#include <QX11EmbedContainer>
#endif


QDockWidget *g_doc2;


DockApp::DockApp ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );

    /// Cambiamos el icono de las mesas.
    mui_dockapp->setIcon ( QIcon ( g_confpr->valor ( CONF_PROGDATA ) + "icons/table.svg" ) );

    g_doc2 = new QDockWidget ( "Docked", g_main );
    g_doc2->setFeatures ( QDockWidget::AllDockWidgetFeatures );
    g_main->addDockWidget ( Qt::TopDockWidgetArea, g_doc2 );
    g_doc2->hide();
}

DockApp::~DockApp()
{}

void DockApp::on_mui_dockapp_clicked()
{

#ifndef Q_OS_WIN32

    QX11EmbedContainer *container = new QX11EmbedContainer ( g_doc2 );
    container->embedClient ( blWindowId("").toInt() );
    g_doc2->setWidget ( container );

#endif

}

