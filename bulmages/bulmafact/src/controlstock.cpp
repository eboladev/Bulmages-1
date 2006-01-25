//
// C++ Implementation: ControlStock
// Description: Implementacion
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
// Copyright: See COPYING file that comes with this distribution
//
//
#include "controlstock.h"
#include "funcaux.h"

ControlStock::ControlStock(company *comp) {
    companyact = comp;
    vaciaControlStock();
}

ControlStock::ControlStock(company *comp, QString idarticulo, QString idalmacen, QString idinventario) {
    companyact = comp;
    QString SQLQuery = "SELECT * FROM controlstock, articulo, almacen WHERE controlstock.idarticulo=articulo.idarticulo AND controlstock.idalmacen=almacen.idalmacen AND controlstock.idarticulo="+idarticulo+" AND controlstock.idalmacen="+idalmacen+" AND controlstock.idinventario="+idinventario;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_idinventariooriginal = cur->valor("idinventario");
        mdb_idalmacenoriginal = cur->valor("idalmacen");
        mdb_idarticulooriginal = cur->valor("idarticulo");

        mdb_idinventario = cur->valor("idinventario");
        mdb_idalmacen = cur->valor("idalmacen");
        mdb_idarticulo = cur->valor("idarticulo");
        mdb_stockantcontrolstock = cur->valor("stockantcontrolstock");
        mdb_stocknewcontrolstock = cur->valor("stocknewcontrolstock");
        mdb_punteocontrolstock = cur->valor("punteocontrolstock");
        mdb_codigocompletoarticulo = cur->valor("codigocompletoarticulo");
        mdb_nomarticulo = cur->valor("nomarticulo");
        mdb_nomalmacen = cur->valor("nomalmacen");
        mdb_codigoalmacen = cur->valor("codigoalmacen");
    } else {
        vaciaControlStock();
    }// end if
}// end ControlStock


ControlStock::ControlStock(company *comp, QString idinventario, QString idalmacen, QString idarticulo, QString stockantcontrolstock, QString stocknewcontrolstock, QString punteocontrolstock, QString codigocompletoarticulo, QString nomarticulo, QString nomalmacen, QString codigoalmacen) {
    companyact = comp;
    mdb_idinventariooriginal = idinventario;
    mdb_idalmacenoriginal = idalmacen;
    mdb_idarticulooriginal = idarticulo;
    mdb_idinventario = idinventario;
    mdb_idalmacen = idalmacen;
    mdb_idarticulo = idarticulo;
    mdb_stockantcontrolstock = stockantcontrolstock;
    mdb_stocknewcontrolstock = stocknewcontrolstock;
    mdb_punteocontrolstock = punteocontrolstock;
    mdb_codigocompletoarticulo = codigocompletoarticulo;
    mdb_nomarticulo = nomarticulo;
    mdb_nomalmacen = nomalmacen;
    mdb_codigoalmacen = codigoalmacen;
}// end ControlStock


ControlStock::~ControlStock() {}


void ControlStock::vaciaControlStock() {
    mdb_idinventariooriginal = "";
    mdb_idalmacenoriginal = "";
    mdb_idarticulooriginal = "";
    mdb_idinventario = "";
    mdb_idalmacen = "";
    mdb_idarticulo = "";
    mdb_stockantcontrolstock = "";
    mdb_stocknewcontrolstock = "";
    mdb_punteocontrolstock = "f";
    mdb_codigocompletoarticulo = "";
    mdb_nomarticulo = "";
    mdb_nomalmacen = "";
    mdb_codigoalmacen = "";
}


void ControlStock::borrar() {
    _depura("ControlStock::borrar",0);
    if ((mdb_idinventario == "") || (mdb_idarticulo == "") || (mdb_idalmacen == ""))
        return;
    companyact->begin();
    QString query = "DELETE FROM controlstock WHERE idarticulo="+mdb_idarticulooriginal+" AND idinventario ="+mdb_idinventariooriginal+" AND idalmacen="+mdb_idalmacenoriginal;
    int error = companyact->ejecuta(query);
    if (error) {
        companyact->rollback();
        return;
    }// end if
    companyact->commit();
    vaciaControlStock();
}// end delete


void ControlStock::guardaControlStock() {
    QString SQLQuery;

/// Como se ejecuta dentro de una transaccion mayor no lo ponemos en una 
/// Transaccion.
//    companyact->begin();
    if (mdb_idinventariooriginal == "") {
        SQLQuery = "INSERT INTO controlstock (idinventario , idalmacen , idarticulo , stocknewcontrolstock , punteocontrolstock) VALUES ("+
                           companyact->sanearCadena(mdb_idinventario)+","+
                           companyact->sanearCadena(mdb_idalmacen)+","+
                           companyact->sanearCadena(mdb_idarticulo)+","+
                           companyact->sanearCadena(mdb_stocknewcontrolstock)+","+
                           "'"+companyact->sanearCadena(mdb_punteocontrolstock)+"')";
    } else {
        SQLQuery = "UPDATE controlstock SET ";
        SQLQuery += " stocknewcontrolstock="+mdb_stocknewcontrolstock;
        SQLQuery += " ,punteocontrolstock='"+mdb_punteocontrolstock+"'";
        SQLQuery += " ,idarticulo="+mdb_idarticulo;
        SQLQuery += " ,idalmacen="+mdb_idalmacen;
        SQLQuery += " ,idinventario="+mdb_idinventario;
        SQLQuery += " WHERE idinventario="+mdb_idinventariooriginal;
        SQLQuery += " AND idarticulo="+mdb_idarticulooriginal;
        SQLQuery += " AND idalmacen="+mdb_idalmacenoriginal;
    }// end if

    int error = companyact->ejecuta(SQLQuery);
    if (error) {
//        companyact->rollback();
        return;
    }// end if
 //   companyact->commit();

}// end guardaControlStock


void ControlStock::setcodigocompletoarticulo(QString val) {
    fprintf(stderr,"setcodigocompletoarticulo(%s)\n", val.ascii());
    mdb_codigocompletoarticulo=val;
    QString SQLQuery = "SELECT nomarticulo, idarticulo FROM articulo WHERE codigocompletoarticulo='"+mdb_codigocompletoarticulo+"'";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomarticulo=cur->valor("nomarticulo");
        mdb_idarticulo= cur->valor("idarticulo");
    }// end if
    delete cur;
}// end setcodigocompletoarticulo


void ControlStock::setidarticulo(QString val) {
    _depura("setidarticulo(%s)\n", 0);
    mdb_idarticulo=val;
    QString SQLQuery = "SELECT nomarticulo, codigocompletoarticulo FROM articulo WHERE idarticulo="+mdb_idarticulo+"";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomarticulo=cur->valor("nomarticulo");
        mdb_codigocompletoarticulo= cur->valor("codigocompletoarticulo");
    }// end if
    delete cur;
    _depura("end setidarticulo\n",0);
}// end setidarticulo

void ControlStock::setcodigoalmacen(QString val) {
    _depura("setcodigoalmacen()\n", 0);
    mdb_codigoalmacen=val;
    QString SQLQuery = "SELECT nomalmacen, idalmacen FROM almacen WHERE codigoalmacen='"+mdb_codigoalmacen+"'";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomalmacen=cur->valor("nomalmacen");
        mdb_idalmacen= cur->valor("idalmacen");
    }// end if
    delete cur;
}// end setcodigocompletoarticulo
