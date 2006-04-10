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

#include "fixed.h"
#include "funcaux.h"
#include "albarancliente.h"


typedef QMap<QString, Fixed> base;

AlbaranCliente::AlbaranCliente(company *comp) : DBRecord (comp)
{
	_depura("AlbaranCliente::AlbaranCliente(company *)", 0);
	companyact = comp;
	setDBTableName("albaran");
	setDBCampoId("idalbaran");
	addDBCampo("idalbaran", DBCampo::DBint, DBCampo::DBPrimaryKey,
			"Identificador Presupuesto");
	addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull,
			"Identificador Presupuesto");
	addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull,
			"Identificador Presupuesto");
	addDBCampo("numalbaran", DBCampo::DBint, DBCampo::DBNothing,
			"Identificador Presupuesto");
	addDBCampo("fechaalbaran", DBCampo::DBdate, DBCampo::DBNothing,
			"Identificador Presupuesto");
	addDBCampo("contactalbaran", DBCampo::DBvarchar, DBCampo::DBNothing,
			"Identificador Presupuesto");
	addDBCampo("telalbaran", DBCampo::DBvarchar, DBCampo::DBNothing,
			"Identificador Presupuesto");
	addDBCampo("comentalbaran", DBCampo::DBvarchar, DBCampo::DBNothing,
			"Identificador Presupuesto");
	addDBCampo("comentprivalbaran", DBCampo::DBvarchar, DBCampo::DBNothing,
			"Identificador Presupuesto");
	addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing,
			"Identificador Presupuesto");
	addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing,
			"Identificador Presupuesto");
	addDBCampo("procesadoalbaran", DBCampo::DBboolean, DBCampo::DBNothing,
			"Identificador Presupuesto");
	addDBCampo("descalbaran", DBCampo::DBvarchar, DBCampo::DBNothing,
			"Identificador Presupuesto");
	addDBCampo("refalbaran", DBCampo::DBvarchar, DBCampo::DBNothing,
			"Referencia Albaran");
	listalineas = NULL;
	listadescuentos = NULL;
}


AlbaranCliente::~AlbaranCliente()
{
}


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


void AlbaranCliente::vaciaAlbaranCliente()
{
	DBclear();
}


void AlbaranCliente::pintar()
{
	_depura("pintaAlbaranCliente.", 0);
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
//	listalineas->pintaListLinAlbaranCliente();
//	listadescuentos->pintaListDescuentoAlbaranCliente();
	/// Pintamos los totales
	calculaypintatotales();
}


/// Esta funcioncarga un AlbaranCliente.
int AlbaranCliente::cargar(QString idbudget)
{
	_depura("AlbaranCliente::cargaAlbaranCliente(" + idbudget + ").", 0);
	QString query = "SELECT * FROM albaran WHERE idalbaran=" + idbudget;
	cursor2 * cur= companyact->cargacursor(query);
	if (!cur->eof())  {
		DBload(cur);
	}
	delete cur;

	listalineas->cargar(idbudget);
	/// Si no existe lista de descuentos se crea una.

	listadescuentos->cargar(idbudget);
	pintar();
	_depura("Fin AlbaranCliente::cargaAlbaranCliente(" + idbudget + ").", 0);
	return 0;
}


int AlbaranCliente::guardar() {
	/// Todo el guardado es una transaccion.
	QString id;
	companyact->begin();
	int error = DBsave(id);
	if (error)  {
		companyact->rollback();
		return -1;
	}
	setidalbaran(id);
	companyact->commit();
	listalineas->guardar();
	listadescuentos->guardar();
	return 0;
}


void AlbaranCliente::imprimirAlbaranCliente()
{
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
		archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) +
		"logo.jpg";
	#else
		archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) +
		"logo.jpg";
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
	if(!cur->eof())
	{
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
	for (linea = listalineas->lista()->first(); linea; linea = listalineas->lista()->next())
	{
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
	};
	fitxersortidatxt += "</blockTable>\n";
	buff.replace("[story]", fitxersortidatxt);
	Fixed basei("0.00");
	base::Iterator it;
	for (it = basesimp.begin(); it != basesimp.end(); ++it)
	{
		basei = basei + it.data();
	}
	/// Impresion de los descuentos.
	fitxersortidatxt = "";
	Fixed porcentt("0.00");
	SDBRecord *linea1;
	if (listadescuentos->lista()->first())
	{
		fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm," \
					" 2cm, 3cm\" repeatRows=\"1\">\n";
		fitxersortidatxt += "<tr>\n";
		fitxersortidatxt += "<td>Descuento</td>\n";
		fitxersortidatxt += "<td>Porcentaje</td>\n";
		fitxersortidatxt += "<td>Total</td>\n";
		fitxersortidatxt += "</tr>\n";
		for (linea1 = listadescuentos->lista()->first(); linea1;linea1 = listadescuentos->lista()->next())  {
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
		};
		fitxersortidatxt += "</blockTable>\n";
	}
	buff.replace("[descuentos]", fitxersortidatxt);
	/// Impresión de los totales.
	fitxersortidatxt = "";
	/// Rellena el primer tr de titulares
	QString tr1 = "";
	/// Rellena el segundo tr de cantidades
	QString tr2 = "";
	fitxersortidatxt += "<blockTable style=\"tablatotales\">\n";
	Fixed totbaseimp("0.00");
	Fixed parbaseimp("0.00");

	for (it = basesimp.begin(); it != basesimp.end(); ++it)
	{
		if (porcentt > 0)
		{
			parbaseimp = it.data() - it.data() * porcentt / 100;
		} else {
			parbaseimp = it.data();
		};

		totbaseimp = totbaseimp + parbaseimp;
		tr1 += "<td>Base " + it.key() + " %</td>\n";
		tr2 += "<td>" + l.sprintf("%s", parbaseimp.toQString().ascii()) + "</td>\n";
	}

	Fixed totiva("0.0");
	Fixed pariva("0.0");

	for (it = basesimp.begin(); it != basesimp.end(); ++it)
	{
		if (porcentt > 0)
		{
			pariva = (it.data() - it.data() * porcentt / 100) * Fixed(it.key()) / 100;
		} else {
			pariva = it.data() * Fixed(it.key()) / 100;
		};
		totiva = totiva + pariva;
		tr1 += "<td>Iva " + it.key() + " %</td>\n";
		tr2 += "<td>" + l.sprintf("%s", pariva.toQString().ascii()) + "</td>\n";
	}

	tr1 += "<td>Total </td>\n";
	tr2 += "<td>"+l.sprintf("%s", (totiva + totbaseimp).toQString().ascii()) + "</td>\n";
	fitxersortidatxt += "<tr>" + tr1 + "</tr><tr>" + tr2 + "</tr></blockTable>\n";
	buff.replace("[totales]",fitxersortidatxt);

	if (file.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&file);
		stream << buff;
		file.close();
	}

	invocaPDF("albaran");
}


void AlbaranCliente::calculaypintatotales()  {
	_depura("calculaypintatotales.",0);
	base basesimp;
	SDBRecord *linea;
	/// Impresion de los contenidos.
	QString l;

	for (linea = listalineas->lista()->first(); linea; linea = listalineas->lista()->next())
	{
		Fixed cant(linea->DBvalue("cantlalbaran").ascii());
		Fixed pvpund(linea->DBvalue("pvplalbaran").ascii());
		Fixed desc1(linea->DBvalue("descontlalbaran").ascii());
		Fixed cantpvp = cant * pvpund;
		Fixed base = cantpvp - cantpvp * desc1 / 100;
		basesimp[linea->DBvalue("ivalalbaran")] = basesimp[linea->DBvalue("ivalalbaran")] + base;
	}

	Fixed basei("0.00");
	base::Iterator it;

	for (it = basesimp.begin(); it != basesimp.end(); ++it)  {
		basei = basei + it.data();
	}

	/// Impresion de los descuentos.
	Fixed porcentt("0.00");
	SDBRecord *linea1;

	if (listadescuentos->lista()->first())	{
		for (linea1 = listadescuentos->lista()->first(); linea1;linea1 = listadescuentos->lista()->next())  {
			Fixed propor(linea1->DBvalue("proporciondalbaran").ascii());
			porcentt = porcentt + propor;
		}// end for
	}// end if

	Fixed totbaseimp("0.00");
	Fixed parbaseimp("0.00");

	for (it = basesimp.begin(); it != basesimp.end(); ++it)
	{
		if (porcentt > Fixed("0.00"))
		{
			parbaseimp = it.data() - it.data() * porcentt / 100;
		} else {
			parbaseimp = it.data();
		}
		totbaseimp = totbaseimp + parbaseimp;
	}

	Fixed totiva("0.00");
	Fixed pariva("0.00");

	for (it = basesimp.begin(); it != basesimp.end(); ++it)
	{
		Fixed piva(it.key().ascii());

		if (porcentt > Fixed("0.00"))
		{
			pariva = (it.data() - it.data() * porcentt / 100) * piva / 100;
		} else {
			pariva = it.data() * piva / 100;
		}

		totiva = totiva + pariva;
	}

	pintatotales(totiva, totbaseimp, totiva + totbaseimp, basei * porcentt / 100);
}

