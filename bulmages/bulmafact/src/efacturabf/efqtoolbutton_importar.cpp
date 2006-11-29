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
#include <QMap>
#include <QList>
#include "facturapview.h"
#include "fixed.h"

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

QString EFQToolButtonImportar::obten_descuento_factura(QDomDocument *doc) {
	// Obtenemos el nodo padre
	QDomNodeList lista_padre = doc->elementsByTagName("Invoice");
	QDomNode padre = lista_padre.item(0);
		
	QDomNode nodo1, nodo2;
	QDomElement e;
	
	Fixed total_descuento = "0.00";
	
	int i = 0;
	
	/// Nos movemos entre los nodos hijos cac:AllowanceCharge
	/// Ojo, no confundir con los elementos cac:AllowanceCharge que estan
	/// dentro del elemento cac:InvoiceLine. Por eso se hace este tipo de
	/// recorrido. Cogemos los nodos cac:AllowanceCharge que son hijos
	/// del nodo raiz, no todos los nodos cac:AllowanceCharge.
	nodo1 = padre.firstChildElement("cac:AllowanceCharge");
	
	while ( !nodo1.isNull() ) {
		
		/// Nos situamos sobre el elemento que contiene la cantidad
		nodo2 = nodo1.firstChildElement("cbc:Amount");
		
		e = nodo2.toElement();
		
		/// Acumulamos...
		total_descuento = total_descuento + Fixed(e.text());
		/// Siguiente descuento, si existe
		nodo1 = nodo1.nextSiblingElement("cac:AllowanceCharge");
		
		i++;
	}
	
	return total_descuento.toQString();
}

void EFQToolButtonImportar::obten_linea_factura(QDomDocument *doc, QMap<QString, QString> &mapa_lfactura, int i) {
	QDomNodeList lista_lineas = doc->elementsByTagName("cac:InvoiceLine");
	QDomNode padre = lista_lineas.item(i);
	QDomNode tmp;
	
	// Buscamos la descripcion de esa linea de factura
	tmp = padre;
	tmp = tmp.firstChildElement("cac:Item");
	tmp = tmp.firstChildElement("cbc:Description");
	
	mapa_lfactura["desclfactura"] = tmp.toElement().text();
	
	// Buscamos la cantidad de elementos pertenecientes a la linea
	tmp = padre;
	tmp = tmp.firstChildElement("cbc:InvoicedQuantity");
	
	mapa_lfactura["cantlfactura"] = tmp.toElement().text();
	
	// Buscamos PVP de la linea
	tmp = padre;
	tmp = tmp.firstChildElement("cbc:LineExtensionAmount");
	
	mapa_lfactura["pvplfactura"] = tmp.toElement().text();
	
	// Buscamos IVA de la linea
	tmp = padre;
	tmp = tmp.firstChildElement("cac:TaxTotal");
	tmp = tmp.firstChildElement("cbc:TotalTaxAmount");
	
	mapa_lfactura["ivalfactura"] = tmp.toElement().text();
	
	// Buscamos descuento de la linea
	tmp = padre;
	tmp = tmp.firstChildElement("cac:AllowanceCharge");
	tmp = tmp.firstChildElement("cbc:MultiplierFactorNumeric");
	
	mapa_lfactura["descuentolfactura"] = tmp.toElement().text();
	
	// Buscamos ID de articulo de la linea
	tmp = padre;
	tmp = tmp.firstChildElement("cac:Item");
	tmp = tmp.firstChildElement("cac:SellersItemIdentification");
	tmp = tmp.firstChildElement("cac:ID");
	
	mapa_lfactura["idarticulo"] = tmp.toElement().text();
	
	return;
}

QString EFQToolButtonImportar::obten_id_proveedor(QDomDocument *doc) {

	// Nos situamos sobre la parte que identifica al vendedor
	QDomNodeList lista_nodos = doc->elementsByTagName("cac:SellerParty");
	// Obtenemos el nodo padre de esta seccion
	QDomNode padre = lista_nodos.item(0);
	// Lo recorremos hasta llegar a donde nos interesa
	QDomNode nodo = padre;;
	
	nodo = nodo.firstChildElement("cac:Party");
	nodo = nodo.firstChildElement("cac:PartyIdentification");
	nodo = nodo.firstChildElement("cac:ID");
	
	// Obtenemos el valor de ese nodo transformandolo en QDomElement
	// y usando el metodo text()
	QDomElement e = nodo.toElement();
	
	return e.text();
}

/// ------------------ Importa una factura desde un fichero en formato UBL 1.0 ------------------- ///

void EFQToolButtonImportar::importa_factura_ubl() {
	_depura("EFQToolButtonImportar::importa_factura_ubl 1", 0);

	
	QString fichero = QFileDialog::getOpenFileName(
			this,
			"Escoja un fichero que contenga una efactura para importarlo a la base de datos de BulmaFact",
			"/tmp",
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
	
	/// Comprobamos que el proveedor existe. Si no, abortamos y damos mensaje de error.
	QString idProveedor = obten_id_proveedor(&doc);
	
	QString query = "SELECT * FROM proveedor WHERE cifproveedor = '" + idProveedor + "'";
	cursor2 *proveedor = m_companyact->cargacursor(query);
	
	if (proveedor->numregistros() == 0) {
		_depura("El proveedor con CIF " + idProveedor + " no existe en la base de datos. Hay que crearlo antes de importar esta factura.", 2);
		
		return;
	}
	
	QString descuentoFactura = obten_descuento_factura(&doc);
	
/// Obtenemos lineas de factura ------------------------------------------------------------------------
	
	// Contamos las lineas que hay
	
	QDomNodeList lista_lfactura = doc.elementsByTagName("cac:InvoiceLine");
	
	lista_lfactura.item(0);
	
	int numlineas = 0;
	
	for (int i = 0; !lista_lfactura.item(i).isNull(); i++)
		numlineas = i+1;
	
	if (numlineas == 0) {
		_depura("Esta factura es erronea. No contiene ninguna linea asociada", 2);
		exit(-1);
	} 
// 	else {
// 		_depura("Hay lineas de factura: " + QString::number(numlineas), 2);
// 	}
	
/// Vamos a usar un QMap para ir recorriendo las lineas de factura y los valores
/// los iremos guardando, una vez obtenidos los que nos interesan, en una lista de QMaps.

	QMap<QString, QString> mapa_lfactura;
	QList< QMap<QString, QString> > lista_mapas_lfactura;
	
/// 	Estas son las claves que vamos a usar dentro del QMap mapa_lfactura

/// 	mapa_lfactura["desclfactura"]
/// 	mapa_lfactura["cantlfactura"]
/// 	mapa_lfactura["pvplfactura"]
/// 	mapa_lfactura["ivalfactura"]
/// 	mapa_lfactura["descuentolfactura"]
/// 	mapa_lfactura["idarticulo"]
	
	
	/// Mientras haya lineas, las vamos obteniendo
	for (int i = 0; i < numlineas; i++) {
		obten_linea_factura(&doc, mapa_lfactura, i);
		lista_mapas_lfactura.append(mapa_lfactura);
	}
	
	/// Debug en pantalla
// 	for (int i = 0; i < lista_mapas_lfactura.size(); i++) {
// 		mapa_lfactura = lista_mapas_lfactura.at(i);
// 		_depura(mapa_lfactura["desclfactura"] + "--" + mapa_lfactura["cantlfactura"] + "--" + mapa_lfactura["pvplfactura"] + "--" + mapa_lfactura["ivalfactura"] + "--" + mapa_lfactura["descuentolfactura"] + "--" + mapa_lfactura["idarticulo"], 2);
// 	}
	
/// FIN Obtenemos lineas de factura --------------------------------------------------------------------
		
	/// Mostramos la ficha con la informacion de la factura importada
	
	FacturaProveedorView *fp = m_companyact->newFacturaProveedorView();
	m_companyact->m_pWorkspace->addWindow(fp);
	fp->inicializar();
	fp->pintar();
	fp->show();

/*
addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Articulo"));
addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo completo"));
addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("Nombre"));
addSHeader("idlfacturap", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("Linea"));
addSHeader("desclfacturap", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr("Descripcion"));
addSHeader("cantlfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Cantidad"));
addSHeader("pvplfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("P.V.P."));
addSHeader("ivalfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("% I.V.A."));
addSHeader("descuentolfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Descuento"));
addSHeader("idfacturap", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Factura"));
*/		

/// Creamos las lineas de factura --------------------------------------------

	ListLinFacturaProveedorView *lineas = fp->getlistalineas();
	SDBRecord *rec = lineas->lista()->last();
	cursor2 *articulo = NULL;

	for (int i = 0; i < numlineas; i++) {
		lineas->setinsercion(FALSE); /// Hace falta que este en el bucle? preguntar a Tomeu
		mapa_lfactura = lista_mapas_lfactura.at(i);	

		/// Comprobamos que existe un articulo con ese codigo en la BD
		/// Si no es asi, mandamos una alerta al usuario y anyadimos ese
		/// articulo a la linea como articulo generico		
		query = "SELECT * FROM articulo WHERE codarticulo = '" + mapa_lfactura["idarticulo"] + "'";
		articulo = m_companyact->cargacursor(query);
		
		if (articulo->numregistros() == 0) {
			_depura("El articulo con codigo completo " + mapa_lfactura["idarticulo"] + " no existe en la base de datos. Se importara como articulo generico.", 2);
			
			mapa_lfactura["idarticulo"] = "ARTGEN";
		}
			
		rec->setDBvalue("desclfacturap", mapa_lfactura["desclfactura"]);
		rec->setDBvalue("cantlfacturap", mapa_lfactura["cantlfactura"]);
		rec->setDBvalue("pvplfacturap", mapa_lfactura["pvplfactura"]);
		rec->setDBvalue("ivalfacturap", mapa_lfactura["ivalfactura"]);
		rec->setDBvalue("descuentolfacturap", mapa_lfactura["descuentolfactura"]);
		
		rec->setDBvalue("codarticulo", mapa_lfactura["idarticulo"]); // poner el del generico

// 		rec->setDBvalue("idarticulo", mapa_lfactura["idarticulo"]);
// 		rec->setDBvalue("", mapa_lfactura[""]);

		lineas->setinsercion(TRUE);
		lineas->nuevoRegistro();
		
		rec = lineas->lista()->last();
	} // end for
	
	delete articulo;
	delete rec;
	
/// Creamos los descuentos --------------------------------------------
/*	
	ListDescuentoFacturaProvView *descuentos = fp->getlistadescuentos();
	SDBRecord *rec = descuentos->lista()->last();
	cursor2 *articulo = NULL;

	for (int i = 0; i < numlineas; i++) {
		lineas->setinsercion(FALSE); /// Hace falta que este en el bucle? preguntar a Tomeu
		mapa_lfactura = lista_mapas_lfactura.at(i);	
			
		rec->setDBvalue("desclfacturap", mapa_lfactura["desclfactura"]);
		rec->setDBvalue("cantlfacturap", mapa_lfactura["cantlfactura"]);
		rec->setDBvalue("pvplfacturap", mapa_lfactura["pvplfactura"]);
		rec->setDBvalue("ivalfacturap", mapa_lfactura["ivalfactura"]);
		rec->setDBvalue("descuentolfacturap", mapa_lfactura["descuentolfactura"]);
		
		rec->setDBvalue("codarticulo", mapa_lfactura["idarticulo"]); // poner el del generico

// 		rec->setDBvalue("idarticulo", mapa_lfactura["idarticulo"]);
// 		rec->setDBvalue("", mapa_lfactura[""]);

		lineas->setinsercion(TRUE);
		lineas->nuevoRegistro();
		
		rec = lineas->lista()->last();
	} // end for
	
	delete articulo;
	delete rec;
*/

// 	fp->cargar("0");
// 	fp->show();
	
	fp->pintanumfacturap(numeroFactura);
	fp->pintafechafacturap(fechaFactura);
	fp->pintadescfacturap(descFactura);
	fp->pintaidproveedor(proveedor->valor("idproveedor"));
	
	delete proveedor;
	
	Fixed bimp(bimpFactura);
	Fixed iva(impFactura);
	
 	fp->pintatotales(bimp, iva);
	fp->m_totalDiscounts->setText(descuentoFactura);
	
	/// Que seleccione la forma de pago el que esta importanto la factura
	/// Esto lo hacemos asi porque guardamos este campo como una cadena
	/// de texto dentro de la efactura, al ser algo tan variable mejor que
	/// lo haga de nuevo el propio usuario
	fp->pintaidforma_pago("");
	
	//dibujar_lineas_factura;
	//dibujar_descuentos_factura

// 	delete fp;
}

void EFQToolButtonImportar::click() {
	importa_factura_ubl();
}
