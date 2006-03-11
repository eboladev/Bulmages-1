//
// C++ Implementation: LTarifa
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ltarifa.h"

void LTarifa::definetabla() {
    setDBTableName("ltarifa");
    setDBCampoId("idltarifa");
    addDBCampo("idltarifa", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Linea Presupuesto");
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("idtarifa", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("pvpltarifa", DBCampo::DBnumeric, DBCampo::DBNotNull, "Pvp");
    addDBCampo("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, "Codigo Articulo");
    addDBCampo("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, "Nombre 	Articulo");
    addDBCampo("nomalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, "Nombre 	Articulo");
    addDBCampo("codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, "Nombre 	Articulo");
    addDBCampo("nomtarifa", DBCampo::DBvarchar, DBCampo::DBNoSave, "Nombre 	Articulo");
}// end definetabla


LTarifa::LTarifa(company *comp) : DBRecord(comp) {
    companyact = comp;
    definetabla();
}


LTarifa::LTarifa(company *comp, QString idltarifa) : DBRecord(comp) {
    companyact = comp;
    definetabla();
    QString SQLQuery = "SELECT * FROM ltarifa ";
	    SQLQuery+= "LEFT JOIN tarifa ON ltarifa.idtarifa=tarifa.idtarifa ";
	    SQLQuery+= "LEFT JOIN articulo ON articulo.idarticulo = ltarifa.idarticulo ";
	    SQLQuery+= "LEFT JOIN almacen ON almacen.idalmacen = ltarifa.idalmacen ";
	    SQLQuery+= "WHERE idltarifa="+idltarifa;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        DBload(cur);
    } else {
        vaciar();
    }// end if
}// end LTarifa


LTarifa::LTarifa(company *comp, cursor2 *cur) : DBRecord(comp) {
    companyact = comp;
    definetabla();
    DBload(cur);
}


LTarifa::~LTarifa() {}



void LTarifa::setcodigocompletoarticulo(QString val) {
    _depura("setcodigocompletoarticulo()\n", 0);
    setDBvalue("codigocompletoarticulo",val);
    QString SQLQuery = "SELECT nomarticulo, idarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE codigocompletoarticulo='"+val+"'";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        setDBvalue("nomarticulo",cur->valor("nomarticulo"));
        setDBvalue("desclpresupuesto",cur->valor("nomarticulo"));
        setDBvalue("idarticulo", cur->valor("idarticulo"));
        setDBvalue("pvplpresupuesto",cur->valor("pvp"));
        setDBvalue("ivalpresupuesto", cur->valor("iva"));
        if (DBvalue("cantlpresupuesto") == "") {
            setDBvalue("cantlpresupuesto" , "1");
            setDBvalue("descuentolpresupuesto" , "0");
        }// end if
    }// end if
    delete cur;
}// end setcodigocompletoarticulo


void LTarifa::setidarticulo(QString val) {
    _depura("setidarticulo()\n", 0);
    setDBvalue("idarticulo",val);
    QString SQLQuery = "SELECT nomarticulo, codigocompletoarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE idarticulo="+val+"";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        setDBvalue("nomarticulo" ,cur->valor("nomarticulo"));
        setDBvalue("desclpresupuesto",cur->valor("nomarticulo"));
        setDBvalue("codigocompletoarticulo",cur->valor("codigocompletoarticulo"));
        setDBvalue("pvplpresupuesto", cur->valor("pvp"));
        setDBvalue("ivalpresupuesto", cur->valor("iva"));
        if (DBvalue("cantlpresupuesto") == "") {
            setDBvalue("cantlpresupuesto", "1");
            setDBvalue("descuentolpresupuesto", "0");
        }// end if
    }// end if
    delete cur;
    _depura("end setidarticulo\n",0);
}// end setidarticulo



