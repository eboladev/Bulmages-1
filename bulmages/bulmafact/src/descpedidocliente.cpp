//
// C++ Implementation: DescuentoPedidoCliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "descpedidocliente.h"

DescuentoPedidoCliente::DescuentoPedidoCliente(company *comp) {
    companyact = comp;
    vaciaDescuentoPedidoCliente();
}

DescuentoPedidoCliente::DescuentoPedidoCliente(company *comp, QString id) {
    companyact = comp;
    QString SQLQuery = "SELECT * FROM dpedidocliente WHERE  iddpedidocliente="+id;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_iddpedidocliente = cur->valor("iddpedidocliente");
        mdb_conceptdpedidocliente = cur->valor("conceptdpedidocliente");
        mdb_proporciondpedidocliente = cur->valor("proporciondpedidocliente");
        mdb_idpedidocliente = cur->valor("idpedidocliente");
    } else {
        vaciaDescuentoPedidoCliente();
    }// end if
}// end DescuentoPedidoCliente


DescuentoPedidoCliente::DescuentoPedidoCliente(company *comp, QString a, QString b, QString c, QString d) {
    companyact = comp;
    mdb_iddpedidocliente = a;
    mdb_conceptdpedidocliente = b;
    mdb_proporciondpedidocliente = c;
    mdb_idpedidocliente =d;
}// end DescuentoPedidoCliente


DescuentoPedidoCliente::~DescuentoPedidoCliente() {}


void DescuentoPedidoCliente::vaciaDescuentoPedidoCliente() {
    mdb_iddpedidocliente = "";
    mdb_conceptdpedidocliente = "";
    mdb_proporciondpedidocliente = "";
    mdb_idpedidocliente ="";
}


void DescuentoPedidoCliente::borrar() {
    if (mdb_iddpedidocliente != "") {
        companyact->begin();
        companyact->ejecuta("DELETE FROM dpedidocliente WHERE iddpedidocliente="+mdb_iddpedidocliente);
        companyact->commit();
        vaciaDescuentoPedidoCliente();
    }// end if
}// end delete

void DescuentoPedidoCliente::guardaDescuentoPedidoCliente() {
    /// Segun est�la linea en la base de datos o no se hace una cosa u otra.
    if (mdb_iddpedidocliente == "") {
        QString SQLQuery = "INSERT INTO dpedidocliente (conceptdpedidocliente, proporciondpedidocliente, idpedidocliente) VALUES ('"+
	companyact->sanearCadena(mdb_conceptdpedidocliente)+"',"+
	companyact->sanearCadena(mdb_proporciondpedidocliente)+","+
	companyact->sanearCadena(mdb_idpedidocliente)+")";
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(iddpedidocliente) AS m FROM dpedidocliente ");
        if(!cur->eof())
            mdb_iddpedidocliente = cur->valor("m");
        delete cur;
        companyact->commit();
    } else {
        QString SQLQuery = "UPDATE dpedidocliente SET ";
        SQLQuery += " conceptdpedidocliente = '"+companyact->sanearCadena(mdb_conceptdpedidocliente)+"' ";
        SQLQuery += " ,proporciondpedidocliente = "+companyact->sanearCadena(mdb_proporciondpedidocliente)+" ";
        SQLQuery += " ,idpedidocliente = "+companyact->sanearCadena(mdb_idpedidocliente)+" ";
        SQLQuery += " WHERE iddpedidocliente = "+companyact->sanearCadena(mdb_iddpedidocliente);
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        companyact->commit();
    }// end if
}// end guardaDescuentoPedidoCliente


