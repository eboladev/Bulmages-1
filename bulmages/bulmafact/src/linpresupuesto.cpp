//
// C++ Implementation: linpresupuesto
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "linpresupuesto.h"

linpresupuesto::linpresupuesto(company *comp) {
    companyact = comp;
    vacialinpresupuesto();
}

linpresupuesto::linpresupuesto(company *comp, QString idlinpresupuesto) {
    companyact = comp;
    QString SQLQuery = "SELECT * FROM lpresupuesto, articulo WHERE lpresupuesto.idarticulo=articulo.idarticulo AND idlpresupuesto="+idlinpresupuesto;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_idlpresupuesto = cur->valor("idlpresupuesto");
        mdb_desclpresupuesto = cur->valor("desclpresupuesto");
        mdb_cantlpresupuesto = cur->valor("cantlpresupuesto");
        mdb_pvplpresupuesto = cur->valor("pvplpresupuesto");
        mdb_descuentolpresupuesto = cur->valor("descuentolpresupuesto");
        mdb_idpresupuesto = cur->valor("idpresupuesto");
        mdb_idarticulo = cur->valor("idarticulo");
        mdb_codigocompletoarticulo = cur->valor("codigocompletoarticulo");
        mdb_nomarticulo = cur->valor("nomarticulo");
        mdb_ivalpresupuesto = cur->valor("ivalpresupuesto");
    } else {
        vacialinpresupuesto();
    }// end if
}// end linpresupuesto


linpresupuesto::linpresupuesto(company *comp, QString a, QString b, QString c, QString d, QString e, QString f, QString g, QString h, QString i, QString j) {
    companyact = comp;
    mdb_idlpresupuesto = a;
    mdb_desclpresupuesto = b;
    mdb_cantlpresupuesto = c;
    mdb_pvplpresupuesto =d;
    mdb_descuentolpresupuesto = e;
    mdb_idpresupuesto = f;
    mdb_idarticulo = g;
    mdb_codigocompletoarticulo = h;
    mdb_nomarticulo = i;
    mdb_ivalpresupuesto = j;
}// end linpresupuesto


linpresupuesto::~linpresupuesto() {}


void linpresupuesto::vacialinpresupuesto() {
    mdb_idlpresupuesto = "";
    mdb_desclpresupuesto = "";
    mdb_cantlpresupuesto = "";
    mdb_pvplpresupuesto ="";
    mdb_descuentolpresupuesto = "";
    mdb_idpresupuesto = "";
    mdb_idarticulo = "";
    mdb_codigocompletoarticulo = "";
    mdb_nomarticulo = "";
    mdb_ivalpresupuesto="";
}


void linpresupuesto::borrar() {
    if (mdb_idlpresupuesto != "") {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM lpresupuesto WHERE idlpresupuesto="+mdb_idlpresupuesto);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
        vacialinpresupuesto();
    }// end if
}// end delete

void linpresupuesto::guardalinpresupuesto() {
    /// Segun est�la linea en la base de datos o no se hace una cosa u otra.
    if (mdb_idlpresupuesto == "") {
        QString SQLQuery = "INSERT INTO lpresupuesto (desclpresupuesto, cantlpresupuesto, pvplpresupuesto, descuentolpresupuesto, idpresupuesto, idarticulo, ivalpresupuesto) VALUES ('"+
	companyact->sanearCadena(mdb_desclpresupuesto)+"',"+
	companyact->sanearCadena(mdb_cantlpresupuesto)+","+
	companyact->sanearCadena(mdb_pvplpresupuesto)+","+
	companyact->sanearCadena(mdb_descuentolpresupuesto)+","+
	companyact->sanearCadena(mdb_idpresupuesto)+","+
	companyact->sanearCadena(mdb_idarticulo)+", "+
	companyact->sanearCadena(mdb_ivalpresupuesto)+")";
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idlpresupuesto) AS m FROM lpresupuesto ");
        if(!cur->eof())
            mdb_idlpresupuesto = cur->valor("m");
        delete cur;
        companyact->commit();
    } else {
        QString SQLQuery = "UPDATE lpresupuesto SET ";
        SQLQuery += " desclpresupuesto = '"+companyact->sanearCadena(mdb_desclpresupuesto)+"' ";
        SQLQuery += " ,cantlpresupuesto = "+companyact->sanearCadena(mdb_cantlpresupuesto)+" ";
        SQLQuery += " ,pvplpresupuesto = "+companyact->sanearCadena(mdb_pvplpresupuesto)+" ";
        SQLQuery += " ,descuentolpresupuesto = "+companyact->sanearCadena(mdb_descuentolpresupuesto)+" ";
        SQLQuery += " ,idpresupuesto = "+companyact->sanearCadena(mdb_idpresupuesto)+" ";
        SQLQuery += " ,idarticulo = "+companyact->sanearCadena(mdb_idarticulo)+" ";
        SQLQuery += " ,ivalpresupuesto = "+companyact->sanearCadena(mdb_ivalpresupuesto)+" ";
        SQLQuery += " WHERE idlpresupuesto = "+companyact->sanearCadena(mdb_idlpresupuesto);
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
}// end guardalinpresupuesto


void linpresupuesto::setcodigocompletoarticulo(QString val) {
    fprintf(stderr,"setcodigocompletoarticulo(%s)\n", val.ascii());
    mdb_codigocompletoarticulo=val;
    QString SQLQuery = "SELECT nomarticulo, idarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE codigocompletoarticulo='"+mdb_codigocompletoarticulo+"'";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomarticulo=cur->valor("nomarticulo");
        mdb_desclpresupuesto = mdb_nomarticulo;
        mdb_idarticulo= cur->valor("idarticulo");
        mdb_pvplpresupuesto = cur->valor("pvp");
        mdb_ivalpresupuesto = cur->valor("iva");
        if (mdb_cantlpresupuesto == "") {
            mdb_cantlpresupuesto = "1";
            mdb_descuentolpresupuesto = "0";
        }// end if
    }// end if
    delete cur;
}// end setcodigocompletoarticulo


void linpresupuesto::setidarticulo(QString val) {
    fprintf(stderr,"setidarticulo(%s)\n", val.ascii());
    mdb_idarticulo=val;
    QString SQLQuery = "SELECT nomarticulo, codigocompletoarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE idarticulo="+mdb_idarticulo+"";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomarticulo=cur->valor("nomarticulo");
        mdb_desclpresupuesto = mdb_nomarticulo;
        mdb_codigocompletoarticulo= cur->valor("codigocompletoarticulo");
        mdb_pvplpresupuesto = cur->valor("pvp");
        mdb_ivalpresupuesto = cur->valor("iva");
        if (mdb_cantlpresupuesto == "") {
            mdb_cantlpresupuesto = "1";
            mdb_descuentolpresupuesto = "0";
        }// end if
    }// end if
    delete cur;
    fprintf(stderr,"end setidarticulo\n");
}// end setidarticulo

float linpresupuesto::calculabase() {
    fprintf(stderr,"calculabase()\n");
    float cant=0;
    if (mdb_cantlpresupuesto != "" && mdb_pvplpresupuesto != "" && mdb_desclpresupuesto != "") {
        cant = mdb_cantlpresupuesto.toFloat() * mdb_pvplpresupuesto.toFloat();
        cant = cant - (cant* mdb_desclpresupuesto.toFloat());
    }// end if
    return cant;
}// end calculabase

float linpresupuesto::calculaiva() {
    fprintf(stderr,"calculaiva()\n");
    float cant=0;
    if (mdb_cantlpresupuesto != "" && mdb_pvplpresupuesto != "" && mdb_desclpresupuesto != "" && mdb_ivalpresupuesto != "") {
        cant = mdb_cantlpresupuesto.toFloat() * mdb_pvplpresupuesto.toFloat();
        cant = cant - (cant* mdb_desclpresupuesto.toFloat()/100);
        cant = cant * mdb_ivalpresupuesto.toFloat()/100;
    }// end if
    return cant;
}// end calculabase

