//
// C++ Implementation: LinFactura
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "linfactura.h"

LinFactura::LinFactura(company *comp) {
    companyact = comp;
    vaciaLinFactura();
}

LinFactura::LinFactura(company *comp, QString idLinFactura) {
    companyact = comp;
    QString SQLQuery = "SELECT * FROM lfactura, articulo WHERE lfactura.idarticulo=articulo.idarticulo AND idlfactura="+idLinFactura;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_idlfactura = cur->valor("idlfactura");
        mdb_desclfactura = cur->valor("desclfactura");
        mdb_cantlfactura = cur->valor("cantlfactura");
        mdb_pvplfactura = cur->valor("pvplfactura");
        mdb_descuentolfactura = cur->valor("descuentolfactura");
        mdb_idfactura = cur->valor("idfactura");
        mdb_idarticulo = cur->valor("idarticulo");
        mdb_codigocompletoarticulo = cur->valor("codigocompletoarticulo");
        mdb_nomarticulo = cur->valor("nomarticulo");
        mdb_ivalfactura = cur->valor("ivalfactura");
    } else {
        vaciaLinFactura();
    }// end if
}// end LinFactura


LinFactura::LinFactura(company *comp, QString idlfactura, QString desclfactura, QString cantlfactura, QString pvplfactura, QString descuentolfactura, QString idfactura, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString ivalfactura) {
    companyact = comp;
    mdb_idlfactura = idlfactura;
    mdb_desclfactura = desclfactura;
    mdb_cantlfactura = cantlfactura;
    mdb_pvplfactura =pvplfactura;
    mdb_descuentolfactura = descuentolfactura;
    mdb_idfactura = idfactura;
    mdb_idarticulo = idarticulo;
    mdb_codigocompletoarticulo = codigocompletoarticulo;
    mdb_nomarticulo = nomarticulo;
    mdb_ivalfactura = ivalfactura;
}// end LinFactura


LinFactura::~LinFactura() {}


void LinFactura::vaciaLinFactura() {
    mdb_idlfactura = "";
    mdb_desclfactura = "";
    mdb_cantlfactura = "";
    mdb_pvplfactura ="";
    mdb_descuentolfactura = "";
    mdb_idfactura = "";
    mdb_idarticulo = "";
    mdb_codigocompletoarticulo = "";
    mdb_nomarticulo = "";
    mdb_ivalfactura="";
}


void LinFactura::borrar() {
    if (mdb_idlfactura != "") {
        companyact->begin();
        companyact->ejecuta("DELETE FROM lfactura WHERE idlfactura="+mdb_idlfactura);
        companyact->commit();
        vaciaLinFactura();
    }// end if
}// end delete

void LinFactura::guardaLinFactura() {
    /// Segun est�la linea en la base de datos o no se hace una cosa u otra.
    if (mdb_idlfactura == "") {
        QString SQLQuery = "INSERT INTO lFactura (desclFactura, cantlFactura, pvplFactura, descuentolFactura, idFactura, idarticulo, ivalFactura) VALUES ('"+
	companyact->sanearCadena(mdb_desclfactura)+"',"+
	companyact->sanearCadena(mdb_cantlfactura)+","+
	companyact->sanearCadena(mdb_pvplfactura)+","+
	companyact->sanearCadena(mdb_descuentolfactura)+","+
	companyact->sanearCadena(mdb_idfactura)+","+
	companyact->sanearCadena(mdb_idarticulo)+", "+
	companyact->sanearCadena(mdb_ivalfactura)+")";
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idlFactura) AS m FROM lFactura ");
        if(!cur->eof())
            mdb_idlfactura = cur->valor("m");
        delete cur;
        companyact->commit();
    } else {
        QString SQLQuery = "UPDATE lFactura SET ";
        SQLQuery += " desclfactura = '"+companyact->sanearCadena(mdb_desclfactura)+"' ";
        SQLQuery += " ,cantlfactura = "+companyact->sanearCadena(mdb_cantlfactura)+" ";
        SQLQuery += " ,pvplfactura = "+companyact->sanearCadena(mdb_pvplfactura)+" ";
        SQLQuery += " ,descuentolfactura = "+companyact->sanearCadena(mdb_descuentolfactura)+" ";
        SQLQuery += " ,idfactura = "+companyact->sanearCadena(mdb_idfactura)+" ";
        SQLQuery += " ,idarticulo = "+companyact->sanearCadena(mdb_idarticulo)+" ";
        SQLQuery += " ,ivalfactura = "+companyact->sanearCadena(mdb_ivalfactura)+" ";
        SQLQuery += " WHERE idlfactura = "+companyact->sanearCadena(mdb_idlfactura);
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        companyact->commit();
    }// end if
}// end guardaLinFactura


void LinFactura::setcodigocompletoarticulo(QString val) {
    fprintf(stderr,"setcodigocompletoarticulo(%s)\n", val.ascii());
    mdb_codigocompletoarticulo=val;
    QString SQLQuery = "SELECT nomarticulo, idarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE codigocompletoarticulo='"+mdb_codigocompletoarticulo+"'";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomarticulo=cur->valor("nomarticulo");
        mdb_desclfactura = mdb_nomarticulo;
        mdb_idarticulo= cur->valor("idarticulo");
        mdb_pvplfactura = cur->valor("pvp");
        mdb_ivalfactura = cur->valor("iva");
        if (mdb_cantlfactura == "") {
            mdb_cantlfactura = "1";
            mdb_descuentolfactura = "0";
        }// end if
    }// end if
    delete cur;
}// end setcodigocompletoarticulo


void LinFactura::setidarticulo(QString val) {
    fprintf(stderr,"setidarticulo(%s)\n", val.ascii());
    mdb_idarticulo=val;
    QString SQLQuery = "SELECT nomarticulo, codigocompletoarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE idarticulo="+mdb_idarticulo+"";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomarticulo=cur->valor("nomarticulo");
        mdb_desclfactura = mdb_nomarticulo;
        mdb_codigocompletoarticulo= cur->valor("codigocompletoarticulo");
        mdb_pvplfactura = cur->valor("pvp");
        mdb_ivalfactura = cur->valor("iva");
        if (mdb_cantlfactura == "") {
            mdb_cantlfactura = "1";
            mdb_descuentolfactura = "0";
        }// end if
    }// end if
    delete cur;
    fprintf(stderr,"end setidarticulo\n");
}// end setidarticulo

float LinFactura::calculabase() {
    fprintf(stderr,"calculabase()\n");
    float cant=0;
    if (mdb_cantlfactura != "" && mdb_pvplfactura != "" && mdb_desclfactura != "") {
        cant = mdb_cantlfactura.toFloat() * mdb_pvplfactura.toFloat();
        cant = cant - (cant* mdb_desclfactura.toFloat());
    }// end if
    return cant;
}// end calculabase

float LinFactura::calculaiva() {
    fprintf(stderr,"calculaiva()\n");
    float cant=0;
    if (mdb_cantlfactura != "" && mdb_pvplfactura != "" && mdb_desclfactura != "" && mdb_ivalfactura != "") {
        cant = mdb_cantlfactura.toFloat() * mdb_pvplfactura.toFloat();
        cant = cant - (cant* mdb_desclfactura.toFloat()/100);
        cant = cant * mdb_ivalfactura.toFloat()/100;
    }// end if
    return cant;
}// end calculabase

