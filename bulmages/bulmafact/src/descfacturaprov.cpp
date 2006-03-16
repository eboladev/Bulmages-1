//
// C++ Implementation: DescuentoFacturaProv
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "descfacturaprov.h"
void DescuentoFacturaProv::definetabla() {

    setDBTableName("dfacturap");
    setDBCampoId("iddfacturap");
    addDBCampo("iddfacturap", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Linea Presupuesto");
    addDBCampo("conceptdfacturap", DBCampo::DBvarchar, DBCampo::DBNothing, "Descripcion");
    addDBCampo("idfacturap", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("proporciondfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, "Proporcion");
}// end definetabla


DescuentoFacturaProv::DescuentoFacturaProv(company *comp) : DBRecord(comp) {
    companyact = comp;
    definetabla();
}

DescuentoFacturaProv::DescuentoFacturaProv(company *comp, QString id) : DBRecord(comp) {
    companyact = comp;
    definetabla();
    QString SQLQuery = "SELECT * FROM dfacturap WHERE  iddfacturap="+id;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        DBload(cur);
    } else {
        vaciaDescuentoFacturaProv();
    }// end if
}// end DescuentoFacturaProv


DescuentoFacturaProv::DescuentoFacturaProv(company *comp, QString a, QString b, QString c, QString d) : DBRecord(comp) {
    companyact = comp;
    definetabla();
    setDBvalue("iddfacturap", a);
    setDBvalue("conceptdfacturap", b);
    setDBvalue("proporciondfacturap", c);
    setDBvalue("idfacturap",d);
}// end DescuentoFacturaProv


DescuentoFacturaProv::~DescuentoFacturaProv() {}


void DescuentoFacturaProv::vaciaDescuentoFacturaProv() {
    DBclear();
}



void DescuentoFacturaProv::guardaDescuentoFacturaProv() {
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return;
    }// end if
    setDBvalue("iddfacturap",id);
    companyact->commit();
}// end guardaDescuentoFacturaProv


