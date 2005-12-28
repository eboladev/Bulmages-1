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


DescuentoAlbaranProv::DescuentoAlbaranProv(company *comp) {
    companyact = comp;
    vaciaDescuentoAlbaranProv();
}

DescuentoAlbaranProv::DescuentoAlbaranProv(company *comp, QString id) {
    companyact = comp;
    QString SQLQuery = "SELECT * FROM dalbaranp WHERE  iddalbaranp="+id;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_iddalbaranp = cur->valor("iddalbaranp");
        mdb_conceptdalbaranp = cur->valor("conceptdalbaranp");
        mdb_proporciondalbaranp = cur->valor("proporciondalbaranp");
        mdb_idalbaranp = cur->valor("idalbaranp");
    } else {
        vaciaDescuentoAlbaranProv();
    }// end if
}// end DescuentoAlbaranProv


DescuentoAlbaranProv::DescuentoAlbaranProv(company *comp, QString a, QString b, QString c, QString d) {
    companyact = comp;
    mdb_iddalbaranp = a;
    mdb_conceptdalbaranp = b;
    mdb_proporciondalbaranp = c;
    mdb_idalbaranp =d;
}// end DescuentoAlbaranProv


DescuentoAlbaranProv::~DescuentoAlbaranProv() {}


void DescuentoAlbaranProv::vaciaDescuentoAlbaranProv() {
    mdb_iddalbaranp = "";
    mdb_conceptdalbaranp = "";
    mdb_proporciondalbaranp = "";
    mdb_idalbaranp ="";
}


void DescuentoAlbaranProv::borrar() {
    if (mdb_iddalbaranp != "") {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM dalbaranp WHERE iddalbaranp="+mdb_iddalbaranp);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
        vaciaDescuentoAlbaranProv();
    }// end if
}// end delete

void DescuentoAlbaranProv::guardaDescuentoAlbaranProv() {
    /// Segun est�la linea en la base de datos o no se hace una cosa u otra.
    if (mdb_iddalbaranp == "") {
        QString SQLQuery = "INSERT INTO dalbaranp (conceptdalbaranp, proporciondalbaranp, idalbaranp) VALUES ('"+
	companyact->sanearCadena(mdb_conceptdalbaranp)+"',"+
	companyact->sanearCadena(mdb_proporciondalbaranp)+","+
	companyact->sanearCadena(mdb_idalbaranp)+")";
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        cursor2 *cur = companyact->cargacursor("SELECT MAX(iddalbaranp) AS m FROM dalbaranp ");
        if(!cur->eof())
            mdb_iddalbaranp = cur->valor("m");
        delete cur;
        companyact->commit();
    } else {
        QString SQLQuery = "UPDATE dalbaranp SET ";
        SQLQuery += " conceptdalbaranp = '"+companyact->sanearCadena(mdb_conceptdalbaranp)+"' ";
        SQLQuery += " ,proporciondalbaranp = "+companyact->sanearCadena(mdb_proporciondalbaranp)+" ";
        SQLQuery += " ,idalbaranp = "+companyact->sanearCadena(mdb_idalbaranp)+" ";
        SQLQuery += " WHERE iddalbaranp = "+companyact->sanearCadena(mdb_iddalbaranp);
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
}// end guardaDescuentoAlbaranProv


