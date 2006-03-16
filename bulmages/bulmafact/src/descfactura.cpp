//
// C++ Implementation: DescuentoFactura
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "descfactura.h"

void DescuentoFactura::definetabla() {

    setDBTableName("dfactura");
    setDBCampoId("iddfactura");
    addDBCampo("iddfactura", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Linea Presupuesto");
    addDBCampo("conceptdfactura", DBCampo::DBvarchar, DBCampo::DBNothing, "Descripcion");
    addDBCampo("idfactura", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("proporciondfactura", DBCampo::DBnumeric, DBCampo::DBNotNull, "Proporcion");
}// end definetabla

DescuentoFactura::DescuentoFactura(company *comp) : DBRecord(comp) {
    companyact = comp;
    definetabla();
}

DescuentoFactura::DescuentoFactura(company *comp, QString id) : DBRecord(comp){
    companyact = comp;
	definetabla();
    QString SQLQuery = "SELECT * FROM dfactura WHERE  iddfactura="+id;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        DBload(cur);
    } else {
        vaciaDescuentoFactura();
    }// end if
}// end DescuentoFactura


DescuentoFactura::DescuentoFactura(company *comp, QString a, QString b, QString c, QString d) : DBRecord(comp){
    companyact = comp;
	definetabla();
    setDBvalue("iddfactura", a);
    setDBvalue("conceptdfactura", b);
    setDBvalue("proporciondfactura", c);
    setDBvalue("idfactura",d);
}// end DescuentoFactura


DescuentoFactura::~DescuentoFactura() {}


void DescuentoFactura::vaciaDescuentoFactura() {
    DBclear();
}



void DescuentoFactura::guardaDescuentoFactura() {
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return;
    }// end if
    setDBvalue("iddfactura",id);
    companyact->commit();
}// end guardaDescuentoFactura


