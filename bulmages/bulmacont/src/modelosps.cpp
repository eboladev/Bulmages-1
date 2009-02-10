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

#include <stdio.h>
#include <fstream>
#include <iostream>

#include <QString>
#include <QComboBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QCustomEvent>
#include <QTextStream>
#include <QMessageBox>
#include <QObject>

#include "modelosps.h"
#include "blconfiguration.h"
#include "postgresiface2.h"


///
/**
**/
Modgenps::Modgenps()
{
    _depura ( "Modgenps::Modgenps", 0 );
    _depura ( "END Modgenps::Modgenps", 0 );
}


///
/**
**/
Modgenps::~Modgenps()
{
    _depura ( "Modgenps::~Modgenps", 0 );
    _depura ( "Modgenps::~Modgenps", 0 );
}


///
/**
\param cad1
\param cad2
\param x
**/
void Modgenps::formatdigits ( QString *cad1, QString *cad2, float x )
{
    _depura ( "Modgenps::formatdigits", 0 );
    QString tempstr;
    /// Formatea n&uacute;mero x con 2 cifras decimales (centimos de euro).
    tempstr.sprintf ( "%.2f", x );
    *cad2 = tempstr.section ( '.', 1, 1 );
    *cad1 = tempstr.section ( '.', 0, 0 );
    /// Se le a&ntilde;de a la parte entera la coma (a&uacute;n no s&eacute; si dejarlo o no)
    cad1->append ( ',' );
    _depura ( "END Modgenps::formatdigits", 0 );
}


///
/**
\param cad
\param x
\param y
**/
void Modgenps::escrizq ( QString cad, int x, int y )
{
    _depura ( "Modgenps::escrizq", 0 );
    /// Genera c&oacute;digo postscript para escribir 'cad' alineado a la derecha,
    /// suponiendo fuente Courier-Bold 12.
    m_output << "(" << cad << ") " << x << " " << y << " " << cad.length() << " escrizq\n";
    _depura ( "END Modgenps::escrizq", 0 );
}


///
/**
\param valor
\param x
\param y
**/
void Modgenps::escrizq ( float valor, int x, int y )
{
    _depura ( "Modgenps::escrizq", 0 );
    QString tempstr;
    tempstr.sprintf ( "%g", valor );
    escrizq ( tempstr, x, y );
    _depura ( "END Modgenps::escrizq", 0 );
}


/// Writes text in the postscript file aligned to the left (that is, to the right of the
/// given point).
/** @param cad String to write.
    @param x x coordinate of the right point.
    @param y y coordinate of the right point. */
void Modgenps::escrder ( QString cad, int x, int y )
{
    _depura ( "Modgenps::escrder", 0 );
    /// Genera c&oacute;digo postscript para escribir 'cad' alineado a la izquierda,
    /// suponiendo fuente Courier-Bold 12.
    m_output << "(" << cad << ") " << x << " " << y << " " << cad.length() << " escrder\n";
    _depura ( "END Modgenps::escrder", 0 );
}


///
/**
\param valor
\param x
\param y
**/
void Modgenps::escrder ( float valor, int x, int y )
{
    _depura ( "Modgenps::escrder", 0 );
    QString tempstr;
    tempstr.sprintf ( "%g", valor );
    escrder ( tempstr, x, y );
    _depura ( "END Modgenps::escrder", 0 );
}


/// Writes 2 string left and right aligned respectively.
/** @param cad1 Left aligned text.
    @param cad2 Right aligned text.
    @param x x coordinate of center.
    @param y y coordinate of center. */
void Modgenps::escrizqder ( QString cad1, QString cad2, int x, int y )
{
    _depura ( "Modgenps::escrizqder", 0 );
    escrizq ( cad1, x, y );
    escrder ( cad2, x, y );
    _depura ( "END Modgenps::escrizqder", 0 );
}


/// Writes 2 strings left and right aligned respectively.
/** @param cad1 Left aligned text.
    @param cad2 Right aligned text.
    @param x x coordinate of center.
    @param y y coordinate of center. */
void Modgenps::escrizqder ( float valor, int x, int y )
{
    _depura ( "Modgenps::escrizqder", 0 );
    QString cad1, cad2;
    formatdigits ( &cad1, &cad2, valor );
//    cout << "Si le digo" << valor << " me sale:\n";
//    cout << "OJO!!!:"<< cad1.toAscii().constData() << " ," << cad2.toAscii().constData() <<"\n";
    escrizq ( cad1, x, y );
    escrder ( cad2, x, y );
    _depura ( "END Modgenps::escrizqder", 0 );
}


/// Put a character in a square box.
/**
\param marca
\param x
\param y
**/
void Modgenps::marca_casilla ( QString marca, int x, int y )
{
    _depura ( "Modgenps::marca_casilla", 0 );
    escrder ( marca, x - 2, y );
    _depura ( "END Modgenps::marca_casilla", 0 );
}


///
/**
**/
void Modgenps::marcadeagua_borrador()
{
    _depura ( "Modgenps::marcadeagua_borrador", 0 );
    m_output << "gsave\n";
    m_output << "1 setgray\n";
    m_output << "newpath\n";
    m_output << "360 685 moveto\n";
    m_output << "567 685 lineto\n";
    m_output << "567 630 lineto\n";
    m_output << "360 630 lineto\n";
    m_output << "closepath\n";
    m_output << "fill\n";
    m_output << ".7 setgray\n";
    m_output << "/Helvetica-Bold findfont\n";
    m_output << "100 scalefont\n";
    m_output << "setfont\n";
    m_output << "147 107 moveto\n";
    m_output << "45 rotate\n";
    m_output << "(BORRADOR) true charpath\n";
    m_output << "stroke\n";
    m_output << "grestore\n";
    _depura ( "END Modgenps::marcadeagua_borrador", 0 );
}


///
/**
\param pdfnamepar
\param tempnamepar
\param dialpar
**/
Genps_thread::Genps_thread ( QString pdfnamepar, QString tempnamepar, QProgressDialog *dialpar )
{
    _depura ( "Genps_thread::Genps_thread", 0 );
    m_pdfname = pdfnamepar;
    m_tempname = tempnamepar;
    m_progressdia = dialpar;
    _depura ( "END Genps_thread::Genps_thread", 0 );
}


/// Invoca al programa Acrobat Reader en un servidor virtual XVfb.
/**
**/
void Genps_thread::run()
{
    _depura ( "Genps_thread::run", 0 );
    QTextStream m_output;
    QString command;
    /// Lo borro para asegurarme de que Acrobat no me pregunte "overwrite?".
    command = "rm -f " + m_tempname;
    system ( command.toAscii().constData() );
//    cout << "Llamando a XVfb...\n";
    system ( "Xvfb :5.0 -ac -fbdir /tmp -screen 0 800x600x8 &" );
    system ( "xmodmap -display :5.0 /usr/X11R6/lib/X11/xmodmap.std" );
//    cout << "XVfb iniciado...\n";
//    cout << "Iniciando acrobat reader...\n";
    command = "acroread -display :5.0 -geometry 800x600+0+0 -tempFile +useFrontEndProgram " + m_pdfname + " &";
    system ( command.toAscii().constData() );
//    cout << "Acrobat reader iniciado...\n";

    QString macrofilename = QString ( getenv ( "HOME" ) ) + "/.bulmages/macrotmp";
    QFile macro ( macrofilename );

    /// Escribo la serie de macros que me interactuaran con Acrobat Reader, esto es:
    /// situar el rat&oacute;n en la ventana, pulsar ctrl+P, seleccionar "imprimir a fichero",
    /// escribir el nombre del fichero y darle a imprimir.
    /// m_output << "Delay 10\n";

    /// This sleeps are necessary to wait for Acrobat Reader to be started.
    for ( int i = 1; i < 11; i++ ) {
        sleep ( 1 );
        //QCoreApplication::postEvent(m_progressdia, new QCustomEvent(sleep10));
//        cout << i << "\n";
    } // end for

    macro.open ( QIODevice::WriteOnly );
    m_output.setDevice ( &macro );
    m_output << "MotionNotify 400 300\n";
    m_output << "KeyStrPress Control_L\n";
    m_output << "KeyStrPress p\n";
    m_output << "KeyStrRelease p\n";
    m_output << "KeyStrRelease Control_L\n";
    macro.close();
    command = "xmacroplay :5.0 < " + macrofilename;
    system ( command.toAscii().constData() );

    sleep ( 1 );
    //QCoreApplication::postEvent(m_progressdia, new QCustomEvent(sleep3));

    macro.open ( QIODevice::WriteOnly );
    m_output.setDevice ( &macro );
    m_output << "KeyStrPress Tab\n";
    m_output << "KeyStrRelease Tab\n";
    m_output << "KeyStrPress Down\n";
    m_output << "KeyStrRelease Down\n";
    m_output << "KeyStrPress space\n";
    m_output << "KeyStrRelease space\n";
    /// Aqu&iacute; escribo el nombre del fichero donde quiero que imprima.
    m_output << "String " << m_tempname + "\n";
    m_output << "KeyStrPress Return\n";
    m_output << "KeyStrRelease Return\n";
    macro.close();
    command = "xmacroplay :5.0 < " + macrofilename;
    system ( command.toAscii().constData() );

    sleep ( 3 );
    //QCoreApplication::postEvent(m_progressdia, new QCustomEvent(sleep3));

    macro.open ( QIODevice::WriteOnly );
    m_output.setDevice ( &macro );
    m_output << "KeyStrPress Control_L\n";
    m_output << "KeyStrPress q\n";
    m_output << "KeyStrRelease q\n";
    m_output << "KeyStrRelease Control_L\n";
    macro.close();
    command = "xmacroplay :5.0 < " + macrofilename;
    system ( command.toAscii().constData() );

    /// Con este comando busco el servidor Xvfb que corra en el display :5.0 y lo mato.
    command = "kill $(ps aux|grep 'Xvfb :5.0'|grep -v grep|awk '{print $2}')";
    system ( command.toAscii().constData() );
//    cout << "Se acabo!!\n";
    _depura ( "END Genps_thread::run", 0 );
}


///
/**
\param etiqueta
\param btcancelar
\param minimo
\param maximo
\param widget
\param bandera
**/
Psprogressdialog::Psprogressdialog ( QString etiqueta, QString btcancelar, int minimo, int maximo, QWidget *widget, Qt::WFlags bandera )
        : QProgressDialog ( etiqueta, btcancelar, minimo, maximo, widget, bandera )
{
    _depura ( "Psprogressdialog::Psprogressdialog", 0 );
    _depura ( "END Psprogressdialog::Psprogressdialog", 0 );
}

/*
void Psprogressdialog::customEvent(QCustomEvent *event) {
    if ((int)event->type() == sleep10) {
        this->setValue(value() + 5);
    } // end if
    if ((int)event->type() == sleep3) {
        this->setValue(value() + 25);
    } // end if
    if (value() > 98) {
        setValue(100);
    } // end if
}

*/

