/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
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

#ifndef WIN32
#include <unistd.h>
#endif

#include <QPixmap>
#include <QEvent>

#include "bulmacont.h"
#include "configuracion.h"
#include "aboutview.h"
#include "actualizacionesview.h"

Bulmacont::Bulmacont(QWidget *parent, Qt::WFlags f, QString DB)
        : QMainWindow(parent, f) {
    _depura("Bulmacont::Bulmacont", 0);
    setupUi(this);

    m_pWorkspace = new QWorkspace2(this);
    m_pWorkspace->setScrollBarsEnabled(TRUE);

    setCentralWidget(m_pWorkspace);
    show();

    m_empresaactual = new empresa();
    m_empresaactual->setWorkspace(m_pWorkspace);
    m_empresaactual->init(DB);

    /// Aqu&iacute; creamos la ventana dock para meter las distintas ventanas.
    m_list = new listventanas(0);

    /// Indicamos a listventanas cual es el workspace para que pueda operar con el
    m_list->setWorkspace(m_pWorkspace);
    addDockWidget(Qt::LeftDockWidgetArea, m_list);
    m_empresaactual->setListVentanas(m_list);
    m_empresaactual->inicializa1();

    setWindowTitle(tr("BulmaCont -- ") + DBName + " --");
    initStatusBar();
    _depura("END Bulmacont::Bulmacont", 0);
}


Bulmacont::~Bulmacont() {
    _depura("Bulmacont::~Bulmacont", 0);
    _depura("END Bulmacont::~Bulmacont", 0);
}


void Bulmacont::initStatusBar() {
    statusBar()->showMessage(tr("Listo."));
}


bool Bulmacont::eventFilter(QObject *object, QEvent *event)  {
    /// Standard event processing.
    return QWidget::eventFilter( object, event );
}


/// Slot implementation.
void Bulmacont::slotEditUndo()  {
    statusBar()->showMessage(tr("Deshaciendo la ultima accion..."));
    statusBar()->showMessage(tr("Listo."));
}


void Bulmacont::slotEditCut()  {
    statusBar()->showMessage(tr("Cortando seleccion..."));
    statusBar()->showMessage(tr("Listo."));
}


void Bulmacont::slotEditCopy()  {
    statusBar()->showMessage(tr("Copiando la seleccion al portapapeles..."));
    statusBar()->showMessage(tr("Listo."));
}


void Bulmacont::slotEditPaste()  {
    statusBar()->showMessage(tr("Insertando el contenido del portapapeles..."));
    statusBar()->showMessage(tr("Listo."));
}


void Bulmacont::slotViewStatusBar(bool toggle)  {
    statusBar()->showMessage(tr("Cambinado barra de estado..."));
    if (toggle == false) {
        statusBar()->hide();
    } else {
        statusBar()->show();
    } // end if
    statusBar()->showMessage(tr("Listo."));
}


void Bulmacont::slotViewFullScreen(bool toggle)  {
    statusBar()->showMessage(tr("Cambiando a modo de pantalla completa..."));
    if (toggle == false) {
        showNormal();
    } else {
        showFullScreen();
    } // end if
    statusBar()->showMessage(tr("Listo."));
}


void Bulmacont::slotStatusHelpMsg(const QString &text) {
    /// Change status message of whole statusbar temporary (text, msec).
    statusBar()->showMessage(text, 2000);
}


void Bulmacont::slotListadoCuentas()  {
    statusBar()->showMessage(tr("Listado de cuentas"));
}


void Bulmacont::slotAsientos()  {
    m_empresaactual->muestraasientos();
}


void Bulmacont::slotCentrosCoste() {
    m_empresaactual->ccostes();
}


void Bulmacont::slotCanales() {
    m_empresaactual->canales();
}


void Bulmacont::slotCompBalance() {
    m_empresaactual->compbalance();
}


void Bulmacont::slotPerdidas() {}


void Bulmacont::windowMenuAboutToShow()  {}


void Bulmacont::slotWindowNewWindow() {}


void Bulmacont::closeEvent(QCloseEvent *) {
    _depura("closeEvent", 0);
    delete m_empresaactual;
#ifdef WINDOWS

    exit(0);
#endif

    delete m_pWorkspace;
}


void Bulmacont::on_actionAyuda_triggered() {
    _depura("Bulmacont::on_actionAyuda_triggered", 0);
    actualizacionesview *act = new actualizacionesview(0);
    m_pWorkspace->addWindow(act);
    act->show();
    _depura("END Bulmacont::on_actionAyuda_triggered", 0);

}


void Bulmacont::on_actionAcerca_de_triggered() {
    aboutview *about = new aboutview(0);
    about->exec();
}

