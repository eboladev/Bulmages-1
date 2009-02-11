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

#include <stdio.h>

#include <QMenu>
#include <QAction>
#include <QObject>
#include <QMessageBox>
#include <QInputDialog>
#include <QStringList>
#include <QClipboard>
#include <QTextStream>
#include <QTranslator>
#include <QTextCodec>
#include <QLocale>

#include "empresabase.h"
#include "funcaux.h"
#include "pluginclipboardbf.h"


///
/**
\return
**/
int entryPoint ( QApplication * )
{
    _depura ( "entryPoint", 0, "Punto de Entrada del plugin de Clipboard" );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginclipboardbf", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    /// Cargamos el sistema de traducciones una vez pasado por las configuraciones generales
    QTranslator *traductor = new QTranslator ( 0 );
    if ( confpr->valor ( CONF_TRADUCCION ) == "locales" ) {
        traductor->load ( QString ( "pluginclipboardbf_" ) + QLocale::system().name(),
                          confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
    } else {
        QString archivo = "pluginclipboardbf_" + confpr->valor ( CONF_TRADUCCION );
        traductor->load ( archivo, confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
    } // end if
    theApp->installTranslator ( traductor );
    _depura ( "END entryPoint", 0, "Punto de Entrada del plugin de Clipboard" );
    return 0;
}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    _depura ( "BlSubForm_BlSubForm_Post", 0 );
    myplugclipboard *subformclip = new myplugclipboard ( sub );
    sub->connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformclip, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformclip, SLOT ( s_trataMenu ( QAction * ) ) );
    _depura ( "END BlSubForm_BlSubForm_Post", 0 );
    return 0;
}


///
/**
\param parent
**/
myplugclipboard::myplugclipboard ( BlSubForm *parent ) : QObject ( parent )
{
    _depura ( "myplugclipboard::myplugclipboard", 0 );
    _depura ( "END myplugclipboard::myplugclipboard", 0 );
}


///
/**
**/
myplugclipboard::~myplugclipboard()
{
    _depura ( "myplugclipboard::~myplugclipboard", 0 );
    _depura ( "END myplugclipboard::~myplugclipboard", 0 );
}


///
/**
\param menu
**/
void myplugclipboard::s_pintaMenu ( QMenu *menu )
{
    _depura ( "myplugclipboard::s_pintaMenu", 0 );
    menu->addSeparator();
    menu->addAction ( _( "Pegar desde hoja de calculo" ) );
    _depura ( "END myplugclipboard::s_pintaMenu", 0 );
}


///
/**
\param action
**/
void myplugclipboard::s_trataMenu ( QAction *action )
{
    _depura ( "myplugclipboard::s_trataMenu", 0 );
    if ( action->text() == _( "Pegar desde Hoja de Calculo" ) ) {
        if ( theApp->clipboard() ->text().contains ( "\t" ) ) {
            pegaODS();
        } else {
            pegaSXC();
        } // end if
    } // end if
    _depura ( "myplugclipboard::s_trataMenu", 0 );
}


/// Funcion que pega desde KSpread, que copia al portapapeles la informacion con campos de tamanyo fijo.
/**
**/
void myplugclipboard::pegaSXC()
{
    _depura ( "myplugclipboard::pegaSXC", 0 );
    BlSubForm *subform = ( BlSubForm * ) parent();
    QString clipboard = theApp->clipboard() ->text();

    QStringList lineas = clipboard.split ( "\n" );

    /// La primera linea tiene los nombres de las columnas.
    QStringList campos = lineas.at ( 0 ).simplified().split ( " " );

    /// Calculamos el tamanyo de cada campo.
    int numcampos = campos.size();
    int numchars  = lineas.at ( 0 ).size() / numcampos;

    /// Iteramos para cada linea
    for ( int i = 1; i < lineas.size() - 1 ; ++i ) {
        QString cadena_valores = lineas.at ( i );

        /// Creamos un elemento en la factura
        SDBRecord  *linea1;
        linea1 = subform->lineaat ( subform->rowCount() - 1 );
        /// Haciendo el nuevo registro antes nos evitamos problemas de foco.
        subform->nuevoRegistro();

        /// Iteramos para cada columna.
        for ( int j = 0; j < numcampos; ++j ) {
            /// Cogemos un valor.
            QString valorcampo = cadena_valores.left ( numchars ).simplified();
            cadena_valores = cadena_valores.right ( cadena_valores.size() - numchars );
            linea1->setDBvalue ( campos.at ( j ), valorcampo );
        } // end for
    } // end for
    _depura ( "END myplugclipboard::pegaSXC", 0 );
}


/// Funcion que pega desde OpenOffice.org, que copia al portapapeles la informacion con separador de campos un tabulador.
/**
**/
void myplugclipboard::pegaODS()
{
    _depura ( "myplugclipboard::pegaODS", 0 );
    BlSubForm *subform = ( BlSubForm * ) parent();
    QString clipboard = theApp->clipboard() ->text();

    QStringList lineas = clipboard.split ( "\n" );

    /// La primera linea tiene los nombres de las columnas.
// QStringList campos = lineas.at(0).simplified().split(" ");
    QStringList campos = lineas.at ( 0 ).split ( "\t" );

    /// Calculamos el tamanyo de cada campo.
    int numcampos = campos.size();

    /// Iteramos para cada linea
    for ( int i = 1; i < lineas.size() - 1 ; ++i ) {
        QStringList campos_valores = lineas.at ( i ).split ( "\t" );

        /// Creamos un elemento en la factura
        SDBRecord  *linea1;
        linea1 = subform->lineaat ( subform->rowCount() - 1 );
        /// Haciendo el nuevo registro antes nos evitamos problemas de foco.
        subform->nuevoRegistro();

        /// Iteramos para cada columna.
        for ( int j = 0; j < numcampos; ++j ) {
            /// Cogemos un valor.
            linea1->setDBvalue ( campos.at ( j ), campos_valores.at ( j ) );
        } // end for
    } // end for
    _depura ( "END myplugclipboard::pegaODS", 0 );
}


