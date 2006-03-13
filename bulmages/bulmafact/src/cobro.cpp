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




void Cobro::pintar() {
    pintaidcobro(DBvalue("idcobro"));
    pintaidcliente(DBvalue("idcliente"));
    pintafechacobro(DBvalue("fechacobro"));
    pintacantcobro(DBvalue("cantcobro"));
    pintarefcobro(DBvalue("refcobro"));
    pintaprevisioncobro(DBvalue("previsioncobro"));
    pintacomentcobro(DBvalue("comentcobro"));
}// end pintaCobro






