//
// C++ Implementation: LinPedidoCliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "linpedidocliente.h"
#include "funcaux.h"

LinPedidoCliente::LinPedidoCliente(company *comp) {
    companyact = comp;
    vaciaLinPedidoCliente();
}

LinPedidoCliente::LinPedidoCliente(company *comp, QString idLinPedidoCliente) {
    companyact = comp;
    QString SQLQuery = "SELECT * FROM lpedidocliente, articulo WHERE lpedidocliente.idarticulo=articulo.idarticulo AND lpedidocliente="+idLinPedidoCliente;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_numlpedidocliente = cur->valor("numlpedidocliente");
        mdb_desclpedidocliente = cur->valor("desclpedidocliente");
        mdb_cantlpedidocliente = cur->valor("cantlpedidocliente");
        mdb_pvplpedidocliente = cur->valor("pvplpedidocliente");
        mdb_prevlpedidocliente = cur->valor("prevlpedidocliente");
        mdb_ivalpedidocliente = cur->valor("ivalpedidocliente");
        mdb_descuentolpedidocliente = cur->valor("descuentolpedidocliente");
        mdb_idpedidocliente = cur->valor("idpedidocliente");
        mdb_idarticulo = cur->valor("idarticulo");
        mdb_codigocompletoarticulo = cur->valor("codigocompletoarticulo");
        mdb_nomarticulo = cur->valor("nomarticulo");
        mdb_puntlpedidocliente = cur->valor("puntlpedidocliente");
    } else {
        vaciaLinPedidoCliente();
    }// end if
}// end LinPedidoCliente


LinPedidoCliente::LinPedidoCliente(company *comp, QString numlpedidocliente, QString desclpedidocliente, QString cantlpedidocliente, QString pvplpedidocliente, QString prevlpedidocliente, QString ivalpedidocliente, QString descuentolpedidocliente, QString idpedidocliente, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString puntlpedidocliente) {
    companyact = comp;
    mdb_numlpedidocliente = numlpedidocliente;
    mdb_desclpedidocliente = desclpedidocliente;
    mdb_cantlpedidocliente = cantlpedidocliente;
    mdb_pvplpedidocliente = pvplpedidocliente;
    mdb_prevlpedidocliente = prevlpedidocliente;
    mdb_ivalpedidocliente = ivalpedidocliente;
    mdb_descuentolpedidocliente = descuentolpedidocliente;
    mdb_idpedidocliente = idpedidocliente;
    mdb_idarticulo = idarticulo;
    mdb_codigocompletoarticulo = codigocompletoarticulo;
    mdb_nomarticulo = nomarticulo;
    if (puntlpedidocliente == "TRUE" || puntlpedidocliente == "t")
        mdb_puntlpedidocliente = "TRUE";
    else
        mdb_puntlpedidocliente = "FALSE";
}// end LinPedidoCliente


LinPedidoCliente::~LinPedidoCliente() {}


void LinPedidoCliente::vaciaLinPedidoCliente() {
    mdb_numlpedidocliente = "";
    mdb_desclpedidocliente = "";
    mdb_cantlpedidocliente = "";
    mdb_pvplpedidocliente = "";
    mdb_prevlpedidocliente = "";
    mdb_ivalpedidocliente = "";
    mdb_descuentolpedidocliente = "";
    mdb_idpedidocliente = "";
    mdb_idarticulo = "";
    mdb_codigocompletoarticulo = "";
    mdb_nomarticulo = "";
    mdb_puntlpedidocliente="FALSE";
}


void LinPedidoCliente::borrar() {
    if (mdb_idpedidocliente != "") {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM lpedidocliente WHERE numlpedidocliente="+mdb_numlpedidocliente);
        if (error) {
            companyact->rollback();
            return;
        }// end if
        companyact->commit();
        vaciaLinPedidoCliente();
    }// end if
}// end delete


void LinPedidoCliente::guardaLinPedidoCliente() {
    QString prevlpedidocliente;
    if (mdb_prevlpedidocliente == "") {
        prevlpedidocliente = "NULL";
    } else {
        prevlpedidocliente = "'"+mdb_prevlpedidocliente+"'";
    }// end if
    if (mdb_puntlpedidocliente == "")
        mdb_puntlpedidocliente = "FALSE";
    /// Segun est�la linea en la base de datos o no se hace una cosa u otra.
    if (mdb_numlpedidocliente == "") {
        QString SQLQuery = "INSERT INTO lpedidocliente (desclpedidocliente, cantlpedidocliente, pvplpedidocliente, prevlpedidocliente, ivalpedidocliente, descuentolpedidocliente, idpedidocliente, idarticulo, puntlpedidocliente) VALUES ('"+
                           companyact->sanearCadena(mdb_desclpedidocliente)+"',"+
                           companyact->sanearCadena(mdb_cantlpedidocliente)+","+
                           companyact->sanearCadena(mdb_pvplpedidocliente)+","+
                           companyact->sanearCadena(prevlpedidocliente)+","+
                           companyact->sanearCadena(mdb_ivalpedidocliente)+","+
                           companyact->sanearCadena(mdb_descuentolpedidocliente)+", "+
                           companyact->sanearCadena(mdb_idpedidocliente)+","+
                           companyact->sanearCadena(mdb_idarticulo)+", "+
       			   companyact->sanearCadena(mdb_puntlpedidocliente)+")";
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
        if (error) {
            companyact->rollback();
            return;
        }// end if
        cursor2 *cur = companyact->cargacursor("SELECT MAX(numlpedidocliente) AS m FROM lpedidocliente ");
        if(!cur->eof())
            mdb_numlpedidocliente = cur->valor("m");
        delete cur;
        companyact->commit();
    } else {
        QString SQLQuery = "UPDATE lpedidocliente SET ";
        SQLQuery += " desclpedidocliente = '"+companyact->sanearCadena(mdb_desclpedidocliente)+"' ";
        SQLQuery += " ,cantlpedidocliente = "+companyact->sanearCadena(mdb_cantlpedidocliente)+" ";
        SQLQuery += " ,pvplpedidocliente = "+companyact->sanearCadena(mdb_pvplpedidocliente)+" ";
        SQLQuery += " ,prevlpedidocliente = "+companyact->sanearCadena(prevlpedidocliente);
        SQLQuery += " ,ivalpedidocliente = "+companyact->sanearCadena(mdb_ivalpedidocliente)+" ";
        SQLQuery += " ,descuentolpedidocliente = "+companyact->sanearCadena(mdb_descuentolpedidocliente)+" ";
        SQLQuery += " ,idpedidocliente = "+companyact->sanearCadena(mdb_idpedidocliente)+" ";
        SQLQuery += " ,idarticulo = "+companyact->sanearCadena(mdb_idarticulo)+" ";
        SQLQuery += " ,puntlpedidocliente = "+companyact->sanearCadena(mdb_puntlpedidocliente)+" ";
        SQLQuery += " WHERE numlpedidocliente = "+companyact->sanearCadena(mdb_numlpedidocliente);
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
        if (error) {
            companyact->rollback();
            return;
        }// end if
        companyact->commit();
    }// end if
}// end guardaLinPedidoCliente


void LinPedidoCliente::setcodigocompletoarticulo(QString val) {
    fprintf(stderr,"setcodigocompletoarticulo(%s)\n", val.ascii());
    mdb_codigocompletoarticulo=val;
    QString SQLQuery = "SELECT nomarticulo, idarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE codigocompletoarticulo='"+mdb_codigocompletoarticulo+"'";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomarticulo=cur->valor("nomarticulo");
        mdb_desclpedidocliente = mdb_nomarticulo;
        mdb_idarticulo= cur->valor("idarticulo");
        mdb_pvplpedidocliente = cur->valor("pvp");
        mdb_ivalpedidocliente = cur->valor("iva");
        if (mdb_cantlpedidocliente == "") {
            mdb_cantlpedidocliente = "1";
            mdb_descuentolpedidocliente = "0";
        }// end if
    }// end if
    delete cur;
}// end setcodigocompletoarticulo


void LinPedidoCliente::setidarticulo(QString val) {
    fprintf(stderr,"setidarticulo(%s)\n", val.ascii());
    mdb_idarticulo=val;
    QString SQLQuery = "SELECT nomarticulo, codigocompletoarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE idarticulo="+mdb_idarticulo+"";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomarticulo=cur->valor("nomarticulo");
        mdb_desclpedidocliente = mdb_nomarticulo;
        mdb_codigocompletoarticulo= cur->valor("codigocompletoarticulo");
        mdb_pvplpedidocliente = cur->valor("pvp");
        mdb_ivalpedidocliente = cur->valor("iva");
        if (mdb_cantlpedidocliente == "") {
            mdb_cantlpedidocliente = "1";
            mdb_descuentolpedidocliente = "0";
        }// end if
    }// end if
    delete cur;
    fprintf(stderr,"end setidarticulo\n");
}// end setidarticulo

float LinPedidoCliente::calculabase() {
    fprintf(stderr,"calculabase()\n");
    float cant=0;
    if (mdb_cantlpedidocliente != "" && mdb_pvplpedidocliente != "" && mdb_descuentolpedidocliente != "") {
        cant = mdb_cantlpedidocliente.toFloat() * mdb_pvplpedidocliente.toFloat();
        cant = cant - (cant* mdb_descuentolpedidocliente.toFloat());
    }// end if
    return cant;
}// end calculabase

float LinPedidoCliente::calculaiva() {
    fprintf(stderr,"calculaiva()\n");
    float cant=0;
    if (mdb_cantlpedidocliente != "" && mdb_pvplpedidocliente != "" && mdb_descuentolpedidocliente != "" && mdb_ivalpedidocliente != "") {
        cant = mdb_cantlpedidocliente.toFloat() * mdb_pvplpedidocliente.toFloat();
        cant = cant - (cant* mdb_descuentolpedidocliente.toFloat()/100);
        cant = cant * mdb_ivalpedidocliente.toFloat()/100;
    }// end if
    return cant;
}// end calculabase

