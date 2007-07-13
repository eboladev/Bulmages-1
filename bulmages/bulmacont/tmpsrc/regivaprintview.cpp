/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras                                    *
 *   tborras@conetxia.com                                                  *
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

#include "regivaprintview.h"
#include "empresa.h"

#include "modelo300.h"
#ifndef WIN32
#include <unistd.h>
#endif
#include <fstream>
#include "fixed.h"

using namespace std;

extern Mod300ps *modelo;


regivaprintview::regivaprintview(empresa *emp, QWidget *parent)
        : QDialog(parent) {
    _depura("regivaprintview::regivaprintview", 0);
    setupUi(this);
    fichero = NULL;
    empresaactual = emp;
    conexionbase = emp->bdempresa();
    _depura("END regivaprintview::regivaprintview", 0);
}


regivaprintview::~regivaprintview() {}


/// Se ha pulsado sobre el bot&oacute;n aceptar del formulario.
void regivaprintview::accept() {
    _depura("regivaprintview::accept", 0);
    if (radiotexto->isChecked())
        presentar("txt");
    if (radiohtml->isChecked())
        presentar("html");
    if (radioPS->isChecked()) {
        /// Lanza dialogo de generacion de modelo 300
        /// Deberia revisarse el Widget parent de la variable modelo
        /// (listivaview--->modelo, en vez de regivaprintview--->modelo)
        /// modelo->exec();
    } // end if
    _depura("END regivaprintview::accept", 0);
}


void regivaprintview::inicializa1(QString finicial1, QString ffinal1) {
    _depura("regivaprintview::inicializa1", 0);
    fechainicial1->setText(finicial1);
    fechafinal1->setText(ffinal1);
    _depura("END regivaprintview::inicializa1", 0);
}


/// Se ha pulsado sobre el bot&oacute;n aceptar del formulario.
void regivaprintview::presentar(char *tipus) {
    _depura("regivaprintview::presentar", 0);
#ifndef WIN32
    int txt, html, txtapren, htmlapren;
    int error;
    int pid;
    QString data;
    QString datahora;

    QString query;

    /// tipo de presentaci&oacute;n.
    txt =! strcmp(tipus, "txt");
    html =! strcmp(tipus, "html");
    txtapren =! strcmp(tipus, "txtapren");
    htmlapren =! strcmp(tipus, "htmlapren");

    /// Cogemos los valores del formulario.
    QString finicial = fechainicial1->text();
    QString ffinal = fechafinal1->text();

    if (txt | html) {
        char *argstxt[] = {"iva.txt", "iva.txt", NULL}; /// Presentaci&oacute;n txt normal.
        char *argshtml[]={"iva.html","iva.html",NULL}; /// Presentaci&oacute;n html normal.

        ofstream fitxersortidatxt(argstxt[0]); /// Creamos los archivos de salida.
        ofstream fitxersortidahtml(argshtml[0]);

        if (!fitxersortidatxt)
            txt = 0; ///  Verificamos que se hayan creado correctamente los archivos.
        if (!fitxersortidahtml)
            html = 0;  /// Se puede mejorar el tratamiento de errores.
        if (txt | html) { /// S&oacute;lo continuamos si hemos podido crear alg&uacute;n archivo.
            int num1;
            cursor2 *cursorapt;
            conexionbase->begin();
            query.sprintf("SELECT * FROM registroiva, cuenta, borrador, asiento  where cuenta.idcuenta = borrador.idcuenta AND borrador.idborrador = registroiva.idborrador AND asiento.idasiento = borrador.idasiento AND (cuenta.codigo LIKE '43%%' OR cuenta.codigo LIKE '600%%') AND borrador.fecha >= '%s' AND borrador.fecha <= '%s' ORDER BY asiento.ordenasiento", fechainicial1->text().toAscii().constData(), fechafinal1->text().toAscii().constData());
            fprintf(stderr, "%s\n", query.toAscii().constData());
            cursorapt = conexionbase->cargacursor(query, "mycursor");
            conexionbase->commit();
            /// Calculamos cuantos registros van a crearse y dimensionamos la tabla.
            num1 = cursorapt->numregistros();
            int hoja = 0;

            if (txt) {
                /// Presentaci&oacute;n txt normal.
                fitxersortidatxt.setf(ios::fixed)
                    ;
                fitxersortidatxt.precision(2);
                fitxersortidatxt << "                                        IVA Repercutido \n" ;
                fitxersortidatxt << "Fecha Inicial: " << finicial.toAscii().constData() <<
                "   Fecha Final: " << ffinal.toAscii().constData() << endl;
                fitxersortidatxt << "Asiento  Fecha   Cuenta   Descripción                        Base imponible    Factura \n" ;
                fitxersortidatxt << "__________________________________________________________________________________________________________\n";
            } // end if
            if (html) {
                /// Presentaci&oacute;n html normal.
                fitxersortidahtml.setf(ios::fixed)
                    ;
                fitxersortidahtml.precision(2);
                fitxersortidahtml << "<html>\n";
                fitxersortidahtml << "<head>\n";
                fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
                fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
                fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
                fitxersortidahtml << "  <title> IVA Repercutit </title>\n";
                fitxersortidahtml << "</head>\n";
                fitxersortidahtml << "<body>\n";
                fitxersortidahtml << "<table><tr><td colspan=\"10\" class=titoliva> IVA Repercutido <hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td colspan=\"10\" class periodeiva> Data Inicial: " << finicial.toAscii().constData() << " -  Data Final: " << ffinal.toAscii().constData() << "<hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td class=titolcolumnaiva> Asiento </td><td class=titolcolumnaiva> Data </td><td class=titolcolumnaiva> Cuenta </td><td class=titolcolumnaiva> Descripción</td><td class=titolcolumnaiva> Base Imponible </td><td class=titolcolumnaiva> % IVA </td><td class=titolcolumnaiva> Quota IVA </td><td class=titolcolumnaiva> Factura </td><td class=titolcolumnaiva> CIF </td><td class=titolcolumnaiva> Cuenta IVA </td></tr>\n";
            } // end if
            while (!cursorapt->eof()) {
                datahora = cursorapt->valor("fecha");
                data = datahora.mid(0, 10);
                int baseimp = cursorapt->valor("baseimp").replace(".", "").toInt();
                int total = baseimp *cursorapt->valor("baseimp").replace(".", "").toInt() / 10000;
                QString numberstr = QString::number(total);
                numberstr = numberstr.left(numberstr.length() - 2) + "." + numberstr.right(2);
                if (txt) {
                    /// Presentaci&oacute;n txt normal.
                    fitxersortidatxt << setw(10) << cursorapt->valor("ordenasiento").toAscii().data() << " " <<
                    data.toAscii().constData() << " " <<
                    cursorapt->valor("contrapartida").toAscii().constData() << " " << cursorapt->valor("descripcion").toAscii().constData() << " " << cursorapt->valor("baseimp").toAscii().constData() << " " <<
                    cursorapt->valor("iva").toAscii().constData() << " " <<
                    numberstr.toAscii().constData() <<" " <<
                    cursorapt->valor("factura").toAscii().constData() << " " <<
                    cursorapt->valor("cif").toAscii().constData() << " " <<
                    endl;
                } // end if
                if (html) {
                    /// Presentaci&oacute;n html normal.
                    fitxersortidahtml << "<tr><td class=assentamentiva>" << cursorapt->valor("idasiento").toAscii().constData() << "</td><td class=dataiva>" << data.toAscii().constData() << "</td><td class=contrapartidaiva>" << cursorapt->valor("contrapartida").toAscii().constData() << "</td><td class=descripcioiva>" << cursorapt->valor("descripcion").toAscii().constData() << "</td><td class=baseimponibleiva>" << cursorapt->valor("baseimp").toAscii().constData() << "</td><td class=tipusiva>" << cursorapt->valor("iva").toAscii().constData() << "</td><td class=quotaiva>" << numberstr.toAscii().constData() << "</td><td class=facturaiva>" << cursorapt->valor("factura").toAscii().constData() << "</td><td class=cifiva>" << cursorapt->valor("cif").toAscii().constData() << "</td></tr> \n";
                } // end if
                /// Calculamos la siguiente cuenta registro y finalizamos el bucle.
                cursorapt->siguienteregistro();
            } // end while
            /// Vaciamos el cursor de la base de datos.
            delete cursorapt;

            /// AHORA PONEMOS EL RESUMEN DEL IVA REPERCUTIDO.
            QString SQLQuery = "SELECT * FROM cuenta, tipoiva LEFT JOIN (SELECT idtipoiva, SUM(baseiva) AS tbaseiva FROM iva iva.idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE ffactura >='"+fechainicial1->text()+"' AND ffactura <='"+fechafinal1->text()+"' ) GROUP BY idtipoiva) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE tipoiva.idcuenta = cuenta.idcuenta AND cuenta.codigo LIKE '477%'";
            conexionbase->begin();
            cursor2* cur = conexionbase->cargacursor(SQLQuery, "elcursor");
            conexionbase->commit();
            int j = 0;
            Fixed tivar("0");
            Fixed tbaseimpr("0");
            while (!cur->eof()) {
                Fixed baseiva(cur->valor("tbaseiva").replace(".", "").toAscii().constData());
                Fixed porcent(cur->valor("porcentajetipoiva").replace(".", "").toAscii().constData());
                Fixed baseimp = baseiva * 1000000 / porcent;
                QString numberstr = baseimp.toQString();

                /// Pasamos al formato de representaci&oacute;n espa&ntilde;ol las
                /// cantidades a mostrar.
                QString iva = QString::number(cursorapt->valor("tbaseiva").toDouble(), 'f', 2);
                QString bi = QString::number(numberstr.toDouble(), 'f', 2);

                fitxersortidatxt << setiosflags( ios::left ) << setw(16) << cur->valor("nombretipoiva").toAscii().constData() << " IVA: ";
                fitxersortidatxt << resetiosflags( ios::left ) << setw(12) << iva.toAscii().constData() << " BI: ";
                fitxersortidatxt << setw(10) << bi.toAscii().constData() << endl;

                tivar = tivar+baseiva;
                tbaseimpr = tbaseimpr+baseimp;
                cur->siguienteregistro();
                j++;
            } // end while
            delete cur;

            conexionbase->begin();
            query.sprintf("SELECT *, (baseimp + iva) AS total, (iva / baseimp * 100)::INTEGER AS cuota FROM registroiva, cuenta, borrador, asiento  WHERE cuenta.idcuenta=borrador.idcuenta AND borrador.idborrador=registroiva.idborrador AND asiento.idasiento=borrador.idasiento AND (cuenta.codigo NOT LIKE '43%%' AND cuenta.codigo NOT LIKE '600%%') AND borrador.fecha >= '%s' AND borrador.fecha <= '%s' ORDER BY cuota, borrador.fecha", fechainicial1->text().toAscii().constData(), fechafinal1->text().toAscii().constData());
            fprintf(stderr, "%s\n", query.toAscii().constData());
            cursorapt = conexionbase->cargacursor(query, "mycursor");
            conexionbase->commit();
            /// Calculamos cuantos registros van a crearse y dimensionamos la tabla.
            num1 = cursorapt->numregistros();
            hoja = 0;
            if (txt) {
                /// Presentaci&oacute;n txt normal.
                fitxersortidatxt.setf(ios::fixed)
                    ;
                fitxersortidatxt.precision(2);
                fitxersortidatxt << "                                        IVA Soportado \n" ;
                fitxersortidatxt << "Asiento   Fecha     Cuenta   Descripción                        Subtotal Cuota   IVA       Total   Factura    CIF\n" ;
                fitxersortidatxt << "_______________________________________________________________________________________________________________________\n";
            }
            if (html) {
                /// Presentaci&oacute;n html normal.
                fitxersortidahtml.setf(ios::fixed)
                    ;
                fitxersortidahtml.precision(2);
                fitxersortidahtml << "<html>\n";
                fitxersortidahtml << "<head>\n";
                fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
                fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
                fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
                fitxersortidahtml << "  <title> IVA Soportat </title>\n";
                fitxersortidahtml << "</head>\n";
                fitxersortidahtml << "<body>\n";
                fitxersortidahtml << "<tr><td colspan=\"10\" class=titoliva> IVA Soportado <hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td class=titolcolumnaiva> Asiento </td><td class=titolcolumnaiva> Fecha </td><td class=titolcolumnaiva> Cuenta </td><td class=titolcolumnaiva> Descripcion</td><td class=titolcolumnaiva> Base Imponible </td><td class=titolcolumnaiva> % IVA </td><td class=titolcolumnaiva> Quota IVA </td><td class=titolcolumnaiva> Factura </td><td class=titolcolumnaiva> CIF </td><td class=titolcolumnaiva> Cuenta IVA </td></tr>\n";
            }
            while (!cursorapt->eof()) {
                datahora = cursorapt->valor("fecha");
                data = datahora.mid(0, 10);
                /// Pasamos al formato de representacion español las cantidades a mostrar.
                QString bi = QString::number(cursorapt->valor("baseimp").toDouble(), 'f', 2);
                QString iva = QString::number(cursorapt->valor("iva").toDouble(), 'f', 2);
                QString total = QString::number(cursorapt->valor("total").toDouble(), 'f', 2);
                /// Hacemos el c&aacute;lculo de la cuota (as&iacute; nos sirve para ver
                /// si se c&aacute;lculo el % correcto).
                // int calculo = round(cursorapt->valor("iva").toDouble()/cursorapt->valor("baseimp").toDouble()*100);
                // QString cuota = QString::number(calculo);
                QString cuota = QString::number(cursorapt->valor("cuota").toInt());
                /// Acumulamos los totales para al final poder escribirlos.
                if (txt) {
                    /// Presentaci&oacute;n txt normal.
                    fitxersortidatxt << setiosflags(ios::left) << setw(8) << cursorapt->valor("ordenasiento").toAscii().constData() << setw(12) << data.toAscii().constData() << setw(9) << cursorapt->valor("codigo").toAscii().constData() << setw(30) << cursorapt->valor("descripcion").left(30).toAscii().constData() << " " << resetiosflags( ios::left ) << setw(12) << bi.toAscii().constData() << " " << setw(3) << cuota.toAscii().constData() << "%" << setw(9) << iva.toAscii().constData() << setw(12) << total.toAscii().constData() << setiosflags( ios::left ) << "  " << setw(10) << cursorapt->valor("factura").right(8).toAscii().constData() <<  setw(10) << cursorapt->valor("cif").toAscii().constData() << endl;
                }
                if (html) {
                    /// Presentaci&oacute;n html normal.
                    fitxersortidahtml << "<tr><td class=assentamentiva>" << cursorapt->valor("ordenasiento").toAscii().constData() << "</td><td class=dataiva>" << data.toAscii().constData() << "</td><td class=contrapartidaiva>" << cursorapt->valor("contrapartida").toAscii().constData() << "</td><td class=descripcioiva>" << cursorapt->valor("descripcion").toAscii().constData() << "</td><td class=baseimponibleiva>" << cursorapt->valor("baseimp").toAscii().constData() << "</td><td class=tipusiva>" << cursorapt->valor("iva").toAscii().constData() << "</td><td class=quotaiva>" << total.toAscii().constData() << "</td><td class=facturaiva>" << cursorapt->valor("factura").toAscii().constData() << "</td><td class=cifiva>" << cursorapt->valor("cif").toAscii().constData() << "</td></tr> \n";
                }
                /// Calculamos la siguiente cuenta registro y finalizamos el bucle.
                cursorapt->siguienteregistro();
            } // end while
            /// Vaciamos el cursor de la base de datos.
            delete cursorapt;

            /// Ahora ponemos el resumen del IVA soportado.
            SQLQuery = "SELECT * FROM cuenta, tipoiva LEFT JOIN (SELECT idtipoiva, SUM(baseiva) AS tbaseiva FROM iva  WHERE iva.idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE ffactura >='"+fechainicial1->text()+"' AND ffactura <='"+fechafinal1->text()+"' ) GROUP BY idtipoiva) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE tipoiva.idcuenta = cuenta.idcuenta AND cuenta.codigo LIKE '472%'";
            conexionbase->begin();
            cur = conexionbase->cargacursor(SQLQuery, "elcursor");
            conexionbase->commit();
            j = 0;
            Fixed tivas("0");
            Fixed tbaseimps("0");
            while (!cur->eof()) {
                Fixed baseiva(cur->valor("tbaseiva").toAscii().constData());
                Fixed porcent(cur->valor("porcentajetipoiva").toAscii().constData());
                Fixed ivacalculado = baseiva * porcent / 100;
                QString numberstr = ivacalculado.toQString();

                /// Pasamos al formato de representaci&oacute;n espa&ntilde;ol las
                /// cantidades a mostrar.
                QString bi = QString::number(cur->valor("tbaseiva").toDouble(), 'f', 2);
                QString iva = QString::number(numberstr.toDouble(), 'f', 2);

                if (j == 0) {
                    fitxersortidatxt << endl;
                } // end if
                fitxersortidatxt << setiosflags(ios::left) << setw(16) << cur->valor("nombretipoiva").toAscii().constData() << " BI: ";
                fitxersortidatxt << resetiosflags( ios::left ) << setw(12) << bi.toAscii().constData() << " IVA: ";
                fitxersortidatxt << setw(10) << iva.toAscii().constData() << endl;

                tivas = tivas + ivacalculado;
                tbaseimps = tbaseimps + baseiva;
                cur->siguienteregistro();
                j++;
            } // end while
            delete cur;

            /// Ahora cerramos los ficheros.
            if (txt) {
                fitxersortidatxt.close();
                /// Presentaci&oacute;n txt normal.
                if ((pid=fork()) < 0) {
                    perror ("Fork failed");
                    exit(errno);
                } // end if
                if (!pid) {
                    error = execvp(confpr->valor(CONF_EDITOR).toAscii(), argstxt);
                } // end if
            } // end if
            if (html) {
                fitxersortidahtml << "\n</table>\n</body>\n</html>\n";
                fitxersortidahtml.close();
                /// Presentaci&oacute;n html normal.
                if ((pid = fork()) < 0) {
                    perror("Fork failed");
                    exit(errno);
                } // end if
                if (!pid) {
                    error = execvp(confpr->valor(CONF_NAVEGADOR).toAscii(),argshtml);
                } // end if
            } // end if
        }
    }

#endif
    _depura("END regivaprintview::presentar", 0);
}

