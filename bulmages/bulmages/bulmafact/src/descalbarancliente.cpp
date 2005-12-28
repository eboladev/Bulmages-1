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


DescuentoAlbaranCliente::DescuentoAlbaranCliente(company *comp) {
    companyact = comp;
    vaciaDescuentoAlbaranCliente();
}

DescuentoAlbaranCliente::DescuentoAlbaranCliente(company *comp, QString id) {
    companyact = comp;
    QString SQLQuery = "SELECT * FROM dalbaran WHERE  iddalbaran="+id;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_iddalbaran = cur->valor("iddalbaran");
        mdb_conceptdalbaran = cur->valor("conceptdalbaran");
        mdb_proporciondalbaran = cur->valor("proporciondalbaran");
        mdb_idalbaran = cur->valor("idalbaran");
    } else {
        vaciaDescuentoAlbaranCliente();
    }// end if
}// end DescuentoAlbaranCliente


DescuentoAlbaranCliente::DescuentoAlbaranCliente(company *comp, QString a, QString b, QString c, QString d) {
    companyact = comp;
    mdb_iddalbaran = a;
    mdb_conceptdalbaran = b;
    mdb_proporciondalbaran = c;
    mdb_idalbaran =d;
}// end DescuentoAlbaranCliente


DescuentoAlbaranCliente::~DescuentoAlbaranCliente() {}


void DescuentoAlbaranCliente::vaciaDescuentoAlbaranCliente() {
    mdb_iddalbaran = "";
    mdb_conceptdalbaran = "";
    mdb_proporciondalbaran = "";
    mdb_idalbaran ="";
}


void DescuentoAlbaranCliente::borrar() {
    if (mdb_iddalbaran != "") {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM dalbaran WHERE iddalbaran="+mdb_iddalbaran);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
        vaciaDescuentoAlbaranCliente();
    }// end if
}// end delete

void DescuentoAlbaranCliente::guardaDescuentoAlbaranCliente() {
    /// Segun est�la linea en la base de datos o no se hace una cosa u otra.
    if (mdb_iddalbaran == "") {
        QString SQLQuery = "INSERT INTO dalbaran (conceptdalbaran, proporciondalbaran, idalbaran) VALUES ('"+
	companyact->sanearCadena(mdb_conceptdalbaran)+"',"+
	companyact->sanearCadena(mdb_proporciondalbaran)+","+
	companyact->sanearCadena(mdb_idalbaran)+")";
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        cursor2 *cur = companyact->cargacursor("SELECT MAX(iddalbaran) AS m FROM dalbaran ");
        if(!cur->eof())
            mdb_iddalbaran = cur->valor("m");
        delete cur;
        companyact->commit();
    } else {
        QString SQLQuery = "UPDATE dalbaran SET ";
        SQLQuery += " conceptdalbaran = '"+companyact->sanearCadena(mdb_conceptdalbaran)+"' ";
        SQLQuery += " ,proporciondalbaran = "+companyact->sanearCadena(mdb_proporciondalbaran)+" ";
        SQLQuery += " ,idalbaran = "+companyact->sanearCadena(mdb_idalbaran)+" ";
        SQLQuery += " WHERE iddalbaran = "+companyact->sanearCadena(mdb_iddalbaran);
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
}// end guardaDescuentoAlbaranCliente


