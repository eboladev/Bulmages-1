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

#include <QString>
#include <QFile>
#include <QTextStream>

#include "informereferencia.h"
#include "company.h"
#include "postgresiface2.h"
#include "funcaux.h"


InformeReferencia::InformeReferencia (company *comp) {
    companyact = comp;
}


InformeReferencia::~InformeReferencia() {}


void InformeReferencia::generarinforme() {
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "informereferencia.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "informereferencia.rml";
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

    QFile file;
    file.setName(archivod);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt = "";

    buff.replace("[referencia]", m_referencia);


    QString SQLQuery = "SELECT * FROM cliente WHERE idcliente IN (";
    SQLQuery += "SELECT idcliente FROM presupuesto WHERE refpresupuesto = '" +m_referencia+ "'";
    SQLQuery += "UNION SELECT idcliente FROM pedidocliente   WHERE refpedidocliente = '" +m_referencia+ "'";
    SQLQuery += "UNION SELECT idcliente FROM albaran         WHERE refalbaran       = '" +m_referencia+ "'";
    SQLQuery += "UNION SELECT idcliente FROM factura         WHERE reffactura       = '" +m_referencia+ "'";
    SQLQuery += "UNION SELECT idcliente FROM pedidoproveedor WHERE refpedidoproveedor = '" +m_referencia+ "'";
    SQLQuery += "UNION SELECT idcliente FROM albaranp        WHERE refalbaranp = '" +m_referencia+ "'";
    SQLQuery += "UNION SELECT idcliente FROM facturap        WHERE reffacturap = '" +m_referencia+ "'";
    SQLQuery += "UNION SELECT idcliente FROM cobro           WHERE refcobro    = '" +m_referencia+ "'";
    SQLQuery += "UNION SELECT idcliente FROM pago            WHERE refpago    = '" +m_referencia+ "'";
    SQLQuery += ")";  
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    while (!cur->eof() ) {
        buff.replace("[nomcliente]", cur->valor("nomcliente"));
        buff.replace("[telcliente]", cur->valor("telcliente"));
        buff.replace("[dircliente]", cur->valor("dircliente"));
        buff.replace("[poblcliente]", cur->valor("poblcliente"));
        cur->siguienteregistro();
    } // end while
    delete cur;


    /// Generacion del informe de ventas.
    fitxersortidatxt = "<spacer length=\"15\"/>";
    fitxersortidatxt += "<para>Resumen de ventas por articulo</para>\n";
    fitxersortidatxt += "<blockTable style=\"tablaresumen\" colWidths=\"7cm, 3cm, 3cm, 3cm, 3cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "	<td>" + QApplication::translate("InformeReferencia", "Articulo") + "</td>\n";
    fitxersortidatxt += "	<td>" + QApplication::translate("InformeReferencia", "Pres.") + "</td>\n";
    fitxersortidatxt += "	<td>" + QApplication::translate("InformeReferencia", "Pedido") + "</td>\n";
    fitxersortidatxt += "	<td>" + QApplication::translate("InformeReferencia", "Entregado") + "</td>\n";
    fitxersortidatxt += "	<td>" + QApplication::translate("InformeReferencia", "Facturado") + "</td>\n";
    fitxersortidatxt += "</tr>\n";

 

    SQLQuery = " SELECT * FROM articulo ";
    SQLQuery += " LEFT JOIN (SELECT idarticulo, SUM(cantlpresupuesto) AS cantlpresupuestot  FROM lpresupuesto WHERE idpresupuesto IN (SELECT idpresupuesto FROM presupuesto WHERE refpresupuesto = '" + m_referencia + "') GROUP BY idarticulo) AS t1 ON t1.idarticulo = articulo.idarticulo ";
    SQLQuery += " LEFT JOIN (SELECT idarticulo, SUM(cantlpedidocliente) AS cantlpedidoclientet  FROM lpedidocliente WHERE idpedidocliente IN (SELECT idpedidocliente FROM pedidocliente WHERE refpedidocliente = '" + m_referencia + "') GROUP BY idarticulo) AS t2 ON t2.idarticulo = articulo.idarticulo ";
    SQLQuery += " LEFT JOIN (SELECT idarticulo, SUM(cantlalbaran) AS cantlalbarant  FROM lalbaran WHERE idalbaran IN (SELECT idalbaran FROM albaran WHERE refalbaran = '" + m_referencia + "') GROUP BY idarticulo) AS t3 ON t3.idarticulo = articulo.idarticulo ";
    SQLQuery += " LEFT JOIN (SELECT idarticulo, SUM(cantlfactura) AS cantlfacturat  FROM lfactura WHERE idfactura IN (SELECT idfactura FROM factura WHERE reffactura = '" + m_referencia + "') GROUP BY idarticulo) AS t4 ON t4.idarticulo = articulo.idarticulo ";
    SQLQuery += " WHERE  (cantlpresupuestot <>0 OR cantlpedidoclientet <> 0 OR cantlalbarant <> 0 OR cantlfacturat <> 0) ";

    cur = companyact->cargacursor(SQLQuery);
    while (!cur->eof() ) {
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "<td>" + cur->valor("nomarticulo") + "</td>\n";
        fitxersortidatxt += "<td>" + cur->valor("cantlpresupuestot") + "</td>\n";
        fitxersortidatxt += "<td>" + cur->valor("cantlpedidoclientet") + "</td>\n";
        fitxersortidatxt += "<td>" + cur->valor("cantlalbarant") + "</td>\n";
        fitxersortidatxt += "<td>" + cur->valor("cantlfacturat") + "</td>\n";
        fitxersortidatxt += "</tr>\n";
        cur->siguienteregistro();
    } // end while
    delete cur;

    fitxersortidatxt += "</blockTable>\n";

    buff.replace("[referencia]", m_referencia);

    fitxersortidatxt += "<spacer length=\"15\"/>";
    fitxersortidatxt += "<para>Resumen de compras por articulo</para>\n";
    /// Generacion del informe de compras.
    fitxersortidatxt += "<blockTable style=\"tablaresumen\" colWidths=\"10cm, 3cm, 3cm, 3cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "	<td>" + QApplication::translate("InformeReferencia", "Articulo") + "</td>\n";
    fitxersortidatxt += "	<td>" + QApplication::translate("InformeReferencia", "Pedido") + "</td>\n";
    fitxersortidatxt += "	<td>" + QApplication::translate("InformeReferencia", "Entregado") + "</td>\n";
    fitxersortidatxt += "	<td>" + QApplication::translate("InformeReferencia", "Facturado") + "</td>\n";
    fitxersortidatxt += "</tr>\n";

    SQLQuery = " SELECT * FROM articulo ";
    SQLQuery += " LEFT JOIN (SELECT idarticulo, SUM(cantlpedidoproveedor) AS cantlpedidoproveedort  FROM lpedidoproveedor WHERE idpedidoproveedor IN (SELECT idpedidoproveedor FROM pedidoproveedor WHERE refpedidoproveedor = '" + m_referencia + "') GROUP BY idarticulo) AS t2 ON t2.idarticulo = articulo.idarticulo ";

    SQLQuery += " LEFT JOIN (SELECT idarticulo, SUM(cantlalbaranp) AS cantlalbaranpt  FROM lalbaranp WHERE idalbaranp IN (SELECT idalbaranp FROM albaranp WHERE refalbaranp = '" + m_referencia + "') GROUP BY idarticulo) AS t3 ON t3.idarticulo = articulo.idarticulo ";

    SQLQuery += " LEFT JOIN (SELECT idarticulo, SUM(cantlfacturap) AS cantlfacturapt  FROM lfacturap WHERE idfacturap IN (SELECT idfacturap FROM facturap WHERE reffacturap = '" + m_referencia + "') GROUP BY idarticulo) AS t4 ON t4.idarticulo = articulo.idarticulo ";
    SQLQuery += " WHERE  ( cantlpedidoproveedort <> 0 OR cantlalbaranpt <> 0 OR cantlfacturapt <> 0) ";

    fprintf(stdout,"%s\n",SQLQuery.ascii());

    cur = companyact->cargacursor(SQLQuery);
    while (!cur->eof() ) {
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "<td>" + cur->valor("nomarticulo") + "</td>\n";
        fitxersortidatxt += "<td>" + cur->valor("cantlpedidoproveedort") + "</td>\n";
        fitxersortidatxt += "<td>" + cur->valor("cantlalbaranpt") + "</td>\n";
        fitxersortidatxt += "<td>" + cur->valor("cantlfacturapt") + "</td>\n";
        fitxersortidatxt += "</tr>\n";
        cur->siguienteregistro();
    } // end while
    delete cur;

    fitxersortidatxt += "</blockTable>\n";


    buff.replace("[story]", fitxersortidatxt);

    fitxersortidatxt = "<spacer length=\"15\"/>";
    /// Generacion del informe de totales de ventas.
    fitxersortidatxt += "<para>Totales ventas</para>\n";
    fitxersortidatxt += "<blockTable style=\"tablatotales\" colWidths=\"3cm, 3cm, 3cm, 3cm, 3cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "    <td>" + QApplication::translate("InformeReferencia", "Pres.") + "</td>\n";
    fitxersortidatxt += "    <td>" + QApplication::translate("InformeReferencia", "Pedido") + "</td>\n";
    fitxersortidatxt += "    <td>" + QApplication::translate("InformeReferencia", "Entregado") + "</td>\n";
    fitxersortidatxt += "    <td>" + QApplication::translate("InformeReferencia", "Facturado") + "</td>\n";
    fitxersortidatxt += "    <td>" + QApplication::translate("InformeReferencia", "Cobrado") + "</td>\n";
    fitxersortidatxt += "</tr>\n";

    /// Calculo de las cantidades totales en moneda.
    fitxersortidatxt += "<tr>\n";

    /// Total presupuestado.
    SQLQuery = "SELECT SUM(totalpresupuesto) AS tpres FROM presupuesto WHERE refpresupuesto='" + m_referencia + "' ";
    cur = companyact->cargacursor(SQLQuery);
    fitxersortidatxt += "    <td>" + cur->valor("tpres") + "</td>\n";
    delete cur;

    /// Total pedido.
    SQLQuery = "SELECT SUM(totalpedidocliente) AS tpedcli FROM pedidocliente WHERE refpedidocliente='" + m_referencia + "' ";
    cur = companyact->cargacursor(SQLQuery);
    fitxersortidatxt += "    <td>" + cur->valor("tpedcli") + "</td>\n";
    delete cur;

    /// Total trabajado.
    SQLQuery = "SELECT SUM(totalalbaran) AS talb FROM albaran WHERE refalbaran='" + m_referencia + "' ";
    cur = companyact->cargacursor(SQLQuery);
    fitxersortidatxt += "    <td>" + cur->valor("talb") + "</td>\n";
    delete cur;

    /// Total facturado.
    SQLQuery = "SELECT SUM(totalfactura) AS tfact FROM factura WHERE reffactura='" + m_referencia + "' ";
    cur = companyact->cargacursor(SQLQuery);
    fitxersortidatxt += "    <td>" + cur->valor("tfact") + "</td>\n";
    delete cur;

    /// Total cobrado.
    SQLQuery = "SELECT SUM(cantcobro) AS tcobro FROM cobro WHERE refcobro = '" + m_referencia + "'";
    cur = companyact->cargacursor(SQLQuery);
    fitxersortidatxt += "     <td>" + cur->valor("tcobro") + "</td>\n";
    delete cur;

    fitxersortidatxt += "</tr>\n";
    fitxersortidatxt += "</blockTable>\n";

    fitxersortidatxt += "<spacer length=\"15\"/>";
    /// Generacion del informe de totales de compras.
    fitxersortidatxt += "<para>Totales compras</para>\n";
    fitxersortidatxt += "<blockTable style=\"tablatotales\" colWidths=\" 4cm, 4cm, 4cm, 3cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "    <td>" + QApplication::translate("InformeReferencia", "Pedido") + "</td>\n";
    fitxersortidatxt += "    <td>" + QApplication::translate("InformeReferencia", "Entregado") + "</td>\n";
    fitxersortidatxt += "    <td>" + QApplication::translate("InformeReferencia", "Facturado") + "</td>\n";
    fitxersortidatxt += "    <td>" + QApplication::translate("InformeReferencia", "Pagado") + "</td>\n";
    fitxersortidatxt += "</tr>\n";

    /// Calculo de las cantidades totales en moneda.
    fitxersortidatxt += "<tr>\n";

    /// Total pedido.
    SQLQuery = "SELECT SUM(totalpedidoproveedor) AS tpedpro FROM pedidoproveedor WHERE refpedidoproveedor='" + m_referencia + "' ";
    cur = companyact->cargacursor(SQLQuery);
    fitxersortidatxt += "    <td>" + cur->valor("tpedpro") + "</td>\n";
    delete cur;

    /// Total trabajado.
    SQLQuery = "SELECT SUM(totalalbaranp) AS talbp FROM albaranp WHERE refalbaranp='" + m_referencia + "' ";
    cur = companyact->cargacursor(SQLQuery);
    fitxersortidatxt += "    <td>" + cur->valor("talbp") + "</td>\n";
    delete cur;

    /// Total facturado.
    SQLQuery = "SELECT SUM(totalfacturap) AS tfactp FROM facturap WHERE reffacturap='" + m_referencia + "' ";
    cur = companyact->cargacursor(SQLQuery);
    fitxersortidatxt += "    <td>" + cur->valor("tfactp") + "</td>\n";
    delete cur;

    /// Total cobrado.
    SQLQuery = "SELECT SUM(cantpago) AS tpago FROM pago WHERE refpago = '" + m_referencia + "'";
    cur = companyact->cargacursor(SQLQuery);
    fitxersortidatxt += "     <td>" + cur->valor("tpago") + "</td>\n";
    delete cur;

    fitxersortidatxt += "</tr>\n";
    fitxersortidatxt += "</blockTable>\n";


    buff.replace("[totales]", fitxersortidatxt);


    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    } // end if
    invocaPDF("informereferencia");
}

