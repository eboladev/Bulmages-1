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


PedidoCliente::PedidoCliente(company *comp, QWidget *parent) : FichaBf(comp, parent) {
    setDBTableName("pedidocliente");
    setDBCampoId("idpedidocliente");
    addDBCampo("idpedidocliente", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("PedidoCliente", "Identificador"));
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("PedidoCliente", "Cliente"));
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("PedidoCliente", "Almacen"));
    addDBCampo("numpedidocliente", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Numero pedido cliente"));
    addDBCampo("fechapedidocliente", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Identificador presupuesto"));
    addDBCampo("descpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Fecha"));
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Forma pago"));
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Trabajador"));
    addDBCampo("contactpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Contacto"));
    addDBCampo("telpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Telefono"));
    addDBCampo("comentpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Comentarios"));
    addDBCampo("procesadopedidocliente", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Procesado"));
    addDBCampo("refpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Referencia"));
}


PedidoCliente::~PedidoCliente() {}


int PedidoCliente::borrar() {
    if (DBvalue("idpedidocliente") != "") {
        companyact->begin();
        m_listalineas->borrar();
        m_listadescuentos->borrar();
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
    _depura("PedidoCliente::cargar", 0);
    QString query = "SELECT * FROM pedidocliente WHERE idpedidocliente=" + idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;
    m_listalineas->cargar(idbudget);
    m_listadescuentos->cargar(idbudget);
    pintar();
    _depura("END PedidoCliente::cargar", 0);
    return 0;
}

/// Guardamos el pedido cliente.
int PedidoCliente::guardar() {
    _depura("PedidoCliente::guardar", 0);
    QString id;
    companyact->begin();
    try {
        DBsave(id);
        setidpedidocliente(id);
        m_listalineas->guardar();
        m_listadescuentos->guardar();
        companyact->commit();

	/// Hacemos una carga para recuperar los campos Referencia y num
	cargar(id);
        _depura("END PedidoCliente::guardar", 0);
        return 0;
    } catch(...) {
        _depura("PedidoCliente::guardar se produjo un error al guardar, cancelamos la operacion", 0);
        companyact->rollback();
        throw -1;
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

    system(archivo.toAscii().constData());

    /// Copiamos el logo.
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

    /// Linea de totales del presupuesto.
    QString SQLQuery = "SELECT * FROM cliente WHERE idcliente = " + DBvalue("idcliente");
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
    QString l;

    SDBRecord *linea;
    for (int i = 0; i < m_listalineas->rowCount() - 1; ++i) {
        linea = m_listalineas->lineaat(i);
        Fixed base = Fixed(linea->DBvalue("cantlpedidocliente").toAscii().constData()) * Fixed(linea->DBvalue("pvplpedidocliente").toAscii().constData());
        basesimp[linea->DBvalue("ivalpedidocliente")] = basesimp[linea->DBvalue("ivalpedidocliente")] + base - base * Fixed(linea->DBvalue("descuentolpedidocliente").toAscii().constData()) / 100;
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>" + linea->DBvalue("codigocompletoarticulo") + "</td>\n";
        fitxersortidatxt += "	<td>" + linea->DBvalue("desclpedidocliente") + "</td>\n";
        fitxersortidatxt += "	<td>" + l.sprintf("%s", linea->DBvalue("cantlpedidocliente").toAscii().constData())+"</td>\n";
        fitxersortidatxt += "	<td>" + l.sprintf("%s", linea->DBvalue("pvplpedidocliente").toAscii().constData())+"</td>\n";
        fitxersortidatxt += "	<td>" + l.sprintf("%s", linea->DBvalue("descuentolpedidocliente").toAscii().constData()) + " %</td>\n";
        fitxersortidatxt += "	<td>" + l.sprintf("%s", (base - base * Fixed (linea->DBvalue("descuentolpedidocliente")) / 100).toQString().toAscii().constData()) + "</td>\n";
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
        fitxersortidatxt += "        <td>" + QApplication::translate("PedidoCliente", "Descuento") + "</td>\n";
        fitxersortidatxt += "        <td>" + QApplication::translate("PedidoCliente", "Porcentaje") + "</td>\n";
        fitxersortidatxt += "        <td>" + QApplication::translate("PedidoCliente", "Total") + "</td>\n";
        fitxersortidatxt += "</tr>\n";
        for (int i = 0; i < m_listadescuentos->rowCount() - 1; ++i) {
            linea1 = m_listadescuentos->lineaat(i);
            porcentt = porcentt + Fixed(linea1->DBvalue("proporciondpedidocliente").toAscii().constData());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "        <td>" + linea1->DBvalue("conceptdpedidocliente") + "</td>\n";
            fitxersortidatxt += "        <td>" + l.sprintf("%s", linea1->DBvalue("proporciondpedidocliente").toAscii().constData()) + " %</td>\n";
            fitxersortidatxt += "        <td>" + l.sprintf("-%s", (Fixed(linea1->DBvalue("proporciondpedidocliente")) * basei / 100).toQString().toAscii().constData()) + "</td>\n";
            fitxersortidatxt += "</tr>";
        } // end for
        fitxersortidatxt += "</blockTable>\n";
    } // end if
    buff.replace("[descuentos]", fitxersortidatxt);

    /// Impresion de los totales.
    fitxersortidatxt = "";
    QString tr1 = ""; /// Rellena el primer tr de titulares.
    QString tr2 = ""; /// Rellena el segundo tr de cantidades.
    fitxersortidatxt += "<blockTable style=\"tablatotales\">\n";

    Fixed totbaseimp("0.00");
    Fixed parbaseimp("0.00");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        if (porcentt > 0) {
            parbaseimp = it.value() - it.value() * porcentt / 100;
        } else {
            parbaseimp = it.value();
        } // end if
        totbaseimp = totbaseimp + parbaseimp;
        tr1 += "        <td>" + QApplication::translate("PedidoCliente", "Base ") + it.key() + " %</td>\n";
        tr2 += "        <td>" + l.sprintf("%s", parbaseimp.toQString().toAscii().constData()) + "</td>\n";
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
        tr1 += "        <td>" + QApplication::translate("PedidoCliente", "Iva ") + it.key() + " %</td>\n";
        tr2 += "        <td>" + l.sprintf("%s", pariva.toQString().toAscii().constData()) + "</td>\n";
    } // end for
    tr1 += "        <td>" + QApplication::translate("PedidoCliente", "Total ") + " </td>\n";
    tr2 += "        <td>" + l.sprintf("%s", (totiva + totbaseimp).toQString().toAscii().constData()) + "</td>\n";
    fitxersortidatxt += "<tr>" + tr1 + "</tr><tr>" + tr2 + "</tr></blockTable>\n";
    buff.replace("[totales]", fitxersortidatxt);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    }

    invocaPDF("pedidocliente");
}

