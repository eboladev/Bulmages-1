/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QFile>
#include <QTextStream>

#include "cliente.h"
#include "company.h"
#include "configuracion.h"
#include "plugins.h"


Cliente::Cliente(company *comp) : DBRecord(comp) {
    _depura("Cliente::Cliente", 0);
    m_companyact = comp;
    setDBTableName("cliente");
    setDBCampoId("idcliente");
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("Cliente", "ID cliente"));
    addDBCampo("nomcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Nombre del cliente"));
    addDBCampo("nomaltcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Nombre alternativo del cliente"));
    addDBCampo("cifcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "C.I.F. del cliente"));
    addDBCampo("bancocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Numero cuenta corriente"));
    addDBCampo("dircliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Direccion"));
    addDBCampo("poblcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Poblacion"));
    addDBCampo("provcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Provincia"));
    addDBCampo("cpcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Codigo postal"));
    addDBCampo("telcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Numero de telefono"));
    addDBCampo("faxcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Numero de fax"));
    addDBCampo("mailcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Direccion electronica"));
    addDBCampo("urlcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Identificador Presupuesto"));
    addDBCampo("faltacliente", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Cliente", "Fecha de alta del cliente"));
    addDBCampo("fbajacliente", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Cliente", "Fecha de baja del cliente"));
    addDBCampo("comentcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Comentarios"));
    addDBCampo("inactivocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Cliente inactivo"));
    _depura("END Cliente::Cliente", 0);
}


Cliente::~Cliente() {}


void Cliente::borraCliente() {
    if (DBvalue("idcliente") != "") {
        m_companyact->begin();
        int error = m_companyact->ejecuta("DELETE FROM cliente WHERE idcliente=" + DBvalue("idcliente"));
        if (error) {
            m_companyact->rollback();
            return;
        } // end if
        m_companyact->commit();
        vaciaCliente();
    } // end if
}


void Cliente::vaciaCliente() {
    DBclear();
}


void Cliente::pintaCliente() {
    _depura("Cliente::pintaCliente", 0);

    /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
    int res = g_plugins->lanza("Cliente_pintaCliente", this);
    if (res != 0)
        return;

    pintaidcliente(DBvalue("idcliente"));
    pintanomcliente(DBvalue("nomcliente"));
    pintanomaltcliente(DBvalue("nomaltcliente"));
    pintacifcliente(DBvalue("cifcliente"));
    pintabancocliente(DBvalue("bancocliente"));
    pintadircliente(DBvalue("dircliente"));
    pintapoblcliente(DBvalue("poblcliente"));
    pintacpcliente(DBvalue("cpcliente"));
    pintatelcliente(DBvalue("telcliente"));
    pintafaxcliente(DBvalue("faxcliente"));
    pintamailcliente(DBvalue("mailcliente"));
    pintaurlcliente(DBvalue("urlcliente"));
    pintafaltacliente(DBvalue("faltacliente"));
    pintafbajacliente(DBvalue("fbajacliente"));
    pintacomentcliente(DBvalue("comentcliente"));
    pintainactivocliente(DBvalue("inactivocliente"));
    pintaprovcliente(DBvalue("provcliente"));

    _depura("END Cliente::pintaCliente", 0);

}


/// Esta funcion carga un cliente.
int Cliente::cargar(QString idcliente) {
    _depura("Cliente::cargaCliente", 0);
    QString query = "SELECT * FROM cliente WHERE idcliente=" + idcliente;
    cursor2 * cur= m_companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;
    _depura("END Cliente::cargaCliente", 0);
    return 0;
}


int Cliente::guardar() {
    _depura("Cliente::guardar", 0);
    QString id;
    m_companyact->begin();
    try {
        DBsave(id);
        setidcliente(id);
        m_companyact->commit();
        _depura("END Cliente::guardar", 0);
        return 0;
    } catch(...) {
        _depura("error al guardar el cliente", 1);
        m_companyact->rollback();
        return -1;
    }
}

