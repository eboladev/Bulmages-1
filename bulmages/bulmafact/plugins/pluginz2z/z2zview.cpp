/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#include <QCloseEvent>
#include <QComboBox>
#include <QLayout>
#include <QMessageBox>
#include <QObject>
#include <QToolButton>
#include <QWidget>
#include <QDateTime>
#include <QTextStream>
#include <fstream>

#include "z2zview.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "plugins.h"


/** Inicializa la pantalla.
    Inicializa todos los componentes y mete la pantalla en el workSpace.
*/
/**
\param comp
\param parent
**/
Z2ZView::Z2ZView ( BfCompany *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "Z2ZView::Z2ZView", 0 );
    setupUi ( this );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {

        meteWindow ( windowTitle(), this, FALSE );
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear la factura" ), this );
    }
    _depura ( "END Z2ZView::Z2ZView" );
}


/** Al destruir la clase provocamos que se refresque el listado de facturas.
    Para que salga actualizado.
*/
/**
**/
Z2ZView::~Z2ZView()
{
    _depura ( "Z2ZView::~Z2ZView", 0 );
    _depura ( "END Z2ZView::~Z2ZView", 0 );
}


void Z2ZView::on_mui_referencia_returnPressed() {
	new QListWidgetItem(mui_referencia->text(), mui_listarefs);
	calculaTotalTickets();
	mui_referencia->setText("");
	mui_referencia->setFocus();
}

void 	Z2ZView::on_mui_listarefs_itemDoubleClicked(QListWidgetItem *it) {
	delete it;
	calculaTotalTickets();
}


void Z2ZView::calculaTotalTickets() {
	QString listarefs;
	if (mui_listarefs->count() > 0) {
		listarefs = "upper('" + mui_listarefs->item(0)->text() + "')";
		for (int i=1; i < mui_listarefs->count(); i++) {
			listarefs += ", upper('" + mui_listarefs->item(i)->text() + "')";
		} // end for
	} else {
		mensajeInfo("lista vacia");
		return;
	} // end if


	QString query = "SELECT sum(pvpivainclalbaran*cantlalbaran)::NUMERIC(12,2) AS tot FROM lalbaran LEFT JOIN albaran ON albaran.idalbaran= lalbaran.idalbaran WHERE upper(albaran.refalbaran) IN ("+listarefs+")";
	

	BlDbRecordSet *cur = empresaBase()->cargacursor(query);
	mui_totaltickets->setText(cur->valor("tot"));
	delete cur;

	BlFixed total("0.00");
	BlFixed min("0.00");

	for (int i=0; i < mui_listarefs->count(); i++) {

		QString query1 = "SELECT COALESCE(sum(pvpivainclalbaran*cantlalbaran)::NUMERIC(12,2), 0) AS tot FROM lalbaran LEFT JOIN albaran ON albaran.idalbaran= lalbaran.idalbaran WHERE upper(albaran.refalbaran) = upper('" + mui_listarefs->item(i)->text() + "')";
		BlDbRecordSet *cur1 = empresaBase()->cargacursor(query1);
		total = total + BlFixed(cur1->valor("tot"));
		delete cur1;

		QString query2 = "SELECT COALESCE(min(pvpivainclalbaran),0) AS tot FROM lalbaran LEFT JOIN albaran ON albaran.idalbaran= lalbaran.idalbaran WHERE upper(albaran.refalbaran) = upper('" + mui_listarefs->item(i)->text() + "')";
		BlDbRecordSet *cur2 = empresaBase()->cargacursor(query2);
		min = min + BlFixed(cur2->valor("tot"));
		delete cur2;
	} // end for

	BlFixed traspasable = total - min;
	mui_totaltraspasable->setText(traspasable.toQString());
}


void Z2ZView::on_mui_traspasar_clicked() {

	BlDbRecordSet *cur4 = NULL;

try {

    BlFixed totaltraspasable (mui_totaltraspasado->text());
    BlFixed totaltraspasado("0.00");

    if (confpr->valor(CONF_FACT_ALT) == "") {
	mensajeInfo("Debe configurar el parametro CONF_FACT_ALT");
	return;
    } // end if
    BlPostgreSqlClient *db;
    db = new BlPostgreSqlClient();
    db->inicializa ( confpr->valor(CONF_FACT_ALT) );
    db->begin();

    QFile file ( confpr->valor ( CONF_DIR_USER ) + "z2z_" + empresaBase()->nameDB() + "_" + QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm") +".sql" );
    /// Guardado del orden y de configuraciones varias.
    if (! file.open ( QIODevice::WriteOnly ) ) {
	return;
    } // end if
    QTextStream stream ( &file );

    QFile file1 ( confpr->valor ( CONF_DIR_USER ) + "insert_z2z_" + empresaBase()->nameDB() + "_" + QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm") +".sql" );
    /// Guardado del orden y de configuraciones varias.
    if (! file1.open ( QIODevice::WriteOnly ) ) {
	file.close();
	return;
    } // end if
    QTextStream stream1 ( &file1 );
	
    QFile file2 ( confpr->valor ( CONF_DIR_USER ) + "delete_z2z_" + empresaBase()->nameDB() + "_" + QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm") +".sql" );
    /// Guardado del orden y de configuraciones varias.
    if (! file2.open ( QIODevice::WriteOnly ) ) {
	file.close();
	file1.close();
	return;
    } // end if
    QTextStream stream2 ( &file2 );

    QFile file3 ( confpr->valor ( CONF_DIR_USER ) + "refs_z2z_" + empresaBase()->nameDB() + "_" + QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm") +".sql" );
    /// Guardado del orden y de configuraciones varias.
    if (! file3.open ( QIODevice::WriteOnly ) ) {
	file.close();
	file1.close();
	file2.close();
	return;
    } // end if
    QTextStream stream3 ( &file3 );


		QString query7 = "INSERT INTO z (idalmacen) VALUES (3)";
		db->ejecuta(query7);

		QString query8 = "SELECT max(idz) as idz from z";
		BlDbRecordSet *curz = db->cargacursor(query8);


	for (int i=0; i < mui_listarefs->count(); i++) {


		/// Guardamos la referencia.
		stream3 << mui_listarefs->item(i)->text() << endl;

		/// Buscamos el minimo del albaran pasado.
		QString query2 = "SELECT COALESCE(min(pvpivainclalbaran),0) AS tot FROM lalbaran LEFT JOIN albaran ON albaran.idalbaran= lalbaran.idalbaran WHERE upper(albaran.refalbaran) = upper('" + mui_listarefs->item(i)->text() + "')";
		BlDbRecordSet *cur2 = empresaBase()->cargacursor(query2);
		QString min = cur2->valor("tot");
		delete cur2;

		QString query3 = "SELECT * FROM  albaran WHERE upper(albaran.refalbaran) = upper('" + mui_listarefs->item(i)->text() + "')";
		BlDbRecordSet *cur3 = empresaBase()->cargacursor(query3);

		/// Los albaranes con visa no los ponemos
		if (cur3->valor("idforma_pago") != "2") {


			/// Iteramos sobre las lineas de albaran.
			QString query1 = "SELECT * FROM lalbaran LEFT JOIN albaran ON albaran.idalbaran= lalbaran.idalbaran WHERE upper(albaran.refalbaran) = upper('" + mui_listarefs->item(i)->text() + "')";
			BlDbRecordSet *cur1 = empresaBase()->cargacursor(query1);

			if (!cur1->eof() && totaltraspasado < totaltraspasable) {
				QString query4 = "INSERT INTO albaran (refalbaran, idcliente, idalmacen, idz, idforma_pago) VALUES ('"+ cur3->valor("refalbaran")+"', "+cur3->valor("idcliente")+", "+cur3->valor("idalmacen")+", "+curz->valor("idz")+", "+cur3->valor("idforma_pago")+")";
				db->ejecuta(query4);
				delete cur3;
			} // end if

			QString query5 = "SELECT max(idalbaran) as idalbaran from albaran";
			cur4 = db->cargacursor(query5);

			while (!cur1->eof() && totaltraspasado < totaltraspasable) {

				/// Generamos el archivo de cambios.
				stream << "INSERT INTO lalbaran (cantlalbaran, pvpivainclalbaran, desclalbaran, idarticulo, idalbaran ) VALUES ("+cur1->valor("cantlalbaran")+","+cur1->valor("pvpivainclalbaran")+",'"+cur1->valor("desclalbaran")+"',"+cur1->valor("idarticulo")+", "+cur4->valor("idalbaran")+");" << endl;
				
				QString query6 = "INSERT INTO lalbaran (cantlalbaran, pvpivainclalbaran, ivalalbaran, desclalbaran, idarticulo, idalbaran ) VALUES ("+cur1->valor("cantlalbaran")+","+cur1->valor("pvpivainclalbaran")+", 0,'"+cur1->valor("desclalbaran")+"',"+cur1->valor("idarticulo")+", "+cur4->valor("idalbaran")+")";
				db->ejecuta(query6);
	
				totaltraspasado = totaltraspasado + BlFixed(cur1->valor("cantlalbaran")) * BlFixed(cur1->valor("pvpivainclalbaran"));

				/// Hacemos el reversible para que pueda volverse atras el paso.
				stream1 << "INSERT INTO lalbaran (cantlalbaran, pvpivainclalbaran, desclalbaran, idarticulo, idalbaran ) VALUES ("+cur1->valor("cantlalbaran")+","+cur1->valor("pvpivainclalbaran")+",'"+cur1->valor("desclalbaran")+"',"+cur1->valor("idarticulo")+","+cur1->valor("idalbaran")+");" << endl;
	
				stream2 << "DELETE FROM lalbaran WHERE numlalbaran="+cur1->valor("numlalbaran") << endl;
				QString query9 = "DELETE FROM lalbaran WHERE numlalbaran = " +cur1->valor("numlalbaran");
				empresaBase()->ejecuta(query9);

				cur1->siguienteregistro();
			} // end while
			delete cur1;
	
			delete cur4;
		} // end if
	} // end for
	
	delete curz;
	file.close();
	file1.close();
	file2.close();
	file3.close();
	QString query15 = "UPDATE z set idz=idz";
	db->ejecuta(query15);

	db->commit();
	delete db;
} catch(...) {
	mensajeInfo("Error en el traspaso");
}// end try

}



