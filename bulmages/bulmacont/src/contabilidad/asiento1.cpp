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
 
*/

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
	_depura("Asiento1::abreAsiento1",2);
	QString id= DBvalue("idasiento");
	if (id == "") {
		_depura("No hay asiento");
		return;
	}
	companyact->abreasiento(id.toInt());
}

void Asiento1::cierraAsiento1() {
	_depura("Asiento1::cierraAsiento1",2);
	guardaAsiento1();
	QString id= DBvalue("idasiento");
	if (id == "") {
		_depura("No hay asiento");
		return;
	}
	companyact->cierraasiento(id.toInt());
}

int  estadoAsiento1() {
	return 0;
}

 
void Asiento1::guardaAsiento1() {
    /// Todo el guardado es una transacci�
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return;
    }// end if
    setidasiento(id);
    companyact->commit();
    listalineas->guardaListLinAsiento1();
    cargaAsiento1(id);
}// end guardaAlbaranCliente
 
 

