/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#include <QWidget>
#include "efqtoolbutton_importar.h"
#include "facturasplist.h"
#include "funcaux.h"

/// Necesarios para importacion de efactura
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QString>
#include <QFileDialog>
#include "facturapview.h"

EFQToolButtonImportar::EFQToolButtonImportar(FacturasProveedorList *faclistado, QWidget *parent) : QToolButton(parent) {
	_depura("EFQToolButtonImportar_EFQToolButtonImportar", 0);
	m_faclistado = faclistado;
	m_companyact = faclistado->get_company();
	connect(this, SIGNAL(clicked()), this, SLOT(click()));
}

EFQToolButtonImportar::~EFQToolButtonImportar() {}

QString EFQToolButtonImportar::obten_valor_nodo(QString nombre, QDomDocument *doc) {
	QDomNodeList lista_nodos = doc->elementsByTagName(nombre);
	QDomNode nodo = lista_nodos.item(0);
	
	if (nodo.isNull())
		_depura("No hay un nodo con ese nombre", 2);
		
	QDomElement e = nodo.toElement();
	
	return e.text();
}

/// ------------------ Importa una factura desde un fichero en formato UBL 1.0 ------------------- ///

void EFQToolButtonImportar::importa_factura_ubl() {

	QString fichero = QFileDialog::getOpenFileName(
			this,
			"Escoja un fichero que contenga una efactura para importarlo a la base de datos de BulmaFact",
			"/home",
			"Todos los archivos (*)"
			);

	/// Si se le da a cancelar en el dialogo anterior
	/// cancelamos la ejecucion del resto del codigo
	if (fichero == "")
		return;
	
	/// Declaramos una variable de tipo QDomDocument que usaremos
	/// para cargar los datos del fichero en memoria
	QDomDocument doc("efactura");
	QFile file(fichero);
	
	if (!file.open(QIODevice::ReadOnly))
		return;

	/// Cargamos los datos en memoria usando el contenido de 'fichero'
	if (!doc.setContent(&file)) {
		file.close();
		return;
	}
	
	file.close();
	
	/// Empezamos a obtener datos de la efactura
	
	QString numeroFactura = obten_valor_nodo("ID", &doc);
	QString fechaFactura = obten_valor_nodo("cbc:IssueDate", &doc);
	QString descFactura = obten_valor_nodo("cbc:Note", &doc);
	QString bimpFactura = obten_valor_nodo("cbc:LineExtensionTotalAmount", &doc);
	QString impFactura = obten_valor_nodo("cbc:TotalTaxAmount", &doc);
	QString totalFactura = obten_valor_nodo("cbc:TaxInclusiveTotalAmount", &doc);
// 	QString descuentoFactura = obten_valor_nodo_hijo("padre", "hijo", &doc);
// 	QString  = obten_valor_nodo("", &doc);
	
	
// 	_depura(descFactura, 2);
	
	/// Mostramos la ficha con la informacion de la factura importada
// 	FacturaProveedorView *fp = new FacturaProveedorView(m_companyact);
// 	fp->show();
}

void EFQToolButtonImportar::click() {
	importa_factura_ubl();
}
