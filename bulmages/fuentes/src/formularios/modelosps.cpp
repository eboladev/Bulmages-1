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
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qstring.h>
#include "modelosps.h"
//#include "tomain.h"
#include "configuracion.h"
#include "postgresiface2.h"

#include <qmessagebox.h>


Mod300ps::Mod300ps(QWidget *parent) :mod300dlg(parent)
{
  //Realizo una consulta para obtener las cuentas bancarias

  QString query="select descripcion,bancoent_cuenta,codigo from cuenta where codigo like '572%%' and codigo>572";

  postgresiface2 *metabase = new postgresiface2();
  metabase->inicializa("bulmages");//[TODO] CAMBIAR!!!!
  metabase->begin();
  fprintf(stderr,"%s\n",query.ascii());
  cursor2 *cur = metabase->cargacursor(query,"bancos");



  int nTuples=cur->numregistros();
  nombresccc=new QString[nTuples];

  numerccc=new QString[nTuples];


  for (int i=0;i<nTuples;i++)
    //while (!cur->eof())
    {
      nombresccc[i]=cur->valor("descripcion");
      nombresccc[i]+="  ";
      nombresccc[i]+=cur->valor("codigo");

      numerccc[i]=cur->valor("bancoent_cuenta");
      cout << nombresccc[i] << "\t" << numerccc[i]<< "\n";
      combocuentas->insertItem(nombresccc[i]);
      cur->siguienteregistro();
    }
  delete cur;
  delete metabase;

  //Ahora realizo otra consulta para obtener datos sobre la empresa

  query="select nombre,ano from empresa where nombredb='bulmages'";

  metabase = new postgresiface2();
  metabase->inicializa("metabd");//[TODO] CAMBIAR!!!
  metabase->begin();
  fprintf(stderr,"%s\n",query.ascii());
  cur = metabase->cargacursor(query,"datos");

  if (cur->numregistros()>1)
    {
      cout << "ERROR: Empresa duplicada!!\n";
    }

  empresa=cur->valor("nombre");
  ano=cur->valor("ano");


  cout << "Para " << empresa << ", " << ano << "\n";


  cout << "Objeto Mod300ps generado\n";
}
void Mod300ps::accept()
{
  cout << "Generando vista preliminar...\n";

  if (cuentaButton->isChecked())
    {
      ccc=new numerocuenta(numerccc[combocuentas->currentItem()]);
    }
  else
    ccc=new numerocuenta(banco->text(),entidad->text(),dc->text(),cuenta->text());

  cout << "Elegida cuenta numero "<< ccc->getcodigo("-") << "\n";

  cout << "dc="<< ccc->getdc()<<"\n";
  if (!ccc->cuentaesvalida())
    {
      switch( QMessageBox::warning( this, tr("Formulario 300"),
                                    tr("Aviso: El número de cuenta bancario introducido\n"
                                       "no se corresponde con un CCC correcto."),
                                    tr("Generar de todas formas"), tr("Volver"), 0,
                                    0, 1 ))
        {
        case 0: // Try again or Enter
          generaps();
          break;
        case 1: // Quit or Escape

          break;
        }
    }
  else
    generaps();

}

void Mod300ps::generaps()
{

  QString psname,cad1;




  cout << "Elegido trimestre" << trimestre->currentItem() << "\n";


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


      //Aqui va el contenido de la pagina 1
      rellena_identificacion();
      rellena_liquidacion();
      rellena_compensacion();




      cad1=confpr->valor(CONF_PROGDATA)+"formularios/mod300-2.ps.part";
      fichlec.open(cad1);
      fich << fichlec.rdbuf(); // Copia el contenido de fichlec en fich (segundo trozo de postscript)
      fichlec.close();// explicit close, unnecessary in this case

      //Aqui va todo el codigo para la segunda pagina
      rellena_identificacion();
      rellena_liquidacion();
      rellena_compensacion();

      cad1=confpr->valor(CONF_PROGDATA)+"formularios/mod300-3.ps.part";
      fichlec.open(cad1);
      fich << fichlec.rdbuf(); // Copia el contenido de fichlec en fich (tercer trozo de postscript)
      fichlec.close();// explicit close, unnecessary in this case

      rellena_identificacion();
      rellena_compensacion();


      cad1=confpr->valor(CONF_PROGDATA)+"formularios/mod300-4.ps.part";
      fichlec.open(cad1);
      fich << fichlec.rdbuf(); // Copia el contenido de fichlec en fich (cuartotrozo de postscript)
      fichlec.close();// explicit close, unnecessary in this case
      fich.close();
      cout << "Se supone que tengo que leer los formularios desde " << confpr->valor(CONF_PROGDATA) << "\n";
      cout << "[TODO] ¡¡OJO!! Los formularios que genera no son validos, ya que han de tener un numero de serie UNICO\n";
      cout << "[TODO]  Es decir, hay que bajarse de internet uno nuevo CADA VEZ que se haga un modelo nuevo\n";
      QString command;

      int pid;
      if ((pid = fork()) < 0)
        {
          cout << "Error haciendo fork()\n";
          exit(1);
        }

      if (pid==0)
        {
          command="kghostview "+psname;
          system(command);
          command="rm "+psname+" 2>/dev/null";
          system(command);

          exit(1);
        }
    }//End of if fich.open

}//End of function generaps


void Modgenps::formatdigits(QString *cad1,QString *cad2,float x)
{
  QString tempstr;
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
{
  QString tempstr;
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
  escrizq(cad1,x,y);
  escrder(cad2,x,y);
}

void Modgenps::marca_casilla(QString marca,int x,int y)
{
  escrder(marca,x-2,y);
}

void Mod300ps::personalButtonPressed()
{
  bool dis=cuentaButton->isChecked();
  banco->setDisabled(dis);
  entidad->setDisabled(dis);
  dc->setDisabled(dis);
  cuenta->setDisabled(dis);
  combocuentas->setDisabled(!dis);
}







/*!
    \fn Mod300ps::escribe_cuenta_bancaria
 */
void Mod300ps::escribe_cuenta_bancaria(int x,int y)
{

  const int steps[]=
    {
      0,11,11,11,12,11,11,11,12,11,12,11,11,11,12,11,11,11,12,11,11
    };
  //76,32 para devolver
  //338,73 para pagar
  int acum=x;//coordenada x de la primera casilla a rellenar
  QString tem=ccc->getcodigo();
  for (int i=0;i<20;i++)
    {
      acum+=steps[i];
      marca_casilla(QString(tem[i]),acum,y);
    }
}


/*!
    \fn Mod300ps::rellena_identificacion()
 */
void Mod300ps::rellena_identificacion()
{
  QString cad1;





  cad1.sprintf("%d",trimestre->currentItem()+1);//Si elegido item 0 ---> cad1="1", etc.

  marca_casilla(cad1,452,690);
  marca_casilla("T",467,690);





  escrder(empresa,209,601);
  //(453,706)+(14,0)
  for (int i=0;i<4;i++)
    marca_casilla(QString(ano[i]),453+i*14,706);


}
/*!
    \fn Mod300ps::rellena_liquidacion()
 */
void Mod300ps::rellena_liquidacion()
{

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

  escrizqder(ivas4+ivas7+ivas16,532,366);//Casilla 22
  escrizqder(ivas4+ivas7+ivas16,532,295);//Casilla 27
  cas34=ivas4+ivas7+ivas16-(ivar16+ivar7+ivar4);
  escrizqder(cas34,532,196);//Casilla 34

}

/*!
    \fn Mod300ps::rellena_compensacion()
 */
void Mod300ps::rellena_compensacion()
{


  if (cas34<0)
    {
      escrizqder(-cas34,248,145);//Casilla a compensar si la 34 sale negativa

      if (trimestre->currentItem()==3) //si estamos en el cuarto trimestre...
        escribe_cuenta_bancaria(76,32);//ponemos la cuenta bancaria si hay que devolver
    }
  else
    {
      if (trimestre->currentItem()==3) //si estamos en el cuarto trimestre...
        {
          //marca_casilla("X",396,134);//Casilla de pago en efectivo
          marca_casilla("X",464,134);//Casilla de adeudo en cuenta
          //76,32 para devolver
          //338,73 para pagar
          escribe_cuenta_bancaria(338,73);
        }
    }


}
