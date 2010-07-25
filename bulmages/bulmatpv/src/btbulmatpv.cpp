/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#include <QImage>
#include <QPixmap>
#include <QToolButton>
#include <QMenuBar>
#include <QFile>
#include <QStatusBar>
#include <QMessageBox>
#include <QPrinter>
#include <QApplication>
#include <QTextStream>
#include <QPainter>
#include <QWorkspace>
#include <QProgressBar>

#include "blworkspace.h"
#include "btbulmatpv.h"
#include "blwindowlistdock.h"
#include "btaboutview.h"
#include "blperiodicitycombobox.h"
#include "blmainwindow.h"

/** Prepara la pantalla principal para que tenga todos los componentes.
    Crea el workspace y lo inicializa.
    Crea la instancia de company y la inicializa y almacena esta en el puntero m_company.
    Crea la lista de ventanas.
*/
/**
\param bd
**/
BtBulmaTPV::BtBulmaTPV ( QString bd ) : BlMainWindow()
{
    _depura ( "BtBulmaTPV::BtBulmaTPV", 0 );
    
    setupUi ( this );
    setUpdatesEnabled ( TRUE );
    pWorkspace = new BlWorkspace ( this );
    pWorkspace->setScrollBarsEnabled ( TRUE );
//     QFrame *m_frame1 = new QFrame();
    QProgressBar *m_pb = new QProgressBar();
    m_pb->setMaximum ( 100 );
    m_pb->setMinimum ( 0 );
    m_pb->setValue ( 0 );
    /// Hacemos que el ProgressBar est&eacute; invisible hasta que se seleccione una empresa.
    m_pb->setVisible ( FALSE );
//     setCentralWidget ( m_frame1 );


    m_stackedWidget = new QStackedWidget(this);
    BlMainWindow::setCentralWidget(m_stackedWidget);


    /// Creamos un VerticalLayout donde metemos el contenido central del QMainWindow.
    QVBoxLayout *vboxlayout = new QVBoxLayout ( this->centralWidget() );
    vboxlayout->setSpacing ( 0 );
    vboxlayout->setMargin ( 0 );
    vboxlayout->addWidget ( pWorkspace );
    vboxlayout->addWidget ( m_pb );

    m_empresaTPV = new BtCompany ( this );
    m_empresaTPV->setProgressBar ( m_pb );
    m_empresaTPV->init ( bd, "BulmaFact" );
    m_empresaTPV->setWorkspace ( pWorkspace );

    setCorner ( Qt::TopLeftCorner, Qt::TopDockWidgetArea );
    setCorner ( Qt::TopRightCorner, Qt::RightDockWidgetArea );
    setCorner ( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );
    setCorner ( Qt::BottomRightCorner, Qt::RightDockWidgetArea );

// ============== OJO El listventanas no se utiliza pero lo pongo para poder usar componentes de bulmafact.
    /// Aqui creamos la ventana dock para meter las distintas ventanas.
    BlWindowListDock *list = new BlWindowListDock ( 0 );
    list->setVisible ( FALSE );
    /// Iniciamos el listventanas con el workspace para que pueda operar con el.
    list->setWorkspace ( pWorkspace );

    m_empresaTPV->setListVentanas ( list );

    m_pb->setVisible ( FALSE );
    statusBar() ->showMessage ( bd, 2000 );
    setWindowTitle ( bd );

    _depura ( "END BtBulmaTPV::BtBulmaTPV", 0 );
}

/** Hacemos la creacion de las ventanas principales para que enten en plugins
*/
/**
\param splashScr
**/
void BtBulmaTPV::createMainWindows ( BlSplashScreen *splashScr )
{
    _depura ( "BtBulmaTPV::createMainWindows", 0 );
    m_empresaTPV->createMainWindows ( splashScr );
    _depura ( "END BtBulmaTPV::createMainWindows", 0 );
}

/** Libera memoria destruyendo todos los objetos creados.
*/
/**
**/
BtBulmaTPV::~BtBulmaTPV()
{
    _depura ( "BtBulmaTPV::~BtBulmaTPV", 0 );
    delete m_empresaTPV;
    /// En MS-Windows no termina bien la ejecucion del programa y por eso
    /// agregamos esta salida rapida.
#ifdef WINDOWS

    exit ( 0 );
#endif

    _depura ( "END BtBulmaTPV::~BtBulmaTPV", 0 );
}

/** Intercambia entre el modo ventana completa y el modo ventana normal
*/
/**
**/
void BtBulmaTPV::s_ventanaCompleta()
{
    _depura ( "BtBulmaTPV::s_ventanaCompleta", 0 );

    if ( isFullScreen() ) {
        showNormal();
    } else {
        showFullScreen();
    } // end if

    _depura ( "END BtBulmaTPV::s_ventanaCompleta", 0 );
}


///
/**
**/
void BtBulmaTPV::on_actionVentana_Completa_triggered()
{
    _depura ( "BtBulmaTPV::on_actionVentana_Completa_triggered", 0 );
    s_ventanaCompleta();
    _depura ( "END BtBulmaTPV::on_actionVentana_Completa_triggered", 0 );
}

/** Metodo que responde a la pulsacion de About en el menu.
    Inicializa la ventana de About y la muestra.
*/
/**
**/
void BtBulmaTPV::s_About()
{
    _depura ( "BtBulmaTPV::s_About", 0 );
    
    BtAboutView about;
    about.exec();
    
    _depura ( "END BtBulmaTPV::s_About", 0 );
}


/** Evento de cerrar la ventana principal.
    Libera parte de la memoria reservada. Ya que sabemos que se va a cerrar el programa.
*/
/**
**/
void BtBulmaTPV::closeEvent ( QCloseEvent * )
{
    _depura ( "BtBulmaTPV::closeEvent", 0 );
#ifdef WINDOWS
    exit ( 0 );
#endif

    _depura ( "END BtBulmaTPV::closeEvent", 0 );
}


///
/**
\return
**/
BlWorkspace * BtBulmaTPV::workspace()
{
    _depura ( "BtBulmaTPV::workspace", 0 );
    _depura ( "END BtBulmaTPV::workspace", 0 );
    return pWorkspace;
}


void BtBulmaTPV::keyReleaseEvent ( QKeyEvent * e )
{
    _depura ( "BtBulmaTPV::keyReleaseEvent", 0 );
    m_empresaTPV->keyPressEvent ( e );
    _depura ( "END BtBulmaTPV::keyReleaseEvent", 0 );
}


BtCompany *BtBulmaTPV::empresaTPV()
{
    return m_empresaTPV;
}


void BtBulmaTPV::setCentralBtWidget(QWidget *wid) {

  m_stackedWidget->addWidget(wid);
  m_stackedWidget->setCurrentWidget(wid);
}


QWidget *BtBulmaTPV::centralBtWidget() {
    return m_stackedWidget->currentWidget();
}

