//
// C++ Implementation: CompArticulo
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "comparticulo.h"

CompArticulo::CompArticulo(company *comp) {
    companyact = comp;
    vaciaCompArticulo();
}

CompArticulo::CompArticulo(company *comp, QString idarticulo, QString idcomponente) {
    companyact = comp;
    QString SQLQuery = "SELECT * FROM comparticulo, articulo WHERE comparticulo.idcomponente=articulo.idarticulo AND idarticulo="+idarticulo+" AND idcomponente="+idcomponente;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_idarticulo = cur->valor("idarticulo");
        mdb_idcomponente = cur->valor("idcomponente");
        mdb_cantcomparticulo = cur->valor("cantcomparticulo");
        mdb_codigocompletocomponente = cur->valor("codigocompletoarticulo");
        mdb_nomcomponente = cur->valor("nomarticulo");
    } else {
        vaciaCompArticulo();
    }// end if
}// end CompArticulo


CompArticulo::CompArticulo(company *comp, QString a, QString b, QString c, QString d, QString e) {
    companyact = comp;
    mdb_idarticulo = a;
    mdb_idcomponente = b;
    mdb_cantcomparticulo = c;
    mdb_codigocompletocomponente =d;
    mdb_nomcomponente = e;
}// end CompArticulo


CompArticulo::~CompArticulo() {}


void CompArticulo::vaciaCompArticulo() {
    mdb_idarticulo = "";
    mdb_idcomponente = "";
    mdb_cantcomparticulo = "";
    mdb_codigocompletocomponente ="";
    mdb_nomcomponente = "";
}


void CompArticulo::borrar() {
    if (mdb_idarticulo != "" && mdb_idcomponente != "") {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM comparticulo WHERE idarticulo="+mdb_idarticulo+" AND idcomponente="+mdb_idcomponente);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
        vaciaCompArticulo();
    }// end if
}// end delete

void CompArticulo::guardaCompArticulo() {
    /// Segun est�la linea en la base de datos o no se hace una cosa u otra.
    if (mdb_idarticulo != "" && mdb_idcomponente != "") {

        QString SQLQuery = "INSERT INTO comparticulo (idarticulo, idcomponente, cantcomparticulo) VALUES ("+
	companyact->sanearCadena(mdb_idarticulo)+","+
	companyact->sanearCadena(mdb_idcomponente)+","+
	companyact->sanearCadena(mdb_cantcomparticulo)+")";
        companyact->begin();
          int error = companyact->ejecuta("DELETE FROM comparticulo WHERE idarticulo="+mdb_idarticulo+" AND idcomponente="+mdb_idcomponente);
	  if (error) {
		companyact->rollback();
		return;
		}// end if
	  error = companyact->ejecuta(SQLQuery);
	 if (error) {
		companyact->rollback();
		return;
	 }// end if
        companyact->commit();
    }
}// end guardaCompArticulo


void CompArticulo::setcodigocompletocomponente(QString val) {
    fprintf(stderr,"setcodigocompletoarticulo(%s)\n", val.ascii());
    mdb_codigocompletocomponente=val;
    QString SQLQuery = "SELECT nomarticulo, idarticulo FROM articulo WHERE codigocompletoarticulo='"+mdb_codigocompletocomponente+"'";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomcomponente=cur->valor("nomarticulo");
        mdb_idcomponente= cur->valor("idarticulo");
    }// end if
    delete cur;
}// end setcodigocompletoarticulo


void CompArticulo::setidcomponente(QString val) {
    fprintf(stderr,"setidcomponente(%s)\n", val.ascii());
    mdb_idcomponente=val;
    QString SQLQuery = "SELECT nomarticulo, codigocompletoarticulo FROM articulo WHERE idarticulo="+mdb_idcomponente+"";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomcomponente=cur->valor("nomarticulo");
        mdb_codigocompletocomponente = cur->valor("codigocompletoarticulo");
    }// end if
    delete cur;
}// end setidarticulo
