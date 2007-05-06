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

    showNormal();
    m_company = new company();
    m_company->setProgressBar(m_pb);
    m_company->init(bd, "BulmaFact");
    m_company->setWorkspace(pWorkspace);

    connect(pWorkspace, SIGNAL(windowActivated(QWidget *)), this, SLOT(informaindexador(QWidget *)));

    /// Aqui creamos la ventana dock para meter las distintas ventanas.
    m_list = new listventanas(0);
    m_list->setVisible(FALSE);
    /// Iniciamos el listventanas con el workspace para que pueda operar con el.
    m_list->setWorkspace(pWorkspace);

    connect(m_list, SIGNAL(cambiaEstadoVisible(bool)), this, SLOT(setActionIndexador(bool)));
    addDockWidget(Qt::LeftDockWidgetArea, m_list);

    m_company->setListVentanas(m_list);

    m_list->setVisible(TRUE);
    m_pb->setVisible(FALSE);
    statusBar()->showMessage(tr("Listo"), 2000);
    _depura("END bulmafact::bulmafact", 0);
}

/** Hacemos la creacion de las ventanas principales para que enten en plugins
*/
void bulmafact::createMainWindows() {
    _depura("bulmafact::createMainWindows", 0);
    m_company->createMainWindows();
    _depura("END bulmafact::createMainWindows", 0);
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
    m_company = NULL;
    delete m_list;
    m_list = NULL;
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

    /// Si no esta inicializado company no se le puede informar.
    if ( m_company == NULL)
        return;

    /// No existe una ventana que activar.
    if (w == NULL) {
        m_company->deSeleccionaWindow();
        return;
    } // end if
    m_company->seleccionaWindow(w->windowTitle(), w);

    QString texto = "Window activated. " + w->windowTitle();
    _depura(texto, 10);

    _depura("END bulmafact::informaindexador", 0);
}


/** Muestra la ayuda del programa.
*/
void bulmafact::on_actionDocumentacion_triggered() {
    _depura("bulmafact::on_actionDocumentacion_triggered", 0);
    QAssistantClient *asistenteAyuda = new QAssistantClient(QLibraryInfo::location(QLibraryInfo::BinariesPath), 0);
    connect(asistenteAyuda, SIGNAL(error(const QString)), this, SLOT(documentacionError(const QString)));
    QStringList parametros;
    parametros << "-profile" << QString("/usr/share/bulmages/ayuda/bulmafact/bulmafact.adp");
    asistenteAyuda->setArguments(parametros);
    asistenteAyuda->openAssistant();
    _depura("END bulmafact::on_actionDocumentacion_triggered", 0);
}


void bulmafact::documentacionError(const QString docError) {
    _depura("Error en la ayuda: " + docError, 10);
}

void bulmafact::setActionIndexador(bool visible) {
    _depura("bulmafact::setActionIndexador", 0);
    if (visible == TRUE) {
        actionIndexador->setChecked(TRUE);
    } else {
        actionIndexador->setChecked(FALSE);
    } // end if
    _depura("END bulmafact::setActionIndexador", 0);
}

void bulmafact::on_actionIndexador_triggered() {
    _depura("bulmafact::on_actionIndexador_triggered", 0);
    if (actionIndexador->isChecked() == TRUE) {
        m_company->s_indexadorCambiaEstado(TRUE);
    } else {
        m_company->s_indexadorCambiaEstado(FALSE);
    } // end if
    _depura("END bulmafact::on_actionIndexador_triggered", 0);
}


void bulmafact::on_actionTipoIVA_triggered() {
    _depura("bulmafact::on_actionTipoIVA_triggered", 0);
    m_company->s_TipoIVAView();
    _depura("END bulmafact::on_actionTipoIVA_triggered", 0);
}


void bulmafact::on_actionTasaIVA_triggered() {
    _depura("bulmafact::on_actionTasaIVA_triggered", 0);
    m_company->s_TasaIVAView();
    _depura("END bulmafact::on_actionTasaIVA_triggered", 0);
}

QWorkspace2 * bulmafact::workspace() {
    _depura("bulmafact::workspace", 0);
    _depura("END bulmafact::workspace", 0);
    return pWorkspace;
}

company * bulmafact::getcompany() {
    _depura("bulmafact::getcompany", 0);
    _depura("END bulmafact::getcompany", 0);
    return m_company;
}

void bulmafact::on_actionNuevo_Pedido_Proveedor_triggered() {
    _depura("bulmafact::on_actionNuevo_Pedido_Proveedor_triggered", 0);
    m_company->s_newPedidoPro();
    _depura("END bulmafact::on_actionNuevo_Pedido_Proveedor_triggered", 0);
}
void bulmafact::on_actionListado_Pedidos_Proveedor_triggered() {
    _depura("bulmafact::on_actionListado_Pedidos_Proveedor_triggered", 0);
    m_company->s_listPedidosPro();
    _depura("END bulmafact::on_actionListado_Pedidos_Proveedor_triggered", 0);
}
void bulmafact::on_actionNuevo_Albaran_Proveedor_triggered() {
    _depura("bulmafact::on_actionNuevo_Albaran_Proveedor_triggered", 0);
    m_company->s_newAlbaranPro();
    _depura("END bulmafact::on_actionNuevo_Albaran_Proveedor_triggered", 0);
}
void bulmafact::on_actionListado_Albaranes_Proveedor_triggered() {
    _depura("bulmafact::on_actionListado_Albaranes_Proveedor_triggered", 0);
    m_company->lAlbaranesProveedor();
    _depura("END bulmafact::on_actionListado_Albaranes_Proveedor_triggered", 0);
}
void bulmafact::on_actionNueva_Factura_Proveedor_triggered() {
    _depura("bulmafact::on_actionNueva_Factura_Proveedor_triggered", 0);
    m_company->s_newFacturaPro();
    _depura("END bulmafact::on_actionNueva_Factura_Proveedor_triggered", 0);
}
void bulmafact::on_actionListado_Facturas_Proveedor_triggered() {
    _depura("bulmafact::on_actionListado_Facturas_Proveedor_triggered", 0);
    m_company->s_listFacturasPro();
    _depura("END bulmafact::on_actionListado_Facturas_Proveedor_triggered", 0);
}
void bulmafact::on_actionNuevo_Presupuesto_triggered() {
    _depura("bulmafact::on_actionNuevo_Presupuesto_triggered", 0);
    m_company->s_newPresupuestoCli();
    _depura("END bulmafact::on_actionNuevo_Presupuesto_triggered", 0);
}
void bulmafact::on_actionListado_Presupuestos_triggered() {
    _depura("bulmafact::on_actionListado_Presupuestos_triggered", 0);
    m_company->listBudgets();
    _depura("END bulmafact::on_actionListado_Presupuestos_triggered", 0);
}
void bulmafact::on_actionNuevo_Pedido_Cliente_triggered() {
    _depura("bulmafact::on_actionNuevo_Pedido_Cliente_triggered", 0);
    m_company->s_newPedidoClienteView();
    _depura("END bulmafact::on_actionNuevo_Pedido_Cliente_triggered", 0);
}
void bulmafact::on_actionListado_Pedidos_Cliente_triggered() {
    _depura("bulmafact::on_actionListado_Pedidos_Cliente_triggered", 0);
    m_company->s_listPedidosCli();
    _depura("END bulmafact::on_actionListado_Pedidos_Cliente_triggered", 0);
}
void bulmafact::on_actionNuevo_Albaran_Cliente_triggered() {
    _depura("bulmafact::on_actionNuevo_Albaran_Cliente_triggered", 0);
    m_company->s_newAlbaranClienteView();
    _depura("END bulmafact::on_actionNuevo_Albaran_Cliente_triggered", 0);
}
void bulmafact::on_actionListado_Albaranes_Cliente_triggered() {
    _depura("bulmafact::on_actionListado_Albaranes_Cliente_triggered", 0);
    m_company->listClientDelivNotes();
    _depura("END bulmafact::on_actionListado_Albaranes_Cliente_triggered", 0);
}
void bulmafact::on_actionNueva_Factura_Cliente_triggered() {
    _depura("bulmafact::on_actionNueva_Factura_Cliente_triggered", 0);
    m_company->s_newFacturaCli();
    _depura("END bulmafact::on_actionNueva_Factura_Cliente_triggered", 0);
}
void bulmafact::on_actionListado_Facturas_Cliente_triggered() {
    _depura("bulmafact::on_actionListado_Facturas_Cliente_triggered", 0);
    m_company->s_listFacturasCli();
    _depura("END bulmafact::on_actionListado_Facturas_Cliente_triggered", 0);
}
void bulmafact::on_actionGestion_Familias_triggered() {
    _depura("bulmafact::on_actionGestion_Familias_triggered", 0);
    m_company->s_newfamiliasview();
    _depura("END bulmafact::on_actionGestion_Familias_triggered", 0);
}
void bulmafact::on_actionGestion_Tipos_Articulo_triggered() {
    _depura("bulmafact::on_actionGestion_Tipos_Articulo_triggered", 0);
    m_company->s_newTipoArticuloList();
    _depura("END bulmafact::on_actionGestion_Tipos_Articulo_triggered", 0);
}
void bulmafact::on_actionNuevo_Articulo_triggered() {
    _depura("bulmafact::on_actionNuevo_Articulo_triggered", 0);
    m_company->s_newArticulo();
    _depura("END bulmafact::on_actionNuevo_Articulo_triggered", 0);
}
void bulmafact::on_actionListado_de_Articulos_triggered() {
    _depura("bulmafact::on_actionListado_de_Articulos_triggered", 0);
    m_company->listarticles();
    _depura("END bulmafact::on_actionListado_de_Articulos_triggered", 0);
}

void bulmafact::on_actionNuevo_Proveedor_triggered() {
    _depura("bulmafact::on_actionNuevo_Proveedor_triggered", 0);
    m_company->s_newProveedorView();
    _depura("END bulmafact::on_actionNuevo_Proveedor_triggered", 0);
}
void bulmafact::on_actionListado_Proveedores_triggered() {
    _depura("bulmafact::on_actionListado_Proveedores_triggered", 0);
    m_company->listproviders();
    _depura("END bulmafact::on_actionListado_Proveedores_triggered", 0);
}
void bulmafact::on_actionNuevo_Cliente_triggered() {
    _depura("bulmafact::on_actionNuevo_Cliente_triggered", 0);
    m_company->s_newClienteView();
    _depura("END bulmafact::on_actionNuevo_Cliente_triggered", 0);
}
void bulmafact::on_actionListado_Clientes_triggered() {
    _depura("bulmafact::on_actionListado_Clientes_triggered", 0);
    m_company->listClients();
    _depura("END bulmafact::on_actionListado_Clientes_triggered", 0);
}
void bulmafact::on_actionProvincias_triggered() {
    _depura("bulmafact::on_actionProvincias_triggered", 0);
    m_company->s_provincias();
    _depura("END bulmafact::on_actionProvincias_triggered", 0);
}
void bulmafact::on_actionSeries_de_Factura_triggered() {
    _depura("bulmafact::on_actionSeries_de_Factura_triggered", 0);
    m_company->s_seriesFactura();
    _depura("END bulmafact::on_actionSeries_de_Factura_triggered", 0);
}


void bulmafact::on_actionTrabajadores_triggered() {
    _depura("bulmafact::on_actionTrabajadores_triggered", 0);
    m_company->s_trabajadores();
    _depura("END bulmafact::on_actionTrabajadores_triggered", 0);
}

void bulmafact::on_actionBancos_triggered() {
    _depura("bulmafact::on_actionBancos_triggered", 0);
    m_company->s_bancos();
    _depura("END bulmafact::on_actionBancos_triggered", 0);
}

void bulmafact::on_actionVentana_Completa_triggered() {
    _depura("bulmafact::on_actionVentana_Completa_triggered", 0);
    s_ventanaCompleta();
    _depura("END bulmafact::on_actionVentana_Completa_triggered", 0);
}
void bulmafact::on_actionAcerta_de_triggered() {
    _depura("bulmafact::on_actionAcerta_de_triggered", 0);
    s_About();
    _depura("END bulmafact::on_actionAcerta_de_triggered", 0);
}
void bulmafact::on_actionAlmacenes_triggered() {
    _depura("bulmafact::on_actionAlmacenes_triggered", 0);
    m_company->s_almacenes();
    _depura("END bulmafact::on_actionAlmacenes_triggered", 0);
}
void bulmafact::on_actionListado_de_Pagos_triggered() {
    _depura("bulmafact::on_actionListado_de_Pagos_triggered", 0);
    m_company->viewPagosList();
    _depura("END bulmafact::on_actionListado_de_Pagos_triggered", 0);
}
void bulmafact::on_actionNuevo_Pago_triggered() {
    _depura("bulmafact::on_actionNuevo_Pago_triggered", 0);
    m_company->s_newPagoView();
    _depura("END bulmafact::on_actionNuevo_Pago_triggered", 0);
}
void bulmafact::on_actionNuevo_Cobro_triggered() {
    _depura("bulmafact::on_actionNuevo_Cobro_triggered", 0);
    m_company->s_newCobroView();
    _depura("END bulmafact::on_actionNuevo_Cobro_triggered", 0);
}
void bulmafact::on_actionListado_de_Cobros_triggered() {
    _depura("bulmafact::on_actionListado_de_Cobros_triggered", 0);
    m_company->viewCobrosList();
    _depura("END bulmafact::on_actionListado_de_Cobros_triggered", 0);
}
void bulmafact::on_actionParametros_triggered() {
    _depura("bulmafact::on_actionParametros_triggered", 0);
    m_company->s_newListConfiguracionView();
    _depura("END bulmafact::on_actionParametros_triggered", 0);
}
void bulmafact::on_actionFormas_de_Pago_triggered() {
    _depura("bulmafact::on_actionFormas_de_Pago_triggered", 0);
    s_FPago();
    _depura("END bulmafact::on_actionFormas_de_Pago_triggered", 0);
}


