//
// C++ Implementation: DescuentoAlbaranCliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "descalbarancliente.h"



void DescuentoAlbaranCliente::definetabla() {

    setDBTableName("dalbaran");
    setDBCampoId("iddalbaran");
    addDBCampo("iddalbaran", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Linea Presupuesto");
    addDBCampo("conceptdalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, "Descripcion");
    addDBCampo("idalbaran", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("proporciondalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, "Proporcion");

}// end definetabla

DescuentoAlbaranCliente::DescuentoAlbaranCliente(company *comp) : DBRecord(comp) {
    companyact = comp;
    definetabla();
}

DescuentoAlbaranCliente::DescuentoAlbaranCliente(company *comp, QString id) : DBRecord(comp) {
    companyact = comp;
    definetabla();
    QString SQLQuery = "SELECT * FROM dalbaran WHERE  iddalbaran="+id;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        DBload(cur);
    } else {
        vaciaDescuentoAlbaranCliente();
    }// end if
}// end DescuentoAlbaranCliente


DescuentoAlbaranCliente::DescuentoAlbaranCliente(company *comp, QString a, QString b, QString c, QString d) : DBRecord(comp) {
    companyact = comp;
    definetabla();
    setDBvalue("iddalbaran", a);
    setDBvalue("conceptdalbaran", b);
    setDBvalue("proporciondalbaran", c);
    setDBvalue("idalbaran",d);
}// end DescuentoAlbaranCliente


DescuentoAlbaranCliente::~DescuentoAlbaranCliente() {}


void DescuentoAlbaranCliente::vaciaDescuentoAlbaranCliente() {
    DBclear();
}




void DescuentoAlbaranCliente::guardaDescuentoAlbaranCliente() {
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return;
    }// end if
    setDBvalue("iddalbaran",id);
    companyact->commit();
}// end guardaDescuentoAlbaranCliente


