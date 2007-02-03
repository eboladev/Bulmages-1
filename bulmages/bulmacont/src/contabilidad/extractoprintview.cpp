/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2003 by Antoni Mirabete i Teres                         *
 *   amirabet@biada.org                                                    *
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

#include <unistd.h>

#include <QWidget>
#include <QCheckBox>

#include "extractoprintview.h"
#include "listcuentasview1.h"
#include "selectccosteview.h"
#include "selectcanalview.h"
#include "empresa.h"
#include "busquedafecha.h"
#include "busquedacuenta.h"

using namespace std;

/// Constructor de la clase que inicializa los parametros necesarios para esta
/** @param emp Empresa con la que va a trabajar esta clase.
    @param parent Ventana que llama a esta ventana.
    @param name Nombre de la ventana.
    Aqu&iacute; tambi&eacute;n se inicializa la variable global conexionbase. */
ExtractoPrintView::ExtractoPrintView(empresa *emp, QWidget *parent = 0)
        : QDialog(parent) {
    _depura("ExtractoPrintView::ExtractoPrintView", 0);
    setupUi(this);
    fichero = NULL;
    m_companyact = emp;
    _depura("END ExtractoPrintView::ExtractoPrintView", 0);
}


ExtractoPrintView::~ExtractoPrintView() {}


/// Se ha pulsado sobre el bot&oacute;n aceptar del formulario.
void ExtractoPrintView::accept() {
    /// Versi&oacute;n por si s&oacute;lo permitimos elegir una opci&oacute;n.
    if (radiotexto->isChecked()) {
        presentar("txt");
    } else if (radiohtml->isChecked()) {
        presentar("html");
    } // end if
}



/// Esta funci&oacute;n monta la consulta que se va a realizar contra la base de datos.
/** La consulta es de bastante detalle y por eso es conveniente dedicar una funci&oacute;n
    a realizarla. Adem&aacute;s dicha consulta puede ser invocada desde distintos sitios. */
QString ExtractoPrintView::montaQuery() {
    _depura("ExtractoPrintView::montaQuery", 0);
    extractoview1 *extracto = m_companyact->extractoempresa();
    /// Cogemos los valores del formulario para poder hacer un filtraje adecuado.
    QString finicial = extracto->m_fechainicial1->text();
    QString ffinal = extracto->m_fechafinal1->text();
    QString cinicial = extracto->m_codigoinicial->text();
    QString cfinal = extracto->m_codigofinal->text();
    QString contra = extracto->mui_codigocontrapartida->text();

    /// Preparamos el string para que aparezca una u otra cosa seg&uacute;n el punteo.
    QString tipopunteo;
    tipopunteo = "";
    if (extracto->mui_punteotodos->isChecked()) {
        tipopunteo = "";
    } else if (extracto->mui_punteopunteado->isChecked()) {
        tipopunteo = " AND apunte.punteo = TRUE ";
    } else {
        tipopunteo = " AND apunte.punteo = FALSE ";
    } // end if
    if (contra != "") {
        tipopunteo += " AND apunte.contrapartida = id_cuenta('" + contra + "') ";
    } // end if

    selectcanalview *scanal = m_companyact->getselcanales();
    selectccosteview *scoste = m_companyact->getselccostes();
    QString ccostes = scoste->cadcoste();
    if (ccostes != "") {
        ccostes.sprintf(" AND idc_coste IN (%s) ", ccostes.toAscii().constData());
    } // end if
    QString ccanales = scanal->cadcanal();
    if (ccanales != "") {
        ccanales.sprintf(" AND idcanal IN (%s) ", ccanales.toAscii().constData());
    } // end if
    QString tabla;
    if (extracto->mui_asAbiertos->isChecked()) {
        tabla = "borrador";
    } else {
        tabla = "apunte";
    } // end if

    QString query = "SELECT desccontrapartida, codcontrapartida, " + tabla + ".contrapartida AS contrapartida, ordenasiento, cuenta.descripcion AS descripcion, " + tabla + ".debe AS debe , " + tabla + ".haber AS haber, conceptocontable, idc_coste, codigo, cuenta.descripcion AS desc1, " + tabla + ".fecha AS fecha FROM " + tabla + "";
    query += " LEFT JOIN asiento ON " + tabla + ".idasiento = asiento.idasiento ";
    query += " LEFT JOIN cuenta ON " + tabla + ".idcuenta = cuenta.idcuenta ";
    query += " LEFT JOIN (SELECT codigo AS codcontrapartida, idcuenta as contrapartida, descripcion AS desccontrapartida FROM cuenta) AS t1 ON " + tabla + ".contrapartida = t1.contrapartida ";
    query += " WHERE " + tabla + ".idcuenta >= id_cuenta('" + cinicial + "')   AND " + tabla + ".idcuenta <= id_cuenta('" + cfinal + "') ";
    query += " AND " + tabla + ".fecha >= '" + finicial + "' AND " + tabla + ".fecha <= '" + ffinal + "'";
    query += tipopunteo;
    query += ccostes;
    query += ccanales;
    query += " ORDER BY codigo, fecha";
    return query;
}


void ExtractoPrintView::presentar(char *tipus) {
    int txt, html;
    float debe, haber,saldo;
    float debeinicial = 0, haberinicial = 0, saldoinicial = 0;
    float debefinal, haberfinal, saldofinal;
    int idcuenta;
    int idasiento;
    int contrapartida;
    string codcontrapartida;
    int activo;
    string cad;
    cursor2 *cursoraux, *cursoraux1, *cursoraux2, *cursoraux3;
    extractoview1 *extracto = m_companyact->extractoempresa();
    QString finicial = extracto->m_fechainicial1->text();
    QString ffinal = extracto->m_fechafinal1->text();
    QString cinicial = extracto->m_codigoinicial->text();
    QString cfinal = extracto->m_codigofinal->text();

    /// Tipos de presentaci&oacute;n.
    txt =! strcmp(tipus,"txt");
    html =! strcmp(tipus,"html");

    ofstream fitxersortidatxt("mayor.txt"); /// Creamos los archivos de salida.
    ofstream fitxersortidahtml("mayor.htm");

    if (!fitxersortidatxt) {
        txt = 0; /// Verificamos que se hayan creado correctamente los archivos.
    } // end if
    if (!fitxersortidahtml) {
        html = 0; /// Se puede mejorar el tratamiento de errores.
    } // end if
    /// S&oaqcute;lo continuamos si hemos podido crear alg&uacute;n archivo.
    if (txt | html) {
        if (txt) {
            /// Presentaci&oacute;n txt.
            fitxersortidatxt.setf(ios::fixed)
                ;
            fitxersortidatxt.precision(2);
            fitxersortidatxt << "                                    MAYOR \n" ;
            fitxersortidatxt << "Fecha Inicial: " << finicial.toAscii().constData() << "   Fecha Final: " << ffinal.toAscii().constData() << endl;
            fitxersortidatxt << "_________________________________________________________________________________________________________\n";
        } // end if
        if (html) {
            /// Presentaci&oacute;n html.
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
            fitxersortidahtml << "<tr><td colspan=\"6\" class=periodemajor> Fecha Inicial: " << finicial.toAscii().constData() << " -  Fecha Final: " << ffinal.toAscii().constData() << "<hr></td></tr>\n\n";
        } // end if
        m_companyact->begin();
        cursoraux = m_companyact->cargacuentascodigo(-1,cinicial, cfinal);
        m_companyact->commit();
        while (!cursoraux->eof()) {
            idcuenta = atoi(cursoraux->valor("idcuenta").toAscii());
            m_companyact->begin();
            cursoraux1 = m_companyact->cargaapuntesctafecha(idcuenta, finicial.toAscii(), ffinal.toAscii());
            m_companyact->commit();
            if (!cursoraux1->eof()) {
                activo = strcmp((char *) cursoraux->valor("activo").toAscii().constData(), "f");
                if (txt) {
                    fitxersortidatxt << "\n\n" << setw(12) << cursoraux->valor("codigo").toAscii().constData() << setw(50) << cursoraux->valor("descripcion").toAscii().constData() << endl;
                    if (activo) {
                        fitxersortidatxt << " Cuenta de activo";
                    } else {
                        fitxersortidatxt << " Cuenta de pasivo";
                    } // end if
                } // end if
                if (html) {
                    fitxersortidahtml << "<tr><td colspan=\"6\" class=comptemajor>" << cursoraux->valor("codigo").toAscii().constData() << " "<< cursoraux->valor("descripcion").toAscii().constData() << "</td></tr>\n";
                    if (activo) {
                        fitxersortidahtml << "<tr><td colspan=\"6\" class=tipuscomptemajor> Cuenta de activo </td></tr>\n";
                    } else {
                        fitxersortidahtml << "<tr><td colspan=\"6\" class=tipuscomptemajor> Cuenta de pasivo </td></tr>\n";
                    } // end if
                } // end if
                m_companyact->begin();
                cursoraux2 = m_companyact->cargasaldoscuentafecha(idcuenta, (char *)finicial.toAscii().constData());
                m_companyact->commit();
                if (!cursoraux2->eof()) {
                    debeinicial = atof(cursoraux2->valor("tdebe").toAscii());
                    haberinicial = atof(cursoraux2->valor("thaber").toAscii());
                    if (activo) {
                        saldoinicial = debeinicial - haberinicial;
                    } else {
                        saldoinicial = haberinicial-debeinicial;
                    } // end if

                    if (txt) {
                        /// Presentaci&oacute;n txt.
                        fitxersortidatxt << "\nAsiento  Fecha   Contrapartida   Descripcion                          Debe         Haber         Saldo\n";
                        fitxersortidatxt << "                                                 SUMAS ANTERIORES...   " << setw(10) << debeinicial << setw(10) << haberinicial << setw(10) << saldoinicial << endl;
                        fitxersortidatxt << "_________________________________________________________________________________________________________\n";
                    } // end if

                    if (html) {
                        /// Presentaci&oacute;n html.
                        fitxersortidahtml << "<tr><td class=titolcolumnamajor> Asiento </td><td class=titolcolumnamajor> Fecha </td><td class=titolcolumnamajor> Contrapartida </td><td class=titolcolumnamajor> Descripcion </td><td class=titolcolumnamajor> Debe </td><td class=titolcolumnamajor> Haber </td><td class=titolcolumnamajor> Saldo </td></tr>\n";
                        fitxersortidahtml << "<tr><td></td><td></td><td></td><td class=sumamajor> Sumes anteriors...</td><td class=dosdecimals> " << debeinicial << " </td><td class=dosdecimals> " << haberinicial << " </td><td class=dosdecimals> " << saldoinicial << "</td><td>\n";
                    } // end if

                    saldo = saldoinicial;
                    debefinal = debeinicial;
                    haberfinal = haberinicial;

                    for (; !cursoraux1->eof(); cursoraux1->siguienteregistro()) {
                        idasiento = atoi(cursoraux1->valor("idasiento").toAscii());
                        contrapartida = atoi(cursoraux1->valor("contrapartida").toAscii());
                        m_companyact->begin();
                        cursoraux3 = m_companyact->cargacuenta(contrapartida);
                        codcontrapartida = cursoraux3->valor("codigo").toAscii().constData();
                        m_companyact->commit();
                        debe = atof(cursoraux1->valor("debe").toAscii());
                        haber = atof(cursoraux1->valor("haber").toAscii());
                        if (activo) {
                            saldo += debe - haber;
                        } else {
                            saldo += haber - debe;
                        } // end if
                        debefinal += debe;
                        haberfinal += haber;
                        cad = cursoraux1->valor("fecha").toAscii().constData();
                        /// Presentaci&oacute;n txt.
                        if (txt) {
                            fitxersortidatxt <<  setw(5) << idasiento << setw(14) << cad.substr(0,10).c_str() << setw(10) << codcontrapartida << "  " << setw(40)  << setiosflags(ios::left) << cursoraux1->valor("conceptocontable").toAscii().constData() << setw(10) << resetiosflags(ios::left) << debe << setw(10) << haber << setw(10) << saldo << endl;
                        } // end if
                        /// Presentaci&oacute;n html.
                        if (html) {
                            fitxersortidahtml << " <tr><td class=assentamentmajor> " << idasiento << " </td><td> " << cad.substr(0,10).c_str() << " </td><td class=contrapartidamajor> " << codcontrapartida << " </td><td> " << cursoraux1->valor("conceptocontable").toAscii().constData() << " </td><td class=dosdecimals> " << debe << " </td><td class=dosdecimals> " << haber << " </td><td class=dosdecimals> " << saldo << " </td></tr>\n ";
                        } // end if
                        cursoraux3->cerrar();
                    } // end for

                    if (activo) {
                        saldofinal = debefinal - haberfinal;
                    } else {
                        saldofinal = haberfinal - debefinal;
                    } // end if
                    if (txt) {
                        /// Presentaci&oacute;n txt.
                        fitxersortidatxt << "                                               __________________________________________________________\n";
                        fitxersortidatxt << "                                                  TOTAL SUBCUENTA...   " << setw(10) << debefinal << setw(10) << haberfinal << setw(10) << saldofinal << endl;
                    } // end if
                    if (html) {
                        /// Presentaci&oacute;n html.
                        fitxersortidahtml << "<tr><td></td><td></td><td></td><td class=sumamajor> Total subcompte... </td><td class=dosdecimals>" << debefinal << " </td><td class=dosdecimals> " << haberfinal << " </td><td class=dosdecimals> " << saldofinal << "</td></tr>\n\n";
                    } // end if
                    cursoraux2->cerrar();
                } // end if
            }
            cursoraux1->cerrar();
            cursoraux->siguienteregistro();
        } // end while
        if (html) {
            /// Presentaci&oacute;n html.
            fitxersortidahtml << "\n</table></body></html>\n";
        } // end if
        m_companyact->commit();
        delete cursoraux;
    }
    if (txt) {
        /// Presentaci&oacute;n txt.
        fitxersortidatxt.close();
        QString cad = confpr->valor(CONF_EDITOR) + " mayor.txt";
        system(cad.toAscii().constData());
    } // end if
    if (html) {
        /// Presentaci&oacute;n html.
        fitxersortidahtml.close();
        QString cad = confpr->valor(CONF_NAVEGADOR) + " mayor.html";
        system(cad.toAscii().constData());
    } // end if
}

