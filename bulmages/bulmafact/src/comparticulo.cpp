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

#include "comparticulo.h"


void CompArticulo::definetabla() {
    _depura("CompArticulo::definetabla", 0);
    setDBTableName("comparticulo");
    setDBCampoId("idcomponente");
    addDBCampo("idarticulo", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("ID articulo", "comparticulo"));
    addDBCampo("idcomponente", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("ID componente", "comparticulo"));
    addDBCampo("cantcomparticulo", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate("Cantidad", "comparticulo"));
    addDBCampo("codigocompletocomponente", DBCampo::DBvarchar, DBCampo::DBNoSave, QApplication::translate("Codigo completo del componente", "comparticulo"));
    addDBCampo("nomcomponente", DBCampo::DBvarchar, DBCampo::DBNoSave, QApplication::translate("Nombre del componente", "comparticulo"));
}


CompArticulo::CompArticulo(company *comp) : DBRecord(comp) {
    _depura("CompArticulo::CompArticulo", 0);
    companyact = comp;
    definetabla();
}


CompArticulo::CompArticulo(company *comp, QString idarticulo, QString idcomponente) : DBRecord(comp) {
    _depura("CompArticulo::CompArticulo", 0);
    companyact = comp;
    definetabla();
    QString SQLQuery = "SELECT * FROM comparticulo, articulo WHERE comparticulo.idcomponente=articulo.idarticulo AND idarticulo=" + idarticulo + " AND idcomponente=" + idcomponente;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        DBload(cur);
    } else {
        vaciaCompArticulo();
    } // end if
}


CompArticulo::CompArticulo(company *comp, QString a, QString b, QString c, QString d, QString e)
        : DBRecord(comp) {
    _depura("CompArticulo::CompArticulo", 0);
    companyact = comp;
    definetabla();
    setDBvalue("idarticulo", a);
    setDBvalue("idcomponente", b);
    setDBvalue("cantcomparticulo", c);
    setDBvalue("codigocompletocomponente",d);
    setDBvalue("nomcomponente", e);
}


CompArticulo::~CompArticulo() {}


void CompArticulo::vaciaCompArticulo() {
    DBclear();
}


void CompArticulo::guardaCompArticulo() {
    /// Segun esta la linea en la base de datos o no se hace una cosa u otra.
    try {
        if (DBvalue("idarticulo") != "" && DBvalue("idcomponente") != "") {
            QString SQLQuery = "INSERT INTO comparticulo (idarticulo, idcomponente, cantcomparticulo) VALUES (" +
                               DBvalueprep("idarticulo") + "," +
                               DBvalueprep("idcomponente") + "," +
                               DBvalueprep("cantcomparticulo") + ")";
            companyact->begin();
            companyact->ejecuta("DELETE FROM comparticulo WHERE idarticulo=" + DBvalueprep("idarticulo") + " AND idcomponente=" + DBvalueprep("idcomponente"));
            companyact->ejecuta(SQLQuery);
            companyact->commit();
        } // end if
    } catch(...) {
        _depura("se produjo un error al guardar componentes del articulo", 1);
        companyact->rollback();
    }
}


void CompArticulo::setcodigocompletocomponente(QString val) {
    _depura("setcodigocompletoarticulo()\n", 0);
    setDBvalue("codigocompletocomponente", val);
    QString SQLQuery = "SELECT nomarticulo, idarticulo FROM articulo WHERE codigocompletoarticulo='" + val + "'";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        setDBvalue("nomcomponente", cur->valor("nomarticulo"));
        setDBvalue("idcomponente", cur->valor("idarticulo"));
    } // end if
    delete cur;
}


void CompArticulo::setidcomponente(QString val) {
    _depura("setidcomponente()\n", 0);
    setDBvalue("idcomponente", val);
    QString SQLQuery = "SELECT nomarticulo, codigocompletoarticulo FROM articulo WHERE idarticulo=" + val + "";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        setDBvalue("nomcomponente", cur->valor("nomarticulo"));
        setDBvalue("codigocompletocomponente", cur->valor("codigocompletoarticulo"));
    } // end if
    delete cur;
}

