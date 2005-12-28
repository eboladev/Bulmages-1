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

DescuentoFacturaProv::DescuentoFacturaProv(company *comp) {
    companyact = comp;
    vaciaDescuentoFacturaProv();
}

DescuentoFacturaProv::DescuentoFacturaProv(company *comp, QString id) {
    companyact = comp;
    QString SQLQuery = "SELECT * FROM dfacturap WHERE  iddfacturap="+id;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_iddfacturap = cur->valor("iddfacturap");
        mdb_conceptdfacturap = cur->valor("conceptdfacturap");
        mdb_proporciondfacturap = cur->valor("proporciondfacturap");
        mdb_idfacturap = cur->valor("idfacturap");
    } else {
        vaciaDescuentoFacturaProv();
    }// end if
}// end DescuentoFacturaProv


DescuentoFacturaProv::DescuentoFacturaProv(company *comp, QString a, QString b, QString c, QString d) {
    companyact = comp;
    mdb_iddfacturap = a;
    mdb_conceptdfacturap = b;
    mdb_proporciondfacturap = c;
    mdb_idfacturap =d;
}// end DescuentoFacturaProv


DescuentoFacturaProv::~DescuentoFacturaProv() {}


void DescuentoFacturaProv::vaciaDescuentoFacturaProv() {
    mdb_iddfacturap = "";
    mdb_conceptdfacturap = "";
    mdb_proporciondfacturap = "";
    mdb_idfacturap ="";
}


void DescuentoFacturaProv::borrar() {
    if (mdb_iddfacturap != "") {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM dfacturap WHERE iddfacturap="+mdb_iddfacturap);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
        vaciaDescuentoFacturaProv();
    }// end if
}// end delete

void DescuentoFacturaProv::guardaDescuentoFacturaProv() {
    /// Segun est�la linea en la base de datos o no se hace una cosa u otra.
    if (mdb_iddfacturap == "") {
        QString SQLQuery = "INSERT INTO dfacturap (conceptdfacturap, proporciondfacturap, idfacturap) VALUES ('"+
	companyact->sanearCadena(mdb_conceptdfacturap)+"',"+
	companyact->sanearCadena(mdb_proporciondfacturap)+","+
	companyact->sanearCadena(mdb_idfacturap)+")";
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        cursor2 *cur = companyact->cargacursor("SELECT MAX(iddfacturap) AS m FROM dfacturap ");
        if(!cur->eof())
            mdb_iddfacturap = cur->valor("m");
        delete cur;
        companyact->commit();
    } else {
        QString SQLQuery = "UPDATE dfacturap SET ";
        SQLQuery += " conceptdfacturap = '"+companyact->sanearCadena(mdb_conceptdfacturap)+"' ";
        SQLQuery += " ,proporciondfacturap = "+companyact->sanearCadena(mdb_proporciondfacturap)+" ";
        SQLQuery += " ,idfacturap = "+companyact->sanearCadena(mdb_idfacturap)+" ";
        SQLQuery += " WHERE iddfacturap = "+companyact->sanearCadena(mdb_iddfacturap);
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
}// end guardaDescuentoFacturaProv


