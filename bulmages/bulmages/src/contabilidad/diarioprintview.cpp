/***************************************************************************
                          diarioprintview.cpp  -  description
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
 *                                                                         *
 ***************************************************************************/

#include "diarioprintview.h"
#include <unistd.h>
// EStas inclusiones son las primeras pruebas con rtkserver
// EL SISTEMA DE REPORTS.

/*
#include <qapplication.h>
#include <qfont.h>
#include <qfontdatabase.h>
#include <qstring.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qtextcodec.h>
#include <qtranslator.h>
#include <qpicture.h>
#include "rtkserver/rtkserver.h"
#include "rtkserver/rtk.h"
#include "rtkserver/rtkqtreportviewer.h"
//#include "rtkserver/rtkinputsql.h"
#include "rtkserver/rtkinputpsql.h"
// Fin de las inclusiones con rtk
*/

DiarioPrintView::DiarioPrintView(QWidget *parent, const char *name ) : DiarioPrintDlg(parent,name) {
   fichero=NULL;
}
DiarioPrintView::~DiarioPrintView(){
}



// *********************** PRUEBAS CON LA LIBRERIA DE REPORTS DE S.CAPEL
void DiarioPrintView::pruebasRTK() {

   /*
     fprintf(stderr,"--------------------\npruebasRTK\n");
//     RTK::Input *orinput;
      RTK::InputPSql myinput;

//     myquery.init1(conexionbase);

     RTK::Report unReport("", RTK::chars, "lopd");
//     unReport.readXml("../reports/diario.xml");
    unReport.readXml("/usr/share/bulmages/reports/diario.xml");
//    orinput = unReport.getInput();

     myinput.init1(conexionbase);
//     myinput.init();


//    string query = "SELECT asiento.ordenasiento, cuenta.codigo, cuenta.descripcion AS nomcuenta, apunte.debe AS debe, apunte.haber AS haber FROM apunte, cuenta, asiento WHERE  cuenta.idcuenta=apunte.idcuenta AND apunte.idasiento=asiento.idasiento ORDER BY ordenasiento";
   // Cogemos los valores de la fecha del formulario.
   QString finicial = fechainicial1->text();
   QString ffinal = fechafinal1->text();

    QString query;
    query.sprintf("SELECT asiento.ordenasiento, cuenta.codigo, cuenta.descripcion AS nomcuenta, apunte.debe AS debe, apunte.haber AS haber FROM apunte, cuenta, asiento WHERE  cuenta.idcuenta=apunte.idcuenta AND apunte.idasiento=asiento.idasiento ANd apunte.fecha >='%s' AND apunte.fecha<='%s'  ORDER BY ordenasiento, apunte.fecha", finicial.latin1(), ffinal.latin1());
    myinput.setQuery(query.latin1());

    unReport.insertInput(&myinput);

    unReport.debug_print();


//    RTK::OutputFile salida("salida.txt", RTK::chars, 80, 10, 0, 2,2,2,2);
    RTK::OutputQPainter salida(RTK::A3, RTK::dots,
      81, 81, 0, 0, 20, 20, 20, 20);
    unReport.print(salida);

    RTK::QReportViewer viewer(&salida);
    viewer.setPageDimensions((int)(salida.getSizeX()), (int)(salida.getSizeY()));
    viewer.setPageCollection(salida.getPageCollection());
    viewer.showMaximized();
    viewer.slotFirstPage();
    viewer.exec();
//   viewer.show();
   fprintf(stderr,"------------------\nfin de pruebasRTK\n");


   */
}// end pruebasRTK


int DiarioPrintView::inicializa(postgresiface2 *conn) {
    conexionbase = conn;
    return(0);
}


void DiarioPrintView::inicializa1(QString fechai, QString fechaf) {
   fechainicial1->setText(fechai);
   fechafinal1->setText(fechaf);
}

/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/
void DiarioPrintView::accept() {
// Versió per si només permetem escollir una opció
   if (radiotexto->isChecked()) {
      if (radionormal->isChecked()) {
         presentar("txt");
      } else if (radioaprendizaje->isChecked()) {
         presentar("txtapren");
      }// end if
   } else  if (radiohtml->isChecked()){
      if (radionormal->isChecked()) {
         presentar("html");
      } else if (radioaprendizaje->isChecked()) {
         presentar("htmlapren");
      }// end if
   } else if (radiopropietario->isChecked()) {
      pruebasRTK();
   }// end if
}// end accept


// Versió per si decidim que es poden escollir vàries opcions

/*

   if (radionormal->isChecked()){
      if (radiotexto->isChecked()) presentar("txt");
      if (!(radiotexto->isChecked())) presentar("html");
   }
   if (!(radionormal->isChecked())){ 
      if (radiotexto->isChecked()) presentar("txtapren");
      if (!(radiotexto->isChecked())) presentar("htmlapren");
   }
}

*/



void DiarioPrintView::presentar(char * tipus){

   int txt, html, txtapren, htmlapren;
   int error;
   int pid;
   float debe, haber;
   int idcuenta;
   int idasiento;
   char data[15];
   string fecha;
   string fechaasiento;
   string descripcion;
   string concepto;
   string codigocuenta;
   string cad;
   cursor2 *cursoraux, *cursoraux1, *cursoraux2;

// tipus de presentació
   txt=!strcmp(tipus,"txt");
   html=!strcmp(tipus,"html");
   txtapren=!strcmp(tipus,"txtapren");
   htmlapren=!strcmp(tipus,"htmlapren");

   // Cogemos los valores del formulario.
   QString finicial = fechainicial1->text();
   QString ffinal = fechafinal1->text();

   if (txt | html) {
      char *argstxt[]={"diari.txt","diari.txt",NULL};      //presentació txt normal
      char *argshtml[]={"diari.html","diari.html",NULL};   //presentació html normal

      ofstream fitxersortidatxt(argstxt[0]);     // creem els fitxers de sordida
      ofstream fitxersortidahtml(argshtml[0]);

      if (!fitxersortidatxt) txt=0;    // verifiquem que s'hagin creat correctament els fitxers
      if (!fitxersortidahtml) html=0;  // es pot millorar el tractament d'errors

      if (txt | html) {                // només continuem si hem pogut crear algun fitxer
         if (txt) {
            //presentació txt normal

            fitxersortidatxt.setf(ios::fixed);
            fitxersortidatxt.precision(2);

            fitxersortidatxt << "                                        LLIBRE DIARI \n" ;
            fitxersortidatxt << "Data Inicial: " << finicial.latin1() << "   Data Final: " << ffinal.latin1() << endl;
            fitxersortidatxt << "ASIENTO   FECHA    SUBCUENTA   DESCRIPCION                                   DEBE     HABER \n" ;
            fitxersortidatxt << "_______________________________________________________________________________________________\n";
         }
         if (html) {
            //presentació html normal

            fitxersortidahtml.setf(ios::fixed);
            fitxersortidahtml.precision(2);

            fitxersortidahtml << "<html>\n";
            fitxersortidahtml << "<head>\n";
            fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
            fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
//            fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
            // TBR 09/11/03 Pruebas con la plantilla de CSS de HTML
            fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\""<< confpr->valor(CONF_PLANTILLA) <<"\" TYPE=\"text/css\" MEDIA=screen>\n";
            
            fitxersortidahtml << "  <title> Llibre Diari </title>\n";
            fitxersortidahtml << "</head>\n";
            fitxersortidahtml << "<body>\n";
            fitxersortidahtml << "<table><tr><td colspan=\"6\" class=titoldiari> Llibre diari <hr></td></tr>\n\n";
            fitxersortidahtml << "<tr><td colspan=\"6\" class=periodediari> Data Inicial: " << finicial.latin1() << " -  Data Final: " << ffinal.latin1() << "<hr></td></tr>\n\n";
            fitxersortidahtml << "<tr><td class=titolcolumnadiari>Assentament</td><td class=titolcolumnadiari>Data</td><td class=titolcolumnadiari>Subcompte</td><td class=titolcolumnadiari>Descripció</td><td class=titolcolumnadiari>Deure</td><td class=titolcolumnadiari>Haver</td></tr>\n";
         }

         conexionbase->begin();
         cursoraux = conexionbase->cargaasientosfecha((char *)finicial.latin1(),(char *)ffinal.latin1());

         for(;!cursoraux->eof();cursoraux->siguienteregistro()) {
            fechaasiento = cursoraux->valor(2).latin1();
            idasiento = atoi(cursoraux->valor(0).latin1());
            cursoraux1 = conexionbase->cargaapuntes(idasiento);

            for(;!cursoraux1->eof();cursoraux1->siguienteregistro()) {
               fecha = cursoraux1->valor(4).latin1();
               descripcion = cursoraux1->valor(5).latin1();
               concepto = cursoraux->valor(1).latin1();
               debe = atof(cursoraux1->valor(8).latin1());
               haber = atof(cursoraux1->valor(9).latin1());
               idcuenta = atoi(cursoraux1->valor(6).latin1());

               strncpy(data,fecha.c_str(),10);
               data[10]='\0';

               cursoraux2 = conexionbase->cargacuenta(idcuenta, NULL);
               if (!cursoraux2->eof()) {
                  codigocuenta = cursoraux2->valor(1).latin1();
               }
               cursoraux2->cerrar();
               delete cursoraux2;
               if (txt) {
                  //presentació txt normal
                  fitxersortidatxt << setw(5) << idasiento << "  " << setw(10) << data << " " << setw(10) << codigocuenta.c_str() <<  "  " << setw(40) <<  setiosflags(ios::left) << descripcion.c_str() <<  resetiosflags(ios::left)<< " " << setw(10) << debe << " " << setw(10) << haber << endl;
               }
               if (html){
                  //presentació html normal
               fitxersortidahtml << "<tr><td class=assentamentdiari>" << idasiento << "</td><td class=datadiari>" << data << "</td><td class=codicomptediari>" << codigocuenta.c_str() << "</td><td class=descripciodiari>" <<  descripcion.c_str() << "</td><td class=dosdecimals>" << debe << "</td><td class=dosdecimals>" << haber << "</td></tr>\n";
               }

            }

            cursoraux1->cerrar();
            delete cursoraux1;

         }

         delete cursoraux;
         conexionbase->commit();
         if (html){
            fitxersortidahtml << "</table>\n<hr>\n</body>\n</html>\n";     //presentació html normal
            fitxersortidahtml.close();
         }
         if (txt) {
            fitxersortidatxt.close();
            fitxersortidatxt << "_______________________________________________________________________________________________\n";
         }
      }

      if ((pid=fork()) < 0) {
         perror ("Fork failed");
         exit(errno);
      }

      if (txt) {
         //presentació txt normal
         if (!pid) {
            error = execvp(confpr->valor(CONF_EDITOR).c_str(),argstxt);
         }
      }   
      if (html) {
         //presentació html normal
         if (!pid) {
            error = execvp(confpr->valor(CONF_NAVEGADOR).c_str(),argshtml);
         }
      }
   }      
   if (txtapren | htmlapren ) {

      char *argstxt[]={"diariaprenentatge.txt","diariaprenentatge.txt",NULL};      //presentació txt normal
      char *argshtml[]={"diariaprenentatge.html","diariaprenentatge.html",NULL};   //presentació html normal

      ofstream fitxersortidatxt(argstxt[0]);     // creem els fitxers de sordida
      ofstream fitxersortidahtml(argshtml[0]);

      if (!fitxersortidatxt) txtapren=0;    // verifiquem que s'hagin creat correctament els fitxers
      if (!fitxersortidahtml) htmlapren=0;  // es pot millorar el tractament d'errors

      if (txtapren | htmlapren){       // només continuem si hem pogut crear algun fitxer


         if (txtapren) {
            //presentació txt format aprenentatge

            fitxersortidatxt.setf(ios::fixed);
            fitxersortidatxt.precision(2);

            fitxersortidatxt << "                                                      LLIBRE DIARI \n" ;
            fitxersortidatxt << "Data Inicial: " << finicial.latin1() << "   Data Final: " << ffinal.latin1() << endl;
            fitxersortidatxt << "___________________________________________________________________________________________________________________________________\n";

         }
         if (htmlapren) {
            //presentació html format aprenentatge

            fitxersortidahtml.setf(ios::fixed);
            fitxersortidahtml.precision(2);

            fitxersortidahtml << "<html>\n";
            fitxersortidahtml << "<head>\n";
            fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
            fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
            fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
            fitxersortidahtml << "  <title> Llibre Diari </title>\n";
            fitxersortidahtml << "</head>\n";
            fitxersortidahtml << "<body>\n";
            fitxersortidahtml << "<table><tr><td colspan=\"7\" class=titoldiariapren> Llibre diari <hr></td></tr>\n\n";
            fitxersortidahtml << "<tr><td colspan=\"7\" class=periodediariapren> Data Inicial: " << finicial.latin1() << " -  Data Final: " << ffinal.latin1() << "<hr></td></tr>\n\n";
         }
         conexionbase->begin();
         cursoraux = conexionbase->cargaasientosfecha((char *)finicial.latin1(),(char *)ffinal.latin1());

         for(;!cursoraux->eof();cursoraux->siguienteregistro()) {
            int idasiento=atoi(cursoraux->valor("idasiento").latin1());

            if (txtapren) {
               //presentació txt format aprenentatge
               fitxersortidatxt << "_________________________________________________________   " << idasiento << "   ___________________________________________________________________\n";
            }
            if (htmlapren) {
               //presentació html format aprenentatge
               fitxersortidahtml << "\n<tr><td colspan=\"7\" class=liniadiariapren>____________________________________________________   " << idasiento << "   _____________________________________________________</td></tr>\n" ;
            }

            char consulta[400];
            char codicompte[20];
            sprintf(consulta,"SELECT cuenta.codigo, cuenta.descripcion AS nomcuenta, apunte.debe AS debe, apunte.haber AS haber FROM apunte, cuenta WHERE apunte.idasiento=%d AND apunte.haber=0 AND cuenta.idcuenta=apunte.idcuenta",idasiento);
            conexionbase->begin();
            cursor2 *cursasiento = conexionbase->cargacursor(consulta,"asiento");
            conexionbase->commit();

            while (!cursasiento->eof()) {

               if (txtapren) {
                  //presentació txt format aprenentatge
                  sprintf(codicompte,"( %d )",cursasiento->valor("codigo").latin1());
                  fitxersortidatxt << setw(12) << atof(cursasiento->valor("debe").latin1()) << "  " << setw(10) << codicompte << "  " << cursasiento->valor("nomcuenta").latin1() << endl;
               }
               if (htmlapren) {
                  //presentació html format aprenentatge
                  fitxersortidahtml << " <tr><td class=deurediariapren> " << atof(cursasiento->valor("debe").latin1()) << " </td><td class=codidiariapren> ( " << cursasiento->valor("codigo").latin1() << " ) </td><td class=nomcomptediariapren> " << cursasiento->valor("nomcuenta").latin1() << " </td><td> </td><td> </td><td> </td><td> </td></tr>\n";
               }
               cursasiento->siguienteregistro();

            }

            delete cursasiento;
            sprintf(consulta,"SELECT cuenta.codigo, cuenta.descripcion AS nomcuenta, apunte.debe AS debe, apunte.haber AS haber FROM apunte, cuenta WHERE apunte.idasiento=%d AND apunte.debe=0 AND cuenta.idcuenta=apunte.idcuenta",idasiento);
            conexionbase->begin();
            cursasiento = conexionbase->cargacursor(consulta,"asiento");
            conexionbase->commit();

            while (!cursasiento->eof()) {

               if (txtapren) {
                  //presentació txt format aprenentatge
                  sprintf(codicompte,"( %d )",cursasiento->valor("codigo").latin1());
                  fitxersortidatxt << "                                                                 a  " << setw(30) << cursasiento->valor("nomcuenta").latin1() << "  " << setw(10) << codicompte << "  " << setw(12) <<  atof(cursasiento->valor("haber").latin1()) <<endl;
               }
               if (htmlapren) {
                  //presentació html format aprenentatge
                  fitxersortidahtml << " <tr><td> </td><td> </td><td> </td><td class=adiariapren>  a  </td><td class=nomcomptediariapren> " << cursasiento->valor("nomcuenta").latin1() << " </td><td class=codidiariapren> ( " << cursasiento->valor("codigo").latin1() << " ) </td><td class=haverdiariapren> " <<  atof(cursasiento->valor("haber").latin1()) << " </td></tr>\n";
               }
               cursasiento->siguienteregistro();
            }
            delete cursasiento;
         }

         delete cursoraux;
         conexionbase->commit();

         if (txtapren) {
            fitxersortidatxt << "____________________________________________________________________________________________________________________________________\n" ;//presentació text format aprenentatge
            fitxersortidatxt.close();
         }

         if (htmlapren) {
            fitxersortidahtml << "<tr><td colspan=\"7\"  class=liniadiariapren>\n_____________________________________________________________________________________________________________\n<hr></td></tr>\n</table>\n</body></html>\n";     //presentació html format aprenentatge
            fitxersortidahtml.close();
         }



         if (txtapren) {
            //presentació txt format aprenentatge
            if ((pid=fork()) < 0) {
               perror ("Fork failed");
               exit(errno);
            }
            if (!pid) {
               error = execvp(confpr->valor(CONF_EDITOR).c_str(),argstxt);
            }
         }

         if (htmlapren) {
            //presentació html format aprenentatge
            if ((pid=fork()) < 0) {
               perror ("Fork failed");
               exit(errno);
            }
            if (!pid) {
               error = execvp(confpr->valor(CONF_NAVEGADOR).c_str(),argshtml);
            }
         }
      } 
   }   
}
