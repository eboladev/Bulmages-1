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
#include <qcheckbox.h>

#include "extractoprintview.h"
#include "listcuentasview1.h"
#include "selectccosteview.h"
#include "selectcanalview.h"
#include "empresa.h"

#include "busquedafecha.h"
#include "busquedacuenta.h"



/** \brief Constructor de la clase que inicializa los parametros necesarios para esta
  * @param emp Empresa con la que va a trabajar esta clase
  * @param parent Ventana que llama a esta ventana
  * @param name Nombre de la ventana
  * Aqui también se inicializa la variable global conexionbase.
  */
ExtractoPrintView::ExtractoPrintView(empresa *emp, QWidget *parent=0, const char *name=0 ) : QDialog(parent,name) {
    setupUi(this);
    fichero=NULL;
    empresaactual=emp;
    conexionbase = emp->bdempresa();
}// end ExtractoPrintView

ExtractoPrintView::~ExtractoPrintView() {}

/**************************************************************
 * Se ha pulsado sobre el botón aceptar del formulario
 **************************************************************/
void ExtractoPrintView::accept() {
    // Versió per si només permetem escollir una opció
    if (radiotexto->isChecked())
        presentar("txt");
    else if (radiohtml->isChecked())
        presentar("html");
}



/** \brief Esta función monta la consulta que se va a realizar contra la base de datos
  * La consulta es de bastante detalle y por eso es conveniente dedicar una función a realizarla
  * Además dicha consulta puede ser invocada desde distintos sitios
  */
QString ExtractoPrintView::montaQuery() {
    _depura("ExtractoPrintView::montaQuery",0);

    extractoview1 *extracto = empresaactual->extractoempresa();
    /// Cogemos los valores del formulario para poder hacer un filtraje adecuado
    QString finicial = extracto->m_fechainicial1->text();
    QString ffinal = extracto->m_fechafinal1->text();
    QString cinicial = extracto->m_codigoinicial->text();
    QString cfinal = extracto->m_codigofinal->text();
    QString contra = extracto->mui_codigocontrapartida->text();

    // Preparamos el string para que aparezca una u otra cosa según el punteo.
    QString tipopunteo;
    tipopunteo="";
    if (extracto->mui_punteotodos->isChecked()) {
        tipopunteo = "";
    } else if (extracto->mui_punteopunteado->isChecked()) {
        tipopunteo = " AND apunte.punteo=TRUE ";
    } else {
        tipopunteo = " AND apunte.punteo=FALSE ";
    }// end if
    if (contra != "") {
        tipopunteo += " AND apunte.contrapartida = id_cuenta('"+contra+"') ";
    }// end if

    selectcanalview *scanal=empresaactual->getselcanales();
    selectccosteview *scoste=empresaactual->getselccostes();
    QString ccostes = scoste->cadcoste();
    if (ccostes != "")
        ccostes.sprintf(" AND idc_coste IN (%s) ", ccostes.ascii());
    QString ccanales = scanal->cadcanal();
    if (ccanales != "")
        ccanales.sprintf(" AND idcanal IN (%s) ", ccanales.ascii());

    QString tabla;
    if (extracto->mui_asAbiertos->isChecked()) {
        tabla= "borrador";
    } else {
        tabla = "apunte";
    }// end if

    QString query = "SELECT desccontrapartida, codcontrapartida, "+tabla+".contrapartida AS contrapartida, ordenasiento, cuenta.descripcion AS descripcion, "+tabla+".debe AS debe , "+tabla+".haber AS haber, conceptocontable, idc_coste, codigo, cuenta.descripcion AS desc1, "+tabla+".fecha AS fecha FROM "+tabla+"";
    query += " LEFT JOIN asiento ON "+tabla+".idasiento=asiento.idasiento ";
    query += " LEFT JOIN cuenta ON "+tabla+".idcuenta=cuenta.idcuenta ";
    query += " LEFT JOIN (SELECT codigo AS codcontrapartida, idcuenta as contrapartida, descripcion AS desccontrapartida  FROM cuenta) AS t1 ON "+tabla+".contrapartida = t1.contrapartida ";
    query += " WHERE "+tabla+".idcuenta >= id_cuenta('"+cinicial+"')   AND "+tabla+".idcuenta <= id_cuenta('"+cfinal+"') ";
    query += " AND "+tabla+".fecha >= '"+finicial+"' AND "+tabla+".fecha <= '"+ffinal+"'";
    query += tipopunteo;
    query += ccostes;
    query += ccanales;
    query += " ORDER BY codigo, fecha";
    return query;
}// end montaQuery




void ExtractoPrintView::presentar(char *tipus) {

    int txt, html;
    float debe, haber,saldo;
    float debeinicial=0, haberinicial=0, saldoinicial=0;
    float debefinal, haberfinal, saldofinal;
    int idcuenta;
    int idasiento;
    int contrapartida;
    string codcontrapartida;
    int activo;
    string cad;
    cursor2 *cursoraux, *cursoraux1, *cursoraux2, *cursoraux3;


    extractoview1 *extracto = empresaactual->extractoempresa();

    QString finicial = extracto->m_fechainicial1->text();
    QString ffinal = extracto->m_fechafinal1->text();
    QString cinicial = extracto->m_codigoinicial->text();
    QString cfinal = extracto->m_codigofinal->text();

    // tipus de presentació
    txt=!strcmp(tipus,"txt");
    html=!strcmp(tipus,"html");

    ofstream fitxersortidatxt("mayor.txt");     // creem els fitxers de sordida
    ofstream fitxersortidahtml("mayor.htm");

    if (!fitxersortidatxt)
        txt=0;    // verifiquem que s'hagin creat correctament els fitxers
    if (!fitxersortidahtml)
        html=0;  // es pot millorar el tractament d'errors

    if (txt | html) {                 // només continuem si hem pogut crear algun fitxer
        if (txt) {
            //presentació txt

            fitxersortidatxt.setf(ios::fixed)
                ;
            fitxersortidatxt.precision(2);
            fitxersortidatxt << "                                    MAYOR \n" ;
            fitxersortidatxt << "Fecha Inicial: " << finicial.ascii() << "   Fecha Final: " << ffinal.ascii() << endl;
            fitxersortidatxt << "_________________________________________________________________________________________________________\n";
        }
        if (html) {
            //presentació html

            fitxersortidahtml.setf(ios::fixed)
                ;
            fitxersortidahtml.precision(2);
            fitxersortidahtml << "<html>\n";
            fitxersortidahtml << "<head>\n";
            fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
            fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
            fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
            fitxersortidahtml << "  <title> Major </title>\n";
            fitxersortidahtml << "</head>\n";
            fitxersortidahtml << "<body>\n";
            fitxersortidahtml << "<table><tr><td colspan=\"6\" class=titolmajor> Mayor <hr></td></tr>\n\n";
            fitxersortidahtml << "<tr><td colspan=\"6\" class=periodemajor> Fecha Inicial: " << finicial.ascii() << " -  Fecha Final: " << ffinal.ascii() << "<hr></td></tr>\n\n";
        }
        conexionbase->begin();
        cursoraux = conexionbase->cargacuentascodigo(-1,cinicial, cfinal);
        conexionbase->commit();
        while(!cursoraux->eof()) {
            idcuenta = atoi(cursoraux->valor("idcuenta").ascii());
            conexionbase->begin();
            cursoraux1 = conexionbase->cargaapuntesctafecha(idcuenta, finicial.ascii(), ffinal.ascii());
            conexionbase->commit();

            if (!cursoraux1->eof()) {
                activo = strcmp((char *) cursoraux->valor("activo").ascii() , "f");

                if (txt) {
                    fitxersortidatxt << "\n\n" << setw(12) << cursoraux->valor("codigo").ascii() << setw(50) << cursoraux->valor("descripcion").ascii() << endl;
                    if (activo) {
                        fitxersortidatxt << " Cuenta de activo";
                    } else {
                        fitxersortidatxt << " Cuenta de pasivo";
                    }
                }

                if (html) {
                    fitxersortidahtml << "<tr><td colspan=\"6\" class=comptemajor>" << cursoraux->valor("codigo").ascii() << " "<< cursoraux->valor("descripcion").ascii() << "</td></tr>\n";
                    if (activo) {
                        fitxersortidahtml << "<tr><td colspan=\"6\" class=tipuscomptemajor> Cuenta de activo </td></tr>\n";
                    } else {
                        fitxersortidahtml << "<tr><td colspan=\"6\" class=tipuscomptemajor> Cuenta de pasivo </td></tr>\n";
                    }

                }

                conexionbase->begin();
                cursoraux2 = conexionbase->cargasaldoscuentafecha(idcuenta, (char *)finicial.ascii());
                conexionbase->commit();
                if (!cursoraux2->eof()) {
                    debeinicial = atof(cursoraux2->valor("tdebe").ascii());
                    haberinicial = atof(cursoraux2->valor("thaber").ascii());
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
                        idasiento=atoi(cursoraux1->valor("idasiento").ascii());
                        contrapartida = atoi(cursoraux1->valor("contrapartida").ascii());
                        conexionbase->begin();
                        cursoraux3 = conexionbase->cargacuenta(contrapartida);
                        codcontrapartida = cursoraux3->valor("codigo").ascii();
                        conexionbase->commit();
                        debe=atof(cursoraux1->valor("debe").ascii());
                        haber=atof(cursoraux1->valor("haber").ascii());
                        if (activo) {
                            saldo += debe - haber;
                        } else {
                            saldo += haber -debe;
                        }
                        debefinal += debe;
                        haberfinal += haber;
                        cad = cursoraux1->valor("fecha").ascii();
                        //presentació txt
                        if (txt)
                            fitxersortidatxt <<  setw(5) << idasiento << setw(14) << cad.substr(0,10).c_str() << setw(10) << codcontrapartida << "  " << setw(40)  << setiosflags(ios::left) << cursoraux1->valor("conceptocontable").ascii() << setw(10) << resetiosflags(ios::left) << debe << setw(10) << haber << setw(10) << saldo << endl;
                        //presentació html
                        if (html)
                            fitxersortidahtml << " <tr><td class=assentamentmajor> " << idasiento << " </td><td> " << cad.substr(0,10).c_str() << " </td><td class=contrapartidamajor> " << codcontrapartida << " </td><td> " << cursoraux1->valor("conceptocontable").ascii() << " </td><td class=dosdecimals> " << debe << " </td><td class=dosdecimals> " << haber << " </td><td class=dosdecimals> " << saldo << " </td></tr>\n ";
                        cursoraux3->cerrar();
                    }

                    if (activo) {
                        saldofinal = debefinal - haberfinal;
                    } else {
                        saldofinal = haberfinal - debefinal;
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
                }
            }
            cursoraux1->cerrar();
            cursoraux->siguienteregistro();
        }
        if (html)
            fitxersortidahtml << "\n</table></body></html>\n";     //presentació html
        conexionbase->commit();
        delete cursoraux;
    }
    if (txt) {
        //presentació txt

        fitxersortidatxt.close();
	QString cad = confpr->valor(CONF_EDITOR)+" mayor.txt";
	system (cad.ascii());
    }
    if (html) {
        //presentació html
        fitxersortidahtml.close();
	QString cad = confpr->valor(CONF_NAVEGADOR)+" mayor.html";
	system (cad.ascii());
    }
}




