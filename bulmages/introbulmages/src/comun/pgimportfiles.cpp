/***************************************************************************
 *   Copyright (C) 2003 by Santiago Capel                                  *
 *   Santiago Capel Torres (GestiONG)                                      *
 *   Tomeu Borrás Riera                                                    *
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
 ***************************************************************************/
#include <qdatetime.h>
#include "pgimportfiles.h"
#include <qtextstream.h>
#include <qobject.h>
#include <qstring.h>
#include <qdatetime.h>
#include <stdio.h>
#include <qstring.h>

#define EURO  166.386
#define S_EURO  "166.386"

#define LEN_CODIGO_CUENTA 12
#define LEN_TITULO  40
#define LEN_NIF  15
#define LEN_DOMICILIO  35
#define LEN_POBLACION  25
#define LEN_PROVINCIA  20
#define LEN_CODPOSTAL  5
#define LEN_DIVISA  1
#define LEN_CTA_CODDIVISA  5
#define LEN_CTA_DOCUMENTO  1
#define LEN_AJUSTAME  1
#define LEN_TIPOIVA  1


#define LEN_ASIEN  6
#define LEN_FECHA  8
#define LEN_SUBCTA  12
#define LEN_CONTRA  12
#define LEN_PTADEBE  16
#define LEN_CONCEPTO  25
#define LEN_PTAHABER  16
#define LEN_FACTURA  8
#define LEN_BASEIMPO  16
#define LEN_IVA  5
#define LEN_RECEQUIV  5
#define LEN_DOCUMENTO  10
#define LEN_DEPARTA  3
#define LEN_CLAVE  6
#define LEN_ESTADO  1
#define LEN_NCASADO  6
#define LEN_TCASADO  1
#define LEN_TRANS  6
#define LEN_CAMBIO  16
#define LEN_DEBEME  16
#define LEN_HABERME  16
#define LEN_AUXILIAR  1
#define LEN_SERIE  1
#define LEN_SUCURSAL  4
#define LEN_CODDIVISA  5
#define LEN_IMPAUXME  16
#define LEN_MONEDAUSO  1
#define LEN_EURODEBE  16
#define LEN_EUROHABER  16
#define LEN_BASEEURO  16
#define LEN_NOCONV  1
#define LEN_NUMEROINV  10


pgimportfiles::pgimportfiles(postgresiface2 *con, void (*func)(int,int), void (*func1) (QString)) {
	conexionbase = con;
	alerta = func;
	mensajeria = func1;
	m_fInicial="";
	m_fFinal="";
	setModoNormal();
}// end pgimportfiles

/** \brief Esta función se encarga de pasar los datos de BulmaGés a Contaplus.
  */
int pgimportfiles::bulmages2Contaplus(QFile &subcuentas, QFile &asientos) {
	QString codigo, descripcion;
	QString strblancomax;
	QTextStream stream( &subcuentas );
	QTextStream streamas( &asientos );
	// Se supone que ho hay campos mayores de 100 caracteres para que el algoritmo funcione.
	strblancomax.fill(' ',100);
	/// Sólo se van a exportar las cuentas utilizadas, Ya que contaplus no hace ordenación en árbol.
	QString query = "SELECT * FROM cuenta WHERE idcuenta IN (SELECT DISTINCT idcuenta FROM apunte)";
	conexionbase->begin();
	cursor2 *curcta = conexionbase->cargacursor(query,"elquery");
	conexionbase->commit();
	while (!curcta->eof()) {
		QString linea="";
		linea += (curcta->valor("codigo")        +strblancomax).left(LEN_CODIGO_CUENTA);
		linea += (curcta->valor("descripcion")   +strblancomax).left(LEN_TITULO);
		linea += (curcta->valor("cifent_cuenta") +strblancomax).left(LEN_NIF);
		linea += (curcta->valor("dirent_cuenta") +strblancomax).left(LEN_DOMICILIO);
		linea += (                                strblancomax).left(LEN_POBLACION);
		linea += (                                strblancomax).left(LEN_CODPOSTAL);
		linea += (                                strblancomax).left(LEN_DIVISA);
		linea += (                                strblancomax).left(LEN_CTA_CODDIVISA);
		linea += (                                strblancomax).left(LEN_CTA_DOCUMENTO);
		linea += (                                strblancomax).left(LEN_AJUSTAME);
		linea += (                                strblancomax).left(LEN_TIPOIVA);
		linea += "\n";		
		stream << linea;
		curcta->siguienteregistro();
	}// end while
	delete curcta;
	query = "SELECT * FROM asiento, apunte, cuenta WHERE asiento.idasiento = apunte.idasiento AND cuenta.idcuenta = apunte.idcuenta ";
	if (m_fInicial != "") 
		query += " AND asiento.fecha >= '"+m_fInicial+"'";
	if (m_fFinal != "") 
		query += " AND asiento.fecha <= '"+m_fFinal+"'";
	query +=" ORDER BY asiento.idasiento ";
	conexionbase->begin();
	cursor2 *curas = conexionbase->cargacursor(query, "masquery");
	conexionbase->commit();
	int i =0;
	int numreg = curas->numregistros()+1;
	while (!curas->eof()) {
		alerta(i++,numreg);    
		QString linea = "";
		linea += (strblancomax + curas->valor("ordenasiento")).right(LEN_ASIEN);
		QString fecha = curas->valor("fecha");
		fecha = fecha.mid(6,4)+fecha.mid(3,2)+fecha.mid(0,2);
		linea += (fecha                            +strblancomax).left(LEN_FECHA);
		linea += (curas->valor("codigo")           +strblancomax).left(LEN_SUBCTA);
		linea += (                                  strblancomax).left(LEN_CONTRA);
		QString cadaux;
		cadaux.sprintf("%2.2f", curas->valor("debe").toFloat());
		linea += (strblancomax + cadaux).right(LEN_PTADEBE);
		linea += (curas->valor("conceptocontable") +strblancomax).left(LEN_CONCEPTO);
		cadaux.sprintf("%2.2f", curas->valor("haber").toFloat());
		linea += (strblancomax + cadaux).right(LEN_PTAHABER);
		linea += (                                  strblancomax).left(LEN_FACTURA);
		linea += (                                  strblancomax).left(LEN_BASEIMPO);
		linea += (                                  strblancomax).left(LEN_IVA);
		linea += (                                  strblancomax).left(LEN_RECEQUIV);
		linea += (                                  strblancomax).left(LEN_DOCUMENTO);
		linea += (                                  strblancomax).left(LEN_DEPARTA);
		linea += (                                  strblancomax).left(LEN_CLAVE);
		linea += (                                  strblancomax).left(LEN_ESTADO);
		linea += (                                  strblancomax).left(LEN_NCASADO);
		linea += (                                  strblancomax).left(LEN_TCASADO);
		linea += (                                  strblancomax).left(LEN_TRANS);
		linea += (                                  strblancomax).left(LEN_CAMBIO);
		linea += (strblancomax+"0.00").right(LEN_DEBEME);
		linea += (strblancomax+"0.00").right(LEN_HABERME);
		linea += (                                  strblancomax).left(LEN_AUXILIAR);
		linea += (                                  strblancomax).left(LEN_SERIE);
		linea += (                                  strblancomax).left(LEN_SUCURSAL);
		linea += (                                  strblancomax).left(LEN_CODDIVISA);
		linea += (                                  strblancomax).left(LEN_IMPAUXME);
		linea += ("2"                              +strblancomax).left(LEN_MONEDAUSO);
		/// Para evitar redondeos usamos el valor devuelto en forma de texto por la base de datos que ya opera ella en punto fijo
		cadaux.sprintf("%2.2f", curas->valor("debe").toFloat());
		cadaux=curas->valor("debe");
		linea += (strblancomax+cadaux).right(LEN_EURODEBE);
		cadaux.sprintf("%2.2f", curas->valor("haber").toFloat());
		cadaux = curas->valor("haber");
		linea += (strblancomax+cadaux).right(LEN_EUROHABER);
		linea += (strblancomax+"0.00").right(LEN_BASEEURO);
		linea += ( "F"+strblancomax).left(LEN_NOCONV);
		linea += (                                  strblancomax).left(LEN_NUMEROINV);
		linea += "\n";
		mensajeria("Exportando :"+curas->valor("codigo")+"--"+fecha+"\n");
		streamas << linea;
		curas->siguienteregistro();
	}// end while
	delete curas;
	alerta (100,100);
	return 0;
}// end if

int pgimportfiles::contaplus2Bulmages(QFile &subcuentas, QFile &asientos) {
    QString idasiento;
    QString lopd_str, cuenta_str;
    QString debe, haber;
    QString query;
    QDate fecha1;
    QDate fechain(1,1,1);
    QDate fechafi(2999,12,31);
    if (m_fInicial != "")
    	fechain.setYMD(m_fInicial.mid(6,4).toInt(), m_fInicial.mid(3,2).toInt(), m_fInicial.mid(0,2).toInt());
    if (m_fFinal != "")
    	fechafi.setYMD(m_fFinal.mid(6,4).toInt(), m_fFinal.mid(3,2).toInt(), m_fFinal.mid(0,2).toInt());

	// Subcuentas
	QTextStream stream( &subcuentas );
	while( !subcuentas.atEnd() ) {
		alerta(subcuentas.at()+asientos.at(),subcuentas.size()+asientos.size());    
		QString line = stream.readLine();
		if( line.length()<2 )
		break;
		int pos = 0;
		QString cod = line.mid(pos,LEN_CODIGO_CUENTA).stripWhiteSpace();
		pos += LEN_CODIGO_CUENTA;
		QString titulo = line.mid(pos,LEN_TITULO).stripWhiteSpace();
		pos += LEN_TITULO;
		QString nif = line.mid(pos,LEN_NIF).stripWhiteSpace();
		pos += LEN_NIF;
		QString domicilio = line.mid(pos,LEN_DOMICILIO).stripWhiteSpace();
		pos += LEN_DOMICILIO;
		QString poblacion = line.mid(pos,LEN_POBLACION).stripWhiteSpace();
		pos += LEN_POBLACION;
		QString provincia = line.mid(pos,LEN_PROVINCIA).stripWhiteSpace();
		pos += LEN_PROVINCIA;
		QString codpostal = line.mid(pos,LEN_CODPOSTAL).stripWhiteSpace();
		pos += LEN_CODPOSTAL;
		QString divisa = line.mid(pos,LEN_DIVISA).stripWhiteSpace();
		pos += LEN_DIVISA;
		QString cta_coddivisa = line.mid(pos,LEN_CTA_CODDIVISA).stripWhiteSpace();
		pos += LEN_CTA_CODDIVISA;
		QString cta_documento = line.mid(pos,LEN_CTA_DOCUMENTO).stripWhiteSpace();
		pos += LEN_CTA_DOCUMENTO;
		QString ajustame = line.mid(pos,LEN_AJUSTAME).stripWhiteSpace();
		pos += LEN_AJUSTAME;
		QString tipoiva = line.mid(pos,LEN_TIPOIVA).stripWhiteSpace();
		pos += LEN_TIPOIVA;
		/// Antes de hacer una inserción comprobamos que la cuenta no exista ya en el sistema.
		QString query = "SELECT * FROM cuenta WHERE codigo = '"+cod+"'";
		conexionbase->begin();
		cursor2 *cursaux=conexionbase->cargacursor(query,"hol");
		conexionbase->commit();
		if (cursaux->eof()) {
			if( !cod.isEmpty() ) {
				QString padre = searchParent(cod);
				QString idgrupo = cod.left(1);
				query = "INSERT INTO cuenta (imputacion, activo, tipocuenta, codigo, descripcion, cifent_cuenta, padre, idgrupo, nombreent_cuenta, dirent_cuenta, telent_cuenta, coment_cuenta, bancoent_cuenta, emailent_cuenta, webent_cuenta) VALUES  (TRUE, TRUE, 1,'"+cod+"', '"+titulo+"', '"+nif+"', "+padre+", "+idgrupo+", 'importada de ContaPlus','"+domicilio + poblacion+ provincia+codpostal+"','','','','','')";
				conexionbase->begin();
				conexionbase->ejecuta(query);
				conexionbase->commit();
				mensajeria("<LI>Se ha insertado la cuenta "+cod+"</LI>\n");
			}// end if
		} else {
			mensajeria("<LI>Ya hay una cuenta con el código "+cod+"</LI>\n");
		}// end if
		delete cursaux;
	}// end while

    QTextStream stream2( &asientos );
    QString lastasiento="0";
    int napunte=0;
    while( !asientos.atEnd() ) {
        		alerta(subcuentas.at()+asientos.at(),subcuentas.size()+asientos.size());
			QString line = stream2.readLine();
			if( line.length()<2 )
	  		  break;
			int pos=0;
			QString asiento = line.mid(pos,LEN_ASIEN).stripWhiteSpace();
			pos += LEN_ASIEN;
			QString fecha = line.mid(pos,LEN_FECHA).stripWhiteSpace();
			fecha1.setYMD(fecha.mid(0,4).toInt() ,fecha.mid(4,2).toInt() ,fecha.mid(6,2).toInt());
			fecha = fecha.mid(0,4) + "-" + fecha.mid(4,2) + "-" + fecha.mid(6,2);
			pos += LEN_FECHA;
			QString subcta = line.mid(pos,LEN_SUBCTA).stripWhiteSpace();
			pos += LEN_SUBCTA;
			QString contra = line.mid(pos,LEN_CONTRA).stripWhiteSpace();
			pos += LEN_CONTRA;
			QString ptadebe = line.mid(pos,LEN_PTADEBE).stripWhiteSpace();
			pos += LEN_PTADEBE;
			QString concepto = line.mid(pos,LEN_CONCEPTO).stripWhiteSpace();
			pos += LEN_CONCEPTO;
			QString ptahaber = line.mid(pos,LEN_PTAHABER).stripWhiteSpace();
			pos += LEN_PTAHABER;
			QString factura = line.mid(pos,LEN_FACTURA).stripWhiteSpace();
			pos += LEN_FACTURA;
			QString baseimpo = line.mid(pos,LEN_BASEIMPO).stripWhiteSpace();
			pos += LEN_BASEIMPO;
			QString iva = line.mid(pos,LEN_IVA).stripWhiteSpace();
			pos += LEN_IVA;
			QString recequiv = line.mid(pos,LEN_RECEQUIV).stripWhiteSpace();
			pos += LEN_RECEQUIV;
			QString documento = line.mid(pos,LEN_DOCUMENTO).stripWhiteSpace();
			pos += LEN_DOCUMENTO;
			QString departa = line.mid(pos,LEN_DEPARTA).stripWhiteSpace();
			pos += LEN_DEPARTA;
			QString clave = line.mid(pos,LEN_CLAVE).stripWhiteSpace();
			pos += LEN_CLAVE;
			QString estado = line.mid(pos,LEN_ESTADO).stripWhiteSpace();
			pos += LEN_ESTADO;
			QString ncasado = line.mid(pos,LEN_NCASADO).stripWhiteSpace();
			pos += LEN_NCASADO;
			QString tcasado = line.mid(pos,LEN_TCASADO).stripWhiteSpace();
			pos += LEN_TCASADO;
			QString trans = line.mid(pos,LEN_TRANS).stripWhiteSpace();
			pos += LEN_TRANS;
			QString cambio = line.mid(pos,LEN_CAMBIO).stripWhiteSpace();
			pos += LEN_CAMBIO;
			QString debeme = line.mid(pos,LEN_DEBEME).stripWhiteSpace();
			pos += LEN_DEBEME;
			QString haberme = line.mid(pos,LEN_HABERME).stripWhiteSpace();
			pos += LEN_HABERME;
			QString auxiliar = line.mid(pos,LEN_AUXILIAR).stripWhiteSpace();
			pos += LEN_AUXILIAR;
			QString serie = line.mid(pos,LEN_SERIE).stripWhiteSpace();
			pos += LEN_SERIE;
			QString sucursal = line.mid(pos,LEN_SUCURSAL).stripWhiteSpace();
			pos += LEN_SUCURSAL;
			QString coddivisa = line.mid(pos,LEN_CODDIVISA).stripWhiteSpace();
			pos += LEN_CODDIVISA;
			QString impauxme = line.mid(pos,LEN_IMPAUXME).stripWhiteSpace();
			pos += LEN_IMPAUXME;
			QString monedauso = line.mid(pos,LEN_MONEDAUSO).stripWhiteSpace();
			pos += LEN_MONEDAUSO;
			QString eurodebe = line.mid(pos,LEN_EURODEBE).stripWhiteSpace();
			pos += LEN_EURODEBE;
			QString eurohaber = line.mid(pos,LEN_EUROHABER).stripWhiteSpace();
			pos += LEN_EUROHABER;
			QString baseeuro = line.mid(pos,LEN_BASEEURO).stripWhiteSpace();
			pos += LEN_BASEEURO;
			QString noconv = line.mid(pos,LEN_NOCONV).stripWhiteSpace();
			pos += LEN_NOCONV;
			QString numeroinv = line.mid(pos,LEN_NUMEROINV).stripWhiteSpace();
			pos += LEN_NUMEROINV;
	
			if( asiento != lastasiento ) {
				if (lastasiento != "0") {
					query = "SELECT cierraasiento("+idasiento+")";
					if (!modoTest()) {
						conexionbase->begin();
						cursor2 * cur = conexionbase->cargacursor(query,"hola"); 
						conexionbase->commit();
						delete cur;
					}// end if
				}// end if
				if (fecha1 >= fechain && fecha1 <= fechafi) {
					query="INSERT INTO asiento (fecha, comentariosasiento, clase) VALUES ('"+fecha+"','Importado de Contaplus', 1 )";
					if (!modoTest()) {
						conexionbase->begin();
						conexionbase->ejecuta(query);
						conexionbase->commit();
					}// end if
					query = "SELECT max(idasiento) as idasiento FROM asiento";
					conexionbase->begin();
					cursor2 *cur=conexionbase->cargacursor(query,"lolailo");
					idasiento = cur->valor("idasiento");
					conexionbase->commit();
					delete cur;
					napunte = 0;
					lastasiento = asiento;
					mensajeria("<LI>Inserción de Asiento" + idasiento+"</LI>\n");
				}// end if
			}// end if
			napunte++;
			if( monedauso == "1" ) { // Ptas
			/// Aqui está el peor error cometido, usar punto flotante
				debe = ptahaber +"/"+S_EURO;
				haber = ptadebe+"/"+S_EURO;
			} else {
				debe = eurodebe;
				haber = eurohaber;
			}// end if	
			query = "SELECT * FROM cuenta WHERE codigo='"+subcta+"'";
			conexionbase->begin();
			cursor2 *cur=conexionbase->cargacursor(query,"elquery");
			conexionbase->commit();
			if (!cur->eof()) {
				if (fecha1 >= fechain && fecha1 <= fechafi) {
					if (!modoTest() ) {
						query="INSERT INTO borrador (idasiento,idcuenta,fecha, conceptocontable, debe, haber) VALUES ("+idasiento+",id_cuenta('"+subcta+"'), '"+fecha+"','"+concepto+"',"+debe+","+haber+" )";
						conexionbase->begin();
						conexionbase->ejecuta(query);
						conexionbase->commit();
					}// end if
					mensajeria("<LI>Inserción de Apunte"+subcta+","+concepto+"</LI>\n");
				} else {
					mensajeria("<LI>Apunte fuera de fecha</LI>\n");
				}// end if
			}// end if
	}// end while
	if (lastasiento != "0") {
		if(!modoTest()) {
			query = "SELECT cierraasiento("+idasiento+")";
			conexionbase->begin();
			cursor2 * cur = conexionbase->cargacursor(query,"hola"); 
			conexionbase->commit();
			delete cur;
		}// end if
	}// end if
	mensajeria("<LI>Terminado</LI>\n");
    alerta(subcuentas.size()+asientos.size(),subcuentas.size()+asientos.size());    
   return 1;
}

/**
  * This function search in the database the account parent of the account selected
  * if there are not parent returns NULL
  */
QString pgimportfiles::searchParent(QString cod) {
//	int lencod = cod.length();
	QString padre="NULL"; //almacena el padre de la cuenta.
	QString query;
	int i = 2;
	int fin=0;
	while (!fin) {
		query = "SELECT * FROM cuenta WHERE codigo = '"+cod.left(i)+"'";
		conexionbase->begin();
		cursor2 *cur = conexionbase->cargacursor(query,"unquery");
		conexionbase->commit();
		if (!cur->eof()) {
			padre = cur->valor("idcuenta");
		} else {
			fin=1;
		}// end if
		delete cur;
		i++;
	}// end while
	return padre;
}// end searchParent




/** \brief Esta función se encarga de pasar los datos de BulmaGés a XML
  * Los datos pasados de esta forma son mucho más sencillos de pasar.
  */
int pgimportfiles::bulmages2XML(QFile &xmlfile) {
	QString codigo, descripcion;
	QString strblancomax;
	QTextStream stream( &xmlfile );
	stream << "<?xml version=\"1.0\" encoding = \"iso-8859-1\"?>\n"
	"<!DOCTYPE FUGIT>\n"
	"<FUGIT version='0.3.1' origen='BulmaGés'"
        " date='" << QDate().toString(Qt::ISODate) << "'>\n";
	    
	/// Sólo se van a exportar las cuentas utilizadas
	QString query = "SELECT * FROM cuenta LEFT JOIN (SELECT codigo AS codpadre, idcuenta as idpadre FROM cuenta ) AS t1 ON cuenta.padre = t1.idpadre WHERE idcuenta IN (SELECT DISTINCT idcuenta FROM apunte)";
	conexionbase->begin();
	cursor2 *curcta = conexionbase->cargacursor(query,"elquery");
	conexionbase->commit();
	while (!curcta->eof()) {
		QString linea="";
		stream << "<CUENTA>\n";
		stream << "\t<IDCUENTA>"       << curcta->valor("idcuenta")      << "</IDCUENTA>\n";
		stream << "\t<CODIGO>"         << curcta->valor("codigo")        << "</CODIGO>\n";
		stream << "\t<DESCRIPCION>"    << curcta->valor("descripcion")   << "</DESCRIPCION>\n";
		stream << "\t<CIFENT_CUENTA>"  << curcta->valor("cifent_cuenta") << "</CIFENT_CUENTA>\n";
		stream << "\t<DIRENT_CUENTA>"  << curcta->valor("dirent_cuenta") << "</DIRENT_CUENTA>\n";
		stream << "\t<CODPADRE>"       << curcta->valor("codpadre")      << "</CODPADRE>\n";
		stream << "</CUENTA>\n";
		curcta->siguienteregistro();
	}// end while
	delete curcta;
	
	/// Hacemos la exportación de asientos
	query = "SELECT * FROM asiento WHERE 1=1 ";
	if (m_fInicial != "") 
		query += " AND asiento.fecha >= '"+m_fInicial+"'";
	if (m_fFinal != "") 
		query += " AND asiento.fecha <= '"+m_fFinal+"'";
	query +=" ORDER BY asiento.fecha, asiento.idasiento ";	
	conexionbase->begin();
	cursor2 *curas = conexionbase->cargacursor(query, "masquery");
	conexionbase->commit();
	int i =0;
	int numreg = curas->numregistros()+1;	
	while (!curas->eof()) {
		alerta(i++,numreg);    
		stream << "<ASIENTO>\n";
		stream << "\t<ORDENASIENTO>" << curas->valor("ordenasiento") << "</ORDENASIENTO>\n";
		QString fechas = curas->valor("fecha");
		fechas = fechas.mid(6,4)+fechas.mid(3,2)+fechas.mid(0,2);
		stream << "\t<FECHA>" << fechas << "</FECHA>\n";
		query = "SELECT * FROM apunte LEFT JOIN cuenta ON apunte.idcuenta=cuenta.idcuenta ";
		query += "LEFT JOIN (SELECT nombre AS nomcanal, idcanal FROM canal) AS canal1 ON apunte.idcanal = canal1.idcanal "; query += "LEFT JOIN (SELECT nombre AS nc_coste, idc_coste FROM c_coste) AS c_coste1 ON c_coste1.idc_coste = apunte.idc_coste ";
		query += "LEFT JOIN (SELECT codigo AS codcontrapartida,  idcuenta FROM cuenta) AS contra ON contra.idcuenta=apunte.contrapartida ";
		query +=" WHERE "+curas->valor("idasiento")+"= apunte.idasiento ";
		conexionbase->begin();
		cursor2 *curap = conexionbase->cargacursor(query, "masquery1");
		conexionbase->commit();
		while (!curap->eof()) {   
			stream << "\t<APUNTE>\n";
			QString fecha = curap->valor("fecha");
			fecha = fecha.mid(6,4)+fecha.mid(3,2)+fecha.mid(0,2);
			stream << "\t\t<FECHA>"               << fecha << "</FECHA>\n";
			stream << "\t\t<CODIGO>"              << curap->valor("codigo")             << "</CODIGO>\n";
			stream << "\t\t<DEBE>"                << curap->valor("debe")               << "</DEBE>\n";
			stream << "\t\t<HABER>"               << curap->valor("haber")              << "</HABER>\n";
			stream << "\t\t<CONCEPTOCONTABLE>"    << curap->valor("conceptocontable")   << "</CONCEPTOCONTABLE>\n";
			stream << "\t\t<IDCANAL>"      << curap->valor("idcanal")     << "</IDCANAL>\n";
			stream << "\t\t<CANAL>"        << curap->valor("nomcanal")    << "</CANAL>\n";
			stream << "\t\t<IDC_COSTE>"    << curap->valor("idc_coste")   << "</IDC_COSTE>\n";	
			stream << "\t\t<C_COSTE>"      << curap->valor("nc_coste")    << "</C_COSTE>\n";
			stream << "\t\t<PUNTEO>"       << curap->valor("punteo")      << "</PUNTEO>\n";
			stream << "\t\t<ORDEN>"        << curap->valor("orden")       << "</ORDEN>\n";
			stream << "\t\t<CONTRAPARTIDA>"<< curap->valor("codcontrapartida")<< "</CONTRAPARTIDA>\n";
			
			
			/// Hacemos la exportación de registros de IVA
			query  = "SELECT * FROM registroiva";
			query += " LEFT JOIN (SELECT codigo, idcuenta FROM cuenta) AS t1 ON registroiva.contrapartida = t1.idcuenta ";
			query += " WHERE idborrador IN (SELECT idborrador FROM borrador WHERE idasiento="+curas->valor("idasiento")+" AND orden = "+curap->valor("orden")+")";
			conexionbase->begin();
			cursor2 *curreg = conexionbase->cargacursor(query, "queryregiva");
			conexionbase->commit();
			while (!curreg->eof()) {
				stream << "\t\t<REGISTROIVA>\n";
				stream << "\t\t\t<CONTRAPARTIDA>"  << curreg->valor("contrapartida")  << "</CONTRAPARTIDA>\n";
				stream << "\t\t\t<BASEIMP>"        << curreg->valor("baseimp")        << "</BASEIMP>\n";
				stream << "\t\t\t<IVA>"            << curreg->valor("iva")            << "</IVA>\n";
				stream << "\t\t\t<FFACTURA>"       << curreg->valor("ffactura")       << "</FFACTURA>\n";
				stream << "\t\t\t<FACTURA>"        << curreg->valor("factura")        << "</FACTURA>\n";
				stream << "\t\t\t<NUMORDEN>"       << curreg->valor("numorden")       << "</NUMORDEN>\n";
				stream << "\t\t\t<CIF>"            << curreg->valor("cif")            << "</CIF>\n";
				stream << "\t\t\t<IDFPAGO>"        << curreg->valor("idfpago")        << "</idfpago>\n";
				stream << "\t\t\t<RECTIFICAAREGISTROIVA>"        << curreg->valor("rectificaaregistroiva")        << "</RECTIFICAAREGISTROIVA>\n";
				
				
				/// Hacemos la exportación deIVAs
				query  = "SELECT * FROM iva";
				query += " WHERE idregistroiva = "+curreg->valor("idregistroiva");
				conexionbase->begin();
				cursor2 *curiva = conexionbase->cargacursor(query, "queryiva");
				conexionbase->commit();
				while (!curiva->eof()) {
					stream << "\t\t\t<RIVA>\n";
					stream << "\t\t\t\t<IDTIPOIVA>"        << curiva->valor("idtipoiva")        << "</IDTIPOIVA>\n";
					stream << "\t\t\t\t<BASEIVA>"          << curiva->valor("baseiva")        << "</baseiva>\n";
					stream << "\t\t\t</RIVA>\n";
					curiva->siguienteregistro();
				}// end while
				delete curiva;	
					
				
				stream << "\t\t</REGISTROIVA>\n";
				curreg->siguienteregistro();
			}// end while
			delete curreg;	
			
			
			
			
			mensajeria("Exportando :"   + curap->valor("codigo")   + "--" +fecha+"\n");
			curap->siguienteregistro();
			stream << "\t</APUNTE>\n";
		}// end while
		delete curap;
		stream << "</ASIENTO>\n";
		curas->siguienteregistro();
	}// end while
	delete curas;
	
	

	
	
	stream << "</FUGIT>\n";
	alerta (numreg,numreg);
	return 0;
}// end if



int pgimportfiles::XML2Bulmages (QFile &fichero) {
        StructureParser handler(conexionbase, alerta);
        QXmlInputSource source( &fichero );
        QXmlSimpleReader reader;
        reader.setContentHandler( &handler );
        reader.parse( source );
	alerta(100,100);
	return 0;
}// end XML2Bulmages


StructureParser::StructureParser(postgresiface2 *con,void (*func)(int,int) ) {
	alerta = func;
	conexionbase = con;
}// end StructureParser

bool StructureParser::startDocument() {
    indent = "";
    return TRUE;
}// end startDocument

bool StructureParser::startElement( const QString&, const QString&, const QString& qName, const QXmlAttributes& ) {
    fprintf( stderr, "%s<%s>", (const char*)indent, (const char*)qName);
    indent += "..";
    if (qName == "ASIENTO") {
    	tagpadre = "ASIENTO";
	QString query = "INSERT INTO ASIENTO (descripcion, fecha) VALUES ('un nuevo asiento', '01/01/2005')";
	conexionbase->begin();
	conexionbase->ejecuta(query);
	cursor2 *cur= conexionbase->cargacursor("SELECT MAX(idasiento) AS max FROM asiento","otroquery");
	conexionbase->commit();
	if (!cur->eof() ) {
		idasiento = cur->valor("max");
		fprintf(stderr,"INSERCION DE ASIENTO:%s",idasiento.ascii());
	}// end if
	delete cur;
    }// end if
    if (qName == "APUNTE") {
    	tagpadre = "APUNTE";
    }// end if
    if (qName == "CUENTA") {
    	tagpadre = "CUENTA";
    }// end if
    cadintermedia = "";    
    return TRUE;
}// end startElement

bool StructureParser::endElement( const QString&, const QString&, const QString& qName) {
    indent.remove( (uint)0, 2 );
    fprintf( stderr,"<\\%s>\n", (const char*)qName);
/// VAmos a ir distinguiendo casos y actuando segun cada caso. En la mayoría de casos iremos actuando en consecuencia.    
    if (qName == "ASIENTO") {
    	fprintf(stderr,"Fin de Asiento");
	QString query = "UPDATE asiento set fecha='"+fechaasiento+"' WHERE idasiento="+idasiento;
	conexionbase->begin();
	conexionbase->ejecuta(query);
	cursor2 *cur = conexionbase->cargacursor("SELECT cierraasiento("+idasiento+")", "elauery");
	conexionbase->commit();
	delete cur;
    }// end if
/// Si es una punte hacemos su inserción.
    if (qName == "APUNTE") {
    	QString query = "INSERT INTO borrador (idasiento, debe, haber, idcuenta, fecha, conceptocontable) VALUES ("+idasiento+","+debeapunte+","+haberapunte+",id_cuenta('"+codigocuentaapunte+"'), '"+fechaapunte+"', '"+conceptocontableapunte+"')";
	conexionbase->begin();
	conexionbase->ejecuta(query);
	conexionbase->commit();
    }// end if
    if (qName == "FECHA" && tagpadre == "ASIENTO") {
    	fechaasiento = cadintermedia;
    }// end if
    if (qName == "FECHA" && tagpadre == "APUNTE") {
    	fechaapunte = cadintermedia;
    }// end if
    if (qName == "DEBE" && tagpadre == "APUNTE") {
    	debeapunte = cadintermedia;
    }// end if
    if (qName == "HABER" && tagpadre == "APUNTE") {
    	haberapunte = cadintermedia;
    }// end if
    if (qName == "CODIGO" && tagpadre == "APUNTE") {
    	codigocuentaapunte = cadintermedia;
    }// end if
    if (qName == "CONCEPTOCONTABLE" && tagpadre == "APUNTE") {
    	conceptocontableapunte = cadintermedia;
    }// end if
 
/// Si es una cuenta la tratamos.
    if (qName == "CUENTA" ) {
    	QString idgrupo = codigocuenta.left(1);
	fprintf(stderr,"codigocuenta [%s], primer caracter [%s] \n", codigocuenta.ascii(), idgrupo.ascii());
    	QString query = "INSERT INTO cuenta (codigo, descripcion, padre, idgrupo) VALUES ('"+codigocuenta+"','"+descripcioncuenta+"', id_cuenta('"+codigopadre+"'), "+idgrupo+")";
	conexionbase->begin();
	conexionbase->ejecuta(query);
	conexionbase->commit();
    }// end if
    if (qName == "CODIGO" && tagpadre == "CUENTA") {
    	codigocuenta = cadintermedia;
    }// end if
    if (qName == "DESCRIPCION" && tagpadre == "CUENTA") {
    	descripcioncuenta = cadintermedia;
    }// end if
    if (qName == "CODPADRE" && tagpadre == "CUENTA") {
    	codigopadre = cadintermedia;
    }// end if
    
    cadintermedia = "";
    return TRUE;
}// end endElement


bool StructureParser::characters( const QString& n1) {
    fprintf( stderr,"[%s]", (const char*)n1);
    cadintermedia += n1;
    return TRUE;
}// end endElement

