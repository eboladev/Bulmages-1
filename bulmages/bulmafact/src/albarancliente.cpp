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
#include <QMap>

#include "fixed.h"
#include "funcaux.h"
#include "albarancliente.h"


typedef QMap<QString, Fixed> base;

AlbaranCliente::AlbaranCliente(company *comp) : DBRecord (comp)  {
    _depura("AlbaranCliente::AlbaranCliente(company *)", 0);
    companyact = comp;
    setDBTableName("albaran");
    setDBCampoId("idalbaran");
    addDBCampo("idalbaran", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Presupuesto");
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("numalbaran", DBCampo::DBint, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("fechaalbaran", DBCampo::DBdate, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("contactalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("telalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("comentalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("comentprivalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("procesadoalbaran", DBCampo::DBboolean, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("descalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("refalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, "Referencia Albaran");
    listalineas = NULL;
    listadescuentos = NULL;
}


AlbaranCliente::~AlbaranCliente() {}


int AlbaranCliente::borrar() {
    if (DBvalue("idalbaran") != "")  {
        companyact->begin();
        listalineas->borrar();
        listadescuentos->borrar();

        int error = companyact->ejecuta("DELETE FROM albaran WHERE idalbaran=" +
                                        DBvalue("idalbaran"));
        if (error)  {
            companyact->rollback();
            return -1;
        }
        companyact->commit();
    }
    return 0;
}


void AlbaranCliente::vaciaAlbaranCliente() {
    DBclear();
}


void AlbaranCliente::pintar() {
    _depura("AlbaranCliente::pintar", 0);
    pintaIdAlbaran(DBvalue("idalbaran"));
    pintaNumAlbaran(DBvalue("numalbaran"));
    pintafechaalbaran(DBvalue("fechaalbaran"));
    pintaComentAlbaran(DBvalue("comentalbaran"));
    pintaComentPrivAlbaran(DBvalue("comentprivalbaran"));
    pintaidcliente(DBvalue("idcliente"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintaidalmacen(DBvalue("idalmacen"));
    pintarefalbaran(DBvalue("refalbaran"));
    pintadescalbaran(DBvalue("descalbaran"));
    pintaidtrabajador(DBvalue("idtrabajador"));
    pintacontactalbaran(DBvalue("contactalbaran"));
    pintatelalbaran(DBvalue("telalbaran"));
    pintaprocesadoalbaran(DBvalue("procesadoalbaran"));

    /// Pinta el subformulario de detalle del AlbaranCliente.
    /// Pintamos los totales.
    calculaypintatotales();
    _depura("END AlbaranCliente::pintar", 0);
}


/// Esta funcioncarga un AlbaranCliente.
int AlbaranCliente::cargar(QString idalbaran)  {
    _depura("AlbaranCliente::cargar", 0);
    QString query = "SELECT * FROM albaran WHERE idalbaran=" + idalbaran;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof())
        DBload(cur);
    delete cur;
    listalineas->cargar(idalbaran);
    listadescuentos->cargar(idalbaran);
    pintar();
    _depura("Fin AlbaranCliente::cargar", 0);
    return 0;
}


int AlbaranCliente::guardar() {
    _depura("AlbaranCliente::guardar",0);
    /// Todo el guardado es una transaccion.
    companyact->begin();
try {
    QString id;
    DBsave(id);
    setidalbaran(id);
    listalineas->guardar();
    listadescuentos->guardar();
    companyact->commit();
    _depura("END AlbaranCliente::guardar",0);
    return 0;
}// end try

catch(...) {
	_depura("AlbaranCliente::guardar error al guardar albaran cliente",1);
	companyact->rollback();
	return -1;
}
}


void AlbaranCliente::imprimirAlbaranCliente()  {
    base basesimp;
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS) + "albaran.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "albaran.rml";
    QString archivologo=confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS
    archivo = "copy " + archivo + " " + archivod;
#else
    archivo = "cp " + archivo + " " + archivod;
#endif

    system (archivo.ascii());

    /// Copiamos el logo.
#ifdef WINDOWS
    archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#else
    archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#endif

    system (archivologo.ascii());
    QFile file;
    file.setName(archivod);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt = "";
    /// Linea de totales del presupuesto.
    QString SQLQuery = "SELECT * FROM cliente WHERE idcliente=" + DBvalue("idcliente");
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        buff.replace("[dircliente]", cur->valor("dircliente"));
        buff.replace("[poblcliente]",cur->valor("poblcliente"));
        buff.replace("[telcliente]", cur->valor("telcliente"));
        buff.replace("[nomcliente]", cur->valor("nomcliente"));
        buff.replace("[cifcliente]", cur->valor("cifcliente"));
        buff.replace("[idcliente]", cur->valor("idcliente"));
        buff.replace("[cpcliente]", cur->valor("cpcliente"));
    }
    delete cur;
    buff.replace("[numalbaran]", DBvalue("numalbaran"));
    buff.replace("[falbaran]", DBvalue("fechaalbaran"));
    buff.replace("[contactalbaran]", DBvalue("contactalbaran"));
    buff.replace("[telalbaran]", DBvalue("telalbaran"));
    buff.replace("[comentalbaran]", DBvalue("comentalbaran"));
    buff.replace("[descalbaran]", DBvalue("descalbaran"));
    buff.replace("[refalbaran]", DBvalue("refalbaran"));

    /// Impresion de la tabla de contenidos.
    fitxersortidatxt += "<blockTable style=\"tablacontenido\" colWidths=\"1.75cm, " \
                        "8.75cm, 1.5cm, 1.5cm, 1.5cm, 2.25cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "<td>Cod.</td>\n";
    fitxersortidatxt += "<td>Concepto</td>\n";
    fitxersortidatxt += "<td>Cant.</td>\n";
    fitxersortidatxt += "<td>Precio</td>\n";
    fitxersortidatxt += "<td>Desc.</td>\n";
    fitxersortidatxt += "<td>Total</td>\n";
    fitxersortidatxt += "</tr>\n";
    QString l;
    /// Contador que sirve para poner lineas de mas en caso de que sea preciso.
    int i=0;
    SDBRecord *linea;
    for (i = 0; i < listalineas->rowCount(); ++i) {
        linea = listalineas->lineaat(i);
        Fixed base = Fixed(linea->DBvalue("cantlalbaran").ascii()) *
                     Fixed(linea->DBvalue("pvplalbaran").ascii());
        basesimp[linea->DBvalue("ivalalbaran")] = basesimp[linea->DBvalue("ivalalbaran")] + base -
                base * Fixed(linea->DBvalue("descontlalbaran").ascii()) / 100;

        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "<td>" + XMLProtect(linea->DBvalue("codigocompletoarticulo")) +
                            "</td>\n";
        fitxersortidatxt += "<td>" + XMLProtect(linea->DBvalue("desclalbaran")) + "</td>\n";
        fitxersortidatxt += "<td>" + l.sprintf("%s",linea->DBvalue("cantlalbaran").ascii()) +
                            "</td>\n";
        fitxersortidatxt += "<td>" + l.sprintf("%s",linea->DBvalue("pvplalbaran").ascii()) +
                            "</td>\n";
        fitxersortidatxt += "<td>" + l.sprintf("%s",linea->DBvalue("descontlalbaran").ascii()) +
                            " %</td>\n";
        fitxersortidatxt += "<td>" + l.sprintf("%s",(base - base *
                            Fixed(linea->DBvalue("descontlalbaran")) / 100).toQString().ascii())
                            + "</td>\n";
        fitxersortidatxt += "</tr>";
        i++;
    }

    while (i++ < 15) {
        fitxersortidatxt += "<tr></tr>";
    } // end while

    fitxersortidatxt += "</blockTable>\n";
    buff.replace("[story]", fitxersortidatxt);
    Fixed basei("0.00");
    base::Iterator it;

    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        basei = basei + it.data();
    }

    /// Impresion de los descuentos.
    fitxersortidatxt = "";
    Fixed porcentt("0.00");
    SDBRecord *linea1;
    if (listadescuentos->rowCount()) {
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm," \
                            " 2cm, 3cm\" repeatRows=\"1\">\n";
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "<td>Descuento</td>\n";
        fitxersortidatxt += "<td>Porcentaje</td>\n";
        fitxersortidatxt += "<td>Total</td>\n";
        fitxersortidatxt += "</tr>\n";

        for (int i = 0; i < listadescuentos->rowCount(); ++i) {
            linea1 = listadescuentos->lineaat(i);
            porcentt = porcentt + Fixed(linea1->DBvalue("proporciondalbaran").ascii());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "<td>" + linea1->DBvalue("conceptdalbaran") + "</td>\n";
            fitxersortidatxt += "<td>" + l.sprintf("%s",
                                linea1->DBvalue("proporciondalbaran").ascii()) +
                                " %</td>\n";
            fitxersortidatxt += "<td>" + l.sprintf("-%s",
                                (Fixed(linea1->DBvalue("proporciondalbaran")) * basei /
                                100).toQString().ascii()) + "</td>\n";
            fitxersortidatxt += "</tr>";
        } // end for

        fitxersortidatxt += "</blockTable>\n";
    }

    buff.replace("[descuentos]", fitxersortidatxt);
    /// Impresión de los totales.
    fitxersortidatxt = "";
    /// Rellena el primer tr de titulares.
    QString tr1 = "";
    /// Rellena el segundo tr de cantidades.
    QString tr2 = "";
    fitxersortidatxt += "<blockTable style=\"tablatotales\">\n";
    Fixed totbaseimp("0.00");
    Fixed parbaseimp("0.00");

    for (it = basesimp.begin(); it != basesimp.end(); ++it) {

        if (porcentt > 0) {
            parbaseimp = it.data() - it.data() * porcentt / 100;
        } else {
            parbaseimp = it.data();
        } // end if

        totbaseimp = totbaseimp + parbaseimp;
        tr1 += "<td>Base " + it.key() + " %</td>\n";
        tr2 += "<td>" + l.sprintf("%s", parbaseimp.toQString().ascii()) + "</td>\n";
    }

    Fixed totiva("0.0");
    Fixed pariva("0.0");

    for (it = basesimp.begin(); it != basesimp.end(); ++it) {

        if (porcentt > 0) {
            pariva = (it.data() - it.data() * porcentt / 100) * Fixed(it.key()) / 100;
        } else {
            pariva = it.data() * Fixed(it.key()) / 100;
        } // end if

        totiva = totiva + pariva;
        tr1 += "<td>Iva " + it.key() + " %</td>\n";
        tr2 += "<td>" + l.sprintf("%s", pariva.toQString().ascii()) + "</td>\n";
    } // end for

    tr1 += "<td>Total </td>\n";
    tr2 += "<td>"+l.sprintf("%s", (totiva + totbaseimp).toQString().ascii()) + "</td>\n";
    fitxersortidatxt += "<tr>" + tr1 + "</tr><tr>" + tr2 + "</tr></blockTable>\n";
    buff.replace("[totales]",fitxersortidatxt);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    } // end if

    invocaPDF("albaran");
}


void AlbaranCliente::calculaypintatotales()  {
    _depura("calculaypintatotales.",0);
    base basesimp;
    SDBRecord *linea;
    /// Impresion de los contenidos.
    QString l;

    for (int i = 0; i < listalineas->rowCount(); ++i) {
        linea = listalineas->lineaat(i);
        Fixed cant(linea->DBvalue("cantlalbaran").ascii());
        Fixed pvpund(linea->DBvalue("pvplalbaran").ascii());
        Fixed desc1(linea->DBvalue("descontlalbaran").ascii());
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        Fixed val("0.00");
        val = basesimp[linea->DBvalue("ivalalbaran")];
        val = val + base;
        QString lin = linea->DBvalue("ivalalbaran");
        basesimp[lin] = val;
    } // end for

    Fixed basei("0.00");
    base::Iterator it;

    for (it = basesimp.begin(); it != basesimp.end(); ++it)
        basei = basei + it.data();

    /// Impresion de los descuentos.
    Fixed porcentt("0.00");
    SDBRecord *linea1;

    for(int i = 0; i < listadescuentos->rowCount(); ++i) {
        linea1 = listadescuentos->lineaat(i);
        Fixed propor(linea1->DBvalue("proporciondalbaran").ascii());
        porcentt = porcentt + propor;
    } // end for

    Fixed totbaseimp("0.00");
    Fixed parbaseimp("0.00");

    for (it = basesimp.begin(); it != basesimp.end(); ++it) {

        if (porcentt > Fixed("0.00")) {
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
    _depura("END AlbaranCliente::calculaypintatotales",0);
}

