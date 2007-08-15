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

#include <QMessageBox>
#include <QFile>
#include <QTextStream>

#include "inventariosview.h"
#include "company.h"
#include "funcaux.h"
#include "inventarioview.h"


#define COL_IDINVENTARIO 0
#define COL_NOMINVENTARIO 1
#define COL_FECHAINVENTARIO 2


// ==============================================
//             InventariosView
// ==============================================

void InventariosView::setcompany(Company *comp) {
    companyact = comp;
    mui_listado->setEmpresaBase(comp);
}


void InventariosView::on_mui_listado_itemDoubleClicked(QTableWidgetItem *) {
    on_mui_editar_clicked();
}

void InventariosView::on_mui_crear_clicked() {
            InventarioView *bud = new InventarioView(companyact, 0);
            if (bud->cargar("0"))
                return;
            companyact->m_pWorkspace->addWindow(bud);
            bud->show();
            bud->mui_nominventario->setFocus();
}

void InventariosView::on_mui_listado_itemDoubleClicked() {
    on_mui_editar_clicked();
}

InventariosView::InventariosView(QWidget *parent, Qt::WFlags flag)
        : Ficha(parent, flag) {
    setupUi(this);
    companyact = NULL;
    meteWindow(windowTitle(), this);
}


InventariosView::InventariosView(Company *comp, QWidget *parent)
        : Ficha(parent) {
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    companyact = comp;
    mui_listado->setEmpresaBase(comp);
    mui_listado->cargar();
    meteWindow(windowTitle(), this);
}


InventariosView::~InventariosView() {}




void InventariosView::on_mui_editar_clicked() {
    int a = mui_listado->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Tiene que seleccionar un inventario"));
        return;
    } else {
        QString idinventario = mui_listado->item(a, COL_IDINVENTARIO)->text();
        if (idinventario != "") {
            InventarioView *bud = new InventarioView(companyact, 0);
            if (bud->cargar(idinventario))
                return;
            companyact->m_pWorkspace->addWindow(bud);
            bud->show();
            bud->mui_nominventario->setFocus();
        } // end if
    } // end if
}


void InventariosView::on_mui_borrar2_clicked() {
    int a = mui_listado->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Tiene que seleccionar un inventario"));
        return;
    } else {
        QString idinventario = mui_listado->item(a, COL_IDINVENTARIO)->text();
        if (idinventario != "") {
            InventarioView *inv = new InventarioView(companyact, 0);
            companyact->m_pWorkspace->addWindow(inv);
            inv->cargar(idinventario);
            /// Hacemos el borrado sin mostrar pantalla ni nada.
            inv->on_mui_borrar_clicked();
            mui_listado->cargar();
        } // end if
    } // end if
}

void InventariosView::on_mui_imprimir_clicked() {
	_depura("InventariosView::on_mui_imprimir_clicked", 0);

   QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "listado.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "listado.rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system (archivo.toAscii().constData());
    /// Copiamos el logo
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#endif

    system(archivologo.toAscii().constData());


    QFile file;
    file.setFileName(archivod);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString buff = stream.readAll();
    file.close();


	QString txt = "<blockTable style=\"tabla\" repeatRows=\"1\">\n";
	txt += "<tr>\n\t<td></td>\n";
	
	QString query = "SELECT * FROM articulo ";
	cursor2 *almacenes = companyact->cargacursor("SELECT * FROM almacen");
	while (!almacenes->eof()) {
		QString idalmacen = almacenes->valor("idalmacen");
		query += " LEFT JOIN ( SELECT stock AS stock"+idalmacen+", idarticulo FROM stock_almacen WHERE idalmacen="+almacenes->valor("idalmacen")+") AS t" + idalmacen +" ON " + " t"+idalmacen+".idarticulo = articulo.idarticulo";

		txt += "\t<td>" + almacenes->valor("nomalmacen") + "</td>\n";
		almacenes->siguienteregistro();
	} // end while
	txt += "</tr>\n";
	query += " WHERE articulo.stockarticulo <> 0";


	cursor2 *cstock = companyact->cargacursor(query);
	while (!cstock->eof()) {
		txt += "<tr>\n";
		txt += "\t<td>" + cstock->valor("nomarticulo")+"</td>\n";
		almacenes->primerregistro();
		while(!almacenes->eof()) {
			txt += "\t<td>"+cstock->valor("stock"+almacenes->valor("idalmacen"))+"</td>\n";
			almacenes->siguienteregistro();
		} // end while
		cstock->siguienteregistro();
		txt += "</tr>\n";
	} // end while
	delete cstock;
	delete almacenes;
	txt += "</blockTable>\n";

    buff.replace("[story]", txt);
    buff.replace("[titulo]", "Listado de Stocks");


    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    } // end if

    invocaPDF("listado");
	_depura("END InventariosView::on_mui_imprimir_clicked", 0);
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================

void InventariosSubForm::cargar() {
    _depura("InventariosSubForm::cargar", 0);
    QString SQLQuery = "SELECT * FROM inventario";
    cursor2 * cur= empresaBase()->cargacursor(SQLQuery);
    SubForm3::cargar(cur);
    delete cur;
    _depura("END InventariosSubForm::cargar", 0);
}

InventariosSubForm::InventariosSubForm(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("inventario");
    setDBCampoId("idinventario");
    addSHeader("idinventario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView, tr("Id inventario"));
    addSHeader("nominventario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre del inventario"));
    addSHeader("fechainventario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha del inventario"));
    setinsercion(FALSE);
}

