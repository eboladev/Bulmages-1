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
#include <QDomDocument>


#include "empresa.h"
#include "abreempresaview.h"
#include "listcuentasview1.h"
#include "asiento1view.h"
#include "asientosview.h"
#include "amortizacionesview.h"
#include "canualesview.h"
//#include "balance1view.h"
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
#include "balanceview.h"
#include "listado347.h"
#include "paisview.h"
#include "bulmacont.h"

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif


/// SLOT que muestra la ventana de asientos.
void Empresa::s_asiento1() {
    _depura("Empresa::s_asiento1", 0);
    introapunts2->setWindowState(introapunts2->windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
    _depura("Empresa::s_asiento1", 0);
}


Empresa::Empresa(Bulmacont *bcont) : EmpresaBase() {
    _depura("Empresa::Empresa", 0);
    m_bulmacont = bcont;
    diario = NULL;
    extracto = NULL;
    balance = NULL;
    selccostes  = NULL;
    selcanales = NULL;
    introapunts2 = NULL;
    m_listasientos = NULL;
    nombre = "";
    _depura("END Empresa::Empresa", 0);
}


Empresa::~Empresa() {
    _depura("Empresa::~Empresa", 0);
    guardaConf();
    /// Borramos todas las ventanas.
    m_listventanas->vaciarCompleto();
    _depura("END Empresa::~Empresa", 0);
}


postgresiface2 *Empresa::bdempresa() {
    return (this);
}


BalanceView *Empresa::balanceempresa() {
    return balance;
}


extractoview1 *Empresa::extractoempresa() {
    return extracto;
}


DiarioView *Empresa::diarioempresa() {
    return diario;
}




Asiento1View *Empresa::intapuntsempresa() {
    return introapunts2;
}


Asiento1View *Empresa::intapuntsempresa2() {
    return introapunts2;
}


int Empresa::numdigitosempresa() {
    return numdigitos;
}


QString Empresa::nomuserempresa() {
    return nombre;
}


QString Empresa::nombreempresa() {
    return nombre;
}


/// Inicializa la clase con el nombre de la base de datos y con el workspace.
int Empresa::createMainWindows(Splash *splash) {
    _depura("Empresa::createMainWindows", 0);
    try {
    /// Calculamos el n&uacute;mero de d&iacute;gitos que tiene el nivel &uacute;ltimo
    /// de la Empresa.
    QString query = "SELECT length(valor) AS numdigitos FROM configuracion WHERE nombre = 'CodCuenta'";
    cursor2 *cursoraux1 = cargacursor(query);
    numdigitos = cursoraux1->valor("numdigitos").toInt();
    delete cursoraux1;
    if (extracto != NULL) {
        delete extracto;
        delete diario;
        delete balance;
        delete selccostes;
        delete selcanales;
        delete m_listasientos;
    } // end if

    /// Inicializamos los selectores de centros de coste y canales.
    selccostes = new SelectCCosteView(this, 0);
    selcanales = new selectcanalview(this, 0);

    /// Inicializamos las ventanas de uso generalizado.

    /// Establecemos el porcentaje del carga de informaci&oacute;n en las diferentes ventanas.
    /// pb = 0%
    splash->mensaje(QApplication::translate("Empresa", "Inicializando Extracto"));
    splash->setBarraProgreso(7);
    m_progressbar->setValue(0);
    extracto = new extractoview1(this, 0);
    m_pWorkspace->addWindow(extracto);

    /// pb = 20%
    splash->mensaje(QApplication::translate("Empresa", "Inicializando Diario"));
    splash->setBarraProgreso(20);
    m_progressbar->setValue(20);
    diario = new DiarioView(this, 0);
    m_pWorkspace->addWindow(diario);

    /// pb = 35%
    splash->mensaje(QApplication::translate("Empresa", "Inicializando Balance"));
    splash->setBarraProgreso(35);
    m_progressbar->setValue(35);
    balance = new BalanceView(this, 0);
    m_pWorkspace->addWindow(balance);

    /// pb = 50%
/*
    splash->mensaje(QApplication::translate("company", "Inicializando Balance Jerarquico"));
    splash->setBarraProgreso(50);
    m_progressbar->setValue(50);
    balance1 = new BalanceTreeView(this, 0);
    m_pWorkspace->addWindow(balance1);
*/

    /// pb = 60%
    splash->mensaje(QApplication::translate("company", "Inicializando Asientos"));
    splash->setBarraProgreso(60);
    m_progressbar->setValue(60);
    introapunts2 = new Asiento1View(this, 0);
    m_pWorkspace->addWindow(introapunts2);

    /// pb = 75%
    splash->mensaje(QApplication::translate("company", "Inicializando Asientos"));
    splash->setBarraProgreso(75);
    m_progressbar->setValue(75);
    m_listasientos = new AsientosView(this);
    m_listasientos->presentar();
    m_pWorkspace->addWindow(m_listasientos);

    /// pb = 90%
    splash->mensaje(QApplication::translate("company", "Inicializando Cuentas"));
    splash->setBarraProgreso(90);
    m_progressbar->setValue(90);
    m_listcuentas = new listcuentasview1(this, 0);
    m_listcuentas->inicializa();
    m_pWorkspace->addWindow(m_listcuentas);

    /// pb = 100%
    splash->mensaje(QApplication::translate("company", "Terminado"));
    splash->setBarraProgreso(100);
    m_progressbar->setValue(100);


    cargaConf();

    } catch(...) {
        _depura("Error al iniciar la clase company", 2);
    } // end try

    _depura("END Empresa::createMainWindows", 0);
    return(0);
}


/// Maximiza todas las ventanas que est&aacute;n en el workspace de la aplicaci&oacute;n.
/** Env&iacute;a un mensaje showMaximized a todos los objetos que deben ser maximizados. */
void Empresa::maximiza() {
    _depura("Empresa::maximiza", 0);
    diario->showMaximized();
    extracto->showMaximized();
    balance->showMaximized();
    _depura("END Empresa::maximiza", 0);
}


/// Esta funci&oacute;n abre la ventana que presenta las cuentas contables.
int Empresa::muestracuentas() {
    _depura("Empresa::muestracuentas", 0);
    m_listcuentas->show();
    _depura("END Empresa::muestracuentas", 0);
    return 0;
}


/// Esta funci&oacute;n presenta los centros de coste.
/** Luego recarga los comboboxes de todo el resto de pantallas para que se actualizen. */
int Empresa::ccostes() {
    _depura("Empresa::ccostes", 0);
    ccosteview *ccoste = new ccosteview(this, 0);
    m_pWorkspace->addWindow(ccoste);
    ccoste->show();
    _depura("END Empresa::ccostes", 0);
    return 0;
}


/// Esta funci&oacute;n presenta la pantalla de edicion de canales.
/** Luego recarga los comboboxes de todo el resto de pantallas para que se actualizen. */
int Empresa::canales() {
    _depura("Empresa::canales", 0);
    canalview *canal = new canalview(this, 0);
    m_pWorkspace->addWindow(canal);
    canal->show();
    _depura("END Empresa::canales", 0);
    return 0;
}


/// Crea la ventana de tipos de IVA y la presenta en modo modal.
/** Es invocada desde la clase principal. */
void Empresa::bloqueoFechas() {
    _depura("Empresa::bloqueoFechas", 0);
    BbloqFecha *tip = new BbloqFecha(this, 0);
    m_pWorkspace->addWindow(tip);
    tip->show();
    _depura("END Empresa::bloqueoFechas", 0);
}


/// Crea la ventana de tipos de IVA y la presenta en modo modal.
/** Es invocada desde la clase principal. */
int Empresa::tiposIVA() {
    _depura("Empresa::tiposIVA", 0);
    tipoivaview *tip = new tipoivaview(this, 0);
    m_pWorkspace->addWindow(tip);
    tip->show();
    _depura("END Empresa::tiposIVA", 0);
    return 0;
}


/// Crea la ventana de Formas de Pago y la presenta en modo modal.
/** Es invocada desde la clase principal. */
int Empresa::fPago() {
    _depura("Empresa::fPago", 0);
    fpagoview *fp = new fpagoview(this, 0);
    m_pWorkspace->addWindow(fp);
    fp->show();
    _depura("END Empresa::fPago", 0);
    return 0;
}


int Empresa::cambioejercicio() {
    _depura("Empresa::cambioejercicio", 0);
    /// El ejercicio ha cambiado y recargamos el cursor de asientos del nuevo ejercicio.
    introapunts2->cargaasientos();
    introapunts2->boton_fin();
    _depura("END Empresa::cambioejercicio", 0);
    return 0;
}


/// Crea la ventana de edici&oacute;n de cuentas y devuelve un puntero a esta.
CuentaView* Empresa::newcuentaview() {
    _depura("Empresa::newcuentaview", 0);
    CuentaView *nuevae = new CuentaView(this, 0);
    _depura("END Empresa::newcuentaview", 0);
    return nuevae;
}


int Empresa::nuevacuenta() {
    _depura("Empresa::nuevacuenta", 0);
    CuentaView *nuevae = newcuentaview();
    m_pWorkspace->addWindow(nuevae);
    nuevae->show();
    _depura("END Empresa::nuevacuenta", 0);
    return 0;
}


int Empresa::muestraapuntes1() {
    _depura("Empresa::muestraapuntes1", 0);
    introapunts2->setWindowState(introapunts2->windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
    _depura("END Empresa::muestraapuntes1", 0);
    return 0;
}


int Empresa::muestraasientos() {
    _depura("Empresa::muestraasientos", 0);
    m_listasientos->setWindowState(m_listasientos->windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
    _depura("END Empresa::muestraasientos", 0);
    return 0;
}


int Empresa::propiedadempresa() {
    _depura("Empresa::propiedadempresa", 0);
    propiedadesempresa *nuevae = new propiedadesempresa(this, 0);
    m_pWorkspace->addWindow(nuevae);
    nuevae->show();
    _depura("END Empresa::propiedadempresa", 0);
    return 0;
}


int Empresa::amortizaciones() {
    _depura("Empresa::amortizaciones", 0);
    AmortizacionesView *amors = new AmortizacionesView(this, 0);
    m_pWorkspace->addWindow(amors);
    amors->show();
    _depura("END Empresa::amortizaciones", 0);
    return 0;
}


/// Esta funci&oacute;n llama a la pantalla de creacion, modificaci&oacute;n
/// de masas patrimoniales.
int Empresa::mpatrimoniales() {
    _depura("Empresa::mpatrimoniales", 0);
    mpatrimonialesview *nuevae = new mpatrimonialesview(this, 0);
    nuevae->inicializa();
    nuevae->exec();
    delete nuevae;
    _depura("END Empresa::mpatrimoniales", 0);
    return 0;
}


/// Esta funci&oacute;n llama a la pantalla de creacion, modificaci&oacute;n
/// de masas patrimoniales.
int Empresa::compbalance() {
    _depura("Empresa::compbalance", 0);
    CAnualesView *nueva = new CAnualesView(this, 0);
    m_pWorkspace->addWindow(nueva);
    nueva->show();
    _depura("END Empresa::compbalance", 0);
    return 0;
}


int Empresa::libromayor() {
    _depura("Empresa::libromayor", 0);
    extracto->setWindowState(extracto->windowState() &  ~Qt::WindowMinimized | Qt::WindowActive);
    _depura("END Empresa::libromayor", 0);
    return 0;
}


int Empresa::boton_siguiente() {
    _depura("Empresa::boton_siguiente", 0);
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if (widget == extracto) {
        extracto->boton_siguiente();
    } else if (widget == introapunts2) {
        introapunts2->boton_siguiente();
    } // end if
    _depura("END Empresa::boton_siguiente", 0);
    return 0;
}


int Empresa::boton_anterior() {
    _depura("Empresa::boton_anterior", 0);
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if (widget == extracto) {
        extracto->boton_anterior();
    } else if (widget == introapunts2) {
        introapunts2->boton_anterior();
    } // end if
    _depura("END Empresa::boton_anterior", 0);
    return 0;
}


int Empresa::boton_guardar() {
    _depura("Empresa::boton_guardar", 0);
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if (widget == diario) {
        diario->boton_guardar();
    } else {
        extracto->boton_guardar();
    } // end if
    _depura("END Empresa::boton_guardar", 0);
    return 0;
}


int Empresa::boton_imprimir() {
    _depura("Empresa::boton_imprimir", 0);
    QWidget *widget = m_pWorkspace->activeWindow();
    if (widget == diario) {
        diario->boton_imprimir();
    } else if (widget == balance) {
        balance->on_mui_imprimir_clicked();
    } else if (widget == extracto) {
        extracto->boton_imprimir();
    } // end if
    _depura("END Empresa::boton_imprimir", 0);
    return 0;
}


int Empresa::boton_reload() {
    _depura("Empresa::boton_reload", 0);
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if (widget == diario) {
        diario->accept();
    } else if (widget == extracto) {
        extracto->accept();
    } else if (widget == balance) {
        balance->accept();
    } // end if
    _depura("END Empresa::boton_reload", 0);
    return 0;
}


int Empresa::boton_primero() {
    _depura("Empresa::boton_primero", 0);
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if (widget == introapunts2) {
        introapunts2->boton_inicio();
    } else if (widget == diario) {}
    else if (widget == extracto) {
        extracto->boton_inicio();
    } // end if
    _depura("END Empresa::boton_primero", 0);
    return 0;
}


int Empresa::boton_ultimo() {
    _depura("Empresa::boton_ultimo", 0);
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if (widget == introapunts2) {
        introapunts2->boton_fin();
    } else if (widget == diario) {}
    else if (widget == extracto) {
        extracto->boton_fin();
    } // end if
    _depura("END Empresa::boton_ultimo", 0);
    return 0;
}


int Empresa::librodiario() {
    _depura("Empresa::librodiario", 0);
    diario->setWindowState(diario->windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
    _depura("END Empresa::librodiario", 0);
    return 0;
}


int Empresa::librobalance() {
    _depura("Empresa::librobalance", 0);
    balance->setWindowState(balance->windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
    _depura("END Empresa::librobalance", 0);
    return 0;
}


void Empresa::Abrirasientos() {
    _depura("Empresa::Abrirasientos", 0);
    cursor2 *cur = NULL;
    try {
        cur = cargacursor("SELECT abreasientos()");
        //introapunts2->cargaasientos();
        //introapunts2->boton_fin();
        //introapunts2->show();
        g_main->statusBar()->showMessage(QObject::tr("Se han espaciado los asientos"), 2000);
    } catch (...) {
        mensajeError("Ha habido un error al espaciar los asientos");
    }
    delete cur;
    _depura("END Empresa::Abrirasientos", 0);
}


void Empresa::Ordenarasientos() {
    _depura("Empresa::Ordenarasientos", 10);
    QString query= "SELECT reordenaasientosall()";
    cursor2 *cur = NULL;
    try {
        cur = cargacursor(query, "hola");
        //introapunts2->cargaasientos();
        //introapunts2->boton_fin();
        //introapunts2->show();
        g_main->statusBar()->showMessage(QObject::tr("Se han ordenado los asientos"), 2000);
    } catch (...) {
        mensajeError("Ha habido un error al ordenar los asientos");
    }
    delete cur;
    _depura("END Empresa::Ordenarasientos", 10);
}


int Empresa::registroiva() {
    _depura("Empresa::registroiva", 0);
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
    int res = g_plugins->lanza("empresa_registroiva", this);
    _depura("END Empresa::registroiva", 0);
    return res;
}


int Empresa::listado347() {
    _depura("Empresa::modelo347", 0);
    Listado347 *dlg347 = new Listado347(this, "0");
    dlg347->exec();
    delete dlg347;
    _depura("END Empresa::modelo347", 0);
    return 0;
}


void Empresa::cierraempresa() {
    _depura("Empresa::cierraempresa", 0);
    introapunts2->show();
    introapunts2->asiento_cierre();
    _depura("END Empresa::cierraempresa", 0);
}


void Empresa::abreempresa() {
    _depura("Empresa::abreempresa", 0);
    introapunts2->show();
    introapunts2->asiento_apertura();
    _depura("END Empresa::abreempresa", 0);
}


void Empresa::regularizaempresa() {
    _depura("Empresa::regularizaempresa", 0);
    QString hoy = QDate::currentDate().toString("dd/MM/yyyy");
    QString finicial = "01/01/"+hoy.right(4);
    regularizaempresa(finicial, hoy); // TODO: Habria que preguntar con que fecha realizar la regularizacion
    _depura("END Empresa::regularizaempresa", 0);
}

void Empresa::regularizaempresa(QString finicial, QString ffinal) {
    _depura("Empresa::regularizaempresa", 0);
    introapunts2->show();
    introapunts2->asiento_regularizacion(finicial, ffinal);
    _depura("END Empresa::regularizaempresa", 0);
}

void Empresa::Filtro() {
    _depura("Empresa::Filtro", 0);
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if (widget == introapunts2) {
        introapunts2->boton_filtrar();
    } // end if
    _depura("END Empresa::Filtro", 0);
}

/// Esta funci&oacute;n reemplaza en los apuntes todas las ocurrencias de una
/// determinada cuenta por otra cuenta.
void Empresa::reemplazacuentaenasientos() {
    _depura("Empresa::reemplazacuentaenasientos", 0);
    CambiaCtaView *ctac = new CambiaCtaView(this, 0, false);
    ctac->exec();
    _depura("END Empresa::reemplazacuentaenasientos", 0);
}


/// Esta funci&oacute;n recalcula los saldos parciales del plan contable.
void Empresa::recalculasaldos() {
    _depura("Empresa::recalculasaldos", 0);
    ejecuta("SELECT recalculasaldos()");
    _depura("END Empresa::recalculasaldos", 0);
}


/// Esta funci&oacute;n se dispara para poner en marcha la gesti&oacute;n de cobros y pagos.
int Empresa::cobPag() {
    _depura("Empresa::cobPag", 0);
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
    int res = g_plugins->lanza("empresa_cobPag", this);
    _depura("END Empresa::cobPag", 0);
    return res;
}


SelectCCosteView *Empresa::getselccostes() {
    _depura("Empresa::getselccostes", 0);
    _depura("END Empresa::getselccostes", 0);
    return selccostes;
}


selectcanalview *Empresa::getselcanales() {
    _depura("Empresa::getselcanales", 0);
    _depura("END Empresa::getselcanales", 0);
    return selcanales;
}


void Empresa::centrocostedefecto() {
    _depura("Empresa::centrocostedefecto", 0);
    selccostes->exec();
    _depura("END Empresa::centrocostedefecto", 0);
}


void Empresa::canaldefecto() {
    _depura("Empresa::canaldefecto", 0);
    selcanales->exec();
    _depura("END Empresa::canaldefecto", 0);
}


void Empresa::muestrapaises() {
    _depura("Empresa::muestrapaises", 0);
    PaisView *pais = new PaisView(this, 0);
    m_pWorkspace->addWindow(pais);
    pais->show();
    _depura("END Empresa::muestrapaises", 0);
}

/*
Empresa* Empresa::empresaBase() {
        _depura("Empresa::empresaBase", 0);
        _depura("END Empresa::empresaBase", 0);
	return this;
}
*/

/// Guarda la configuracion de programa para poder recuperar algunas cosas de presentacion.
void Empresa::guardaConf() {
    QFile file(confpr->valor(CONF_DIR_USER) + "bulmacont_" + nameDB() + ".cfn");
    /// Guardado del orden y de configuraciones varias.
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << "<CONFIG>\n";
        stream << "\t<PRINCIPAL>\n";
        stream << "\t\t\t<X>" + QString::number(m_bulmacont->x()) + "</X>\n";
        stream << "\t\t\t<Y>" + QString::number(m_bulmacont->y()) + "</Y>\n";
        stream << "\t\t\t<WIDTH>" + QString::number(m_bulmacont->width()) + "</WIDTH>\n";
        stream << "\t\t\t<HEIGHT>" + QString::number(m_bulmacont->height()) + "</HEIGHT>\n";
        stream << "\t\t\t<INDEXADOR>" + (m_bulmacont->actionIndexador->isChecked() ? QString("TRUE") : QString("FALSE")) + "</INDEXADOR>\n";
        stream << "\t</PRINCIPAL>\n";


        for (int i = 0; i < m_listventanas->numVentanas(); i++) {
		QObject *obj = m_listventanas->ventana(i);
                QWidget *wid = (QWidget *) obj;
		stream << "\t<VENTANA>\n";
		stream << "\t\t<VNAME>" + obj->objectName() + "</VNAME>\n";
		stream << "\t\t<VX>" + QString::number(wid->parentWidget()->x()) + "</VX>\n";
		stream << "\t\t<VY>" + QString::number(wid->parentWidget()->y()) + "</VY>\n";
		stream << "\t\t<VWIDTH>" + QString::number(wid->width()) + "</VWIDTH>\n";
		stream << "\t\t<VHEIGHT>" + QString::number(wid->height()) + "</VHEIGHT>\n";
		stream << "\t\t<VVISIBLE>" + (wid->isVisible() ? QString("TRUE") : QString("FALSE")) + "</VVISIBLE>\n";
		stream << "\t\t<VMAXIMIZED>" + (wid->isMaximized() ? QString("TRUE") : QString("FALSE")) + "</VMAXIMIZED>\n";
		stream << "\t\t<VACTIVEWINDOW>" + (m_bulmacont->workspace()->activeWindow() == wid ? QString("TRUE") : QString("FALSE")) + "</VACTIVEWINDOW>";
		stream << "\t</VENTANA>\n";
	} // end for

        stream << "</CONFIG>\n";
        file.close();
    } // end if
}

/// Guarda la configuracion de programa para poder recuperar algunas cosas de presentacion.
void Empresa::cargaConf() {
    QFile file(confpr->valor(CONF_DIR_USER) + "bulmacont_" + nameDB() + ".cfn");
    QDomDocument doc("mydocument");
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = doc.documentElement();
    QDomElement principal = docElem.firstChildElement("PRINCIPAL");
    /// Cogemos la coordenada X
    QString nx = principal.firstChildElement("X").toElement().text();

    /// Cogemos la coordenada Y
    QString ny = principal.firstChildElement("Y").toElement().text();

    /// Cogemos el ancho
    QString nwidth = principal.firstChildElement("WIDTH").toElement().text();

    /// Cogemos el alto
    QString nheight = principal.firstChildElement("HEIGHT").toElement().text();

    /// Establecemos la geometria de la ventana principal.
    m_bulmacont->setGeometry(nx.toInt(), ny.toInt(), nwidth.toInt(), nheight.toInt());

    /// Cogemos el indexador
    QString indexador = principal.firstChildElement("INDEXADOR").toElement().text();
    if (indexador == "TRUE") {
	s_indexadorCambiaEstado(TRUE);
	m_bulmacont->actionIndexador->setChecked(TRUE);
    } else {
	s_indexadorCambiaEstado(FALSE);
	m_bulmacont->actionIndexador->setChecked(FALSE);
    } // end if

    /// Tratamos cada ventana
   QWidget *activewindow = NULL;
   QDomNodeList nodos = docElem.elementsByTagName("VENTANA");
        for (int i = 0; i < nodos.count(); i++) {
            QDomNode ventana = nodos.item(i);
            QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
            if( !e1.isNull() ) { /// the node was really an element.
		     QString vname = e1.firstChildElement("VNAME").toElement().text();
			for (int j = 0; j < m_listventanas->numVentanas(); j++) {
				QObject *obj = m_listventanas->ventana(j);
				QWidget *wid = (QWidget *) obj;
				if (obj->objectName() == vname) {
					QString vx = e1.firstChildElement("VX").toElement().text();
					QString vy = e1.firstChildElement("VY").toElement().text();
					QString vwidth = e1.firstChildElement("VWIDTH").toElement().text();
					QString vheight = e1.firstChildElement("VHEIGHT").toElement().text();
					QString vvisible = e1.firstChildElement("VVISIBLE").toElement().text();
					QString vmaximized = e1.firstChildElement("VMAXIMIZED").toElement().text();
					QString vactivewindow = e1.firstChildElement("VACTIVEWINDOW").toElement().text();
					/// Establecemos la geometria de la ventana principal.
					wid->resize(vwidth.toInt(), vheight.toInt());
					wid->parentWidget()->move(vx.toInt(), vy.toInt());
					if ( vvisible == "TRUE") {
						wid->showNormal();
					} else {
						wid->hide();
					} // end if
					if ( vmaximized == "TRUE") {
						wid->showMaximized();
					}
					if ( vactivewindow == "TRUE") {
						activewindow = wid;
					}
				} // end if
			} // end for
            } // end if
        } // end for
	/// Si hay una ventana activa se pone como activa.
	if (activewindow) 
		m_bulmacont->workspace()->setActiveWindow(activewindow);
}


