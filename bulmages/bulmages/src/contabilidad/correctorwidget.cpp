//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "correctorwidget.h"


#include <qtable.h>
#include <qheader.h>
#include <qtextbrowser.h>


correctorwidget::correctorwidget( QWidget* parent, const char* name, WFlags fl ):correctorwdt(parent, name, fl) {
}// end correctorwidget;


correctorwidget::~correctorwidget() {
}

void correctorwidget::link(const QString &linker) {
	fprintf(stderr,"Link Pulsado: %s\n", linker.latin1());
}// end link


// Esta función se encarga de procesar las pulsaciones sobre el QTextBrowser, para
// Saber de que va la cosa.
void correctorwidget::alink(const QString &linker, const QString &l) {
	fprintf(stderr,"Anchor pulsado: %s, %s\n", linker.latin1(), l.latin1());
	if (linker == "ver") {
		empresaactual->muestracuentas();
	} else if (linker== "asiento") {
		QString ordenasiento = l.right(l.length()-2);
		intapunts3view *view = empresaactual->intapuntsempresa();
		bool ok;
		view->muestraasiento(ordenasiento.toInt(&ok));
	} else {
		QMessageBox::warning(0,"hola","hola",0,1,2);
	}// endif
}// end link


void correctorwidget::agregarError(QString texto, QString texto1, QString texto2) {
						textBrowser += "<HR><table><tr><td colspan=2>"+texto+"</td></tr><tr><td><a name='masinfo' href='#?"+texto1+"'>+ info</a></td><td><a name='"+texto1+"' href='#?"+texto2+"'>ver error</a></td></tr></table>";
}// end agregarError


// Esta función es el disparador del corrector, que se encarga de hacer los tests a la empresa
// Y sacar la ventana de resultados.
void correctorwidget::corregir() {
	textBrowser = "<HTML><BODY BGCOLOR='#FFFFFF'>";
	cursor2 *cur;
	/*
	conexionbase->begin();
	cursor1 *cur = conexionbase->cargacursor("SELECT * FROM cuenta where codigo LIKE '10%'","cursproves");
	conexionbase->commit();
	while (!cur->eof()) {
		QString cadena;
		cadena.sprintf("<font face='Arial' size=2><img src='/usr/share/bulmages/icons/messagebox_critical.png'>&nbsp;&nbsp;<B><I>Critical Error:</I></B></FONT><font face='Arial' size=2><BR>La cuenta <B>%s</B> no está cerrado, esto causa que el asiento no modifica las cuentas.</FONT>", cur->valor("codigo").c_str());
	
		agregarError(cadena,"algo extraño.",cur->valor("codigo").c_str());
		cur->siguienteregistro();
	}// end while
	delete (cur);
	
		conexionbase->begin();
	cur = conexionbase->cargacursor("SELECT * FROM asiento","cursasiento");
	conexionbase->commit();
	while (!cur->eof()) {
		QString cadena;
		cadena.sprintf("<font face='Arial' size=2><img src='/usr/share/bulmages/icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B></FONT><font face='Arial' size=1><BR>El asiento num. <B>%s</B> no está cerrado, esto causa que el asiento no modifica las cuentas.</FONT>", cur->valor("ordenasiento").c_str());
		agregarError(cadena, cur->valor("idasiento").c_str(),cur->valor("ordenasiento").c_str());
		cur->siguienteregistro();
	}// end while
	delete (cur);
*/
	
	// Calculo de asientos abiertos
	QString query;
	query.sprintf("SELECT * FROM asiento LEFT JOIN (SELECT count(idborrador) AS numborr, idasiento FROM borrador GROUP BY idasiento) AS borr ON borr.idasiento=asiento.idasiento LEFT JOIN (SELECT count(idapunte) AS numap, idasiento  FROM apunte GROUP BY idasiento) AS apunt ON apunt.idasiento=asiento.idasiento WHERE apunt.numap <> borr.numborr OR numap ISNULL");
	fprintf(stderr,"%s\n",query.latin1());
	conexionbase->begin();
	cur = conexionbase->cargacursor(query,"hola");
	conexionbase->commit();
	while (!cur->eof()) {
		QString cadena;
		cadena.sprintf("<font face='Arial' size=2><img src='/usr/share/bulmages/icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B></FONT><font face='Arial' size=1><BR>El asiento num. <B>%s</B> está abierto, esto causa que el asiento no modifique el estado de las cuentas.</FONT>", cur->valor("ordenasiento").latin1());
		agregarError(cadena, "asiento",cur->valor("idasiento").latin1());
		cur->siguienteregistro();
	}// end while
	delete(cur);
	
	// Calculo de inserción en cuentas intermedias (con hijos)
	//--------------------------------------------------------
	query.sprintf("SELECT * FROM asiento, apunte, cuenta WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.idcuenta IN (SELECT padre FROM cuenta) AND apunte.idasiento=asiento.idasiento");
		fprintf(stderr,"%s\n",query.latin1());
	conexionbase->begin();
	cur = conexionbase->cargacursor(query,"hola");
	conexionbase->commit();
	while (!cur->eof()) {
		QString cadena;
		cadena.sprintf("<font face='Arial' size=2><img src='/usr/share/bulmages/icons/messagebox_critical.png'>&nbsp;&nbsp;<B><I>Critial Error:</I></B></FONT><font face='Arial' size=1><BR>El asiento num. <B>%s</B> tiene un apunte con la cuenta <B>%s</B> no hija..</FONT>", cur->valor("ordenasiento").latin1(), cur->valor("codigo").latin1());
		agregarError(cadena, "asiento",cur->valor("idasiento").latin1());
		cur->siguienteregistro();
	}// end while
	delete(cur);
	
	// Calculo de la ecuación fundamental contable A+I= P+N+G
	//--------------------------------------------------------	
query = " SELECT asiento.idasiento AS idasiento, asiento.ordenasiento AS ordenasiento, ingresos,activos, gastos, netos, pasivos FROM asiento ";
query += " LEFT JOIN (SELECT idasiento, sum(apunte.debe)-sum(apunte.haber) AS ingresos  FROM cuenta,apunte WHERE  apunte.idcuenta=cuenta.idcuenta AND cuenta.tipocuenta=4 GROUP BY idasiento) AS ing ON asiento.idasiento = ing.idasiento ";
query +=" LEFT JOIN (SELECT idasiento, sum(apunte.debe)-sum(apunte.haber) AS gastos  FROM cuenta,apunte WHERE  apunte.idcuenta=cuenta.idcuenta AND cuenta.tipocuenta=5 GROUP BY idasiento) AS gas ON asiento.idasiento= gas.idasiento ";
query += " LEFT JOIN (SELECT idasiento, sum(apunte.debe)-sum(apunte.haber) AS activos  FROM cuenta,apunte WHERE  apunte.idcuenta=cuenta.idcuenta AND cuenta.tipocuenta=1 GROUP BY idasiento) AS act ON act.idasiento=asiento.idasiento ";
query += " LEFT JOIN (SELECT idasiento, sum(apunte.debe)-sum(apunte.haber) AS pasivos  FROM cuenta,apunte WHERE  apunte.idcuenta=cuenta.idcuenta AND cuenta.tipocuenta=2 GROUP BY idasiento) AS pas ON pas.idasiento=asiento.idasiento ";
query += " LEFT JOIN (SELECT idasiento, sum(apunte.debe)-sum(apunte.haber) AS netos  FROM cuenta,apunte WHERE  apunte.idcuenta=cuenta.idcuenta AND cuenta.tipocuenta=3 GROUP BY idasiento) AS net ON net.idasiento=asiento.idasiento ";
query += " ORDER BY ordenasiento";
	conexionbase->begin();
	cur = conexionbase->cargacursor(query,"hol");
	conexionbase->commit();
	while (!cur->eof()) {
		float ing,gas,act,pas,net;
		ing = cur->valor("ingresos").toFloat();
		gas = cur->valor("gastos").toFloat();
		act = cur->valor("activos").toFloat();
		pas = cur->valor("pasivos").toFloat();
		net = cur->valor("netos").toFloat();
	if (-act-gas-pas-net+ing>0.01) {
			QString cadena;
			cadena.sprintf("<font face='Arial' size=2><img src='/usr/share/bulmages/icons/messagebox_critical.png'>&nbsp;&nbsp;<B><I>Critial Error:</I></B></FONT><font face='Arial' size=1><BR>El asiento num. <B>%s</B> no cumple la ecuación fundamental.%2.2f+%2.2f=%2.2f+%2.2f+%2.2f</FONT>", cur->valor("ordenasiento").latin1(), act,gas,pas,net,ing);
			agregarError(cadena, "asiento",cur->valor("idasiento").latin1());
	}// end if
		cur->siguienteregistro();
	}// end while
	delete(cur);

		// Calculo de cuentas con insercion en el debe que lo tienen bloqueado
	//-----------------------------------------------------------------------
	query.sprintf("SELECT * FROM asiento, apunte, cuenta WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.nodebe AND apunte.idasiento=asiento.idasiento AND apunte.debe <> 0");
		fprintf(stderr,"%s\n",query.latin1());
	conexionbase->begin();
	cur = conexionbase->cargacursor(query,"hola1");
	conexionbase->commit();
	while (!cur->eof()) {
		QString cadena;
		cadena.sprintf("<font face='Arial' size=2><img src='/usr/share/bulmages/icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B></FONT><font face='Arial' size=1><BR>El asiento num. <B>%s</B> tiene una inserción en el debe de la cuenta <B>%s</B> que no permite inserciones en el debe de dicha cuenta.</FONT>", cur->valor("ordenasiento").latin1(), cur->valor("codigo").latin1());
		agregarError(cadena, "asiento",cur->valor("idasiento").latin1());
		cur->siguienteregistro();
	}// end while
	delete(cur);
	

			// Calculo de cuentas con insercion en el haber que lo tienen bloqueado
	//-----------------------------------------------------------------------
	query.sprintf("SELECT * FROM asiento, apunte, cuenta WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.nohaber AND apunte.idasiento=asiento.idasiento AND apunte.haber <> 0");
	conexionbase->begin();
	cur = conexionbase->cargacursor(query,"hola1");
	conexionbase->commit();
	while (!cur->eof()) {
		QString cadena;
		cadena.sprintf("<font face='Arial' size=2><img src='/usr/share/bulmages/icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B></FONT><font face='Arial' size=1><BR>El asiento num. <B>%s</B> tiene una inserción en el haber de la cuenta <B>%s</B> que no permite inserciones en el haber de dicha cuenta.</FONT>", cur->valor("ordenasiento").latin1(), cur->valor("codigo").latin1());
		agregarError(cadena, "asiento",cur->valor("idasiento").latin1());
		cur->siguienteregistro();
	}// end while
	delete(cur);

	
	
	textBrowser += "</BODY></HTML>";
	browser->setText(textBrowser);
}// end corregir

