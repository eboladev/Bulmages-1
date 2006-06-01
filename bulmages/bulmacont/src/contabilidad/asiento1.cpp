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
#include <QTextStream>
#include <qmessagebox.h>
#include "fixed.h"
#include "funcaux.h"
#include "plugins.h"
#include "empresa.h"

Asiento1::Asiento1(empresa *comp) : DBRecord (comp) {
    _depura("Asiento1::Asiento1(empresa *)",0);
    m_companyact=comp;
    setDBTableName("asiento");
    setDBCampoId("idasiento");
    addDBCampo("idasiento", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Asiento");
    addDBCampo("descripcion", DBCampo::DBvarchar, DBCampo::DBNoSave, "Descripcion Asiento");
    addDBCampo("fecha", DBCampo::DBdate, DBCampo::DBNothing, "Fecha Asiento");
    addDBCampo("comentariosasiento", DBCampo::DBvarchar, DBCampo::DBNothing, "Comentarios Asiento");
    addDBCampo("ordenasiento", DBCampo::DBint, DBCampo::DBNotNull, "Orden Asiento");
    addDBCampo("clase", DBCampo::DBint, DBCampo::DBNothing, "Tipo Asiento");
    listalineas = NULL;
}


Asiento1::~Asiento1() {}


void Asiento1::borraAsiento1() {
    _depura("Asiento1::borraAsiento1",0);
    int error;
    if (DBvalue("idasiento") != "") {
        switch( QMessageBox::warning( 0, "Borrar Asento",
                                      "Se va a borrar el asiento,\n"
                                      "Esta seguro?\n",
                                      QMessageBox::Ok ,
                                      QMessageBox::Cancel )) {
        case QMessageBox::Ok: // Retry clicked or Enter pressed
            listalineas->borrar();
            m_companyact->begin();
            error = m_companyact->ejecuta("DELETE FROM apunte WHERE idasiento="+DBvalue("idasiento"));
            error += m_companyact->ejecuta("DELETE FROM asiento WHERE idasiento="+DBvalue("idasiento"));
            if (error) {
                m_companyact->rollback();
                return;
            }// end if
            m_companyact->commit();
            vaciaAsiento1();
            break;
        case QMessageBox::Cancel: // Abort clicked or Escape pressed
            break;
        }// end switch
    }// end if
}


void Asiento1::vaciaAsiento1() {
    _depura("Asiento1::vaciaAsiento1",0);
    DBclear();
    listalineas->cargar("0");
}



void Asiento1::pintaAsiento1() {
    _depura("pintaAlbaranCliente\n",0);
    pintaidasiento(DBvalue("idasiento"));
    pintadescripcion(DBvalue("descripcion"));
    pintafecha(DBvalue("fecha"));
    pintacomentariosasiento(DBvalue("comentariosasiento"));
    pintaordenasiento(DBvalue("ordenasiento"));
    pintaclase(DBvalue("clase"));
    /// Pintamos los totales
    calculaypintatotales();
    trataestadoAsiento1();
}


// Esta funci� carga un Asiento.
int Asiento1::cargar(QString idbudget) {
    _depura("Asiento1::cargar("+idbudget+")\n",0);
    QString query = "SELECT * FROM asiento WHERE idasiento="+idbudget;
    cursor2 * cur= m_companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    }// end if
    delete cur;
    listalineas->cargar(idbudget);
    /// Si no existe lista de descuentos se crea una.
    pintaAsiento1();
    _depura("END Asiento1::cargar("+idbudget+")\n",0);
    return 0;
}


void Asiento1::abreAsiento1() {
    _depura("Asiento1::abreAsiento1",0);
    if (estadoAsiento1() != ASCerrado)
        return;
    QString id= DBvalue("idasiento");
    if (id == "") {
        _depura("No hay asiento");
        return;
    }
    m_companyact->abreasiento(id.toInt());
    trataestadoAsiento1();
}

void Asiento1::cierraAsiento1() {
    _depura("Asiento1::cierraAsiento1",0);
    if (estadoAsiento1() != ASAbierto)
        return;

    if (guardar())
        return;
    QString id= DBvalue("idasiento");
    if (id == "") {
        _depura("No hay asiento");
        return;
    }
    m_companyact->cierraasiento(id.toInt());
    QString idasiento = DBvalue("idasiento");
    vaciaAsiento1();
    cargar(idasiento);
    _depura("END Asiento1::cierraasiento1",0);
}


Asiento1::estadoasiento  Asiento1::estadoAsiento1() {
    if (DBvalue("idasiento") == "")
        return ASVacio;

    QString SQLQuery = "SELECT count(idborrador) AS cuenta FROM borrador WHERE idasiento="+DBvalue("idasiento");
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    QString numborr = cur->valor("cuenta");
    delete cur;

    SQLQuery = "SELECT count(idapunte) AS cuenta FROM apunte WHERE idasiento="+DBvalue("idasiento");
    cur = m_companyact->cargacursor(SQLQuery);
    QString numap = cur->valor("cuenta");
    delete cur;

    if (numborr == "0")
        return ASVacio;
    if (numap != "0")
        return ASCerrado;
    return ASAbierto;
}


int Asiento1::guardar() {
    _depura("Asiento1::guardar",0);
    QString id;
    m_companyact->begin();
    try {
        DBsave(id);
        setidasiento(id);
        listalineas->guardar();
        m_companyact->commit();

        /// Disparamos los plugins con presupuesto_imprimirPresupuesto
        int res = g_plugins->lanza("Asiento1_guardaAsiento1_post", this);
        _depura("END Asiento1::guardar",0);

        if (estadoAsiento1() == ASCerrado)
   		 m_companyact->cierraasiento(id.toInt());


        return 0;
    } catch(...) {
        _depura("Error guardando, se cancela la operacion",1);
        m_companyact->rollback();
        return -1;
    }
}




