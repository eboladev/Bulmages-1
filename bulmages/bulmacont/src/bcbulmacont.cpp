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

#include "bcbulmacont.h"
#include "blconfiguration.h"
#include "bcaboutview.h"
#include "blplugins.h"

///
/**
\param parent
\param f
\param DB
**/
BcBulmaCont::BcBulmaCont ( QWidget *parent, Qt::WFlags f, QString DB )
        : BlMainWindow ( parent, f )
{
    _depura ( "BcBulmaCont::BcBulmaCont", 0 );
    setupUi ( this );

    m_pWorkspace = new BlWorkspace ( this );
    m_pWorkspace->setScrollBarsEnabled ( TRUE );

    QFrame *m_frame1 = new QFrame();
    QProgressBar *m_pb = new QProgressBar();
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
    vboxlayout->addWidget ( m_pWorkspace );
    vboxlayout->addWidget ( m_pb );


    m_empresaactual = new BcCompany ( this );
    m_empresaactual->setProgressBar ( m_pb );
    m_empresaactual->init ( DB, "BulmaCont" );
    m_empresaactual->setWorkspace ( m_pWorkspace );

    connect ( m_pWorkspace, SIGNAL ( windowActivated ( QWidget * ) ), this, SLOT ( informaindexador ( QWidget * ) ) );

    /// Aqu&iacute; creamos la ventana dock para meter las distintas ventanas.
    m_list = new BlWindowListDock ( 0 );
    m_list->setVisible ( FALSE );

    /// Iniciamos el listventanas con el workspace para que pueda operar con el.
    m_list->setWorkspace ( m_pWorkspace );

    connect ( m_list, SIGNAL ( cambiaEstadoVisible ( bool ) ), this, SLOT ( setActionIndexador ( bool ) ) );

    addDockWidget ( Qt::LeftDockWidgetArea, m_list );

    m_empresaactual->setListVentanas ( m_list );

    m_list->setVisible ( TRUE );
    m_pb->setVisible ( FALSE );

    initStatusBar();
    statusBar() ->showMessage ( DB, 2000 );
    setWindowTitle ( DB );
    _depura ( "END BcBulmaCont::BcBulmaCont", 0 );
}


///
/**
**/
BcBulmaCont::~BcBulmaCont()
{
    _depura ( "BcBulmaCont::~BcBulmaCont", 0 );
    delete m_pWorkspace;
    /// En MS-Windows no termina bien la ejecucion del programa y por eso
    /// agregamos esta salida rapida.
#ifdef WINDOWS
    exit ( 0 );
#endif
    _depura ( "END BcBulmaCont::~BcBulmaCont", 0 );
}


///
/**
\return
**/
BcCompany *BcBulmaCont::empresaactual()
{
    _depura ( "BcBulmaCont::empresaactual", 0 );
    _depura ( "END BcBulmaCont::empresaactual", 0 );
    return m_empresaactual;
}


/// Exporta menues.
/**
\return
**/
QMenu *BcBulmaCont::mui_MenuHerramientas()
{
    _depura ( "BcBulmaCont::mui_MenuHerramientas", 0 );
    _depura ( "END BcBulmaCont::mui_MenuHerramientas", 0 );
    return menuHerramientas;
}


///
/**
\return
**/
/*
QMenu *BcBulmaCont::mui_MenuVer()
{
    _depura ( "BcBulmaCont::mui_MenuVer", 0 );
    _depura ( "END BcBulmaCont::mui_MenuVer", 0 );
    return menuVer;
}
*/

///
/**
**/
void BcBulmaCont::s_asiento1()
{
    _depura ( "BcBulmaCont::s_asiento1", 0 );
    m_empresaactual->s_asiento1();
    _depura ( "END BcBulmaCont::s_asiento1", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionTipos_de_IVA_triggered()
{
    _depura ( "BcBulmaCont::on_actionTipos_de_IVA_triggered", 0 );
    m_empresaactual->tiposIVA();
    _depura ( "END BcBulmaCont::on_actionTipos_de_IVA_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionCerrar_Asientos_triggered()
{
    _depura ( "BcBulmaCont::on_actionCerrar_Asientos_triggered", 0 );
    m_empresaactual->cerrarAsientos();
    _depura ( "END BcBulmaCont::on_actionCerrar_Asientos_triggered", 0 );
}

///
/**
**/
void BcBulmaCont::on_actionFormas_de_Pago_triggered()
{
    _depura ( "BcBulmaCont::on_actionFormas_de_Pago_triggered", 0 );
    m_empresaactual->fPago();
    _depura ( "END BcBulmaCont::on_actionFormas_de_Pago_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionConfiguracion_triggered()
{
    _depura ( "BcBulmaCont::on_actionConfiguracion_triggered", 0 );
    m_empresaactual->propiedadempresa();
    _depura ( "END BcBulmaCont::on_actionConfiguracion_triggered", 0 );
}



///
/**
**/
void BcBulmaCont::on_actionPlan_Contable_triggered()
{
    _depura ( "BcBulmaCont::on_actionPlan_Contable_triggered", 0 );
    m_empresaactual->muestracuentas();
    _depura ( "END BcBulmaCont::on_actionPlan_Contable_triggered", 0 );
}



///
/**
**/
void BcBulmaCont::on_actionRecalcular_Saldos_Iniciales_triggered()
{
    _depura ( "BcBulmaCont::on_actionRecalcular_Saldos_Iniciales_triggered", 0 );
    _depura ( "END BcBulmaCont::on_actionRecalcular_Saldos_Iniciales_triggered", 0 );
    m_empresaactual->recalculasaldos();
}


///
/**
**/
void BcBulmaCont::on_actionSustituir_Cuentas_triggered()
{
    _depura ( "BcBulmaCont::on_actionSustituir_Cuentas_triggered", 0 );
    m_empresaactual->reemplazacuentaenasientos();
    _depura ( "BcBulmaCont::on_actionSustituir_Cuentas_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionCanal_por_Defecto_triggered()
{
    _depura ( "BcBulmaCont::on_actionCanal_por_Defecto_triggered", 0 );
    m_empresaactual->canaldefecto();
    _depura ( "END BcBulmaCont::on_actionCanal_por_Defecto_triggered", 0 );
}

///
/**
**/
void BcBulmaCont::on_actionCentros_de_Coste_triggered()
{
    _depura ( "BcBulmaCont::on_actionCentros_de_Coste_triggered", 0 );
    m_empresaactual->ccostes();
    _depura ( "END BcBulmaCont::on_actionCentros_de_Coste_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionCanales_triggered()
{
    _depura ( "BcBulmaCont::on_actionCanales_triggered", 0 );
    m_empresaactual->canales();
    _depura ( "END BcBulmaCont::on_actionCanales_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionCentro_de_Coste_por_Defecto_triggered()
{
    _depura ( "BcBulmaCont::on_actionCentro_de_Coste_por_Defecto_triggered", 0 );
    m_empresaactual->centrocostedefecto();
    _depura ( "END BcBulmaCont::on_actionCentro_de_Coste_por_Defecto_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionOrdenar_Ventanas_triggered()
{
    _depura ( "BcBulmaCont::on_actionOrdenar_Ventanas_triggered", 0 );
    m_pWorkspace->tile();
    _depura ( "END BcBulmaCont::on_actionOrdenar_Ventanas_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionOrganizaci_n_en_Cascada_triggered()
{
    _depura ( "BcBulmaCont::on_actionOrganizaci_n_en_Cascada_triggered", 0 );
    m_pWorkspace->cascade();
    _depura ( "END BcBulmaCont::on_actionOrganizaci_n_en_Cascada_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionCuentas_Anuales_triggered()
{
    _depura ( "BcBulmaCont::on_actionCuentas_Anuales_triggered", 0 );
    m_empresaactual->compbalance();
    _depura ( "END BcBulmaCont::on_actionCuentas_Anuales_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionMasas_Patrimoniales_triggered()
{
    _depura ( "BcBulmaCont::on_actionMasas_Patrimoniales_triggered", 0 );
    m_empresaactual->mpatrimoniales();
    _depura ( "END BcBulmaCont::on_actionMasas_Patrimoniales_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionListado_347_triggered()
{
    _depura ( "BcBulmaCont::on_actionListado_347_triggered", 0 );
    m_empresaactual->listado347();
    _depura ( "END BcBulmaCont::on_actionListado_347_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionAmortizaciones_triggered()
{
    _depura ( "BcBulmaCont::on_actionAmortizaciones_triggered", 0 );
    m_empresaactual->amortizaciones();
    _depura ( "END BcBulmaCont::on_actionAmortizaciones_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionPantalla_Completa_triggered()
{
    _depura ( "BcBulmaCont::on_actionPantalla_Completa_triggered", 0 );
    if ( isFullScreen() ) {
        showNormal();
    } else {
        showFullScreen();
    } // end if
    _depura ( "END BcBulmaCont::on_actionPantalla_Completa_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionIndexador_triggered()
{
    _depura ( "BcBulmaCont::on_actionIndexador_triggered", 0 );
    if ( actionIndexador->isChecked() == TRUE ) {
        m_empresaactual->s_indexadorCambiaEstado ( TRUE );
    } else {
        m_empresaactual->s_indexadorCambiaEstado ( FALSE );
    } // end if
    _depura ( "END BcBulmaCont::on_actionIndexador_triggered", 0 );
}


///
/**
\param visible
**/
void BcBulmaCont::setActionIndexador ( bool visible )
{
    _depura ( "BcBulmaCont::setActionIndexador", 0 );
    if ( visible == TRUE ) {
        actionIndexador->setChecked ( TRUE );
    } else {
        actionIndexador->setChecked ( FALSE );
    } // end if
    _depura ( "END BcBulmaCont::setActionIndexador", 0 );
}


///
/**
**/
void BcBulmaCont::initStatusBar()
{
    _depura ( "BcBulmaCont::initStatusBar", 0 );
    statusBar() ->showMessage ( _ ( "Listo." ) );
    _depura ( "END BcBulmaCont::initStatusBar", 0 );
}


///
/**
\param object
\param event
\return
**/
bool BcBulmaCont::eventFilter ( QObject *object, QEvent *event )
{
    _depura ( "BcBulmaCont::eventFilter", 0 );
    /// Standard event processing.
    return QWidget::eventFilter ( object, event );
}


///
/**
**/
void BcBulmaCont::slotEditUndo()
{
    _depura ( "BcBulmaCont::slotEditUndo", 0 );
    statusBar() ->showMessage ( _ ( "Deshaciendo la ultima accion..." ) );
    statusBar() ->showMessage ( _ ( "Listo." ) );
    _depura ( "END BcBulmaCont::slotEditUndo", 0 );
}


///
/**
**/
void BcBulmaCont::slotEditCut()
{
    _depura ( "BcBulmaCont::slotEditCut", 0 );
    statusBar() ->showMessage ( _ ( "Cortando seleccion..." ) );
    statusBar() ->showMessage ( _ ( "Listo." ) );
    _depura ( "EMD BcBulmaCont::slotEditCut", 0 );
}


///
/**
**/
void BcBulmaCont::slotEditCopy()
{
    _depura ( "BcBulmaCont::slotEditCopy", 0 );
    statusBar() ->showMessage ( _ ( "Copiando la seleccion al portapapeles..." ) );
    statusBar() ->showMessage ( _ ( "Listo." ) );
    _depura ( "END BcBulmaCont::slotEditCopy", 0 );
}


///
/**
**/
void BcBulmaCont::slotEditPaste()
{
    _depura ( "BcBulmaCont::slotEditPaste", 0 );
    statusBar() ->showMessage ( _ ( "Insertando el contenido del portapapeles..." ) );
    statusBar() ->showMessage ( _ ( "Listo." ) );
    _depura ( "END BcBulmaCont::slotEditPaste", 0 );
}


///
/**
\param toggle
**/
void BcBulmaCont::slotViewStatusBar ( bool toggle )
{
    _depura ( "BcBulmaCont::slotViewStatusBar", 0 );
    statusBar() ->showMessage ( _ ( "Cambinado barra de estado..." ) );
    if ( toggle == false ) {
        statusBar() ->hide();
    } else {
        statusBar() ->show();
    } // end if
    statusBar() ->showMessage ( _ ( "Listo." ) );
    _depura ( "END BcBulmaCont::slotViewStatusBar", 0 );
}


///
/**
\param toggle
**/
void BcBulmaCont::slotViewFullScreen ( bool toggle )
{
    _depura ( "BcBulmaCont::slotViewFullScreen", 0 );
    statusBar() ->showMessage ( _ ( "Cambiando a modo de pantalla completa..." ) );
    if ( toggle == false ) {
        showNormal();
    } else {
        showFullScreen();
    } // end if
    statusBar() ->showMessage ( _ ( "Listo." ) );
    _depura ( "END BcBulmaCont::slotViewFullScreen", 0 );
}


///
/**
\param text
**/
void BcBulmaCont::slotStatusHelpMsg ( const QString &text )
{
    _depura ( "BcBulmaCont::slotStatusHelpMsg", 0 );
    /// Change status message of whole statusbar temporary (text, msec).
    statusBar() ->showMessage ( text, 2000 );
    _depura ( "END BcBulmaCont::slotStatusHelpMsg", 0 );
}


///
/**
**/
void BcBulmaCont::slotListadoCuentas()
{
    _depura ( "BcBulmaCont::slotListadoCuentas", 0 );
    statusBar() ->showMessage ( _ ( "Listado de cuentas" ) );
    _depura ( "END BcBulmaCont::slotListadoCuentas", 0 );
}



///
/**
**/
void BcBulmaCont::slotCentrosCoste()
{
    _depura ( "BcBulmaCont::slotCentrosCoste", 0 );
    m_empresaactual->ccostes();
    _depura ( "END BcBulmaCont::slotCentrosCoste", 0 );
}


///
/**
**/
void BcBulmaCont::slotCanales()
{
    _depura ( "BcBulmaCont::slotCanales", 0 );
    m_empresaactual->canales();
    _depura ( "END BcBulmaCont::slotCanales", 0 );
}


///
/**
**/
void BcBulmaCont::slotCompBalance()
{
    _depura ( "BcBulmaCont::slotCompBalance", 0 );
    m_empresaactual->compbalance();
    _depura ( "END BcBulmaCont::slotCompBalance", 0 );
}


///
/**
**/
void BcBulmaCont::slotPerdidas()
{
    _depura ( "BcBulmaCont::slotPerdidas", 0 );
    _depura ( "END BcBulmaCont::slotPerdidas", 0 );
}


///
/**
**/
void BcBulmaCont::windowMenuAboutToShow()
{
    _depura ( "BcBulmaCont::windowMenuAboutToShow", 0 );
    _depura ( "END BcBulmaCont::windowMenuAboutToShow", 0 );
}


///
/**
**/
void BcBulmaCont::slotWindowNewWindow()
{
    _depura ( "BcBulmaCont::slotWindowNewWindow", 0 );
    _depura ( "END BcBulmaCont::slotWindowNewWindow", 0 );
}


///
/**
**/
void BcBulmaCont::closeEvent ( QCloseEvent *event )
{
    _depura ( "BcBulmaCont::closeEvent", 0 );


    /// Antes de salir hacemos un mensaje de advertencia.
    QMessageBox msgBox;
    msgBox.setText(_("Seguro que desea abandonar el programa "));
    msgBox.setInformativeText(_("Se perderan los cambios no guardados"));
    msgBox.setStandardButtons( QMessageBox::Close | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Cancel) {
      event->ignore();
      _depura ( "END BfBulmaFact::closeEvent", 0 );
      return;
    }



    g_plugins->lanza ( "BcBulmaCont_closeEvent", this );

    delete m_empresaactual;
    m_empresaactual = NULL;
    delete m_list;
    m_list = NULL;

#ifdef WINDOWS
    exit ( 0 );
#endif

    _depura ( "END BcBulmaCont::closeEvent", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionAyuda_triggered()
{
    _depura ( "BcBulmaCont::on_actionAyuda_triggered", 0 );
    QAssistantClient *asistenteAyuda = new QAssistantClient ( QLibraryInfo::location ( QLibraryInfo::BinariesPath ), 0 );
    connect ( asistenteAyuda, SIGNAL ( error ( const QString ) ), this, SLOT ( documentacionError ( const QString ) ) );
    QStringList parametros;
    parametros << "-profile" << QString ( g_confpr->valor ( CONF_PROGDATA ) + "ayuda/bulmacont/bulmacont.adp" );
    asistenteAyuda->setArguments ( parametros );
    asistenteAyuda->openAssistant();
    _depura ( "END BcBulmaCont::on_actionAyuda_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionAcerca_de_triggered()
{
    _depura ( "BcBulmaCont::on_actionAcerca_de_triggered", 0 );
    BcAboutView *sobre = new BcAboutView ( 0 );
    sobre->exec();
    _depura ( "END BcBulmaCont::on_actionAcerca_de_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionPaises_triggered()
{
    _depura ( "BcBulmaCont::on_actionPaises_triggered", 0 );
    m_empresaactual->muestraPaises();
    _depura ( "BcBulmaCont::on_actionPaises_triggered", 0 );
}



/** Captura el evento de cambio de ventana en el workSpace y actua sobre el
    listado de ventanas para que seleccione la ventana adecuada.
*/
///
/**
\param w
\return
**/
void BcBulmaCont::informaindexador ( QWidget *w )
{
    _depura ( "BcBulmaCont::informaindexador", 0 );
    /// No existe una ventana que activar.
    if ( m_empresaactual == NULL ) {
        _depura ( "END BcBulmaCont::informaindexador", 0, "Sin empresa" );
        return;
    } // end if

    if ( w == NULL ) {
        m_empresaactual->deSeleccionaWindow();
        _depura ( "END BcBulmaCont::informaindexador", 0, "Sin Widget" );
        return;
    } // end if
    m_empresaactual->deSeleccionaWindow();
    m_empresaactual->seleccionaWindow ( w->windowTitle(), w );

    QString texto = "Window activated. " + w->windowTitle() + "\n";
    printf ( "%s", texto.toAscii().constData() );

    _depura ( "END BcBulmaCont::informaindexador", 0 );
}


///
/**
\return
**/
BlWorkspace *BcBulmaCont::workspace()
{
    _depura ( "BcBulmaCont::workspace", 0 );
    _depura ( "END BcBulmaCont::workspace", 0 );
    return m_pWorkspace;
}



