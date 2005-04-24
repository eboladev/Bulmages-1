/***************************************************************************
                          balanceprintview.cpp  -  description
                             -------------------
    begin                : mi�jun 25 2003
    copyright            : (C) 2003 by Tomeu Borr� Riera
    email                : tborras@conetxia.com
    modificat per        : (C) 2003 Antoni Mirabete i Ter� - amirabet@biada.org
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "balanceprintview.h"
#include "listcuentasview1.h"


#include <qwidget.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qdatetimeedit.h>
#include <qtable.h>
#include <qtoolbutton.h>
#include <qcheckbox.h>
#include <qlocale.h>

#include "empresa.h"

#ifndef WIN32
#include <unistd.h>
#endif



BalancePrintView::BalancePrintView(empresa *emp,QWidget *parent, const char *name ) : BalancePrintDlg(parent,name) {
   empresaactual=emp;
   conexionbase = emp->bdempresa();
 // Inicializamos la tabla de nivel
   combonivel->insertItem("2",0);
   combonivel->insertItem("3",1);
   combonivel->insertItem("4",2);
   combonivel->insertItem("5",3);
   combonivel->insertItem("6",4);
   combonivel->insertItem("7",5);
}

BalancePrintView::~BalancePrintView(){
}

/** \brief inicializa la clase para que tenga parametros por defecto (no es obligatorio)
  * codinicial, codfinal, finicial, ffinal, arbol)
  * codinicial indica el c�igo inicial para mostrar el balance
  * cofinal indica el codigo de cuenta para no mostrar m� balances
  * finicial indica la fecha a partir de la cual comenzar los saldos
  * ffinal indica la fecha para terminar de contar los saldos
  * arbol indica si hay que representar el balance en forma de �bol o no.
  */
void BalancePrintView::inicializa1(QString codinicial1, QString codfinal1, QString finicial1, QString ffinal1, bool arbol) {
   fechainicial1->setText(finicial1);
   fechafinal1->setText(ffinal1);
   codigoinicial->setText(codinicial1);
   codigofinal->setText(codfinal1);
   checksuperiores->setChecked(arbol);
}// end if


/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/
void BalancePrintView::accept() {
      if (radiotexto->isChecked()) presentar("txt");
      if(radiohtml->isChecked()) presentar("html");
      if (radiopropietario->isChecked()) presentar("rtk");
      if (radiokugar->isChecked()) presentar("kugar");      
}// end accept


void BalancePrintView::boton_codigoinicial() {
   listcuentasview1 *listcuentas = new listcuentasview1(empresaactual);
   listcuentas->modo=1;
   listcuentas->inicializa();
   listcuentas->exec();
   codigoinicial->setText(listcuentas->codcuenta);
   delete listcuentas;
}

void BalancePrintView::boton_codigofinal() {
   listcuentasview1 *listcuentas = new listcuentasview1(empresaactual);
   listcuentas->modo=1;
   listcuentas->inicializa();
   listcuentas->exec();
   codigofinal->setText(listcuentas->codcuenta);
   delete listcuentas;
}


/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/
void BalancePrintView::presentar(char *tipus){
#ifndef WIN32
   int kugar, txt, html, txtapren, htmlapren;
   int error;
   int pid;

   int num1;
   double tsaldoant=0, tdebe=0, thaber=0, tsaldo=0, debeej=0, haberej=0, saldoej=0;
   QString query;
   cursor2 *cursorapt;

// tipos de presentaciones   rtk=!strcmp(tipus,"rtk");
   txt=!strcmp(tipus,"txt");
   html=!strcmp(tipus,"html");
   txtapren=!strcmp(tipus,"txtapren");
   htmlapren=!strcmp(tipus,"htmlapren");
   kugar=!strcmp(tipus,"kugar");

// Cogemos los valores del formulario.
   QString finicial = fechainicial1->text();
   QString ffinal = fechafinal1->text();
   QString cinicial = codigoinicial->text();
   QString cfinal = codigofinal->text();
   int nivel = combonivel->currentItem()+2;

   if (txt | html | kugar) {

      char *argstxt[]={"balance.txt","balance.txt",NULL}; //presentación txt normal
      char *argshtml[]={"balance.html","balance.html",NULL}; //presentación html normal
      char *argskugar[]={"balance.kud","balance.kud",NULL}; // presentación kugar normal
      
      ofstream fitxersortidakugar(argskugar[0]);   // creamos los ficheros de salida
      ofstream fitxersortidatxt(argstxt[0]);  
      ofstream fitxersortidahtml(argshtml[0]);

      if (!fitxersortidatxt) txt=0;    // verifiquem que s'hagin creat correctament els fitxers
      if (!fitxersortidahtml) html=0;  // es pot millorar el tractament d'errors
      if (!fitxersortidakugar) kugar=0;
      
      QLocale spanish(QLocale::Spanish); // vamos a formatear los números con punto para los millares y coma para los decimales

      if (txt | html | kugar) {                // nom� continuem si hem pogut crear algun fitxer


         // Hacemos la consulta de los apuntes a listar en la base de datos.
         // La consulta es compleja, requiere la creaci� de una tabla temporal y de cierta mandanga por lo que puede
         // Causar problemas con el motor de base de datos.
         fprintf(stderr,"BALANCE: Empezamos a hacer la presentacion\n");
         conexionbase->begin();
         //query.sprintf( "CREATE TEMPORARY TABLE balancetmp AS SELECT 0 AS hoja, cuenta.idcuenta AS idcuenta, codigo AS codigo, nivel(codigo) AS nivel, cuenta.descripcion AS descripcion, padre AS padre, tipocuenta ,debe, haber, tdebe, thaber,(tdebe-thaber) AS tsaldo, (debe-haber) AS saldo, adebe, ahaber, (adebe-ahaber) AS asaldo FROM cuenta LEFT JOIN (SELECT idcuenta, sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE fecha >= '%s' AND fecha<= '%s' GROUP BY idcuenta) AS t1 ON t1.idcuenta = cuenta.idcuenta LEFT JOIN (SELECT idcuenta, sum(debe) AS adebe, sum(haber) AS ahaber FROM apunte WHERE fecha < '%s' GROUP BY idcuenta) AS t2 ON t2.idcuenta = cuenta.idcuenta", finicial.ascii(), ffinal.ascii(), finicial.ascii() );
	 query.sprintf("CREATE TEMPORARY TABLE balancetmp AS SELECT 0 AS hoja, cuenta.idcuenta AS idcuenta, codigo, nivel(codigo) AS nivel, cuenta.descripcion AS descripcion, padre AS padre, tipocuenta, debeej, haberej, tdebe, thaber,(tdebe-thaber) AS tsaldo, (debeej-haberej) AS saldo, adebe, ahaber, (adebe-ahaber) AS asaldo FROM cuenta LEFT JOIN (SELECT idcuenta, sum(debe) AS debeej, sum(haber) AS haberej FROM apunte NATURAL JOIN (SELECT idcuenta, idapunte, debe, haber FROM (SELECT idcuenta, idapunte, debe, haber, EXTRACT(year FROM fecha) AS fecha1, EXTRACT(year FROM TIMESTAMP '%s') AS fecha2 FROM apunte) AS fechas WHERE fecha1=fecha2 UNION SELECT idcuenta, idapunte, debe, haber FROM (SELECT idcuenta, idapunte, debe, haber, EXTRACT(year FROM fecha) AS fecha1, EXTRACT(year FROM TIMESTAMP '%s') AS fecha2 FROM apunte) AS fechas WHERE fecha1=fecha2) AS apuntesej GROUP BY idcuenta) AS t1 ON t1.idcuenta=cuenta.idcuenta LEFT JOIN (SELECT idcuenta, sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE fecha >= '%s' AND fecha<= '%s' GROUP BY idcuenta) AS t2 ON t2.idcuenta = cuenta.idcuenta LEFT JOIN (SELECT idcuenta, sum(debe) AS adebe, sum(haber) AS ahaber FROM apunte WHERE fecha < '%s' GROUP BY idcuenta) AS t3 ON t3.idcuenta = cuenta.idcuenta", finicial.ascii(), ffinal.ascii(), finicial.ascii(), ffinal.ascii(), finicial.ascii());
         fprintf(stderr,"%s\n",query.ascii());
         conexionbase->ejecuta(query);
         query.sprintf("UPDATE balancetmp SET padre=0 WHERE padre ISNULL");
         fprintf(stderr,"%s\n",query.ascii());
         conexionbase->ejecuta(query);
         query.sprintf("DELETE FROM balancetmp WHERE debeej=0 AND haberej=0");
         fprintf(stderr,"%s\n",query.ascii());
         conexionbase->ejecuta(query);

         // Para evitar problemas con los nulls hacemos algunos updates
         query.sprintf("UPDATE balancetmp SET tsaldo=0 WHERE tsaldo ISNULL");
         fprintf(stderr,"%s\n",query.ascii());
         conexionbase->ejecuta(query);
         query.sprintf("UPDATE balancetmp SET tdebe=0 WHERE tdebe ISNULL");
         fprintf(stderr,"%s\n",query.ascii());
         conexionbase->ejecuta(query);
         query.sprintf("UPDATE balancetmp SET thaber=0 WHERE thaber ISNULL");
         fprintf(stderr,"%s\n",query.ascii());
         conexionbase->ejecuta(query);
         query.sprintf("UPDATE balancetmp SET asaldo=0 WHERE asaldo ISNULL");
         fprintf(stderr,"%s\n",query.ascii());
         conexionbase->ejecuta(query);

         query.sprintf( "SELECT idcuenta FROM balancetmp ORDER BY padre DESC");
         fprintf(stderr,"%s\n",query.ascii());
         cursorapt = conexionbase->cargacursor(query,"Balance1view");
         while (!cursorapt->eof())  {
            query.sprintf("SELECT * FROM balancetmp WHERE idcuenta=%s",cursorapt->valor("idcuenta").ascii());
            cursor2 *mycur = conexionbase->cargacursor(query,"cursorrefresco");
            query.sprintf("UPDATE balancetmp SET tsaldo = tsaldo + (%2.2f), tdebe = tdebe + (%2.2f), thaber = thaber +(%2.2f), asaldo= asaldo+(%2.2f) WHERE idcuenta = %d",atof(mycur->valor("tsaldo").ascii()), atof(mycur->valor("tdebe").ascii()), atof(mycur->valor("thaber").ascii()),atof(mycur->valor("asaldo").ascii()),  atoi(mycur->valor("padre").ascii()));
            //fprintf(stderr,"%s para el código\n",query, cursorapt->valor("codigo").c_str());
            conexionbase->ejecuta(query);
            delete mycur;
            cursorapt->siguienteregistro();
         }// end while
         delete cursorapt;
         // Borramos todo lo que no es de este nivel

         // Borramos los niveles inferiores
         query.sprintf("DELETE FROM balancetmp where nivel(codigo)>%d",nivel);
         conexionbase->ejecuta(query);

         // No hay que incluir los niveles superiores.
         if (!checksuperiores->isChecked()) {
            //Borramos todo lo que tiene un hijo en el balancetmp
            query.sprintf("DELETE FROM balancetmp WHERE idcuenta IN (SELECT padre FROM balancetmp)");
            conexionbase->ejecuta(query);
         } else {
            // Si hay que incluir los niveles superiores
            query.sprintf("UPDATE balancetmp SET hoja=1 WHERE idcuenta NOT IN (SELECT padre FROM balancetmp)");
            conexionbase->ejecuta(query);
         }

         // Hacemos la consulta.
         query.sprintf("SELECT * FROM balancetmp WHERE debeej <> 0  OR haberej <> 0 ORDER BY codigo");
         cursorapt = conexionbase->cargacursor(query,"mycursor");
         // Calculamos cuantos registros van a crearse y dimensionamos la tabla.
         num1 = cursorapt->numregistros();

         if (kugar) {
		 fitxersortidakugar.setf(ios::fixed);
		 fitxersortidakugar.precision(2);
		 fitxersortidakugar << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" ;
		 fitxersortidakugar << "<!DOCTYPE KugarData [\n" ;
		 fitxersortidakugar << "\t<!ELEMENT KugarData (Row* )>\n" ;
		 fitxersortidakugar << "\t\t<!ATTLIST KugarData\n";
		 fitxersortidakugar << "\t\tTemplate CDATA #REQUIRED>\n";
		 fitxersortidakugar << "\t<!ELEMENT Row EMPTY>\n";
		 fitxersortidakugar << "\t<!ATTLIST Row \n";
		 fitxersortidakugar << "\t\tlevel CDATA #REQUIRED\n";
		 fitxersortidakugar << "\t\tfinicial CDATA #IMPLIED\n";
		 fitxersortidakugar << "\t\tffinal CDATA #IMPLIED\n";
		 fitxersortidakugar << "\t\tcuenta CDATA #IMPLIED\n";
		 fitxersortidakugar << "\t\tdescripcion CDATA #IMPLIED\n";
		 fitxersortidakugar << "\t\tsaldoant CDATA #IMPLIED\n";
		 fitxersortidakugar << "\t\tdebe CDATA #IMPLIED\n";
		 fitxersortidakugar << "\t\thaber CDATA #IMPLIED\n";
		 fitxersortidakugar << "\t\tsaldo CDATA #IMPLIED\n";
		 fitxersortidakugar << "\t\tdebeej CDATA #IMPLIED\n";
		 fitxersortidakugar << "\t\thaberej CDATA #IMPLIED\n";
		 fitxersortidakugar << "\t\tsaldoej CDATA #IMPLIED>\n";		 
		 fitxersortidakugar << "]>\n\n";
		 fitxersortidakugar << "<KugarData Template=\"" << confpr->valor(CONF_DIR_KUGAR).ascii()<<"balance.kut\">\n";
		 fitxersortidakugar << "\t<Row";
		 fitxersortidakugar << " level=\"0\"";
		 fitxersortidakugar << " finicial='"<< finicial.ascii() <<"'";		
		 fitxersortidakugar << " ffinal='"<< ffinal.ascii() <<"'/>\n";
	 }// end if

                  
         if (txt) {
            //presentaci�txt normal
            fitxersortidatxt.setf(ios::fixed);
            fitxersortidatxt.precision(2);
            fitxersortidatxt << "                                        Balance \n" ;
            fitxersortidatxt << "Fecha Inicial: " << finicial.ascii() << "   Fecha Final: " << ffinal.ascii() << endl;
            fitxersortidatxt << "Cuenta            Denominaci�                        Saldo ant.         Debe        Haber        Saldo     Debe ej.    Haber ej.    Saldo ej.\n" ;
            fitxersortidatxt << "______________________________________________________________________________________________________________________________________________\n";
         }// end if
         if (html) {
            //presentaci�html normal
            fitxersortidahtml.setf(ios::fixed);
            fitxersortidahtml.precision(2);
            fitxersortidahtml << "<html>\n";
            fitxersortidahtml << "<head>\n";
            fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
            fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
            fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
            fitxersortidahtml << "  <title> Balan�</title>\n";
            fitxersortidahtml << "</head>\n";
            fitxersortidahtml << "<body>\n";
            fitxersortidahtml << "<table><tr><td colspan=\"9\" class=titolbalanc> Balan�<hr></td></tr>\n\n";
            fitxersortidahtml << "<tr><td colspan=\"9\" class=periodebalanc> Data Inicial: " << finicial.ascii() << " -  Data Final: " << ffinal.ascii() << "<hr></td></tr>\n\n";
            fitxersortidahtml << "<tr><td class=titolcolumnabalanc>lcuenta</td><td class=titolcolumnabalanc> ldenominacion</td><td class=titolcolumnabalanc>lsaldoant</td><td class=titolcolumnabalanc>ldebe</td><td class=titolcolumnabalanc>lhaber</td><td class=titolcolumnabalanc>lsaldo</td><td class=titolcolumnabalanc> ldebeej  </td><td class=titolcolumnabalanc> lhaberej </td><td class=titolcolumnabalanc> lsaldoej </td></tr>\n";
         }// end if

	 float linea = 1; // irá contando las líneas impresas en el impreso de kugar
	 float lineaskugar = 51; // determina cuántas líneas caben en el impreso de kugar
         while (!cursorapt->eof()) {
	    QString lcuenta = cursorapt->valor("codigo");
            QString ldenominacion = cursorapt->valor("descripcion").left(40);
	    QString lsaldoant = cursorapt->valor("asaldo");
            QString ldebe = cursorapt->valor("tdebe");
            QString lhaber = cursorapt->valor("thaber");
            QString lsaldo = cursorapt->valor("tsaldo");
            QString ldebeej = cursorapt->valor("debeej");
            QString lhaberej = cursorapt->valor("haberej");
            QString lsaldoej = cursorapt->valor("saldo");
            
	    // Acumulamos los totales para al final poder escribirlos
            tsaldoant += atof(lsaldoant.ascii());
            tsaldo += atof(lsaldo.ascii());
            tdebe += atof(ldebe.ascii());
            thaber += atof(lhaber.ascii());
	    debeej += atof(ldebeej.ascii());
	    haberej += atof(lhaberej.ascii());
	    saldoej += atof(lsaldoej.ascii());

	    // Las variables de las filas en formato español
	    lsaldoant = spanish.toString(atof(lsaldoant.ascii()),'f',2);
	    ldebe = spanish.toString(atof(ldebe.ascii()),'f',2);
	    lhaber = spanish.toString(atof(lhaber.ascii()),'f',2);
	    lsaldo = spanish.toString(atof(lsaldo.ascii()),'f',2);
	    ldebeej = spanish.toString(atof(ldebeej.ascii()),'f',2);
	    lhaberej = spanish.toString(atof(lhaberej.ascii()),'f',2);
	    lsaldoej = spanish.toString(atof(lsaldoej.ascii()),'f',2);

	    if (txt) {
               //presentaci�txt normal
               fitxersortidatxt << setiosflags(ios::left) << setw(10) <<  lcuenta.ascii() << " " << setw(40) <<  ldenominacion.ascii() << " " << resetiosflags(ios::left) << setw(12) <<  lsaldoant.ascii() << " " << setw(12) <<  ldebe.ascii() << " " << setw(12) <<  lhaber.ascii() << " " << setw(12) <<  lsaldo << " " << setw(12) <<  ldebeej.ascii() << " " << setw(12) <<  lhaberej.ascii() << " " << setw(12) <<  lsaldoej.ascii() << " " << setw(12) <<  endl;
            }// end if
	    
            if (html) {
               //presentaci�html normal
               fitxersortidahtml << "<tr><td class=comptebalanc>" << lcuenta.ascii() << "</td><td class=assentamentbalanc>" <<  ldenominacion.ascii() << "</td><td class=dosdecimals>" << lsaldoant.ascii() << "</td><td class=dosdecimals>" << ldebe.ascii() << "</td><td class=dosdecimals>" << lhaber.ascii() << "</td><td class=dosdecimals>" << lsaldo.ascii() << "</td><td class=dosdecimals>" << ldebeej.ascii() << "</td><td class=dosdecimals>" << lhaberej.ascii() << "</td><td class=dosdecimals>" << lsaldoej.ascii() << endl;
            }// end if
	    
	    if (kugar){
		//presentación en kugar
		// primero vamos a establecer si hay que imprimir una línea de cabecera (Detail 0 en balance.kut) con los datos del período
		if(fmod(linea,lineaskugar) == 0){
		    fitxersortidakugar << "\t<Row";
		    fitxersortidakugar << " level=\"0\"";
		    fitxersortidakugar << " finicial='"<< finicial.ascii() <<"'";		
		    fitxersortidakugar << " ffinal='"<< ffinal.ascii() <<"'/>\n";
		}// end if
		if(linea == lineaskugar){
		    lineaskugar = 54; // a partir de la segunda página caben más líneas
		    linea = lineaskugar;
		}
		// ahora, imprimimos tantas líneas como nos permite la variable "lineaskugar" sin imprimir cabecera (Detail 1 en balance.kut)
		ldenominacion = cursorapt->valor("descripcion").left(47); // aquí nos cabe una descripción algo más amplia	
		fitxersortidakugar << "\t<Row";
		fitxersortidakugar << " level=\"1\"";
		fitxersortidakugar << " cuenta='"<< lcuenta.ascii() <<"'";		
		fitxersortidakugar << " descripcion='"<< ldenominacion.ascii() <<"'";
		fitxersortidakugar << " saldoant='"<< lsaldoant.ascii() <<"'";
		fitxersortidakugar << " debe='"<< ldebe.ascii() <<"'";	    
		fitxersortidakugar << " haber='"<< lhaber.ascii() <<"'";
		fitxersortidakugar << " saldo='"<< lsaldo.ascii() <<"'";
		fitxersortidakugar << " debeej='"<< ldebeej.ascii() <<"'";	    
		fitxersortidakugar << " haberej='"<< lhaberej.ascii() <<"'";		
		fitxersortidakugar << " saldoej='"<< lsaldoej.ascii() <<"'/>\n";
		linea++;
	    }// end if

            // Calculamos la siguiente cuenta registro y finalizamos el bucle
            cursorapt->siguienteregistro();
         }// end while

         // Vaciamos el cursor de la base de datos.
         delete cursorapt;

         // Borramos la tabla temporal y demás cosas.
         query.sprintf("DROP TABLE balancetmp");
         conexionbase->ejecuta(query);
         conexionbase->commit();

         // Hacemos la actualizacion de los saldos totales en formato español
         QString totalsaldoant = spanish.toString(tsaldoant,'f',2);
         QString totaldebe = spanish.toString(tdebe,'f',2);
         QString totalhaber = spanish.toString(thaber,'f',2);
         QString totalsaldo = spanish.toString(tsaldo,'f',2);
	 QString totaldebeej = spanish.toString(debeej,'f',2);
         QString totalhaberej = spanish.toString(haberej,'f',2);
         QString totalsaldoej = spanish.toString(saldoej,'f',2);
	    	 
         if (txt) {
            //presentación txt normal
	    fitxersortidatxt << "                                            __________________________________________________________________________________________________\n";
            fitxersortidatxt << "                                            Totales " << setw(12) <<  totalsaldoant.ascii() << " " << setw(12) <<  totaldebe.ascii() << " " << setw(12) <<  totalhaber.ascii() << " " << setw(12) <<  totalsaldo.ascii()  << " " << setw(12) <<  totaldebeej.ascii() << " " << setw(12) <<  totalhaberej.ascii() << " " << setw(12) <<  totalsaldoej.ascii() << endl;
         }
         if (html) {
            //presentación html normal
            fitxersortidahtml << "<tr><td></td><td class=totalbalanc>Totals</td><td class=dosdecimals>" <<  totalsaldoant.ascii() << "</td><td class=dosdecimals>" << totaldebe.ascii() << "</td><td class=dosdecimals>" << totalhaber.ascii() << "</td><td class=dosdecimals>" << totalsaldo.ascii() << "</td></tr>\n</table>\n</body>\n</html>\n";
         }
	 if (kugar){
            //presentación kugar normal
	    fitxersortidakugar << "\t<Row";
	    fitxersortidakugar << " level=\"2\"";
	    fitxersortidakugar << " tsaldoant='"<< totalsaldoant.ascii() <<"'";		
	    fitxersortidakugar << " tdebe='"<< totaldebe.ascii() <<"'";
	    fitxersortidakugar << " thaber='"<< totalhaber.ascii() <<"'";
	    fitxersortidakugar << " tsaldo='"<< totalsaldo.ascii() <<"'";	    
	    fitxersortidakugar << " tdebeej='"<< totaldebeej.ascii() <<"'";
	    fitxersortidakugar << " thaberej='"<< totalhaberej.ascii() <<"'";
	    fitxersortidakugar << " tsaldoej='"<< totalsaldoej.ascii() <<"'/>\n";	    
	    fitxersortidakugar <<"</KugarData>\n";
	    fitxersortidakugar.close();
	 }

         if (txt) {
            fitxersortidatxt.close();
            //presentación txt normal
            if ((pid=fork()) < 0) {
               perror ("Fork failed");
               exit(errno);
            }
            if (!pid) {
               error = execvp(confpr->valor(CONF_EDITOR).ascii(),argstxt);
            }
         }
         if (html) {
            fitxersortidahtml.close();
            //presentación html normal
            if ((pid=fork()) < 0) {
               perror ("Fork failed");
               exit(errno);
            }
            if (!pid) {
               error = execvp(confpr->valor(CONF_NAVEGADOR).ascii(),argshtml);
            }
         }
	 if (kugar){
            //presentación kugar normal
	    if ((pid=fork()) < 0) {
               perror ("Fork failed");
               exit(errno);
            }
            if (!pid) {
               error = execvp("kugar",argskugar);
            }
	 }
      }
   }
#endif
}

/** \brief SLOT que responde a la pulsaci� del bot� de selecci� de canal.
  * Presenta la ventana de selecci� de canales \ref selectcanalview
  */
void BalancePrintView::boton_canales() {
    selectcanalview *selcanales = empresaactual->getselcanales();
    selcanales->exec();
    selcanales->firstcanal();
}// end boton_canales


/** \brief SLOT que responde a la pulsaci� del bot� de selecci� de centros de coste.
  * Presenta la ventana de selecci� de centros de coste \ref selectccosteview
  */
void BalancePrintView::boton_ccostes() {
    selectccosteview *selccostes = empresaactual->getselccostes();
    selccostes->exec();
    selccostes->firstccoste();
}// end boton_ccostes

