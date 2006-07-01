/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include "presupuesto.h"
#include "company.h"
#include "configuracion.h"
#include "fixed.h"
#include "funcaux.h"
#include "plugins.h"


typedef QMap<QString, Fixed> base;

presupuesto::presupuesto(company *comp) : DBRecord(comp) {
    companyact = comp;
    setDBTableName("presupuesto");
    setDBCampoId("idpresupuesto");
    addDBCampo("idpresupuesto", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("presupuesto", "ID presupuesto"));
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("presupuesto", "ID cliente"));
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("presupuesto", "ID almacen"));
    addDBCampo("numpresupuesto", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("presupuesto", "Numero de presupuesto"));
    addDBCampo("fpresupuesto", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("presupuesto", "Fecha de creacion"));
    addDBCampo("vencpresupuesto", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("presupuesto", "Fecha de vencimiento"));
    addDBCampo("contactpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("presupuesto", "Persona de contacto"));
    addDBCampo("telpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("presupuesto", "Numero de telefono"));
    addDBCampo("comentpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("presupuesto", "Comentarios"));
    addDBCampo("procesadopresupuesto", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("presupuesto", "Presupuesto procesado"));
    addDBCampo("descpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("presupuesto", "Descuento"));
    addDBCampo("refpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("presupuesto", "Referencia"));
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("presupuesto", "ID forma de pago"));
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("presupuesto", "ID trabajador"));
}


presupuesto::~presupuesto() {}


int presupuesto::borrar() {
    if (DBvalue("idpresupuesto") != "") {
        companyact->begin();
        listalineas->borrar();
        listadescuentos->borrar();
        int error = companyact->ejecuta("DELETE FROM presupuesto WHERE idpresupuesto=" + DBvalue("idpresupuesto"));
        if (error) {
            companyact->rollback();
            return -1;
        } // end if
        companyact->commit();
    } // end if
    return 0;
}


void presupuesto::vaciaPresupuesto() {
    DBclear();
}


void presupuesto::pintaPresupuesto() {
    pintaidcliente(DBvalue("idcliente"));
    pintaIdAlmacen(DBvalue("idalmacen"));
    pintaNumPresupuesto(DBvalue("numpresupuesto"));
    pintaFPresupuesto(DBvalue("fpresupuesto"));
    pintaVencPresupuesto(DBvalue("vencpresupuesto"));
    pintaContractPresupuesto(DBvalue("contactpresupuesto"));
    pintaTelPresupuesto(DBvalue("telpresupuesto"));
    pintaComentPresupuesto(DBvalue("comentpresupuesto"));
    pintaprocesadopresupuesto(DBvalue("procesadopresupuesto"));
    pintadescpresupuesto(DBvalue("descpresupuesto"));
    pintarefpresupuesto(DBvalue("refpresupuesto"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintaidalmacen(DBvalue("idalmacen"));
    pintaidtrabajador(DBvalue("idtrabajador"));
    // Pinta el subformulario de detalle del presupuesto.
    //    listalineas->pintalistlinpresupuesto();
    // Pinta el subformulario de descuentos del presupuesto
    //    listadescuentos->pintaListDescuentoPresupuesto();
    calculaypintatotales();
}


/// Esta funcion carga un presupuesto.
int presupuesto::cargar(QString idbudget) {
    int error = 0;
    QString query = "SELECT * FROM presupuesto WHERE idpresupuesto=" + idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (cur->error())
        error = 1;
    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;

    listalineas->cargar(idbudget);
    listadescuentos->cargar(idbudget);

    /// Tratamiento de excepciones.
    if (error) {
        _depura("Error en la carga del presupuesto\n", 2);
        return 1;
    } // end if

    pintaPresupuesto();
    return 0;
}


int presupuesto::guardar() {
    _depura("presupuesto::guardar", 0);
    QString id;
    companyact->begin();
    try {
        DBsave(id);
        setidpresupuesto(id);
        listalineas->guardar();
        listadescuentos->guardar();
        companyact->commit();
        _depura("END presupuesto::guardar", 0);
        return 0;
    } // end try

    catch(...) {
        _depura("Error guardando, se cancela la operacion", 1);
        companyact->rollback();
        return -1;
    } // end catch
}


QString presupuesto::detalleArticulos() {
    QString texto = "";

    cursor2 *cur=companyact->cargacursor("SELECT * FROM lpresupuesto LEFT JOIN articulo ON lpresupuesto.idarticulo = articulo.idarticulo WHERE presentablearticulo AND idpresupuesto=" + DBvalue("idpresupuesto"));
    int i = 0;
    while(!cur->eof()) {
        i = !i;

        if (i) {
            texto += "<blockTable style=\"tabladetalle1\" colWidths=\"5cm, 8cm\" rowHeights=\"5.5cm\">\n";
        } else {
            texto += "<blockTable style=\"tabladetalle2\" colWidths=\"8cm, 5cm\" rowHeights=\"5.5cm\">\n";
        } // end if

        texto += "<tr>\n";

        if (i) {
            texto += "<td><h1>" + cur->valor("nomarticulo") + "</h1>";
            texto += "<para><pre>" + cur->valor("obserarticulo") + "</pre></para></td>\n";
        } // end if

        QString file = confpr->valor(CONF_DIR_IMG_ARTICLES) + cur->valor("codigocompletoarticulo") + ".jpg";
        QFile f(file);
        if (f.exists()) {
            texto += "        <td><illustration x=\"0\" y=\"0\" height=\"5cm\">\n"
                     "            <image file=\"" + confpr->valor(CONF_DIR_IMG_ARTICLES) + cur->valor("codigocompletoarticulo") + ".jpg\" x=\"0\" y=\"0\" height=\"5cm\"/>\n"
                     "            </illustration></td>\n";
        } else {
            texto += "<td></td>\n";
        }

        if (!i) {
            texto += "<td><h1>" + cur->valor("nomarticulo") + "</h1>";
            texto += "<para><pre>" + cur->valor("obserarticulo") + "</pre></para></td>\n";
        } // end if

        texto += "</tr>\n";
        texto += "</blockTable>";
        cur->siguienteregistro();
    } // end while
    delete cur;
    return texto;
}


void presupuesto::imprimirPresupuesto() {
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
    int res = g_plugins->lanza("presupuesto_imprimirPresupuesto", this);
    if (res != 0)
        return;

    base basesimp;

    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS) + "presupuesto.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "presupuesto.rml";
    QString archivologo=confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system (archivo.ascii());

    /// Copiamos el logo
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#endif

    system (archivologo.ascii());

    QFile file;
    file.setName(archivod);
    file.open( QIODevice::ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt = "";
    /// Linea de totales del presupuesto.

    QString SQLQuery = "SELECT * FROM cliente WHERE idcliente=" + DBvalue("idcliente");
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        buff.replace("[dircliente]", cur->valor("dircliente"));
        buff.replace("[poblcliente]", cur->valor("poblcliente"));
        buff.replace("[telcliente]", cur->valor("telcliente"));
        buff.replace("[nomcliente]", cur->valor("nomcliente"));
        buff.replace("[cifcliente]", cur->valor("cifcliente"));
        buff.replace("[idcliente]", cur->valor("idcliente"));
        buff.replace("[cpcliente]", cur->valor("cpcliente"));
    } // end if
    delete cur;

    buff.replace("[numpresupuesto]", DBvalue("numpresupuesto"));
    buff.replace("[fpresupuesto]", DBvalue("fpresupuesto"));
    buff.replace("[vencpresupuesto]", DBvalue("vencpresupuesto"));
    buff.replace("[contactpresupuesto]", DBvalue("contactpresupuesto"));
    buff.replace("[telpresupuesto]", DBvalue("telpresupuesto"));
    buff.replace("[comentpresupuesto]", DBvalue("comentpresupuesto"));
    buff.replace("[descpresupuesto]", DBvalue("descpresupuesto"));
    buff.replace("[refpresupuesto]", DBvalue("refpresupuesto"));

    /// Impresion de la tabla de contenidos.
    fitxersortidatxt += "<blockTable style=\"tablacontenido\" colWidths=\"1.75cm, 8.75cm, 1.2cm, 1.5cm, 1.8cm, 2.25cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "        <td>" + QApplication::translate("presupuesto", "Codigo") + "</td>\n";
    fitxersortidatxt += "        <td>" + QApplication::translate("presupuesto", "Concepto") + "</td>\n";
    fitxersortidatxt += "        <td>" + QApplication::translate("presupuesto", "Und") + "</td>\n";
    fitxersortidatxt += "        <td>" + QApplication::translate("presupuesto", "Precio") + "</td>\n";
    fitxersortidatxt += "        <td>" + QApplication::translate("presupuesto", "Dto") + "</td>\n";
    fitxersortidatxt += "        <td>" + QApplication::translate("presupuesto", "Total") + "</td>\n";
    fitxersortidatxt += "</tr>\n";
    QString l;

    /// Contador que sirve para poner lineas de mas en caso de que sea preciso.
    int i = 0;
    SDBRecord *linea;
    for (int i = 0; i < listalineas->rowCount()-1; ++i) {
        linea = listalineas->lineaat(i);
        Fixed base = Fixed(linea->DBvalue("cantlpresupuesto").ascii()) * Fixed(linea->DBvalue("pvplpresupuesto").ascii());
        basesimp[linea->DBvalue("ivalpresupuesto")] = basesimp[linea->DBvalue("ivalpresupuesto")] + base - base * Fixed(linea->DBvalue("descuentolpresupuesto").ascii()) / 100;

        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>" + XMLProtect(linea->DBvalue("codigocompletoarticulo")) + "</td>\n";
        fitxersortidatxt += "	<td>" + XMLProtect(linea->DBvalue("desclpresupuesto")) + "</td>\n";
        fitxersortidatxt += "	<td>" + l.sprintf("%s",XMLProtect(linea->DBvalue("cantlpresupuesto")).ascii()) + "</td>\n";
        fitxersortidatxt += "	<td>" + l.sprintf("%s",XMLProtect(linea->DBvalue("pvplpresupuesto")).ascii()) + "</td>\n";
        fitxersortidatxt += "	<td>" + l.sprintf("%s",XMLProtect(linea->DBvalue("descuentolpresupuesto")).ascii()) + " %</td>\n";
        fitxersortidatxt += "	<td>" + l.sprintf("%s",(base - base * Fixed (linea->DBvalue("descuentolpresupuesto")) / 100).toQString().ascii()) + "</td>\n";
        fitxersortidatxt += "</tr>";
    } // end for

    while (i++ < 15)
        fitxersortidatxt += "<tr></tr>";

    fitxersortidatxt += "</blockTable>\n";
    buff.replace("[story]", fitxersortidatxt);

    Fixed basei("0.00");
    base::Iterator it;
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        basei = basei + it.data();
    } // end for

    /// Impresion de los descuentos.
    fitxersortidatxt = "";
    Fixed porcentt("0.00");
    SDBRecord *linea1;
    if (listadescuentos->rowCount()-1) {
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm, 2cm, 3cm\" repeatRows=\"1\">\n";
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "        <td>" + QApplication::translate("presupuesto", "Descuento") + "</td>\n";
        fitxersortidatxt += "        <td>" + QApplication::translate("presupuesto", "Porcentaje") + "</td>\n";
        fitxersortidatxt += "        <td>" + QApplication::translate("presupuesto", "Total") + "</td>\n";
        fitxersortidatxt += "</tr>\n";
        for (int i = 0; i < listadescuentos->rowCount()-1; ++i) {
            linea1 = listadescuentos->lineaat(i);
            porcentt = porcentt + Fixed(linea1->DBvalue("proporciondpresupuesto").ascii());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "        <td>" + XMLProtect(linea1->DBvalue("conceptdpresupuesto")) + "</td>\n";
            fitxersortidatxt += "        <td>" + l.sprintf("%s", linea1->DBvalue("proporciondpresupuesto").ascii()) + " %</td>\n";
            fitxersortidatxt += "        <td>" + l.sprintf("-%s", (Fixed(linea1->DBvalue("proporciondpresupuesto")) * basei / 100).toQString().ascii()) + "</td>\n";
            fitxersortidatxt += "</tr>";
        } // end for
        fitxersortidatxt += "</blockTable>\n";
    } // end if
    buff.replace("[descuentos]", fitxersortidatxt);

    /// Impresión de los totales.
    fitxersortidatxt = "";
    QString tr1 = ""; /// Rellena el primer tr de titulares
    QString tr2 = ""; /// Rellena el segundo tr de cantidades
    fitxersortidatxt += "<blockTable style=\"tablatotales\">\n";

    Fixed totbaseimp("0.00");
    Fixed parbaseimp("0.00");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        if (porcentt > 0) {
            parbaseimp = it.data()-it.data() * porcentt / 100;
        } else {
            parbaseimp = it.data();
        } // end if
        totbaseimp = totbaseimp + parbaseimp;
        tr1 += "        <td>" + QApplication::translate("presupuesto", "Base ") + XMLProtect(it.key()) + " %</td>\n";
        tr2 += "        <td>" + l.sprintf("%s", parbaseimp.toQString().ascii()) + "</td>\n";
    } // end for

    Fixed totiva("0.0");
    Fixed pariva("0.0");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        if (porcentt > 0) {
            pariva = (it.data() - it.data()*porcentt / 100) * Fixed(it.key()) / 100;
        } else {
            pariva = it.data() * Fixed(it.key()) / 100;
        } // end if
        totiva = totiva + pariva;
        tr1 += "        <td>" + QApplication::translate("presupuesto", "I.V.A. ") + XMLProtect(it.key()) + " %</td>\n";
        tr2 += "        <td>" + l.sprintf("%s", pariva.toQString().ascii()) + "</td>\n";
    } // end for
    tr1 += "        <td>" + QApplication::translate("presupuesto", "Total ") + "</td>\n";
    tr2 += "        <td>" + l.sprintf("%s", (totiva+totbaseimp).toQString().ascii()) + "</td>\n";
    fitxersortidatxt += "<tr>" + tr1 + "</tr><tr>" + tr2 + "</tr></blockTable>\n";
    buff.replace("[totales]", fitxersortidatxt);

    /// En la version para windows hay problemas con las imagenes,
    /// por eso de momento lo dejamos asi.

#ifndef WINDOWS

    buff.replace("[detallearticulos]", detalleArticulos());
#endif

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    }
    QString cadsys;
    invocaPDF("presupuesto");
}


void presupuesto::calculaypintatotales() {
    _depura("calculaypintatotales \n", 0);

    /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
    int res = g_plugins->lanza("presupuesto_calculaypintatotales", this);
    if (res != 0)
        return;

    base basesimp;
    SDBRecord *linea;
    /// Impresion de los contenidos.
    QString l;
    for (int i = 0; i < listalineas->rowCount(); ++i) {
        linea = listalineas->lineaat(i);
        Fixed cant(linea->DBvalue("cantlpresupuesto").ascii());
        Fixed pvpund(linea->DBvalue("pvplpresupuesto").ascii());
        Fixed desc1(linea->DBvalue("descuentolpresupuesto").ascii());
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        Fixed val("0.00");
        val = basesimp[linea->DBvalue("ivalpresupuesto")];
        val = val + base;
        QString lin = linea->DBvalue("ivalpresupuesto");
        basesimp[lin] = val;
    } // end for
    Fixed basei("0.00");
    base::Iterator it;
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        basei = basei + it.data();
    } // end for

    /// Impresion de los descuentos.
    Fixed porcentt("0.00");
    SDBRecord *linea1;
    if (listadescuentos->rowCount()) {
        for (int i = 0; i < listadescuentos->rowCount(); ++i) {
            linea1 = listadescuentos->lineaat(i);
            Fixed propor(linea1->DBvalue("proporciondpresupuesto").ascii());
            porcentt = porcentt + propor;
        } // end for
    } // end if

    Fixed totbaseimp("0.00");
    Fixed parbaseimp("0.00");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        if (porcentt > Fixed("0.00") ) {
            parbaseimp = it.data() - it.data() * porcentt / 100;
        } else {
            parbaseimp = it.data();
        } // end if
        totbaseimp = totbaseimp + parbaseimp;
    } // end for

    Fixed totiva("0.00");
    Fixed pariva("0.00");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        Fixed piva(it.key().ascii());
        if (porcentt > Fixed("0.00")) {
            pariva = (it.data() - it.data() * porcentt / 100) * piva / 100;
        } else {
            pariva = it.data() * piva / 100;
        } // end if
        totiva = totiva + pariva;
    } // end for
    pintatotales(totiva, totbaseimp, totiva + totbaseimp, basei * porcentt / 100);
    _depura("END calculaypintatotales\n", 0);
}

