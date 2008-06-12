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

#include "pluginsubformods.h"
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
        traductor->load ( QString ( "pluginsubformods_" ) + QLocale::system().name(),
                          confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
    } else {
        QString archivo = "pluginsubformods_" + confpr->valor ( CONF_TRADUCCION );
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
    menu->addAction ( tr ( "Exportar a hoja de calculo (ODS)" ) );
    _depura ( "END myplugsubformods::s_pintaMenu", 0 );
}


///
/**
\param action
**/
void myplugsubformods::s_trataMenu ( QAction *action )
{
    _depura ( "myplugsubformods::s_trataMenu", 0 );
    if ( action->text() == tr ( "Exportar a hoja de calculo (ODS)" ) ) {
        sacaods();
    } // end if
    _depura ( "END myplugsubformods::s_trataMenu", 0 );
}


///
/**
**/
void myplugsubformods::sacaods()
{
    _depura ( "myplugsubformods::sacaods", 0 );

    QString archivod = confpr->valor ( CONF_DIR_USER ) + "listadoods.py";
    SubForm3 * subf = ( SubForm3 * ) parent();


    QString fitxersortidatxt = "";

    fitxersortidatxt += "#!/usr/bin/python\n";
    fitxersortidatxt += "# -*- coding: utf8 -*-\n";
    fitxersortidatxt += "\n";
    fitxersortidatxt += "import ooolib\n";
    fitxersortidatxt += "\n";

    fitxersortidatxt += "# Crea el documento\n";
    fitxersortidatxt += "doc = ooolib.Calc(\"Listado\")\n";

    int y = 1;
    int x = 1;

    /// Sacamos las cabeceras con los nombres de los campos descriptivos (largos).
    for ( int h = 0; h < subf->mui_listcolumnas->rowCount(); ++h ) {
        if ( subf->mui_listcolumnas->item ( h, 0 ) ->checkState() == Qt::Checked ) {

            fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";

            QString textocabecera = ( subf->mui_listcolumnas->item ( h, 2 ) ->text() );
            textocabecera.replace ( QString ( "\n" ), QString ( "\\n\\\n" ) );

            /// Devuelve el ancho de la columna para ponerlo igual en el archivo de salida.
            fitxersortidatxt += "doc.set_column_property(" + QString::number ( x ) + ", 'width', '" + QString::number ( ( double ) subf->mui_list->columnWidth ( h ) / 90 ) + "in')\n\n";

            fitxersortidatxt += "doc.set_cell_property('bold', True)\n";
            fitxersortidatxt += "doc.set_cell_value(" + QString::number ( x++ ) + "," + QString::number ( y ) + ", 'string', '" + textocabecera + "')\n";
            fitxersortidatxt += "doc.set_cell_property('bold', False)\n";
        } // end if
    } // end for

    y += 1;
    x = 1;
    /// Sacamos las cabeceras con los nombres de los campos de la base de datos.
    for ( int h = 0; h < subf->mui_listcolumnas->rowCount(); ++h ) {
        if ( subf->mui_listcolumnas->item ( h, 0 ) ->checkState() == Qt::Checked ) {

            fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";

            QString textocabecera = ( subf->mui_listcolumnas->item ( h, 1 ) ->text() );
            //QString textocabecera = (subf->mui_listcolumnas->columnDBfieldName());
            textocabecera.replace ( QString ( "\n" ), QString ( "\\n\\\n" ) );

            fitxersortidatxt += "doc.set_cell_property('bold', True)\n";
            fitxersortidatxt += "doc.set_cell_value(" + QString::number ( x++ ) + "," + QString::number ( y ) + ", 'string', '" + textocabecera + "')\n";
            fitxersortidatxt += "doc.set_cell_property('bold', False)\n";
        } // end if
    } // end for

    y += 1;

    bool resultconvdouble, resultconvinteger;
    double resultadodouble, resultadointeger;

    /// Sacamos el contenido
    for ( int i = 0; i < subf->mui_list->rowCount(); ++i ) {

        int x = 1;

        for ( int j = 0; j < subf->mui_listcolumnas->rowCount(); ++j ) {
            if ( subf->mui_listcolumnas->item ( j, 0 ) ->checkState() == Qt::Checked ) {
                fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";

                QString textocontenido = ( subf->mui_list->item ( i, j ) ->text() );


                //TODO: Mirar de mejorar el mecanismo de deteccion de tipo de dato.

                /// Detecta el tipo de dato que es para configurar el formato de la celda.
                /// Comprueba que esta alineado a la derecha para saber si es un numero o un texto.
                if ( subf->mui_list->item ( i, j ) ->textAlignment() == Qt::AlignRight ) {
                    /// Prueba con 'double'.
                    resultadodouble = textocontenido.toDouble ( &resultconvdouble );
                    /// Prueba con 'integer'.
                    resultadointeger = textocontenido.toInt ( &resultconvinteger );
                    /// Prueba con un porcentaje.
                    //TODO:
                    /// Prueba con una fecha.
                    //TODO:

                    if ( resultconvdouble )  {
                        /// Es 'double'.
                        fitxersortidatxt += "doc.set_cell_value(" + QString::number ( x++ ) + "," + QString::number ( y ) + ", 'float' , '" + textocontenido + "')\n\n";
                    } else if ( resultconvinteger ) {
                        /// Es un 'integer'.
                        fitxersortidatxt += "doc.set_cell_value(" + QString::number ( x++ ) + "," + QString::number ( y ) + ", 'float' , '" + textocontenido + "')\n\n";
                    } else {
                        /// Es tratado como un 'string'.
                        textocontenido.replace ( QString ( "\n" ), QString ( "\\n\\\n" ) );
                        fitxersortidatxt += "doc.set_cell_value(" + QString::number ( x++ ) + "," + QString::number ( y ) + ", 'string', '" + textocontenido + "')\n\n";
                    } // end if
                } else {
                    /// Es tratado como un 'string'.
                    textocontenido.replace ( QString ( "\n" ), QString ( "\\n\\\n" ) );
                    fitxersortidatxt += "doc.set_cell_value(" + QString::number ( x++ ) + "," + QString::number ( y ) + ", 'string', '" + textocontenido + "')\n\n";
                } // end if

            } // end if
        } // end forXMLProtect(subf->mui_list->item(i, j)->text())

        y++;

    } // end for

    fitxersortidatxt += "doc.save(\"listadoods.ods\")\n";

    QString cadena = "rm " + confpr->valor ( CONF_DIR_USER ) + "listadoods.ods";
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

    cadena = " cd " + confpr->valor ( CONF_DIR_USER ) + "; python " + archivod;
    system ( cadena.toAscii() );
    cadena = "oocalc " + confpr->valor ( CONF_DIR_USER ) + "listadoods.ods &";
    system ( cadena.toAscii() );

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

