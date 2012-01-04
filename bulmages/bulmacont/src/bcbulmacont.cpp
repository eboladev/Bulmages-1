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

#ifndef Q_OS_WIN32
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
    BL_FUNC_DEBUG
    setupUi ( this );

    m_pWorkspace = new BlWorkspace ( this );
    
#ifdef AREA_QMDI
    m_pWorkspace->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pWorkspace->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
#else
    m_pWorkspace->setScrollBarsEnabled ( TRUE );
#endif

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


    m_company = new BcCompany ( this );
    m_company->setProgressBar ( m_pb );
    m_company->init ( DB, "BulmaCont" );
    m_company->setWorkspace ( m_pWorkspace );

#ifdef AREA_QMDI
    connect ( m_pWorkspace, SIGNAL ( subWindowActivated ( QMdiSubWindow * ) ), this, SLOT ( informaindexador ( QMdiSubWindow * ) ) );
#else
    connect ( m_pWorkspace, SIGNAL ( windowActivated ( QWidget * ) ), this, SLOT ( informaindexador ( QWidget * ) ) );
#endif
    
    /// Aqu&iacute; creamos la ventana dock para meter las distintas ventanas.
    m_list = new BlWindowListDock ( 0 );
    m_list->setVisible ( FALSE );

    /// Iniciamos el listventanas con el workspace para que pueda operar con el.
    m_list->setWorkspace ( m_pWorkspace );

    connect ( m_list, SIGNAL ( visibilityStateChanged ( bool ) ), this, SLOT ( setActionIndexador ( bool ) ) );

    addDockWidget ( Qt::LeftDockWidgetArea, m_list );

    m_company->setListVentanas ( m_list );

    m_list->setVisible ( TRUE );
    m_pb->setVisible ( FALSE );

    initStatusBar();
    statusBar() ->showMessage ( DB, 2000 );
    setWindowTitle ( DB );
    
}


///
/**
**/
BcBulmaCont::~BcBulmaCont()
{
    BL_FUNC_DEBUG
    delete m_pWorkspace;
    
}


///
/**
\return
**/
BcCompany *BcBulmaCont::company()
{
    BL_FUNC_DEBUG
    
    return m_company;
}


/// Exporta menues.
/**
\return
**/
QMenu *BcBulmaCont::mui_MenuHerramientas()
{
    BL_FUNC_DEBUG
    
    return menuHerramientas;
}


///
/**
**/
void BcBulmaCont::s_asiento1()
{
    BL_FUNC_DEBUG
    m_company->s_asiento1();
    
}


///
/**
**/
void BcBulmaCont::on_actionTipos_de_IVA_triggered()
{
    BL_FUNC_DEBUG
    m_company->tiposIVA();
    
}


///
/**
**/
void BcBulmaCont::on_actionCerrar_Asientos_triggered()
{
    BL_FUNC_DEBUG
    m_company->cerrarAsientos();
    
}

///
/**
**/
void BcBulmaCont::on_actionFormas_de_Pago_triggered()
{
    BL_FUNC_DEBUG
    m_company->fPago();
}


///
/**
**/
void BcBulmaCont::on_actionConfiguracion_triggered()
{
    BL_FUNC_DEBUG
    m_company->propiedadempresa();
}


///
/**
**/
void BcBulmaCont::on_actionRecalcular_Saldos_Iniciales_triggered()
{
    BL_FUNC_DEBUG
    m_company->recalculasaldos();
}


///
/**
**/
void BcBulmaCont::on_actionSustituir_Cuentas_triggered()
{
    BL_FUNC_DEBUG
    m_company->reemplazacuentaenasientos();
}


///
/**
**/
void BcBulmaCont::on_actionCanal_por_Defecto_triggered()
{
    BL_FUNC_DEBUG
    m_company->canaldefecto();
}

///
/**
**/
void BcBulmaCont::on_actionCentros_de_Coste_triggered()
{
    BL_FUNC_DEBUG
    m_company->ccostes();
}


///
/**
**/
void BcBulmaCont::on_actionCanales_triggered()
{
    BL_FUNC_DEBUG
    m_company->canales();
}


///
/**
**/
void BcBulmaCont::on_actionCentro_de_Coste_por_Defecto_triggered()
{
    BL_FUNC_DEBUG
    m_company->centrocostedefecto();
}


///
/**
**/
void BcBulmaCont::on_actionOrdenar_Ventanas_triggered()
{
    BL_FUNC_DEBUG
    
#ifdef AREA_QMDI
    m_pWorkspace->tileSubWindows();
#else
    m_pWorkspace->tile();
#endif
    
    
}


///
/**
**/
void BcBulmaCont::on_actionOrganizaci_n_en_Cascada_triggered()
{
    BL_FUNC_DEBUG

#ifdef AREA_QMDI
    m_pWorkspace->cascadeSubWindows ();
#else
    m_pWorkspace->cascade();
#endif

}


///
/**
**/
void BcBulmaCont::on_actionMasas_Patrimoniales_triggered()
{
    BL_FUNC_DEBUG
    m_company->mpatrimoniales();
}


///
/**
**/
void BcBulmaCont::on_actionPantalla_Completa_triggered()
{
    BL_FUNC_DEBUG
    if ( isFullScreen() ) {
        showNormal();
    } else {
        showFullScreen();
    } // end if
}


///
/**
**/
void BcBulmaCont::on_actionIndexador_triggered()
{
    BL_FUNC_DEBUG
    if ( actionIndexador->isChecked() == TRUE ) {
        m_company->s_indexadorCambiaEstado ( TRUE );
    } else {
        m_company->s_indexadorCambiaEstado ( FALSE );
    } // end if
}


///
/**
\param visible
**/
void BcBulmaCont::setActionIndexador ( bool visible )
{
    BL_FUNC_DEBUG
    if ( visible == TRUE ) {
        actionIndexador->setChecked ( TRUE );
    } else {
        actionIndexador->setChecked ( FALSE );
    } // end if
}


///
/**
**/
void BcBulmaCont::initStatusBar()
{
    BL_FUNC_DEBUG
    statusBar() ->showMessage ( _ ( "Listo." ) );
}


///
/**
\param object
\param event
\return
**/
bool BcBulmaCont::eventFilter ( QObject *object, QEvent *event )
{
    BL_FUNC_DEBUG
    /// Standard event processing.
    return QWidget::eventFilter ( object, event );
}


///
/**
**/
void BcBulmaCont::slotEditUndo()
{
    BL_FUNC_DEBUG
    statusBar() ->showMessage ( _ ( "Deshaciendo la ultima accion..." ) );
    statusBar() ->showMessage ( _ ( "Listo." ) );
}


///
/**
**/
void BcBulmaCont::slotEditCut()
{
    BL_FUNC_DEBUG
    statusBar() ->showMessage ( _ ( "Cortando seleccion..." ) );
    statusBar() ->showMessage ( _ ( "Listo." ) );
}


///
/**
**/
void BcBulmaCont::slotEditCopy()
{
    BL_FUNC_DEBUG
    statusBar() ->showMessage ( _ ( "Copiando la seleccion al portapapeles..." ) );
    statusBar() ->showMessage ( _ ( "Listo." ) );
}


///
/**
**/
void BcBulmaCont::slotEditPaste()
{
    BL_FUNC_DEBUG
    statusBar() ->showMessage ( _ ( "Insertando el contenido del portapapeles..." ) );
    statusBar() ->showMessage ( _ ( "Listo." ) );
}


///
/**
\param toggle
**/
void BcBulmaCont::slotViewStatusBar ( bool toggle )
{
    BL_FUNC_DEBUG
    statusBar() ->showMessage ( _ ( "Cambinado barra de estado..." ) );
    if ( toggle == false ) {
        statusBar() ->hide();
    } else {
        statusBar() ->show();
    } // end if
    statusBar() ->showMessage ( _ ( "Listo." ) );
}


///
/**
\param toggle
**/
void BcBulmaCont::slotViewFullScreen ( bool toggle )
{
    BL_FUNC_DEBUG
    statusBar() ->showMessage ( _ ( "Cambiando a modo de pantalla completa..." ) );
    if ( toggle == false ) {
        showNormal();
    } else {
        showFullScreen();
    } // end if
    statusBar() ->showMessage ( _ ( "Listo." ) );
}


///
/**
\param text
**/
void BcBulmaCont::slotStatusHelpMsg ( const QString &text )
{
    BL_FUNC_DEBUG
    /// Change status message of whole statusbar temporary (text, msec).
    statusBar() ->showMessage ( text, 2000 );
}


///
/**
**/
void BcBulmaCont::slotListadoCuentas()
{
    BL_FUNC_DEBUG
    statusBar() ->showMessage ( _ ( "Listado de cuentas" ) );
}



///
/**
**/
void BcBulmaCont::slotCentrosCoste()
{
    BL_FUNC_DEBUG
    m_company->ccostes();
}


///
/**
**/
void BcBulmaCont::slotCanales()
{
    BL_FUNC_DEBUG
    m_company->canales();
}


///
/**
**/
void BcBulmaCont::slotPerdidas()
{
    BL_FUNC_DEBUG
}


///
/**
**/
void BcBulmaCont::windowMenuAboutToShow()
{
    BL_FUNC_DEBUG
}


///
/**
**/
void BcBulmaCont::slotWindowNewWindow()
{
    BL_FUNC_DEBUG
}


///
/**
**/
void BcBulmaCont::closeEvent ( QCloseEvent *event )
{
    BL_FUNC_DEBUG
    /// Antes de salir hacemos un mensaje de advertencia.
    if ( g_confpr->value( CONF_ASK_BEFORE_EXIT ) == "TRUE" ) {
	 QMessageBox msgBox;
	 msgBox.setText(_("Seguro que desea abandonar el programa "));
	 msgBox.setInformativeText(_("Se perderan los cambios no guardados"));
	 msgBox.setStandardButtons( QMessageBox::Close | QMessageBox::Cancel);
	 msgBox.setDefaultButton(QMessageBox::Cancel);
	 int ret = msgBox.exec();

	 if (ret == QMessageBox::Cancel) {
	   event->ignore();
	   
	   return;
	 }
    } // END IF

    g_plugins->lanza ( "BcBulmaCont_closeEvent", this );

    delete m_company;
    m_company = NULL;
    delete m_list;
    m_list = NULL;
}


///
/**
**/
void BcBulmaCont::on_actionAyuda_triggered()
{
    BL_FUNC_DEBUG
    blWebBrowser("http://www.iglues.org/wiki_users/doku.php");
  /*
    QAssistantClient *asistenteAyuda = new QAssistantClient ( QLibraryInfo::location ( QLibraryInfo::BinariesPath ), 0 );
    connect ( asistenteAyuda, SIGNAL ( error ( const QString ) ), this, SLOT ( documentacionError ( const QString ) ) );
    QStringList parametros;
    parametros << "-profile" << QString ( g_confpr->value( CONF_PROGDATA ) + "ayuda/bulmacont/bulmacont.adp" );
    asistenteAyuda->setArguments ( parametros );
    asistenteAyuda->openAssistant();
    */
}


///
/**
**/
void BcBulmaCont::on_actionAcerca_de_triggered()
{
    BL_FUNC_DEBUG
    BcAboutView *sobre = new BcAboutView ( 0 );
    sobre->exec();
}


///
/**
**/
void BcBulmaCont::on_actionPaises_triggered()
{
    BL_FUNC_DEBUG
    m_company->muestraPaises();
    
}



/** Captura el evento de cambio de ventana en el workSpace y actua sobre el
    listado de ventanas para que seleccione la ventana adecuada.
*/
///
/**
\param w
\return
**/
#ifdef AREA_QMDI
  void BcBulmaCont::informaindexador ( QMdiSubWindow *w )
#else
  void BcBulmaCont::informaindexador ( QWidget *w )
#endif
{
    BL_FUNC_DEBUG
    /// No existe una ventana que activar.
    if ( m_company == NULL ) {
	
        return;
    } // end if

    if ( w == NULL ) {
        m_company->deselectWindow();
	
        return;
    } // end if
    m_company->deselectWindow();
    m_company->selectWindow ( w->windowTitle(), w );

    QString texto = "Window activated. " + w->windowTitle() + "\n";
    printf ( "%s", texto.toAscii().constData() );
}


///
/**
\return
**/
BlWorkspace *BcBulmaCont::workspace()
{
    BL_FUNC_DEBUG
    return m_pWorkspace;
}

