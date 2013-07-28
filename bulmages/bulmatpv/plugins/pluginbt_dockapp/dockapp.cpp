#include "dockapp.h"
#include "btbulmatpv.h"
#include "blfunctions.h"
#include <QtWidgets/QWidget>
#include <QtWidgets/QMenu>
#include <QtSvg/QSvgRenderer>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QInputDialog>
#include <QtCore/QTextStream>
#include <QtXml/QDomDocument>

#include "blmainwindow.h"
#include "blplugins.h"



QDockWidget *g_doc2;


DockApp::DockApp ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );

    /// Cambiamos el icono de las mesas.
    mui_dockapp->setIcon ( QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/table.svg" ) );

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

