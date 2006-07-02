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

#include "linpresupuesto.h"


void linpresupuesto::definetabla() {
    setDBTableName("lpresupuesto");
    setDBCampoId("idlpresupuesto");
    addDBCampo("idlpresupuesto", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Linea Presupuesto");
    addDBCampo("idpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("desclpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, "Descripcion");
    addDBCampo("cantlpresupuesto", DBCampo::DBnumeric, DBCampo::DBNotNull, "Cantidad");
    addDBCampo("pvplpresupuesto", DBCampo::DBnumeric, DBCampo::DBNotNull, "Precio Linea Presupuesto");
    addDBCampo("descuentolpresupuesto", DBCampo::DBnumeric, DBCampo::DBNothing, "Descuento Linea Presupuesto");
    addDBCampo("idarticulo", DBCampo::DBint, DBCampo::DBNothing, "Articulo Presupuesto");
    addDBCampo("ivalpresupuesto", DBCampo::DBnumeric, DBCampo::DBNothing, "IVA linea Presupuesto");
    addDBCampo("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, "Codigo Articulo");
    addDBCampo("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, "Nombre 	Articulo");
}


linpresupuesto::linpresupuesto(company *comp) : DBRecord(comp) {
    companyact = comp;
    definetabla();
}


linpresupuesto::linpresupuesto(company *comp, QString idlinpresupuesto) : DBRecord(comp) {
    companyact = comp;
    definetabla();
    QString SQLQuery = "SELECT * FROM lpresupuesto, articulo WHERE lpresupuesto.idarticulo=articulo.idarticulo AND idlpresupuesto=" + idlinpresupuesto;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        DBload(cur);
    } else {
        vacialinpresupuesto();
    } // end if
}


linpresupuesto::linpresupuesto(company *comp, QString a, QString b, QString c, QString d, QString e, QString f, QString g, QString h, QString i, QString j)
        : DBRecord(comp) {
    companyact = comp;
    definetabla();
    setDBvalue("idlpresupuesto", a);
    setDBvalue("desclpresupuesto", b);
    setDBvalue("cantlpresupuesto", c);
    setDBvalue("pvplpresupuesto",d);
    setDBvalue("descuentolpresupuesto", e);
    setDBvalue("idpresupuesto", f);
    setDBvalue("idarticulo", g);
    setDBvalue("codigocompletoarticulo", h);
    setDBvalue("nomarticulo", i);
    setDBvalue("ivalpresupuesto", j);
}


linpresupuesto::linpresupuesto(company *comp, cursor2 *cur)
        : DBRecord(comp) {
    companyact = comp;
    definetabla();
    DBload(cur);
}


linpresupuesto::~linpresupuesto() {}


void linpresupuesto::vacialinpresupuesto() {
    DBclear();
}


void linpresupuesto::guardalinpresupuesto() {
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return;
    } // end if
    setDBvalue("idlpresupuesto", id);
    companyact->commit();
}


void linpresupuesto::setcodigocompletoarticulo(QString val) {
    _depura("setcodigocompletoarticulo()\n", 0);
    setDBvalue("codigocompletoarticulo",val);
    QString SQLQuery = "SELECT nomarticulo, idarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE codigocompletoarticulo='" + val + "'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        setDBvalue("nomarticulo", cur->valor("nomarticulo"));
        setDBvalue("desclpresupuesto", cur->valor("nomarticulo"));
        setDBvalue("idarticulo", cur->valor("idarticulo"));
        setDBvalue("pvplpresupuesto", cur->valor("pvp"));
        setDBvalue("ivalpresupuesto", cur->valor("iva"));
        if (DBvalue("cantlpresupuesto") == "") {
            setDBvalue("cantlpresupuesto" , "1");
            setDBvalue("descuentolpresupuesto" , "0");
        } // end if
    } // end if
    delete cur;
}


void linpresupuesto::setidarticulo(QString val) {
    _depura("setidarticulo()\n", 0);
    setDBvalue("idarticulo", val);
    QString SQLQuery = "SELECT nomarticulo, codigocompletoarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE idarticulo=" + val + "";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        setDBvalue("nomarticulo", cur->valor("nomarticulo"));
        setDBvalue("desclpresupuesto", cur->valor("nomarticulo"));
        setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
        setDBvalue("pvplpresupuesto", cur->valor("pvp"));
        setDBvalue("ivalpresupuesto", cur->valor("iva"));
        if (DBvalue("cantlpresupuesto") == "") {
            setDBvalue("cantlpresupuesto", "1");
            setDBvalue("descuentolpresupuesto", "0");
        } // end if
    } // end if
    delete cur;
    _depura("end setidarticulo\n", 0);
}


float linpresupuesto::calculabase() {
    _depura("calculabase()\n", 0);
    float cant = 0;
    if (DBvalue("cantlpresupuesto") != "" && DBvalue("pvplpresupuesto") != "" && DBvalue("desclpresupuesto") != "") {
        cant = DBvalue("cantlpresupuesto").toFloat() * DBvalue("pvplpresupuesto").toFloat();
        cant = cant - (cant* DBvalue("desclpresupuesto").toFloat());
    } // end if
    return cant;
}


float linpresupuesto::calculaiva() {
    _depura("calculaiva()\n", 0);
    float cant = 0;
    if (DBvalue("cantlpresupuesto") != "" && DBvalue("pvplpresupuesto") != "" && DBvalue("desclpresupuesto") != "" && DBvalue("ivalpresupuesto") != "") {
        cant = DBvalue("cantlpresupuesto").toFloat() * DBvalue("pvplpresupuesto").toFloat();
        cant = cant - (cant* DBvalue("desclpresupuesto").toFloat() / 100);
        cant = cant * DBvalue("ivalpresupuesto").toFloat() / 100;
    } // end if
    return cant;
}

