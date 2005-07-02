//
// C++ Implementation: Pago
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pago.h"
#include "company.h"
#include "configuracion.h"
#include <qfile.h>
#include <qtextstream.h>



Pago::Pago(company *comp) {
    companyact=comp;
    vaciaPago();
}

Pago::~Pago() {}


void Pago::borraPago() {
    if (mdb_idpago != "") {
        companyact->begin();
        companyact->ejecuta("DELETE FROM pago WHERE idpago="+mdb_idpago);
        companyact->commit();
        vaciaPago();
        pintaPago();
    }// end if
}// end borraPago


void Pago::vaciaPago() {
    mdb_idpago = "";
    mdb_idproveedor = "";
    mdb_fechapago = "";
    mdb_cantpago = "";
    mdb_refpago = "";
    mdb_previsionpago = "FALSE";
    mdb_comentpago = "";
}// end vaciaPago

void Pago::pintaPago() {
    pintaidpago(mdb_idpago);
    pintaidproveedor(mdb_idproveedor);
    pintafechapago(mdb_fechapago);
    pintacantpago(mdb_cantpago);
    pintarefpago(mdb_refpago);
    pintaprevisionpago(mdb_previsionpago);
    pintacomentpago(mdb_comentpago);
}// end pintaPago


// Esta función carga un Pago.
void Pago::cargaPago(QString idbudget) {
    mdb_idpago = idbudget;
    QString query = "SELECT * FROM pago WHERE idPago="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        mdb_idproveedor= cur->valor("idproveedor");
        mdb_fechapago= cur->valor("fechapago");
        mdb_cantpago= cur->valor("cantpago");
        mdb_refpago= cur->valor("refpago");
        mdb_comentpago= cur->valor("comentpago");

        if (cur->valor("previsionpago") == "t")
            mdb_previsionpago = "TRUE";
        else
            mdb_previsionpago = "FALSE";
    }// end if
    delete cur;
    fprintf(stderr,"Vamos a cargar las lineas\n");
    pintaPago();
}// end chargeBudget


void Pago::guardaPago() {
    companyact->begin();
    if (mdb_idpago == "") {
        /// Se trata de una inserción
        QString SQLQuery = "INSERT INTO pago (idproveedor, fechapago, cantpago, refpago, comentpago, previsionpago) VALUES ("+mdb_idproveedor+",'"+mdb_fechapago+"',"+mdb_cantpago+",'"+mdb_refpago+"','"+mdb_comentpago+"',"+mdb_previsionpago+")";
        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idpago) AS m FROM pago");
        if (!cur->eof())
            setidpago(cur->valor("m"));
        delete cur;
        companyact->commit();
    } else {
        /// Se trata de una modificación
        QString SQLQuery = "UPDATE pago SET ";
        SQLQuery += " idproveedor="+mdb_idproveedor;
        SQLQuery += " ,fechapago='"+mdb_fechapago+"'";
        SQLQuery += " ,cantpago='"+mdb_cantpago+"'";
        SQLQuery += " ,refpago='"+mdb_refpago+"'";
        SQLQuery += " ,comentpago='"+mdb_comentpago+"'";
        SQLQuery += " ,previsionpago='"+mdb_previsionpago+"'";
        SQLQuery += " WHERE idpago="+mdb_idpago;
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        companyact->commit();
    }// end if
    cargaPago(mdb_idpago);
}// end guardaPago


