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


// typedef QMap<QString, Fixed> base;

PedidoProveedor::PedidoProveedor(company *comp, QWidget *parent) : FichaBf(comp, parent) {
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


PedidoProveedor::~PedidoProveedor() {
   _depura("PedidoProveedor::~PedidoProveedor", 0);
   _depura("END PedidoProveedor::~PedidoProveedor", 0);
}


int PedidoProveedor::borrar() {
    _depura("PedidoProveedor::borrar", 0);
    if (DBvalue("idpedidoproveedor") != "") {
        m_listalineas->borrar();
        m_listadescuentos->borrar();
        m_companyact->begin();
        int error = m_companyact->ejecuta("DELETE FROM pedidoproveedor WHERE idpedidoproveedor=" + DBvalue("idpedidoproveedor"));
        if (error) {
            m_companyact->rollback();
            return -1;
        } // end if
        m_companyact->commit();
        vaciaPedidoProveedor();
        pintar();
    } // end if
    _depura("END PedidoProveedor::borrar", 0);
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
    cursor2 * cur= m_companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;
    m_listalineas->cargar(idbudget);
    m_listadescuentos->cargar(idbudget);
    pintar();
    _depura("END PedidoProveedor::cargar", 0);
    return 0;
}


int PedidoProveedor::guardar() {
    _depura("PedidoProveedor::guardar", 0);
    QString id;
    try {
        m_companyact->begin();
        int error = DBsave(id);
        if (error)
            throw -1;
        setidpedidoproveedor(id);
        error = m_listalineas->guardar();
        if (error)
            throw -1;
        error = m_listadescuentos->guardar();
        if (error)
            throw -1;
        m_companyact->commit();

	/// Hacemos una carga para recuperar la referencia
	cargar(id);

        _depura("END PedidoProveedor::guardar", 0);
        return 0;
    } catch (...) {
        m_companyact->rollback();
        _depura("PedidoProveedor::guardar Error al guardar el pedido proveedor", 2);
        throw -1;
    } // end try
}


/// Impresion de un Pedido de Proveedor
/** Usa la plantilla pedidoproveedor.rml */
void PedidoProveedor::imprimirPedidoProveedor() {
    _depura("PedidoProveedor::imprimirPedidoProveedor", 0);

    base basesimp;

    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "pedidoproveedor.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "pedidoproveedor.rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system (archivo.toAscii().constData());

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
    QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor = " + DBvalue("idproveedor");
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        buff.replace("[dirproveedor]", cur->valor("dirproveedor"));
        buff.replace("[poblproveedor]", cur->valor("poblproveedor"));
        buff.replace("[telproveedor]", cur->valor("telproveedor"));
        buff.replace("[nomproveedor]", cur->valor("nomproveedor"));
        buff.replace("[cifproveedor]", cur->valor("cifproveedor"));
        buff.replace("[idproveedor]", cur->valor("idproveedor"));
        buff.replace("[cpproveedor]", cur->valor("cpproveedor"));
    } // end if
    delete cur;

    buff.replace("[numpedidoproveedor]", DBvalue("numpedidoproveedor"));
    buff.replace("[fechapedidoproveedor]", DBvalue("fechapedidoproveedor"));
    buff.replace("[contactpedidoproveedor]", DBvalue("contactpedidoproveedor"));
    buff.replace("[telpedidoproveedor]", DBvalue("telpedidoproveedor"));
    buff.replace("[comentpedidoproveedor]", DBvalue("comentpedidoproveedor"));
    buff.replace("[descpedidoproveedor]", DBvalue("descpedidoproveedor"));
    buff.replace("[refpedidoproveedor]", DBvalue("refpedidoproveedor"));

    /// Impresion de la tabla de contenidos.
    QString l;

    SDBRecord *linea;
    for (int i = 0; i < m_listalineas->rowCount() - 1; ++i) {
        linea = m_listalineas->lineaat(i);
        Fixed base = Fixed(linea->DBvalue("cantlpedidoproveedor").toAscii().constData()) * Fixed(linea->DBvalue("pvplpedidoproveedor").toAscii().constData());
        basesimp[linea->DBvalue("ivalpedidoproveedor")] = basesimp[linea->DBvalue("ivalpedidoproveedor")] + base - base * Fixed(linea->DBvalue("descuentolpedidoproveedor").toAscii().constData()) / 100;
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>" + linea->DBvalue("codigocompletoarticulo") + "</td>\n";
        fitxersortidatxt += "	<td>" + linea->DBvalue("desclpedidoproveedor") + "</td>\n";
        fitxersortidatxt += "	<td>" + l.sprintf("%s", linea->DBvalue("cantlpedidoproveedor").toAscii().constData())+"</td>\n";
        fitxersortidatxt += "	<td>" + l.sprintf("%s", linea->DBvalue("pvplpedidoproveedor").toAscii().constData())+"</td>\n";
        fitxersortidatxt += "	<td>" + l.sprintf("%s", linea->DBvalue("descuentolpedidoproveedor").toAscii().constData()) + " %</td>\n";
        fitxersortidatxt += "	<td>" + l.sprintf("%s", (base - base * Fixed (linea->DBvalue("descuentolpedidoproveedor")) / 100).toQString().toAscii().constData()) + "</td>\n";
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
            porcentt = porcentt + Fixed(linea1->DBvalue("proporciondpedidoproveedor").toAscii().constData());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "        <td>" + linea1->DBvalue("conceptdpedidoproveedor") + "</td>\n";
            fitxersortidatxt += "        <td>" + l.sprintf("%s", linea1->DBvalue("proporciondpedidoproveedor").toAscii().constData()) + " %</td>\n";
            fitxersortidatxt += "        <td>" + l.sprintf("-%s", (Fixed(linea1->DBvalue("proporciondpedidoproveedor")) * basei / 100).toQString().toAscii().constData()) + "</td>\n";
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

    invocaPDF("pedidoproveedor");
   _depura("PedidoProveedor::imprimirPedidoProveedor", 0);
}

