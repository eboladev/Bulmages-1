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
#include <qstring.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qstring.h>
#include "modelosps.h"
#include "configuracion.h"
#include "postgresiface2.h"
#include <qmessagebox.h>

#include <qprogressdialog.h>
#include <qstring.h>
#include <qobject.h>
using namespace std;

void Modgenps::formatdigits(QString *cad1,QString *cad2,float x)
{
  QString tempstr;
  tempstr.sprintf("%.2f",x);//Formatea numero x con 2 cifras decimales (centimos de euro)
  *cad2=tempstr.section('.',1,1);
  *cad1=tempstr.section('.',0,0);
  cad1->append(',');//Se le añade a la parte entera la coma (aun no se si dejarlo o no)
}



void Modgenps::escrizq(QString cad,int x,int y)
{
  /** Genera codigo postscript para escribir cad alineado a la derecha, suponiendo fuente Courier-Bold 12
  */
  output << "("<< cad<< ") " << x << " "<< y << " " << cad.length() << " escrizq\n";
}
void Modgenps::escrizq(float valor,int x,int y)
{
  QString tempstr;
  tempstr.sprintf("%g",valor);
  escrizq(tempstr,x,y);
}



void Modgenps::escrder(QString cad,int x,int y)
{
  /** Genera codigo postscript para escribir cad alineado a la izquierda, suponiendo fuente Courier-Bold 12
  */
  output << "("<< cad<< ") " << x << " " << y << " "<< cad.length() << " escrder\n";
}
void Modgenps::escrder(float valor,int x,int y)
{
  QString tempstr;
  tempstr.sprintf("%g",valor);
  escrder(tempstr,x,y);
}


void Modgenps::escrizqder(QString cad1,QString cad2,int x,int y)
{
  escrizq(cad1,x,y);
  escrder(cad2,x,y);
}
void Modgenps::escrizqder(float valor,int x,int y)
{
  QString cad1,cad2;
  formatdigits(&cad1,&cad2,valor);
  cout << "Si le digo" << valor << " me sale:\n";
  cout << "OJO!!!:"<< cad1 << " ," << cad2<<"\n";
    escrizq(cad1,x,y);
  escrder(cad2,x,y);
}

void Modgenps::marca_casilla(QString marca,int x,int y)
{
  escrder(marca,x-2,y);
}
void Modgenps::marcadeagua_borrador()
{
output << "gsave\n";
output << "1 setgray\n";
output << "newpath\n";
output << "360 685 moveto\n";
output << "567 685 lineto\n";
output << "567 630 lineto\n";
output << "360 630 lineto\n";
output << "closepath\n";
output << "fill\n";
output << ".7 setgray\n";
output << "/Helvetica-Bold findfont\n";
output << "100 scalefont\n";
output << "setfont\n";
output << "147 107 moveto\n";
output << "45 rotate\n";
output << "(BORRADOR) true charpath\n";
output << "stroke\n";
output << "grestore\n";
}











genps_thread::genps_thread(QString pdfnamepar,QString tempnamepar,QProgressDialog *dialpar)
{
pdfname=pdfnamepar;
tempname=tempnamepar;
progressdia=dialpar;
}

void genps_thread::run()
{//Invoca al programa acrobat reader en un servidor virtual XVfb

QTextStream output;
system("rm -f "+tempname); //Lo borro para asegurarme de que Acrobat no me pregunte "¿overwrite?"
cout << "Llamando a XVfb...\n";
system("Xvfb :5.0 -ac -fbdir /tmp -screen 0 800x600x8 &");
system("xmodmap -display :5.0 /usr/X11R6/lib/X11/xmodmap.std");
cout << "XVfb iniciado...\n";
cout << "Iniciando acrobat reader...\n";
system("acroread -display :5.0 -geometry 800x600+0+0 -tempFile +useFrontEndProgram "+pdfname+" &");
cout << "Acrobat reader iniciado...\n";

QString macrofilename=QString(getenv("HOME"))+"/.bulmages/macrotmp";
QFile macro(macrofilename);


//Escribo la serie de macros que me interactuaran con Acrobat Reader, esto es: situar el raton en la ventana, pulsar ctrl+P, seleccionar "imprimr a fichero",
//escribir el nombre del fichero y darle a imprimir. 
//output << "Delay 10\n";

for (int i=1;i<11;i++)
{
sleep(1);
postEvent(progressdia,new QCustomEvent(sleep10));
cout << i<<"\n";
}


macro.open(IO_WriteOnly);
output.setDevice(&macro);
output << "MotionNotify 400 300\n";
output << "KeyStrPress Control_L\n";
output << "KeyStrPress p\n";
output << "KeyStrRelease p\n";
output << "KeyStrRelease Control_L\n";
macro.close();
system("xmacroplay :5.0 < "+macrofilename);


sleep(1);
postEvent(progressdia,new QCustomEvent(sleep3));

macro.open(IO_WriteOnly);
output.setDevice(&macro);
output << "KeyStrPress Tab\n";
output << "KeyStrRelease Tab\n";
output << "KeyStrPress Down\n";
output << "KeyStrRelease Down\n";
output << "KeyStrPress space\n";
output << "KeyStrRelease space\n";
output << "String "<< tempname+"\n"; //Aqui escribo el nombre del fichero donde quiero que imprima
output << "KeyStrPress Return\n";
output << "KeyStrRelease Return\n";
macro.close();
system("xmacroplay :5.0 < "+macrofilename);

sleep(3);
postEvent(progressdia,new QCustomEvent(sleep3));

macro.open(IO_WriteOnly);
output.setDevice(&macro);
output << "KeyStrPress Control_L\n";
output << "KeyStrPress q\n";
output << "KeyStrRelease q\n";
output << "KeyStrRelease Control_L\n";
macro.close();
system("xmacroplay :5.0 < "+macrofilename);

//Con este comando busco el servidor Xvfb que corra en el display :5.0 y lo mato
system("kill $(ps aux|grep 'Xvfb :5.0'|grep -v grep|awk '{print $2}')");
cout << "Se acabó!!!\n";
//postEvent(progressdia,new QCustomEvent(acabado));
}

psprogressdialog::psprogressdialog(QString a,QString b,int max):QProgressDialog(a,b,max) {}




void psprogressdialog::customEvent(QCustomEvent *event)
{

if ((int)event->type()==sleep10) this->setProgress(progress()+5);
if ((int)event->type()==sleep3) this->setProgress(progress()+25);
if (progress()>98) setProgress(100);
//if ((int)event->type()==acabado) this->done(0);
}









/*
void Modgenps::download_form(QWidget *parent,QString tempname,QString inname)
{//tempname: Nombre del fichero temporal donde se guardara el pdf
//inname: Nombre del fichero temporal donde se convertira el pdf a postscript
QString command,url;
//command="wget http://a104.g.akamai.net/f/104/3242/15m/www.aeat.es/formularios/captura/300/mod300e.pdf -O"+tempname;
//command="wget 80.59.9.31/bulmages/AA.pdf -O"+tempname;
command="wget http://eurus2.us.es/ssh/ssh-2.4.0.tar.gz -O"+tempname;
url="http://eurus2.us.es/ssh/ssh-2.4.0.tar.gz";

QHttp *conexion=new QHttp();
QFile tmpfile(tempname);
//tmpfile.open(IO_WriteOnly);

QProgressDialog progress("Descargando formulario...", "Cancel", 262461);
QObject::connect(conexion, SIGNAL(dataReadProgress(int,int)), &progress, SLOT(setProgress(int,int)));
QObject::connect(conexion, SIGNAL(done(bool)), this, SLOT(yata(bool)));
//QObject::connect(&QObject(conexion), SIGNAL(ReadProgress(int,int)), &QObject(this), SLOT(printda(int,int)));


cout << "Descargando desde " << url <<" hasta " << tempname << "\n";
conexion->setHost("80.59.9.31");
conexion->get("/bulmages/AA.pdf",&tmpfile);
conexion->closeConnection();

progress.exec();
}
void Modgenps::yata(bool error)
{
cout << "Hemos acabado!!\n";
 if (error) cout << "QUE ALGO HA SALIDO MAAAAAAAL\n";

}
 */
 

   