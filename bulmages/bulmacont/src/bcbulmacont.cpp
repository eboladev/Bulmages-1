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
    blDebug ( "BcBulmaCont::BcBulmaCont", 0 );
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
    blDebug ( "END BcBulmaCont::BcBulmaCont", 0 );
}


///
/**
**/
BcBulmaCont::~BcBulmaCont()
{
    blDebug ( "BcBulmaCont::~BcBulmaCont", 0 );
    delete m_pWorkspace;
    /// En MS-Windows no termina bien la ejecucion del programa y por eso
    /// agregamos esta salida rapida.
#ifdef WINDOWS
    exit ( 0 );
#endif
    blDebug ( "END BcBulmaCont::~BcBulmaCont", 0 );
}


///
/**
\return
**/
BcCompany *BcBulmaCont::empresaactual()
{
    blDebug ( "BcBulmaCont::empresaactual", 0 );
    blDebug ( "END BcBulmaCont::empresaactual", 0 );
    return m_empresaactual;
}


/// Exporta menues.
/**
\return
**/
QMenu *BcBulmaCont::mui_MenuHerramientas()
{
    blDebug ( "BcBulmaCont::mui_MenuHerramientas", 0 );
    blDebug ( "END BcBulmaCont::mui_MenuHerramientas", 0 );
    return menuHerramientas;
}


///
/**
\return
**/
/*
QMenu *BcBulmaCont::mui_MenuVer()
{
    blDebug ( "BcBulmaCont::mui_MenuVer", 0 );
    blDebug ( "END BcBulmaCont::mui_MenuVer", 0 );
    return menuVer;
}
*/

///
/**
**/
void BcBulmaCont::s_asiento1()
{
    blDebug ( "BcBulmaCont::s_asiento1", 0 );
    m_empresaactual->s_asiento1();
    blDebug ( "END BcBulmaCont::s_asiento1", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionTipos_de_IVA_triggered()
{
    blDebug ( "BcBulmaCont::on_actionTipos_de_IVA_triggered", 0 );
    m_empresaactual->tiposIVA();
    blDebug ( "END BcBulmaCont::on_actionTipos_de_IVA_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionCerrar_Asientos_triggered()
{
    blDebug ( "BcBulmaCont::on_actionCerrar_Asientos_triggered", 0 );
    m_empresaactual->cerrarAsientos();
    blDebug ( "END BcBulmaCont::on_actionCerrar_Asientos_triggered", 0 );
}

///
/**
**/
void BcBulmaCont::on_actionFormas_de_Pago_triggered()
{
    blDebug ( "BcBulmaCont::on_actionFormas_de_Pago_triggered", 0 );
    m_empresaactual->fPago();
    blDebug ( "END BcBulmaCont::on_actionFormas_de_Pago_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionConfiguracion_triggered()
{
    blDebug ( "BcBulmaCont::on_actionConfiguracion_triggered", 0 );
    m_empresaactual->propiedadempresa();
    blDebug ( "END BcBulmaCont::on_actionConfiguracion_triggered", 0 );
}



///
/**
**/
void BcBulmaCont::on_actionPlan_Contable_triggered()
{
    blDebug ( "BcBulmaCont::on_actionPlan_Contable_triggered", 0 );
    m_empresaactual->muestracuentas();
    blDebug ( "END BcBulmaCont::on_actionPlan_Contable_triggered", 0 );
}



///
/**
**/
void BcBulmaCont::on_actionRecalcular_Saldos_Iniciales_triggered()
{
    blDebug ( "BcBulmaCont::on_actionRecalcular_Saldos_Iniciales_triggered", 0 );
    blDebug ( "END BcBulmaCont::on_actionRecalcular_Saldos_Iniciales_triggered", 0 );
    m_empresaactual->recalculasaldos();
}


///
/**
**/
void BcBulmaCont::on_actionSustituir_Cuentas_triggered()
{
    blDebug ( "BcBulmaCont::on_actionSustituir_Cuentas_triggered", 0 );
    m_empresaactual->reemplazacuentaenasientos();
    blDebug ( "BcBulmaCont::on_actionSustituir_Cuentas_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionCanal_por_Defecto_triggered()
{
    blDebug ( "BcBulmaCont::on_actionCanal_por_Defecto_triggered", 0 );
    m_empresaactual->canaldefecto();
    blDebug ( "END BcBulmaCont::on_actionCanal_por_Defecto_triggered", 0 );
}

///
/**
**/
void BcBulmaCont::on_actionCentros_de_Coste_triggered()
{
    blDebug ( "BcBulmaCont::on_actionCentros_de_Coste_triggered", 0 );
    m_empresaactual->ccostes();
    blDebug ( "END BcBulmaCont::on_actionCentros_de_Coste_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionCanales_triggered()
{
    blDebug ( "BcBulmaCont::on_actionCanales_triggered", 0 );
    m_empresaactual->canales();
    blDebug ( "END BcBulmaCont::on_actionCanales_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionCentro_de_Coste_por_Defecto_triggered()
{
    blDebug ( "BcBulmaCont::on_actionCentro_de_Coste_por_Defecto_triggered", 0 );
    m_empresaactual->centrocostedefecto();
    blDebug ( "END BcBulmaCont::on_actionCentro_de_Coste_por_Defecto_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionOrdenar_Ventanas_triggered()
{
    blDebug ( "BcBulmaCont::on_actionOrdenar_Ventanas_triggered", 0 );
    m_pWorkspace->tile();
    blDebug ( "END BcBulmaCont::on_actionOrdenar_Ventanas_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionOrganizaci_n_en_Cascada_triggered()
{
    blDebug ( "BcBulmaCont::on_actionOrganizaci_n_en_Cascada_triggered", 0 );
    m_pWorkspace->cascade();
    blDebug ( "END BcBulmaCont::on_actionOrganizaci_n_en_Cascada_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionCuentas_Anuales_triggered()
{
    blDebug ( "BcBulmaCont::on_actionCuentas_Anuales_triggered", 0 );
    m_empresaactual->compbalance();
    blDebug ( "END BcBulmaCont::on_actionCuentas_Anuales_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionMasas_Patrimoniales_triggered()
{
    blDebug ( "BcBulmaCont::on_actionMasas_Patrimoniales_triggered", 0 );
    m_empresaactual->mpatrimoniales();
    blDebug ( "END BcBulmaCont::on_actionMasas_Patrimoniales_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionListado_347_triggered()
{
    blDebug ( "BcBulmaCont::on_actionListado_347_triggered", 0 );
    m_empresaactual->listado347();
    blDebug ( "END BcBulmaCont::on_actionListado_347_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionAmortizaciones_triggered()
{
    blDebug ( "BcBulmaCont::on_actionAmortizaciones_triggered", 0 );
    m_empresaactual->amortizaciones();
    blDebug ( "END BcBulmaCont::on_actionAmortizaciones_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionPantalla_Completa_triggered()
{
    blDebug ( "BcBulmaCont::on_actionPantalla_Completa_triggered", 0 );
    if ( isFullScreen() ) {
        showNormal();
    } else {
        showFullScreen();
    } // end if
    blDebug ( "END BcBulmaCont::on_actionPantalla_Completa_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionIndexador_triggered()
{
    blDebug ( "BcBulmaCont::on_actionIndexador_triggered", 0 );
    if ( actionIndexador->isChecked() == TRUE ) {
        m_empresaactual->s_indexadorCambiaEstado ( TRUE );
    } else {
        m_empresaactual->s_indexadorCambiaEstado ( FALSE );
    } // end if
    blDebug ( "END BcBulmaCont::on_actionIndexador_triggered", 0 );
}


///
/**
\param visible
**/
void BcBulmaCont::setActionIndexador ( bool visible )
{
    blDebug ( "BcBulmaCont::setActionIndexador", 0 );
    if ( visible == TRUE ) {
        actionIndexador->setChecked ( TRUE );
    } else {
        actionIndexador->setChecked ( FALSE );
    } // end if
    blDebug ( "END BcBulmaCont::setActionIndexador", 0 );
}


///
/**
**/
void BcBulmaCont::initStatusBar()
{
    blDebug ( "BcBulmaCont::initStatusBar", 0 );
    statusBar() ->showMessage ( _ ( "Listo." ) );
    blDebug ( "END BcBulmaCont::initStatusBar", 0 );
}


///
/**
\param object
\param event
\return
**/
bool BcBulmaCont::eventFilter ( QObject *object, QEvent *event )
{
    blDebug ( "BcBulmaCont::eventFilter", 0 );
    /// Standard event processing.
    return QWidget::eventFilter ( object, event );
}


///
/**
**/
void BcBulmaCont::slotEditUndo()
{
    blDebug ( "BcBulmaCont::slotEditUndo", 0 );
    statusBar() ->showMessage ( _ ( "Deshaciendo la ultima accion..." ) );
    statusBar() ->showMessage ( _ ( "Listo." ) );
    blDebug ( "END BcBulmaCont::slotEditUndo", 0 );
}


///
/**
**/
void BcBulmaCont::slotEditCut()
{
    blDebug ( "BcBulmaCont::slotEditCut", 0 );
    statusBar() ->showMessage ( _ ( "Cortando seleccion..." ) );
    statusBar() ->showMessage ( _ ( "Listo." ) );
    blDebug ( "EMD BcBulmaCont::slotEditCut", 0 );
}


///
/**
**/
void BcBulmaCont::slotEditCopy()
{
    blDebug ( "BcBulmaCont::slotEditCopy", 0 );
    statusBar() ->showMessage ( _ ( "Copiando la seleccion al portapapeles..." ) );
    statusBar() ->showMessage ( _ ( "Listo." ) );
    blDebug ( "END BcBulmaCont::slotEditCopy", 0 );
}


///
/**
**/
void BcBulmaCont::slotEditPaste()
{
    blDebug ( "BcBulmaCont::slotEditPaste", 0 );
    statusBar() ->showMessage ( _ ( "Insertando el contenido del portapapeles..." ) );
    statusBar() ->showMessage ( _ ( "Listo." ) );
    blDebug ( "END BcBulmaCont::slotEditPaste", 0 );
}


///
/**
\param toggle
**/
void BcBulmaCont::slotViewStatusBar ( bool toggle )
{
    blDebug ( "BcBulmaCont::slotViewStatusBar", 0 );
    statusBar() ->showMessage ( _ ( "Cambinado barra de estado..." ) );
    if ( toggle == false ) {
        statusBar() ->hide();
    } else {
        statusBar() ->show();
    } // end if
    statusBar() ->showMessage ( _ ( "Listo." ) );
    blDebug ( "END BcBulmaCont::slotViewStatusBar", 0 );
}


///
/**
\param toggle
**/
void BcBulmaCont::slotViewFullScreen ( bool toggle )
{
    blDebug ( "BcBulmaCont::slotViewFullScreen", 0 );
    statusBar() ->showMessage ( _ ( "Cambiando a modo de pantalla completa..." ) );
    if ( toggle == false ) {
        showNormal();
    } else {
        showFullScreen();
    } // end if
    statusBar() ->showMessage ( _ ( "Listo." ) );
    blDebug ( "END BcBulmaCont::slotViewFullScreen", 0 );
}


///
/**
\param text
**/
void BcBulmaCont::slotStatusHelpMsg ( const QString &text )
{
    blDebug ( "BcBulmaCont::slotStatusHelpMsg", 0 );
    /// Change status message of whole statusbar temporary (text, msec).
    statusBar() ->showMessage ( text, 2000 );
    blDebug ( "END BcBulmaCont::slotStatusHelpMsg", 0 );
}


///
/**
**/
void BcBulmaCont::slotListadoCuentas()
{
    blDebug ( "BcBulmaCont::slotListadoCuentas", 0 );
    statusBar() ->showMessage ( _ ( "Listado de cuentas" ) );
    blDebug ( "END BcBulmaCont::slotListadoCuentas", 0 );
}



///
/**
**/
void BcBulmaCont::slotCentrosCoste()
{
    blDebug ( "BcBulmaCont::slotCentrosCoste", 0 );
    m_empresaactual->ccostes();
    blDebug ( "END BcBulmaCont::slotCentrosCoste", 0 );
}


///
/**
**/
void BcBulmaCont::slotCanales()
{
    blDebug ( "BcBulmaCont::slotCanales", 0 );
    m_empresaactual->canales();
    blDebug ( "END BcBulmaCont::slotCanales", 0 );
}


///
/**
**/
void BcBulmaCont::slotCompBalance()
{
    blDebug ( "BcBulmaCont::slotCompBalance", 0 );
    m_empresaactual->compbalance();
    blDebug ( "END BcBulmaCont::slotCompBalance", 0 );
}


///
/**
**/
void BcBulmaCont::slotPerdidas()
{
    blDebug ( "BcBulmaCont::slotPerdidas", 0 );
    blDebug ( "END BcBulmaCont::slotPerdidas", 0 );
}


///
/**
**/
void BcBulmaCont::windowMenuAboutToShow()
{
    blDebug ( "BcBulmaCont::windowMenuAboutToShow", 0 );
    blDebug ( "END BcBulmaCont::windowMenuAboutToShow", 0 );
}


///
/**
**/
void BcBulmaCont::slotWindowNewWindow()
{
    blDebug ( "BcBulmaCont::slotWindowNewWindow", 0 );
    blDebug ( "END BcBulmaCont::slotWindowNewWindow", 0 );
}


///
/**
**/
void BcBulmaCont::closeEvent ( QCloseEvent *event )
{
    blDebug ( "BcBulmaCont::closeEvent", 0 );


    /// Antes de salir hacemos un mensaje de advertencia.
    QMessageBox msgBox;
    msgBox.setText(_("Seguro que desea abandonar el programa "));
    msgBox.setInformativeText(_("Se perderan los cambios no guardados"));
    msgBox.setStandardButtons( QMessageBox::Close | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Cancel) {
      event->ignore();
      blDebug ( "END BfBulmaFact::closeEvent", 0 );
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

    blDebug ( "END BcBulmaCont::closeEvent", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionAyuda_triggered()
{
    blDebug ( "BcBulmaCont::on_actionAyuda_triggered", 0 );
    QAssistantClient *asistenteAyuda = new QAssistantClient ( QLibraryInfo::location ( QLibraryInfo::BinariesPath ), 0 );
    connect ( asistenteAyuda, SIGNAL ( error ( const QString ) ), this, SLOT ( documentacionError ( const QString ) ) );
    QStringList parametros;
    parametros << "-profile" << QString ( g_confpr->valor ( CONF_PROGDATA ) + "ayuda/bulmacont/bulmacont.adp" );
    asistenteAyuda->setArguments ( parametros );
    asistenteAyuda->openAssistant();
    blDebug ( "END BcBulmaCont::on_actionAyuda_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionAcerca_de_triggered()
{
    blDebug ( "BcBulmaCont::on_actionAcerca_de_triggered", 0 );
    BcAboutView *sobre = new BcAboutView ( 0 );
    sobre->exec();
    blDebug ( "END BcBulmaCont::on_actionAcerca_de_triggered", 0 );
}


///
/**
**/
void BcBulmaCont::on_actionPaises_triggered()
{
    blDebug ( "BcBulmaCont::on_actionPaises_triggered", 0 );
    m_empresaactual->muestraPaises();
    blDebug ( "BcBulmaCont::on_actionPaises_triggered", 0 );
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
    blDebug ( "BcBulmaCont::informaindexador", 0 );
    /// No existe una ventana que activar.
    if ( m_empresaactual == NULL ) {
        blDebug ( "END BcBulmaCont::informaindexador", 0, "Sin empresa" );
        return;
    } // end if

    if ( w == NULL ) {
        m_empresaactual->deSeleccionaWindow();
        blDebug ( "END BcBulmaCont::informaindexador", 0, "Sin Widget" );
        return;
    } // end if
    m_empresaactual->deSeleccionaWindow();
    m_empresaactual->seleccionaWindow ( w->windowTitle(), w );

    QString texto = "Window activated. " + w->windowTitle() + "\n";
    printf ( "%s", texto.toAscii().constData() );

    blDebug ( "END BcBulmaCont::informaindexador", 0 );
}


///
/**
\return
**/
BlWorkspace *BcBulmaCont::workspace()
{
    blDebug ( "BcBulmaCont::workspace", 0 );
    blDebug ( "END BcBulmaCont::workspace", 0 );
    return m_pWorkspace;
}



