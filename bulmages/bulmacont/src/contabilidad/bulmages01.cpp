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
#include <Q3Frame>
#include <QEvent>
#include <q3accel.h>
#include <q3dockwindow.h>

#include "bulmages01.h"
#include "configuracion.h"
#include "aboutview.h"
#include "actualizacionesview.h"
#include "log.h"


Bulmages01::Bulmages01(QWidget *parent, const char *name, Qt::WFlags f, QString DB)
        : QMainWindow(parent, name, f) {
    _depura("Bulmages01::Bulmages01", 0);
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

    setCaption(tr("BulmaCont -- ") + DBName + " --");
    initStatusBar();
    ctllog->setempresa(m_empresaactual);
    ctllog->add
    (LOG_SEG | LOG_TRA, 1, "BmgCtt001" , "El usuario ha entrado en bulmacont");
    _depura("END Bulmages01::Bulmages01", 0);
}


Bulmages01::~Bulmages01() {
    _depura("~Bulmages01", 0);
    /// En el constructor asignamos un puntero a empresa {ctllog->setempresa(&empresaactual);}
    /// Al destruir la clase empresa no podemos seguir apuntando a un objeto que ya no existe.
    /// es necesario eliminar la referencia a este objeto, por ejemplo aqu&iacute;
    /// justo antes logeo la salida del usuario (Ren&eacute;)
    ctllog->add
    (LOG_SEG | LOG_TRA, 1, "BmgDtt017", "El usuario sale de bulmacont.");
    ctllog->setempresa(NULL);
}


void Bulmages01::initStatusBar() {
    statusBar()->message(tr("Listo."));
}


bool Bulmages01::eventFilter(QObject *object, QEvent *event)  {
    /// Standard event processing.
    return QWidget::eventFilter( object, event );
}


/// Slot implementation.
void Bulmages01::slotEditUndo()  {
    statusBar()->message(tr("Reverting last action..."));
    statusBar()->message(tr("Ready."));
}


void Bulmages01::slotEditCut()  {
    statusBar()->message(tr("Cutting selection..."));
    statusBar()->message(tr("Ready."));
}


void Bulmages01::slotEditCopy()  {
    statusBar()->message(tr("Copying selection to clipboard..."));
    statusBar()->message(tr("Ready."));
}


void Bulmages01::slotEditPaste()  {
    statusBar()->message(tr("Inserting clipboard contents..."));
    statusBar()->message(tr("Ready."));
}


void Bulmages01::slotViewStatusBar(bool toggle)  {
    statusBar()->message(tr("Toggle statusbar..."));
    if (toggle == false) {
        statusBar()->hide();
    } else {
        statusBar()->show();
    } // end if
    statusBar()->message(tr("Ready."));
}


void Bulmages01::slotViewFullScreen(bool toggle)  {
    statusBar()->message(tr("Toggle full screen mode..."));
    if (toggle == false) {
        showNormal();
    } else {
        showFullScreen();
    } // end if
    statusBar()->message(tr("Ready."));
}


void Bulmages01::slotStatusHelpMsg(const QString &text) {
    /// Change status message of whole statusbar temporary (text, msec).
    statusBar()->message(text, 2000);
}


void Bulmages01::slotListadoCuentas()  {
    statusBar()->message(tr("Listado de cuentas"));
}


void Bulmages01::slotAsientos()  {
    m_empresaactual->muestraasientos();
}


void Bulmages01::slotCentrosCoste() {
    m_empresaactual->ccostes();
}


void Bulmages01::slotCanales() {
    m_empresaactual->canales();
}


void Bulmages01::slotCompBalance() {
    m_empresaactual->compbalance();
}


void Bulmages01::slotPerdidas() {}


void Bulmages01::windowMenuAboutToShow()  {}


void Bulmages01::slotWindowNewWindow() {}


void Bulmages01::closeEvent(QCloseEvent *) {
    _depura("closeEvent", 0);
    delete m_empresaactual;
#ifdef WINDOWS

    exit(0);
#endif

    delete m_pWorkspace;
}


void Bulmages01::on_actionAyuda_triggered() {
    _depura("Bulmages01::on_actionAyuda_triggered", 0);
    actualizacionesview *act = new actualizacionesview(0);
    m_pWorkspace->addWindow(act);
    act->show();
    _depura("END Bulmages01::on_actionAyuda_triggered", 0);

}


void Bulmages01::on_actionAcerca_de_triggered() {
    aboutview *about = new aboutview(0, 0);
    about->exec();
}

