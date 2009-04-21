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
#include <QInputDialog>

#include "bccompany.h"
#include "blcompanydialog.h"
#include "bcplancontablelistview.h"
#include "asiento1view.h"
#include "asientosview.h"
#include "bcamortizacionlistview.h"
#include "bccuentasanualesview.h"
#include "bccuentaview.h"
#include "bccanalview.h"
#include "bccentrocosteview.h"
#include "bcmasapatrimoniallistview.h"
#include "bcconfiguracionview.h"
#include "bccambiacuentaview.h"
#include "bctipoivaview.h"
#include "bcformapagoview.h"
#include "bccentrocosteseleccionarview.h"
#include "bccanalseleccionarview.h"
#include "blplugins.h"
#include "bcbloqueafechaview.h"
#include "bcdiarioview.h"
#include "bcmodelo347listview.h"
#include "blcountryview.h"
#include "bcbulmacont.h"
#include "bcextractoview.h"

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif


/// SLOT que muestra la ventana de asientos.
/**
**/
void BcCompany ::s_asiento1()
{
    _depura ( "BcCompany ::s_asiento1", 0 );
    introapunts2->setWindowState ( introapunts2->windowState() & ~Qt::WindowMinimized | Qt::WindowActive );
    _depura ( "BcCompany ::s_asiento1", 0 );
}


///
/**
\param bcont
**/
BcCompany ::BcCompany ( BcBulmaCont *bcont ) : BlMainCompany()
{
    _depura ( "BcCompany ::BcCompany ", 0 );
    m_bulmacont = bcont;
    diario = NULL;
    extracto = NULL;
    selccostes  = NULL;
    selcanales = NULL;
    introapunts2 = NULL;
    m_listasientos = NULL;
    nombre = "";
    _depura ( "END BcCompany ::BcCompany", 0 );
}


///
/**
**/
BcCompany::~BcCompany()
{
    _depura ( "BcCompany::~BcCompany", 0 );
    guardaConf();
    /// Borramos todas las ventanas.
    m_listventanas->vaciarCompleto();
    _depura ( "END BcCompany::~BcCompany", 0 );
}


///
/**
\return
**/
BlPostgreSqlClient *BcCompany::bdempresa()
{
    return ( this );
}



///
/**
\return
**/
BcExtractoView *BcCompany::extractoempresa()
{
    return extracto;
}


///
/**
\return
**/
BcDiarioView *BcCompany::diarioempresa()
{
    return diario;
}


///
/**
\return
**/
Asiento1View *BcCompany::intapuntsempresa()
{
    return introapunts2;
}


///
/**
\return
**/
Asiento1View *BcCompany::intapuntsempresa2()
{
    return introapunts2;
}


///
/**
\return
**/
int BcCompany::numdigitosempresa()
{
    return numdigitos;
}


///
/**
\return
**/
QString BcCompany::nomuserempresa()
{
    return nombre;
}


///
/**
\return
**/
QString BcCompany::nombreempresa()
{
    return nombre;
}


/// Inicializa la clase con el nombre de la base de datos y con el workspace.
/**
\param splash
\return
**/
int BcCompany::createMainWindows ( BlSplashScreen *splash )
{
    _depura ( "BcCompany::createMainWindows", 0 );
    try {
        /// Calculamos el n&uacute;mero de d&iacute;gitos que tiene el nivel &uacute;ltimo
        /// de la Empresa.
        QString query = "SELECT length(valor) AS numdigitos FROM configuracion WHERE nombre = 'CodCuenta'";
        BlDbRecordSet *cursoraux1 = loadQuery ( query );
        numdigitos = cursoraux1->valor ( "numdigitos" ).toInt();
        delete cursoraux1;
        if ( extracto != NULL ) {
            delete extracto;
            delete diario;
            delete selccostes;
            delete selcanales;
            delete m_listasientos;
        } // end if

        /// Inicializamos los selectores de centros de coste y canales.
        selccostes = new BcCentroCosteSeleccionarView ( this, 0 );
        selcanales = new BcCanalSeleccionarView ( this, 0 );

        /// Inicializamos las ventanas de uso generalizado.

        /// Establecemos el porcentaje del carga de informaci&oacute;n en las diferentes ventanas.
        /// pb = 0%
        splash->mensaje ( _ ( "Inicializando extracto" ) );
        splash->setBarraProgreso ( 7 );
        m_progressbar->setValue ( 0 );
        extracto = new BcExtractoView ( this, 0 );
        m_pWorkspace->addWindow ( extracto );

        /// pb = 20%
        splash->mensaje ( _ ( "Inicializando diario" ) );
        splash->setBarraProgreso ( 20 );
        m_progressbar->setValue ( 20 );
        diario = new BcDiarioView ( this, 0 );
        m_pWorkspace->addWindow ( diario );
        /*
                /// pb = 35%
                splash->mensaje ( _( "Inicializando balance" ) );
                splash->setBarraProgreso ( 35 );
                m_progressbar->setValue ( 35 );
                balance = new BalanceView ( this, 0 );
                m_pWorkspace->addWindow ( balance );
        */
        /// pb = 50%
        /*
                    splash->mensaje( _( "Inicializando balance jerarquico"));
                    splash->setBarraProgreso(50);
                    m_progressbar->setValue(50);
                    balance1 = new BalanceTreeView(this, 0);
                    m_pWorkspace->addWindow(balance1);
        */

        /// pb = 60%
        splash->mensaje ( _ ( "Inicializando asientos" ) );
        splash->setBarraProgreso ( 60 );
        m_progressbar->setValue ( 60 );
        introapunts2 = new Asiento1View ( this, 0 );
        m_pWorkspace->addWindow ( introapunts2 );

        /// pb = 75%
        splash->mensaje ( _ ( "Inicializando asientos" ) );
        splash->setBarraProgreso ( 75 );
        m_progressbar->setValue ( 75 );
        m_listasientos = new AsientosView ( this );
        m_listasientos->presentar();
        m_pWorkspace->addWindow ( m_listasientos );

        /// pb = 90%
        splash->mensaje ( _ ( "Inicializando cuentas" ) );
        splash->setBarraProgreso ( 90 );
        m_progressbar->setValue ( 90 );
        m_listcuentas = new BcPlanContableListView ( this, 0 );
        m_listcuentas->inicializa();
        m_pWorkspace->addWindow ( m_listcuentas );

        /// pb = 100%
        splash->mensaje ( _ ( "Terminado" ) );
        splash->setBarraProgreso ( 100 );
        m_progressbar->setValue ( 100 );

        /// Disparamos los plugins.
        int res = g_plugins->lanza ( "BcCompany_createMainWindows_Post", this );
        if ( res != 0 ) {
            return 0;
        } // end if

        cargaConf();

    } catch ( ... ) {
        _depura ( "Error al iniciar la clase company", 2 );
    } // end try

    _depura ( "END BcCompany::createMainWindows", 0 );
    return ( 0 );
}


/// Maximiza todas las ventanas que est&aacute;n en el workspace de la aplicaci&oacute;n.
/** Env&iacute;a un mensaje showMaximized a todos los objetos que deben ser maximizados. */
/**
**/
void BcCompany::maximiza()
{
    _depura ( "BcCompany::maximiza", 0 );
    diario->showMaximized();
    extracto->showMaximized();
    _depura ( "END BcCompany::maximiza", 0 );
}


/// Esta funci&oacute;n abre la ventana que presenta las cuentas contables.
/**
\return
**/
int BcCompany::muestracuentas()
{
    _depura ( "BcCompany::muestracuentas", 0 );
    m_listcuentas->setWindowState ( introapunts2->windowState() & ~Qt::WindowMinimized | Qt::WindowActive );
    _depura ( "END BcCompany::muestracuentas", 0 );
    return 0;
}


/// Esta funci&oacute;n presenta los centros de coste.
/** Luego recarga los comboboxes de todo el resto de pantallas para que se actualizen. */
/**
\return
**/
int BcCompany::ccostes()
{
    _depura ( "BcCompany::ccostes", 0 );
    BcCentroCosteView *ccoste = new BcCentroCosteView ( this, 0 );
    m_pWorkspace->addWindow ( ccoste );
    ccoste->show();
    _depura ( "END BcCompany::ccostes", 0 );
    return 0;
}


/// Esta funci&oacute;n presenta la pantalla de edicion de canales.
/** Luego recarga los comboboxes de todo el resto de pantallas para que se actualizen. */
/**
\return
**/
int BcCompany::canales()
{
    _depura ( "BcCompany::canales", 0 );
    BcCanalView *canal = new BcCanalView ( this, 0 );
    m_pWorkspace->addWindow ( canal );
    canal->show();
    _depura ( "END BcCompany::canales", 0 );
    return 0;
}


/// Crea la ventana de tipos de IVA y la presenta en modo modal.
/** Es invocada desde la clase principal. */
/**
**/
void BcCompany::bloqueoFechas()
{
    _depura ( "BcCompany::bloqueoFechas", 0 );
    BcBloqueaFechaView *tip = new BcBloqueaFechaView ( this, 0 );
    m_pWorkspace->addWindow ( tip );
    tip->show();
    _depura ( "END BcCompany::bloqueoFechas", 0 );
}


/// Crea la ventana de tipos de IVA y la presenta en modo modal.
/** Es invocada desde la clase principal. */
/**
\return
**/
int BcCompany::tiposIVA()
{
    _depura ( "BcCompany::tiposIVA", 0 );
    BcTipoIVAView *tip = new BcTipoIVAView ( this, 0 );
    m_pWorkspace->addWindow ( tip );
    tip->show();
    _depura ( "END BcCompany::tiposIVA", 0 );
    return 0;
}


/// Crea la ventana de Formas de Pago y la presenta en modo modal.
/** Es invocada desde la clase principal. */
/**
\return
**/
int BcCompany::fPago()
{
    _depura ( "BcCompany::fPago", 0 );
    BcFormaPagoView *fp = new BcFormaPagoView ( this, 0 );
    m_pWorkspace->addWindow ( fp );
    fp->show();
    _depura ( "END BcCompany::fPago", 0 );
    return 0;
}


///
/**
\return
**/
int BcCompany::cambioejercicio()
{
    _depura ( "BcCompany::cambioejercicio", 0 );
    /// El ejercicio ha cambiado y recargamos el cursor de asientos del nuevo ejercicio.
    introapunts2->cargaasientos();
    introapunts2->boton_fin();
    _depura ( "END BcCompany::cambioejercicio", 0 );
    return 0;
}


/// Crea la ventana de edici&oacute;n de cuentas y devuelve un puntero a esta.
/**
\return
**/
BcCuentaView* BcCompany::newcuentaview()
{
    _depura ( "BcCompany::newcuentaview", 0 );
    BcCuentaView *nuevae = new BcCuentaView ( this, 0 );
    _depura ( "END BcCompany::newcuentaview", 0 );
    return nuevae;
}


///
/**
\return
**/
int BcCompany::nuevacuenta()
{
    _depura ( "BcCompany::nuevacuenta", 0 );
    BcCuentaView *nuevae = newcuentaview();
    m_pWorkspace->addWindow ( nuevae );
    nuevae->show();
    _depura ( "END BcCompany::nuevacuenta", 0 );
    return 0;
}


///
/**
\return
**/
int BcCompany::muestraapuntes1()
{
    _depura ( "BcCompany::muestraapuntes1", 0 );
    introapunts2->setWindowState ( introapunts2->windowState() & ~Qt::WindowMinimized | Qt::WindowActive );
    _depura ( "END BcCompany::muestraapuntes1", 0 );
    return 0;
}


///
/**
\return
**/
int BcCompany::muestraasientos()
{
    _depura ( "BcCompany::muestraasientos", 0 );
    m_listasientos->setWindowState ( m_listasientos->windowState() & ~Qt::WindowMinimized | Qt::WindowActive );
    _depura ( "END BcCompany::muestraasientos", 0 );
    return 0;
}


///
/**
\return
**/
int BcCompany::propiedadempresa()
{
    _depura ( "BcCompany::propiedadempresa", 0 );
    BcConfiguracionView *nuevae = new BcConfiguracionView ( this, 0 );
    m_pWorkspace->addWindow ( nuevae );
    nuevae->show();
    _depura ( "END BcCompany::propiedadempresa", 0 );
    return 0;
}


///
/**
\return
**/
int BcCompany::amortizaciones()
{
    _depura ( "BcCompany::amortizaciones", 0 );
    BcAmortizacionListView *amors = new BcAmortizacionListView ( this, 0 );
    m_pWorkspace->addWindow ( amors );
    amors->show();
    _depura ( "END BcCompany::amortizaciones", 0 );
    return 0;
}


/// Esta funci&oacute;n llama a la pantalla de creacion, modificaci&oacute;n
/// de masas patrimoniales.
/**
\return
**/
int BcCompany::mpatrimoniales()
{
    _depura ( "BcCompany::mpatrimoniales", 0 );
    BcMasaPatrimonialListView *nuevae = new BcMasaPatrimonialListView ( this, 0 );
    nuevae->inicializa();
    nuevae->exec();
    delete nuevae;
    _depura ( "END BcCompany::mpatrimoniales", 0 );
    return 0;
}


/// Esta funci&oacute;n llama a la pantalla de creacion, modificaci&oacute;n
/// de masas patrimoniales.
/**
\return
**/
int BcCompany::compbalance()
{
    _depura ( "BcCompany::compbalance", 0 );
    BcCuentasAnualesView *nueva = new BcCuentasAnualesView ( this, 0 );
    m_pWorkspace->addWindow ( nueva );
    nueva->show();
    _depura ( "END BcCompany::compbalance", 0 );
    return 0;
}


///
/**
\return
**/
int BcCompany::libromayor()
{
    _depura ( "BcCompany::libromayor", 0 );
    extracto->setWindowState ( extracto->windowState() &  ~Qt::WindowMinimized | Qt::WindowActive );
    _depura ( "END BcCompany::libromayor", 0 );
    return 0;
}


///
/**
\return
**/
int BcCompany::boton_siguiente()
{
    _depura ( "BcCompany::boton_siguiente", 0 );
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if ( widget == extracto ) {
        extracto->boton_siguiente();
    } else if ( widget == introapunts2 ) {
        introapunts2->boton_siguiente();
    } // end if
    _depura ( "END BcCompany::boton_siguiente", 0 );
    return 0;
}


///
/**
\return
**/
int BcCompany::boton_anterior()
{
    _depura ( "BcCompany::boton_anterior", 0 );
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if ( widget == extracto ) {
        extracto->boton_anterior();
    } else if ( widget == introapunts2 ) {
        introapunts2->boton_anterior();
    } // end if
    _depura ( "END BcCompany::boton_anterior", 0 );
    return 0;
}


///
/**
\return
**/
int BcCompany::boton_guardar()
{
    _depura ( "BcCompany::boton_guardar", 0 );
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if ( widget == diario ) {
        diario->boton_guardar();
    } else {
        extracto->boton_guardar();
    } // end if
    _depura ( "END BcCompany::boton_guardar", 0 );
    return 0;
}


///
/**
\return
**/
int BcCompany::boton_imprimir()
{
    _depura ( "BcCompany::boton_imprimir", 0 );
    QWidget *widget = m_pWorkspace->activeWindow();
    if ( widget == diario ) {
        diario->boton_imprimir();
    } else if ( widget == extracto ) {
        extracto->boton_imprimir();
    } // end if
    _depura ( "END BcCompany::boton_imprimir", 0 );
    return 0;
}


///
/**
\return
**/
int BcCompany::boton_reload()
{
    _depura ( "BcCompany::boton_reload", 0 );
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if ( widget == diario ) {
        diario->accept();
    } else if ( widget == extracto ) {
        extracto->accept();
    } // end if
    _depura ( "END BcCompany::boton_reload", 0 );
    return 0;
}


///
/**
\return
**/
int BcCompany::boton_primero()
{
    _depura ( "BcCompany::boton_primero", 0 );
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if ( widget == introapunts2 ) {
        introapunts2->boton_inicio();
    } else if ( widget == diario ) {} else if ( widget == extracto ) {
        extracto->boton_inicio();
    } // end if
    _depura ( "END BcCompany::boton_primero", 0 );
    return 0;
}


///
/**
\param
**/
int BcCompany::boton_ultimo()
{
    _depura ( "BcCompany::boton_ultimo", 0 );
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if ( widget == introapunts2 ) {
        introapunts2->boton_fin();
    } else if ( widget == diario ) {} else if ( widget == extracto ) {
        extracto->boton_fin();
    } // end if
    _depura ( "END BcCompany::boton_ultimo", 0 );
    return 0;
}


///
/**
\return
**/
int BcCompany::librodiario()
{
    _depura ( "BcCompany::librodiario", 0 );
    diario->setWindowState ( diario->windowState() & ~Qt::WindowMinimized | Qt::WindowActive );
    _depura ( "END BcCompany::librodiario", 0 );
    return 0;
}


///
/**
\return
**/
/*
int BcCompany::librobalance()
{
    _depura ( "BcCompany::librobalance", 0 );
    balance->setWindowState ( balance->windowState() & ~Qt::WindowMinimized | Qt::WindowActive );
    _depura ( "END BcCompany::librobalance", 0 );
    return 0;
}
*/


///
/**
**/
void BcCompany::abrirAsientos()
{
    _depura ( "BcCompany::Abrirasientos", 0 );
    BlDbRecordSet *cur = NULL;
    try {
        cur = loadQuery ( "SELECT abreasientos()" );
        g_main->statusBar() ->showMessage ( _ ( "Se han espaciado los asientos" ), 2000 );
        delete cur;
        _depura ( "END BcCompany::Abrirasientos", 0 );
    } catch ( ... ) {
        mensajeError ( "Ha habido un error al espaciar los asientos" );
        if ( cur ) delete cur;
        return;
    } // end try
}


///
/**
**/
void BcCompany::cerrarAsientos()
{
    _depura ( "BcCompany::cerrarAsientos", 0 );
    BlDbRecordSet *cur = NULL;
    try {
        cur = loadQuery ( "SELECT cierraasiento(idasiento) FROM asiento" );
        g_main->statusBar() ->showMessage ( _ ( "Se han cerrado los asientos" ), 2000 );
        delete cur;
        _depura ( "END BcCompany::cerrarAsientos", 0 );
    } catch ( ... ) {
        mensajeError ( "Ha habido un error al cerrar los asientos" );
        if ( cur ) delete cur;
        return;
    } // end try
}


/**
**/
void BcCompany::Ordenarasientos()
{
    _depura ( "BcCompany::Ordenarasientos", 10 );
    QString query = "SELECT reordenaasientosall()";
    BlDbRecordSet *cur = NULL;
    try {
        cur = loadQuery ( query );
        g_main->statusBar() ->showMessage ( _ ( "Se han ordenado los asientos" ), 2000 );
        delete cur;
        _depura ( "END BcCompany::Ordenarasientos", 10 );
    } catch ( ... ) {
        mensajeError ( "Ha habido un error al ordenar los asientos" );
        if ( cur ) delete cur;
        return;
    }
}


///
/**
\return
**/
int BcCompany::listado347()
{
    _depura ( "BcCompany::modelo347", 0 );
    BcModelo347ListView *dlg347 = new BcModelo347ListView ( this, "0" );
    dlg347->exec();
    delete dlg347;
    _depura ( "END BcCompany::modelo347", 0 );
    return 0;
}


///
/**
**/
void BcCompany::cierraempresa()
{
    _depura ( "BcCompany::cierraempresa", 0 );
    QString hoy = QDate::currentDate().toString ( "dd/MM/yyyy" );
    QString finicial = "01/01/" + hoy.right ( 4 );

    bool ok;
    QString text = QInputDialog::getText ( 0,
                                           _ ( "Fecha inicial" ), _ ( "Fecha inicial ejercicio:" ), QLineEdit::Normal,
                                           finicial, &ok );
    if ( ok && !text.isEmpty() ) {
        finicial = text;
    } else {
        return;
    } // end if

    QString text1 = QInputDialog::getText ( 0,
                                            _ ( "Fecha final" ), _ ( "Fecha final ejercicio:" ), QLineEdit::Normal,
                                            hoy, &ok );
    if ( ok && !text1.isEmpty() ) {
        hoy = text1;
    } else {
        return;
    } // end if

    introapunts2->show();
    introapunts2->asiento_cierre ( finicial, hoy );
    _depura ( "END BcCompany::cierraempresa", 0 );
}


///
/**
**/
void BcCompany::abreempresa()
{
    _depura ( "BcCompany::abreempresa", 0 );
    QString hoy = QDate::currentDate().toString ( "dd/MM/yyyy" );
    QString finicial = "01/01/" + hoy.right ( 4 );

    bool ok;
    QString text1 = QInputDialog::getText ( 0,
                                            _ ( "Fecha" ), _ ( "Fecha:" ), QLineEdit::Normal,
                                            hoy, &ok );
    if ( ok && !text1.isEmpty() ) {
        hoy = text1;
    } else {
        return;
    } // end if

    introapunts2->show();
    introapunts2->asiento_apertura ( hoy );
    _depura ( "END BcCompany::abreempresa", 0 );
}


///
/**
**/
void BcCompany::regularizaempresa()
{
    _depura ( "BcCompany::regularizaempresa", 0, "Sin parametros" );
    QString hoy = QDate::currentDate().toString ( "dd/MM/yyyy" );
    QString finicial = "01/01/" + hoy.right ( 4 );

    bool ok;
    QString text = QInputDialog::getText ( 0,
                                           _ ( "Fecha inicial" ), _ ( "Fecha inicial regularizacion:" ), QLineEdit::Normal,
                                           finicial, &ok );
    if ( ok && !text.isEmpty() ) {
        finicial = text;
    } else {
        return;
    } // end if

    QString text1 = QInputDialog::getText ( 0,
                                            _ ( "Fecha final" ), _ ( "Fecha final regularizacion:" ), QLineEdit::Normal,
                                            hoy, &ok );
    if ( ok && !text1.isEmpty() ) {
        hoy = text1;
    } else {
        return;
    } // end if

    regularizaempresa ( finicial, hoy );
    _depura ( "END BcCompany::regularizaempresa", 0 );
}


///
/**
\param finicial
\param ffinal
**/
void BcCompany::regularizaempresa ( QString finicial, QString ffinal )
{
    _depura ( "BcCompany::regularizaempresa", 0, finicial + "--" + ffinal );
    introapunts2->show();
    introapunts2->asiento_regularizacion ( finicial, ffinal );
    _depura ( "END BcCompany::regularizaempresa", 0 );
}

///
/**
**/
void BcCompany::Filtro()
{
    _depura ( "BcCompany::Filtro", 0 );
    QWidget *widget;
    widget = m_pWorkspace->activeWindow();
    if ( widget == introapunts2 ) {
        introapunts2->boton_filtrar();
    } // end if
    _depura ( "END BcCompany::Filtro", 0 );
}

/// Esta funci&oacute;n reemplaza en los apuntes todas las ocurrencias de una
/// determinada cuenta por otra cuenta.
/**
**/
void BcCompany::reemplazacuentaenasientos()
{
    _depura ( "BcCompany::reemplazacuentaenasientos", 0 );
    BcCambiaCuentaView *ctac = new BcCambiaCuentaView ( this, 0, false );
    ctac->exec();
    _depura ( "END BcCompany::reemplazacuentaenasientos", 0 );
}


/// Esta funci&oacute;n recalcula los saldos parciales del plan contable.
/**
**/
void BcCompany::recalculasaldos()
{
    _depura ( "BcCompany::recalculasaldos", 0 );
    mensajeInfo ( "recalcular Saldos" );
    BlDbRecordSet *cur = loadQuery ( "SELECT recalculasaldos()" );
    delete cur;
    _depura ( "END BcCompany::recalculasaldos", 0 );
}


///
/**
\return
**/
BcCentroCosteSeleccionarView *BcCompany::getselccostes()
{
    _depura ( "BcCompany::getselccostes", 0 );
    _depura ( "END BcCompany::getselccostes", 0 );
    return selccostes;
}


///
/**
\return
**/
BcCanalSeleccionarView *BcCompany::getselcanales()
{
    _depura ( "BcCompany::getselcanales", 0 );
    _depura ( "END BcCompany::getselcanales", 0 );
    return selcanales;
}


///
/**
**/
void BcCompany::centrocostedefecto()
{
    _depura ( "BcCompany::centrocostedefecto", 0 );
    selccostes->exec();
    _depura ( "END BcCompany::centrocostedefecto", 0 );
}


///
/**
**/
void BcCompany::canaldefecto()
{
    _depura ( "BcCompany::canaldefecto", 0 );
    selcanales->exec();
    _depura ( "END BcCompany::canaldefecto", 0 );
}


/// Guarda la configuracion de programa para poder recuperar algunas cosas de presentacion.
/**
**/
void BcCompany::guardaConf()
{
    _depura ( "BcCompany::guardaConf", 0 );

    /// Si iniciamos una empresa de contabilidad sin aplicacion entonces no
    /// guardamos la configuracion.
    if ( !m_bulmacont ) return;

    QFile file ( g_confpr->valor ( CONF_DIR_USER ) + "bulmacont_" + dbName() + ".cfn" );
    /// Guardado del orden y de configuraciones varias.
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << "<CONFIG>\n";
        stream << "\t<PRINCIPAL>\n";
        stream << "\t\t\t<X>" + QString::number ( m_bulmacont->x() ) + "</X>\n";
        stream << "\t\t\t<Y>" + QString::number ( m_bulmacont->y() ) + "</Y>\n";
        stream << "\t\t\t<WIDTH>" + QString::number ( m_bulmacont->width() ) + "</WIDTH>\n";
        stream << "\t\t\t<HEIGHT>" + QString::number ( m_bulmacont->height() ) + "</HEIGHT>\n";
        stream << "\t\t\t<INDEXADOR>" + ( m_bulmacont->actionIndexador->isChecked() ? QString ( "TRUE" ) : QString ( "FALSE" ) ) + "</INDEXADOR>\n";
        stream << "\t\t\t<TOOLBARSDOCKWIDGETS>" + QString ( m_bulmacont->saveState().toBase64() ) + "</TOOLBARSDOCKWIDGETS>\n";
        stream << "\t</PRINCIPAL>\n";


        for ( int i = 0; i < m_listventanas->numVentanas(); i++ ) {
            QObject *obj = m_listventanas->ventana ( i );
            QWidget *wid = ( QWidget * ) obj;
            stream << "\t<VENTANA>\n";
            stream << "\t\t<VNAME>" + obj->objectName() + "</VNAME>\n";
            stream << "\t\t<VX>" + QString::number ( wid->parentWidget() ->x() ) + "</VX>\n";
            stream << "\t\t<VY>" + QString::number ( wid->parentWidget() ->y() ) + "</VY>\n";
            stream << "\t\t<VWIDTH>" + QString::number ( wid->width() ) + "</VWIDTH>\n";
            stream << "\t\t<VHEIGHT>" + QString::number ( wid->height() ) + "</VHEIGHT>\n";
            stream << "\t\t<VVISIBLE>" + ( wid->isVisible() ? QString ( "TRUE" ) : QString ( "FALSE" ) ) + "</VVISIBLE>\n";
            stream << "\t\t<VMAXIMIZED>" + ( wid->isMaximized() ? QString ( "TRUE" ) : QString ( "FALSE" ) ) + "</VMAXIMIZED>\n";
            stream << "\t\t<VACTIVEWINDOW>" + ( m_bulmacont->workspace() ->activeWindow() == wid ? QString ( "TRUE" ) : QString ( "FALSE" ) ) + "</VACTIVEWINDOW>";
            stream << "\t</VENTANA>\n";
        } // end for

        stream << "</CONFIG>\n";
        file.close();
    } // end if
    _depura ( "END BcCompany::guardaConf", 0 );
}

/// Guarda la configuracion de programa para poder recuperar algunas cosas de presentacion.
/**
\return
**/
void BcCompany::cargaConf()
{
    _depura ( "BcCompany::cargaConf", 0 );


    /// Si iniciamos una empresa de contabilidad sin aplicacion entonces no
    /// guardamos la configuracion.
    if ( !m_bulmacont ) return;

    QFile file ( g_confpr->valor ( CONF_DIR_USER ) + "bulmacont_" + dbName() + ".cfn" );
    QDomDocument doc ( "mydocument" );
    if ( !file.open ( QIODevice::ReadOnly ) )
        return;
    if ( !doc.setContent ( &file ) ) {
        file.close();
        return;
    }
    file.close();

    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = doc.documentElement();
    QDomElement principal = docElem.firstChildElement ( "PRINCIPAL" );
    /// Cogemos la coordenada X
    QString nx = principal.firstChildElement ( "X" ).toElement().text();

    /// Cogemos la coordenada Y
    QString ny = principal.firstChildElement ( "Y" ).toElement().text();

    /// Cogemos el ancho
    QString nwidth = principal.firstChildElement ( "WIDTH" ).toElement().text();

    /// Cogemos el alto
    QString nheight = principal.firstChildElement ( "HEIGHT" ).toElement().text();

    /// Establecemos la geometria de la ventana principal.
    m_bulmacont->setGeometry ( nx.toInt(), ny.toInt(), nwidth.toInt(), nheight.toInt() );

    /// Cogemos el indexador
    QString indexador = principal.firstChildElement ( "INDEXADOR" ).toElement().text();
    if ( indexador == "TRUE" ) {
        s_indexadorCambiaEstado ( TRUE );
        m_bulmacont->actionIndexador->setChecked ( TRUE );
    } else {
        s_indexadorCambiaEstado ( FALSE );
        m_bulmacont->actionIndexador->setChecked ( FALSE );
    } // end if

    /// Cogemos el ancho del indexador
    m_bulmacont->restoreState ( QByteArray::fromBase64 ( QByteArray ( principal.firstChildElement ( "TOOLBARSDOCKWIDGETS" ).toElement().text().toAscii() ) ) );


    /// Tratamos cada ventana
    QWidget *activewindow = NULL;
    QDomNodeList nodos = docElem.elementsByTagName ( "VENTANA" );
    for ( int i = 0; i < nodos.count(); i++ ) {
        QDomNode ventana = nodos.item ( i );
        QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.
            QString vname = e1.firstChildElement ( "VNAME" ).toElement().text();
            for ( int j = 0; j < m_listventanas->numVentanas(); j++ ) {
                QObject *obj = m_listventanas->ventana ( j );
                QWidget *wid = ( QWidget * ) obj;
                if ( obj->objectName() == vname ) {
                    QString vx = e1.firstChildElement ( "VX" ).toElement().text();
                    QString vy = e1.firstChildElement ( "VY" ).toElement().text();
                    QString vwidth = e1.firstChildElement ( "VWIDTH" ).toElement().text();
                    QString vheight = e1.firstChildElement ( "VHEIGHT" ).toElement().text();
                    QString vvisible = e1.firstChildElement ( "VVISIBLE" ).toElement().text();
                    QString vmaximized = e1.firstChildElement ( "VMAXIMIZED" ).toElement().text();
                    QString vactivewindow = e1.firstChildElement ( "VACTIVEWINDOW" ).toElement().text();
                    /// Establecemos la geometria de la ventana principal.
                    wid->resize ( vwidth.toInt(), vheight.toInt() );
                    wid->parentWidget() ->move ( vx.toInt(), vy.toInt() );
                    if ( vvisible == "TRUE" ) {
                        wid->showNormal();
                    } else {
                        wid->hide();
                    } // end if
                    if ( vmaximized == "TRUE" ) {
                        wid->showMaximized();
                    }
                    if ( vactivewindow == "TRUE" ) {
                        activewindow = wid;
                    }
                } // end if
            } // end for
        } // end if
    } // end for
    /// Si hay una ventana activa se pone como activa.
    if ( activewindow )
        m_bulmacont->workspace() ->setActiveWindow ( activewindow );
    _depura ( "END BcCompany::cargaConf", 0 );
}


