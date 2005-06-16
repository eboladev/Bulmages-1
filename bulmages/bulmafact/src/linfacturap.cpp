//
// C++ Implementation: LinFacturaProveedor
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "linfacturap.h"

LinFacturaProveedor::LinFacturaProveedor(company *comp) {
    companyact = comp;
    vaciaLinFacturaProveedor();
}

LinFacturaProveedor::LinFacturaProveedor(company *comp, QString idLinFacturaProveedor) {
    companyact = comp;
    QString SQLQuery = "SELECT * FROM lfacturapp, articulo WHERE lfacturap.idarticulo=articulo.idarticulo AND idlfacturap="+idLinFacturaProveedor;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_idlfacturap = cur->valor("idlfacturap");
        mdb_desclfacturap = cur->valor("desclfacturap");
        mdb_cantlfacturap = cur->valor("cantlfacturap");
        mdb_pvplfacturap = cur->valor("pvplfacturap");
        mdb_descuentolfacturap = cur->valor("descuentolfacturap");
        mdb_idfacturap = cur->valor("idfacturap");
        mdb_idarticulo = cur->valor("idarticulo");
        mdb_codigocompletoarticulo = cur->valor("codigocompletoarticulo");
        mdb_nomarticulo = cur->valor("nomarticulo");
        mdb_ivalfacturap = cur->valor("ivalfacturap");
    } else {
        vaciaLinFacturaProveedor();
    }// end if
}// end LinFacturaProveedor


LinFacturaProveedor::LinFacturaProveedor(company *comp, QString idlfacturap, QString desclfacturap, QString cantlfacturap, QString pvplfacturap, QString descuentolfacturap, QString idfacturap, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString ivalfacturap) {
    companyact = comp;
    mdb_idlfacturap = idlfacturap;
    mdb_desclfacturap = desclfacturap;
    mdb_cantlfacturap = cantlfacturap;
    mdb_pvplfacturap =pvplfacturap;
    mdb_descuentolfacturap = descuentolfacturap;
    mdb_idfacturap = idfacturap;
    mdb_idarticulo = idarticulo;
    mdb_codigocompletoarticulo = codigocompletoarticulo;
    mdb_nomarticulo = nomarticulo;
    mdb_ivalfacturap = ivalfacturap;
}// end LinFacturaProveedor


LinFacturaProveedor::~LinFacturaProveedor() {}


void LinFacturaProveedor::vaciaLinFacturaProveedor() {
    mdb_idlfacturap = "";
    mdb_desclfacturap = "";
    mdb_cantlfacturap = "";
    mdb_pvplfacturap ="";
    mdb_descuentolfacturap = "";
    mdb_idfacturap = "";
    mdb_idarticulo = "";
    mdb_codigocompletoarticulo = "";
    mdb_nomarticulo = "";
    mdb_ivalfacturap="";
}


void LinFacturaProveedor::borrar() {
    if (mdb_idlfacturap != "") {
        companyact->begin();
        companyact->ejecuta("DELETE FROM lfacturap WHERE idlfacturap="+mdb_idlfacturap);
        companyact->commit();
        vaciaLinFacturaProveedor();
    }// end if
}// end delete

void LinFacturaProveedor::guardaLinFacturaProveedor() {
    /// Segun esté la linea en la base de datos o no se hace una cosa u otra.
    if (mdb_idlfacturap == "") {
        QString SQLQuery = "INSERT INTO lfacturap (desclfacturap, cantlfacturap, pvplfacturap, descuentolfacturap, idfacturap, idarticulo, ivalfacturap) VALUES ('"+mdb_desclfacturap+"',"+mdb_cantlfacturap+","+mdb_pvplfacturap+","+mdb_descuentolfacturap+","+mdb_idfacturap+","+mdb_idarticulo+", "+mdb_ivalfacturap+")";
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idlFactura) AS m FROM lFactura ");
        if(!cur->eof())
            mdb_idlfacturap = cur->valor("m");
        delete cur;
        companyact->commit();
    } else {
        QString SQLQuery = "UPDATE lFactura SET ";
        SQLQuery += " desclfacturap = '"+mdb_desclfacturap+"' ";
        SQLQuery += " ,cantlfacturap = "+mdb_cantlfacturap+" ";
        SQLQuery += " ,pvplfacturap = "+mdb_pvplfacturap+" ";
        SQLQuery += " ,descuentolfacturap = "+mdb_descuentolfacturap+" ";
        SQLQuery += " ,idfacturap = "+mdb_idfacturap+" ";
        SQLQuery += " ,idarticulo = "+mdb_idarticulo+" ";
        SQLQuery += " ,ivalfacturap = "+mdb_ivalfacturap+" ";
        SQLQuery += " WHERE idlfacturap = "+mdb_idlfacturap;
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        companyact->commit();
    }// end if
}// end guardaLinFacturaProveedor


void LinFacturaProveedor::setcodigocompletoarticulo(QString val) {
    fprintf(stderr,"setcodigocompletoarticulo(%s)\n", val.ascii());
    mdb_codigocompletoarticulo=val;
    QString SQLQuery = "SELECT nomarticulo, idarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE codigocompletoarticulo='"+mdb_codigocompletoarticulo+"'";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomarticulo=cur->valor("nomarticulo");
        mdb_desclfacturap = mdb_nomarticulo;
        mdb_idarticulo= cur->valor("idarticulo");
        mdb_pvplfacturap = cur->valor("pvp");
        mdb_ivalfacturap = cur->valor("iva");
        if (mdb_cantlfacturap == "") {
            mdb_cantlfacturap = "1";
            mdb_descuentolfacturap = "0";
        }// end if
    }// end if
    delete cur;
}// end setcodigocompletoarticulo


void LinFacturaProveedor::setidarticulo(QString val) {
    fprintf(stderr,"setidarticulo(%s)\n", val.ascii());
    mdb_idarticulo=val;
    QString SQLQuery = "SELECT nomarticulo, codigocompletoarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE idarticulo="+mdb_idarticulo+"";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomarticulo=cur->valor("nomarticulo");
        mdb_desclfacturap = mdb_nomarticulo;
        mdb_codigocompletoarticulo= cur->valor("codigocompletoarticulo");
        mdb_pvplfacturap = cur->valor("pvp");
        mdb_ivalfacturap = cur->valor("iva");
        if (mdb_cantlfacturap == "") {
            mdb_cantlfacturap = "1";
            mdb_descuentolfacturap = "0";
        }// end if
    }// end if
    delete cur;
    fprintf(stderr,"end setidarticulo\n");
}// end setidarticulo

float LinFacturaProveedor::calculabase() {
    fprintf(stderr,"calculabase()\n");
    float cant=0;
    if (mdb_cantlfacturap != "" && mdb_pvplfacturap != "" && mdb_desclfacturap != "") {
        cant = mdb_cantlfacturap.toFloat() * mdb_pvplfacturap.toFloat();
        cant = cant - (cant* mdb_desclfacturap.toFloat());
    }// end if
    return cant;
}// end calculabase

float LinFacturaProveedor::calculaiva() {
    fprintf(stderr,"calculaiva()\n");
    float cant=0;
    if (mdb_cantlfacturap != "" && mdb_pvplfacturap != "" && mdb_desclfacturap != "" && mdb_ivalfacturap != "") {
        cant = mdb_cantlfacturap.toFloat() * mdb_pvplfacturap.toFloat();
        cant = cant - (cant* mdb_desclfacturap.toFloat()/100);
        cant = cant * mdb_ivalfacturap.toFloat()/100;
    }// end if
    return cant;
}// end calculabase

