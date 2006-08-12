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

#include "qworkspace2.h"
#include "bulmafact.h"
#include "listventanas.h"
#include "funcaux.h"
#include "aboutview.h"
#include "busquedaperiodo.h"


bulmafact::bulmafact(QString bd) : QMainWindow() {
    _depura("bulmafact::bulmafact\n", 0);
    setupUi(this);
    m_company = new company();
    m_company->init(bd);

    pWorkspace = new QWorkspace2(this);

    pWorkspace->setScrollBarsEnabled(TRUE);
    setCentralWidget(pWorkspace);
    m_company->setWorkspace(pWorkspace);

    /// Aqui creamos la ventana dock para meter las distintas ventanas.
    m_list = new listventanas(0);

    /// Iniciamos el listventanas con el workspace para que pueda operar con el
    m_list->setWorkspace(pWorkspace);

    addDockWidget(Qt::LeftDockWidgetArea, m_list);

    m_company->setListVentanas(m_list);
    m_company->createMainWindows();

    showMaximized();

    statusBar()->showMessage(tr("Listo"), 2000);
    _depura("END bulmafact::bulmafact\n", 0);
}


bulmafact::~bulmafact() {
    _depura("Destructor de BulmaFact", 0);
    delete pWorkspace;
    /// En MS-Windows no termina bien la ejecucion del programa y por eso
    /// agregamos esta salida rapida.
#ifdef WINDOWS
    exit(0);
#endif
    _depura("End Destructor de BulmaFact", 0);
}


void bulmafact::emitirfactura() {
    _depura("emitir factura activado", 0);
    QMessageBox::information(this,
                             tr("BulmaFact"),
                             tr("Pulse aceptar para emitir un monton de facturas"));
}


void bulmafact::recibirfactura() {
    _depura("recibir factura activado", 0);
    QMessageBox::critical(this,
                          tr("BulmaFact"),
                          tr("Pulse aceptar para recibir(destruir) un monton de facturas"));
}


void bulmafact::about() {
    QMessageBox::about(this,
                       tr("Qt Application Example"),
                       tr("This example demonstrates simple use of "
                          "QMainWindow,\nQMenuBar and QToolBar."));
}


void bulmafact::aboutQt() {
    QMessageBox::aboutQt(this, tr("Qt Application Example"));
}


void bulmafact::s_FPago() {
    m_company->s_FPago();
}


void bulmafact::s_ventanaCompleta() {
    if (isFullScreen()) {
        showNormal();
    } else {
        showFullScreen();
    } // end if
}


void bulmafact::s_About() {
    AboutView about;
    about.exec();
}


void bulmafact::closeEvent(QCloseEvent *) {
    _depura("bulmafact::closeEvent", 0);
    delete m_company;
    delete m_list;
#ifdef WINDOWS
    exit(0);
#endif
    _depura("END bulmafact::closeEvent", 0);

}

