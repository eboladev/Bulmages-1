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
#include <fstream.h>
#include <iostream.h>
#include <qstring.h>
#include "modelosps.h"
#include "tomain.h"
#include "configuracion.h"

void Mod300ps::generaps(){
#define CONF_PROGDATA 800

confpr->setValor(CONF_PROGDATA,"/usr/share/bulmages/");


QString psname,cad1;


psname=tempnam(NULL,"m300_");
fich.open(psname);
//fich.open("/home/davidi/textop1.ps");
if (fich.is_open())
{

cad1=confpr->valor(CONF_PROGDATA)+"formularios/mod300-1.ps.part";
fichlec.open(cad1);
fich << fichlec.rdbuf(); // Copia el contenido de fichlec en fich (primer trozo de postscript)
fichlec.close();// explicit close, unnecessary in this case


cad1=confpr->valor(CONF_PROGDATA)+"formularios/mod300-defs.ps.part";
fichlec.open(cad1);
fich << fichlec.rdbuf(); // Copia el contenido de fichlec en fich (definiciones necesarias para nuestro postscript)
fichlec.close();// explicit close, unnecessary in this case


//formatdigits(&cad1,&cad2,ivas16); //iva 16% en coordenadas 328 516



escrizqder(baser16,328,516);//Casilla 01
escrizqder(baser7,328,502);//Casilla 04
escrizqder(baser4,328,487);//Casilla 07

escrizq(16,435,516);//Casilla 02
escrizq(7,435,502);//Casilla 05
escrizq(4,435,487);//Casilla 08

escrizqder(ivar16,531,516);//Casilla 03
escrizqder(ivar7,531,502);//Casilla 06
escrizqder(ivar4,531,487);//Casilla 09


escrizqder(ivar16+ivar7+ivar4,531,400);//Casilla 21

// escrizqder(bases16,328,516);
// escrizqder(bases7,328,502);
// escrizqder(bases4,328,487);
// 
// escrizqder(ivas16,,516);
// escrizqder(ivas7,328,502);
// escrizqder(ivas4,328,487);

escrizqder(ivas4+ivas7+ivas16,532,366);//Casilla 22
escrizqder(ivas4+ivas7+ivas16,532,295);//Casilla 27

float cas34=ivas4+ivas7+ivas16-(ivar16+ivar7+ivar4);
escrizqder(cas34,532,196);//Casilla 34

if (cas34<0)  escrizqder(-cas34,248,145);

//escrder("X",394,134);
marca_casilla(396,134);


cad1=confpr->valor(CONF_PROGDATA)+"formularios/mod300-2.ps.part";
fichlec.open(cad1);
fich << fichlec.rdbuf(); // Copia el contenido de fichlec en fich (segundo trozo de postscript)
fichlec.close();// explicit close, unnecessary in this case

//Aqui va todo el codigo para la segunda pagina
escrizqder(ivas4+ivas7+ivas16,532,366);//Casilla 22
escrizqder(ivas4+ivas7+ivas16,532,295);//Casilla 27


cad1=confpr->valor(CONF_PROGDATA)+"formularios/mod300-3.ps.part";
fichlec.open(cad1);
fich << fichlec.rdbuf(); // Copia el contenido de fichlec en fich (tercer trozo de postscript)
fichlec.close();// explicit close, unnecessary in this case



cad1=confpr->valor(CONF_PROGDATA)+"formularios/mod300-4.ps.part";
fichlec.open(cad1);
fich << fichlec.rdbuf(); // Copia el contenido de fichlec en fich (cuartotrozo de postscript)
fichlec.close();// explicit close, unnecessary in this case



fich.close();
cout << "Se supone que tengo que leer los formularios desde " << confpr->valor(CONF_PROGDATA) << "\n";
QString command;
command="kghostview "+psname;
system(command);
command="rm "+psname+" 2>/dev/null";
system(command);

}//End of if fich.open
	
}//End of function generaps


void Modgenps::formatdigits(QString *cad1,QString *cad2,float x)
{QString tempstr;
tempstr.sprintf("%.2f",x);//Formatea numero x con 2 cifras decimales (centimos de euro)
*cad2=tempstr.section('.',1,1);
*cad1=tempstr.section('.',0,0);
cad1->append(',');
}



void Modgenps::escrizq(QString cad,int x,int y)
{
/** Genera codigo postscript para escribir cad alineado a la derecha, suponiendo fuente Courier-Bold 12
*/
fich << "("<< cad<< ") " << x << " "<< y << " " << cad.length() << " escrizq\n";
}
void Modgenps::escrizq(float valor,int x,int y)
{QString tempstr;
tempstr.sprintf("%g",valor);
escrizq(tempstr,x,y);
}



void Modgenps::escrder(QString cad,int x,int y)
{
/** Genera codigo postscript para escribir cad alineado a la izquierda, suponiendo fuente Courier-Bold 12
*/
fich << "("<< cad<< ") " << x << " " << y << " "<< cad.length() << " escrder\n";
}
void Modgenps::escrder(float valor,int x,int y)
{QString tempstr;
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
escrizq(cad1,x,y);
escrder(cad2,x,y);
}

void Modgenps::marca_casilla(int x,int y)
{
escrder("X",x-2,y);
}


