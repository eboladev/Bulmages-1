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
    addDBCampo("descripcion", DBCampo::DBvarchar, DBCampo::DBNoSave, "Descripcion Asiento");
    addDBCampo("fecha", DBCampo::DBdate, DBCampo::DBNothing, "Fecha Asiento");
    addDBCampo("comentariosasiento", DBCampo::DBvarchar, DBCampo::DBNoSave, "Comentarios Asiento");
    addDBCampo("ordenasiento", DBCampo::DBint, DBCampo::DBNotNull, "Orden Asiento");
    addDBCampo("clase", DBCampo::DBint, DBCampo::DBNoSave, "Tipo Asiento");
    listalineas = NULL;
}


Asiento1::~Asiento1() {}


void Asiento1::borraAsiento1() {
    if (DBvalue("idasiento") != "") {
        listalineas->borrar();
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM asiento WHERE idasiento="+DBvalue("idasiento"));
        if (error) {
            companyact->rollback();
            return;
        }// end if
        companyact->commit();
        vaciaAsiento1();
        pintaAsiento1();
    }// end if
}// end borraAlbaranCliente
 

void Asiento1::vaciaAsiento1() {
    DBclear();
}// end vaciaAlbaranCliente



void Asiento1::pintaAsiento1() {
    _depura("pintaAlbaranCliente\n",0);
    pintaidasiento(DBvalue("idasiento"));
    pintadescripcion(DBvalue("descripcion"));
    pintafecha(DBvalue("fecha"));
    pintacomentariosasiento(DBvalue("comentariosasiento"));
    pintaordenasiento(DBvalue("ordenasiento"));
    pintaclase(DBvalue("clase"));
    /// Pinta el subformulario de detalle del AlbaranCliente.
    listalineas->pintaListLinAsiento1();
    /// Pintamos los totales
    calculaypintatotales();
    trataestadoAsiento1();
}// end pintaAlbaranCliente


// Esta funci� carga un Asiento.
int Asiento1::cargaAsiento1(QString idbudget) {
    _depura("Asiento1::cargaAsiento1("+idbudget+")\n",0);
    QString query = "SELECT * FROM asiento WHERE idasiento="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    }// end if
    delete cur;
    /// Si no existe lista de lineas se crea una.
    if (listalineas == NULL)
        listalineas = new ListLinAsiento1(companyact);
    listalineas->cargaListLinAsiento1(idbudget);
    /// Si no existe lista de descuentos se crea una.
    pintaAsiento1();
    _depura("Fin AlbaranCliente::cargaAlbaranCliente("+idbudget+")\n",0);
    return 0;
}// end chargeBudget


void Asiento1::abreAsiento1() {
    _depura("Asiento1::abreAsiento1",0);
    QString id= DBvalue("idasiento");
    if (id == "") {
        _depura("No hay asiento");
        return;
    }
    companyact->abreasiento(id.toInt());
    trataestadoAsiento1();
}

void Asiento1::cierraAsiento1() {
    _depura("Asiento1::cierraAsiento1",0);
    if (guardaAsiento1())
        return;
    QString id= DBvalue("idasiento");
    if (id == "") {
        _depura("No hay asiento");
        return;
    }
    companyact->cierraasiento(id.toInt());
    trataestadoAsiento1();
    _depura("END Asiento1::cierraasiento1",0);
}// end cierraAsiento1

Asiento1::estadoasiento  Asiento1::estadoAsiento1() {
    QString SQLQuery = "SELECT count(idborrador) AS cuenta FROM borrador WHERE idasiento="+DBvalue("idasiento");
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    QString numborr = cur->valor("cuenta");
    delete cur;

    SQLQuery = "SELECT count(idapunte) AS cuenta FROM apunte WHERE idasiento="+DBvalue("idasiento");
    cur = companyact->cargacursor(SQLQuery);
    QString numap = cur->valor("cuenta");
    delete cur;

    if (numborr == "0")
        return ASVacio;
    if (numborr == numap)
        return ASCerrado;
    return ASAbierto;
}


int Asiento1::guardaAsiento1() {
    /// Todo el guardado es una transacci�
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return -1;
    }// end if
    setidasiento(id);
    companyact->commit();
    error = listalineas->guardaListLinAsiento1();
    if (error)
        return -1;
//    cargaAsiento1(id);
    return 0;
}// end guardaAlbaranCliente



