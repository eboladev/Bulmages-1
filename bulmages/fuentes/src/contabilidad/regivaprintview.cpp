/***************************************************************************
                          regivaprintview.cpp  -  description
                             -------------------
    begin                : dom jul 13 2003
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

#include "regivaprintview.h"
#ifndef WIN32
#include <unistd.h>
#endif

regivaprintview::regivaprintview(QWidget *parent, const char *name ) : regivaprintdlg(parent,name) {
  	fichero = NULL;
}


regivaprintview::~regivaprintview(){
}

/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/
void regivaprintview::accept() {
// Versió per si només permetem escollir una opci

//   if (radionormal->isChecked()){
      if (radiotexto->isChecked()) presentar("txt");
      else presentar("html");
/*   } else {
      if (radiotexto->isChecked()) presentar("txtapren");
      else presentar("htmlapren");
   } */

}


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




int regivaprintview::inicializa(postgresiface2 *conn ) {
    conexionbase = conn;
    return(0);
}// end inicializa


void regivaprintview::inicializa1(QString finicial1, QString ffinal1) {
	   fechainicial1->setText(finicial1);
      fechafinal1->setText(ffinal1);
}// end if


/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/

void regivaprintview::presentar(char *tipus){
#ifndef WIN32

   int txt, html, txtapren, htmlapren;
   int error;
   int pid;
   QString data;
   QString datahora;

   QString query;

// tipus de presentaci
   txt=!strcmp(tipus,"txt");
   html=!strcmp(tipus,"html");
   txtapren=!strcmp(tipus,"txtapren");
   htmlapren=!strcmp(tipus,"htmlapren");

// Cogemos los valores del formulario.
   QString finicial = fechainicial1->text();
   QString ffinal = fechafinal1->text();

   if (txt | html) {

      char *argstxt[]={"iva.txt","iva.txt",NULL};      //presentació txt normal
      char *argshtml[]={"iva.html","iva.html",NULL};   //presentació html normal

      ofstream fitxersortidatxt(argstxt[0]);     // creem els fitxers de sordida
      ofstream fitxersortidahtml(argshtml[0]);

      if (!fitxersortidatxt) txt=0;    // verifiquem que s'hagin creat correctament els fitxers
      if (!fitxersortidahtml) html=0;  // es pot millorar el tractament d'errors

      if (txt | html) {                // només continuem si hem pogut crear algun fitxer

         int num1;
         cursor2 *cursorapt;

         // Hacemos la consulta de los apuntes a listar en la base de datos.
         //      int idc_coste = ccostes[combocoste->currentItem()];

         // La consulta es compleja, requiere la creación de una tabla temporal y de cierta mandanga por lo que puede

         conexionbase->begin();
         query.sprintf("SELECT * FROM registroiva, cuenta, borrador, asiento  where cuenta.idcuenta=borrador.idcuenta AND borrador.idborrador=registroiva.idborrador AND asiento.idasiento=borrador.idasiento AND cuenta.codigo LIKE '477%%' AND borrador.fecha>='%s' AND borrador.fecha<='%s'",fechainicial1->text().ascii(), fechafinal1->text().ascii());
         fprintf(stderr,"%s\n",query.ascii());
         cursorapt = conexionbase->cargacursor(query,"mycursor");
         conexionbase->commit();
         // Calculamos cuantos registros van a crearse y dimensionamos la tabla.
         num1 = cursorapt->numregistros();
         //      listado->setNumRows(num1);

         int hoja=0;

         if (txt) {
            //presentació txt normal

            fitxersortidatxt.setf(ios::fixed);
            fitxersortidatxt.precision(2);

            fitxersortidatxt << "                                        IVA Repercutit \n" ;
            fitxersortidatxt << "Data Inicial: " << finicial.ascii() << "   Data Final: " << ffinal.ascii() << endl;
            fitxersortidatxt << "Assentament  Data   Compte  Descripció   Base Imponible  % IVA  Quota IVA   Factura  Cif Compte IVA\n" ;
            fitxersortidatxt << "__________________________________________________________________________________________________________\n";
         }
         if (html) {
            //presentació html normal

            fitxersortidahtml.setf(ios::fixed);
            fitxersortidahtml.precision(2);

            fitxersortidahtml << "<html>\n";
            fitxersortidahtml << "<head>\n";
            fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
            fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
            fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
            fitxersortidahtml << "  <title> IVA Repercutit </title>\n";
            fitxersortidahtml << "</head>\n";
            fitxersortidahtml << "<body>\n";
            fitxersortidahtml << "<table><tr><td colspan=\"10\" class=titoliva> IVA Repercutit <hr></td></tr>\n\n";
            fitxersortidahtml << "<tr><td colspan=\"10\" class periodeiva> Data Inicial: " << finicial.ascii() << " -  Data Final: " << ffinal.ascii() << "<hr></td></tr>\n\n";
            fitxersortidahtml << "<tr><td class=titolcolumnaiva> Assentament </td><td class=titolcolumnaiva> Data </td><td class=titolcolumnaiva> Compte </td><td class=titolcolumnaiva> Descripció </td><td class=titolcolumnaiva> Base Imponible </td><td class=titolcolumnaiva> % IVA </td><td class=titolcolumnaiva> Quota IVA </td><td class=titolcolumnaiva> Factura </td><td class=titolcolumnaiva> Cif </td><td class=titolcolumnaiva> Compte IVA </td></tr>\n";
         }


         while (!cursorapt->eof()) {
            datahora=cursorapt->valor("fecha");
            data=datahora.mid(0,10);

            // Acumulamos los totales para al final poder escribirlos
            if (txt) {
               //presentació txt normal
               fitxersortidatxt << setw(10) << cursorapt->valor("idasiento") << " " << data.ascii() << " " << cursorapt->valor("contrapartida").ascii() << " " << cursorapt->valor("descripcion").ascii() << " " << cursorapt->valor("baseimp").ascii() << " " << cursorapt->valor("iva").ascii() << " " << atof(cursorapt->valor("baseimp").ascii())*atof(cursorapt->valor("iva").ascii())/100 << cursorapt->valor("factura").ascii() << " " << cursorapt->valor("cif").ascii() << " " << cursorapt->valor("cif").ascii() <<  endl;
            }
            if (html) {
               //presentació html normal
               fitxersortidahtml << "<tr><td class=assentamentiva>" << cursorapt->valor("idasiento").ascii() << "</td><td class=dataiva>" << data.ascii() << "</td><td class=contrapartidaiva>" << cursorapt->valor("contrapartida").ascii() << "</td><td class=descripcioiva>" << cursorapt->valor("descripcion").ascii() << "</td><td class=baseimponibleiva>" << cursorapt->valor("baseimp").ascii() << "</td><td class=tipusiva>" << cursorapt->valor("iva").ascii() << "</td><td class=quotaiva>" << atof(cursorapt->valor("baseimp").ascii())*atof(cursorapt->valor("iva").ascii())/100 << "</td><td class=facturaiva>" << cursorapt->valor("factura").ascii() << "</td><td class=cifiva>" << cursorapt->valor("cif").ascii() << "</td></tr> \n";
            }
            // Calculamos la siguiente cuenta registro y finalizamos el bucle
            cursorapt->siguienteregistro();
         }// end while
         // Vaciamos el cursor de la base de datos.
         delete cursorapt;


         conexionbase->begin();
         query.sprintf("SELECT * FROM registroiva, cuenta, borrador, asiento  where cuenta.idcuenta=borrador.idcuenta AND borrador.idborrador=registroiva.idborrador AND asiento.idasiento=borrador.idasiento AND cuenta.codigo LIKE '472%%' AND borrador.fecha>='%s' AND borrador.fecha<='%s'",fechainicial1->text().ascii(), fechafinal1->text().ascii());
         fprintf(stderr,"%s\n",query.ascii());
         cursorapt = conexionbase->cargacursor(query,"mycursor");
         conexionbase->commit();
         // Calculamos cuantos registros van a crearse y dimensionamos la tabla.
         num1 = cursorapt->numregistros();
         hoja=0;
         if (txt) {
            //presentació txt normal

            fitxersortidatxt.setf(ios::fixed);
            fitxersortidatxt.precision(2);

            fitxersortidatxt << "                                        IVA Soportat \n" ;
            fitxersortidatxt << "Assentament  Data   Compte  Descripció   Base Imponible  % IVA  Quota IVA   Factura  Cif Compte IVA \n" ;
            fitxersortidatxt << "___________________________________________________________________________________________________________\n";
         }
         if (html) {
            //presentació html normal

            fitxersortidahtml.setf(ios::fixed);
            fitxersortidahtml.precision(2);

            fitxersortidahtml << "<html>\n";
            fitxersortidahtml << "<head>\n";
            fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
            fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
            fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
            fitxersortidahtml << "  <title> IVA Soportat </title>\n";
            fitxersortidahtml << "</head>\n";
            fitxersortidahtml << "<body>\n";
            fitxersortidahtml << "<tr><td colspan=\"10\" class=titoliva> IVA Soportat <hr></td></tr>\n\n";
            fitxersortidahtml << "<tr><td class=titolcolumnaiva> Assentament </td><td class=titolcolumnaiva> Data </td><td class=titolcolumnaiva> Compte </td><td class=titolcolumnaiva> Descripció </td><td class=titolcolumnaiva> Base Imponible </td><td class=titolcolumnaiva> % IVA </td><td class=titolcolumnaiva> Quota IVA </td><td class=titolcolumnaiva> Factura </td><td class=titolcolumnaiva> Cif </td><td class=titolcolumnaiva> Compte IVA </td></tr>\n";
         }

         while (!cursorapt->eof()) {

	    datahora=cursorapt->valor("fecha");
            data=datahora.mid(0,10);

            // Acumulamos los totales para al final poder escribirlos
            if (txt) {
               //presentació txt normal
               fitxersortidatxt << setw(10) << cursorapt->valor("idasiento").ascii() << " " << data.ascii() << " " << cursorapt->valor("contrapartida").ascii() << " " << cursorapt->valor("descripcion").ascii() << " " << cursorapt->valor("baseimp").ascii() << " " << cursorapt->valor("iva").ascii() << " " << atof(cursorapt->valor("baseimp").ascii())*atof(cursorapt->valor("iva").ascii())/100 << " " << cursorapt->valor("factura").ascii() << " " << cursorapt->valor("cif").ascii() << " " << cursorapt->valor("cif").ascii() <<  endl;
            }
            if (html) {
               //presentació html normal
               fitxersortidahtml << "<tr><td class=assentamentiva>" << cursorapt->valor("idasiento").ascii() << "</td><td class=dataiva>" << data.ascii() << "</td><td class=contrapartidaiva>" << cursorapt->valor("contrapartida").ascii() << "</td><td class=descripcioiva>" << cursorapt->valor("descripcion").ascii() << "</td><td class=baseimponibleiva>" << cursorapt->valor("baseimp").ascii() << "</td><td class=tipusiva>" << cursorapt->valor("iva").ascii() << "</td><td class=quotaiva>" << atof(cursorapt->valor("baseimp").ascii())*atof(cursorapt->valor("iva").ascii())/100 << "</td><td class=facturaiva>" << cursorapt->valor("factura").ascii() << "</td><td class=cifiva>" << cursorapt->valor("cif").ascii() << "</td></tr> \n";
            }
            // Calculamos la siguiente cuenta registro y finalizamos el bucle
            cursorapt->siguienteregistro();
         }// end while
         // Vaciamos el cursor de la base de datos.
         delete cursorapt;

#ifndef WIN32
         if (txt) {
            fitxersortidatxt.close();
            //presentació txt normal
            if ((pid=fork()) < 0) {
               perror ("Fork failed");
               exit(errno);
            }
            if (!pid) {
               error = execvp(confpr->valor(CONF_EDITOR).c_str(),argstxt);
            }
         }
         if (html) {
            fitxersortidahtml << "\n</table>\n</body>\n</html>\n";
            fitxersortidahtml.close();
            //presentació html normal
            if ((pid=fork()) < 0) {
               perror ("Fork failed");
               exit(errno);
            }
            if (!pid) {
               error = execvp(confpr->valor(CONF_NAVEGADOR).c_str(),argshtml);
            }
         }
#endif
      }
   }
#endif
}

