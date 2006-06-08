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

#include "pedidocliente.h"
#include "company.h"
#include "fixed.h"
#include "funcaux.h"


typedef QMap<QString, Fixed> base;

PedidoCliente::PedidoCliente(company *comp) : DBRecord(comp) {
    companyact = comp;
    setDBTableName("pedidocliente");
    setDBCampoId("idpedidocliente");
    addDBCampo("idpedidocliente", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("Identificador", "pedidocliente"));
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("Cliente", "pedidocliente"));
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("Almacen", "pedidocliente"));
    addDBCampo("numpedidocliente", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Numero pedido cliente", "pedidocliente"));
    addDBCampo("fechapedidocliente", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Identificador presupuesto", "pedidocliente"));
    addDBCampo("descpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Fecha", "pedidocliente"));
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Forma pago", "pedidocliente"));
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Trabajador", "pedidocliente"));
    addDBCampo("contactpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Contacto", "pedidocliente"));
    addDBCampo("telpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Telefono", "pedidocliente"));
    addDBCampo("comentpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Comentarios", "pedidocliente"));
    addDBCampo("procesadopedidocliente", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("Procesado", "pedidocliente"));
    addDBCampo("refpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Referencia", "pedidocliente"));
}


PedidoCliente::~PedidoCliente() {}


int PedidoCliente::borrar() {
    if (DBvalue("idpedidocliente") != "") {
        companyact->begin();
        listalineas->borrar();
        listadescuentos->borrar();
        int error = companyact->ejecuta("DELETE FROM pedidocliente WHERE idpedidocliente=" + DBvalue("idpedidocliente"));
        if (error) {
            companyact->rollback();
            return -1;
        } // end if
        companyact->commit();
    } // end if
    return 0;
}


void PedidoCliente::vaciaPedidoCliente() {
    DBclear();
}


void PedidoCliente::pintar() {
    _depura("PedidoCliente::pintaPedidoCliente\n", 0);
    pintaidcliente(DBvalue("idcliente"));
    pintaidalmacen(DBvalue("idalmacen"));
    pintaidpedidocliente(DBvalue("idpedidocliente"));
    pintanumpedidocliente(DBvalue("numpedidocliente"));
    pintafechapedidocliente(DBvalue("fechapedidocliente"));
    pintadescpedidocliente(DBvalue("descpedidocliente"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintacomentpedidocliente(DBvalue("comentpedidocliente"));
    pintarefpedidocliente(DBvalue("refpedidocliente"));
    pintaprocesadopedidocliente(DBvalue("procesadopedidocliente"));
    pintacontactpedidocliente(DBvalue("contactpedidocliente"));
    pintatelpedidocliente(DBvalue("telpedidocliente"));
    pintaidtrabajador(DBvalue("idtrabajador"));
    calculaypintatotales();
    _depura("FIN PedidoCliente::pintaPedidoCliente()\n", 0);
}


/// Esta funcion carga un PedidoCliente.
int PedidoCliente::cargar(QString idbudget) {
    _depura("cargaPedidoCliente()\n", 0);
    QString query = "SELECT * FROM pedidocliente WHERE idpedidocliente=" + idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;
    listalineas->cargar(idbudget);
    listadescuentos->cargar(idbudget);
    pintar();
    return 0;
}


int PedidoCliente::guardar() {
    _depura("PedidoCliente::guardar", 0);
    QString id;
    companyact->begin();
    try {
        DBsave(id);
        setidpedidocliente(id);
        listalineas->guardar();
        listadescuentos->guardar();
        companyact->commit();
        _depura("END PedidoCliente::guardar", 0);
        return 0;
    } // end try

    catch(...) {
        _depura("se produjo un error al guardar, cancelamos la operacion", 1);
        companyact->rollback();
        return -1;
    } // end catch
}


void PedidoCliente::imprimirPedidoCliente() {
    _depura("PedidoCliente::imprimirPedidoCliente", 0);

    base basesimp;

    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "pedidocliente.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "pedidocliente.rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";

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
    QString fitxersortidatxt="";

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

    buff.replace("[numpedidocliente]", DBvalue("numpedidocliente"));
    buff.replace("[fechapedidocliente]", DBvalue("fechapedidocliente"));
    buff.replace("[contactpedidocliente]", DBvalue("contactpedidocliente"));
    buff.replace("[telpedidocliente]", DBvalue("telpedidocliente"));
    buff.replace("[comentpedidocliente]", DBvalue("comentpedidocliente"));
    buff.replace("[descpedidocliente]", DBvalue("descpedidocliente"));
    buff.replace("[refpedidocliente]", DBvalue("refpedidocliente"));

    /// Impresion de la tabla de contenidos.
    fitxersortidatxt += "<blockTable style=\"tablacontenido\" colWidths=\"1.75cm, 8.75cm, 1.5cm, 1.5cm, 1.5cm, 2.25cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "	<td>Cod.</td>\n";
    fitxersortidatxt += "	<td>Concepto</td>\n";
    fitxersortidatxt += "	<td>Cant.</td>\n";
    fitxersortidatxt += "	<td>Precio</td>\n";
    fitxersortidatxt += "	<td>Desc.</td>\n";
    fitxersortidatxt += "	<td>Total</td>\n";
    fitxersortidatxt += "</tr>\n";
    QString l;

    /// Contador que sirve para poner lineas de mas en caso de que sea preciso.
    int i = 0;

    SDBRecord *linea;
    for (int i = 0; i < listalineas->rowCount()-1; ++i) {
        linea = listalineas->lineaat(i);
        Fixed base = Fixed(linea->DBvalue("cantlpedidocliente").ascii()) * Fixed(linea->DBvalue("pvplpedidocliente").ascii());
        basesimp[linea->DBvalue("ivalpedidocliente")] = basesimp[linea->DBvalue("ivalpedidocliente")] + base - base * Fixed(linea->DBvalue("descuentolpedidocliente").ascii()) / 100;
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>" + linea->DBvalue("codigocompletoarticulo") + "</td>\n";
        fitxersortidatxt += "	<td>" + linea->DBvalue("desclpedidocliente") + "</td>\n";
        fitxersortidatxt += "	<td>" + l.sprintf("%s", linea->DBvalue("cantlpedidocliente").ascii())+"</td>\n";
        fitxersortidatxt += "	<td>" + l.sprintf("%s", linea->DBvalue("pvplpedidocliente").ascii())+"</td>\n";
        fitxersortidatxt += "	<td>" + l.sprintf("%s", linea->DBvalue("descuentolpedidocliente").ascii()) + " %</td>\n";
        fitxersortidatxt += "	<td>" + l.sprintf("%s", (base - base * Fixed (linea->DBvalue("descuentolpedidocliente")) / 100).toQString().ascii()) + "</td>\n";
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
    if (listadescuentos->rowCount()) {
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm, 2cm, 3cm\" repeatRows=\"1\">\n";
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>Descuento</td>\n";
        fitxersortidatxt += "	<td>Porcentaje</td>\n";
        fitxersortidatxt += "	<td>Total</td>\n";
        fitxersortidatxt += "</tr>\n";
        for (int i = 0; i < listadescuentos->rowCount(); ++i) {
            linea1 = listadescuentos->lineaat(i);
            porcentt = porcentt + Fixed(linea1->DBvalue("proporciondpedidocliente").ascii());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "	<td>" + linea1->DBvalue("conceptdpedidocliente") + "</td>\n";
            fitxersortidatxt += "	<td>" + l.sprintf("%s", linea1->DBvalue("proporciondpedidocliente").ascii()) + " %</td>\n";
            fitxersortidatxt += "	<td>" + l.sprintf("-%s", (Fixed(linea1->DBvalue("proporciondpedidocliente")) * basei / 100).toQString().ascii()) + "</td>\n";
            fitxersortidatxt += "</tr>";
        } // end for
        fitxersortidatxt += "</blockTable>\n";
    } // end if
    buff.replace("[descuentos]", fitxersortidatxt);

    /// Impresion de los totales.
    fitxersortidatxt = "";
    QString tr1 = "";/// Rellena el primer tr de titulares.
    QString tr2 = "";/// Rellena el segundo tr de cantidades.
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
        tr1 += "	<td>Base " + it.key() + " %</td>\n";
        tr2 += "	<td>" + l.sprintf("%s",parbaseimp.toQString().ascii()) + "</td>\n";
    } // end for

    Fixed totiva("0.0");
    Fixed pariva("0.0");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        if (porcentt > 0) {
            pariva = (it.data() - it.data() * porcentt / 100) * Fixed(it.key()) / 100;
        } else {
            pariva = it.data() * Fixed(it.key()) / 100;
        } // end if
        totiva = totiva + pariva;
        tr1 += "	<td>Iva " + it.key() + " %</td>\n";
        tr2 += "	<td>" + l.sprintf("%s", pariva.toQString().ascii()) + "</td>\n";
    } // end for
    tr1 += "	<td>Total </td>\n";
    tr2 += "	<td>" + l.sprintf("%s", (totiva + totbaseimp).toQString().ascii()) + "</td>\n";
    fitxersortidatxt += "<tr>" + tr1 + "</tr><tr>" + tr2 + "</tr></blockTable>\n";
    buff.replace("[totales]", fitxersortidatxt);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    }

    invocaPDF("pedidocliente");
}


void PedidoCliente::calculaypintatotales() {
    _depura("PedidoCliente::calculaypintatotales \n", 0);
    base basesimp;

    /// Impresion de los contenidos.
    QString l;
    SDBRecord *linea;
    for (int i = 0; i < listalineas->rowCount(); ++i) {
        linea = listalineas->lineaat(i);
        Fixed cant(linea->DBvalue("cantlpedidocliente").ascii());
        Fixed pvpund(linea->DBvalue("pvplpedidocliente").ascii());
        Fixed desc1(linea->DBvalue("descuentolpedidocliente").ascii());
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        basesimp[linea->DBvalue("ivalpedidocliente")] = basesimp[linea->DBvalue("ivalpedidocliente")] + base;
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
        for ( int i = 0; i < listadescuentos->rowCount(); ++i) {
            linea1 = listadescuentos->lineaat(i);
            Fixed propor(linea1->DBvalue("proporciondpedidocliente").ascii());
            porcentt = porcentt + propor;
        } // end for
    } // end if

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
}

