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

#include "albaranproveedor.h"


AlbaranProveedor::AlbaranProveedor(company *comp) : DBRecord(comp)
{
	companyact = comp;
	setDBTableName("albaranp");
	setDBCampoId("idalbaranp");
	addDBCampo("idalbaranp", DBCampo::DBint, DBCampo::DBPrimaryKey,
			"Identificador Presupuesto");
	addDBCampo("numalbaranp", DBCampo::DBint, DBCampo::DBNothing,
			"Identificador Presupuesto");
	addDBCampo("fechaalbaranp", DBCampo::DBdate, DBCampo::DBNothing,
			"Identificador Presupuesto");
	addDBCampo("comentalbaranp", DBCampo::DBvarchar, DBCampo::DBNothing,
			"Identificador Presupuesto");
	addDBCampo("idproveedor", DBCampo::DBint, DBCampo::DBNotNull,
			"Identificador Presupuesto");
	addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing,
			"Identificador Presupuesto");
	addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull,
			"Identificador Presupuesto");
	addDBCampo("refalbaranp", DBCampo::DBvarchar, DBCampo::DBNothing,
			"Identificador Presupuesto");
	addDBCampo("descalbaranp", DBCampo::DBvarchar, DBCampo::DBNothing,
			"Identificador Presupuesto");
};


AlbaranProveedor::~AlbaranProveedor()
{
};


int AlbaranProveedor::borrar()  {
	if (DBvalue("idalbaranp") != "")
	{
		listalineas->borrar();
		listadescuentos->borrar();
		companyact->begin();
		int error = companyact->ejecuta("DELETE FROM albaranp WHERE idalbaranp=" +
				DBvalue("idalbaranp"));
		if (error)
		{
			companyact->rollback();
			return -1;
		};

		companyact->commit();
		vaciaAlbaranProveedor();
		pintaAlbaranProveedor();
	};
	return 0;
}


void AlbaranProveedor::vaciaAlbaranProveedor()  {
	DBclear();
}


void AlbaranProveedor::pintaAlbaranProveedor()
{
	_depura("pintaAlbaranProveedor\n", 0);
	pintaidalbaranp(DBvalue("idalbaranp"));
	pintanumalbaranp(DBvalue("numalbaranp"));
	pintafechaalbaranp(DBvalue("fechaalbaranp"));
	pintacomentalbaranp(DBvalue("comentalbaranp"));
	pintaidproveedor(DBvalue("idproveedor"));
	pintaidforma_pago(DBvalue("idforma_pago"));
	pintaidalmacen(DBvalue("idalmacen"));
	pintarefalbaranp(DBvalue("refalbaranp"));
	pintadescalbaranp(DBvalue("descalbaranp"));
	/// Pinta el subformulario de detalle del AlbaranProveedor.
	listalineas->pintaListLinAlbaranProveedor();
	listadescuentos->pintaListDescuentoAlbaranProv();
	/// Pintamos los totales.
	pintatotales(listalineas->calculabase(), listalineas->calculaiva());
};


/// Esta funcion carga un AlbaranProveedor.
int AlbaranProveedor::cargar(QString idbudget)
{
	_depura("AlbaranProveedor::cargaAlbaranProveedor()\n", 0);
	QString query = "SELECT * FROM albaranp WHERE idalbaranp=" + idbudget;
	cursor2 * cur = companyact->cargacursor(query);

	if (!cur->eof())
	{
		DBload(cur);
	};

	delete cur;
	listalineas->cargaListLinAlbaranProveedor(idbudget);
	listadescuentos->cargaDescuentos(idbudget);
	pintaAlbaranProveedor();
	return 0;
};


void AlbaranProveedor::guardaAlbaranProveedor()
{
	QString id;
	companyact->begin();
	int error = DBsave(id);

	if (error)
	{
		companyact->rollback();
		return;
	};
    
	companyact->commit();
	setidalbaranp(id);
	listalineas->guardaListLinAlbaranProveedor();
	listadescuentos->guardaListDescuentoAlbaranProv();
	cargar(id);
};


void AlbaranProveedor::imprimirAlbaranProveedor()
{
	/// Copiamos el archivo.
	QString archivo=confpr->valor(CONF_DIR_OPENREPORTS) + "albaranpproveedor.rml";
	archivo = "cp " + archivo + " /tmp/albaranpproveedor.rml";
	system(archivo.ascii());
	/// Copiamos el logo.
	archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";
	archivo = "cp " + archivo + " /tmp/logo.jpg";
	system(archivo.ascii());

	QFile file;
	file.setName("/tmp/albaranpproveedor.rml");
	file.open(QIODevice::ReadOnly);
	QTextStream stream(&file);
	QString buff = stream.read();
	file.close();
	QString fitxersortidatxt;
	/// Linea de totales del presupuesto.
	QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor=" +
			DBvalue("idproveedor");
	cursor2 *cur = companyact->cargacursor(SQLQuery);

	if (!cur->eof())
	{
		buff.replace("[dirproveedor]", cur->valor("dirproveedor"));
		buff.replace("[poblproveedor]", cur->valor("poblproveedor"));
		buff.replace("[telproveedor]", cur->valor("telproveedor"));
		buff.replace("[nomproveedor]", cur->valor("nomproveedor"));
		buff.replace("[cifproveedor]", cur->valor("cifproveedor"));
	};

	buff.replace("[numalbaranp]", DBvalue("numalbaranp"));
	buff.replace("[fechaalbaranp]", DBvalue("fechaalbaranp"));
	buff.replace("[comentalbaranp]", DBvalue("comentalbaranp"));
	buff.replace("[descalbaranp]", DBvalue("descalbaranp"));
	buff.replace("[refalbaranp]", DBvalue("refalbaranp"));

	fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"10cm, 2cm, 2cm, " \
				"3cm\" repeatRows=\"1\">";
	fitxersortidatxt += "<tr>";
	fitxersortidatxt += "<td>Concepto</td>";
	fitxersortidatxt += "<td>Cantidad</td>";
	fitxersortidatxt += "<td>Precio Und.</td>";
	fitxersortidatxt += "<td>Total</td>";
	fitxersortidatxt += "</tr>";

	QString l;
	LinAlbaranProveedor *linea;
	uint i = 0;

	for (linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next())
	{
		fitxersortidatxt += "<tr>";
		fitxersortidatxt += "<td>" + linea->desclalbaranp() + "</td>";
		fitxersortidatxt += "<td>" + l.sprintf("%2.2f", linea->cantlalbaranp().toFloat()) +
					"</td>";
		fitxersortidatxt += "<td>" + l.sprintf("%2.2f", linea->pvplalbaranp().toFloat()) +
					"</td>";
		fitxersortidatxt += "<td>" + l.sprintf("%2.2f", linea->cantlalbaranp().toFloat()
					* linea->pvplalbaranp().toFloat()) + "</td>";
		fitxersortidatxt += "</tr>";
		i++;
	};

	fitxersortidatxt += "<tr>";
	fitxersortidatxt += "<td></td>";
	fitxersortidatxt += "<td></td>";
	fitxersortidatxt += "<td>Base</td>";
	fitxersortidatxt += "<td>" + l.sprintf("%2.2f", listalineas->calculabase()) + "</td>";
	fitxersortidatxt += "</tr>";
	fitxersortidatxt += "<tr>";
	fitxersortidatxt += "<td></td>";
	fitxersortidatxt += "<td></td>";
	fitxersortidatxt += "<td>Iva</td>";
	fitxersortidatxt += "<td>" + l.sprintf("%2.2f", listalineas->calculaiva()) + "</td>";
	fitxersortidatxt += "</tr>";
	fitxersortidatxt += "<tr>";
	fitxersortidatxt += "<td></td>";
	fitxersortidatxt += "<td></td>";
	fitxersortidatxt += "<td>Total</td>";
	fitxersortidatxt += "<td>" + l.sprintf("%2.2f", listalineas->calculabase() +
				listalineas->calculaiva()) + "</td>";
	fitxersortidatxt += "</tr>";
	fitxersortidatxt += "</blockTable>";

	buff.replace("[story]", fitxersortidatxt);

	if (file.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&file);
		stream << buff;
		file.close();
	};

	system("trml2pdf.py /tmp/albaranpproveedor.rml > /tmp/albaranpproveedor.pdf");
	system("kpdf /tmp/albaranpproveedor.pdf &");
};