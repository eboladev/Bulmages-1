//
// C++ Implementation: Cliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "cliente.h"
#include "company.h"
#include "configuracion.h"
#include "plugins.h"

#include <QFile>
#include <QTextStream>



Cliente::Cliente(company *comp) : DBRecord(comp) {
    _depura("Cliente::Cliente",0);
    m_companyact=comp;
    setDBTableName("cliente");
    setDBCampoId("idcliente");
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador");
    addDBCampo("nomcliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("nomaltcliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("cifcliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("bancocliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("dircliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("poblcliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("cpcliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("telcliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("faxcliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("mailcliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("urlcliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("faltacliente", DBCampo::DBdate, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("fbajacliente", DBCampo::DBdate, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("comentcliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("inactivocliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("provcliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    _depura("END Cliente::Cliente",0);
}

Cliente::~Cliente() {}


void Cliente::borraCliente() {
    if (DBvalue("idcliente") != "") {
        m_companyact->begin();
        int error = m_companyact->ejecuta("DELETE FROM cliente WHERE idcliente="+DBvalue("idcliente"));
        if (error) {
            m_companyact->rollback();
            return;
        }// end if
        m_companyact->commit();
        vaciaCliente();
    }// end if
}


void Cliente::vaciaCliente() {
    DBclear();
}
void Cliente::pintaCliente() {
    _depura("Cliente::pintaCliente",0);

    /// Disparamos los plugins con presupuesto_imprimirPresupuesto
    int res = g_plugins->lanza("Cliente_pintaCliente", this);
    if (res != 0)
        return;

    pintaidcliente(DBvalue("idcliente"));
    pintanomcliente (DBvalue("nomcliente"));
    pintanomaltcliente (DBvalue("nomaltcliente"));
    pintacifcliente (DBvalue("cifcliente"));
    pintabancocliente (DBvalue("bancocliente"));
    pintadircliente (DBvalue("dircliente"));
    pintapoblcliente (DBvalue("poblcliente"));
    pintacpcliente (DBvalue("cpcliente"));
    pintatelcliente (DBvalue("telcliente"));
    pintafaxcliente (DBvalue("faxcliente"));
    pintamailcliente (DBvalue("mailcliente"));
    pintaurlcliente (DBvalue("urlcliente"));
    pintafaltacliente (DBvalue("faltacliente"));
    pintafbajacliente (DBvalue("fbajacliente"));
    pintacomentcliente  (DBvalue("comentcliente"));
    pintainactivocliente  (DBvalue("inactivocliente"));
    pintaprovcliente  (DBvalue("provcliente"));

    _depura("END Cliente::pintaCliente",0);

}


// Esta funci� carga un Cliente.
int Cliente::cargar(QString idcliente) {
    _depura("Cliente::cargaCliente",0);
    QString query = "SELECT * FROM cliente WHERE idcliente="+idcliente;
    cursor2 * cur= m_companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    }// end if
    delete cur;
    _depura("END Cliente::cargaCliente",0);
    return 0;
}


void Cliente::guardaCliente() {
    QString id;
    m_companyact->begin();
    int error = DBsave(id);
    if (error ) {
        m_companyact->rollback();
        return;
    }// end if
    setidcliente(id);
    m_companyact->commit();
}


