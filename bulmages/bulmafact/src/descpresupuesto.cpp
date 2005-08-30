//
// C++ Implementation: DescuentoPresupuesto
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "descpresupuesto.h"

/*
-- Descuento de presupuesto.
-- Numero
--Concepte: Descripció del motiu de descompte.
--Proporcio: Percentatge a descomptar.
-- Descompte de pressupost a clients.
CREATE TABLE dpresupuesto (
   iddpresupuesto serial PRIMARY KEY,
   conceptdpresupuesto character varying(2000),
   proporciondpresupuesto numeric(5,2),
   idpresupuesto integer REFERENCES presupuesto(idpresupuesto)
   -- Falta poner el lugar donde se aplica el descuento, antes de la factura o después de ésta.
);
*/

DescuentoPresupuesto::DescuentoPresupuesto(company *comp) {
    companyact = comp;
    vaciaDescuentoPresupuesto();
}

DescuentoPresupuesto::DescuentoPresupuesto(company *comp, QString id) {
    companyact = comp;
    QString SQLQuery = "SELECT * FROM dpresupuesto WHERE  iddpresupuesto="+id;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_iddpresupuesto = cur->valor("iddpresupuesto");
        mdb_conceptdpresupuesto = cur->valor("conceptdpresupuesto");
        mdb_proporciondpresupuesto = cur->valor("proporciondpresupuesto");
        mdb_idpresupuesto = cur->valor("idpresupuesto");
    } else {
        vaciaDescuentoPresupuesto();
    }// end if
}// end DescuentoPresupuesto


DescuentoPresupuesto::DescuentoPresupuesto(company *comp, QString a, QString b, QString c, QString d) {
    companyact = comp;
    mdb_iddpresupuesto = a;
    mdb_conceptdpresupuesto = b;
    mdb_proporciondpresupuesto = c;
    mdb_idpresupuesto =d;
}// end DescuentoPresupuesto


DescuentoPresupuesto::~DescuentoPresupuesto() {}


void DescuentoPresupuesto::vaciaDescuentoPresupuesto() {
    mdb_iddpresupuesto = "";
    mdb_conceptdpresupuesto = "";
    mdb_proporciondpresupuesto = "";
    mdb_idpresupuesto ="";
}


void DescuentoPresupuesto::borrar() {
    if (mdb_iddpresupuesto != "") {
        companyact->begin();
        companyact->ejecuta("DELETE FROM dpresupuesto WHERE iddpresupuesto="+mdb_iddpresupuesto);
        companyact->commit();
        vaciaDescuentoPresupuesto();
    }// end if
}// end delete

void DescuentoPresupuesto::guardaDescuentoPresupuesto() {
    /// Segun est�la linea en la base de datos o no se hace una cosa u otra.
    if (mdb_iddpresupuesto == "") {
        QString SQLQuery = "INSERT INTO dpresupuesto (conceptdpresupuesto, proporciondpresupuesto, idpresupuesto) VALUES ('"+
	companyact->sanearCadena(mdb_conceptdpresupuesto)+"',"+
	companyact->sanearCadena(mdb_proporciondpresupuesto)+","+
	companyact->sanearCadena(mdb_idpresupuesto)+")";
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(iddpresupuesto) AS m FROM dpresupuesto ");
        if(!cur->eof())
            mdb_iddpresupuesto = cur->valor("m");
        delete cur;
        companyact->commit();
    } else {
        QString SQLQuery = "UPDATE dpresupuesto SET ";
        SQLQuery += " conceptdpresupuesto = '"+companyact->sanearCadena(mdb_conceptdpresupuesto)+"' ";
        SQLQuery += " ,proporciondpresupuesto = "+companyact->sanearCadena(mdb_proporciondpresupuesto)+" ";
        SQLQuery += " ,idpresupuesto = "+companyact->sanearCadena(mdb_idpresupuesto)+" ";
        SQLQuery += " WHERE iddpresupuesto = "+companyact->sanearCadena(mdb_iddpresupuesto);
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        companyact->commit();
    }// end if
}// end guardaDescuentoPresupuesto


