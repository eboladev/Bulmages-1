/***************************************************************************
 *   Copyright (C) 2004 by J. M. Torres Rigo                               *
 *   joanmi@bulma.net                                                      *
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

#include <Qt>
#include <QObject>
#include <qnamespace.h>
#include <QFile>
#include <QTextStream>
#include <QDomDocument>

#include "stdio.h"
#include "blcompanydialog.h"
#include "company.h"
#include "fpagoview.h"
#include "blfunctions.h"
#include "listconfiguracionview.h"
#include "listseriefacturaview.h"
#include "plugins.h"
#include "bancoview.h"
#include "bulmafact.h"


/** No precisa de operaciones en su construccion.
*/
/**
\param bges
**/
Company::Company ( Bulmafact *bges ) : BlMainCompany()
{
    _depura ( "Company::Company", 0 );
    m_bulmafact = bges;
    _depura ( "END Company::Company", 0 );
}


/// El destructor de la clase Company borra toda la memoria almacenada.
/**
**/
Company::~Company()
{
    _depura ( "Company::~Company", 0 );

    /// Guardamos la configuracion.
    guardaConf();

    /// Borramos las ventanas flotantes antes de eliminar esta clase ya que se produciria un segfault
    m_listventanas->vaciarCompleto();
    _depura ( "END Company::~Company", 0 );
}


/** Crea todas las ventanas que aparecen creadas al inicio del programa.
    Todas estas ventanas son principalmente los listados mas utilizados a partir de los
    cuales se pueden crear nuevas fichas y manejar todo.
*/
/**
\param splash
\return
**/
void Company::createMainWindows ( BlSplashScreen *splash )
{
    _depura ( "Company::createMainWindows", 0 );

    /// Comprobamos que tengamos permisos para trabajar con 'Paises'.
    m_bulmafact->actionPaises->setEnabled ( FALSE );

    if ( has_table_privilege ( "pais", "SELECT" ) ) {
        m_bulmafact->actionPaises->setEnabled ( TRUE );
    } // end if

    /// Comprobamos que tengamos permisos para trabajar con 'Formas de pago'.
    m_bulmafact->actionFormas_de_Pago->setEnabled ( FALSE );

    if ( has_table_privilege ( "forma_pago", "SELECT" ) ) {
        m_bulmafact->actionFormas_de_Pago->setEnabled ( TRUE );
    } // end if

    /// Comprobamos que tengamos permisos para trabajar con 'Series de factura'.
    m_bulmafact->actionSeries_de_Factura->setEnabled ( FALSE );

    if ( has_table_privilege ( "serie_factura", "SELECT" ) ) {
        m_bulmafact->actionSeries_de_Factura->setEnabled ( TRUE );
    } // end if

    /// Comprobamos que tengamos permisos para trabajar con 'Bancos'.
    m_bulmafact->actionBancos->setEnabled ( FALSE );

    if ( has_table_privilege ( "banco", "SELECT" ) ) {
        m_bulmafact->actionBancos->setEnabled ( TRUE );
    } // end if

    /// pb = 100%
    m_progressbar->setValue ( 100 );

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "Company_createMainWindows_Post", this );
    if ( res != 0 ) {
        return;
    } // end if

    cargaConf();

    /// Ponemos el titulo de la ventana
    m_bulmafact->statusBar() ->showMessage ( nameDB(), 2000 );
    m_bulmafact->setWindowTitle ( _( "Facturacion GPL" ) + " :: " + nameDB() );

    _depura ( "END Company::createMainWindows", 0 );
}



/** Crea y Presenta la ventana de formas de pago.
*/
/// \TODO: Deberia dividirse en dos y deberia pasar por sistema de plugins.
/**
**/
void Company::s_FPago()
{
    _depura ( "Company::s_FPago", 0 );
    FPagoView *f = new FPagoView ( this, NULL );
    m_pWorkspace->addWindow ( f );
    f->show();
    _depura ( "END Company::s_FPago", 0 );
}


/** Crea y Presenta la ventana de trabajadores
*/
/// \TODO: Deberia dividirse en dos y deberia pasar por sistema de plugins.
/**
**/
void Company::s_trabajadores()
{
    _depura ( "Company::s_trabajadores", 0 );
//    TrabajadorView *t = new TrabajadorView ( this, NULL );
//    m_pWorkspace->addWindow ( t );
//    t->show();
    _depura ( "END Company::s_trabajadores", 0 );
}


/** Crea y Presenta la ventana de bancos
*/
/// \TODO: Deberia dividirse en dos y deberia pasar por sistema de plugins.
/**
**/
void Company::s_bancos()
{
    _depura ( "Company::s_bancos", 0 );
    BancoView *t = new BancoView ( this, NULL );
    m_pWorkspace->addWindow ( t );
    t->show();
    _depura ( "END Company::s_bancos", 0 );
}


/** Crea y Presenta la ventana de Series de Factura
*/
/// \TODO: Deberia dividirse en dos y deberia pasar por sistema de plugins.
/**
**/
void Company::s_seriesFactura()
{
    _depura ( "Company::s_seriesFactura", 0 );
    ListSerieFacturaView *lser = new ListSerieFacturaView ( this, 0 );
    m_pWorkspace->addWindow ( lser );
    lser->show();
    _depura ( "END Company::s_seriesFactura", 0 );
}





/** Crea una instancia de ListAlmacenView y la presenta.
*/
/**
\return
**/
void Company::s_almacenes()
{
    _depura ( "Company::s_almacenes", 0 );
/*
    /// Lanzamos los plugins necesarios.
    if ( g_plugins->lanza ( "Company_s_almacenes", this ) ) {
        _depura ( "END Company::s_almacenes", 0, "Salida por plugin" );
        return;
    } // end if
    ListAlmacenView *lser = new ListAlmacenView ( this, 0 );
    m_pWorkspace->addWindow ( lser );
    lser->show();
*/
    _depura ( "END Company::s_almacenes", 0 );
}


/** Crea una instancia de ListConfiguracionView y la presenta.
*/
/**
**/
void Company::s_newListConfiguracionView()
{
    _depura ( "Company::s_newListConfiguracionView", 0 );
    ListConfiguracionView *lser = new ListConfiguracionView ( this, 0 );
    m_pWorkspace->addWindow ( lser );
    lser->show();
    _depura ( "END Company::s_newListConfiguracionView", 0 );
}


/** Crea y muestra el listado de Tipos de IVA
*/
/**
**/
void Company::s_TipoIVAView()
{
    _depura ( "Company::s_TipoIVAView", 0 );
/*
    ListTipoIVAView *pag = new ListTipoIVAView ( this, 0 );
    m_pWorkspace->addWindow ( pag );
    pag->show();
*/
    _depura ( "END Company::s_TipoIVAView", 0 );
}


/** Crea y muestra el listado de Tasas de IVA
*/
/**
**/
void Company::s_TasaIVAView()
{
    _depura ( "Company::s_TasaIVAView", 0 );
/*
    ListTasaIVAView *pag = new ListTasaIVAView ( this, 0 );
    m_pWorkspace->addWindow ( pag );
    pag->show();
*/
    _depura ( "END Company::s_TasaIVAView", 0 );
}


/// Guarda la configuracion de programa para poder recuperar algunas cosas de presentacion.
/**
**/
void Company::guardaConf()
{
    _depura ( "Company::guardaConf", 0 );
    QFile file ( confpr->valor ( CONF_DIR_USER ) + "bulmafact_" + nameDB() + ".cfn" );
    /// Guardado del orden y de configuraciones varias.
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << "<CONFIG>\n";
        stream << "\t<PRINCIPAL>\n";
        stream << "\t\t\t<X>" + QString::number ( m_bulmafact->x() ) + "</X>\n";
        stream << "\t\t\t<Y>" + QString::number ( m_bulmafact->y() ) + "</Y>\n";
        stream << "\t\t\t<WIDTH>" + QString::number ( m_bulmafact->width() ) + "</WIDTH>\n";
        stream << "\t\t\t<HEIGHT>" + QString::number ( m_bulmafact->height() ) + "</HEIGHT>\n";
        stream << "\t\t\t<INDEXADOR>" + ( m_bulmafact->actionIndexador->isChecked() ? QString ( "TRUE" ) : QString ( "FALSE" ) ) + "</INDEXADOR>\n";
	stream << "\t\t\t<TOOLBARSDOCKWIDGETS>" + QString(m_bulmafact->saveState().toBase64()) + "</TOOLBARSDOCKWIDGETS>\n";
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
            stream << "\t\t<VACTIVEWINDOW>" + ( m_bulmafact->workspace() ->activeWindow() == wid ? QString ( "TRUE" ) : QString ( "FALSE" ) ) + "</VACTIVEWINDOW>\n";
            stream << "\t</VENTANA>\n";
        } // end for

        stream << "</CONFIG>\n";
        file.close();
    } // end if
    _depura ( "END Company::guardaConf", 0 );
}


/// Guarda la configuracion de programa para poder recuperar algunas cosas de presentacion.
/**
\return
**/
void Company::cargaConf()
{
    _depura ( "Company::cargaConf", 0 );
    QFile file ( confpr->valor ( CONF_DIR_USER ) + "bulmafact_" + nameDB() + ".cfn" );
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
    m_bulmafact->setGeometry ( nx.toInt(), ny.toInt(), nwidth.toInt(), nheight.toInt() );

    /// Cogemos el indexador
    QString indexador = principal.firstChildElement ( "INDEXADOR" ).toElement().text();
    if ( indexador == "TRUE" ) {
        s_indexadorCambiaEstado ( TRUE );
        m_bulmafact->actionIndexador->setChecked ( TRUE );
    } else {
        s_indexadorCambiaEstado ( FALSE );
        m_bulmafact->actionIndexador->setChecked ( FALSE );
    } // end if

    /// Cogemos el ancho del indexador
    m_bulmafact->restoreState( QByteArray::fromBase64(QByteArray(principal.firstChildElement ( "TOOLBARSDOCKWIDGETS" ).toElement().text().toAscii())) );

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
        m_bulmafact->workspace() ->setActiveWindow ( activewindow );
    _depura ( "END Company::cargaConf", 0 );
}
