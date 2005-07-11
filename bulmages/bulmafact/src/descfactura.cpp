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

DescuentoFactura::DescuentoFactura(company *comp) {
    companyact = comp;
    vaciaDescuentoFactura();
}

DescuentoFactura::DescuentoFactura(company *comp, QString id) {
    companyact = comp;
    QString SQLQuery = "SELECT * FROM dfactura WHERE  iddfactura="+id;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_iddfactura = cur->valor("iddfactura");
        mdb_conceptdfactura = cur->valor("conceptdfactura");
        mdb_proporciondfactura = cur->valor("proporciondfactura");
        mdb_idfactura = cur->valor("idfactura");
    } else {
        vaciaDescuentoFactura();
    }// end if
}// end DescuentoFactura


DescuentoFactura::DescuentoFactura(company *comp, QString a, QString b, QString c, QString d) {
    companyact = comp;
    mdb_iddfactura = a;
    mdb_conceptdfactura = b;
    mdb_proporciondfactura = c;
    mdb_idfactura =d;
}// end DescuentoFactura


DescuentoFactura::~DescuentoFactura() {}


void DescuentoFactura::vaciaDescuentoFactura() {
    mdb_iddfactura = "";
    mdb_conceptdfactura = "";
    mdb_proporciondfactura = "";
    mdb_idfactura ="";
}


void DescuentoFactura::borrar() {
    if (mdb_iddfactura != "") {
        companyact->begin();
        companyact->ejecuta("DELETE FROM dfactura WHERE iddfactura="+mdb_iddfactura);
        companyact->commit();
        vaciaDescuentoFactura();
    }// end if
}// end delete

void DescuentoFactura::guardaDescuentoFactura() {
    /// Segun est�la linea en la base de datos o no se hace una cosa u otra.
    if (mdb_iddfactura == "") {
        QString SQLQuery = "INSERT INTO dfactura (conceptdfactura, proporciondfactura, idfactura) VALUES ('"+
	companyact->sanearCadena(mdb_conceptdfactura)+"',"+
	companyact->sanearCadena(mdb_proporciondfactura)+","+
	companyact->sanearCadena(mdb_idfactura)+")";
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(iddfactura) AS m FROM dfactura ");
        if(!cur->eof())
            mdb_iddfactura = cur->valor("m");
        delete cur;
        companyact->commit();
    } else {
        QString SQLQuery = "UPDATE dfactura SET ";
        SQLQuery += " conceptdfactura = '"+companyact->sanearCadena(mdb_conceptdfactura)+"' ";
        SQLQuery += " ,proporciondfactura = "+companyact->sanearCadena(mdb_proporciondfactura)+" ";
        SQLQuery += " ,idfactura = "+companyact->sanearCadena(mdb_idfactura)+" ";
        SQLQuery += " WHERE iddfactura = "+companyact->sanearCadena(mdb_iddfactura);
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        companyact->commit();
    }// end if
}// end guardaDescuentoFactura


