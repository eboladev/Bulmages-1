//
// C++ Implementation: Cobro
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "cobro.h"
#include "company.h"
#include "configuracion.h"
#include <qfile.h>
#include <qtextstream.h>



Cobro::Cobro(company *comp) {
    companyact=comp;
    vaciaCobro();
}

Cobro::~Cobro() {}


void Cobro::borraCobro() {
    if (mdb_idcobro != "") {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM cobro WHERE idcobro="+mdb_idcobro);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
        vaciaCobro();
        pintaCobro();
    }// end if
}// end borraCobro


void Cobro::vaciaCobro() {
    mdb_idcobro = "";
    mdb_idcliente = "";
    mdb_fechacobro = "";
    mdb_cantcobro = "";
    mdb_refcobro = "";
    mdb_previsioncobro = "FALSE";
    mdb_comentcobro = "";
}// end vaciaCobro

void Cobro::pintaCobro() {
    pintaidcobro(mdb_idcobro);
    pintaidcliente(mdb_idcliente);
    pintafechacobro(mdb_fechacobro);
    pintacantcobro(mdb_cantcobro);
    pintarefcobro(mdb_refcobro);
    pintaprevisioncobro(mdb_previsioncobro);
    pintacomentcobro(mdb_comentcobro);
}// end pintaCobro


// Esta funci� carga un Cobro.
void Cobro::cargaCobro(QString idbudget) {
    mdb_idcobro = idbudget;
    QString query = "SELECT * FROM cobro WHERE idCobro="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        mdb_idcliente= cur->valor("idcliente");
        mdb_fechacobro= cur->valor("fechacobro");
        mdb_cantcobro= cur->valor("cantcobro");
        mdb_refcobro= cur->valor("refcobro");
        mdb_comentcobro= cur->valor("comentcobro");
        if (cur->valor("previsioncobro") == "t")
            mdb_previsioncobro = "TRUE";
        else
            mdb_previsioncobro = "FALSE";
    }// end if
    delete cur;
    fprintf(stderr,"Vamos a cargar las lineas\n");
    pintaCobro();
}// end chargeBudget


void Cobro::guardaCobro() {
    companyact->begin();
    if (mdb_idcobro == "") {
        /// Se trata de una inserci�
        QString SQLQuery = "INSERT INTO cobro (idcliente, fechacobro, cantcobro, refcobro, comentcobro, previsioncobro) VALUES ("+
	companyact->sanearCadena(mdb_idcliente)+",'"+
	companyact->sanearCadena(mdb_fechacobro)+"',"+
	companyact->sanearCadena(mdb_cantcobro)+",'"+
	companyact->sanearCadena(mdb_refcobro)+"','"+
	companyact->sanearCadena(mdb_comentcobro)+"',"+
	companyact->sanearCadena(mdb_previsioncobro)+")";
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	 }// end if
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idcobro) AS m FROM cobro");
        if (!cur->eof())
            setidcobro(cur->valor("m"));
        delete cur;
        companyact->commit();
    } else {
        /// Se trata de una modificaci�
        QString SQLQuery = "UPDATE cobro SET ";
        SQLQuery += " idcliente="+companyact->sanearCadena(mdb_idcliente);
        SQLQuery += " ,fechacobro='"+companyact->sanearCadena(mdb_fechacobro)+"'";
        SQLQuery += " ,cantcobro='"+companyact->sanearCadena(mdb_cantcobro)+"'";
        SQLQuery += " ,refcobro='"+companyact->sanearCadena(mdb_refcobro)+"'";
        SQLQuery += " ,comentcobro='"+companyact->sanearCadena(mdb_comentcobro)+"'";
        SQLQuery += " ,previsioncobro='"+companyact->sanearCadena(mdb_previsioncobro)+"'";
        SQLQuery += " WHERE idcobro="+companyact->sanearCadena(mdb_idcobro);
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
    cargaCobro(mdb_idcobro);
}// end guardaCobro


