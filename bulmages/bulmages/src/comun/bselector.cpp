/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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

#include <QPushButton>
#include <QFile>

#include "ui_selectorbase.h"
#include "bselector.h"
#include "funcaux.h"
#include "plugins.h"

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif


///
/**
\param parent
**/
BSelector::BSelector ( QWidget *parent )
        : QMainWindow ( parent )
{
    _depura ( "BSelector::BSelector", 0 );
    setupUi ( this );

    g_plugins->lanza ( "BSelector_BSelector", this );

    QObject::connect ( seleccionaempresa, SIGNAL ( clicked() ), this, SLOT ( seleccionaempresa_clicked() ) );
    QObject::connect ( m_bulmatpv, SIGNAL ( clicked() ), this, SLOT ( m_bulmatpv_clicked() ) );
    QObject::connect ( m_bulmacont, SIGNAL ( clicked() ), this, SLOT ( contabilidad_clicked() ) );
    QObject::connect ( m_galopin, SIGNAL ( clicked() ), this, SLOT ( produccion_clicked() ) );
    QObject::connect ( m_bulmafact, SIGNAL ( clicked() ), this, SLOT ( m_bulmafact_clicked() ) );
    QObject::connect ( configura, SIGNAL ( clicked() ), this, SLOT ( configura_clicked() ) );


    /// Al crear el selector, todos los modulos estan cerrados = NULL
    m_tipoempresa = "";
    /// Mira si esta instalado el bulmafact y de no estarlo desabilita el boon�.
    /*    QFile f ( "/usr/bin/bulmafact" );
        if ( !f.exists() )
            m_bulmafact->setEnabled ( FALSE );
        QFile f1 ( "/usr/bin/bulmatpv" );
        if ( !f1.exists() )
            m_bulmatpv->setEnabled ( FALSE );
        QFile f2 ( "/usr/bin/bulmacont" );
        if ( !f2.exists() )
            m_bulmacont->setEnabled ( FALSE );
        QFile f3 ( "/var/www/galopin" );
        if ( !f3.exists() )
            m_galopin->setEnabled ( FALSE );
        QFile f4 ( "/var/www/bcontaweb" );
    */
    g_plugins->lanza ( "BSelector_BSelector_Post", this );
    _depura ( "END BSelector::BSelector", 0 );
}


///
/**
**/
BSelector::~BSelector()
{
    _depura ( "BSelector::~BSelector", 0 );
    _depura ( "BSelector::~BSelector", 0 );
}



/// Boton para abrir el dialogo de configuraciones personalizadas.
/**
**/
void BSelector::configura_clicked()
{
    _depura ( "BSelector::configura_clicked", 0 );
    BConfiguracion *VentanaConfiguracion = new BConfiguracion ( this, 0, 0 );
    VentanaConfiguracion->exec();
    delete VentanaConfiguracion;
    _depura ( "END BSelector::configura_clicked", 0 );
}


/// Boton para entrar en la ventana de Iglues.
/**
**/
void BSelector::on_m_iglues_clicked()
{
    _depura ( "BSelector::on_m_iglues_clicked", 0 );
    QString cadena;
    cadena = confpr->valor ( CONF_NAVEGADOR ) + " http://www.iglues.org &";
    system ( cadena.toAscii().constData() );
    _depura ( "END BSelector::on_m_iglues_clicked", 0 );
}


/// Boton para entrar en el modulo de TPV.
/**
**/
void BSelector::m_bulmatpv_clicked()
{
    _depura ( "BSelector::m_bulmatpv_clicked", 0 );
    if ( m_tipoempresa != "BulmaFact" ) {
        abreempresaview * empcont = new abreempresaview ( 0, "BulmaFact", "abreempresa", true );
        empcont->setModoDestructivo ( FALSE );
        empcont->exec();
        m_empresabd = empcont->nomDB();
    } // end while
    if ( m_empresabd != "" ) {
        char cadena[300];
        sprintf ( cadena, "bulmatpv bulmatpv %s %s %s &", m_empresabd.toAscii().constData(), confpr->valor ( CONF_LOGIN_USER ).toAscii().constData(), confpr->valor ( CONF_PASSWORD_USER ).toAscii().constData() );
        system ( cadena );
    } // end if
    _depura ( "END BSelector::m_bulmatpv_clicked", 0 );
}


/// Boton cambio de empresa y/o usuario.
/**
**/
void BSelector::seleccionaempresa_clicked()
{
    _depura ( "BSelector::seleccionaempresa_clicked", 0 );
    abreempresaview *empcont = new abreempresaview ( 0, "", "abreempresa", true );
    empcont->setModoDestructivo ( FALSE );
    empcont->exec();
    m_empresabd = empcont->nomDB();
    /// Cambiamos el nombre en la pantalla.
    nombreempresa->setText ( empcont->nomEmpresa() );
    m_tipoempresa = empcont->tipoEmpresa();
    delete empcont;
    _depura ( "END BSelector::seleccionaempresa_clicked", 0 );
}


/// Boton para entrar en el modulo de CONTABILIDAD.
/**
**/
void BSelector::contabilidad_clicked()
{
    _depura ( "BSelector::contabilidad_clicked", 0 );
    if ( m_tipoempresa != "BulmaCont" ) {
        abreempresaview * empcont = new abreempresaview ( 0, "BulmaCont", "abreempresa", true );
        empcont->setModoDestructivo ( FALSE );
        empcont->exec();
        m_empresabd = empcont->nomDB();
    } // end if
    if ( m_empresabd != "" ) {
        char cadena[300];
        sprintf ( cadena, "bulmacont bulmacont %s %s %s &", m_empresabd.toAscii().constData(), confpr->valor ( CONF_LOGIN_USER ).toAscii().constData(), confpr->valor ( CONF_PASSWORD_USER ).toAscii().constData() );
        system ( cadena );
    } // end if
    _depura ( "END BSelector::contabilidad_clicked", 0 );
}


/// Boton para entrar en el modulo de PRODUCCION.
/**
\return
**/
void BSelector::produccion_clicked()
{
    _depura ( "Selector::produccion_clicked", 0 );
    int res = g_plugins->lanza ( "BSelector_produccion_clicked", this );
    if ( res ) return;
    /// Al crear un nuevo modulo, le paso como primer parametro un puntero al selector.
    /// De este modo puedo acceder facilmente al selector desde el modulo.
    QString cadena;
    cadena = confpr->valor ( CONF_NAVEGADOR ) + " http://localhost/galopin/ &";
    system ( cadena.toAscii().constData() );
    _depura ( "END Selector::produccion_clicked", 0 );
}


/// Boton para entrar en el modulo de STOCKS Y ALMACENES.º
/**
\return
**/
void BSelector::on_mui_info_clicked()
{
    _depura ( "BSelector::on_mui_info_clicked", 0 );
    int res = g_plugins->lanza ( "BSelector_on_mui_info_clicked", this );
    if ( res ) return;
    QString cadena;
    cadena = confpr->valor ( CONF_NAVEGADOR ) + " http://www.iglues.org/wiki &";
    system ( cadena.toAscii().constData() );
    _depura ( "END BSelector::on_mui_info_clicked", 0 );
}


/// Boton para entrar en el modulo de Facturacion.
/**
**/
void BSelector::m_bulmafact_clicked()
{
    _depura ( "BSelector::m_bulmafact_clicked", 0 );
    if ( m_tipoempresa != "BulmaFact" ) {
        abreempresaview * empcont = new abreempresaview ( 0, "BulmaFact", "abreempresa", true );
        empcont->setModoDestructivo ( FALSE );
        empcont->exec();
        m_empresabd = empcont->nomDB();
    } // end while
    if ( m_empresabd != "" ) {
        char cadena[300];
        sprintf ( cadena, "bulmafact bulmafact %s %s %s &", m_empresabd.toAscii().constData(), confpr->valor ( CONF_LOGIN_USER ).toAscii().constData(), confpr->valor ( CONF_PASSWORD_USER ).toAscii().constData() );
        system ( cadena );
    } // end if
    _depura ( "END BSelector::m_bulmafact_clicked", 0 );
}


///
/**
\return
**/
QString BSelector::empresaDB()
{
    _depura ( "BSelector::empresaDB", 0 );
    _depura ( "END BSelector::empresaDB", 0 );
    return m_empresabd;
}


///
/**
\param emp
**/
void BSelector::setEmpresaBD ( QString emp )
{
    _depura ( "BSelector::setEmpresaBD", 0 );
    m_empresabd = emp;
    _depura ( "END BSelector::setEmpresaBD", 0 );
}

///
/**
\return
**/
QString BSelector::tipoEmpresa()
{
    _depura ( "BSelector::tipoEmpresa", 0 );
    _depura ( "END BSelector::tipoEmpresa", 0 );
    return m_tipoempresa;
}


///
/**
**/
void BSelector::setTipoEmpresa ( QString tip )
{
    _depura ( "BSelector::setTipoEmpresa", 0 );
    m_tipoempresa = tip;
    _depura ( "END BSelector::setTipoEmpresa", 0 );
}

