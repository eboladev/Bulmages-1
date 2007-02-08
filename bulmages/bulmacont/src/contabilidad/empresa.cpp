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

#include <QObject>

#include "empresa.h"
#include "abreempresaview.h"
#include "listcuentasview1.h"
#include "asiento1view.h"
#include "asientosview.h"
#include "amortizacionesview.h"
#include "canualesview.h"
#include "balance1view.h"
#include "cuentaview.h"
#include "canalview.h"
#include "ccosteview.h"
#include "mpatrimonialesview.h"
#include "propiedadesempresa.h"
#include "cambiactaview.h"
#include "tipoivaview.h"
#include "fpagoview.h"
#include "selectccosteview.h"
#include "selectcanalview.h"
#include "plugins.h"
#include "bbloqfecha.h"
#include "diarioview.h"

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif


/// SLOT que muestra la ventana de asientos.
void empresa::s_asiento1() {
    _depura("empresa::s_asiento1", 0);
    introapunts2->setWindowState(introapunts2->windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
}


empresa::empresa() : EmpresaBase() {
    _depura("empresa::empresa", 0);
    diario = NULL;
    extracto = NULL;
    balance1 = NULL;
    balance = NULL;
    selccostes  = NULL;
    selcanales = NULL;
    introapunts2 = NULL;
    m_listasientos = NULL;
    nombre = "";
    _depura("END empresa::empresa", 0);
}


empresa::~empresa() {
    _depura("~empresa", 0);
    if (diario) {
        delete diario;
    } // end if
    if (extracto) {
        delete extracto;
    } // end if
    if (balance1) {
        delete balance1;
    } // end if
    if (balance) {
        delete balance;
    } // end if
    if (selccostes) {
        delete selccostes;
    } // end if
    if (selcanales) {
        delete selcanales;
    } // end if
    if (introapunts2) {
        delete introapunts2;
    } // end if
    if (m_listasientos) {
        delete m_listasientos;
    } // end if
    _depura("END ~empresa", 0);
}

/// Inicializa la clase con el nombre de la base de datos y con el workspace.
int empresa::inicializa1() {
    _depura("empresa::inicializa1", 0);
    /// Calculamos el n&uacute;mero de d&iacute;gitos que tiene el nivel &uacute;ltimo
    /// de la empresa.
    QString query = "SELECT * FROM configuracion WHERE nombre = 'CodCuenta'";
    cursor2 *cursoraux1 = cargacursor(query, "codcuenta");
    numdigitos = cursoraux1->valor(2).length();
    delete cursoraux1;
    if (extracto != NULL) {
        delete extracto;
        delete diario;
        delete balance;
        delete balance1;
        delete selccostes;
        delete selcanales;
        delete m_listasientos;
    } // end if

    /// Inicializamos los selectores de centros de coste y canales.
    selccostes = new selectccosteview(this, 0);
    selcanales = new selectcanalview(this, 0);

    /// Inicializamos las ventanas de uso generalizado.
    extracto = new extractoview1(this, 0);
    m_pWorkspace->addWindow(extracto);

    diario = new DiarioView(this, 0);
    m_pWorkspace->addWindow(diario);

    balance = new balanceview(this, 0);
    m_pWorkspace->addWindow(balance);

    balance1 = new BalanceTreeView(this, 0);
    m_pWorkspace->addWindow(balance1);

    introapunts2 = new Asiento1View(this, 0);
    m_pWorkspace->addWindow(introapunts2);

    m_listasientos = new asientosview(this, 0);
    m_listasientos->inicializa();
    m_pWorkspace->addWindow(m_listasientos);

    m_listcuentas = new listcuentasview1(this, 0);
    m_listcuentas->inicializa();
    m_pWorkspace->addWindow(m_listcuentas);

    _depura("END empresa::inicializa1", 0);
    return(0);
}


/// Maximiza todas las ventanas que est&aacute;n en el workspace de la aplicaci&oacute;n.
/** Env&iacute;a un mensaje showMaximized a todos los objetos que deben ser maximizados. */
void empresa::maximiza() {
    _depura("empresa::maximiza", 0);
    diario->showMaximized();
    extracto->showMaximized();
    balance1->showMaximized();
    balance->showMaximized();
    _depura("END empresa::maximiza", 0);
}


/// Esta funci&oacute;n abre la ventana que presenta las cuentas contables.
int empresa::muestracuentas() {
    m_listcuentas->show();
    return 0;
}


/// Esta funci&oacute;n presenta los centros de coste.
/** Luego recarga los comboboxes de todo el resto de pantallas para que se actualizen. */
int empresa::ccostes() {
    _depura("empresa::ccostes", 0);
    ccosteview *ccoste = new ccosteview(this, 0);
    m_pWorkspace->addWindow(ccoste);
    ccoste->show();
    balance->cargacostes();
    _depura("END empresa::ccostes", 0);
    return 0;
}


/// Esta funci&oacute;n presenta la pantalla de edicion de canales.
/** Luego recarga los comboboxes de todo el resto de pantallas para que se actualizen. */
int empresa::canales() {
    _depura("empresa::canales", 0);
    canalview *canal = new canalview(this, 0);
    m_pWorkspace->addWindow(canal);
    canal->show();
    _depura("END empresa::canales", 0);
    return 0;
}


/// Crea la ventana de tipos de IVA y la presenta en modo modal.
/** Es invocada desde la clase principal. */
void empresa::bloqueoFechas() {
    _depura("empresa::bloqueoFechas", 0);
    BbloqFecha *tip = new BbloqFecha(this, 0);
    m_pWorkspace->addWindow(tip);
    tip->show();
    _depura("END empresa::bloqueoFechas", 0);
}


/// Crea la ventana de tipos de IVA y la presenta en modo modal.
/** Es invocada desde la clase principal. */
int empresa::tiposIVA() {
    _depura("empresa::tiposIVA", 0);
    tipoivaview *tip = new tipoivaview(this, 0);
    m_pWorkspace->addWindow(tip);
    tip->show();
    _depura("END empresa::tiposIVA", 0);
    return 0;
}


/// Crea la ventana de Formas de Pago y la presenta en modo modal.
/** Es invocada desde la clase principal. */
int empresa::fPago() {
    _depura("empresa::fPago", 0);
    fpagoview *fp = new fpagoview(this, 0);
    m_pWorkspace->addWindow(fp);
    fp->show();
    _depura("END empresa::fPago", 0);
    return 0;
}


int empresa::cambioejercicio() {
    /// El ejercicio ha cambiado y recargamos el cursor de asientos del nuevo ejercicio.
    introapunts2->cargaasientos();
    introapunts2->boton_fin();
    return 0;
}


int empresa::nuevaempresa() {
    /// La creaci&oacute;n de una nueva empresa ha pasado al selector.
    return 0;
}


/// Crea la ventana de edici&oacute;n de cuentas y devuelve un puntero a esta.
cuentaview* empresa::newcuentaview() {
    cuentaview *nuevae = new cuentaview(this, 0);
    return nuevae;
}


int empresa::nuevacuenta() {
    cuentaview *nuevae = newcuentaview();
    m_pWorkspace->addWindow(nuevae);
    nuevae->show();
    return 0;
}


int empresa::muestraapuntes() {
    return 0;
}


int empresa::muestraapuntes1() {
    introapunts2->setWindowState(introapunts2->windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
    return 0;
}


int empresa::muestraasientos() {
    m_listasientos->setWindowState(m_listasientos->windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
    return 0;
}


int empresa::propiedadempresa() {
    _depura("empresa::propiedadempresa", 0);
    propiedadesempresa *nuevae = new propiedadesempresa(this, 0);
    m_pWorkspace->addWindow(nuevae);
    nuevae->show();
    _depura("empresa::propiedadempresa", 0);
    return 0;
}


int empresa::amortizaciones() {
    amortizacionesview *amors = new amortizacionesview(this, 0);
    m_pWorkspace->addWindow(amors);
    amors->show();
    return 0;
}


/// Esta funci&oacute;n llama a la pantalla de creacion, modificaci&oacute;n
/// de masas patrimoniales.
int empresa::mpatrimoniales() {
    mpatrimonialesview *nuevae = new mpatrimonialesview(0);
    nuevae->inicializa(this);
    nuevae->exec();
    delete nuevae;
    return 0;
}


/// Esta funci&oacute;n llama a la pantalla de creacion, modificaci&oacute;n
/// de masas patrimoniales.
int empresa::compbalance() {
    _depura("empresa::compbalance", 0);
    CAnualesView *nueva = new CAnualesView(this, 0);
    m_pWorkspace->addWindow(nueva);
    nueva->show();
    _depura("END empresa::compbalance", 0);
    return 0;
}


int empresa::libromayor() {
    extracto->setWindowState(extracto->windowState() &  ~Qt::WindowMinimized | Qt::WindowActive);
    return 0;
}


int empresa::boton_siguiente() {
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if (widget == extracto) {
        extracto->boton_siguiente();
    } else if (widget == introapunts2) {
        introapunts2->boton_siguiente();
    } // end if
    return 0;
}


int empresa::boton_anterior() {
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if (widget == extracto) {
        extracto->boton_anterior();
    } else if (widget == introapunts2) {
        introapunts2->boton_anterior();
    } // end if
    return 0;
}


int empresa::boton_guardar() {
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if (widget == diario) {
        diario->boton_guardar();
    } else {
        extracto->boton_guardar();
    } // end if
    return 0;
}


int empresa::boton_imprimir() {
    QWidget *widget = m_pWorkspace->activeWindow();
    if (widget == diario) {
        diario->boton_imprimir();
    } else if (widget == balance1) {
        balance1->boton_imprimir();
    } else if (widget == balance) {
        balance->on_mui_imprimir_clicked();
    } else if (widget == extracto) {
        extracto->boton_imprimir();
    } // end if
    return 0;
}


int empresa::boton_reload() {
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if (widget == diario) {
        diario->accept();
    } else if (widget == extracto) {
        extracto->accept();
    } else if (widget == balance) {
        balance->accept();
    } else if (widget == balance1) {
        balance1->accept();
    } // end if
    return 0;
}


int empresa::boton_primero() {
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if (widget == introapunts2) {
        introapunts2->boton_inicio();
    } else if (widget == diario) {}
    else if (widget == extracto) {
        extracto->boton_inicio();
    } // end if
    return 0;
}


int empresa::boton_ultimo() {
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if (widget == introapunts2) {
        introapunts2->boton_fin();
    } else if (widget == diario) {}
    else if (widget == extracto) {
        extracto->boton_fin();
    } // end if
    return 0;
}


int empresa::librodiario() {
    diario->setWindowState(diario->windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
    return 0;
}


int empresa::librobalance() {
    balance->setWindowState(balance->windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
    return 0;
}


int empresa::librobalancetree() {
    balance1->setWindowState(balance1->windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
    return 0;
}


void empresa::Abrirasientos() {
    cursor2 *cur = cargacursor("SELECT abreasientos()");
    delete cur;
    introapunts2->cargaasientos();
    introapunts2->boton_fin();
    introapunts2->show();
}


void empresa::Ordenarasientos() {
    QString query= "SELECT reordenaasientosall()";
    cursor2 * cur = cargacursor(query, "hola");
    delete cur;
    introapunts2->cargaasientos();
    introapunts2->boton_fin();
    introapunts2->show();
}


int empresa::registroiva() {
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
    int res = g_plugins->lanza("empresa_registroiva", this);
    return res;
}


int empresa::modelo347() {
    BModelo347 *dlg347 = new BModelo347(this, "0");
    dlg347->exec();
    delete dlg347;
    return 0;
}


/// Esta funci&oacute;n borra la empresa actual.
int empresa::borrarempresa() {
    /// El proceso de borrado se realiza desde las herramientas el selector.
    return 0;
}


/// Esta funci&oacute;n es la que se encarga de cerrar al empresa
/// y de crear un nuevo ejercicio para dicha empresa.
void empresa::nuevoejercicio() {}


void empresa::cierraempresa() {
    introapunts2->show();
    introapunts2->asiento_cierre();
}


void empresa::abreempresa() {
    introapunts2->show();
    introapunts2->asiento_apertura();
}


void empresa::regularizaempresa() {
    introapunts2->show();
    introapunts2->asiento_regularizacion();
}


void empresa::Filtro() {
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if (widget == introapunts2) {
        introapunts2->boton_filtrar();
    } // end if
}

/// Esta funci&oacute;n reemplaza en los apuntes todas las ocurrencias de una
/// determinada cuenta por otra cuenta.
void empresa::reemplazacuentaenasientos() {
    cambiactaview *ctac = new cambiactaview(this, 0, false);
    ctac->exec();
}


/// Esta funci&oacute;n recalcula los saldos parciales del plan contable.
void empresa::recalculasaldos() {
    ejecuta("SELECT recalculasaldos()");
}


/// Esta funci&oacute;n se dispara para poner en marcha la gesti&oacute;n de cobros y pagos.
int empresa::cobPag() {
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
    int res = g_plugins->lanza("empresa_cobPag", this);
    return res;
}


selectccosteview *empresa::getselccostes() {
    return selccostes;
}


selectcanalview *empresa::getselcanales() {
    return selcanales;
}


void empresa::centrocostedefecto() {
    selccostes->exec();
}


void empresa::canaldefecto() {
    selcanales->exec();
}

