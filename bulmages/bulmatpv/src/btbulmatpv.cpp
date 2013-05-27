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

#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QMenuBar>
#include <QtCore/QFile>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QMessageBox>
#include <QtPrintSupport/QPrinter>
#include <QtWidgets/QApplication>
#include <QtCore/QTextStream>
#include <QtGui/QPainter>
#include <QtWidgets/QProgressBar>

#include "blworkspace.h"
#include "btbulmatpv.h"
#include "blwindowlistdock.h"
#include "btaboutview.h"
#include "blperiodicitycombobox.h"
#include "blmainwindow.h"
#include "blplugins.h"

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
    BL_FUNC_DEBUG
    
    setupUi ( this );
    setUpdatesEnabled ( true );
    pWorkspace = new BlWorkspace ( this );
    
    pWorkspace->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    pWorkspace->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QProgressBar *m_pb = new QProgressBar();
    m_pb->setMaximum ( 100 );
    m_pb->setMinimum ( 0 );
    m_pb->setValue ( 0 );
    /// Hacemos que el ProgressBar est&eacute; invisible hasta que se seleccione una empresa.
    m_pb->setVisible ( false );


    m_stackedWidget = new QStackedWidget(this);
    BlMainWindow::setCentralWidget(m_stackedWidget);


    /// Creamos un VerticalLayout donde metemos el contenido central del QMainWindow.
    QVBoxLayout *vboxlayout = new QVBoxLayout ( this->centralWidget() );
    vboxlayout->setSpacing ( 0 );
    vboxlayout->setMargin ( 0 );
    vboxlayout->addWidget ( pWorkspace );
    vboxlayout->addWidget ( m_pb );

    m_company = new BtCompany ( this );
    m_company->setProgressBar ( m_pb );
    m_company->init ( bd, "BulmaFact" );
    m_company->setWorkspace ( pWorkspace );

    setCorner ( Qt::TopLeftCorner, Qt::TopDockWidgetArea );
    setCorner ( Qt::TopRightCorner, Qt::RightDockWidgetArea );
    setCorner ( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );
    setCorner ( Qt::BottomRightCorner, Qt::RightDockWidgetArea );

// ============== OJO El listventanas no se utiliza pero lo pongo para poder usar componentes de bulmafact.
    /// Aqui creamos la ventana dock para meter las distintas ventanas.
    BlWindowListDock *list = new BlWindowListDock ( 0 );
    list->setVisible ( false );
    /// Iniciamos el listventanas con el workspace para que pueda operar con el.
    list->setWorkspace ( pWorkspace );

    m_company->setListVentanas ( list );

    m_pb->setVisible ( false );
    statusBar() ->showMessage ( bd, 2000 );
    setWindowTitle ( bd );
}

/** Hacemos la creacion de las ventanas principales para que enten en plugins
*/
/**
\param splashScr
**/
void BtBulmaTPV::createMainWindows ( BlSplashScreen *splashScr )
{
    BL_FUNC_DEBUG
    m_company->createMainWindows ( splashScr );
    
}

/** Libera memoria destruyendo todos los objetos creados.
*/
/**
**/
BtBulmaTPV::~BtBulmaTPV()
{
    BL_FUNC_DEBUG
    delete pWorkspace;
	
/*
    /// En MS-Windows no termina bien la ejecucion del programa y por eso
    /// agregamos esta salida rapida.
#ifdef Q_OS_WIN32

    exit ( 0 );
#endif
*/
    
}

/** Intercambia entre el modo ventana completa y el modo ventana normal
*/
/**
**/
void BtBulmaTPV::s_ventanaCompleta()
{
    BL_FUNC_DEBUG

    if ( isFullScreen() ) {
        showNormal();
    } else {
        showFullScreen();
    } // end if

    
}


///
/**
**/
void BtBulmaTPV::on_actionVentana_Completa_triggered()
{
    BL_FUNC_DEBUG
    s_ventanaCompleta();
    
}

/** Metodo que responde a la pulsacion de About en el menu.
    Inicializa la ventana de About y la muestra.
*/
/**
**/
void BtBulmaTPV::s_About()
{
    BL_FUNC_DEBUG
    
    BtAboutView about;
    about.exec();
    
    
}


/** Evento de cerrar la ventana principal.
    Libera parte de la memoria reservada. Ya que sabemos que se va a cerrar el programa.
*/
/**
**/
void BtBulmaTPV::closeEvent ( QCloseEvent *event )
{
    BL_FUNC_DEBUG


    /// Antes de salir hacemos un mensaje de advertencia.
    if ( g_confpr->value( CONF_ASK_BEFORE_EXIT ) == "true" ) {
	 QMessageBox msgBox;
	 msgBox.setText(_("Seguro que desea abandonar el programa "));
	 msgBox.setInformativeText(_("Se perderan los cambios no guardados"));
	 msgBox.setStandardButtons( QMessageBox::Close | QMessageBox::Cancel);
	 msgBox.setDefaultButton(QMessageBox::Cancel);
	 int ret = msgBox.exec();

	 if (ret == QMessageBox::Cancel) {
	   event->ignore();
	   
	   return;
	 }
    } // END IF

    g_plugins->run ( "BtBulmaTPV_closeEvent", this );

    /// Borramos el BfCompany
    delete m_company;
    m_company = NULL;
}


///
/**
\return
**/
BlWorkspace * BtBulmaTPV::workspace()
{
    BL_FUNC_DEBUG
    
    return pWorkspace;
}


void BtBulmaTPV::keyReleaseEvent ( QKeyEvent * e )
{
    BL_FUNC_DEBUG
    m_company->keyPressEvent ( e );
    
}


BtCompany *BtBulmaTPV::company()
{
    return m_company;
}


void BtBulmaTPV::setCentralBtWidget(QWidget *wid) {

  m_stackedWidget->addWidget(wid);
  m_stackedWidget->setCurrentWidget(wid);
}


QWidget *BtBulmaTPV::centralBtWidget() {
    return m_stackedWidget->currentWidget();
}

