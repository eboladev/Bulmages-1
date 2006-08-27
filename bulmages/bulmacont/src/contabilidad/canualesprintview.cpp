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
    _depura("CAnualesPrintView::accept", 0);

	QString finicial = mui_fechainicial->text();
	QString ffinal = mui_fechafinal->text();
	QString finicial1 = mui_fechainicial1->text();;
	QString ffinal1 = mui_fechafinal1->text();

    if (finicial == "" || ffinal == "" || finicial1 == "" || ffinal1 == "")
	return;



    /// Ponemos todos los valores de las cuentas
    QDomNodeList lcuentas = m_doc.elementsByTagName("CUENTA");
    for (int i = 0; i < lcuentas.count(); i++) {
        QDomNode cuenta = lcuentas.item(i);
        QDomElement e1 = cuenta.toElement(); // try to convert the node to an element.
        if( !e1.isNull() ) { // the node was really an element.
		/// Este es el calculo de los saldos para la cuenta
            QString query = "SELECT saldototal('"+e1.text()+"','"+finicial+"','"+ffinal+"') AS valoract, saldototal('"+e1.text()+"','"+finicial1+"','"+ffinal1+"') AS valorant"; 
	    cursor2 *cur = empresaactual->cargacursor(query);
	    if (!cur->eof()) {
		QString valoract = cur->valor("valoract");
		QString valorant = cur->valor("valorant");
		QDomNode c = e1.parentNode();
		agregaValores(c, valoract, valorant);
	    } // end if
	    delete cur;
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



    /// Una vez que tenemos el objeto bien generado y a punto pasamos a la generacion del PDF
    imprimir();

    _depura("END CAnualesPrintView::accept", 0);
}


bool CAnualesPrintView::procesaFormula(const QDomNode &formula) {
    _depura ("CAnualesPrintView::procesaFormula", 0);
    QDomElement valor = formula.firstChildElement("VALORACT");
    if (!valor.isNull())
        return TRUE;

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
            } else
                return FALSE;
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
    if (!valor.isNull())
        return TRUE;

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
	
	Fixed fvaloract(valoract);
	Fixed fvalorant(valorant);

        /// Miramos las opciones pasadas
    QDomNodeList opcs = enodo.elementsByTagName("OPCIONES");
    for (int i = 0; i < opcs.count(); i++) {
        QDomElement op = opcs.item(i).toElement();

	QString opciones = op.text();
	if (opciones == "POSITIVO") {
		if (fvaloract < 0) 
			fvaloract = fvaloract * -1;
		if (fvalorant < 0)
			fvalorant = fvalorant * -1;
	} // end if

	if (opciones == "NEGATIVO") {
		if (fvaloract > 0) 
			fvaloract = fvaloract * -1;
		if (fvalorant > 0)
			fvalorant = fvalorant * -1;
	} // end if

	if (opciones == "MAYORCERO") {
		if (fvaloract < 0) 
			fvaloract = 0;
		if (fvalorant < 0)
			fvalorant = 0;
	} // end if

	if (opciones == "MENORCERO") {
		if (fvaloract > 0) 
			fvaloract = 0;
		if (fvalorant > 0)
			fvalorant = 0;
	} // end if

	if (opciones == "RESTAR") {
			fvaloract = fvaloract * -1;
			fvalorant = fvalorant * -1;
	} // end if
     } // end for


    QDomElement valoract1 = m_doc.createElement("VALORACT");
    valoract1.setTagName("VALORACT");
    QDomText etx = m_doc.createTextNode(fvaloract.toQString());
    valoract1.appendChild(etx);

    QDomElement valorant1 = m_doc.createElement("VALORANT");
    valorant1.setTagName("VALORANT");
    QDomText etc = m_doc.createTextNode(fvalorant.toQString());
    valorant1.appendChild(etc);

    QDomNode n = nodo;
    enodo.appendChild(valoract1);
    enodo.appendChild(valorant1);

    _depura("END CAnualesPrintView::agregaValores", 0);
}


void CAnualesPrintView::imprimir() {
    _depura("CAnualesPrintView::imprimir", 0);
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "canuales.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "canuales.rml";
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

    system (archivologo.ascii());

    QFile file;
    file.setName(archivod);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;

    fitxersortidatxt = "";


    /// Iteramos para cada componente del listado
    QDomNodeList componentes = m_doc.elementsByTagName("COMPONENTE");
    for (int j = 0; j < componentes.count(); j++) {
	QDomElement comp = componentes.item(j).toElement();

	/// Escribimos el titulo del componente
	QDomElement subtitulo = comp.namedItem("SUBTITULO").toElement();
	fitxersortidatxt += ""+subtitulo.text()+"\n";
	
	/// Cogemos los elementos del componente y los ponemos en forma de tabla
	fitxersortidatxt += "<blockTable style=\"tabla\" repeatRows=\"1\">\n";
	fitxersortidatxt += "<tr><td>Concepto</td>\n";
	fitxersortidatxt += "<td>Ejercicio</td>\n";
	fitxersortidatxt += "<td>Ejercicio -1</td></tr>\n";
	
	QDomNodeList litems = comp.elementsByTagName("LBALANCE");
	for (int i = 0; i < litems.count(); i++) {
		QDomNode item = litems.item(i);
		QDomElement e1 = item.toElement(); // try to convert the node to an element.
		if( !e1.isNull() ) { // the node was really an element.
		QDomNode formul = item.firstChildElement("FORMULA");
		QString vact = formul.firstChildElement("VALORACT").toElement().text();
		QString vant = formul.firstChildElement("VALORANT").toElement().text();
		QString texto = item.firstChildElement("CONCEPTO").toElement().text();
		fitxersortidatxt += "<tr>\n";
		fitxersortidatxt += "<td>"+texto+"</td>\n";
		fitxersortidatxt += "<td>"+vact+"</td>\n";
		fitxersortidatxt += "<td>"+vant+"</td>\n";
		fitxersortidatxt += "</tr>\n";
		} // end if
	} // end for
	
	fitxersortidatxt += "</blockTable>\n";
	fitxersortidatxt += "<nextFrame/>\n";

    } // end for


    buff.replace("[story]", fitxersortidatxt);
    QDomElement nodo = m_doc.namedItem("BALANCE").namedItem("TITULO").toElement();	
    buff.replace("[titulo]", nodo.text());

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    } // end if

    invocaPDF("canuales");
    _depura("END CAnualesPrintView::imprimir", 0);

}


void CAnualesPrintView::setidbalance(QString id) {
    QFile f(id);
    if ( !f.open( IO_ReadOnly ) )
        return;
    if ( !m_doc.setContent( &f ) ) {
        f.close();
        return;
    }
    f.close();
    m_nomBalance->setText(id);
}



