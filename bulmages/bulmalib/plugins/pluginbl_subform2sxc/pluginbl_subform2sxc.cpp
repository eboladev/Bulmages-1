/***************************************************************************
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
#include <QTextCodec>
#include <QLocale>

#include "pluginbl_subform2sxc.h"
#include "blprogressbar.h"


///
/**
\return
**/
int entryPoint ( QApplication * )
{
    _depura ( "entryPoint", 0, "Punto de Entrada del plugin de SubformSXC" );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbl_subform2sxc", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
    _depura ( "END entryPoint", 0, "Punto de Entrada del plugin de SubformSXC" );
    return 0;
}


///
/**
\param parent
**/
myplugsubformsxc::myplugsubformsxc ( BlSubForm *parent ) : QObject ( parent )
{
    _depura ( "myplugsubformsxc::myplugsubformsxc", 0 );
    _depura ( "END myplugsubformsxc::myplugsubformsxc", 0 );
}


///
/**
**/
myplugsubformsxc::~myplugsubformsxc()
{
    _depura ( "myplugsubformsxc::~myplugsubformsxc", 0 );
    _depura ( "END myplugsubformsxc::~myplugsubformsxc", 0 );
}


///
/**
\param menu
**/
void myplugsubformsxc::s_pintaMenu ( QMenu *menu )
{
    _depura ( "myplugsubformsxc::s_pintaMenu", 0 );
    menu->addSeparator();
    menu->addAction ( _ ( "Exportar a hoja de calculo (SXC)" ) );
    _depura ( "END myplugsubformsxc::s_pintaMenu", 0 );
}


///
/**
\param action
**/
void myplugsubformsxc::s_trataMenu ( QAction *action )
{
    _depura ( "myplugsubformsxc::s_trataMenu", 0 );
    if ( action->text() == _ ( "Exportar a hoja de calculo (SXC)" ) ) {
        sacaSXC();
    } // end if
    _depura ( "END myplugsubformsxc::s_trataMenu", 0 );
}


///
/**
**/
void myplugsubformsxc::sacaSXC()
{
    _depura ( "myplugsubformsxc::sacaSXC", 0 );

    QString archivod = g_confpr->valor ( CONF_DIR_USER ) + "listadosxc.perl";
    BlSubForm * subf = ( BlSubForm * ) parent();


    QString fitxersortidatxt = "";

    fitxersortidatxt += "#!/usr/bin/perl\n";
    fitxersortidatxt += "# SubForm Export\n";
    fitxersortidatxt += "\n";
    fitxersortidatxt += "use ooolib;\n";
    fitxersortidatxt += "use utf8;\n";
    fitxersortidatxt += "\n";
    fitxersortidatxt += "# Set variables\n";
    fitxersortidatxt += "$doc = new ooolib(\"sxc\");\n";
    fitxersortidatxt += "$doc->oooSet(\"builddir\", \".\");\n";
    fitxersortidatxt += "$doc->oooSet(\"title\", \"SubFormulario Exportado\");\n";
    fitxersortidatxt += "$doc->oooSet(\"subject\", \"SubFormulario\");\n";
    fitxersortidatxt += "$doc->oooSet(\"comments\", \"SubFormulario Exportado\");\n";
    fitxersortidatxt += "$doc->oooSet(\"author\", \"Tomeu Borras\");\n";
    fitxersortidatxt += "$doc->oooSet(\"meta1-name\", \"Programmer\");\n";
    fitxersortidatxt += "$doc->oooSet(\"meta1-value\", \"Tomeu Borras\");\n";
    fitxersortidatxt += "\n";

    fitxersortidatxt += "$doc->oooSet(\"cell-auto\", \"1,0\");\n";
    fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
    fitxersortidatxt += "$doc->oooSet(\"text-size\", \"18\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 4, 1);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"DATOS EXPORTADOS\");\n";
    fitxersortidatxt += "$doc->oooSet(\"text-size\", \"10\");\n";
    fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";


    int y = 1;
    int x = 1;
    /// Sacamos las cabeceras
    for ( int h = 0; h < subf->mui_listcolumnas->rowCount(); ++h ) {
        if ( subf->mui_listcolumnas->item ( h, 0 ) ->checkState() == Qt::Checked ) {

            fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";
            fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
            fitxersortidatxt += "$doc->oooSet(\"cell-loc\", " + QString::number ( x++ ) + ", " + QString::number ( y ) + ");\n";
            fitxersortidatxt += "$doc->oooData(\"cell-text\", \"" + ( subf->mui_listcolumnas->item ( h, 2 ) ->text() ) + "\");\n";

            fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";
            fitxersortidatxt += "\n";

        } // end if
    } // end for


    y += 2;

    /// Tratamos el progreso de la operacion.
    BlProgressBar barra;
    barra.setRange ( 0, subf->mui_list->rowCount() - 1 );
    barra.setText ( _ ( "Exportando datos a SXC" ) );
    barra.show();
    barra.setText ( _ ( "Exportando datos a SXC" ) );

    /// Sacamos el contenido
    for ( int i = 0; i < subf->mui_list->rowCount(); ++i ) {

        int x = 1;
        for ( int j = 0; j < subf->mui_listcolumnas->rowCount(); ++j ) {

            if ( subf->mui_listcolumnas->item ( j, 0 ) ->checkState() == Qt::Checked ) {


                fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";
                fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
                fitxersortidatxt += "$doc->oooSet(\"cell-loc\", " + QString::number ( x++ ) + ", " + QString::number ( y ) + ");\n";
                fitxersortidatxt += "$doc->oooData(\"cell-text\", \"" + parsearCode ( subf->mui_list->item ( i, j ) ->text() ) + "\");\n";

                fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";
                fitxersortidatxt += "\n";

            } // end if
        } // end forXMLProtect(subf->mui_list->item(i, j)->text())
        y++;
        barra.setValue ( i );
    } // end for


    fitxersortidatxt += "my($filename) = $doc->oooGenerate(\"listadosxc.sxc\");\n";
    fitxersortidatxt += "\n";
    fitxersortidatxt += "exit;\n";


    QString cadena = "rm " + g_confpr->valor ( CONF_DIR_USER ) + "listadosxc.sxc";
    system ( cadena.toAscii() );
    cadena = "rm " + archivod;
    system ( cadena.toAscii() );

    QFile file ( archivod );
    if ( file.open ( QIODevice::WriteOnly ) )  {
        QTextStream stream ( &file );
        stream.setCodec ( "UTF-8" );
        stream << fitxersortidatxt;
        file.close();
    } // end if

    cadena = " cd " + g_confpr->valor ( CONF_DIR_USER ) + "; perl " + archivod;
    system ( cadena.toAscii() );
    cadena = g_confpr->valor(CONF_SXC) + " " + g_confpr->valor ( CONF_DIR_USER ) + "listadosxc.sxc &";
    system ( cadena.toAscii() );

    _depura ( "END myplugsubformsxc::sacaSXC", 0 );
}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    _depura ( "BlSubForm_BlSubForm_Post", 0 );
    myplugsubformsxc *subformsxc = new myplugsubformsxc ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformsxc, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformsxc, SLOT ( s_trataMenu ( QAction * ) ) );
    _depura ( "END BlSubForm_BlSubForm_Post", 0 );
    return 0;
}

