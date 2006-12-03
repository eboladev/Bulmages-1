/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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
#include "bulmafact.h"
#include "listventanas.h"
#include "funcaux.h"
#include "aboutview.h"
#include "busquedaperiodo.h"


/** Prepara la pantalla principal para que tenga todos los componentes.
    Crea el workspace y lo inicializa.
    Crea la instancia de company y la inicializa y almacena esta en el puntero m_company.
    Crea la lista de ventanas.
*/
bulmafact::bulmafact(QString bd) : QMainWindow() {
    _depura("bulmafact::bulmafact", 0);
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

    showMaximized();
    m_company = new company();
    m_company->setProgressBar(m_pb);
    m_company->init(bd);
    m_company->setWorkspace(pWorkspace);

    connect(pWorkspace, SIGNAL(windowActivated(QWidget *)), this, SLOT(informaindexador(QWidget *)));

    /// Aqui creamos la ventana dock para meter las distintas ventanas.
    m_list = new listventanas(0);
    m_list->setVisible(FALSE);
    /// Iniciamos el listventanas con el workspace para que pueda operar con el
    m_list->setWorkspace(pWorkspace);

    addDockWidget(Qt::LeftDockWidgetArea, m_list);

    m_company->setListVentanas(m_list);
    m_company->createMainWindows();

    m_list->setVisible(TRUE);
    m_pb->setVisible(FALSE);
    statusBar()->showMessage(tr("Listo"), 2000);

    _depura("END bulmafact::bulmafact", 0);
}


/** Libera memoria destruyendo todos los objetos creados.
*/
bulmafact::~bulmafact() {
    _depura("bulmafact::~bulmafact", 0);
    delete pWorkspace;
    /// En MS-Windows no termina bien la ejecucion del programa y por eso
    /// agregamos esta salida rapida.
#ifdef WINDOWS

    exit(0);
#endif

    _depura("END bulmafact::~bulmafact", 0);
}


/** La facturacion automatica es un proceso que no se ha implementado todavia.
*/
void bulmafact::emitirfactura() {
    _depura("bulmafact::emitirfactura", 0);
    QMessageBox::information(this,
                             tr("BulmaFact"),
                             tr("Pulse aceptar para emitir un monton de facturas"));
    _depura("END bulmafact::emitirfactura", 0);
}


/** El metodo de recepcion de factura ya no tiene sentido con la
    factura electronica.
*/
/// \TODO: Destruir este metodo.
void bulmafact::recibirfactura() {
    _depura("bulmafact::recibirfactura", 0);
    QMessageBox::critical(this,
                          tr("BulmaFact"),
                          tr("Pulse aceptar para recibir(destruir) un monton de facturas"));
    _depura("END bulmafact::recibirfactura", 0);
}

/** Este metodo ya no se usa puesto que tenemos una ventana de About */
/// \TODO destruir este metodo.
void bulmafact::about() {
    QMessageBox::about(this,
                       tr("Qt Application Example"),
                       tr("This example demonstrates simple use of "
                          "QMainWindow,\nQMenuBar and QToolBar."));
}

/** Este metodo ya no se usa */
/// \TODO: Destruir este metodo.
void bulmafact::aboutQt() {
    QMessageBox::aboutQt(this, tr("Qt Application Example"));
}


/// \TODO: Revisar este metodo.
void bulmafact::s_FPago() {
    m_company->s_FPago();
}

/** Intercambia entre el modo ventana completa y el modo ventana normal
*/
void bulmafact::s_ventanaCompleta() {
    if (isFullScreen()) {
        showNormal();
    } else {
        showFullScreen();
    } // end if
}

/** Metodo que responde a la pulsacion de About en el menu.
    Inicializa la ventana de About y la muestra.
*/
void bulmafact::s_About() {
    _depura("bulmafact::s_About", 0);
    AboutView about;
    about.exec();
    _depura("END bulmafact::s_About", 0);
}


/** Evento de cerrar la ventana principal.
    Libera parte de la memoria reservada. Ya que sabemos que se va a cerrar el programa.
*/
void bulmafact::closeEvent(QCloseEvent *) {
    _depura("bulmafact::closeEvent", 0);
    delete m_company;
    delete m_list;
#ifdef WINDOWS
    exit(0);
#endif
    _depura("END bulmafact::closeEvent", 0);
}


/** Captura el evento de cambio de ventana en el workSpace y actua sobre el
    listado de ventanas para que seleccione la ventana adecuada.
*/
void bulmafact::informaindexador(QWidget *w) {
    _depura("bulmafact::informaindexador", 0);
    /// No existe una ventana que activar.
    if (w == NULL) {
        m_company->deSeleccionaWindow();
        return;
    } // end if
    m_company->seleccionaWindow(w->windowTitle(), w);
    
    QString texto = "Window activated. " + w->windowTitle() + "\n";
    printf(texto.toAscii().constData());
    
    _depura("END bulmafact::informaindexador", 0);
}

