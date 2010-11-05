/**************************************************************************
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
// #include "bcplancontablelistview.h"
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
#include "bcmodelo347listview.h"
#include "blcountryview.h"
#include "bcbulmacont.h"

#ifndef Q_OS_WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif


/// SLOT que muestra la ventana de asientos.
/**
**/
void BcCompany ::s_asiento1()
{
    blDebug ( "BcCompany ::s_asiento1", 0 );

/*
    introapunts2->setWindowState ( ( introapunts2->windowState() &  ~Qt::WindowMinimized ) | Qt::WindowActive );
*/
    blDebug ( "BcCompany ::s_asiento1", 0 );
}


///
/**
\param bcont
**/
BcCompany ::BcCompany ( BcBulmaCont *bcont ) : BlMainCompany()
{
    blDebug ( "BcCompany ::BcCompany ", 0 );
    m_bulmacont = bcont;
    selccostes  = NULL;
    selcanales = NULL;
/*
    introapunts2 = NULL;
    m_listasientos = NULL;
*/
    nombre = "";
    blDebug ( "END BcCompany ::BcCompany", 0 );
}


///
/**
**/
BcCompany::~BcCompany()
{
    blDebug ( "BcCompany::~BcCompany", 0 );
    guardaConf();
    /// Borramos todas las ventanas.
    m_listventanas->vaciarCompleto();
    blDebug ( "END BcCompany::~BcCompany", 0 );
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
/*
BcExtractoView *BcCompany::extractoempresa()
{
    return extracto;
}
*/

///
/**
\return
**/
/*
BcDiarioView *BcCompany::diarioempresa()
{

    return diario;

}
*/

///
/**
\return
**/
/*
BcAsientoView *BcCompany::intapuntsempresa()
{
    return introapunts2;
}
*/

///
/**
\return
**/
/*
BcAsientoView *BcCompany::intapuntsempresa2()
{
    return introapunts2;
}
*/

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
    blDebug ( "BcCompany::createMainWindows", 0 );
    try {
        /// Calculamos el n&uacute;mero de d&iacute;gitos que tiene el nivel &uacute;ltimo
        /// de la Empresa.
        QString query = "SELECT length(valor) AS numdigitos FROM configuracion WHERE nombre = 'CodCuenta'";
        BlDbRecordSet *cursoraux1 = loadQuery ( query );
        numdigitos = cursoraux1->valor ( "numdigitos" ).toInt();
        delete cursoraux1;
        if ( selccostes != NULL ) {
            delete selccostes;
            delete selcanales;
        } // end if

        /// Inicializamos los selectores de centros de coste y canales.
        selccostes = new BcCentroCosteSeleccionarView ( this, 0 );
        selcanales = new BcCanalSeleccionarView ( this, 0 );

        /// Inicializamos las ventanas de uso generalizado.

        /// pb = 90%
/*
        splash->mensaje ( _ ( "Inicializando cuentas" ) );
        splash->setBarraProgreso ( 90 );
        m_progressbar->setValue ( 90 );
        m_listcuentas = new BcPlanContableListView ( this, 0 );
        m_listcuentas->inicializa();
        m_pWorkspace->addWindow ( m_listcuentas );
*/
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

	/// Ponemos el titulo de la ventana
	m_bulmacont->statusBar() ->showMessage ( dbName(), 2000 );
	m_bulmacont->setWindowTitle ( _ ( "Contabilidad GPL" ) + " :: " + dbName() );


    } catch ( ... ) {
        blDebug ( "Error al iniciar la clase company", 2 );
    } // end try

    blDebug ( "END BcCompany::createMainWindows", 0 );
    return ( 0 );
}


/// Maximiza todas las ventanas que est&aacute;n en el workspace de la aplicaci&oacute;n.
/** Env&iacute;a un mensaje showMaximized a todos los objetos que deben ser maximizados. */
/**
**/
void BcCompany::maximiza()
{
    blDebug ( "BcCompany::maximiza", 0 );
//    m_listcuentas->showMaximized();
    blDebug ( "END BcCompany::maximiza", 0 );
}


/// Esta funci&oacute;n abre la ventana que presenta las cuentas contables.
/**
\return
**/
int BcCompany::muestracuentas()
{
    blDebug ( "BcCompany::muestracuentas", 0 );
//    m_listcuentas->setWindowState ( Qt::WindowActive );
    blDebug ( "END BcCompany::muestracuentas", 0 );
    return 0;
}


/// Esta funci&oacute;n presenta los centros de coste.
/** Luego recarga los comboboxes de todo el resto de pantallas para que se actualizen. */
/**
\return
**/
int BcCompany::ccostes()
{
    blDebug ( "BcCompany::ccostes", 0 );
    BcCentroCosteView *ccoste = new BcCentroCosteView ( this, 0 );
    m_pWorkspace->addWindow ( ccoste );
    ccoste->show();
    blDebug ( "END BcCompany::ccostes", 0 );
    return 0;
}


/// Esta funci&oacute;n presenta la pantalla de edicion de canales.
/** Luego recarga los comboboxes de todo el resto de pantallas para que se actualizen. */
/**
\return
**/
int BcCompany::canales()
{
    blDebug ( "BcCompany::canales", 0 );
    BcCanalView *canal = new BcCanalView ( this, 0 );
    m_pWorkspace->addWindow ( canal );
    canal->show();
    blDebug ( "END BcCompany::canales", 0 );
    return 0;
}





/// Crea la ventana de tipos de IVA y la presenta en modo modal.
/** Es invocada desde la clase principal. */
/**
\return
**/
int BcCompany::tiposIVA()
{
    blDebug ( "BcCompany::tiposIVA", 0 );
    BcTipoIVAView *tip = new BcTipoIVAView ( this, 0 );
    m_pWorkspace->addWindow ( tip );
    tip->show();
    blDebug ( "END BcCompany::tiposIVA", 0 );
    return 0;
}


/// Crea la ventana de Formas de Pago y la presenta en modo modal.
/** Es invocada desde la clase principal. */
/**
\return
**/
int BcCompany::fPago()
{
    blDebug ( "BcCompany::fPago", 0 );
    BcFormaPagoView *fp = new BcFormaPagoView ( this, 0 );
    m_pWorkspace->addWindow ( fp );
    fp->show();
    blDebug ( "END BcCompany::fPago", 0 );
    return 0;
}


///
/**
\return
**/
int BcCompany::cambioejercicio()
{
    blDebug ( "BcCompany::cambioejercicio", 0 );
    /// El ejercicio ha cambiado y recargamos el cursor de asientos del nuevo ejercicio.
/*

    introapunts2->cargaasientos();
    introapunts2->boton_fin();

*/
    blDebug ( "END BcCompany::cambioejercicio", 0 );
    return 0;
}






///
/**
\return
**/
int BcCompany::propiedadempresa()
{
    blDebug ( "BcCompany::propiedadempresa", 0 );
    BcConfiguracionView *nuevae = new BcConfiguracionView ( this, 0 );
    m_pWorkspace->addWindow ( nuevae );
    nuevae->show();
    blDebug ( "END BcCompany::propiedadempresa", 0 );
    return 0;
}


///
/**
\return
**/
int BcCompany::amortizaciones()
{
    blDebug ( "BcCompany::amortizaciones", 0 );
/*
    BcAmortizacionListView *amors = new BcAmortizacionListView ( this, 0 );
    m_pWorkspace->addWindow ( amors );
    amors->show();
*/
    blDebug ( "END BcCompany::amortizaciones", 0 );
    return 0;
}


/// Esta funci&oacute;n llama a la pantalla de creacion, modificaci&oacute;n
/// de masas patrimoniales.
/**
\return
**/
int BcCompany::mpatrimoniales()
{
    blDebug ( "BcCompany::mpatrimoniales", 0 );
    BcMasaPatrimonialListView *nuevae = new BcMasaPatrimonialListView ( this, 0 );
    nuevae->inicializa();
    nuevae->exec();
    delete nuevae;
    blDebug ( "END BcCompany::mpatrimoniales", 0 );
    return 0;
}


/// Esta funci&oacute;n llama a la pantalla de creacion, modificaci&oacute;n
/// de las cuentas anuales
/**
\return
**/
int BcCompany::compbalance()
{
    blDebug ( "BcCompany::compbalance", 0 );
/*
    BcCuentasAnualesView *nueva = new BcCuentasAnualesView ( this, 0 );
    m_pWorkspace->addWindow ( nueva );
    nueva->show();
*/
    blDebug ( "END BcCompany::compbalance", 0 );
    return 0;
}


///
///
/**
**/
void BcCompany::cerrarAsientos()
{
    blDebug ( "BcCompany::cerrarAsientos", 0 );
    BlDbRecordSet *cur = NULL;
    try {
        cur = loadQuery ( "SELECT cierraasiento(idasiento) FROM asiento" );
        g_main->statusBar() ->showMessage ( _ ( "Se han cerrado los asientos" ), 2000 );
        delete cur;
        blDebug ( "END BcCompany::cerrarAsientos", 0 );
    } catch ( ... ) {
        blMsgError ( "Ha habido un error al cerrar los asientos" );
        if ( cur ) delete cur;
        return;
    } // end try
}



///
/**
\return
**/
int BcCompany::listado347()
{
    blDebug ( "BcCompany::modelo347", 0 );
    BcModelo347ListView *dlg347 = new BcModelo347ListView ( this, "0" );
    dlg347->exec();
    delete dlg347;
    blDebug ( "END BcCompany::modelo347", 0 );
    return 0;
}


/// Esta funci&oacute;n reemplaza en los apuntes todas las ocurrencias de una
/// determinada cuenta por otra cuenta.
/**
**/
void BcCompany::reemplazacuentaenasientos()
{
    blDebug ( "BcCompany::reemplazacuentaenasientos", 0 );
    BcCambiaCuentaView *ctac = new BcCambiaCuentaView ( this, 0, false );
    ctac->exec();
    blDebug ( "END BcCompany::reemplazacuentaenasientos", 0 );
}


/// Esta funci&oacute;n recalcula los saldos parciales del plan contable.
/**
**/
void BcCompany::recalculasaldos()
{
    blDebug ( "BcCompany::recalculasaldos", 0 );
    blMsgInfo ( "recalcular Saldos" );
    BlDbRecordSet *cur = loadQuery ( "SELECT recalculasaldos()" );
    delete cur;
    blDebug ( "END BcCompany::recalculasaldos", 0 );
}


///
/**
\return
**/
BcCentroCosteSeleccionarView *BcCompany::getselccostes()
{
    blDebug ( "BcCompany::getselccostes", 0 );
    blDebug ( "END BcCompany::getselccostes", 0 );
    return selccostes;
}


///
/**
\return
**/
BcCanalSeleccionarView *BcCompany::getselcanales()
{
    blDebug ( "BcCompany::getselcanales", 0 );
    blDebug ( "END BcCompany::getselcanales", 0 );
    return selcanales;
}


///
/**
**/
void BcCompany::centrocostedefecto()
{
    blDebug ( "BcCompany::centrocostedefecto", 0 );
    selccostes->exec();
    blDebug ( "END BcCompany::centrocostedefecto", 0 );
}


///
/**
**/
void BcCompany::canaldefecto()
{
    blDebug ( "BcCompany::canaldefecto", 0 );
    selcanales->exec();
    blDebug ( "END BcCompany::canaldefecto", 0 );
}


/// Guarda la configuracion de programa para poder recuperar algunas cosas de presentacion.
/**
**/
void BcCompany::guardaConf()
{
    blDebug ( "BcCompany::guardaConf", 0 );

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
    blDebug ( "END BcCompany::guardaConf", 0 );
}

/// Guarda la configuracion de programa para poder recuperar algunas cosas de presentacion.
/**
\return
**/
void BcCompany::cargaConf()
{
    blDebug ( "BcCompany::cargaConf", 0 );


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
    blDebug ( "END BcCompany::cargaConf", 0 );
}


