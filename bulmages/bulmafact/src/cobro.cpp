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
#include <QFile>
#include <QTextStream>



Cobro::Cobro(company *comp) : DBRecord(comp) {
    companyact=comp;

    setDBTableName("cobro");
    setDBCampoId("idcobro");
    addDBCampo("idcobro", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Presupuesto");
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("previsioncobro", DBCampo::DBboolean, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("fechacobro", DBCampo::DBdate, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("refcobro", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("cantcobro", DBCampo::DBnumeric, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("comentcobro", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
}

Cobro::~Cobro() {}


void Cobro::borraCobro() {
    if (DBvalue("idcobro") != "") {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM cobro WHERE idcobro="+DBvalue("idcobro"));
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
    DBclear();
}// end vaciaCobro

void Cobro::pintaCobro() {
    pintaidcobro(DBvalue("idcobro"));
    pintaidcliente(DBvalue("idcliente"));
    pintafechacobro(DBvalue("fechacobro"));
    pintacantcobro(DBvalue("cantcobro"));
    pintarefcobro(DBvalue("refcobro"));
    pintaprevisioncobro(DBvalue("previsioncobro"));
    pintacomentcobro(DBvalue("comentcobro"));
}// end pintaCobro


// Esta funci� carga un Cobro.
void Cobro::cargaCobro(QString idbudget) {
    QString query = "SELECT * FROM cobro WHERE idCobro="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    }// end if
    delete cur;
    pintaCobro();
}// end chargeBudget


void Cobro::guardaCobro() {
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return;
    }// end if
    setidcobro(id);
    companyact->commit();
    cargaCobro(id);
}// end guardaCobro


