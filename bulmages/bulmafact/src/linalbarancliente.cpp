//
// C++ Implementation: LinAlbaranCliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "linalbarancliente.h"

LinAlbaranCliente::LinAlbaranCliente(company *comp) {
    companyact = comp;
    vaciaLinAlbaranCliente();
}

LinAlbaranCliente::LinAlbaranCliente(company *comp, QString numlalbaran) {
    companyact = comp;
    QString SQLQuery = "SELECT * FROM lalbaran, articulo WHERE lalbaran.idarticulo=articulo.idarticulo AND numlalbaran="+numlalbaran;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_numlalbaran = cur->valor("numlalbaran");
        mdb_desclalbaran = cur->valor("desclalbaran");
        mdb_cantlalbaran = cur->valor("cantlalbaran");
        mdb_pvplalbaran = cur->valor("pvplalbaran");
        mdb_descontlalbaran = cur->valor("desconlalbaran");
        mdb_idarticulo = cur->valor("idarticulo");
        mdb_codigocompletoarticulo = cur->valor("codigocompletoarticulo");
        mdb_nomarticulo = cur->valor("nomarticulo");
	mdb_ivalalbaran = cur->valor("ivalalbaran");
    } else {
        vaciaLinAlbaranCliente();
    }// end if
}// end LinAlbaranCliente


LinAlbaranCliente::LinAlbaranCliente(company *comp, QString numlalbaran, QString desclalbaran, QString cantlalbaran, QString pvplalbaran, QString descontlalbaran, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString idalbaran, QString iva) {
    companyact = comp;
    mdb_numlalbaran = numlalbaran;
    mdb_desclalbaran = desclalbaran;
    mdb_cantlalbaran = cantlalbaran;
    mdb_pvplalbaran =pvplalbaran;
    mdb_descontlalbaran = descontlalbaran;
    mdb_idarticulo = idarticulo;
    mdb_codigocompletoarticulo = codigocompletoarticulo;
    mdb_nomarticulo = nomarticulo;
    mdb_idalbaran = idalbaran;
    mdb_ivalalbaran = iva;
}// end LinAlbaranCliente


LinAlbaranCliente::~LinAlbaranCliente() {}


void LinAlbaranCliente::vaciaLinAlbaranCliente() {
    mdb_numlalbaran = "";
    mdb_desclalbaran = "";
    mdb_cantlalbaran = "";
    mdb_pvplalbaran ="";
    mdb_descontlalbaran = "";
    mdb_idarticulo = "";
    mdb_codigocompletoarticulo = "";
    mdb_nomarticulo = "";
    mdb_idalbaran = "";
    mdb_ivalalbaran = "";
}


void LinAlbaranCliente::borrar() {
    if (mdb_numlalbaran != "") {
        companyact->begin();
        companyact->ejecuta("DELETE FROM lalbaran WHERE numlalbaran="+mdb_numlalbaran);
        companyact->commit();
        vaciaLinAlbaranCliente();
    }// end if
}// end delete

void LinAlbaranCliente::guardaLinAlbaranCliente() {
    /// Segun esté la linea en la base de datos o no se hace una cosa u otra.
    if (mdb_numlalbaran == "") {
        QString SQLQuery = "INSERT INTO lalbaran (desclalbaran, cantlalbaran, pvplalbaran, descontlalbaran, idalbaran, idarticulo, ivalalbaran) VALUES ('"+mdb_desclalbaran+"',"+mdb_cantlalbaran+","+mdb_pvplalbaran+","+mdb_descontlalbaran+","+mdb_idalbaran+","+mdb_idarticulo+","+mdb_ivalalbaran+")";
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(numlalbaran) AS m FROM lalbaran ");
        if(!cur->eof())
            mdb_numlalbaran = cur->valor("m");
        delete cur;
        companyact->commit();
    } else {
        QString SQLQuery = "UPDATE lalbaran SET ";
        SQLQuery += " desclalbaran = '"+mdb_desclalbaran+"' ";
        SQLQuery += " ,cantlalbaran = "+mdb_cantlalbaran+" ";
        SQLQuery += " ,pvplalbaran = "+mdb_pvplalbaran+" ";
        SQLQuery += " ,descontlalbaran = "+mdb_descontlalbaran+" ";
        SQLQuery += " ,idarticulo = "+mdb_idarticulo+" ";
	SQLQuery += " ,ivalalbaran = "+mdb_ivalalbaran+" ";
        SQLQuery += " WHERE numlalbaran = "+mdb_numlalbaran;
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        companyact->commit();
    }// end if
}// end guardaLinAlbaranCliente


void LinAlbaranCliente::setcodigocompletoarticulo(QString val) {
    fprintf(stderr,"setcodigocompletoarticulo(%s)\n", val.ascii());
    mdb_codigocompletoarticulo=val;
    QString SQLQuery = "SELECT nomarticulo, idarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE codigocompletoarticulo='"+mdb_codigocompletoarticulo+"'";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomarticulo=cur->valor("nomarticulo");
        mdb_desclalbaran = mdb_nomarticulo;
        mdb_idarticulo= cur->valor("idarticulo");
        mdb_pvplalbaran = cur->valor("pvp");
	mdb_ivalalbaran = cur->valor("iva");
        if (mdb_cantlalbaran == "") {
            mdb_cantlalbaran = "1";
            mdb_descontlalbaran = "0";
        }// end if
    }// end if
    delete cur;
}// end setcodigocompletoarticulo


void LinAlbaranCliente::setidarticulo(QString val) {
    fprintf(stderr,"setidarticulo(%s)\n", val.ascii());
    mdb_idarticulo=val;
    QString SQLQuery = "SELECT nomarticulo, codigocompletoarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE idarticulo="+mdb_idarticulo+"";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomarticulo=cur->valor("nomarticulo");
        mdb_desclalbaran = mdb_nomarticulo;
        mdb_codigocompletoarticulo= cur->valor("codigocompletoarticulo");
        mdb_pvplalbaran = cur->valor("pvp");
	mdb_ivalalbaran = cur->valor("iva");
        if (mdb_cantlalbaran == "") {
            mdb_cantlalbaran = "1";
            mdb_descontlalbaran = "0";
        }// end if
    }// end if
    delete cur;
    fprintf(stderr,"end setidarticulo\n");
}// end setidarticulo

