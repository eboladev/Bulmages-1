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
#include <unistd.h>
#include <qstring.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qstring.h>
#include "modelo300.h"
#include "configuracion.h"
#include "postgresiface2.h"
#include <qmessagebox.h>

#include <q3progressdialog.h>
#include <q3http.h>
#include <qobject.h>
using namespace std;
/**
\brief Constructor de la clase Mod300ps

El constructor llama al cuadro de dialogo para seleccionar parametros adecuados para rellenar el modelo.
Realiza una consulta para obtener las cuentas bancarias.
*/
Mod300ps::Mod300ps(QWidget *parent) :mod300dlg(parent)
{

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
    {
      nombresccc[i]=cur->valor("descripcion");
      nombresccc[i]+="  ";
      nombresccc[i]+=cur->valor("codigo");

      numerccc[i]=cur->valor("bancoent_cuenta");
      cout << nombresccc[i].ascii() << "\t" << numerccc[i].ascii() << "\n";
      combocuentas->insertItem(nombresccc[i]);
      cur->siguienteregistro();
    }
  delete cur;
  delete metabase;

  if (nTuples==0) //Si no tenemos ninguna cuenta de banco...
{
sincuentasbancarias=true;
personalButton->setChecked(true);
personalButtonPressed();
cuentaButton->setDisabled(true);  
}
  
  
  cout << "Objeto Mod300ps generado\n";
}
/** \brief Accept slot for the 300-model dialog.

When pressed, it calls to the \ref generaps method.
*/
void Mod300ps::accept()
{
  m_es_borrador=borradorcheckbox->isChecked();

  if (cuentaButton->isChecked())
    {
      ccc=new numerocuenta(numerccc[combocuentas->currentItem()]);
    }
  else
    ccc=new numerocuenta(banco->text(),entidad->text(),dc->text(),cuenta->text());

  cout << "Elegida cuenta numero "<< ccc->getcodigo("-").ascii() << "\n";

  cout << "dc="<< ccc->getdc().ascii()<<"\n";
  if (!ccc->cuentaesvalida())
    {
      switch( QMessageBox::warning( this, QObject::tr("Formulario 300"),
                                    QObject::tr("Aviso: El nmero de cuenta bancario introducido\n"
                                                "no se corresponde con un CCC correcto."),
                                    QObject::tr("Generar de todas formas"), QObject::tr("Volver"), 0,
                                    0, 1 ))
        {
        case 0:
          generaps();
          break;
        case 1:

          break;
        }
    }
  else
    generaps();

}

/** \brief Generate the postscript of the 300-model with the given parameters.
 
The hardest part is converting the official pdf to postscript.
 */
void Mod300ps::generaps()
{

  QString pdfname,tempname,psname,command;
  char *cad1;
  QString cadaux;

  cout << "Elegido trimestre" << trimestre->currentItem() << "\n";

  //psname=tempnam(NULL,"m300_");//Fichero ps temporal donde convertir el pdf a ps
  tempname=QString(getenv("HOME"))+"/.bulmages/mod300temp.ps";
  /*  if (confpr->valor(CONF_DOWNLOADFORMS)!="0")
    {
    tempname=tempnam(NULL,"m300_");
    inname=tempnam(NULL,"m300_");
  //  download_form(this,tempname,inname);
     command="pdftops "+tempname+" "+inname;
     cout << "Convirtiendo a postscript...\n";
     system(command);
  }
  else*/
  
  pdfname=QString(getenv("HOME"))+"/.bulmages/formularios/mod300e.pdf";
  if (!QFile::exists(pdfname))
    pdfname=confpr->valor(CONF_PROGDATA)+"formularios/mod300e.pdf";

    
    bool doit=true;
  if (!QFile::exists(pdfname))
    {
      QMessageBox::warning( this, QObject::tr("Formulario 300"),
                            QObject::tr("Lo siento, no encuentro el formulario original en pdf.\n"
			                 "Prueba a descargarlo desde www.aeat.es y gu�dalo en\n"
					 "/usr/share/bulmages/formularios/ o en\n"
					 "~/.bulmages/formularios/."),
                             QObject::tr("Aceptar"), 0,0,
                            0, 1 );
			    doit=false;
    }
    
    
     if (doit)
     {
    
    cout << "Convirtiendo a postscript...\n";
    
    
    if (m_es_borrador)
    {
    system("pdftops "+pdfname+" "+tempname);
    }
    else
     {
//    Psprogressdialog progress(QObject::tr("Creando formulario.."), QObject::tr("Cancelar"), 100); 
    Psprogressdialog progress(this,QObject::tr("Creando formulario.."), FALSE, 0); 


    this->convierte_a_postscript=new Genps_thread(pdfname,tempname,&progress);
    progress.setProgress(0);
    
    //LLama a la rutina para convertir el pdf en ps y conservar el dichoso numerito de serie
    //Lo hace mediante un thread, para no bloquear la interfaz de usuario
     this->convierte_a_postscript->start(); 
     
     progress.setModal(true); 
     progress.exec();
     if (progress.wasCanceled())
     {
     system("kill $(ps aux|grep 'Xvfb :5.0'|grep -v grep|awk '{print $2}')");
     doit=false;
     }
     }
     }
    
     //Ahora tengo que procesar tempname y generar psname
     
     if (doit)
     {
     
     psname=QString(getenv("HOME"))+"/.bulmages/mod300.ps";

cout << psname.ascii();
  m_fich.setName(psname);
  if (m_fich.open(QIODevice::WriteOnly))
    {
      m_output.setDevice(&m_fich);


      //m_fichlec.setName(tempname);
      m_fichlec.setName(tempname);
      if (!m_fichlec.open(QIODevice::ReadOnly))
        {
          cout << "Error al abrir fichero de lectura!!\n";
          exit(1);
        }


      m_fichlec.readLine(cad1,256);
	cadaux = cad1;
      while (cadaux.left(7)!="%%Page:")
        {
          m_output << cad1 ;
          m_fichlec.readLine(cad1,256);
        }
      m_output << cad1;
      //Inserta definiciones de postscript al principio de la 1a pagina
      escribe_postscriptdefs();

      m_fichlec.readLine(cad1,256);
      while (cad1!="showpage\n") //hasta que encuentre un showpage
        {
          m_output << cad1;
          m_fichlec.readLine(cad1,256);
        }
	
	if (m_es_borrador) marcadeagua_borrador();
	
 rellena_identificacion();
      rellena_liquidacion();
      rellena_compensacion();
      m_output << cad1;

      m_fichlec.readLine(cad1,256);
      while (cad1!="showpage\n") //hasta que encuentre un showpage
        {
          m_output << cad1;
          m_fichlec.readLine(cad1,256);
        }
	if (m_es_borrador) marcadeagua_borrador();
      rellena_identificacion();
      rellena_liquidacion();
      rellena_compensacion();
      m_output << cad1;

      m_fichlec.readLine(cad1,256);
      while (cad1!="showpage\n") //hasta que encuentre un showpage
        {
          m_output << cad1;
          ;
          m_fichlec.readLine(cad1,256);
        }
	if (m_es_borrador) marcadeagua_borrador();
      rellena_identificacion();
      rellena_compensacion();
      m_output << cad1;



      while (!m_fichlec.atEnd())//Leo el resto del fichero
        {
          m_fichlec.readLine(cad1,256);
          m_output << cad1 << "\n";

        }//end of while eof

      m_fichlec.close();
      m_fich.close();




      cout << "Se supone que tengo que leer los formularios desde " << confpr->valor(CONF_PROGDATA).ascii() << "\n";
      cout << "[TODO] OJO!! Los formularios que genera no son validos, ya que han de tener un numero de serie UNICO\n";
      cout << "[TODO]  Es decir, hay que bajarse de internet uno nuevo CADA VEZ que se haga un modelo nuevo\n";

           command="rm "+tempname+"; kghostview "+psname;
          system(command);

    }//End of if fich.open
  else
    cout << "EEEEHH!!! !QUE NO  HE ABIERTO EL FICHEROOOOOOOOOO!\n";
    }
}//End of function generaps



void Mod300ps::personalButtonPressed()
{
  bool dis=cuentaButton->isChecked();
  
  banco->setDisabled(dis);
  entidad->setDisabled(dis);
  dc->setDisabled(dis);
  cuenta->setDisabled(dis);
  combocuentas->setDisabled(!dis);
}






/** \brief Write definitions in the postscript output file, necessary to insert text later.

It set the Courier-Bold 12pt font, and defines functions in postscript to write left-aligned text and right-aligned text.
\sa \ref escribe_cuenta_bancaria, \ref marca_casilla, \ref Modgenps::escrizq, \ref Modgenps::escrder, \ref Modgenps::escrizqder,
  
 */
void Mod300ps::escribe_postscriptdefs()
{//escribe un clipping path para evitar que aparezcan las casillas de "rellenar formulario" que aparecen si no se usa Acrobat para convertir el pdf a ps
//   output << "newpath\n"
//   "595 815 moveto\n"
//   "0 815 lineto\n"
//   "0 0  lineto\n"
//   "67 0  lineto\n"
//   "67 19.5  lineto\n"
//   "439 19.5 lineto\n"
//   "439 0 lineto\n"
//   "595 0 lineto\n"
//   "closepath\n"
//   "clip\n";

  m_output << "%bulmages\n"
  "%Texto introducido manualmente\n"
  "/Courier-Bold\n"
  "findfont\n"
  "12 scalefont\n"
  "setfont\n"
  "%(texto) numcar escrizq\n"
  "% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
  "/escrizq{\n"
  "% (texto) x_barrita y_barrita numcar escrizq\n"
  " -7  mul -5 add exch 3 add 3 -2 roll add  exch moveto\n"
  "  show\n"
  "  } def\n"
  "  %\n"
  "  /escrder{\n"
  "  % (texto) x_barrita y_barrita numcar escrizq\n"
  "  %newpath\n"
  "  exch 3 add 3 -2 roll add 5 add exch moveto\n"
  "  show} def\n";



}


/** \brief Write the CCC-number in the 300-model.
    
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
    \todo Solucionar problema cuando el nombre de la empresa (u otro campo) lleva acentos.
    \todo El tel�ono no cabe (pero no es culpa m�!!)
 */
void Mod300ps::rellena_identificacion()
{
  QString cad1;
  
  postgresiface2 *m = new postgresiface2();

 m->inicializa("bulmages");//[TODO] CAMBIAR!!!!
escrder(m->propiedadempresa("CIF"),78,601);
escrder(m->propiedadempresa("NombreEmpresa"),213,601);
escrder(m->propiedadempresa("TipoVia"),78,576);
escrder(m->propiedadempresa("NombreVia"),141,576);
escrder(m->propiedadempresa("NumeroVia"),389,576);
escrder(m->propiedadempresa("Escalera"),431,576);
escrder(m->propiedadempresa("Piso"),461,576);
escrder(m->propiedadempresa("Puerta"),490,576);
///\bug Por ahora, el nmero de tel�ono no cabe!!
// escrder(m->propiedadempresa("Telefono"),518,576);
escrder(m->propiedadempresa("Municipio"),78,550);
escrder(m->propiedadempresa("Provincia"),360,550);
escribe_codigo_postal(m->propiedadempresa("CodPostal"));



delete m;


  cad1.sprintf("%d",trimestre->currentItem()+1);//Si elegido item 0 ---> cad1="1", etc.

  marca_casilla(cad1,452,690);
  marca_casilla("T",467,690);





  escrder(empresa,209,601);
  //(453,706)+(14,0)
  for (int i=0;i<4;i++)
    marca_casilla(QString(ano[i]),453+i*14,706);


}
/** \brief Write the data in the second part of the 300-model
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



/**
 * \file modelo300.cpp
 * \brief Modelo 300 (implementaci�)
 *
 * \author David Guti�rez Rubio
 */


/** Escribe el codigo postal en las minusculas casillas
destinadas a tal fin.
*/
void Mod300ps::escribe_codigo_postal(QString cod)
{int offset=3;
marca_casilla(QString(cod[0]),528-offset,550);
marca_casilla(QString(cod[1]),537-offset,550);
marca_casilla(QString(cod[2]),546-offset,550);
marca_casilla(QString(cod[3]),554-offset,550);
marca_casilla(QString(cod[4]),563-offset,550);


}
