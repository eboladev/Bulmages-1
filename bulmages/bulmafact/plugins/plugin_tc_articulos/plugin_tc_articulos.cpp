/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include <QMenu>
#include <QAction>
#include <QObject>
#include <QMessageBox>

#include <stdio.h>

#include "plugin_tc_articulos.h"
#include "company.h"
#include "funcaux.h"


///
/**
**/
plugin_tc_articulos::plugin_tc_articulos()
{
    _depura ( "plugin_tc_articulos::plugin_tc_articulos", 0 );
    _depura ( "END plugin_tc_articulos::plugin_tc_articulos", 0 );
}


///
/**
**/
plugin_tc_articulos::~plugin_tc_articulos()
{
    _depura ( "plugin_tc_articulos::~plugin_tc_articulos", 0 );
    _depura ( "END plugin_tc_articulos::~plugin_tc_articulos", 0 );
}


///
/**
**/
void plugin_tc_articulos::tallas()
{
    _depura ( "plugin_tc_articulos::tallas", 0 );
    fprintf ( stderr, "Sa ha activado tallas\n" );
    QMessageBox::warning ( 0,
                           "Titulo de la ventana",
                           "Mensaje.",
                           QMessageBox::Ok,
                           QMessageBox::Cancel );
    _depura ( "END plugin_tc_articulos::tallas", 0 );
}


///
/**
**/
void plugin_tc_articulos::colores()
{
    _depura ( "plugin_tc_articulos::colores", 0 );
    fprintf ( stderr, "Sa ha activado colores\n" );
    QMessageBox::warning ( 0,
                           "Titulo de la ventana",
                           "Mensaje.",
                           QMessageBox::Ok,
                           QMessageBox::Cancel );
    _depura ( "END plugin_tc_articulos::colores", 0 );
}


///
/**
\param bges
**/
void plugin_tc_articulos::inicializa ( Bulmafact *bges )
{
    _depura ( "plugin_tc_articulos::inicializa", 0 );
    QMenu *pPluginMenuTallasColores;
    /// Miramos si existe un menu Herramientas
    pPluginMenuTallasColores = bges->menuBar() ->findChild<QMenu *> ( "Tallas y colores" );

    /// Creamos el men&uacute;.
    if ( !pPluginMenuTallasColores ) {
        pPluginMenuTallasColores = new QMenu ( "&Tallas y colores", bges->menuBar() );
        pPluginMenuTallasColores->setObjectName ( QString::fromUtf8 ( "Tallas y colores" ) );
    } // end if
    /// Creamos el men&uacute;.

    QAction *acciontallas = new QAction ( "&Tallas", 0 );
    acciontallas->setStatusTip ( "Tallas" );
    acciontallas->setWhatsThis ( "Tallas" );
    connect ( acciontallas, SIGNAL ( activated() ), this, SLOT ( tallas() ) );
    pPluginMenuTallasColores->addAction ( acciontallas );

    QAction *accioncolores = new QAction ( "&Colores", 0 );
    accioncolores->setStatusTip ( "Colores" );
    accioncolores->setWhatsThis ( "Colores" );
    connect ( accioncolores, SIGNAL ( activated() ), this, SLOT ( colores() ) );
    pPluginMenuTallasColores->addAction ( accioncolores );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar() ->insertMenu ( bges->menuVentana->menuAction(), pPluginMenuTallasColores );
    _depura ( "END plugin_tc_articulos::inicializa", 0 );
}


///
/**
\param bges
**/
void entryPoint ( Bulmafact *bges )
{
    _depura ( "Estoy dentro del plugin de tallas y colores", 0 );
    plugin_tc_articulos *plug = new plugin_tc_articulos();
    plug->inicializa ( bges );
}

