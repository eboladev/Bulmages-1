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

#include "inventario.h"
#include "company.h"
#include "funcaux.h"


Inventario::Inventario(company *comp) : DBRecord(comp) {
    _depura("Inventario::Inventario", 0);
    companyact = comp;
    setDBTableName("inventario");
    setDBCampoId("idinventario");
    addDBCampo("idinventario", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("Inventario", "Identificador inventario"));
    addDBCampo("fechainventario", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Inventario", "Fecha inventario"));
    addDBCampo("nominventario", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Inventario", "Nombre inventario"));
}


Inventario::~Inventario() {}


int Inventario::borrar() {
    if (DBvalue("idinventario") != "") {
        _depura("vamos a borrar las lineas del inventario", 0);
        companyact->begin();

        listalineas->borrar();
        _depura("Vamos a borrar el resto",0);
        int error = companyact->ejecuta("DELETE FROM inventario WHERE idinventario = " + DBvalue("idinventario"));
        if (error) {
            companyact->rollback();
            return -1;
        } // end if
        companyact->commit();
    } // end if
    return 0;
}


void Inventario::vaciaInventario() {
    DBclear();
}


void Inventario::pintaInventario() {
    _depura("pintaInventario\n", 0);
    pintaidinventario(DBvalue("idinventario"));
    pintanominventario(DBvalue("nominventario"));
    pintafechainventario(DBvalue("fechainventario"));
    /// Pinta el subformulario de detalle del Inventario.
    //listalineas->pintaListControlStock();
}


/// Esta funcion carga un Inventario.
int Inventario::cargar(QString idbudget) {
    QString query = "SELECT * FROM inventario WHERE idinventario = " + idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;
    listalineas->cargar(idbudget);
    pintaInventario();
    return 0;
}


int Inventario::guardar() {
    _depura("Inventario::guardar()", 0);
    companyact->begin();
    try {
        QString id;
        DBsave(id);
        setidinventario(id);
        listalineas->guardar();
        companyact->commit();
        cargar(id);
        _depura("END Inventario::guardar()", 0);
        return 0;
    } catch (...) {
        _depura("Error guardando el inventario", 2);
        companyact->rollback();
        return -1;
    } // end try
}


void Inventario::pregenerar() {
    _depura("Inventario::pregenerar", 0);
    listalineas->pregenerar();
    _depura("END Inventario::pregenerar", 0);
}

void Inventario::setListControlStock(ListControlStockView *a) {
    _depura("Inventario::setListControlStock", 0);
    listalineas = a;
    listalineas->setcompany(companyact);
}

ListControlStockView *Inventario::getlistalineas() {
    return listalineas;
}

void Inventario::pintaidinventario(QString) {}
void Inventario::pintafechainventario(QString) {}
void Inventario::pintanominventario(QString) {}
void Inventario::setidinventario(QString val) {
    setDBvalue("idinventario", val);
    listalineas->setColumnValue("idinventario", val);
}
void Inventario::setfechainventario(QString val) {
    setDBvalue("fechainventario", val);
}
void Inventario::setnominventario(QString val) {
    setDBvalue("nominventario", val);
}



void Inventario::imprimirInventario() {
	_depura("Inventario::imprimirInventario", 0);
	QString txt = "<blockTable>\n";
	txt += "<tr><td></td>\n";
	
	QString query = "SELECT idarticulo, codigocompeltoarticulo, nomarticulo FROM articulo ";
	cursor2 *almacenes = companyact->cargacursor("SELECT * FROM almacen");
	while (!almacenes->eof()) {
		QString idalmacen = almacenes->valor("idalmacen");
		query += " LEFT JOIN ( SELECT stock, idarticulo FROM stock_almacen WHERE idalmacen="+almacenes->valor("idalmacen")+") AS t" + idalmacen +" ON " + " t"+idalmacen+".idarticulo = articulo.idarticulo";
		txt += "<td>" + almacenes->valor("nomalmacen") + "</td>";
		almacenes->siguienteregistro();
	} // end while
	delete almacenes;
	txt += "</tr>\n";

	cursor2 *cstock = companyact->cargacursor(query);
	while (!cstock->eof()) {
		txt += "<tr>\n";
		txt += "<td>" + cstock->valor("nomarticulo");
		for (int i = 0; i < cstock->numcampos(); i++) {
			txt += "<td>"+cstock->valor(i)+"</td>";
		} // end for
		cstock->siguienteregistro();
		txt += "\n";
		txt += "</tr>\n";
	} // end while
	delete cstock;

	_depura(txt, 2);
	_depura("END Inventario::imprimirInventario", 0);
}

