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
};


AlbaranCliente::~AlbaranCliente()
{
};


void AlbaranCliente::borraAlbaranCliente()
{
	if (DBvalue("idalbaran") != "")
	{
		listalineas->borrar();
		listadescuentos->borrar();
		companyact->begin();
		int error = companyact->ejecuta("DELETE FROM albaran WHERE idalbaran=" + 
							DBvalue("idalbaran"));
		if (error)
		{
			companyact->rollback();
			return;
		}
		companyact->commit();
		vaciaAlbaranCliente();
		pintaAlbaranCliente();
	}
};


void AlbaranCliente::vaciaAlbaranCliente()
{
	DBclear();
};


void AlbaranCliente::pintaAlbaranCliente()
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
	listalineas->pintaListLinAlbaranCliente();
	listadescuentos->pintaListDescuentoAlbaranCliente();
	/// Pintamos los totales
	calculaypintatotales();
};


/// Esta funcioncarga un AlbaranCliente.
int AlbaranCliente::cargar(QString idbudget)
{
	_depura("AlbaranCliente::cargaAlbaranCliente(" + idbudget + ").", 0);
	QString query = "SELECT * FROM albaran WHERE idalbaran=" + idbudget;
	cursor2 * cur= companyact->cargacursor(query);
	if (!cur->eof())
	{
		DBload(cur);
	}
	delete cur;
	/// Si no existe lista de lineas se crea una.
	if (listalineas == NULL)
	{
		listalineas = new ListLinAlbaranCliente(companyact);
	}
	listalineas->cargaListLinAlbaranCliente(idbudget);
	/// Si no existe lista de descuentos se crea una.
	if (listadescuentos == NULL)
	{
		listadescuentos = new ListDescuentoAlbaranCliente(companyact);
	}
	listadescuentos->cargaDescuentos(idbudget);
	pintaAlbaranCliente();
	_depura("Fin AlbaranCliente::cargaAlbaranCliente(" + idbudget + ").", 0);
	return 0;
};


void AlbaranCliente::guardaAlbaranCliente()
{
	/// Todo el guardado es una transaccion.
	QString id;
	companyact->begin();
	int error = DBsave(id);
	if (error)
	{
		companyact->rollback();
		return;
	}
	setidalbaran(id);
	companyact->commit();
	listalineas->guardaListLinAlbaranCliente();
	listadescuentos->guardaListDescuentoAlbaranCliente();
	cargar(id);
};


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
	LinAlbaranCliente *linea;
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
	for (linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next())
	{
		Fixed base = Fixed(linea->cantlalbaran().ascii()) *
				Fixed(linea->pvplalbaran().ascii());
		basesimp[linea->ivalalbaran()] = basesimp[linea->ivalalbaran()] + base -
				base * Fixed(linea->descontlalbaran().ascii()) / 100;

		fitxersortidatxt += "<tr>\n";
		fitxersortidatxt += "<td>" + XMLProtect(linea->codigocompletoarticulo()) +
					"</td>\n";
		fitxersortidatxt += "<td>" + XMLProtect(linea->desclalbaran()) + "</td>\n";
		fitxersortidatxt += "<td>" + l.sprintf("%s",linea->cantlalbaran().ascii()) +
					"</td>\n";
		fitxersortidatxt += "<td>" + l.sprintf("%s",linea->pvplalbaran().ascii()) +
					"</td>\n";
		fitxersortidatxt += "<td>" + l.sprintf("%s",linea->descontlalbaran().ascii()) +
					" %</td>\n";
		fitxersortidatxt += "<td>" + l.sprintf("%s",(base - base *
					Fixed(linea->descontlalbaran()) / 100).toQString().ascii())
					+ "</td>\n";
		fitxersortidatxt += "</tr>";
		i++;
	};

	while (i++ < 15)
	{
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
	DescuentoAlbaranCliente *linea1;
	if (listadescuentos->m_lista.first())
	{
		fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm," \
					" 2cm, 3cm\" repeatRows=\"1\">\n";
		fitxersortidatxt += "<tr>\n";
		fitxersortidatxt += "<td>Descuento</td>\n";
		fitxersortidatxt += "<td>Porcentaje</td>\n";
		fitxersortidatxt += "<td>Total</td>\n";
		fitxersortidatxt += "</tr>\n";
		for (linea1 = listadescuentos->m_lista.first(); linea1;
			linea1 = listadescuentos->m_lista.next())
		{
			porcentt = porcentt + Fixed(linea1->proporciondalbaran().ascii());
			fitxersortidatxt += "<tr>\n";
			fitxersortidatxt += "<td>" + linea1->conceptdalbaran() + "</td>\n";
			fitxersortidatxt += "<td>" + l.sprintf("%s",
						linea1->proporciondalbaran().ascii()) +
						" %</td>\n";
			fitxersortidatxt += "<td>" + l.sprintf("-%s",
						(Fixed(linea1->proporciondalbaran()) * basei /
						100).toQString().ascii()) + "</td>\n";
			fitxersortidatxt += "</tr>";
		};
		fitxersortidatxt += "</blockTable>\n";
	};
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
	};

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
	};

	tr1 += "<td>Total </td>\n";
	tr2 += "<td>"+l.sprintf("%s", (totiva + totbaseimp).toQString().ascii()) + "</td>\n";
	fitxersortidatxt += "<tr>" + tr1 + "</tr><tr>" + tr2 + "</tr></blockTable>\n";
	buff.replace("[totales]",fitxersortidatxt);

	if (file.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&file);
		stream << buff;
		file.close();
	};

	invocaPDF("albaran");

};


void AlbaranCliente::calculaypintatotales()
{
	_depura("calculaypintatotales.",0);
	base basesimp;
	LinAlbaranCliente *linea;
	/// Impresion de los contenidos.
	QString l;

	for (linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next())
	{
		Fixed cant(linea->cantlalbaran().ascii());
		Fixed pvpund(linea->pvplalbaran().ascii());
		Fixed desc1(linea->descontlalbaran().ascii());
		Fixed cantpvp = cant * pvpund;
		Fixed base = cantpvp - cantpvp * desc1 / 100;
		basesimp[linea->ivalalbaran()] = basesimp[linea->ivalalbaran()] + base;
	};

	Fixed basei("0.00");
	base::Iterator it;

	for (it = basesimp.begin(); it != basesimp.end(); ++it)
	{
		basei = basei + it.data();
	};

	/// Impresion de los descuentos.
	Fixed porcentt("0.00");
	DescuentoAlbaranCliente *linea1;

	if (listadescuentos->m_lista.first())
	{
		for (linea1 = listadescuentos->m_lista.first(); linea1;
			linea1 = listadescuentos->m_lista.next())
		{
			Fixed propor(linea1->proporciondalbaran().ascii());
			porcentt = porcentt + propor;
		};
	};

	Fixed totbaseimp("0.00");
	Fixed parbaseimp("0.00");

	for (it = basesimp.begin(); it != basesimp.end(); ++it)
	{
		if (porcentt > Fixed("0.00"))
		{
			parbaseimp = it.data() - it.data() * porcentt / 100;
		} else {
			parbaseimp = it.data();
		};
		totbaseimp = totbaseimp + parbaseimp;
	};

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
		};

		totiva = totiva + pariva;
	};

	pintatotales(totiva, totbaseimp, totiva + totbaseimp, basei * porcentt / 100);
};
