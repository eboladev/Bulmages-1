/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#include <QFile>
#include <QTextStream>

#include "fichabf.h"
#include "plugins.h"


class Fixed;


FichaBf::FichaBf(company *comp, QWidget *parent, Qt::WFlags f)
        : Ficha(parent, f), DBRecord(comp) {
    _depura("FichaBf::FichaBf", 0);
    m_listalineas = NULL;
    m_listadescuentos = NULL;
    m_companyact = comp;
    _depura("END FichaBf::FichaBf", 0);
}


FichaBf::~FichaBf() {
    _depura("FichaBf::~FichaBf", 0);
    _depura("END FichaBf::~FichaBf", 0);
}


/** Calcula los totales de factura, descuentos e impuestos y invoca al metodo de pintaTotales para
    que se pinten dichos valores en la pantalla.
*/
void FichaBf::calculaypintatotales() {
    _depura("FichaBf::calculaypintatotales", 0);
    base basesimp;
    base basesimpreqeq;
    SDBRecord *linea;
    /// Impresion de los contenidos.
    QString l;
    Fixed irpf("0");

    cursor2 *cur = m_companyact->cargacursor("SELECT * FROM configuracion WHERE nombre = 'IRPF'");
    if (!cur->eof()) {
        irpf = Fixed(cur->valor("valor"));
    } // end if
    delete cur;

    if (exists("idproveedor")) {
        cur = m_companyact->cargacursor("SELECT irpfproveedor FROM proveedor WHERE idproveedor="+DBvalue("idproveedor"));
        if (!cur->eof()) {
            irpf = Fixed(cur->valor("irpfproveedor"));
        } // end if
        delete cur;
    } // end if

    for (int i = 0; i < m_listalineas->rowCount(); ++i) {
        linea = m_listalineas->lineaat(i);
        Fixed cant(linea->DBvalue("cant" + m_listalineas->tableName()).toAscii().constData());
        Fixed pvpund(linea->DBvalue("pvp" + m_listalineas->tableName()).toAscii().constData());
        Fixed desc1(linea->DBvalue("descuento" + m_listalineas->tableName()).toAscii().constData());
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        basesimp[linea->DBvalue("iva" + m_listalineas->tableName())] = basesimp[linea->DBvalue("iva"+m_listalineas->tableName())] + base;
        basesimpreqeq[linea->DBvalue("reqeq" + m_listalineas->tableName())] = basesimpreqeq[linea->DBvalue("reqeq" + m_listalineas->tableName())] + base;
    } // end for

    Fixed basei("0.00");
    base::Iterator it;
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        basei = basei + it.value();
    } // end for

    /// Impresion de los descuentos.
    Fixed porcentt("0.00");
    SDBRecord *linea1;
    if (m_listadescuentos->rowCount()) {
        for (int i = 0; i < m_listadescuentos->rowCount(); ++i) {
            linea1 = m_listadescuentos->lineaat(i);
            Fixed propor(linea1->DBvalue("proporcion" + m_listadescuentos->tableName()).toAscii().constData());
            porcentt = porcentt + propor;
        } // end for
    } // end if

    /// Calculamos el total de base imponible.
    Fixed totbaseimp("0.00");
    Fixed parbaseimp("0.00");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        if (porcentt > Fixed("0.00") ) {
            parbaseimp = it.value() - it.value() * porcentt / 100;
        } else {
            parbaseimp = it.value();
        } // end if
        totbaseimp = totbaseimp + parbaseimp;
    } // end for

    /// Calculamos el total de IVA.
    Fixed totiva("0.00");
    Fixed pariva("0.00");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        Fixed piva(it.key().toAscii().constData());
        if (porcentt > Fixed("0.00")) {
            pariva = (it.value() - it.value() * porcentt / 100) * piva / 100;
        } else {
            pariva = it.value() * piva / 100;
        } // end if
        totiva = totiva + pariva;
    } // end for

    /// Calculamos el total de recargo de equivalencia.
    Fixed totreqeq("0.00");
    Fixed parreqeq("0.00");
    for (it = basesimpreqeq.begin(); it != basesimpreqeq.end(); ++it) {
        Fixed preqeq(it.key().toAscii().constData());
        if (porcentt > Fixed("0.00")) {
            parreqeq = (it.value() - it.value() * porcentt / 100) * preqeq / 100;
        } else {
            parreqeq = it.value() * preqeq / 100;
        } // end if
        totreqeq = totreqeq + parreqeq;
    } // end for

    Fixed totirpf = totbaseimp * irpf / 100;
    pintatotales(totiva, totbaseimp, totiva + totbaseimp + totreqeq - totirpf, basei * porcentt / 100, totirpf, totreqeq);
    _depura("FichaBf::calculaypintatotales", 0);
}


void FichaBf::imprimir() {
    _depura("FichaBf::imprimir", 0);
    /// Disparamos los plugins
    int res = g_plugins->lanza("FichaBf_imprimir", this);
    if (res != 0) {
        return;
    } // end if
    base basesimp;
    base basesimpreqeq;
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + m_tablename + ".rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + m_tablename + ".rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";

    Fixed irpf("0");

    cursor2 *cur = m_companyact->cargacursor("SELECT * FROM configuracion WHERE nombre = 'IRPF'");
    if (!cur->eof()) {
        irpf = Fixed(cur->valor("valor"));
    } // end if
    delete cur;

    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system (archivo.toAscii().constData());
    /// Copiamos el logo
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#endif

    system(archivologo.toAscii().constData());
    QFile file;
    file.setFileName(archivod);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt = "";

    /// Linea de totales del Presupuesto.
    QString SQLQuery = "SELECT * FROM cliente WHERE idcliente = " + DBvalue("idcliente");
    cur = m_companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        buff.replace("[dircliente]", cur->valor("dircliente"));
        buff.replace("[poblcliente]", cur->valor("poblcliente"));
        buff.replace("[telcliente]", cur->valor("telcliente"));
        buff.replace("[nomcliente]", cur->valor("nomcliente"));
        buff.replace("[cifcliente]", cur->valor("cifcliente"));
        buff.replace("[idcliente]", cur->valor("idcliente"));
        buff.replace("[cpcliente]", cur->valor("cpcliente"));
    } // end if
    delete cur;

    if (exists("num" + m_tablename))
        buff.replace("[num" + m_tablename + "]", DBvalue("num" + m_tablename));
    if (exists("f" + m_tablename))
        buff.replace("[f" + m_tablename + "]", DBvalue("f" + m_tablename));
    if (exists("venc" + m_tablename))
        buff.replace("[venc" + m_tablename + "]", DBvalue("venc" + m_tablename));
    if (exists("contact" + m_tablename))
        buff.replace("[contact" + m_tablename + "]", DBvalue("contact" + m_tablename));
    if (exists("tel" + m_tablename))
        buff.replace("[tel" + m_tablename+"]", DBvalue("tel" + m_tablename));
    if (exists("coment" + m_tablename))
        buff.replace("[coment" + m_tablename+"]", DBvalue("coment" + m_tablename));
    if (exists("desc" + m_tablename))
        buff.replace("[desc" + m_tablename + "]", DBvalue("desc" + m_tablename));
    if (exists("ref" + m_tablename))
        buff.replace("[ref" + m_tablename + "]", DBvalue("ref" + m_tablename));
    if (exists("codigoserie_" + m_tablename) )
        buff.replace("[codigoserie_" + m_tablename+"]", DBvalue("codigoserie_" + m_tablename));
    if (exists("fecha" + m_tablename) )
        buff.replace("[fecha" + m_tablename + "]", DBvalue("fecha" + m_tablename));
    /// Impresion de la tabla de contenidos.
    QString l;

    /// Contador que sirve para poner lineas de mas en caso de que sea preciso.
    SDBRecord *linea;
    for (int i = 0; i < (m_listalineas->rowCount() - 1); ++i) {
        linea = m_listalineas->lineaat(i);
        Fixed base = Fixed(linea->DBvalue("cant" + m_listalineas->tableName()).toAscii().constData()) * Fixed(linea->DBvalue("pvp"+m_listalineas->tableName()).toAscii().constData());
        basesimp[linea->DBvalue("iva"+m_listalineas->tableName())] = basesimp[linea->DBvalue("iva"+m_listalineas->tableName())] + base - base * Fixed(linea->DBvalue("descuento" + m_listalineas->tableName()).toAscii().constData()) / 100;
        basesimpreqeq[linea->DBvalue("reqeq" + m_listalineas->tableName())] = basesimpreqeq[linea->DBvalue("reqeq"+m_listalineas->tableName())] + base - base * Fixed(linea->DBvalue("descuento" + m_listalineas->tableName()).toAscii().constData()) / 100;

        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "    <td>" + XMLProtect(linea->DBvalue("codigocompletoarticulo")) + "</td>\n";
        fitxersortidatxt += "    <td>" + XMLProtect(linea->DBvalue("desc" + m_listalineas->tableName())) + "</td>\n";
        fitxersortidatxt += "    <td>" + l.sprintf("%s", XMLProtect(linea->DBvalue("cant" + m_listalineas->tableName())).toAscii().constData()) + "</td>\n";
        fitxersortidatxt += "    <td>" + l.sprintf("%s", XMLProtect(linea->DBvalue("pvp" + m_listalineas->tableName())).toAscii().constData()) + "</td>\n";
        fitxersortidatxt += "    <td>" + l.sprintf("%s", XMLProtect(linea->DBvalue("descuento" + m_listalineas->tableName())).toAscii().constData()) + " %</td>\n";
        fitxersortidatxt += "    <td>" + l.sprintf("%s", (base - base * Fixed(linea->DBvalue("descuento" + m_listalineas->tableName())) / 100).toQString().toAscii().constData()) + "</td>\n";
        fitxersortidatxt += "</tr>";
    } // end for

    buff.replace("[story]", fitxersortidatxt);

    Fixed basei("0.00");
    base::Iterator it;
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        basei = basei + it.value();
    } // end for

    /// Impresion de los descuentos.
    fitxersortidatxt = "";
    Fixed porcentt("0.00");
    SDBRecord *linea1;
    if (m_listadescuentos->rowCount() - 1) {
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm, 2cm, 3cm\" repeatRows=\"1\">\n";
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "    <td>" + QApplication::translate("Presupuesto", "Descuento") + "</td>\n";
        fitxersortidatxt += "    <td>" + QApplication::translate("Presupuesto", "Porcentaje") + "</td>\n";
        fitxersortidatxt += "    <td>" + QApplication::translate("Presupuesto", "Total") + "</td>\n";
        fitxersortidatxt += "</tr>\n";
        for (int i = 0; i < (m_listadescuentos->rowCount() - 1); ++i) {
            linea1 = m_listadescuentos->lineaat(i);
            porcentt = porcentt + Fixed(linea1->DBvalue("proporcion" + m_listadescuentos->tableName()).toAscii().constData());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "    <td>" + XMLProtect(linea1->DBvalue("concept" + m_listadescuentos->tableName())) + "</td>\n";
            fitxersortidatxt += "    <td>" + l.sprintf("%s", linea1->DBvalue("proporcion" + m_listadescuentos->tableName()).toAscii().constData()) + " %</td>\n";
            fitxersortidatxt += "    <td>" + l.sprintf("-%s", (Fixed(linea1->DBvalue("proporcion" + m_listadescuentos->tableName())) * basei / 100).toQString().toAscii().constData()) + "</td>\n";
            fitxersortidatxt += "</tr>";
        } // end for
        fitxersortidatxt += "</blockTable>\n";
    } // end if
    buff.replace("[descuentos]", fitxersortidatxt);

    /// Impresion de los totales.
    fitxersortidatxt = "";
    QString tr1 = ""; /// Rellena el primer tr de titulares.
    QString tr2 = ""; /// Rellena el segundo tr de cantidades.
    fitxersortidatxt += "<blockTable style=\"tabladescuento\" >\n";
    Fixed totbaseimp("0.00");
    Fixed parbaseimp("0.00");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        if (porcentt > 0) {
            parbaseimp = it.value() - it.value() * porcentt / 100;
        } else {
            parbaseimp = it.value();
        } // end if
        totbaseimp = totbaseimp + parbaseimp;
        tr1 += "    <td>" + QApplication::translate("Presupuesto", "Base ") + " " + XMLProtect(it.key()) + " %</td>\n";
        tr2 += "    <td>" + l.sprintf(" %s ", parbaseimp.toQString().toAscii().constData()) + "</td>\n";
    } // end for

    /// Impresion de los IVAS.
    Fixed totiva("0.0");
    Fixed pariva("0.0");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        if (porcentt > 0) {
            pariva = (it.value() - it.value() * porcentt / 100) * Fixed(it.key()) / 100;
        } else {
            pariva = it.value() * Fixed(it.key()) / 100;
        } // end if
        totiva = totiva + pariva;
        tr1 += "    <td>" + QApplication::translate("Presupuesto", "I.V.A. ") + " " + XMLProtect(it.key()) + " %</td>\n";
        tr2 += "    <td>" + l.sprintf(" %s ", pariva.toQString().toAscii().constData()) + "</td>\n";
    } // end for

    /// Impresion de los Recargos de Equivalencia.
    Fixed totreqeq("0.0");
    Fixed parreqeq("0.0");
    for (it = basesimpreqeq.begin(); it != basesimpreqeq.end(); ++it) {
        if (porcentt > 0) {
            parreqeq = (it.value() - it.value() * porcentt / 100) * Fixed(it.key()) / 100;
        } else {
            parreqeq = it.value() * Fixed(it.key()) / 100;
        } // end if
        totreqeq = totreqeq + parreqeq;
        if (parreqeq > 0) {
            tr1 += "    <td>" + QApplication::translate("Presupuesto", "R.E. ") + " " + XMLProtect(it.key()) + " %</td>\n";
            tr2 += "    <td>" + l.sprintf(" %s ", parreqeq.toQString().toAscii().constData()) + "</td>\n";
        } // end if
    } // end for

    Fixed totirpf = totbaseimp * irpf / 100;
    if (totirpf > 0) {
        tr1 += "    <td>" + QApplication::translate("Presupuesto", "I.R.P.F  (-") + " " + XMLProtect(irpf.toQString()) + ") %</td>\n";
        tr2 += "    <td>" + l.sprintf(" %s ", totirpf.toQString().toAscii().constData()) + "</td>\n";
    } // end if

    tr1 += "    <td>" + QApplication::translate("Presupuesto", "Total ") + "</td>\n";
    tr2 += "    <td>" + l.sprintf(" %s ", (totiva+totbaseimp + totreqeq - totirpf).toQString().toAscii().constData()) + "</td>\n";
    fitxersortidatxt += "<tr>" + tr1 + "</tr><tr>" + tr2 + "</tr></blockTable>\n";
    buff.replace("[totales]", fitxersortidatxt);

    /// En la version para windows hay problemas con las imagenes,
    /// por eso de momento lo dejamos asi.
#ifndef WINDOWS
    //   buff.replace("[detallearticulos]", detalleArticulos());
#endif

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    } // end if
    invocaPDF(m_tablename);
    _depura("FichaBf::imprimir", 0);
}


/** Saca la ventana del workSpace.
    Este metodo es invocado desde la clase Ficha.
*/
int FichaBf::sacaWindow() {
    _depura("FichaBf::sacaWindow", 0);
    m_companyact->sacaWindow(this);
    _depura("END FichaBf::sacaWindow", 0);
    return 0;
}

