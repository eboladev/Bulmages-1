/***************************************************************************
                          balanceprintview.cpp  -  description
                             -------------------
    begin                : mié jun 25 2003
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

#include "balanceprintview.h"
#include "listcuentasview1.h"


#include <qwidget.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qdatetimeedit.h>
// #include <qfiledialog.h>

#include <qtable.h>
#include <qtoolbutton.h>

#ifndef WIN32
#include <unistd.h>
#endif

BalancePrintView::BalancePrintView(QWidget *parent, const char *name ) : BalancePrintDlg(parent,name) {
	fichero = NULL;

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

int BalancePrintView::inicializa(postgresiface2 *conn ) {
    conexionbase = conn;
    return(0);
}// end inicializa


void BalancePrintView::inicializa1(QString codinicial1, QString codfinal1, QString finicial1, QString ffinal1, bool arbol) {
	   fechainicial1->setText(finicial1);
      fechafinal1->setText(ffinal1);
      codigoinicial->setText(codinicial1);
      codigofinal->setText(codfinal1);
		checksuperiores->setChecked(arbol);
}// end if


void BalancePrintView::inicializa2(char *fich) {
   fichero = fich;
}// end inicializa2


void BalancePrintView::cargacostes() {
   // Hacemos la carga de los centros de coste. Rellenamos el combobox correspondiente.
   combocoste->clear();
   QString query="SELECT * FROM c_coste ORDER BY nombre";
   conexionbase->begin();
   cursor2 *cursorcoste = conexionbase->cargacursor(query,"costes");
   conexionbase->commit();
   combocoste->insertItem("--",0);
   ccostes[0]=0;
   int i=1;
   while (!cursorcoste->eof()) {
      combocoste->insertItem(cursorcoste->valor(2),-1);
      ccostes[i++] = atoi(cursorcoste->valor(0).ascii());
      cursorcoste->siguienteregistro();
   }// end while
   delete cursorcoste;
}// end cargacostes

/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/
void BalancePrintView::accept() {

// Versió per si només permetem escollir una opció

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






void BalancePrintView::boton_codigoinicial() {
   listcuentasview1 *listcuentas = new listcuentasview1();
   listcuentas->modo=1;
   listcuentas->inicializa(conexionbase);
   listcuentas->exec();
   codigoinicial->setText(listcuentas->codcuenta);
   delete listcuentas;
}

void BalancePrintView::boton_codigofinal() {
   listcuentasview1 *listcuentas = new listcuentasview1();
   listcuentas->modo=1;
   listcuentas->inicializa(conexionbase);
   listcuentas->exec();
   codigofinal->setText(listcuentas->codcuenta);
   delete listcuentas;
}


/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/
void BalancePrintView::presentar(char *tipus){
#ifndef WIN32
   int txt, html, txtapren, htmlapren;
   int error;
   int pid;

   int num1;
   double tsaldoant=0, tdebe=0, thaber=0, tsaldo=0;
   QString query;
   cursor2 *cursorapt;

// tipus de presentació
   txt=!strcmp(tipus,"txt");
   html=!strcmp(tipus,"html");
   txtapren=!strcmp(tipus,"txtapren");
   htmlapren=!strcmp(tipus,"htmlapren");

// Cogemos los valores del formulario.
   QString finicial = fechainicial1->text();
   QString ffinal = fechafinal1->text();
   QString cinicial = codigoinicial->text();
   QString cfinal = codigofinal->text();

   if (txt | html) {

      char *argstxt[]={"balanç.txt","balanç.txt",NULL};      //presentació txt normal
      char *argshtml[]={"balanç.html","balanç.html",NULL};   //presentació html normal

      ofstream fitxersortidatxt(argstxt[0]);     // creem els fitxers de sordida
      ofstream fitxersortidahtml(argshtml[0]);

      if (!fitxersortidatxt) txt=0;    // verifiquem que s'hagin creat correctament els fitxers
      if (!fitxersortidahtml) html=0;  // es pot millorar el tractament d'errors

      if (txt | html) {                // només continuem si hem pogut crear algun fitxer


         // Hacemos la consulta de los apuntes a listar en la base de datos.
         int idc_coste;
         idc_coste = ccostes[combocoste->currentItem()];

         // La consulta es compleja, requiere la creación de una tabla temporal y de cierta mandanga por lo que puede
         // Causar problemas con el motor de base de datos.
         fprintf(stderr,"BALANCE: Empezamos a hacer la presentacion\n");
         conexionbase->begin();
         query. sprintf( "CREATE TEMPORARY TABLE balance AS SELECT 0 AS hoja, cuenta.idcuenta, codigo, nivel(codigo) AS nivel, cuenta.descripcion, padre, tipocuenta ,debe, haber, tdebe, thaber,(tdebe-thaber) AS tsaldo, (debe-haber) AS saldo, adebe, ahaber, (adebe-ahaber) AS asaldo FROM cuenta LEFT JOIN (SELECT idcuenta, sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE fecha >= '%s' AND fecha<= '%s' GROUP BY idcuenta) AS t1 ON t1.idcuenta = cuenta.idcuenta LEFT JOIN (SELECT idcuenta, sum(debe) AS adebe, sum(haber) AS ahaber FROM apunte WHERE fecha < '%s' GROUP BY idcuenta) AS t2 ON t2.idcuenta = cuenta.idcuenta", finicial.ascii(), ffinal.ascii(), finicial.ascii() );
         fprintf(stderr,"%s\n",query.ascii());
         conexionbase->ejecuta(query);
         query.sprintf("UPDATE BALANCE SET padre=0 WHERE padre ISNULL");
         fprintf(stderr,"%s\n",query.ascii());
         conexionbase->ejecuta(query);
         query.sprintf("DELETE FROM balance WHERE debe=0 AND haber =0");
         fprintf(stderr,"%s\n",query.ascii());
         conexionbase->ejecuta(query);

         // Para evitar problemas con los nulls hacemos algunos updates
         query.sprintf("UPDATE BALANCE SET tsaldo=0 WHERE tsaldo ISNULL");
         fprintf(stderr,"%s\n",query.ascii());
         conexionbase->ejecuta(query);
         query.sprintf("UPDATE BALANCE SET tdebe=0 WHERE tdebe ISNULL");
         fprintf(stderr,"%s\n",query.ascii());
         conexionbase->ejecuta(query);
         query.sprintf("UPDATE BALANCE SET thaber=0 WHERE thaber ISNULL");
         fprintf(stderr,"%s\n",query.ascii());
         conexionbase->ejecuta(query);
         query.sprintf("UPDATE BALANCE SET asaldo=0 WHERE asaldo ISNULL");
         fprintf(stderr,"%s\n",query.ascii());
         conexionbase->ejecuta(query);

         query.sprintf( "SELECT idcuenta FROM balance ORDER BY padre DESC");
         fprintf(stderr,"%s\n",query.ascii());
         cursorapt = conexionbase->cargacursor(query,"Balance1view");
         while (!cursorapt->eof())  {
            query.sprintf("SELECT * FROM balance WHERE idcuenta=%s",cursorapt->valor("idcuenta").ascii());
            cursor2 *mycur = conexionbase->cargacursor(query,"cursorrefresco");
            query.sprintf("UPDATE balance SET tsaldo = tsaldo + (%2.2f), tdebe = tdebe + (%2.2f), thaber = thaber +(%2.2f), asaldo= asaldo+(%2.2f) WHERE idcuenta = %d",atof(mycur->valor("tsaldo").ascii()), atof(mycur->valor("tdebe").ascii()), atof(mycur->valor("thaber").ascii()),atof(mycur->valor("asaldo").ascii()),  atoi(mycur->valor("padre").ascii()));
            //			fprintf(stderr,"%s para el código\n",query, cursorapt->valor("codigo").c_str());
            conexionbase->ejecuta(query);
            delete mycur;
            cursorapt->siguienteregistro();
         }
         delete cursorapt;
         // Borramos todo lo que no es de este nivel

         // Borramos Los niveles inferiores
         query.sprintf(query,"DELETE FROM balance where nivel(codigo)>%s",combonivel->text(combonivel->currentItem()).ascii());
         conexionbase->ejecuta(query);

         // No hay que incluir los niveles superiores.
         if (!checksuperiores->isChecked()) {
            //Borramos todo lo que tiene un hijo en el balance
            query.sprintf("DELETE FROM balance WHERE idcuenta IN (SELECT padre FROM balance)");
            conexionbase->ejecuta(query);
         } else {
            // Si hay que incluir los niveles superiores
            query.sprintf("UPDATE balance SET hoja=1 WHERE idcuenta NOT IN (SELECT padre FROM balance)");
            conexionbase->ejecuta(query);
         }

         query.sprintf("SELECT * FROM balance WHERE debe <> 0  OR haber <> 0 ORDER BY codigo");
         fprintf(stderr,"%s\n",query.ascii());
         cursorapt = conexionbase->cargacursor(query,"mycursor");
         // Calculamos cuantos registros van a crearse y dimensionamos la tabla.
         num1 = cursorapt->numregistros();

         if (txt) {
            //presentació txt normal

            fitxersortidatxt.setf(ios::fixed);
            fitxersortidatxt.precision(2);

            fitxersortidatxt << "                                        Balanç \n" ;
            fitxersortidatxt << "Data Inicial: " << finicial.ascii() << "   Data Final: " << ffinal.ascii() << endl;
            fitxersortidatxt << "lcuenta           ldenominacion                      lsaldoant      ldebe     lhaber     lsaldo    ldebeej    haberej   lsaldoej\n" ;
            fitxersortidatxt << "________________________________________________________________________________________________________________________________\n";
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
            fitxersortidahtml << "  <title> Balanç </title>\n";
            fitxersortidahtml << "</head>\n";
            fitxersortidahtml << "<body>\n";
            fitxersortidahtml << "<table><tr><td colspan=\"9\" class=titolbalanc> Balanç <hr></td></tr>\n\n";
            fitxersortidahtml << "<tr><td colspan=\"9\" class=periodebalanc> Data Inicial: " << finicial.ascii() << " -  Data Final: " << ffinal.ascii() << "<hr></td></tr>\n\n";
            fitxersortidahtml << "<tr><td class=titolcolumnabalanc>lcuenta</td><td class=titolcolumnabalanc> ldenominacion</td><td class=titolcolumnabalanc>lsaldoant</td><td class=titolcolumnabalanc>ldebe</td><td class=titolcolumnabalanc>lhaber</td><td class=titolcolumnabalanc>lsaldo</td><td class=titolcolumnabalanc> ldebeej  </td><td class=titolcolumnabalanc> lhaberej </td><td class=titolcolumnabalanc> lsaldoej </td></tr>\n";
         }





         while (!cursorapt->eof()) {
            // Acumulamos los totales para al final poder escribirlos
            tsaldoant += atof(cursorapt->valor("asaldo").ascii());
            tsaldo += atof(cursorapt->valor("tsaldo").ascii());
            tdebe += atof(cursorapt->valor("tdebe").ascii());
            thaber += atof(cursorapt->valor("thaber").ascii());


            QString lcuenta = cursorapt->valor("codigo");
            QString ldenominacion = cursorapt->valor("descripcion");
            double lsaldoant = atof(cursorapt->valor("asaldo").ascii());
            double ldebe = atof(cursorapt->valor("tdebe").ascii());
            double lhaber = atof(cursorapt->valor("thaber").ascii());
            double lsaldo = atof(cursorapt->valor("tsaldo").ascii());
            double ldebeej = atof(cursorapt->valor("debe").ascii());
            double lhaberej = atof(cursorapt->valor("haber").ascii());
            double lsaldoej = atof(cursorapt->valor("saldo").ascii());
            ldenominacion = ldenominacion.left(40);

	    if (txt) {
               //presentació txt normal
               fitxersortidatxt << setiosflags(ios::left) << setw(10) <<  lcuenta.ascii() << " " << setw(40) <<  ldenominacion.ascii() << " " << resetiosflags(ios::left) << setw(10) <<  lsaldoant << " " << setw(10) <<  ldebe << " " << setw(10) <<  lhaber << " " << setw(10) <<  lsaldo << " " << setw(10) <<  ldebeej << " " << setw(10) <<  lhaberej << " " << setw(10) <<  lsaldoej << " " << setw(10) <<  endl;
            }
            if (html) {
               //presentació html normal
               fitxersortidahtml << "<tr><td class=comptebalanc>" << lcuenta.ascii() << "</td><td class=assentamentbalanc>" <<  ldenominacion.ascii() << "</td><td class=dosdecimals>" << lsaldoant << "</td><td class=dosdecimals>" << ldebe << "</td><td class=dosdecimals>" << lhaber << "</td><td class=dosdecimals>" << lsaldo << "</td><td class=dosdecimals>" << ldebeej << "</td><td class=dosdecimals>" << lhaberej << "</td><td class=dosdecimals>" << lsaldoej << endl;
            }

            // Calculamos la siguiente cuenta registro y finalizamos el bucle
            cursorapt->siguienteregistro();
         }

         // Vaciamos el cursor de la base de datos.
         delete cursorapt;

         // Borramos la tabla temporal y demás cosas.
         query.sprintf("DROP TABLE balance");
         fprintf(stderr,"%s\n",query.ascii());
         conexionbase->ejecuta(query);
         conexionbase->commit();
         
         
         // Hacemos la actualizacion de los saldos totales
         QString totalsaldoant = QString::number(tsaldoant,'f',2);
         QString totaldebe = QString::number(tdebe,'f',2);
         QString totalhaber = QString::number(thaber,'f',2);
         QString totalsaldo = QString::number(tsaldo,'f',2);


         if (txt) {
            //presentació txt normal
            fitxersortidatxt << "                                             Totals " << setw(10) <<  totalsaldoant.ascii() << " " << setw(10) <<  totaldebe.ascii() << " " << setw(10) <<  totalhaber.ascii() << " " << setw(10) <<  totalsaldo.ascii() << endl;
         }
         if (html) {
            //presentació html normal
            fitxersortidahtml << "<tr><td></td><td class=totalbalanc>Totals</td><td class=dosdecimals>" <<  totalsaldoant.ascii() << "</td><td class=dosdecimals>" << totaldebe.ascii() << "</td><td class=dosdecimals>" << totalhaber.ascii() << "</td><td class=dosdecimals>" << totalsaldo.ascii() << "</td></tr>\n</table>\n</body>\n</html>\n";
         }

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
      }
   }
#endif
}
