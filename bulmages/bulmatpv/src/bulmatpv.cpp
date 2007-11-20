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
#include <QMainWindow>
#include <QProgressBar>

#include "qworkspace2.h"
#include "bulmatpv.h"
#include "listventanas.h"
#include "funcaux.h"
#include "aboutview.h"
#include "busquedaperiodo.h"


/** Prepara la pantalla principal para que tenga todos los componentes.
    Crea el workspace y lo inicializa.
    Crea la instancia de company y la inicializa y almacena esta en el puntero m_company.
    Crea la lista de ventanas.
*/
/**
\param bd
**/
BulmaTPV::BulmaTPV(QString bd) : QMainWindow() {
    _depura("BulmaTPV::BulmaTPV", 0);
    setupUi(this);
    setUpdatesEnabled(TRUE);
    pWorkspace = new QWorkspace2(this);
    pWorkspace->setScrollBarsEnabled(TRUE);
    QFrame *m_frame1 = new QFrame();
    QProgressBar *m_pb = new QProgressBar();
    m_pb->setMaximum(100);
    m_pb->setMinimum(0);
    m_pb->setValue(0);
    /// Hacemos que el ProgressBar est&eacute; invisible hasta que se seleccione una empresa.
    m_pb->setVisible(FALSE);
    setCentralWidget(m_frame1);
    /// Creamos un VerticalLayout donde metemos el contenido central del QMainWindow.
    QVBoxLayout *vboxlayout = new QVBoxLayout(this->centralWidget());
    vboxlayout->setSpacing(0);
    vboxlayout->setMargin(0);
    vboxlayout->addWidget(pWorkspace);
    vboxlayout->addWidget(m_pb);


    m_empresaTPV = new EmpresaTPV(this);
    m_empresaTPV->setProgressBar(m_pb);
    m_empresaTPV->init(bd, "BulmaFact");
    m_empresaTPV->setWorkspace(pWorkspace);


    connect(pWorkspace, SIGNAL(windowActivated(QWidget *)), this, SLOT(informaindexador(QWidget *)));


//    m_company->setListVentanas(m_list);

    m_pb->setVisible(FALSE);
    statusBar()->showMessage(bd, 2000);
    setWindowTitle(bd);
    _depura("END BulmaTPV::BulmaTPV", 0);
}


/** Hacemos la creacion de las ventanas principales para que enten en plugins
*/
/**
\param splashScr
**/
void BulmaTPV::createMainWindows(Splash *splashScr) {
    _depura("BulmaTPV::createMainWindows", 0);
    m_empresaTPV->createMainWindows(splashScr);
    _depura("END BulmaTPV::createMainWindows", 0);
}


/** Libera memoria destruyendo todos los objetos creados.
*/
/**
**/
BulmaTPV::~BulmaTPV() {
    _depura("BulmaTPV::~BulmaTPV", 0);
    delete pWorkspace;
    /// En MS-Windows no termina bien la ejecucion del programa y por eso
    /// agregamos esta salida rapida.
#ifdef WINDOWS

    exit(0);
#endif

    _depura("END BulmaTPV::~BulmaTPV", 0);
}



/** Intercambia entre el modo ventana completa y el modo ventana normal
*/
/**
**/
void BulmaTPV::s_ventanaCompleta() {
    _depura("BulmaTPV::s_ventanaCompleta", 0);
    if (isFullScreen()) {
        showNormal();
    } else {
        showFullScreen();
    } // end if
    _depura("END BulmaTPV::s_ventanaCompleta", 0);
}


/** Metodo que responde a la pulsacion de About en el menu.
    Inicializa la ventana de About y la muestra.
*/
/**
**/
void BulmaTPV::s_About() {
    _depura("BulmaTPV::s_About", 0);
    AboutView about;
    about.exec();
    _depura("END BulmaTPV::s_About", 0);
}


/** Evento de cerrar la ventana principal.
    Libera parte de la memoria reservada. Ya que sabemos que se va a cerrar el programa.
*/
/**
**/
void BulmaTPV::closeEvent(QCloseEvent *) {
    _depura("BulmaTPV::closeEvent", 0);
//    delete m_company;
//    m_company = NULL;
#ifdef WINDOWS

    exit(0);
#endif

    _depura("END BulmaTPV::closeEvent", 0);
}


///
/**
\return
**/
QWorkspace2 * BulmaTPV::workspace() {
    _depura("BulmaTPV::workspace", 0);
    _depura("END BulmaTPV::workspace", 0);
    return pWorkspace;
}


///
/**
\return
**/
/*
Company * BulmaTPV::getcompany() {
    _depura("BulmaTPV::getcompany", 0);
    _depura("END BulmaTPV::getcompany", 0);
    return m_company;
}
*/
