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

#include "iva.h"
#include "empresa.h"


void Iva::definetabla() {
    _depura("Iva::definetabla", 0);
    setDBTableName("iva");
    setDBCampoId("idiva");
    addDBCampo("idiva", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador");
    addDBCampo("idtipoiva", DBCampo::DBint, DBCampo::DBNotNull, "Identificador");
    addDBCampo("idregistroiva", DBCampo::DBint, DBCampo::DBNotNull, "Identificador");
    addDBCampo("baseiva", DBCampo::DBnumeric, DBCampo::DBNotNull, "Cantidad");
    addDBCampo("ivaiva", DBCampo::DBnumeric, DBCampo::DBNotNull, "Precio Linea Presupuesto");
    addDBCampo("idcuenta", DBCampo::DBint, DBCampo::DBNoSave, "Id. Cuenta");
    addDBCampo("codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, "Codigo");
    addDBCampo("nombretipoiva", DBCampo::DBvarchar, DBCampo::DBNoSave, "Nombre Tipo Iva");
    _depura("END Iva::definetabla",0);
}


Iva::Iva(empresa *comp) : DBRecord(comp) {
    _depura("Iva::Iva", 0);
    m_companyact = comp;
    definetabla();
    _depura("END Iva::Iva", 0);
}


Iva::Iva(empresa *comp, QString idiva) : DBRecord(comp) {
    m_companyact = comp;
    definetabla();
    QString SQLQuery = "SELECT * FROM iva LEFT JOIN tipoiva ON iva.idtipoiva = tipoiva.idtipoiva LEFT JOIN cuenta ON cuenta.idcuenta = tipoiva.idtipoiva WHERE idiva="+idiva;
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        DBload(cur);
    } else {
        vaciaIva();
    }// end if
}


Iva::Iva(empresa *comp, cursor2 *cur) : DBRecord(comp) {
    m_companyact = comp;
    definetabla();
    DBload(cur);
}


Iva::~Iva() {}


void Iva::vaciaIva() {
    DBclear();
}


int Iva::borrar() {
    if (DBvalue("idiva") != "") {
        m_companyact->begin();
        int error = m_companyact->ejecuta("DELETE FROM iva WHERE idiva = " + DBvalue("idiva"));
        if (error) {
            m_companyact->rollback();
            return -1;
        } // end if
        m_companyact->commit();
        vaciaIva();
    } // end if
    return 0;
}


void Iva::guardaIva() {
    QString id;
    m_companyact->begin();
    int error = DBsave(id);
    if (error) {
        m_companyact->rollback();
        return;
    } // end if
    setDBvalue("idiva", id);
    m_companyact->commit();
}


void Iva::setidtipoiva(const QString &val) {
    _depura("Iva::setidtipoiva", 0);
    QString SQLQuery = "SELECT * FROM tipoiva LEFT JOIN cuenta ON cuenta.idcuenta = tipoiva.idcuenta WHERE idtipoiva = " + val;
    cursor2 *cur= m_companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        _depura(cur->valor("codigo"), 0);
        setDBvalue("idcuenta",cur->valor("idcuenta"));
        setDBvalue("codigo", cur->valor("codigo"));
        setDBvalue("nombretipoiva", cur->valor("nombretipoiva"));
    } // end if
    delete cur;
    setDBvalue("idtipoiva", val);
}

