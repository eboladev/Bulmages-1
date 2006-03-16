//
// C++ Implementation: DescuentoAlbaranProv
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "descalbaranprov.h"

void DescuentoAlbaranProv::definetabla() {

    setDBTableName("dalbaranp");
    setDBCampoId("iddalbaranp");
    addDBCampo("iddalbaranp", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Linea Presupuesto");
    addDBCampo("conceptdalbaranp", DBCampo::DBvarchar, DBCampo::DBNothing, "Descripcion");
    addDBCampo("idalbaranp", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("proporciondalbaranp", DBCampo::DBnumeric, DBCampo::DBNotNull, "Proporcion");

}// end definetabla


DescuentoAlbaranProv::DescuentoAlbaranProv(company *comp) : DBRecord(comp) {
    companyact = comp;
    definetabla();
}

DescuentoAlbaranProv::DescuentoAlbaranProv(company *comp, QString id) :DBRecord(comp) {
    companyact = comp;
    definetabla();
    QString SQLQuery = "SELECT * FROM dalbaranp WHERE  iddalbaranp="+id;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        DBload(cur);
    } else {
        vaciaDescuentoAlbaranProv();
    }// end if
}// end DescuentoAlbaranProv


DescuentoAlbaranProv::DescuentoAlbaranProv(company *comp, QString a, QString b, QString c, QString d) : DBRecord(comp) {
    companyact = comp;
    definetabla();
    setDBvalue("iddalbaranp", a);
    setDBvalue("conceptdalbaranp", b);
    setDBvalue("proporciondalbaranp", c);
    setDBvalue("idalbaranp",d);
}// end DescuentoAlbaranProv


DescuentoAlbaranProv::~DescuentoAlbaranProv() {}


void DescuentoAlbaranProv::vaciaDescuentoAlbaranProv() {
    DBclear();
}



void DescuentoAlbaranProv::guardaDescuentoAlbaranProv() {
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return;
    }// end if
    setDBvalue("iddalbaranp",id);
    companyact->commit();
}// end guardaDescuentoAlbaranProv


