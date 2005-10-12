//
// C++ Implementation: LinPedidoProveedor
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "linpedidoproveedor.h"

LinPedidoProveedor::LinPedidoProveedor(company *comp) {
    companyact = comp;
    vaciaLinPedidoProveedor();
}

LinPedidoProveedor::LinPedidoProveedor(company *comp, QString idLinPedidoProveedor) {
    companyact = comp;
    QString SQLQuery = "SELECT * FROM lpedidoproveedor, articulo WHERE lpedidoproveedor.idarticulo=articulo.idarticulo AND lpedidoproveedor="+idLinPedidoProveedor;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {	
     mdb_numlpedidoproveedor = cur->valor("numlpedidoproveedor");
     mdb_desclpedidoproveedor = cur->valor("desclpedidoproveedor");
     mdb_cantlpedidoproveedor = cur->valor("cantlpedidoproveedor");
     mdb_pvplpedidoproveedor = cur->valor("pvplpedidoproveedor");
     mdb_prevlpedidoproveedor = cur->valor("prevlpedidoproveedor");
     mdb_ivalpedidoproveedor = cur->valor("ivalpedidoproveedor");
     mdb_descuentolpedidoproveedor = cur->valor("descuentolpedidoproveedor");
     mdb_idpedidoproveedor = cur->valor("idpedidoproveedor");
     mdb_idarticulo = cur->valor("idarticulo");
     mdb_codigocompletoarticulo = cur->valor("codigocompletoarticulo");
     mdb_nomarticulo = cur->valor("nomarticulo");
    } else {
        vaciaLinPedidoProveedor();
    }// end if
}// end LinPedidoProveedor


LinPedidoProveedor::LinPedidoProveedor(company *comp, QString numlpedidoproveedor, QString desclpedidoproveedor, QString cantlpedidoproveedor, QString pvplpedidoproveedor, QString prevlpedidoproveedor, QString ivalpedidoproveedor, QString descuentolpedidoproveedor, QString idpedidoproveedor, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo) {
    companyact = comp;
     mdb_numlpedidoproveedor = numlpedidoproveedor;
     mdb_desclpedidoproveedor = desclpedidoproveedor;
     mdb_cantlpedidoproveedor = cantlpedidoproveedor;
     mdb_pvplpedidoproveedor = pvplpedidoproveedor;
     mdb_prevlpedidoproveedor = prevlpedidoproveedor;
     mdb_ivalpedidoproveedor = ivalpedidoproveedor;
     mdb_descuentolpedidoproveedor = descuentolpedidoproveedor;
     mdb_idpedidoproveedor = idpedidoproveedor;
     mdb_idarticulo = idarticulo;
     mdb_codigocompletoarticulo = codigocompletoarticulo;
     mdb_nomarticulo = nomarticulo;
}// end LinPedidoProveedor


LinPedidoProveedor::~LinPedidoProveedor() {}


void LinPedidoProveedor::vaciaLinPedidoProveedor() {
     mdb_numlpedidoproveedor = "";
     mdb_desclpedidoproveedor = "";
     mdb_cantlpedidoproveedor = "";
     mdb_pvplpedidoproveedor = "";
     mdb_prevlpedidoproveedor = "";
     mdb_ivalpedidoproveedor = "";
     mdb_descuentolpedidoproveedor = "";
     mdb_idpedidoproveedor = "";
     mdb_idarticulo = "";
     mdb_codigocompletoarticulo = "";
     mdb_nomarticulo = "";
}


void LinPedidoProveedor::borrar() {
    if (mdb_idpedidoproveedor != "") {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM lpedidoproveedor WHERE numlpedidoproveedor="+mdb_numlpedidoproveedor);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
        vaciaLinPedidoProveedor();
    }// end if
}// end delete


void LinPedidoProveedor::guardaLinPedidoProveedor() {
    QString prevlpedidoproveedor;
    if (mdb_prevlpedidoproveedor == "") {
    	prevlpedidoproveedor = "NULL";
    } else {
    	prevlpedidoproveedor = "'"+mdb_prevlpedidoproveedor+"'";
    }// end if
    /// Segun est�la linea en la base de datos o no se hace una cosa u otra.
    if (mdb_numlpedidoproveedor == "") {
        QString SQLQuery = "INSERT INTO lpedidoproveedor (desclpedidoproveedor, cantlpedidoproveedor, pvplpedidoproveedor, prevlpedidoproveedor, ivalpedidoproveedor, descuentolpedidoproveedor, idpedidoproveedor, idarticulo) VALUES ('"+
	companyact->sanearCadena(mdb_desclpedidoproveedor)+"',"+
	companyact->sanearCadena(mdb_cantlpedidoproveedor)+","+
	companyact->sanearCadena(mdb_pvplpedidoproveedor)+","+
	companyact->sanearCadena(prevlpedidoproveedor)+","+
	companyact->sanearCadena(mdb_ivalpedidoproveedor)+","+
	companyact->sanearCadena(mdb_descuentolpedidoproveedor)+", "+
	companyact->sanearCadena(mdb_idpedidoproveedor)+","+
	companyact->sanearCadena(mdb_idarticulo)+")";
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        cursor2 *cur = companyact->cargacursor("SELECT MAX(numlpedidoproveedor) AS m FROM lpedidoproveedor ");
        if(!cur->eof())
            mdb_numlpedidoproveedor = cur->valor("m");
        delete cur;
        companyact->commit();
    } else {
        QString SQLQuery = "UPDATE lpedidoproveedor SET ";
        SQLQuery += " desclpedidoproveedor = '"+companyact->sanearCadena(mdb_desclpedidoproveedor)+"' ";
        SQLQuery += " ,cantlpedidoproveedor = "+companyact->sanearCadena(mdb_cantlpedidoproveedor)+" ";
        SQLQuery += " ,pvplpedidoproveedor = "+companyact->sanearCadena(mdb_pvplpedidoproveedor)+" ";
        SQLQuery += " ,prevlpedidoproveedor = "+companyact->sanearCadena(prevlpedidoproveedor);
        SQLQuery += " ,ivalpedidoproveedor = "+companyact->sanearCadena(mdb_ivalpedidoproveedor)+" ";
        SQLQuery += " ,descuentolpedidoproveedor = "+companyact->sanearCadena(mdb_descuentolpedidoproveedor)+" ";
        SQLQuery += " ,idpedidoproveedor = "+companyact->sanearCadena(mdb_idpedidoproveedor)+" ";
        SQLQuery += " ,idarticulo = "+companyact->sanearCadena(mdb_idarticulo)+" ";
        SQLQuery += " WHERE numlpedidoproveedor = "+companyact->sanearCadena(mdb_numlpedidoproveedor);
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
}// end guardaLinPedidoProveedor


void LinPedidoProveedor::setcodigocompletoarticulo(QString val) {
    fprintf(stderr,"setcodigocompletoarticulo(%s)\n", val.ascii());
    mdb_codigocompletoarticulo=val;
    QString SQLQuery = "SELECT nomarticulo, idarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE codigocompletoarticulo='"+mdb_codigocompletoarticulo+"'";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomarticulo=cur->valor("nomarticulo");
        mdb_desclpedidoproveedor = mdb_nomarticulo;
        mdb_idarticulo= cur->valor("idarticulo");
        mdb_pvplpedidoproveedor = cur->valor("pvp");
        mdb_ivalpedidoproveedor = cur->valor("iva");
        if (mdb_cantlpedidoproveedor == "") {
            mdb_cantlpedidoproveedor = "1";
            mdb_descuentolpedidoproveedor = "0";
        }// end if
    }// end if
    delete cur;
}// end setcodigocompletoarticulo


void LinPedidoProveedor::setidarticulo(QString val) {
    fprintf(stderr,"setidarticulo(%s)\n", val.ascii());
    mdb_idarticulo=val;
    QString SQLQuery = "SELECT nomarticulo, codigocompletoarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE idarticulo="+mdb_idarticulo+"";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomarticulo=cur->valor("nomarticulo");
        mdb_desclpedidoproveedor = mdb_nomarticulo;
        mdb_codigocompletoarticulo= cur->valor("codigocompletoarticulo");
        mdb_pvplpedidoproveedor = cur->valor("pvp");
        mdb_ivalpedidoproveedor = cur->valor("iva");
        if (mdb_cantlpedidoproveedor == "") {
            mdb_cantlpedidoproveedor = "1";
            mdb_descuentolpedidoproveedor = "0";
        }// end if
    }// end if
    delete cur;
    fprintf(stderr,"end setidarticulo\n");
}// end setidarticulo

float LinPedidoProveedor::calculabase() {
    fprintf(stderr,"calculabase()\n");
    float cant=0;
    if (mdb_cantlpedidoproveedor != "" && mdb_pvplpedidoproveedor != "" && mdb_descuentolpedidoproveedor != "") {
        cant = mdb_cantlpedidoproveedor.toFloat() * mdb_pvplpedidoproveedor.toFloat();
        cant = cant - (cant* mdb_descuentolpedidoproveedor.toFloat());
    }// end if
    return cant;
}// end calculabase

float LinPedidoProveedor::calculaiva() {
    fprintf(stderr,"calculaiva()\n");
    float cant=0;
    if (mdb_cantlpedidoproveedor != "" && mdb_pvplpedidoproveedor != "" && mdb_descuentolpedidoproveedor != "" && mdb_ivalpedidoproveedor != "") {
        cant = mdb_cantlpedidoproveedor.toFloat() * mdb_pvplpedidoproveedor.toFloat();
        cant = cant - (cant* mdb_descuentolpedidoproveedor.toFloat()/100);
        cant = cant * mdb_ivalpedidoproveedor.toFloat()/100;
    }// end if
    return cant;
}// end calculabase

