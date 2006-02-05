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

void LinAlbaranCliente::definetabla() {

    setDBTableName("lalbaran");
    setDBCampoId("numlalbaran");
    addDBCampo("numlalbaran", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Linea Presupuesto");
    addDBCampo("idalbaran", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("desclalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, "Descripcion");
    addDBCampo("cantlalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, "Cantidad");
    addDBCampo("pvplalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, "Precio Linea Presupuesto");
    addDBCampo("descontlalbaran", DBCampo::DBnumeric, DBCampo::DBNothing, "Descuento Linea Presupuesto");
    addDBCampo("idarticulo", DBCampo::DBint, DBCampo::DBNothing, "Articulo Presupuesto");
    addDBCampo("ivalalbaran", DBCampo::DBnumeric, DBCampo::DBNothing, "IVA linea Presupuesto");
    addDBCampo("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, "Codigo Articulo");
    addDBCampo("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, "Nombre 	Articulo");
}// end definetabla

LinAlbaranCliente::LinAlbaranCliente(company *comp) : DBRecord(comp) {
    companyact = comp;
    definetabla();
}

LinAlbaranCliente::LinAlbaranCliente(company *comp, QString numlalbaran) : DBRecord(comp) {
    companyact = comp;
	definetabla();
    QString SQLQuery = "SELECT * FROM lalbaran, articulo WHERE lalbaran.idarticulo=articulo.idarticulo AND numlalbaran="+numlalbaran;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        DBload(cur);
    } else {
        vaciaLinAlbaranCliente();
    }// end if
}// end LinAlbaranCliente


LinAlbaranCliente::LinAlbaranCliente(company *comp, QString numlalbaran, QString desclalbaran, QString cantlalbaran, QString pvplalbaran, QString descontlalbaran, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString idalbaran, QString iva) : DBRecord(comp) {
    companyact = comp;
	definetabla();
    setDBvalue("numlalbaran", numlalbaran);
    setDBvalue("desclalbaran", desclalbaran);
    setDBvalue("cantlalbaran", cantlalbaran);
    setDBvalue("pvplalbaran",pvplalbaran);
    setDBvalue("descontlalbaran", descontlalbaran);
    setDBvalue("idarticulo", idarticulo);
    setDBvalue("codigocompletoarticulo", codigocompletoarticulo);
    setDBvalue("nomarticulo", nomarticulo);
    setDBvalue("idalbaran", idalbaran);
    setDBvalue("ivalalbaran", iva);
}// end LinAlbaranCliente


LinAlbaranCliente::~LinAlbaranCliente() {}


void LinAlbaranCliente::vaciaLinAlbaranCliente() {
    DBclear();
}


void LinAlbaranCliente::borrar() {
    if (DBvalue("numlalbaran") != "") {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM lalbaran WHERE numlalbaran="+DBvalue("numlalbaran"));
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
        vaciaLinAlbaranCliente();
    }// end if
}// end delete

void LinAlbaranCliente::guardaLinAlbaranCliente() {
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return;
    }// end if
    setDBvalue("numlalbaran",id);
    companyact->commit();
}// end guardaLinAlbaranCliente


void LinAlbaranCliente::setcodigocompletoarticulo(QString val) {
    _depura("setcodigocompletoarticulo()\n",0);
    setDBvalue("codigocompletoarticulo",val);
    QString SQLQuery = "SELECT nomarticulo, idarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE codigocompletoarticulo='"+DBvalue("codigocompletoarticulo")+"'";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        setDBvalue("nomarticulo",cur->valor("nomarticulo"));
        setDBvalue("desclalbaran", DBvalue("nomarticulo"));
        setDBvalue("idarticulo", cur->valor("idarticulo"));
        setDBvalue("pvplalbaran", cur->valor("pvp"));
	setDBvalue("ivalalbaran", cur->valor("iva"));
        if (DBvalue("cantlalbaran") == "") {
            setDBvalue("cantlalbaran", "1");
            setDBvalue("descontlalbaran", "0");
        }// end if
    }// end if
    delete cur;
}// end setcodigocompletoarticulo


void LinAlbaranCliente::setidarticulo(QString val) {
    _depura("setidarticulo()\n", 0);
    setDBvalue("idarticulo",val);
    QString SQLQuery = "SELECT nomarticulo, codigocompletoarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE idarticulo="+DBvalue("idarticulo")+"";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        setDBvalue("nomarticulo",cur->valor("nomarticulo"));
        setDBvalue("desclalbaran", DBvalue("nomarticulo"));
        setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
        setDBvalue("pvplalbaran", cur->valor("pvp"));
	setDBvalue("ivalalbaran", cur->valor("iva"));
        if (DBvalue("cantlalbaran") == "") {
            setDBvalue("cantlalbaran", "1");
            setDBvalue("descontlalbaran", "0");
        }// end if
    }// end if
    delete cur;
    _depura("end setidarticulo\n",0);
}// end setidarticulo

