//
// C++ Implementation: DescuentoPedidoProveedor
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "descpedidoproveedor.h"

DescuentoPedidoProveedor::DescuentoPedidoProveedor(company *comp) {
    companyact = comp;
    vaciaDescuentoPedidoProveedor();
}

DescuentoPedidoProveedor::DescuentoPedidoProveedor(company *comp, QString id) {
    companyact = comp;
    QString SQLQuery = "SELECT * FROM dpedidoproveedor WHERE  iddpedidoproveedor="+id;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_iddpedidoproveedor = cur->valor("iddpedidoproveedor");
        mdb_conceptdpedidoproveedor = cur->valor("conceptdpedidoproveedor");
        mdb_proporciondpedidoproveedor = cur->valor("proporciondpedidoproveedor");
        mdb_idpedidoproveedor = cur->valor("idpedidoproveedor");
    } else {
        vaciaDescuentoPedidoProveedor();
    }// end if
}// end DescuentoPedidoProveedor


DescuentoPedidoProveedor::DescuentoPedidoProveedor(company *comp, QString a, QString b, QString c, QString d) {
    companyact = comp;
    mdb_iddpedidoproveedor = a;
    mdb_conceptdpedidoproveedor = b;
    mdb_proporciondpedidoproveedor = c;
    mdb_idpedidoproveedor =d;
}// end DescuentoPedidoProveedor


DescuentoPedidoProveedor::~DescuentoPedidoProveedor() {}


void DescuentoPedidoProveedor::vaciaDescuentoPedidoProveedor() {
    mdb_iddpedidoproveedor = "";
    mdb_conceptdpedidoproveedor = "";
    mdb_proporciondpedidoproveedor = "";
    mdb_idpedidoproveedor ="";
}


void DescuentoPedidoProveedor::borrar() {
    if (mdb_iddpedidoproveedor != "") {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM dpedidoproveedor WHERE iddpedidoproveedor="+mdb_iddpedidoproveedor);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
        vaciaDescuentoPedidoProveedor();
    }// end if
}// end delete

void DescuentoPedidoProveedor::guardaDescuentoPedidoProveedor() {
    /// Segun est�la linea en la base de datos o no se hace una cosa u otra.
    if (mdb_iddpedidoproveedor == "") {
        QString SQLQuery = "INSERT INTO dpedidoproveedor (conceptdpedidoproveedor, proporciondpedidoproveedor, idpedidoproveedor) VALUES ('"+
	companyact->sanearCadena(mdb_conceptdpedidoproveedor)+"',"+
	companyact->sanearCadena(mdb_proporciondpedidoproveedor)+","+
	companyact->sanearCadena(mdb_idpedidoproveedor)+")";
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        cursor2 *cur = companyact->cargacursor("SELECT MAX(iddpedidoproveedor) AS m FROM dpedidoproveedor ");
        if(!cur->eof())
            mdb_iddpedidoproveedor = cur->valor("m");
        delete cur;
        companyact->commit();
    } else {
        QString SQLQuery = "UPDATE dpedidoproveedor SET ";
        SQLQuery += " conceptdpedidoproveedor = '"+companyact->sanearCadena(mdb_conceptdpedidoproveedor)+"' ";
        SQLQuery += " ,proporciondpedidoproveedor = "+companyact->sanearCadena(mdb_proporciondpedidoproveedor)+" ";
        SQLQuery += " ,idpedidoproveedor = "+companyact->sanearCadena(mdb_idpedidoproveedor)+" ";
        SQLQuery += " WHERE iddpedidoproveedor = "+companyact->sanearCadena(mdb_iddpedidoproveedor);
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
}// end guardaDescuentoPedidoProveedor


