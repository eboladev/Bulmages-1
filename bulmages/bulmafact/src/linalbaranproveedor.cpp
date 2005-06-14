//
// C++ Implementation: LinAlbaranProveedor
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "linalbaranproveedor.h"

LinAlbaranProveedor::LinAlbaranProveedor(company *comp) {
    companyact = comp;
    vaciaLinAlbaranProveedor();
}

LinAlbaranProveedor::LinAlbaranProveedor(company *comp, QString numlalbaranp) {
    companyact = comp;
    QString SQLQuery = "SELECT * FROM lalbaranpp LEFT JOIN articulo ON lalbaranp.idarticulo=articulo.idarticulo WHERE numlalbaranp="+numlalbaranp;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_numlalbaranp = cur->valor("numlalbaranp");
        mdb_desclalbaranp = cur->valor("desclalbaranp");
        mdb_cantlalbaranp = cur->valor("cantlalbaranp");
        mdb_pvplalbaranp = cur->valor("pvplalbaranp");
        mdb_descontlalbaranp = cur->valor("desconlalbaranp");
        mdb_idarticulo = cur->valor("idarticulo");
        mdb_codigocompletoarticulo = cur->valor("codigocompletoarticulo");
        mdb_nomarticulo = cur->valor("nomarticulo");
	mdb_idalbaranp = cur->valor("idalbaranp");
    } else {
        vaciaLinAlbaranProveedor();
    }// end if
}// end LinAlbaranProveedor


LinAlbaranProveedor::LinAlbaranProveedor(company *comp, QString numlalbaranp, QString desclalbaranp, QString cantlalbaranp, QString pvplalbaranp, QString descontlalbaranp, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString idalbaranp, QString ivalalbaranp) {
    companyact = comp;
    mdb_numlalbaranp = numlalbaranp;
    mdb_desclalbaranp = desclalbaranp;
    mdb_cantlalbaranp = cantlalbaranp;
    mdb_pvplalbaranp =pvplalbaranp;
    mdb_descontlalbaranp = descontlalbaranp;
    mdb_idarticulo = idarticulo;
    mdb_codigocompletoarticulo = codigocompletoarticulo;
    mdb_nomarticulo = nomarticulo;
    mdb_idalbaranp = idalbaranp;
    mdb_ivalalbaranp = ivalalbaranp;
}// end LinAlbaranProveedor


LinAlbaranProveedor::~LinAlbaranProveedor() {}


void LinAlbaranProveedor::vaciaLinAlbaranProveedor() {
    mdb_numlalbaranp = "";
    mdb_desclalbaranp = "";
    mdb_cantlalbaranp = "";
    mdb_pvplalbaranp ="";
    mdb_descontlalbaranp = "";
    mdb_idarticulo = "";
    mdb_codigocompletoarticulo = "";
    mdb_nomarticulo = "";
    mdb_idalbaranp = "";
    mdb_ivalalbaranp = "";
}


void LinAlbaranProveedor::borrar() {
    if (mdb_numlalbaranp != "") {
        companyact->begin();
        companyact->ejecuta("DELETE FROM lalbaranp WHERE numlalbaranp="+mdb_numlalbaranp);
        companyact->commit();
        vaciaLinAlbaranProveedor();
    }// end if
}// end delete

void LinAlbaranProveedor::guardaLinAlbaranProveedor() {
    /// Segun esté la linea en la base de datos o no se hace una cosa u otra.
    if (mdb_numlalbaranp == "") {
        QString SQLQuery = "INSERT INTO lalbaranp (ivalalbaranp, desclalbaranp, cantlalbaranp, pvplalbaranp, descontlalbaranp, idalbaranp, idarticulo) VALUES ("+mdb_ivalalbaranp+",'"+mdb_desclalbaranp+"',"+mdb_cantlalbaranp+","+mdb_pvplalbaranp+","+mdb_descontlalbaranp+","+mdb_idalbaranp+","+mdb_idarticulo+")";
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(numlalbaranp) AS m FROM lalbaranp ");
        if(!cur->eof())
            mdb_numlalbaranp = cur->valor("m");
        delete cur;
        companyact->commit();
    } else {
        QString SQLQuery = "UPDATE lalbaranp SET ";
        SQLQuery += " desclalbaranp = '"+mdb_desclalbaranp+"' ";
        SQLQuery += " ,cantlalbaranp = "+mdb_cantlalbaranp+" ";
        SQLQuery += " ,pvplalbaranp = "+mdb_pvplalbaranp+" ";
        SQLQuery += " ,descontlalbaranp = "+mdb_descontlalbaranp+" ";
        SQLQuery += " ,idarticulo = "+mdb_idarticulo+" ";
	SQLQuery += " ,ivalalbaranp = "+mdb_ivalalbaranp+" ";
        SQLQuery += " WHERE numlalbaranp = "+mdb_numlalbaranp;
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        companyact->commit();
    }// end if
}// end guardaLinAlbaranProveedor


void LinAlbaranProveedor::setcodigocompletoarticulo(QString val) {
    fprintf(stderr,"setcodigocompletoarticulo(%s)\n", val.ascii());
    mdb_codigocompletoarticulo=val;
    QString SQLQuery = "SELECT nomarticulo, idarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE codigocompletoarticulo='"+mdb_codigocompletoarticulo+"'";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomarticulo=cur->valor("nomarticulo");
        mdb_desclalbaranp = mdb_nomarticulo;
        mdb_idarticulo= cur->valor("idarticulo");
        mdb_pvplalbaranp = cur->valor("pvp");
	mdb_ivalalbaranp = cur->valor("iva");
        if (mdb_cantlalbaranp == "") {
            mdb_cantlalbaranp = "1";
            mdb_descontlalbaranp = "0";
	    mdb_ivalalbaranp = "0";
        }// end if
    }// end if
    delete cur;
}// end setcodigocompletoarticulo


void LinAlbaranProveedor::setidarticulo(QString val) {
    fprintf(stderr,"setidarticulo(%s)\n", val.ascii());
    mdb_idarticulo=val;
    QString SQLQuery = "SELECT nomarticulo, codigocompletoarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE idarticulo="+mdb_idarticulo+"";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomarticulo=cur->valor("nomarticulo");
        mdb_desclalbaranp = mdb_nomarticulo;
        mdb_codigocompletoarticulo= cur->valor("codigocompletoarticulo");
        mdb_pvplalbaranp = cur->valor("pvp");
	mdb_ivalalbaranp = cur->valor("iva");
        if (mdb_cantlalbaranp == "") {
            mdb_cantlalbaranp = "1";
            mdb_descontlalbaranp = "0";
	    mdb_ivalalbaranp = "0";
        }// end if
    }// end if
    delete cur;
    fprintf(stderr,"end setidarticulo\n");
}// end setidarticulo

float LinAlbaranProveedor::calculabase() {
    fprintf(stderr,"calculabase()\n");
    float cant=0;
    if (mdb_cantlalbaranp != "" && mdb_pvplalbaranp != "" && mdb_descontlalbaranp != "") {
        cant = mdb_cantlalbaranp.toFloat() * mdb_pvplalbaranp.toFloat();
        cant = cant - (cant* mdb_descontlalbaranp.toFloat());
    }// end if
    return cant;
}// end calculabase

float LinAlbaranProveedor::calculaiva() {
    fprintf(stderr,"calculaiva()\n");
    float cant=0;

    if (mdb_cantlalbaranp != "" && mdb_pvplalbaranp != "" && mdb_desclalbaranp != "" && mdb_ivalalbaranp != "") {
        cant = mdb_cantlalbaranp.toFloat() * mdb_pvplalbaranp.toFloat();
        cant = cant - (cant* mdb_descontlalbaranp.toFloat()/100);
        cant = cant * mdb_ivalalbaranp.toFloat()/100;
    }// end if
    return cant;
}// end calculabase

