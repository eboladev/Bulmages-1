/***************************************************************************
                          extractoprintview.cpp  -  description
                             -------------------
    begin                : jue jun 26 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
    email                : tborras@conetxia.com
    modificat per        : (C) 2003 Antoni Mirabete i Terés - amirabet@biada.org
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include <unistd.h>
#include <qwidget.h>

#include "extractoprintview.h"
#include "listcuentasview1.h"
#include "selectccosteview.h"
#include "selectcanalview.h"
#include "empresa.h"

#ifdef REPORTS
#include "rtkinputbges.h"
using namespace RTK;
#endif

ExtractoPrintView::ExtractoPrintView(empresa *emp, QWidget *parent=0, const char *name=0 ) : ExtractoPrintDlg(parent,name) {
   fichero=NULL;
   empresaactual=emp;
   conexionbase = emp->bdempresa();
}

ExtractoPrintView::~ExtractoPrintView(){
}

void ExtractoPrintView::inicializa1(QString fechainicial, QString fechafinal, QString codi, QString codf) {
	fechainicial1->setText(fechainicial);
	fechafinal1->setText(fechafinal);
	codigoinicial->setText(codi);
	codigofinal->setText(codf);
}// end inicializa1

/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/
void ExtractoPrintView::accept() {

// Versió per si només permetem escollir una opci

   if (radiotexto->isChecked()) presentar("txt");
   else if (radiohtml->isChecked()) presentar("html");
   else pruebasRTK();

}


// *********************** PRUEBAS CON LA LIBRERIA DE REPORTS DE S.CAPEL
void ExtractoPrintView::pruebasRTK() {
#ifdef REPORTS
cursor2 *cursoraux;
conexionbase->begin();

cursoraux=conexionbase->cargacursor("SELECT ordenasiento, asiento.idasiento AS idasiento, cuenta.descripcion AS descripcion, apunte.debe AS debe , apunte.haber AS haber, conceptocontable, idc_coste, codigo, cuenta.descripcion AS desc1, apunte.fecha AS fecha FROM (asiento LEFT JOIN apunte ON asiento.idasiento=apunte.idasiento) LEFT JOIN cuenta ON apunte.idcuenta=cuenta.idcuenta ORDER BY ordenasiento", "unquery");

conexionbase->commit();


RTK::Report unReport;
unReport.readXml(confpr->valor(CONF_DIR_REPORTS)+"extracto1.rtk");
InputBGes *inp = static_cast<InputBGes *>(unReport.getInput());
inp->set(InputBGes::diario, empresaactual, cursoraux);
OutputQPainter *salida = new OutputQPainter(A4, dots, 57, 59, 0,0,20,20,20,20);
unReport.print(*salida);
QReportViewer *mViewer = new QReportViewer(salida, true, 0, 0, WShowModal | WDestructiveClose );
mViewer->setCaption(tr("GongReport", "Informe: "));
mViewer->setPageDimensions((int)(salida->getSizeX()), (int)(salida->getSizeY()));
mViewer->setPageCollection(salida->getPageCollection());
mViewer->show();
mViewer->slotFirstPage();
#endif 
}// end pruebasRTK

// Versió per si decidim que es poden escollir vàries opcions

/*
   if (radiotexto->isChecked()) presentar("txt");
   if (!(radiotexto->isChecked())) presentar("html");
}

*/


void ExtractoPrintView::presentar(char *tipus) {

   int txt, html;
   int error;
   int pid;
   float debe, haber,saldo;
   float debeinicial=0, haberinicial=0, saldoinicial=0;
   float debefinal, haberfinal, saldofinal;
   int idcuenta;
   int idasiento;
//   char *textasiento;
   int contrapartida;
   int activo;
   string cad;
   cursor2 *cursoraux, *cursoraux1, *cursoraux2;

   QString finicial = fechainicial1->text();
   QString ffinal = fechafinal1->text();
   QString cinicial = codigoinicial->text();
   QString cfinal = codigofinal->text();

// tipus de presentació

   txt=!strcmp(tipus,"txt");
   html=!strcmp(tipus,"html");

   char *argstxt[]={"major.txt","major.txt",NULL};      //presentació txt normal
   char *argshtml[]={"major.html","major.html",NULL};   //presentació html normal

   ofstream fitxersortidatxt(argstxt[0]);     // creem els fitxers de sordida
   ofstream fitxersortidahtml(argshtml[0]);

   if (!fitxersortidatxt) txt=0;    // verifiquem que s'hagin creat correctament els fitxers
   if (!fitxersortidahtml) html=0;  // es pot millorar el tractament d'errors

   if (txt | html){                 // només continuem si hem pogut crear algun fitxer

 

      if (txt) {
         //presentació txt

         fitxersortidatxt.setf(ios::fixed);
         fitxersortidatxt.precision(2);

         fitxersortidatxt << "                                    MAJOR \n" ;
         fitxersortidatxt << "Data Inicial: " << finicial.ascii() << "   Data Final: " << ffinal.ascii() << endl;
         fitxersortidatxt << "_________________________________________________________________________________________________________\n";
      }

      if (html) {
         //presentació html

         fitxersortidahtml.setf(ios::fixed);
         fitxersortidahtml.precision(2);

         fitxersortidahtml << "<html>\n";
         fitxersortidahtml << "<head>\n";
         fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
         fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
         fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
         fitxersortidahtml << "  <title> Major </title>\n";
         fitxersortidahtml << "</head>\n";
         fitxersortidahtml << "<body>\n";
         fitxersortidahtml << "<table><tr><td colspan=\"6\" class=titolmajor> Major <hr></td></tr>\n\n";
         fitxersortidahtml << "<tr><td colspan=\"6\" class=periodemajor> Data Inicial: " << finicial.ascii() << " -  Data Final: " << ffinal.ascii() << "<hr></td></tr>\n\n";
      }

      conexionbase->begin();
      cursoraux = conexionbase->cargacuentascodigo(-1,cinicial, cfinal);
      conexionbase->commit();

      while(!cursoraux->eof()) {
         idcuenta = atoi(cursoraux->valor(0).ascii());
	 conexionbase->begin();
         cursoraux1 = conexionbase->cargaapuntesctafecha(idcuenta, finicial.ascii(), ffinal.ascii());
	 conexionbase->commit();

         if (!cursoraux1->eof()) {
            activo = strcmp((char *) cursoraux->valor(13).ascii() , "f");

            if (txt) {
               fitxersortidatxt << "\n\n" << setw(12) << cursoraux->valor(1).ascii() << setw(50) << cursoraux->valor(2).ascii() << endl;
               if (activo) {
                  fitxersortidatxt << " Compte d'Actiu";
               } else {
                  fitxersortidatxt << " Compte de Passiu";
               }

            }

            if (html) {
               fitxersortidahtml << "<tr><td colspan=\"6\" class=comptemajor>" << cursoraux->valor(1).ascii() << " "<< cursoraux->valor(2).ascii() << "</td></tr>\n";
               if (activo) {
                  fitxersortidahtml << "<tr><td colspan=\"6\" class=tipuscomptemajor> Compte d'Actiu </td></tr>\n";
               } else {
                  fitxersortidahtml << "<tr><td colspan=\"6\" class=tipuscomptemajor> Compte de Passiu </td></tr>\n";
               }

            }
		conexionbase->begin();
            cursoraux2 = conexionbase->cargasaldoscuentafecha(idcuenta, (char *)finicial.ascii());
	    conexionbase->commit();
            if (!cursoraux2->eof()) {
               debeinicial = atof(cursoraux2->valor(0).ascii());
               haberinicial = atof(cursoraux2->valor(1).ascii());
               if (activo) {
                  saldoinicial = debeinicial - haberinicial;
               } else {
                  saldoinicial = haberinicial-debeinicial;
               }

               if (txt) {
                  //presentació txt
                  fitxersortidatxt << "\nAsiento  Fecha   Contrapartida   Descripcion                          Debe         Haber         Saldo\n";
                  fitxersortidatxt << "                                                 SUMAS ANTERIORES...   " << setw(10) << debeinicial << setw(10) << haberinicial << setw(10) << saldoinicial << endl;
                  fitxersortidatxt << "_________________________________________________________________________________________________________\n";
               }

               if (html) {
                  //presentació html
                  fitxersortidahtml << "<tr><td class=titolcolumnamajor> Asiento </td><td class=titolcolumnamajor> Fecha </td><td class=titolcolumnamajor> Contrapartida </td><td class=titolcolumnamajor> Descripcion </td><td class=titolcolumnamajor> Debe </td><td class=titolcolumnamajor> Haber </td><td class=titolcolumnamajor> Saldo </td></tr>\n";
                  fitxersortidahtml << "<tr><td></td><td></td><td></td><td class=sumamajor> Sumes anteriors...</td><td class=dosdecimals> " << debeinicial << " </td><td class=dosdecimals> " << haberinicial << " </td><td class=dosdecimals> " << saldoinicial << "</td><td>\n";
               }

               saldo = saldoinicial;
               debefinal = debeinicial;
               haberfinal = haberinicial;

               for(;!cursoraux1->eof();cursoraux1->siguienteregistro()) {
                  idasiento=atoi(cursoraux1->valor(2).ascii());
                  contrapartida = atoi(cursoraux1->valor(10).ascii());
                  debe=atof(cursoraux1->valor(8).ascii());
                  haber=atof(cursoraux1->valor(9).ascii());

                  if (activo) {
                     saldo += debe - haber;
                  } else {
                     saldo += haber -debe;
                  }

               debefinal += debe;
               haberfinal += haber;
               cad = cursoraux1->valor(4).ascii();
               //presentació txt
               if (txt) fitxersortidatxt <<  setw(5) << idasiento << setw(14) << cad.substr(0,10).c_str() << setw(10) << contrapartida << "  " << setw(40)  << setiosflags(ios::left) << cursoraux1->valor(5).ascii() << setw(10) << resetiosflags(ios::left) << debe << setw(10) << haber << setw(10) << saldo << endl;
               //presentació html
               if (html) fitxersortidahtml << " <tr><td class=assentamentmajor> " << idasiento << " </td><td> " << cad.substr(0,10).c_str() << " </td><td class=contrapartidamajor> " << contrapartida << " </td><td> " << cursoraux1->valor(5).ascii() << " </td><td class=dosdecimals> " << debe << " </td><td class=dosdecimals> " << haber << " </td><td class=dosdecimals> " << saldo << " </td></tr>\n ";
            }

            if (activo) {
               saldofinal = debefinal - haberfinal;
            } else {
               saldofinal = haberfinal -debefinal;
            }
                if (txt) {
                   //presentació txt
                   fitxersortidatxt << "                                               __________________________________________________________\n";
                   fitxersortidatxt << "                                                  TOTAL SUBCUENTA...   " << setw(10) << debefinal << setw(10) << haberfinal << setw(10) << saldofinal << endl;
                }
                if (html) {
                   //presentació html
                   fitxersortidahtml << "<tr><td></td><td></td><td></td><td class=sumamajor> Total subcompte... </td><td class=dosdecimals>" << debefinal << " </td><td class=dosdecimals> " << haberfinal << " </td><td class=dosdecimals> " << saldofinal << "</td></tr>\n\n";
                }
            cursoraux2->cerrar();
            delete cursoraux2;
         }
      }
         cursoraux1->cerrar();
         delete cursoraux1;
         cursoraux->siguienteregistro();
      }
      if (html) fitxersortidahtml << "\n</table></body></html>\n";     //presentació html
      conexionbase->commit();
      delete cursoraux;
   }
    if (txt) {
       //presentació txt
       fitxersortidatxt.close();
       if ((pid=fork()) < 0) {
          perror ("Fork failed");
          exit(errno);
       }
       if (!pid) {
          error = execvp(confpr->valor(CONF_EDITOR).c_str(),argstxt);
       }
    }
    if (html) {
       //presentació html
       fitxersortidahtml.close();
       if ((pid=fork()) < 0) {
          perror ("Fork failed");
          exit(errno);
       }
       if (!pid) {
          error = execvp(confpr->valor(CONF_NAVEGADOR).c_str(),argshtml);
       }
    }
}



void ExtractoPrintView::boton_codinicial() {
   listcuentasview1 *listcuentas = new listcuentasview1();
   listcuentas->modo=1;
   listcuentas->inicializa(conexionbase);
   listcuentas->exec();
   codigoinicial->setText(listcuentas->codcuenta);
   delete listcuentas;
}// end boton_codinicial

void ExtractoPrintView::boton_codfinal() {
   listcuentasview1 *listcuentas = new listcuentasview1();
   listcuentas->modo=1;
   listcuentas->inicializa(conexionbase);
   listcuentas->exec();
   codigofinal->setText(listcuentas->codcuenta);
   delete listcuentas;
}// end boton_codfinal



void ExtractoPrintView::boton_ccostes() {
   selectccosteview *selccostes = empresaactual->getselccostes();
   selccostes->exec();
}// end boton_ccostes


void ExtractoPrintView::boton_canales() {
   selectcanalview *selcanales = empresaactual->getselcanales();
   selcanales->exec();
}// end boton_ccostes


