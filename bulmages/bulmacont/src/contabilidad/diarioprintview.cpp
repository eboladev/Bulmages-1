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
#include "empresa.h"

#include <unistd.h>
#include <qcheckbox.h>
#include "selectccosteview.h"
#include "selectcanalview.h"
#include "diarioview.h"

DiarioPrintView::DiarioPrintView(empresa *emp,QWidget *parent, const char *name ) : QDialog(parent,name) {
    setupUi(this);
    empresaactual = emp;
    conexionbase = empresaactual->bdempresa();
    numdigitos = empresaactual->numdigitosempresa();
}// end DiarioPrintView



DiarioPrintView::~DiarioPrintView() {}

/** \brief Esta función monta la consulta que se va a realizar contra la base de datos
  * La consulta es de bastante detalle y por eso es conveniente dedicar una función a realizarla
  * Además dicha consulta puede ser invocada desde distintos sitios
  */
QString DiarioPrintView::montaQuery() {
    DiarioView *diario = empresaactual->diarioempresa();

    QString query;
    QString fecha;
    QString fechaasiento;
    QString descripcion;
    QString concepto;
    QString codigocuenta;
    QString nomcuenta;
    QString cadaux;

    QString finicial = diario->mui_fechainicial->text();
    QString ffinal = diario->mui_fechafinal->text();
    QString cad;

    // Consideraciones para centros de coste y canales
    selectcanalview *scanal=empresaactual->getselcanales();
    selectccosteview *scoste=empresaactual->getselccostes();
    QString ccostes = scoste->cadcoste();
    if (ccostes != "")
        ccostes.sprintf(" AND idc_coste IN (%s) ", ccostes.ascii());
    QString ccanales = scanal->cadcanal();
    if (ccanales != "")
        ccanales.sprintf(" AND idcanal IN (%s) ", ccanales.ascii());

    QString tabla;
    if (diario->mui_asAbiertos->isChecked()) {
        tabla = "borrador";
    } else {
        tabla = "apunte";
    }// end if

    query= "SELECT asiento.ordenasiento, "+tabla+".contrapartida, "+tabla+".fecha, asiento.fecha AS fechaasiento,cuenta.tipocuenta, cuenta.descripcion, "+tabla+".conceptocontable,"+tabla+".descripcion AS descapunte, to_char("+tabla+".debe,'MI999G999G990D99') AS debe, to_char("+tabla+".haber,'MI999G999G990D99') AS haber, cuenta.idcuenta, asiento.idasiento, "+tabla+".idc_coste, "+tabla+".idcanal, cuenta.codigo AS codigocuenta FROM asiento, "+tabla+", cuenta WHERE asiento.idasiento="+tabla+".idasiento AND "+tabla+".idcuenta = cuenta.idcuenta AND "+tabla+".fecha >= '"+finicial+"' AND "+tabla+".fecha <= '"+ffinal+"' "+ccostes+" "+ccanales+" ORDER BY asiento.ordenasiento, "+tabla+".orden";
    return query;
}// end montaQuery

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
    } else  if (radiohtml->isChecked()) {
        if (radionormal->isChecked()) {
            presentar("html");
        } else if (radioaprendizaje->isChecked()) {
            presentar("htmlapren");
        }// end if
    }
}// end accept




void DiarioPrintView::presentar(char * tipus) {
    DiarioView *diario = empresaactual->diarioempresa();
    int txt, html, txtapren, htmlapren;

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
    cursor2 *cursoraux;

    // tipus de presentació
    txt=!strcmp(tipus,"txt");
    html=!strcmp(tipus,"html");
    txtapren=!strcmp(tipus,"txtapren");
    htmlapren=!strcmp(tipus,"htmlapren");

    // Cogemos los valores del formulario.
    QString finicial = diario->mui_fechainicial->text();
    QString ffinal = diario->mui_fechafinal->text();

    if (txt | html) {
        ofstream fitxersortidatxt("diario.txt");     // creem els fitxers de sordida
        ofstream fitxersortidahtml("diario.html");

        if (!fitxersortidatxt)
            txt=0;    // verifiquem que s'hagin creat correctament els fitxers
        if (!fitxersortidahtml)
            html=0;  // es pot millorar el tractament d'erro

        if (txt | html) {                // només continuem si hem pogut crear algun fitxer
            if (txt) {
                //presentació txt normal

                fitxersortidatxt.setf(ios::fixed)
                    ;
                fitxersortidatxt.precision(2);

                fitxersortidatxt << "                                        LLIBRE DIARI \n" ;
                fitxersortidatxt << "Data Inicial: " << finicial.ascii() << "   Data Final: " << ffinal.ascii() << endl;
                fitxersortidatxt << "ASIENTO   FECHA    SUBCUENTA   DESCRIPCION                                   DEBE     HABER \n" ;
                fitxersortidatxt << "_______________________________________________________________________________________________\n";
            }// end if
            if (html) {
                //presentació html normal
                fitxersortidahtml.setf(ios::fixed)
                    ;
                fitxersortidahtml.precision(2);
                fitxersortidahtml << "<html>\n";
                fitxersortidahtml << "<head>\n";
                fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
                fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
                fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\""<< confpr->valor(CONF_PLANTILLA).ascii() <<"\" TYPE=\"text/css\" MEDIA=screen>\n";
                fitxersortidahtml << "  <title> Llibre Diari </title>\n";
                fitxersortidahtml << "</head>\n";
                fitxersortidahtml << "<body>\n";
                fitxersortidahtml << "<table><tr><td colspan=\"6\" class=titoldiari> Llibre diari <hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td colspan=\"6\" class=periodediari> Data Inicial: " << finicial.ascii() << " -  Data Final: " << ffinal.ascii() << "<hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td class=titolcolumnadiari>Assentament</td><td class=titolcolumnadiari>Data</td><td class=titolcolumnadiari>Subcompte</td><td class=titolcolumnadiari>Descripció</td><td class=titolcolumnadiari>Deure</td><td class=titolcolumnadiari>Haver</td></tr>\n";
            }

            QString query = montaQuery();
            conexionbase->begin();
            cursoraux = conexionbase->cargacursor(query,"elquerydb");
            conexionbase->commit();

            for(;!cursoraux->eof();cursoraux->siguienteregistro()) {
                fechaasiento = cursoraux->valor("fechaasiento").ascii();
                idasiento = atoi(cursoraux->valor("ordenasiento").ascii());
                fecha = cursoraux->valor("fecha").ascii();
                descripcion = cursoraux->valor("descripcion").ascii();
                concepto = cursoraux->valor("conceptocontable").ascii();
                debe = atof(cursoraux->valor("debe").ascii());
                haber = atof(cursoraux->valor("haber").ascii());
                idcuenta = atoi(cursoraux->valor("idcuenta").ascii());
                codigocuenta = cursoraux->valor("codigocuenta").ascii();
                strncpy(data,fecha.c_str(),10);
                data[10]='\0';
                if (txt) {
                    //presentació txt normal
                    fitxersortidatxt << setw(5) << idasiento << "  " << setw(10) << data << " " << setw(10) << codigocuenta.c_str() <<  "  " << setw(40) <<  setiosflags(ios::left) << descripcion.c_str() <<  resetiosflags(ios::left)<< " " << setw(10) << debe << " " << setw(10) << haber << endl;
                }// end if
                if (html) {
                    //presentació html normal
                    fitxersortidahtml << "<tr><td class=assentamentdiari>" << idasiento << "</td><td class=datadiari>" << data << "</td><td class=codicomptediari>" << codigocuenta.c_str() << "</td><td class=descripciodiari>" <<  descripcion.c_str() << "</td><td class=dosdecimals>" << debe << "</td><td class=dosdecimals>" << haber << "</td></tr>\n";
                }// end if
            }// end for
            delete cursoraux;
            if (html) {
                fitxersortidahtml << "</table>\n<hr>\n</body>\n</html>\n";     //presentació html normal
                fitxersortidahtml.close();
            }// end if
            if (txt) {
                fitxersortidatxt << "_______________________________________________________________________________________________\n";
                fitxersortidatxt.close();
            }// end if
        }// end if


        if (txt) {
            //presentació txt normal
            QString cadaux = confpr->valor(CONF_EDITOR)+" diairo.txt";
            system (cadaux.ascii());
        }
        if (html) {
            //presentació html normal
            QString cadaux = confpr->valor(CONF_NAVEGADOR)+" diario.html";
            system (cadaux.ascii());
        }// end if
    }
    if (txtapren | htmlapren ) {

        char *argstxt[]={"diariaprenentatge.txt","diariaprenentatge.txt",NULL};      //presentació txt normal
        char *argshtml[]={"diariaprenentatge.html","diariaprenentatge.html",NULL};   //presentació html normal

        ofstream fitxersortidatxt(argstxt[0]);     // creem els fitxers de sordida
        ofstream fitxersortidahtml(argshtml[0]);

        if (!fitxersortidatxt)
            txtapren=0;    // verifiquem que s'hagin creat correctament els fitxers
        if (!fitxersortidahtml)
            htmlapren=0;  // es pot millorar el tractament d'errors

        if (txtapren | htmlapren) {       // només continuem si hem pogut crear algun fitxer
            if (txtapren) {
                //presentació txt format aprenentatge

                fitxersortidatxt.setf(ios::fixed)
                    ;
                fitxersortidatxt.precision(2);

                fitxersortidatxt << "                                                      LLIBRE DIARI \n" ;
                fitxersortidatxt << "Data Inicial: " << finicial.ascii() << "   Data Final: " << ffinal.ascii() << endl;
                fitxersortidatxt << "___________________________________________________________________________________________________________________________________\n";

            }
            if (htmlapren) {
                //presentació html format aprenentatge

                fitxersortidahtml.setf(ios::fixed)
                    ;
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
                fitxersortidahtml << "<tr><td colspan=\"7\" class=periodediariapren> Data Inicial: " << finicial.ascii() << " -  Data Final: " << ffinal.ascii() << "<hr></td></tr>\n\n";
            }
            conexionbase->begin();
            cursoraux = conexionbase->cargaasientosfecha((char *)finicial.ascii(),(char *)ffinal.ascii());

            for(;!cursoraux->eof();cursoraux->siguienteregistro()) {
                int idasiento=atoi(cursoraux->valor("idasiento").ascii());

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
                        sprintf(codicompte,"( %s )",cursasiento->valor("codigo").ascii());
                        fitxersortidatxt << setw(12) << atof(cursasiento->valor("debe").ascii()) << "  " << setw(10) << codicompte << "  " << cursasiento->valor("nomcuenta").ascii() << endl;
                    }
                    if (htmlapren) {
                        //presentació html format aprenentatge
                        fitxersortidahtml << " <tr><td class=deurediariapren> " << atof(cursasiento->valor("debe").ascii()) << " </td><td class=codidiariapren> ( " << cursasiento->valor("codigo").ascii() << " ) </td><td class=nomcomptediariapren> " << cursasiento->valor("nomcuenta").ascii() << " </td><td> </td><td> </td><td> </td><td> </td></tr>\n";
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
                        sprintf(codicompte,"( %s )",cursasiento->valor("codigo").ascii());
                        fitxersortidatxt << "                                                                 a  " << setw(30) << cursasiento->valor("nomcuenta").ascii() << "  " << setw(10) << codicompte << "  " << setw(12) <<  atof(cursasiento->valor("haber").ascii()) <<endl;
                    }// end if
                    if (htmlapren) {
                        //presentació html format aprenentatge
                        fitxersortidahtml << " <tr><td> </td><td> </td><td> </td><td class=adiariapren>  a  </td><td class=nomcomptediariapren> " << cursasiento->valor("nomcuenta").ascii() << " </td><td class=codidiariapren> ( " << cursasiento->valor("codigo").ascii() << " ) </td><td class=haverdiariapren> " <<  atof(cursasiento->valor("haber").ascii()) << " </td></tr>\n";
                    }// end if
                    cursasiento->siguienteregistro();
                }// end while
                delete cursasiento;
            }// end for

            delete cursoraux;
            conexionbase->commit();

            if (txtapren) {
                fitxersortidatxt << "____________________________________________________________________________________________________________________________________\n" ;//presentació text format aprenentatge
                fitxersortidatxt.close();
            }// end if

            if (htmlapren) {
                fitxersortidahtml << "<tr><td colspan=\"7\"  class=liniadiariapren>\n_____________________________________________________________________________________________________________\n<hr></td></tr>\n</table>\n</body></html>\n";     //presentació html format aprenentatge
                fitxersortidahtml.close();
            }// end if



            if (txtapren) {
                //presentació txt format aprenentatge
                QString cadaux = confpr->valor(CONF_EDITOR)+" diario.txt";
                system (cadaux.ascii());
            }// end if

            if (htmlapren) {
                //presentació html format aprenentatge
                QString cadaux = confpr->valor(CONF_NAVEGADOR)+" diario.txt";
                system (cadaux.ascii());
            }// end if
        }// end if
    }// end if
}// end presentar






