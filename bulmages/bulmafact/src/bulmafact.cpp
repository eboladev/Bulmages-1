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
#include "plugins.h"


/** Prepara la pantalla principal para que tenga todos los componentes.
    Crea el workspace y lo inicializa.
    Crea la instancia de company y la inicializa y almacena esta en el puntero m_company.
    Crea la lista de ventanas.
*/
/**
\param bd
**/
Bulmafact::Bulmafact ( QString bd ) : QMainWindow()
{
    _depura ( "Bulmafact::Bulmafact", 0 );
    setupUi ( this );
    setUpdatesEnabled ( TRUE );
    pWorkspace = new QWorkspace2 ( this );
    pWorkspace->setScrollBarsEnabled ( TRUE );
    QFrame *m_frame1 = new QFrame();
    m_pb = new QProgressBar();
    m_pb->setMaximum ( 100 );
    m_pb->setMinimum ( 0 );
    m_pb->setValue ( 0 );
    /// Hacemos que el ProgressBar est&eacute; invisible hasta que se seleccione una empresa.
    m_pb->setVisible ( FALSE );
    setCentralWidget ( m_frame1 );
    /// Creamos un VerticalLayout donde metemos el contenido central del QMainWindow.
    QVBoxLayout *vboxlayout = new QVBoxLayout ( this->centralWidget() );
    vboxlayout->setSpacing ( 0 );
    vboxlayout->setMargin ( 0 );
    vboxlayout->addWidget ( pWorkspace );
    vboxlayout->addWidget ( m_pb );

    m_company = new Company ( this );
    m_company->setProgressBar ( m_pb );
    m_company->init ( bd, "BulmaFact" );
    m_company->setWorkspace ( pWorkspace );

    connect ( pWorkspace, SIGNAL ( windowActivated ( QWidget * ) ), this, SLOT ( informaindexador ( QWidget * ) ) );

    /// Aqui creamos la ventana dock para meter las distintas ventanas.
    m_list = new ListVentanas ( 0 );
    m_list->setVisible ( FALSE );
    /// Iniciamos el listventanas con el workspace para que pueda operar con el.
    m_list->setWorkspace ( pWorkspace );

    connect ( m_list, SIGNAL ( cambiaEstadoVisible ( bool ) ), this, SLOT ( setActionIndexador ( bool ) ) );
    addDockWidget ( Qt::LeftDockWidgetArea, m_list );

    m_company->setListVentanas ( m_list );

    m_list->setVisible ( TRUE );
    m_pb->setVisible ( FALSE );
    statusBar() ->showMessage ( bd, 2000 );
    setWindowTitle ( bd );
    _depura ( "END Bulmafact::Bulmafact", 0 );
}


/** Hacemos la creacion de las ventanas principales para que enten en plugins
*/
/**
\param splashScr
**/
void Bulmafact::createMainWindows ( Splash *splashScr )
{
    _depura ( "Bulmafact::createMainWindows", 0 );
    m_company->createMainWindows ( splashScr );
    _depura ( "END Bulmafact::createMainWindows", 0 );
}


/** Libera memoria destruyendo todos los objetos creados.
*/
/**
**/
Bulmafact::~Bulmafact()
{
    _depura ( "Bulmafact::~Bulmafact", 0 );
    delete pWorkspace;
    /// En MS-Windows no termina bien la ejecucion del programa y por eso
    /// agregamos esta salida rapida.
#ifdef WINDOWS

    exit ( 0 );
#endif

    _depura ( "END Bulmafact::~Bulmafact", 0 );
}


/// \TODO: Revisar este metodo.
/**
**/
void Bulmafact::s_FPago()
{
    _depura ( "Bulmafact::s_FPago", 0 );
    m_company->s_FPago();
    _depura ( "END Bulmafact::s_FPago", 0 );
}


/** Intercambia entre el modo ventana completa y el modo ventana normal
*/
/**
**/
void Bulmafact::s_ventanaCompleta()
{
    _depura ( "Bulmafact::s_ventanaCompleta", 0 );
    if ( isFullScreen() ) {
        showNormal();
    } else {
        showFullScreen();
    } // end if
    _depura ( "END Bulmafact::s_ventanaCompleta", 0 );
}


/** Metodo que responde a la pulsacion de About en el menu.
    Inicializa la ventana de About y la muestra.
*/
/**
**/
void Bulmafact::s_About()
{
    _depura ( "Bulmafact::s_About", 0 );
    AboutView about;
    about.exec();
    _depura ( "END Bulmafact::s_About", 0 );
}


/** Evento de cerrar la ventana principal.
    Libera parte de la memoria reservada. Ya que sabemos que se va a cerrar el programa.
*/
/**
**/
void Bulmafact::closeEvent ( QCloseEvent * )
{
    _depura ( "Bulmafact::closeEvent", 0 );

    g_plugins->lanza ( "Bulmafact_closeEvent", this );

    delete m_company;
    m_company = NULL;
    delete m_list;
    m_list = NULL;
#ifdef WINDOWS

    exit ( 0 );
#endif

    _depura ( "END Bulmafact::closeEvent", 0 );
}


/** Captura el evento de cambio de ventana en el workSpace y actua sobre el
    listado de ventanas para que seleccione la ventana adecuada.
*/
/**
\param w
\return
**/
void Bulmafact::informaindexador ( QWidget *w )
{
    _depura ( "Bulmafact::informaindexador", 0 );

    /// Si no esta inicializado company no se le puede informar.
    if ( m_company == NULL ) {
        _depura ( "END Bulmafact::informaindexador", 0, "Company no iniciado" );
        return;
    } // end if

    /// No existe una ventana que activar.
    if ( w == NULL ) {
        m_company->deSeleccionaWindow();
        _depura ( "END Bulmafact::informaindexador", 0, "No existe la ventana a activar" );
        return;
    } // end if
    m_company->seleccionaWindow ( w->windowTitle(), w );

    _depura ( "END Bulmafact::informaindexador", 0 );
}


/** Muestra la ayuda del programa.
*/
/**
**/
void Bulmafact::on_actionDocumentacion_triggered()
{
    _depura ( "Bulmafact::on_actionDocumentacion_triggered", 0 );
    QAssistantClient *asistenteAyuda = new QAssistantClient ( QLibraryInfo::location ( QLibraryInfo::BinariesPath ), 0 );
    connect ( asistenteAyuda, SIGNAL ( error ( const QString ) ), this, SLOT ( documentacionError ( const QString ) ) );
    QStringList parametros;
    parametros << "-profile" << QString ( confpr->valor ( CONF_PROGDATA ) + "help/bulmafact/bulmafact.adp" );
    asistenteAyuda->setArguments ( parametros );
    asistenteAyuda->openAssistant();
    _depura ( "END Bulmafact::on_actionDocumentacion_triggered", 0 );
}


///
/**
\param docError
**/
void Bulmafact::documentacionError ( const QString docError )
{
    _depura ( "Error en la ayuda: " + docError, 10 );
}


///
/**
\param visible
**/
void Bulmafact::setActionIndexador ( bool visible )
{
    _depura ( "Bulmafact::setActionIndexador", 0 );
    if ( visible == TRUE ) {
        actionIndexador->setChecked ( TRUE );
    } else {
        actionIndexador->setChecked ( FALSE );
    } // end if
    _depura ( "END Bulmafact::setActionIndexador", 0 );
}


///
/**
**/
void Bulmafact::on_actionIndexador_triggered()
{
    _depura ( "Bulmafact::on_actionIndexador_triggered", 0 );
    if ( actionIndexador->isChecked() == TRUE ) {
        m_company->s_indexadorCambiaEstado ( TRUE );
    } else {
        m_company->s_indexadorCambiaEstado ( FALSE );
    } // end if
    _depura ( "END Bulmafact::on_actionIndexador_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionTipoIVA_triggered()
{
    _depura ( "Bulmafact::on_actionTipoIVA_triggered", 0 );
    m_company->s_TipoIVAView();
    _depura ( "END Bulmafact::on_actionTipoIVA_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionTasaIVA_triggered()
{
    _depura ( "Bulmafact::on_actionTasaIVA_triggered", 0 );
    m_company->s_TasaIVAView();
    _depura ( "END Bulmafact::on_actionTasaIVA_triggered", 0 );
}


///
/**
\return
**/
QWorkspace2 * Bulmafact::workspace()
{
    _depura ( "Bulmafact::workspace", 0 );
    _depura ( "END Bulmafact::workspace", 0 );
    return pWorkspace;
}


///
/**
\return
**/
Company * Bulmafact::getcompany()
{
    _depura ( "Bulmafact::getcompany", 0 );
    _depura ( "END Bulmafact::getcompany", 0 );
    return m_company;
}



///
/**
**/
void Bulmafact::on_actionGestion_Familias_triggered()
{
    _depura ( "Bulmafact::on_actionGestion_Familias_triggered", 0 );
    m_company->s_newfamiliasview();
    _depura ( "END Bulmafact::on_actionGestion_Familias_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionGestion_Tipos_Articulo_triggered()
{
    _depura ( "Bulmafact::on_actionGestion_Tipos_Articulo_triggered", 0 );
    m_company->s_newTipoArticuloList();
    _depura ( "END Bulmafact::on_actionGestion_Tipos_Articulo_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionNuevo_Articulo_triggered()
{
    _depura ( "Bulmafact::on_actionNuevo_Articulo_triggered", 0 );
    m_company->s_newArticulo();
    _depura ( "END Bulmafact::on_actionNuevo_Articulo_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionListado_de_Articulos_triggered()
{
    _depura ( "Bulmafact::on_actionListado_de_Articulos_triggered", 0 );
    m_company->listarticles();
    _depura ( "END Bulmafact::on_actionListado_de_Articulos_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionNuevo_Proveedor_triggered()
{
    _depura ( "Bulmafact::on_actionNuevo_Proveedor_triggered", 0 );
    m_company->s_newProveedorView();
    _depura ( "END Bulmafact::on_actionNuevo_Proveedor_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionListado_Proveedores_triggered()
{
    _depura ( "Bulmafact::on_actionListado_Proveedores_triggered", 0 );
    m_company->listproviders();
    _depura ( "END Bulmafact::on_actionListado_Proveedores_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionNuevo_Cliente_triggered()
{
    _depura ( "Bulmafact::on_actionNuevo_Cliente_triggered", 0 );
    m_company->s_newClienteView();
    _depura ( "END Bulmafact::on_actionNuevo_Cliente_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionListado_Clientes_triggered()
{
    _depura ( "Bulmafact::on_actionListado_Clientes_triggered", 0 );
    m_company->listClients();
    _depura ( "END Bulmafact::on_actionListado_Clientes_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionPaises_triggered()
{
    _depura ( "Bulmafact::on_actionPaises_triggered", 0 );
    m_company->muestraPaises();
    _depura ( "END Bulmafact::on_actionPaises_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionSeries_de_Factura_triggered()
{
    _depura ( "Bulmafact::on_actionSeries_de_Factura_triggered", 0 );
    m_company->s_seriesFactura();
    _depura ( "END Bulmafact::on_actionSeries_de_Factura_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionTrabajadores_triggered()
{
    _depura ( "Bulmafact::on_actionTrabajadores_triggered", 0 );
    m_company->s_trabajadores();
    _depura ( "END Bulmafact::on_actionTrabajadores_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionBancos_triggered()
{
    _depura ( "Bulmafact::on_actionBancos_triggered", 0 );
    m_company->s_bancos();
    _depura ( "END Bulmafact::on_actionBancos_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionVentana_Completa_triggered()
{
    _depura ( "Bulmafact::on_actionVentana_Completa_triggered", 0 );
    s_ventanaCompleta();
    _depura ( "END Bulmafact::on_actionVentana_Completa_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionAcerta_de_triggered()
{
    _depura ( "Bulmafact::on_actionAcerta_de_triggered", 0 );
    s_About();
    _depura ( "END Bulmafact::on_actionAcerta_de_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionAlmacenes_triggered()
{
    _depura ( "Bulmafact::on_actionAlmacenes_triggered", 0 );
    m_company->s_almacenes();
    _depura ( "END Bulmafact::on_actionAlmacenes_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionParametros_triggered()
{
    _depura ( "Bulmafact::on_actionParametros_triggered", 0 );
    m_company->s_newListConfiguracionView();
    _depura ( "END Bulmafact::on_actionParametros_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionFormas_de_Pago_triggered()
{
    _depura ( "Bulmafact::on_actionFormas_de_Pago_triggered", 0 );
    s_FPago();
    _depura ( "END Bulmafact::on_actionFormas_de_Pago_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionOrdenar_Ventanas_triggered()
{
    _depura ( "Bulmafact::on_actionOrdenar_Ventanas_triggered", 0 );
    pWorkspace->tile();
    _depura ( "END Bulmafact::on_actionOrdenar_Ventanas_triggered", 0 );
}


///
/**
**/
void Bulmafact::on_actionOrganizaci_n_en_Cascada_triggered()
{
    _depura ( "Bulmafact::on_actionOrganizacion_en_Cascada_triggered", 0 );
    pWorkspace->cascade();
    _depura ( "END Bulmafact::on_actionOrganizacion_en_Cascada_triggered", 0 );
}

