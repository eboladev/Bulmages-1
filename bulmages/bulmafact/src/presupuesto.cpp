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


Presupuesto::Presupuesto(company *comp, QWidget *parent) : FichaBf(comp, parent) {
    _depura("Presupuesto::Presupuesto", 0);
    setDBTableName("presupuesto");
    setDBCampoId("idpresupuesto");
    addDBCampo("idpresupuesto", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("Presupuesto", "ID Presupuesto"));
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("Presupuesto", "ID cliente"));
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("Presupuesto", "ID almacen"));
    addDBCampo("numpresupuesto", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Presupuesto", "Numero de Presupuesto"));
    addDBCampo("fpresupuesto", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Presupuesto", "Fecha de creacion"));
    addDBCampo("vencpresupuesto", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Presupuesto", "Fecha de vencimiento"));
    addDBCampo("contactpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Presupuesto", "Persona de contacto"));
    addDBCampo("telpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Presupuesto", "Numero de telefono"));
    addDBCampo("comentpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Presupuesto", "Comentarios"));
    addDBCampo("procesadopresupuesto", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("Presupuesto", "Presupuesto procesado"));
    addDBCampo("descpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Presupuesto", "Descuento"));
    addDBCampo("refpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Presupuesto", "Referencia"));
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Presupuesto", "ID forma de pago"));
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Presupuesto", "ID trabajador"));
    _depura("END Presupuesto::Presupuesto", 0);

}


Presupuesto::~Presupuesto() {
    _depura("Presupuesto::~Presupuesto", 0);
    _depura("END Presupuesto::~Presupuesto", 0);
}


int Presupuesto::borrar() {
    _depura("Presupuesto::borrar", 0);
    if (DBvalue("idpresupuesto") != "") {
        m_companyact->begin();
        m_listalineas->borrar();
        m_listadescuentos->borrar();
        int error = m_companyact->ejecuta("DELETE FROM presupuesto WHERE idpresupuesto = " + DBvalue("idpresupuesto"));
        if (error) {
            m_companyact->rollback();
            return -1;
        } // end if
        m_companyact->commit();
    } // end if
    _depura("END Presupuesto::borrar", 0);
    return 0;
}


void Presupuesto::vaciaPresupuesto() {
    _depura("Presupuesto::vaciaPresupuesto", 0);
    DBclear();
    _depura("END Presupuesto::vaciaPresupuesto", 0);
}


void Presupuesto::pintaPresupuesto() {
    _depura("Presupuesto::pintaPresupuesto", 0);
    pintaidcliente(DBvalue("idcliente"));
    pintaIdAlmacen(DBvalue("idalmacen"));
    pintaNumPresupuesto(DBvalue("numpresupuesto"));
    pintaFPresupuesto(DBvalue("fpresupuesto"));
    pintaVencPresupuesto(DBvalue("vencpresupuesto"));
    pintaContractPresupuesto(DBvalue("contactpresupuesto"));
    pintaTelPresupuesto(DBvalue("telpresupuesto"));
    pintaComentPresupuesto(DBvalue("comentpresupuesto"));
    pintaprocesadoPresupuesto(DBvalue("procesadopresupuesto"));
    pintadescPresupuesto(DBvalue("descpresupuesto"));
    pintarefPresupuesto(DBvalue("refpresupuesto"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintaidalmacen(DBvalue("idalmacen"));
    pintaidtrabajador(DBvalue("idtrabajador"));
    calculaypintatotales();
    _depura("END Presupuesto::pintaPresupuesto", 0);
}


/// Esta funcion carga un Presupuesto.
int Presupuesto::cargar(QString idbudget) {
    _depura("Presupuesto::cargar", 0);
    int error = 0;
    QString query = "SELECT * FROM presupuesto WHERE idpresupuesto = " + idbudget;
    cursor2 * cur= m_companyact->cargacursor(query);
    if (cur->error())
        error = 1;
    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;


    /// Tratamiento de excepciones.
    if (error) {
        _depura("Error en la carga del Presupuesto\n", 2);
        return 1;
    } // end if

    pintaPresupuesto();

    m_listalineas->cargar(idbudget);
    m_listadescuentos->cargar(idbudget);

    calculaypintatotales();

    _depura("END Presupuesto::cargar", 0);
    return 0;
}


int Presupuesto::guardar() {
    _depura("Presupuesto::guardar", 0);
    QString id;
    m_companyact->begin();
    try {
        DBsave(id);
        setidPresupuesto(id);
        m_listalineas->guardar();
        m_listadescuentos->guardar();
        m_companyact->commit();
        /// Hacemos una carga para recuperar el numero y la referencia.
        cargar(id);
        _depura("END Presupuesto::guardar", 0);
        return 0;
    } catch (...) {
        _depura("Error guardando. Se cancela la operacion", 0);
        m_companyact->rollback();
        throw -1;
    } // end try
}


QString Presupuesto::detalleArticulos() {
    _depura("Presupuesto::detalleArticulos", 0);
    QString texto = "";
    cursor2 *cur = m_companyact->cargacursor("SELECT * FROM lpresupuesto LEFT JOIN articulo ON lpresupuesto.idarticulo = articulo.idarticulo WHERE presentablearticulo AND idpresupuesto=" + DBvalue("idpresupuesto"));
    int i = 0;
    while (!cur->eof()) {
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
            texto += "    <td><illustration x=\"0\" y=\"0\" height=\"5cm\">\n"
                     "        <image file=\"" + confpr->valor(CONF_DIR_IMG_ARTICLES) + cur->valor("codigocompletoarticulo") + ".jpg\" x=\"0\" y=\"0\" height=\"5cm\"/>\n"
                     "        </illustration></td>\n";
        } else {
            texto += "<td></td>\n";
        } // end if
        if (!i) {
            texto += "<td><h1>" + cur->valor("nomarticulo") + "</h1>";
            texto += "<para><pre>" + cur->valor("obserarticulo") + "</pre></para></td>\n";
        } // end if
        texto += "</tr>\n";
        texto += "</blockTable>";
        cur->siguienteregistro();
    } // end while
    delete cur;
    _depura("END Presupuesto::detalleArticulos", 0);
    return texto;
}


void Presupuesto::generaRML() {
    _depura("Presupuesto::generaRML", 0);
    /// Disparamos los plugins con Presupuesto_imprimirPresupuesto.
    int res = g_plugins->lanza("Presupuesto_imprimirPresupuesto", this);
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
    QString SQLQuery = "SELECT * FROM cliente WHERE idcliente=" + DBvalue("idcliente");
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
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

    QString l;

    /// Contador que sirve para poner lineas de mas en caso de que sea preciso.
    SDBRecord *linea;
    for (int i = 0; i < (m_listalineas->rowCount() - 1); ++i) {
        linea = m_listalineas->lineaat(i);
        Fixed base = Fixed(linea->DBvalue("cantlpresupuesto").toAscii().constData()) * Fixed(linea->DBvalue("pvplpresupuesto").toAscii().constData());
        basesimp[linea->DBvalue("ivalpresupuesto")] = basesimp[linea->DBvalue("ivalpresupuesto")] + base - base * Fixed(linea->DBvalue("descuentolpresupuesto").toAscii().constData()) / 100;
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "    <td>" + XMLProtect(linea->DBvalue("codigocompletoarticulo")) + "</td>\n";
        fitxersortidatxt += "    <td>" + XMLProtect(linea->DBvalue("desclpresupuesto")) + "</td>\n";
        fitxersortidatxt += "    <td>" + l.sprintf("%s", XMLProtect(linea->DBvalue("cantlpresupuesto")).toAscii().constData()) + "</td>\n";
        fitxersortidatxt += "    <td>" + l.sprintf("%s", XMLProtect(linea->DBvalue("pvplpresupuesto")).toAscii().constData()) + "</td>\n";
        fitxersortidatxt += "    <td>" + l.sprintf("%s", XMLProtect(linea->DBvalue("descuentolpresupuesto")).toAscii().constData()) + " %</td>\n";
        fitxersortidatxt += "    <td>" + l.sprintf("%s", (base - base * Fixed(linea->DBvalue("descuentolpresupuesto")) / 100).toQString().toAscii().constData()) + "</td>\n";
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
            porcentt = porcentt + Fixed(linea1->DBvalue("proporciondpresupuesto").toAscii().constData());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "    <td>" + XMLProtect(linea1->DBvalue("conceptdpresupuesto")) + "</td>\n";
            fitxersortidatxt += "    <td>" + l.sprintf("%s", linea1->DBvalue("proporciondpresupuesto").toAscii().constData()) + " %</td>\n";
            fitxersortidatxt += "    <td>" + l.sprintf("-%s", (Fixed(linea1->DBvalue("proporciondpresupuesto")) * basei / 100).toQString().toAscii().constData()) + "</td>\n";
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
        tr1 += "    <td>" + QApplication::translate("Presupuesto", "Base ") +" "+ XMLProtect(it.key()) + " %</td>\n";
        tr2 += "    <td>" + l.sprintf(" %s ", parbaseimp.toQString().toAscii().constData()) + "</td>\n";
    } // end for

    Fixed totiva("0.0");
    Fixed pariva("0.0");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        if (porcentt > 0) {
            pariva = (it.value() - it.value() * porcentt / 100) * Fixed(it.key()) / 100;
        } else {
            pariva = it.value() * Fixed(it.key()) / 100;
        } // end if
        totiva = totiva + pariva;
        tr1 += "    <td>" + QApplication::translate("Presupuesto", "I.V.A. ") +" "+ XMLProtect(it.key()) + " %</td>\n";
        tr2 += "    <td>" + l.sprintf(" %s ", pariva.toQString().toAscii().constData()) + "</td>\n";
    } // end for
    tr1 += "    <td>" + QApplication::translate("Presupuesto", "Total ") + "</td>\n";
    tr2 += "    <td>" + l.sprintf(" %s ", (totiva+totbaseimp).toQString().toAscii().constData()) + "</td>\n";
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

    _depura("END Presupuesto::generaRML", 0);
}


void Presupuesto::imprimirPresupuesto() {
    _depura("Presupuesto::imprimirPresupuesto", 0);
    generaRML();
    invocaPDF("presupuesto");
    _depura("Presupuesto::imprimirPresupuesto", 0);
}


