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


    m_empresaactual = new Empresa(this);
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

    m_list->setVisible(TRUE);
    m_pb->setVisible(FALSE);

    initStatusBar();
    statusBar()->showMessage(tr("Listo"), 2000);

    _depura("END Bulmacont::Bulmacont", 0);
}


Bulmacont::~Bulmacont() {
    _depura("Bulmacont::~Bulmacont", 0);
    delete m_pWorkspace;
    /// En MS-Windows no termina bien la ejecucion del programa y por eso
    /// agregamos esta salida rapida.
#ifdef WINDOWS

    exit(0);
#endif

    _depura("END Bulmacont::~Bulmacont", 0);
}


Empresa *Bulmacont::empresaactual() {
    return m_empresaactual;
}


/// Exporta menues.
QMenu *Bulmacont::mui_MenuHerramientas() {
    return menuHerramientas;
}


QMenu *Bulmacont::mui_MenuVer() {
    return menuVer;
}


void Bulmacont::s_asiento1() {
    m_empresaactual->s_asiento1();
}


void Bulmacont::on_actionTipos_de_IVA_triggered() {
    m_empresaactual->tiposIVA();
}


void Bulmacont::on_actionFormas_de_Pago_triggered() {
    m_empresaactual->fPago();
}


void Bulmacont::on_actionAsientos_triggered() {
    m_empresaactual->muestraasientos();
}


void Bulmacont::on_actionConfiguracion_triggered() {
    m_empresaactual->propiedadempresa();
}


void Bulmacont::on_actionApuntes_Contables_triggered() {
    m_empresaactual->muestraapuntes1();
}


void Bulmacont::on_actionLibro_Mayor_triggered() {
    m_empresaactual->libromayor();
}


void Bulmacont::on_actionLibro_Diario_triggered() {
    m_empresaactual->librodiario();
}


void Bulmacont::on_actionPlan_Contable_triggered() {
    m_empresaactual->muestracuentas();
}


void Bulmacont::on_actionBalance_de_Cuentas_triggered() {
    m_empresaactual->librobalance();
}



void Bulmacont::on_actionImprimir_triggered() {
    m_empresaactual->boton_imprimir();
}


void Bulmacont::on_actionGuardar_triggered() {
    m_empresaactual->boton_guardar();
}


void Bulmacont::on_actionActualizar_triggered() {
    m_empresaactual->boton_reload();
}


void Bulmacont::on_actionFiltrar_triggered() {
    m_empresaactual->Filtro();
}


void Bulmacont::on_actionRecalcular_Saldos_Iniciales_triggered() {
    // empresaactual->recalculasaldos();
}


void Bulmacont::on_actionSustituir_Cuentas_triggered() {
    m_empresaactual->reemplazacuentaenasientos();
}


void Bulmacont::on_actionBloquear_Fechas_triggered() {
    m_empresaactual->bloqueoFechas();
}


void Bulmacont::on_actionCanal_por_Defecto_triggered() {
    m_empresaactual->canaldefecto();
}


void Bulmacont::on_actionCentros_de_Coste_triggered() {
    m_empresaactual->ccostes();
}


void Bulmacont::on_actionCanales_triggered() {
    m_empresaactual->canales();
}


void Bulmacont::on_actionCentro_de_Coste_por_Defecto_triggered() {
    m_empresaactual->centrocostedefecto();
}


void Bulmacont::on_actionOrdenar_Ventanas_triggered() {
    m_pWorkspace->tile();
}


void Bulmacont::on_actionOrganizaci_n_en_Cascada_triggered() {
    m_pWorkspace->cascade();
}


void Bulmacont::on_actionAbrir_Asientos_triggered() {
    m_empresaactual->Abrirasientos();
}


void Bulmacont::on_actionOrdenar_Asientos_triggered() {
    m_empresaactual->Ordenarasientos();
}


void Bulmacont::on_actionAsiento_de_Apertura_triggered() {
    m_empresaactual->abreempresa();
}


void Bulmacont::on_actionAsiento_de_Regularizacion_triggered() {
    m_empresaactual->regularizaempresa();
}


void Bulmacont::on_actionAsiento_de_Cierre_triggered() {
    m_empresaactual->cierraempresa();
}


void Bulmacont::on_actionCuentas_Anuales_triggered() {
    m_empresaactual->compbalance();
}


void Bulmacont::on_actionMasas_Patrimoniales_triggered() {
    m_empresaactual->mpatrimoniales();
}


void Bulmacont::on_actionRegistro_de_IVA_triggered() {
    m_empresaactual->registroiva();
}


void Bulmacont::on_actionListado_347_triggered() {
    m_empresaactual->listado347();
}


void Bulmacont::on_actionCobros_y_Pagos_triggered() {
    m_empresaactual->cobPag();
}


void Bulmacont::on_actionAmortizaciones_triggered() {
    m_empresaactual->amortizaciones();
}


void Bulmacont::on_actionPantalla_Completa_triggered() {
    if (isFullScreen()) {
        showNormal();
    } else {
        showFullScreen();
    } // end if
}


void Bulmacont::on_actionSiguiente_triggered() {
    m_empresaactual->boton_siguiente();
}


void Bulmacont::on_actionAnterior_triggered() {
    m_empresaactual->boton_anterior();
}


void Bulmacont::on_actionInicio_triggered() {
    m_empresaactual->boton_primero();
}


void Bulmacont::on_actionFin_triggered() {
    m_empresaactual->boton_ultimo();
}


void Bulmacont::on_actionIndexador_triggered() {
    if (actionIndexador->isChecked() == TRUE) {
        m_empresaactual->s_indexadorCambiaEstado(TRUE);
    } else {
        m_empresaactual->s_indexadorCambiaEstado(FALSE);
    } // end if
}


void Bulmacont::setActionIndexador(bool visible) {
    if (visible == TRUE) {
        actionIndexador->setChecked(TRUE);
    } else {
        actionIndexador->setChecked(FALSE);
    } // end if
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
    m_empresaactual = NULL;
    delete m_list;
    m_list = NULL;

#ifdef WINDOWS
    exit(0);
#endif

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


void Bulmacont::on_actionPaises_triggered() {
    _depura("Bulmacont::on_actionPaises_triggered", 0);
    m_empresaactual->muestraPaises();
    _depura("Bulmacont::on_actionPaises_triggered", 0);
}



/** Captura el evento de cambio de ventana en el workSpace y actua sobre el
    listado de ventanas para que seleccione la ventana adecuada.
*/
void Bulmacont::informaindexador(QWidget *w) {
    _depura("Bulmacont::informaindexador", 0);
    /// No existe una ventana que activar.
    if (m_empresaactual == NULL)
        return;

    if (w == NULL) {
        m_empresaactual->deSeleccionaWindow();
        return;
    } // end if
    m_empresaactual->deSeleccionaWindow();
    m_empresaactual->seleccionaWindow(w->windowTitle(), w);

    QString texto = "Window activated. " + w->windowTitle() + "\n";
    printf(texto.toAscii().constData());

    _depura("END Bulmacont::informaindexador", 0);
}

QWorkspace2 *Bulmacont::workspace() {
	return m_pWorkspace;
}



