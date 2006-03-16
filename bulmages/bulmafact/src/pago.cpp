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
#include <QFile>
#include <QTextStream>

Pago::Pago(company *comp) : DBRecord(comp) {
    companyact=comp;
    setDBTableName("pago");
    setDBCampoId("idpago");
    addDBCampo("idpago", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Presupuesto");
    addDBCampo("idproveedor", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("previsionpago", DBCampo::DBboolean, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("fechapago", DBCampo::DBdate, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("refpago", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("cantpago", DBCampo::DBnumeric, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("comentpago", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
}

Pago::~Pago() {}


void Pago::borraPago() {
    if (DBvalue("idpago") != "") {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM pago WHERE idpago="+DBvalue("idpago"));
	if (error) {
		companyact->rollback();
		return;
	}// en dif
        companyact->commit();
        vaciaPago();
        pintaPago();
    }// end if
}// end borraPago


void Pago::vaciaPago() {
    DBclear();
}// end vaciaPago

void Pago::pintaPago() {
    pintaidpago(DBvalue("idpago"));
    pintaidproveedor(DBvalue("idproveedor"));
    pintafechapago(DBvalue("fechapago"));
    pintacantpago(DBvalue("cantpago"));
    pintarefpago(DBvalue("refpago"));
    pintaprevisionpago(DBvalue("previsionpago"));
    pintacomentpago(DBvalue("comentpago"));
}// end pintaPago


// Esta funci� carga un Pago.
int Pago::cargar(QString idpago) {
    QString query = "SELECT * FROM pago WHERE idPago="+idpago;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    }// end if
    delete cur;
    pintaPago();
	return 0;
}// end chargeBudget


void Pago::guardaPago() {
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return;
    }// end if
    setidpago(id);
    companyact->commit();
//    cargaPago(id);
}// end guardaPago


