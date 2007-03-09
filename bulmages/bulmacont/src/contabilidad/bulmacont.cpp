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
#include <QProgressBar>

#include "bulmacont.h"
#include "configuracion.h"
#include "sobreview.h"


Bulmacont::Bulmacont(QWidget *parent, Qt::WFlags f, QString DB)
        : QMainWindow(parent, f) {
    _depura("Bulmacont::Bulmacont", 0);
    setupUi(this);

    m_pWorkspace = new QWorkspace2(this);
    m_pWorkspace->setScrollBarsEnabled(TRUE);

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
    vboxlayout->addWidget(m_pWorkspace);
    vboxlayout->addWidget(m_pb);

    showNormal();

    m_empresaactual = new empresa();
    m_empresaactual->setProgressBar(m_pb);
    m_empresaactual->init(DB, "BulmaCont");
    m_empresaactual->setWorkspace(m_pWorkspace);

    connect(m_pWorkspace, SIGNAL(windowActivated(QWidget *)), this, SLOT(informaindexador(QWidget *)));

    /// Aqu&iacute; creamos la ventana dock para meter las distintas ventanas.
    m_list = new listventanas(0);
    m_list->setVisible(FALSE);

    /// Iniciamos el listventanas con el workspace para que pueda operar con el.
    m_list->setWorkspace(m_pWorkspace);

    connect(m_list, SIGNAL(cambiaEstadoVisible(bool)), this, SLOT(setActionIndexador(bool)));

    addDockWidget(Qt::LeftDockWidgetArea, m_list);

    m_empresaactual->setListVentanas(m_list);
    m_empresaactual->inicializa1();

    m_list->setVisible(TRUE);
    m_pb->setVisible(FALSE);

    initStatusBar();
    statusBar()->showMessage(tr("Listo"), 2000);

    _depura("END Bulmacont::Bulmacont", 0);
}


Bulmacont::~Bulmacont() {
    _depura("Bulmacont::~Bulmacont", 0);
    _depura("END Bulmacont::~Bulmacont", 0);
}


void Bulmacont::initStatusBar() {
    _depura("Bulmacont::initStatusBar", 0);
    statusBar()->showMessage(tr("Listo."));
    _depura("END Bulmacont::initStatusBar", 0);
}


bool Bulmacont::eventFilter(QObject *object, QEvent *event) {
    _depura("Bulmacont::eventFilter", 0);
    /// Standard event processing.
    return QWidget::eventFilter(object, event);
}


void Bulmacont::slotEditUndo()  {
    _depura("Bulmacont::slotEditUndo", 0);
    statusBar()->showMessage(tr("Deshaciendo la ultima accion..."));
    statusBar()->showMessage(tr("Listo."));
    _depura("END Bulmacont::slotEditUndo", 0);
}


void Bulmacont::slotEditCut()  {
    _depura("Bulmacont::slotEditCut", 0);
    statusBar()->showMessage(tr("Cortando seleccion..."));
    statusBar()->showMessage(tr("Listo."));
    _depura("EMD Bulmacont::slotEditCut", 0);
}


void Bulmacont::slotEditCopy()  {
    _depura("Bulmacont::slotEditCopy", 0);
    statusBar()->showMessage(tr("Copiando la seleccion al portapapeles..."));
    statusBar()->showMessage(tr("Listo."));
    _depura("END Bulmacont::slotEditCopy", 0);
}


void Bulmacont::slotEditPaste() {
    _depura("Bulmacont::slotEditPaste", 0);
    statusBar()->showMessage(tr("Insertando el contenido del portapapeles..."));
    statusBar()->showMessage(tr("Listo."));
    _depura("END Bulmacont::slotEditPaste", 0);
}


void Bulmacont::slotViewStatusBar(bool toggle)  {
    _depura("Bulmacont::slotViewStatusBar", 0);
    statusBar()->showMessage(tr("Cambinado barra de estado..."));
    if (toggle == false) {
        statusBar()->hide();
    } else {
        statusBar()->show();
    } // end if
    statusBar()->showMessage(tr("Listo."));
    _depura("END Bulmacont::slotViewStatusBar", 0);
}


void Bulmacont::slotViewFullScreen(bool toggle)  {
    _depura("Bulmacont::slotViewFullScreen", 0);
    statusBar()->showMessage(tr("Cambiando a modo de pantalla completa..."));
    if (toggle == false) {
        showNormal();
    } else {
        showFullScreen();
    } // end if
    statusBar()->showMessage(tr("Listo."));
    _depura("END Bulmacont::slotViewFullScreen", 0);
}


void Bulmacont::slotStatusHelpMsg(const QString &text) {
    _depura("Bulmacont::slotStatusHelpMsg", 0);
    /// Change status message of whole statusbar temporary (text, msec).
    statusBar()->showMessage(text, 2000);
    _depura("END Bulmacont::slotStatusHelpMsg", 0);
}


void Bulmacont::slotListadoCuentas() {
    _depura("Bulmacont::slotListadoCuentas", 0);
    statusBar()->showMessage(tr("Listado de cuentas"));
    _depura("END Bulmacont::slotListadoCuentas", 0);
}


void Bulmacont::slotAsientos()  {
    _depura("Bulmacont::slotAsientos", 0);
    m_empresaactual->muestraasientos();
    _depura("END Bulmacont::slotAsientos", 0);
}


void Bulmacont::slotCentrosCoste() {
    _depura("Bulmacont::slotCentrosCoste", 0);
    m_empresaactual->ccostes();
    _depura("END Bulmacont::slotCentrosCoste", 0);
}


void Bulmacont::slotCanales() {
    _depura("Bulmacont::slotCanales", 0);
    m_empresaactual->canales();
    _depura("END Bulmacont::slotCanales", 0);
}


void Bulmacont::slotCompBalance() {
    _depura("Bulmacont::slotCompBalance", 0);
    m_empresaactual->compbalance();
    _depura("END Bulmacont::slotCompBalance", 0);
}


void Bulmacont::slotPerdidas() {}


void Bulmacont::windowMenuAboutToShow()  {}


void Bulmacont::slotWindowNewWindow() {}


void Bulmacont::closeEvent(QCloseEvent *) {
    _depura("Bulmacont::closeEvent", 0);
    delete m_empresaactual;
#ifdef WINDOWS

    exit(0);
#endif

    delete m_pWorkspace;
    _depura("END Bulmacont::closeEvent", 0);
}


void Bulmacont::on_actionAyuda_triggered() {
    _depura("Bulmacont::on_actionAyuda_triggered", 0);
    QAssistantClient *asistenteAyuda = new QAssistantClient(QLibraryInfo::location(QLibraryInfo::BinariesPath), 0);
    connect(asistenteAyuda, SIGNAL(error(const QString)), this, SLOT(documentacionError(const QString)));
    QStringList parametros;
    parametros << "-profile" << QString("/usr/share/bulmages/ayuda/bulmacont/bulmacont.adp");
    asistenteAyuda->setArguments(parametros);
    asistenteAyuda->openAssistant();
    _depura("END Bulmacont::on_actionAyuda_triggered", 0);
}


void Bulmacont::on_actionAcerca_de_triggered() {
    _depura("Bulmacont::on_actionAcerca_de_triggered", 0);
    sobreview *sobre = new sobreview(0);
    sobre->exec();
    _depura("END Bulmacont::on_actionAcerca_de_triggered", 0);
}


/** Captura el evento de cambio de ventana en el workSpace y actua sobre el
    listado de ventanas para que seleccione la ventana adecuada.
*/
void Bulmacont::informaindexador(QWidget *w) {
    _depura("Bulmacont::informaindexador", 0);
    /// No existe una ventana que activar.
    if (w == NULL) {
        m_empresaactual->deSeleccionaWindow();
        return;
    } // end if
    m_empresaactual->seleccionaWindow(w->windowTitle(), w);
    
    QString texto = "Window activated. " + w->windowTitle() + "\n";
    printf(texto.toAscii().constData());
    
    _depura("END Bulmacont::informaindexador", 0);
}

