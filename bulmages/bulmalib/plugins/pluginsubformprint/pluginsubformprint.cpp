/***************************************************************************
 *   Copyright (C) 2007 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
 *                                                                         *
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#include <stdio.h>

#include <QFile>
#include <QTextStream>
#include <QTranslator>
#include <QTextCodec>
#include <QLocale>

#include "pluginsubformprint.h"
#include "funcaux.h"


///
/**
\return
**/
int entryPoint ( QApplication * )
{
    _depura ( "entryPoint" , 0, "Punto de Entrada del plugin de Subformods" );
    /// Cargamos el sistema de traducciones una vez pasado por las configuraciones generales
    QTranslator *traductor = new QTranslator ( 0 );
    if ( confpr->valor ( CONF_TRADUCCION ) == "locales" ) {
        traductor->load ( QString ( "pluginsubformprint_" ) + QLocale::system().name(),
                          confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
    } else {
        QString archivo = "pluginsubformprint_" + confpr->valor ( CONF_TRADUCCION );
        traductor->load ( archivo, confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
    } // end if
    theApp->installTranslator ( traductor );
    _depura ( "END entryPoint", 0, "Punto de Entrada del plugin de Subformods" );
    return 0;
}


///
/**
\param parent
**/
myplugsubformods::myplugsubformods ( SubForm3 *parent ) : QObject ( parent )
{
    _depura ( "myplugsubformods::myplugsubformods", 0 );
    _depura ( "END myplugsubformods::myplugsubformods", 0 );
}

///
/**
**/
myplugsubformods::~myplugsubformods()
{
    _depura ( "myplugsubformods::~myplugsubformods", 0 );
    _depura ( "END myplugsubformods::~myplugsubformods", 0 );
}


///
/**
\param menu
**/
void myplugsubformods::s_pintaMenu ( QMenu *menu )
{
    _depura ( "myplugsubformods::s_pintaMenu", 0 );
    menu->addSeparator();
    menu->addAction ( tr ( "Imprimir" ) );
    _depura ( "END myplugsubformods::s_pintaMenu", 0 );
}


///
/**
\param action
**/
void myplugsubformods::s_trataMenu ( QAction *action )
{
    _depura ( "myplugsubformods::s_trataMenu", 0 );
    if ( action->text() == tr ( "Imprimir" ) ) {
        imprimir();
    } // end if
    _depura ( "END myplugsubformods::s_trataMenu", 0 );
}


///
/**
**/
void myplugsubformods::imprimir()
{
    _depura ( "myplugsubformods::sacaods", 0 );

    SubForm3 * subf = ( SubForm3 * ) parent();

    subf->imprimirPDF ( "" );
    _depura ( "END myplugsubformods::sacaods", 0 );
}


///
/**
\param sub
\return
**/
int SubForm3_SubForm3_Post ( SubForm3 *sub )
{
    _depura ( "SubForm3_SubForm3_Post", 0 );
    myplugsubformods *subformods = new myplugsubformods ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    _depura ( "END SubForm3_SubForm3_Post", 0 );
    return 0;
}

