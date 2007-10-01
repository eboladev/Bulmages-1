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


///
/**
\param parent
\param f
\param DB
**/
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
    statusBar()->showMessage(DB, 2000);
    setWindowTitle(DB);
    _depura("END Bulmacont::Bulmacont", 0);
}


///
/**
**/
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


///
/**
\return
**/
Empresa *Bulmacont::empresaactual() {
    _depura("Bulmacont::empresaactual", 0);
    _depura("END Bulmacont::empresaactual", 0);
    return m_empresaactual;
}


/// Exporta menues.
/**
\return
**/
QMenu *Bulmacont::mui_MenuHerramientas() {
    _depura("Bulmacont::mui_MenuHerramientas", 0);
    _depura("END Bulmacont::mui_MenuHerramientas", 0);
    return menuHerramientas;
}


///
/**
\return
**/
QMenu *Bulmacont::mui_MenuVer() {
    _depura("Bulmacont::mui_MenuVer", 0);
    _depura("END Bulmacont::mui_MenuVer", 0);
    return menuVer;
}


///
/**
**/
void Bulmacont::s_asiento1() {
    _depura("Bulmacont::s_asiento1", 0);
    m_empresaactual->s_asiento1();
    _depura("END Bulmacont::s_asiento1", 0);
}


///
/**
**/
void Bulmacont::on_actionTipos_de_IVA_triggered() {
    _depura("Bulmacont::on_actionTipos_de_IVA_triggered", 0);
    m_empresaactual->tiposIVA();
    _depura("END Bulmacont::on_actionTipos_de_IVA_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionFormas_de_Pago_triggered() {
    _depura("Bulmacont::on_actionFormas_de_Pago_triggered", 0);
    m_empresaactual->fPago();
    _depura("END Bulmacont::on_actionFormas_de_Pago_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionAsientos_triggered() {
    _depura("Bulmacont::on_actionAsientos_triggered", 0);
    m_empresaactual->muestraasientos();
    _depura("END Bulmacont::on_actionAsientos_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionConfiguracion_triggered() {
    _depura("Bulmacont::on_actionConfiguracion_triggered", 0);
    m_empresaactual->propiedadempresa();
    _depura("END Bulmacont::on_actionConfiguracion_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionApuntes_Contables_triggered() {
    _depura("Bulmacont::on_actionApuntes_Contables_triggered", 0);
    m_empresaactual->muestraapuntes1();
    _depura("END Bulmacont::on_actionApuntes_Contables_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionLibro_Mayor_triggered() {
    _depura("Bulmacont::on_actionLibro_Mayor_triggered", 0);
    m_empresaactual->libromayor();
    _depura("Bulmacont::on_actionLibro_Mayor_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionLibro_Diario_triggered() {
    _depura("Bulmacont::on_actionLibro_Diario_triggered", 0);
    m_empresaactual->librodiario();
    _depura("END Bulmacont::on_actionLibro_Diario_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionPlan_Contable_triggered() {
    _depura("Bulmacont::on_actionPlan_Contable_triggered", 0);
    m_empresaactual->muestracuentas();
    _depura("END Bulmacont::on_actionPlan_Contable_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionBalance_de_Cuentas_triggered() {
    _depura("Bulmacont::on_actionBalance_de_Cuentas_triggered", 0);
    m_empresaactual->librobalance();
    _depura("END Bulmacont::on_actionBalance_de_Cuentas_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionImprimir_triggered() {
    _depura("Bulmacont::on_actionImprimir_triggered", 0);
    m_empresaactual->boton_imprimir();
    _depura("END Bulmacont::on_actionImprimir_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionGuardar_triggered() {
    _depura("Bulmacont::on_actionGuardar_triggered", 0);
    m_empresaactual->boton_guardar();
    _depura("END Bulmacont::on_actionGuardar_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionActualizar_triggered() {
    _depura("Bulmacont::on_actionActualizar_triggered", 0);
    m_empresaactual->boton_reload();
    _depura("END Bulmacont::on_actionActualizar_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionFiltrar_triggered() {
    _depura("Bulmacont::on_actionFiltrar_triggered", 0);
    m_empresaactual->Filtro();
    _depura("END Bulmacont::on_actionFiltrar_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionRecalcular_Saldos_Iniciales_triggered() {
    _depura("Bulmacont::on_actionRecalcular_Saldos_Iniciales_triggered", 0);
    _depura("END Bulmacont::on_actionRecalcular_Saldos_Iniciales_triggered", 0);
    // empresaactual->recalculasaldos();
}


///
/**
**/
void Bulmacont::on_actionSustituir_Cuentas_triggered() {
    _depura("Bulmacont::on_actionSustituir_Cuentas_triggered", 0);
    m_empresaactual->reemplazacuentaenasientos();
    _depura("Bulmacont::on_actionSustituir_Cuentas_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionBloquear_Fechas_triggered() {
    _depura("Bulmacont::on_actionBloquear_Fechas_triggered", 0);
    m_empresaactual->bloqueoFechas();
    _depura("END Bulmacont::on_actionBloquear_Fechas_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionCanal_por_Defecto_triggered() {
    _depura("Bulmacont::on_actionCanal_por_Defecto_triggered", 0);
    m_empresaactual->canaldefecto();
    _depura("END Bulmacont::on_actionCanal_por_Defecto_triggered", 0);
}

///
/**
**/
void Bulmacont::on_actionCentros_de_Coste_triggered() {
    _depura("Bulmacont::on_actionCentros_de_Coste_triggered", 0);
    m_empresaactual->ccostes();
    _depura("END Bulmacont::on_actionCentros_de_Coste_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionCanales_triggered() {
    _depura("Bulmacont::on_actionCanales_triggered", 0);
    m_empresaactual->canales();
    _depura("END Bulmacont::on_actionCanales_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionCentro_de_Coste_por_Defecto_triggered() {
    _depura("Bulmacont::on_actionCentro_de_Coste_por_Defecto_triggered", 0);
    m_empresaactual->centrocostedefecto();
    _depura("END Bulmacont::on_actionCentro_de_Coste_por_Defecto_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionOrdenar_Ventanas_triggered() {
    _depura("Bulmacont::on_actionOrdenar_Ventanas_triggered", 0);
    m_pWorkspace->tile();
    _depura("END Bulmacont::on_actionOrdenar_Ventanas_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionOrganizaci_n_en_Cascada_triggered() {
    _depura("Bulmacont::on_actionOrganizaci_n_en_Cascada_triggered", 0);
    m_pWorkspace->cascade();
    _depura("END Bulmacont::on_actionOrganizaci_n_en_Cascada_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionAbrir_Asientos_triggered() {
    _depura("Bulmacont::on_actionAbrir_Asientos_triggered", 0);
    m_empresaactual->Abrirasientos();
    _depura("END Bulmacont::on_actionAbrir_Asientos_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionOrdenar_Asientos_triggered() {
    _depura("Bulmacont::on_actionOrdenar_Asientos_triggered", 0);
    m_empresaactual->Ordenarasientos();
    _depura("END Bulmacont::on_actionOrdenar_Asientos_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionAsiento_de_Apertura_triggered() {
    _depura("Bulmacont::on_actionAsiento_de_Apertura_triggered", 0);
    m_empresaactual->abreempresa();
    _depura("END Bulmacont::on_actionAsiento_de_Apertura_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionAsiento_de_Regularizacion_triggered() {
    _depura("Bulmacont::on_actionAsiento_de_Regularizacion_triggered", 0);
    m_empresaactual->regularizaempresa();
    _depura("END Bulmacont::on_actionAsiento_de_Regularizacion_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionAsiento_de_Cierre_triggered() {
    _depura("Bulmacont::on_actionAsiento_de_Cierre_triggered", 0);
    m_empresaactual->cierraempresa();
    _depura("END Bulmacont::on_actionAsiento_de_Cierre_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionCuentas_Anuales_triggered() {
    _depura("Bulmacont::on_actionCuentas_Anuales_triggered", 0);
    m_empresaactual->compbalance();
    _depura("END Bulmacont::on_actionCuentas_Anuales_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionMasas_Patrimoniales_triggered() {
    _depura("Bulmacont::on_actionMasas_Patrimoniales_triggered", 0);
    m_empresaactual->mpatrimoniales();
    _depura("END Bulmacont::on_actionMasas_Patrimoniales_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionListado_347_triggered() {
    _depura("Bulmacont::on_actionListado_347_triggered", 0);
    m_empresaactual->listado347();
    _depura("END Bulmacont::on_actionListado_347_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionAmortizaciones_triggered() {
    _depura("Bulmacont::on_actionAmortizaciones_triggered", 0);
    m_empresaactual->amortizaciones();
    _depura("END Bulmacont::on_actionAmortizaciones_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionPantalla_Completa_triggered() {
    _depura("Bulmacont::on_actionPantalla_Completa_triggered", 0);
    if (isFullScreen()) {
        showNormal();
    } else {
        showFullScreen();
    } // end if
    _depura("END Bulmacont::on_actionPantalla_Completa_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionSiguiente_triggered() {
    _depura("Bulmacont::on_actionSiguiente_triggered", 0);
    m_empresaactual->boton_siguiente();
    _depura("END Bulmacont::on_actionSiguiente_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionAnterior_triggered() {
    _depura("Bulmacont::on_actionAnterior_triggered", 0);
    m_empresaactual->boton_anterior();
    _depura("END Bulmacont::on_actionAnterior_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionInicio_triggered() {
    _depura("Bulmacont::on_actionInicio_triggered", 0);
    m_empresaactual->boton_primero();
    _depura("END Bulmacont::on_actionInicio_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionFin_triggered() {
    _depura("Bulmacont::on_actionFin_triggered", 0);
    m_empresaactual->boton_ultimo();
    _depura("END Bulmacont::on_actionFin_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionIndexador_triggered() {
    _depura("Bulmacont::on_actionIndexador_triggered", 0);
    if (actionIndexador->isChecked() == TRUE) {
        m_empresaactual->s_indexadorCambiaEstado(TRUE);
    } else {
        m_empresaactual->s_indexadorCambiaEstado(FALSE);
    } // end if
    _depura("END Bulmacont::on_actionIndexador_triggered", 0);
}


///
/**
\param visible
**/
void Bulmacont::setActionIndexador(bool visible) {
    _depura("Bulmacont::setActionIndexador", 0);
    if (visible == TRUE) {
        actionIndexador->setChecked(TRUE);
    } else {
        actionIndexador->setChecked(FALSE);
    } // end if
    _depura("END Bulmacont::setActionIndexador", 0);
}


///
/**
**/
void Bulmacont::initStatusBar() {
    _depura("Bulmacont::initStatusBar", 0);
    statusBar()->showMessage(tr("Listo."));
    _depura("END Bulmacont::initStatusBar", 0);
}


///
/**
\param object
\param event
\return
**/
bool Bulmacont::eventFilter(QObject *object, QEvent *event) {
    _depura("Bulmacont::eventFilter", 0);
    /// Standard event processing.
    return QWidget::eventFilter(object, event);
}


///
/**
**/
void Bulmacont::slotEditUndo()  {
    _depura("Bulmacont::slotEditUndo", 0);
    statusBar()->showMessage(tr("Deshaciendo la ultima accion..."));
    statusBar()->showMessage(tr("Listo."));
    _depura("END Bulmacont::slotEditUndo", 0);
}


///
/**
**/
void Bulmacont::slotEditCut()  {
    _depura("Bulmacont::slotEditCut", 0);
    statusBar()->showMessage(tr("Cortando seleccion..."));
    statusBar()->showMessage(tr("Listo."));
    _depura("EMD Bulmacont::slotEditCut", 0);
}


///
/**
**/
void Bulmacont::slotEditCopy()  {
    _depura("Bulmacont::slotEditCopy", 0);
    statusBar()->showMessage(tr("Copiando la seleccion al portapapeles..."));
    statusBar()->showMessage(tr("Listo."));
    _depura("END Bulmacont::slotEditCopy", 0);
}


///
/**
**/
void Bulmacont::slotEditPaste() {
    _depura("Bulmacont::slotEditPaste", 0);
    statusBar()->showMessage(tr("Insertando el contenido del portapapeles..."));
    statusBar()->showMessage(tr("Listo."));
    _depura("END Bulmacont::slotEditPaste", 0);
}


///
/**
\param toggle
**/
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


///
/**
\param toggle
**/
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


///
/**
\param text
**/
void Bulmacont::slotStatusHelpMsg(const QString &text) {
    _depura("Bulmacont::slotStatusHelpMsg", 0);
    /// Change status message of whole statusbar temporary (text, msec).
    statusBar()->showMessage(text, 2000);
    _depura("END Bulmacont::slotStatusHelpMsg", 0);
}


///
/**
**/
void Bulmacont::slotListadoCuentas() {
    _depura("Bulmacont::slotListadoCuentas", 0);
    statusBar()->showMessage(tr("Listado de cuentas"));
    _depura("END Bulmacont::slotListadoCuentas", 0);
}


///
/**
**/
void Bulmacont::slotAsientos()  {
    _depura("Bulmacont::slotAsientos", 0);
    m_empresaactual->muestraasientos();
    _depura("END Bulmacont::slotAsientos", 0);
}


///
/**
**/
void Bulmacont::slotCentrosCoste() {
    _depura("Bulmacont::slotCentrosCoste", 0);
    m_empresaactual->ccostes();
    _depura("END Bulmacont::slotCentrosCoste", 0);
}


///
/**
**/
void Bulmacont::slotCanales() {
    _depura("Bulmacont::slotCanales", 0);
    m_empresaactual->canales();
    _depura("END Bulmacont::slotCanales", 0);
}


///
/**
**/
void Bulmacont::slotCompBalance() {
    _depura("Bulmacont::slotCompBalance", 0);
    m_empresaactual->compbalance();
    _depura("END Bulmacont::slotCompBalance", 0);
}


///
/**
**/
void Bulmacont::slotPerdidas() {
    _depura("Bulmacont::slotPerdidas", 0);
    _depura("END Bulmacont::slotPerdidas", 0);
}


///
/**
**/
void Bulmacont::windowMenuAboutToShow()  {
    _depura("Bulmacont::windowMenuAboutToShow", 0);
    _depura("END Bulmacont::windowMenuAboutToShow", 0);
}


///
/**
**/
void Bulmacont::slotWindowNewWindow() {
    _depura("Bulmacont::slotWindowNewWindow", 0);
    _depura("END Bulmacont::slotWindowNewWindow", 0);
}


///
/**
**/
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


///
/**
**/
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


///
/**
**/
void Bulmacont::on_actionAcerca_de_triggered() {
    _depura("Bulmacont::on_actionAcerca_de_triggered", 0);
    sobreview *sobre = new sobreview(0);
    sobre->exec();
    _depura("END Bulmacont::on_actionAcerca_de_triggered", 0);
}


///
/**
**/
void Bulmacont::on_actionPaises_triggered() {
    _depura("Bulmacont::on_actionPaises_triggered", 0);
    m_empresaactual->muestraPaises();
    _depura("Bulmacont::on_actionPaises_triggered", 0);
}



/** Captura el evento de cambio de ventana en el workSpace y actua sobre el
    listado de ventanas para que seleccione la ventana adecuada.
*/
///
/**
\param w
\return
**/
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


///
/**
\return
**/
QWorkspace2 *Bulmacont::workspace() {
    _depura("Bulmacont::workspace", 0);
    _depura("END Bulmacont::workspace", 0);
	return m_pWorkspace;
}



