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

#include "pedidoproveedor.h"
#include "company.h"
#include "fixed.h"
#include "funcaux.h"


typedef QMap<QString, Fixed> base;

PedidoProveedor::PedidoProveedor(company *comp) : DBRecord(comp) {
    companyact = comp;
    setDBTableName("pedidoproveedor");
    setDBCampoId("idpedidoproveedor");
    addDBCampo("idpedidoproveedor", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("PedidoProveedor", "Id pedido proveedor"));
    addDBCampo("idproveedor", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("PedidoProveedor", "Id proveedor"));
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("PedidoProveedor", "Id almacen"));
    addDBCampo("numpedidoproveedor", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("PedidoProveedor", "Numero pedido proveedor"));
    addDBCampo("fechapedidoproveedor", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("PedidoProveedor", "Fecha pedido proveedor"));
    addDBCampo("comentpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("PedidoProveedor", "Comentario pedido"));
    addDBCampo("procesadopedidoproveedor", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("PedidoProveedor", "Pedido procesado"));
    addDBCampo("descpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("PedidoProveedor", "Descripcion pedido"));
    addDBCampo("refpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("PedidoProveedor", "Referencia pedido"));
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("PedidoProveedor", "Id forma de pago"));
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("PedidoProveedor", "Id trabajador"));
    addDBCampo("contactpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("PedidoProveedor", "Persona de contacto proveedor"));
    addDBCampo("telpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("PedidoProveedor", "Telefono proveedor"));
}


PedidoProveedor::~PedidoProveedor() {}


int PedidoProveedor::borrar() {
    if (DBvalue("idpedidoproveedor") != "") {
        listalineas->borrar();
        listadescuentos->borrar();
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM pedidoproveedor WHERE idpedidoproveedor=" + DBvalue("idpedidoproveedor"));
        if (error) {
            companyact->rollback();
            return -1;
        } // end if
        companyact->commit();
        vaciaPedidoProveedor();
        pintar();
    } // end if
    return 0;
}


void PedidoProveedor::vaciaPedidoProveedor() {
    DBclear();
}


void PedidoProveedor::pintar() {
    _depura("PedidoProveedor::pintar", 0);
    pintaidproveedor(DBvalue("idproveedor"));
    pintaidalmacen(DBvalue("idalmacen"));
    pintaidpedidoproveedor(DBvalue("idpedidoproveedor"));
    pintanumpedidoproveedor(DBvalue("numpedidoproveedor"));
    pintafechapedidoproveedor(DBvalue("fechapedidoproveedor"));
    pintadescpedidoproveedor(DBvalue("descpedidoproveedor"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintacomentpedidoproveedor(DBvalue("comentpedidoproveedor"));
    pintarefpedidoproveedor(DBvalue("refpedidoproveedor"));
    pintaprocesadopedidoproveedor(DBvalue("procesadopedidoproveedor"));
    pintacontactpedidoproveedor(DBvalue("contactpedidoproveedor"));
    pintatelpedidoproveedor(DBvalue("telpedidoproveedor"));
    pintaidtrabajador(DBvalue("idtrabajador"));
    /// Pinta el subformulario de detalle del PedidoProveedor.
    // listalineas->pintaListLinPedidoProveedor();
    /// Pinta el subformulario de descuentos del pedidoproveedor
    calculaypintatotales();
    _depura("FIN PedidoProveedor::pintaPedidoProveedor()\n", 0);
}


/// Esta funcion carga un PedidoProveedor.
int PedidoProveedor::cargar(QString idbudget) {
    _depura("PedidoProveedor::cargar", 0);
    QString query = "SELECT * FROM pedidoproveedor WHERE idpedidoproveedor=" + idbudget;
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


int PedidoProveedor::guardar() {
    _depura("PedidoProveedor::guardar", 0);
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error) {
        companyact->rollback();
        return -1;
    } // end if
    setidpedidoproveedor(id);

    error = listalineas->guardar();
    if (error) {
        companyact->rollback();
        return -1;
    } // end if

    error = listadescuentos->guardar();
    if (error) {
        companyact->rollback();
        return -1;
    } // end if
    companyact->commit();
    return 0;
}


void PedidoProveedor::imprimirPedidoProveedor() {
    _depura("PedidoProveedor::imprimirPedidoProveedor", 2);
    /// Copiamos el archivo.
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "pedidoproveedor.rml";
    archivo = "cp " + archivo + " /tmp/pedidoproveedor.rml";
    system (archivo.ascii());

    /// Copiamos el logo.
    archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";
    archivo = "cp " + archivo + " /tmp/logo.jpg";
    system (archivo.ascii());

    QFile file;
    file.setName("/tmp/pedidoproveedor.rml");
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;

    /// Linea de totales del pedidoproveedor
    QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor=" + DBvalue("idproveedor");
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        buff.replace("[dirproveedor]", cur->valor("dirproveedor"));
        buff.replace("[poblproveedor]", cur->valor("poblproveedor"));
        buff.replace("[telproveedor]", cur->valor("telproveedor"));
        buff.replace("[nomproveedor]", cur->valor("nomproveedor"));
        buff.replace("[cifproveedor]", cur->valor("cifproveedor"));
    } // end if

    buff.replace("[numpedidoproveedor]", DBvalue("numpedidoproveedor"));
    buff.replace("[fechapedidoproveedor]", DBvalue("fechapedidoproveedor"));
    buff.replace("[comentpedidoproveedor]", DBvalue("comentpedidoproveedor"));
    buff.replace("[descpedidoproveedor]", DBvalue("descpedidoproveedor"));
    buff.replace("[refpedidoproveedor]", DBvalue("refpedidoproveedor"));

    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"10cm, 2cm, 2cm, 3cm\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "        <td>" + QApplication::translate("PedidoProveedor", "Concepto") + "</td>";
    fitxersortidatxt += "        <td>" + QApplication::translate("PedidoProveedor", "Cantidad") + "</td>";
    fitxersortidatxt += "        <td>" + QApplication::translate("PedidoProveedor", "Precio/u.") + "</td>";
    fitxersortidatxt += "        <td>" + QApplication::translate("PedidoProveedor", "Total") + "</td>";
    fitxersortidatxt += "</tr>";

    QString l;
    _depura("vamos a recorrer el listado de lineas", 2);
    for (int i = 0; i < listalineas->rowCount() - 1; i++) {
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "        <td>" + listalineas->DBvalue("desclpedidoproveedor", i) + "</td>";
        fitxersortidatxt += "        <td>" + l.sprintf("%2.2f",listalineas->DBvalue("cantlpedidoproveedor", i).toFloat()) + "</td>";
        fitxersortidatxt += "        <td>" + l.sprintf("%2.2f",listalineas->DBvalue("pvplpedidoproveedor", i).toFloat()) + "</td>";
        fitxersortidatxt += "        <td>" + l.sprintf("%2.2f",listalineas->DBvalue("cantlpedidoproveedor", i).toFloat() * listalineas->DBvalue("pvplpedidoproveedor", i).toFloat()) + "</td>";
        fitxersortidatxt += "</tr>";
    } // end for
    _depura("Fin de vamos a recorrer el listado de lineas", 2);

    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "        <td></td>";
    fitxersortidatxt += "        <td></td>";
    fitxersortidatxt += "        <td>" + QApplication::translate("PedidoProveedor", "Base") + "</td>";
    fitxersortidatxt += "        <td>" + listalineas->calculabase().toQString() + "</td>";
    fitxersortidatxt += "</tr>";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "        <td></td>";
    fitxersortidatxt += "        <td></td>";
    fitxersortidatxt += "        <td>" + QApplication::translate("PedidoProveedor", "I.V.A.") + "</td>";
    fitxersortidatxt += "        <td>" + listalineas->calculaiva().toQString() + "</td>";
    fitxersortidatxt += "</tr>";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "        <td></td>";
    fitxersortidatxt += "        <td></td>";
    fitxersortidatxt += "        <td>" + QApplication::translate("PedidoProveedor", "Total") + "</td>";
    fitxersortidatxt += "        <td>" + (listalineas->calculabase() + listalineas->calculaiva()).toQString() + "</td>";
    fitxersortidatxt += "</tr>";
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]",fitxersortidatxt);

    if (file.open( QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    }
    _depura("Vamos a ejecutar el trml2pdf", 2);
    system("trml2pdf.py /tmp/pedidoproveedor.rml > /tmp/pedidoproveedor.pdf");
    system("kpdf /tmp/pedidoproveedor.pdf");
}


void PedidoProveedor::calculaypintatotales() {
    _depura("calculaypintatotales \n", 0);
    base basesimp;

    /// Impresion de los contenidos.
    QString l;
    for (int i = 0; i < listalineas->rowCount() - 1; i++) {
        Fixed cant(listalineas->DBvalue("cantlpedidoproveedor", i));
        Fixed pvpund(listalineas->DBvalue("pvplpedidoproveedor", i));
        Fixed desc1(listalineas->DBvalue("descuentolpedidoproveedor", i));
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        basesimp[listalineas->DBvalue("ivalpedidoproveedor", i)] = basesimp[listalineas->DBvalue("ivalpedidoproveedor", i)] + base;
    } // end for

    Fixed basei("0.00");
    base::Iterator it;
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        basei = basei + it.data();
    } // end for
    /// Impresion de los descuentos.
    Fixed porcentt("0.00");

    for (int i = 0; i < listadescuentos->rowCount() - 1; i++) {
        Fixed propor(listadescuentos->DBvalue("proporciondpedidoproveedor", i).ascii());
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
}

