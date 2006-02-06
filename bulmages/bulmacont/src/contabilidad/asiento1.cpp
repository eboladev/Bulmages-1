//
// C++ Implementation: albarancliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "asiento1.h"

#include <qfile.h>
//Added by qt3to4:
#include <QTextStream>
#include "fixed.h"
#include "funcaux.h"



Asiento1::Asiento1(empresa *comp) : DBRecord (comp) {
    _depura("Asiento1::Asiento1(empresa *)",0);
    companyact=comp;


    setDBTableName("asiento");
    setDBCampoId("idasiento");
    addDBCampo("idasiento", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Asiento");
    addDBCampo("descripcion", DBCampo::DBvarchar, DBCampo::DBNothing, "Descripcion Asiento");
    addDBCampo("fecha", DBCampo::DBdate, DBCampo::DBNothing, "Fecha Asiento");
    addDBCampo("comentariosasiento", DBCampo::DBvarchar, DBCampo::DBNothing, "Comentarios Asiento");
    addDBCampo("ordenasiento", DBCampo::DBint, DBCampo::DBNotNull, "Orden Asiento");
    addDBCampo("clase", DBCampo::DBint, DBCampo::DBNotNull, "Tipo Asiento");

/*
    listalineas = NULL;
    listadescuentos = NULL;
*/

}


Asiento1::~Asiento1() {}

/*

void AlbaranCliente::borraAlbaranCliente() {
    if (DBvalue("idalbaran") != "") {
        listalineas->borrar();
        listadescuentos->borrar();
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM albaran WHERE idalbaran="+DBvalue("idalbaran"));
        if (error) {
            companyact->rollback();
            return;
        }// end if
        companyact->commit();
        vaciaAlbaranCliente();
        pintaAlbaranCliente();
    }// end if
}// end borraAlbaranCliente


void AlbaranCliente::vaciaAlbaranCliente() {
	DBclear();
}// end vaciaAlbaranCliente


void AlbaranCliente::pintaAlbaranCliente() {
    _depura("pintaAlbaranCliente\n",0);
    pintaIdAlbaran(DBvalue("idalbaran"));
    pintaNumAlbaran(DBvalue("numalbaran"));
    pintafechaalbaran(DBvalue("fechaalbaran"));
    pintaComentAlbaran(DBvalue("comentalbaran"));
    pintaComentPrivAlbaran(DBvalue("comentprivalbaran"));
    pintaidcliente(DBvalue("idcliente"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintaidalmacen(DBvalue("idalmacen"));
    pintarefalbaran(DBvalue("refalbaran"));
    pintadescalbaran(DBvalue("descalbaran"));
    pintaidtrabajador(DBvalue("idtrabajador"));
    pintacontactalbaran(DBvalue("contactalbaran"));
    pintatelalbaran(DBvalue("telalbaran"));
    pintaprocesadoalbaran(DBvalue("procesadoalbaran"));
    /// Pinta el subformulario de detalle del AlbaranCliente.
    listalineas->pintaListLinAlbaranCliente();
    listadescuentos->pintaListDescuentoAlbaranCliente();
    /// Pintamos los totales
    calculaypintatotales();
}// end pintaAlbaranCliente



// Esta funci� carga un AlbaranCliente.
int AlbaranCliente::cargaAlbaranCliente(QString idbudget) {
    _depura("AlbaranCliente::cargaAlbaranCliente("+idbudget+")\n",0);
    QString query = "SELECT * FROM albaran WHERE idalbaran="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    }// end if
    delete cur;
	/// Si no existe lista de lineas se crea una.
	if (listalineas == NULL) listalineas = new ListLinAlbaranCliente(companyact);
    listalineas->cargaListLinAlbaranCliente(idbudget);
	/// Si no existe lista de descuentos se crea una.
	if (listadescuentos == NULL) listadescuentos = new ListDescuentoAlbaranCliente(companyact);
    listadescuentos->cargaDescuentos(idbudget);
    pintaAlbaranCliente();
    _depura("Fin AlbaranCliente::cargaAlbaranCliente("+idbudget+")\n",0);
    return 0;
}// end chargeBudget



void AlbaranCliente::guardaAlbaranCliente() {
    /// Todo el guardado es una transacci�
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return;
    }// end if
    setidalbaran(id);
    companyact->commit();
    listalineas->guardaListLinAlbaranCliente();
    listadescuentos->guardaListDescuentoAlbaranCliente();
    cargaAlbaranCliente(id);
}// end guardaAlbaranCliente


*/
