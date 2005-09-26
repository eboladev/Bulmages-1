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
#include <qtable.h>
#include <qtoolbutton.h>
#include <qcheckbox.h>
#include <qlocale.h>

#include "empresa.h"
#include "arbol.h"

#include "busquedacuenta.h"
#include "busquedafecha.h"



#ifndef WIN32
#include <unistd.h>
#endif



BalancePrintView::BalancePrintView(empresa *emp) {
    empresaactual=emp;
    conexionbase = emp->bdempresa();
    m_codigoinicial->setempresa(emp);
    m_codigofinal->setempresa(emp);
    // Buscamos los diferentes niveles que existen segun existan en la tabla de cuentas
    conexionbase->begin();
    QString query = "SELECT nivel(codigo) FROM cuenta GROUP BY nivel ORDER BY nivel";
    cursor2* niveles = conexionbase->cargacursor(query, "Niveles");
    int i=0;
    while(!niveles->eof()) {
        // Inicializamos la tabla de nivel
        combonivel->insertItem(niveles->valor("nivel"),i);
        niveles->siguienteregistro();
        i++;
    }// end while
    conexionbase->commit();
    delete niveles;
}

BalancePrintView::~BalancePrintView() {}


/** \brief inicializa la clase para que tenga parámetros por defecto (no es obligatorio)
  * codinicial, codfinal, finicial, ffinal, arbol)
  * codinicial indica el cóigo inicial para mostrar el balance
  * cofinal indica el codigo de cuenta para no mostrar más balances
  * finicial indica la fecha a partir de la cual comenzar los saldos
  * ffinal indica la fecha para terminar de contar los saldos
  * arbol indica si hay que representar el balance en forma de árbol o no.
  */
void BalancePrintView::inicializa1(QString codinicial1, QString codfinal1, QString finicial1, QString ffinal1, bool arbol) {
    m_fechainicial1->setText(finicial1);
    m_fechafinal1->setText(ffinal1);
    m_codigoinicial->setText(codinicial1);
    m_codigofinal->setText(codfinal1);
    checksuperiores->setChecked(arbol);
}// end if


/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/
void BalancePrintView::accept() {
    if (radiotexto->isChecked())
        presentar("txt");
    if(radiohtml->isChecked())
        presentar("html");
    if (radiopropietario->isChecked())
        presentar("rtk");
    if (radiokugar->isChecked())
        presentar("kugar");
}// end accept




/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/
void BalancePrintView::presentar(char *tipus) {
#ifndef WIN32
    int kugar, txt, html, txtapren, htmlapren;
    double tsaldoant, tdebe, thaber, tsaldo, debeej, haberej, saldoej;
    QString query;

    // tipos de presentaciones
    txt = !strcmp(tipus,"txt");
    html = !strcmp(tipus,"html");
    txtapren = !strcmp(tipus,"txtapren");
    htmlapren = !strcmp(tipus,"htmlapren");
    kugar = !strcmp(tipus,"kugar");

    // Cogemos los valores del formulario
    QString finicial = m_fechainicial1->text();
    QString ffinal = m_fechafinal1->text();
    QString cinicial = m_codigoinicial->codigocuenta();
    QString cfinal = m_codigofinal->codigocuenta();
    int nivel = combonivel->currentText().toInt();
    bool superiores = checksuperiores->isChecked();

    if (txt | html | kugar) {
	QString archivo = confpr->valor(CONF_DIR_USER)+"/balance.txt";
	QString archivokugar = confpr->valor(CONF_DIR_USER)+"/balance.kud";
	QString archivohtml = confpr->valor(CONF_DIR_USER)+"/balance.html";
	fprintf(stderr,"%s\n",archivo.ascii());

        ofstream fitxersortidakugar(archivokugar.ascii());   // creamos los ficheros de salida
        ofstream fitxersortidatxt(archivo.ascii());
        ofstream fitxersortidahtml(archivohtml.ascii());

        if (!fitxersortidatxt)
            txt=0;    // verifiquem que s'hagin creat correctament els fitxers
        if (!fitxersortidahtml)
            html=0;  // es pot millorar el tractament d'errors
        if (!fitxersortidakugar)
            kugar=0;

        QLocale spanish(QLocale::Spanish); // vamos a formatear los números con punto para los millares y coma para los decimales

        if (txt | html | kugar) {                // només continuem si hem pogut crear algun fitxer

            // Vamos a crear un arbol en la memoria dinamica con los distintos niveles de cuentas
            // Primero, averiguaremos la cantidad de ramas iniciales (tantos como numero de cuentas de nivel 2) y las vamos creando
            conexionbase->begin();
            query.sprintf("SELECT *, nivel(codigo) AS nivel FROM cuenta ORDER BY codigo");
            cursor2 *ramas;
            ramas = conexionbase->cargacursor(query,"Ramas");
            Arbol *arbol;
            arbol = new Arbol;
            while(!ramas->eof()) {
                if(atoi(ramas->valor("nivel").ascii()) == 2) // Cuenta raiz
                    arbol->nuevarama(ramas);
                ramas->siguienteregistro();
            }
            arbol->inicializa(ramas);
            delete ramas;

            // Ahora despues, usaremos el arbol para poner los datos a cada hoja (cuenta) según los periodos que necesitemos acotar.
            // Pero antes, preparamos las plantillas segun el tipo de salida seleccionado

            if (kugar) {
                fitxersortidakugar.setf(ios::fixed)
                    ;
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
                //presentació txt normal
                fitxersortidatxt.setf(ios::fixed)
                    ;
                fitxersortidatxt.precision(2);
                fitxersortidatxt << "                                        Balance \n" ;
                fitxersortidatxt << "Fecha Inicial: " << finicial.ascii() << "   Fecha Final: " << ffinal.ascii() << endl;
                fitxersortidatxt << "Cuenta            Denominación                        Saldo ant.         Debe        Haber        Saldo     Debe ej.    Haber ej.    Saldo ej.\n" ;
                fitxersortidatxt << "______________________________________________________________________________________________________________________________________________\n";
            }// end if

            if (html) {
                //presentació html normal
                fitxersortidahtml.setf(ios::fixed);
                fitxersortidahtml.precision(2);
                fitxersortidahtml << "<html>\n";
                fitxersortidahtml << "<head>\n";
                fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
                fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
                fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
                fitxersortidahtml << "  <title> Balance </title>\n";
                fitxersortidahtml << "</head>\n";
                fitxersortidahtml << "<body>\n";
                fitxersortidahtml << "<table><tr><td colspan=\"9\" class=titolbalanc> Balance <hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td colspan=\"9\" class=periodebalanc> Data Inicial: " << finicial.ascii() << " -  Data Final: " << ffinal.ascii() << "<hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td class=titolcolumnabalanc>lcuenta</td><td class=titolcolumnabalanc> ldenominacion</td><td class=titolcolumnabalanc>lsaldoant</td><td class=titolcolumnabalanc>ldebe</td><td class=titolcolumnabalanc>lhaber</td><td class=titolcolumnabalanc>lsaldo</td><td class=titolcolumnabalanc> ldebeej  </td><td class=titolcolumnabalanc> lhaberej </td><td class=titolcolumnabalanc> lsaldoej </td></tr>\n";
            }// end if

            // Vamos a recopilar todos los apuntes agrupados por cuenta para poder establecer así los valores de cada cuenta
            query.sprintf("SELECT cuenta.idcuenta, numapuntes, cuenta.codigo, saldoant, debe, haber, saldo, debeej, haberej, saldoej FROM (SELECT idcuenta, codigo FROM cuenta) AS cuenta NATURAL JOIN (SELECT idcuenta, count(idcuenta) AS numapuntes,sum(debe) AS debeej, sum(haber) AS haberej, (sum(debe)-sum(haber)) AS saldoej FROM apunte WHERE EXTRACT(year FROM fecha) = EXTRACT(year FROM timestamp '%s') GROUP BY idcuenta) AS ejercicio LEFT OUTER JOIN (SELECT idcuenta,sum(debe) AS debe, sum(haber) AS haber, (sum(debe)-sum(haber)) AS saldo FROM apunte WHERE fecha >= '%s' AND fecha <= '%s' GROUP BY idcuenta) AS periodo ON periodo.idcuenta=ejercicio.idcuenta LEFT OUTER JOIN (SELECT idcuenta, (sum(debe)-sum(haber)) AS saldoant FROM apunte WHERE fecha < '%s' GROUP BY idcuenta) AS anterior ON cuenta.idcuenta=anterior.idcuenta ORDER BY codigo", finicial.ascii(), finicial.ascii(), ffinal.ascii(), finicial.ascii());
            cursor2 *cuentas;
            cuentas = conexionbase->cargacursor(query,"Periodo");
            while(!cuentas->eof()) { // Para cada cuenta con sus apuntes hechos hay que actualizar hojas del árbol
                arbol->actualizahojas(cuentas);
                cuentas->siguienteregistro();
            }// end while

            float linea = 1; // irá contando las líneas impresas en el impreso de kugar
            float lineaskugar = 53; // determina cuántas líneas caben para el impreso de kugar
            tsaldoant = tdebe = thaber = tsaldo = debeej = haberej = saldoej = 0;
            // Ahora imprimimos los valores
            arbol->inicia();
            while (arbol->deshoja(nivel, superiores)) {
                QString lcuenta = arbol->hojaactual("codigo");
                QString ldenominacion = arbol->hojaactual("descripcion");
                QString lsaldoant = arbol->hojaactual("saldoant");
                QString ldebe = arbol->hojaactual("debe");
                QString lhaber = arbol->hojaactual("haber");
                QString lsaldo = arbol->hojaactual("saldo");
                QString ldebeej = arbol->hojaactual("debeej");
                QString lhaberej = arbol->hojaactual("haberej");
                QString lsaldoej = arbol->hojaactual("saldoej");

                // Acumulamos los totales para al final poder escribirlos
                tsaldoant += lsaldoant.toDouble();
                tsaldo += lsaldo.toDouble();
                tdebe += ldebe.toDouble();
                thaber += lhaber.toDouble();
                debeej += ldebeej.toDouble();
                haberej += lhaberej.toDouble();
                saldoej += lsaldoej.toDouble();

                // Las variables de las filas en formato español
                lsaldoant = spanish.toString(lsaldoant.toDouble(),'f',2);
                ldebe = spanish.toString(ldebe.toDouble(),'f',2);
                lhaber = spanish.toString(lhaber.toDouble(),'f',2);
                lsaldo = spanish.toString(lsaldo.toDouble(),'f',2);
                ldebeej = spanish.toString(ldebeej.toDouble(),'f',2);
                lhaberej = spanish.toString(lhaberej.toDouble(),'f',2);
                lsaldoej = spanish.toString(lsaldoej.toDouble(),'f',2);

                // Imprimimos linea segun formato
                if (txt) { //presentacion en txt normal
                    fitxersortidatxt << setiosflags(ios::left) << setw(10) <<  lcuenta.ascii() << " " << setw(40) <<  ldenominacion.left(40).ascii() << " " << resetiosflags(ios::left) << setw(12) <<  lsaldoant.ascii() << " " << setw(12) <<  ldebe.ascii() << " " << setw(12) <<  lhaber.ascii() << " " << setw(12) <<  lsaldo << " " << setw(12) <<  ldebeej.ascii() << " " << setw(12) <<  lhaberej.ascii() << " " << setw(12) <<  lsaldoej.ascii() << " " << setw(12) <<  endl;
                }// end if

                if (html) { //presentacion en html normal
                    fitxersortidahtml << "<tr><td class=comptebalanc>" << lcuenta.ascii() << "</td><td class=assentamentbalanc>" <<  ldenominacion.left(40).ascii() << "</td><td class=dosdecimals>" << lsaldoant.ascii() << "</td><td class=dosdecimals>" << ldebe.ascii() << "</td><td class=dosdecimals>" << lhaber.ascii() << "</td><td class=dosdecimals>" << lsaldo.ascii() << "</td><td class=dosdecimals>" << ldebeej.ascii() << "</td><td class=dosdecimals>" << lhaberej.ascii() << "</td><td class=dosdecimals>" << lsaldoej.ascii() << endl;
                }// end if

                if (kugar) { //presentacion en kugar segun plantilla balance.kut
                    // Primero vamos a establecer si hay que imprimir una línea de cabecera (Detail 0 en balance.kut) con los datos del período
                    if(fmod(linea,lineaskugar) == 0) {
                        fitxersortidakugar << "\t<Row";
                        fitxersortidakugar << " level=\"0\"";
                        fitxersortidakugar << " finicial='"<< finicial.ascii() <<"'";
                        fitxersortidakugar << " ffinal='"<< ffinal.ascii() <<"'/>\n";
                    }// end if
                    if(linea == lineaskugar) {
                        lineaskugar = 54; // a partir de la segunda página alguna linea mas
                        linea = lineaskugar;
                    }
                    // Ahora, imprimimos tantas líneas como nos permite la variable "lineaskugar" sin imprimir cabecera (Detail 1 en balance.kut)
                    // Una linea por cada iteracion del buble
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
            }// end while

            // Hacemos la actualizacion de los saldos totales en formato español
            QString totalsaldoant = spanish.toString(tsaldoant,'f',2);
            QString totaldebe = spanish.toString(tdebe,'f',2);
            QString totalhaber = spanish.toString(thaber,'f',2);
            QString totalsaldo = spanish.toString(tsaldo,'f',2);
            QString totaldebeej = spanish.toString(debeej,'f',2);
            QString totalhaberej = spanish.toString(haberej,'f',2);
            QString totalsaldoej = spanish.toString(saldoej,'f',2);

            // Imprimimos la linea con los resultados totalizados
            if (txt) { //presentacion txt normal
                fitxersortidatxt << "                                            __________________________________________________________________________________________________\n";
                fitxersortidatxt << "                                            Totales " << setw(12) <<  totalsaldoant.ascii() << " " << setw(12) <<  totaldebe.ascii() << " " << setw(12) <<  totalhaber.ascii() << " " << setw(12) <<  totalsaldo.ascii()  << " " << setw(12) <<  totaldebeej.ascii() << " " << setw(12) <<  totalhaberej.ascii() << " " << setw(12) <<  totalsaldoej.ascii() << endl;
            }

            if (html) { //presentacion html normal
                fitxersortidahtml << "<tr><td></td><td class=totalbalanc>Totals</td><td class=dosdecimals>" <<  totalsaldoant.ascii() << "</td><td class=dosdecimals>" << totaldebe.ascii() << "</td><td class=dosdecimals>" << totalhaber.ascii() << "</td><td class=dosdecimals>" << totalsaldo.ascii() << "</td></tr>\n</table>\n</body>\n</html>\n";
            }

            if (kugar) { //presentacion kugar
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
            }

            // Eliminamos el arbol y cerramos la conexion con la BD
            delete arbol;
            conexionbase->commit();

            fitxersortidatxt.close();
            // Dependiendo del formato de salida ejecutaremos el programa correspondiente
            if (txt) { //presentacion txt normal
		QString cadena = confpr->valor(CONF_EDITOR)+" "+confpr->valor(CONF_DIR_USER)+"/balance.txt";
		system (cadena.ascii());
            }// end if
            if (html) { //presentacion html normal
		QString cadena = confpr->valor(CONF_NAVEGADOR)+" "+confpr->valor(CONF_DIR_USER)+"/balance.txt";
		system (cadena.ascii());
            }
            if (kugar) { //presentacion kugar normal
		QString cadena = "kugar "+confpr->valor(CONF_DIR_USER)+"/balance.txt";
		system (cadena.ascii());
            }

        }
    }
#endif
}

/** \brief SLOT que responde a la pulsacion del boton de seleccion de canal.
  * Presenta la ventana de seleccion de canales \ref selectcanalview
  */
void BalancePrintView::boton_canales() {
    selectcanalview *selcanales = empresaactual->getselcanales();
    selcanales->exec();
    selcanales->firstcanal();
}// end boton_canales


/** \brief SLOT que responde a la pulsacion del boton de seleccion de centros de coste.
  * Presenta la ventana de seleccion de centros de coste \ref selectccosteview
  */
void BalancePrintView::boton_ccostes() {
    selectccosteview *selccostes = empresaactual->getselccostes();
    selccostes->exec();
    selccostes->firstccoste();
}// end boton_ccostes

