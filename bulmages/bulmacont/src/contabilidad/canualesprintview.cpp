/***************************************************************************
                          CAnualesPrintView.cpp  -  description
                             -------------------
    begin                : jue oct 9 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
    email                : tborras@conetxia.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "canualesprintview.h"
#include <qlineedit.h>
#include <q3datetimeedit.h>
#include <q3progressbar.h>
#include "funcaux.h"
#include "calendario.h"
#include "empresa.h"
#include "fixed.h"

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

CAnualesPrintView::CAnualesPrintView(empresa *emp,QWidget *parent ) : QDialog(parent) {
    _depura("CAnualesPrintView::CAnualesPrintView", 0);
    setupUi(this);
    empresaactual = emp;
    fichero = NULL;
    _depura("END CAnualesPrintView::CAnualesPrintView", 0);

}

CAnualesPrintView::~CAnualesPrintView() {}



/*********************************************************************************
 * Se ha pulsado sobre el boton de aceptar del formulario con lo que podemos     *
 * pasar a hacer la impresión.                                                   *
 *********************************************************************************/
/*
void CAnualesPrintView::accept1() {
    QString arch = confpr->valor(CONF_DIR_USER)+"balance.txt";

    string cad;
    QString fechainicial = fechain->text();
    QString fechafinal = fechafin->text();
	/// Si no se especifica fecha inicial se pone una suficientemente antigua.
    if (fechainicial == "") fechainicial = "01/01/1900";
	/// Si no se especifica fecha final se pone una suficientemente futura.
    if (fechafinal == "") fechafinal = "31/12/2100";

    FILE *mifile;
    mifile = fopen(arch.ascii(),"wt");
    if (mifile == NULL) {
	_depura("No se ha podido abrir el fichero de destino\n",2);
	return;
    }// end if

    if (mifile != NULL) {
	/// Cargamos los componentes e itereamos para cada uno de ellos.
        QString query1 = "SELECT * FROM compbalance WHERE idbalance = "+idbalance+" ORDER BY orden";
        cursor2 *cursor = empresaactual->cargacursor(query1);
        int numregistros = cursor->numregistros();
        progreso->reset();
        progreso->setTotalSteps(numregistros);
        progreso->setProgress(1);
        while (!cursor->eof()) {
            QString query = "SELECT saldompatrimonial("+cursor->valor("idmpatrimonial")+",'"+fechainicial+"','"+fechafinal+"') AS saldot";
            cursor2 *mycursor = empresaactual->cargacursor(query);
            int i=0;
            Fixed valor("0");
            while (!mycursor->eof()) {
                int orden = cursor->valor("tabulacion").toInt();
                QString texto = "";
                for (int j=0; j<orden; j++)
                    texto += "   ";
                texto += cursor->valor("concepto");
                valor = Fixed(mycursor->valor("saldot"));
		if (valor != Fixed("0") || orden <=1) 
			if (valor != Fixed("0")) {
                		fprintf(mifile, "%-60.60s %10.10s\n", texto.ascii(), valor.toQString().ascii());
			} else {
                		fprintf(mifile, "%-60.60s\n", texto.ascii());
			}// end if
                i++;
                mycursor->siguienteregistro();
            }// end while
            delete mycursor;
            // Actualizamos la barra de progreso
            progreso->setProgress(progreso->progress()+1);
            cursor->siguienteregistro();
        }// end while
        delete cursor;
        fclose(mifile);
    }// end if

    QString cadena = confpr->valor(CONF_EDITOR)+" "+confpr->valor(CONF_DIR_USER)+"balance.txt &";
    system(cadena.ascii());
}
*/


/**
PSEUDICODIGO
	Ponemos todos los valores de las cuentas.
	Mientras haya sustituciones
		Para cada una de las formulas
			Si la formula no esta completa
				Sustituye todos los valores que se puedan sustituir
				Intenta Completar la formula
			FSi
		FPara
	FMientras

*/
void CAnualesPrintView::accept() {
	_depura("CAnualesPrintView::accept", 2);
	QFile f("/home/tborras/bulmages/trunk/bulmages/installbulmages/canuales/balance.xml");
	if ( !f.open( IO_ReadOnly ) )
		return;
	if ( !m_doc.setContent( &f ) ) {
		f.close();
		return;
	}
	f.close();

	/// Ponemos todos los valores de las cuentas
	QDomNodeList lcuentas = m_doc.elementsByTagName("CUENTA");
	for (int i = 0; i < lcuentas.count(); i++) {
		QDomNode cuenta = lcuentas.item(i);
		QDomElement e1 = cuenta.toElement(); // try to convert the node to an element.
		if( !e1.isNull() ) { // the node was really an element.
			QString query = "SELECT saldot"; /// Este es el calculo de los saldos para la cuenta
			QDomNode c = cuenta.parentNode();
			QString valoract = QString("99.99");
			QString valorant = QString("99.99");
			agregaValores(c, valoract, valorant);
		} // end if
	} // end for

	bool terminado = FALSE;
	while (!terminado) {
		terminado = TRUE;
		/// Recogemos los valores de cuenta
		QDomNodeList litems = m_doc.elementsByTagName("FORMULA");
		for (int i = 0; i < litems.count(); i++) {
			QDomNode item = litems.item(i);
			QDomElement e1 = item.toElement(); // try to convert the node to an element.
			if( !e1.isNull() ) { // the node was really an element.
				terminado &= procesaFormula(item);
			} // end if
		} // end for
	} // end while


	QDomNodeList litems = m_doc.elementsByTagName("LBALANCE");
	for (int i = 0; i < litems.count(); i++) {
		QDomNode item = litems.item(i);
		QDomElement e1 = item.toElement(); // try to convert the node to an element.
		if( !e1.isNull() ) { // the node was really an element.
			QDomNode formul = item.firstChildElement("FORMULA");
			QString vact = formul.firstChildElement("VALORACT").toElement().text();
			QString vant = formul.firstChildElement("VALORACT").toElement().text();
			QString texto = item.firstChildElement("CONCEPTO").toElement().text();
			_depura(texto+" "+vact, 2);
		} // end if
	} // end for



	_depura("END CAnualesPrintView::accept", 2);
}


bool CAnualesPrintView::procesaFormula(const QDomNode &formula) {
	_depura ("CAnualesPrintView::procesaFormula", 0);
	QDomElement valor = formula.firstChildElement("VALORACT");
	if (!valor.isNull()) return TRUE;

	Fixed tvaloract = Fixed("0.0");
	Fixed tvalorant = Fixed("0.0");

	QDomElement formula3 = formula.toElement();
	QDomNodeList litems = formula3.elementsByTagName("OPERADOR");
	for (int i = 0; i < litems.count(); i++) {
		QDomNode item = litems.item(i);
		QDomElement e1 = item.toElement(); // try to convert the node to an element.
		if( !e1.isNull() ) { // the node was really an element.
			if (!procesaOperador(item) )
				return FALSE;
			QString valoract, valorant;
			if (valorItem(item, valoract, valorant)) {
				tvaloract = tvaloract + Fixed(valoract);
				tvalorant = tvalorant + Fixed(valorant);
			} else return FALSE;
		} // end if
	} // end for
	QString tvaloracts = tvaloract.toQString();
	QString tvalorants = tvalorant.toQString();
	agregaValores(formula, tvaloracts, tvalorants);
	_depura ("END CAnualesPrintView::procesaFormula", 0);
	return TRUE;
}

/** Pseudocodigo 
	Si el operador ya tiene valor (devuelve TRUE)
	Si la formula ya tiene valor le ponemos el valor y devuelve true
	devuelve FALSe
*/
bool CAnualesPrintView::procesaOperador(const QDomNode &operador) {
	_depura("CAnualesPrintView::procesaOperador", 0, operador.toElement().text());
	QDomElement valor = operador.firstChildElement("VALORACT");
	if (!valor.isNull()) return TRUE;

	/// Miramos la formula
	QDomElement lineaid = operador.firstChildElement("LINEAID");

	if (!lineaid.isNull()) {
		QDomNodeList litems = m_doc.elementsByTagName("ID");
		for (int i = 0; i < litems.count(); i++) {
			QDomNode item = litems.item(i);
			QDomElement e1 = item.toElement(); // try to convert the node to an element.
			if( !e1.isNull() ) { // the node was really an element.
				if (e1.text() == lineaid.text()) {
					/// Este item es la formula referenciada
					QDomNode formula = item.parentNode().firstChildElement("FORMULA");
					QString valoract, valorant;
					if (valorItem(formula, valoract, valorant)) {
						agregaValores(operador, valoract, valorant);
						return TRUE;
					} else {
						return FALSE;
					} // end if
				} // end if
			} // end if
		} // end for
	} // end if
	_depura("CAnualesPrintView::procesaOperador", 0);
	return FALSE;
}


bool CAnualesPrintView::valorItem(const QDomNode &formula, QString &valoract, QString &valorant) {
	_depura("CAnualesPrintView::valorItem", 0, formula.toElement().tagName());
	QDomElement valor = formula.namedItem("VALORACT").toElement();
	if (valor.isNull())  {
		return FALSE;
	}
	valoract = valor.text();
	valorant = formula.namedItem("VALORANT").toElement().text();
	_depura("END CAnualesPrintView::valorItem", 0, formula.toElement().text() + "--"+ valoract);
	return TRUE;
} 


void CAnualesPrintView::agregaValores(const QDomNode &nodo, const QString &valoract, const QString &valorant) {
	_depura("CAnualesPrintView::agregaValores", 0, nodo.toElement().tagName()+" "+valoract);
	QDomElement enodo = nodo.toElement();
	enodo.setAttribute("valoract", "200");

	QDomElement valoract1 = m_doc.createElement("VALORACT");
	valoract1.setTagName("VALORACT");
	QDomText etx = m_doc.createTextNode(valoract);
	valoract1.appendChild(etx);

	QDomElement valorant1 = m_doc.createElement("VALORANT");
	valorant1.setTagName("VALORANT");
	QDomText etc = m_doc.createTextNode(valorant);
	valorant1.appendChild(etc);
	
	QDomNode n = nodo;
	enodo.appendChild(valoract1);
	enodo.appendChild(valorant1);

	_depura("END CAnualesPrintView::agregaValores", 0);
}


void CAnualesPrintView::setidbalance(QString id) {
    idbalance = id;
    QString query ="SELECT * FROM balance WHERE idbalance="+idbalance;
    cursor2 *cur = empresaactual->cargacursor(query, "micurs");
    if (!cur->eof()) {
        m_nomBalance->setText(cur->valor("nombrebalance"));
    }// end if
    delete cur;
}



