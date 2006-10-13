/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include "dbrecord.h"
#include "funcaux.h"


DBCampo::DBCampo(postgresiface2 *com, QString nom, dbtype typ, int res, QString nomp) {
    m_conexionbase = com;
    m_nomcampo = nom;
    m_tipo = typ;
    m_restrict = res;
    m_nompresentacion = nomp;
    m_valorcampo = "";
}


QString DBCampo::valorcampoprep(int &error) {
    _depura("DBCampo::valorcampoprep", 0);
    error = 0;
    if ((m_restrict & DBNotNull) && !(m_restrict & DBAuto)) {
        if (m_valorcampo == "") {
            _depura("Campo " + m_nompresentacion + " vacio", 2);
            error = -1;
            return "";
        } // end if
    } // end if
    switch (m_tipo) {
    case DBint:
        if (m_valorcampo == "")
            return "NULL";
        return m_conexionbase->sanearCadena(m_valorcampo);
    case DBvarchar:
        if (m_valorcampo == "")
            return "NULL";
        return "'" + m_conexionbase->sanearCadena(m_valorcampo) + "'";
    case DBdate:
        if (m_valorcampo == "")
            return "NULL";
        return "'" + m_conexionbase->sanearCadena(m_valorcampo) + "'";
    case DBnumeric:
        if (m_valorcampo == "")
            return "NULL";
        return m_valorcampo;
    case DBboolean:
        if (m_valorcampo == "")
            return "NULL";
        if (m_valorcampo == "f" || m_valorcampo == "t")
            return "'" + m_conexionbase->sanearCadena(m_valorcampo) + "'";
        return m_conexionbase->sanearCadena(m_valorcampo);
    } // end switch
    error = -1;
    _depura("Error en la conversion de tipos", 2);
    _depura("END DBCampo::valorcampoprep", 0);
    return "";
}


DBRecord::DBRecord(postgresiface2 *con) {
    m_conexionbase = con;
    m_nuevoCampo = TRUE;
}


DBRecord::~DBRecord() {
	_depura("DBRecord::~DBRecord", 0);
	m_lista.clear();
	_depura("END DBRecord::~DBRecord", 0);
}


int DBRecord::DBload(cursor2 *cur) {
    _depura("DBRecord::DBload", 0);
    m_nuevoCampo = FALSE;
    DBCampo *linea;
    int error = 0;
    for (int i = 0; i < m_lista.size(); ++i) {
        linea = m_lista.at(i);
        QString nom = linea->nomcampo();
        QString val = cur->valor(nom);
        if ((linea->restrictcampo() & DBCampo::DBPrimaryKey) && (val == ""))
            m_nuevoCampo = TRUE;
        if ((linea->restrictcampo() & DBCampo::DBDupPrimaryKey) && (val == ""))
            m_nuevoCampo = TRUE;
        error += linea->set(val);
    } // end for
    _depura("END DBRecord::DBload", 0);
    return error;
}


void DBRecord::DBclear() {
    _depura("DBRecord::DBclear", 0);
    m_nuevoCampo = TRUE;
    DBCampo *linea;
    for(int i = 0; i < m_lista.size(); ++i) {
        linea = m_lista.at(i);
        linea->set("");
    } // end for
    _depura("END DBRecord::DBclear", 0);
}


int DBRecord::DBsave(QString &id) {
    _depura("DBRecord::DBsave - " + id, 0);
    try {
        DBCampo *linea;
        QString listcampos = "";
        QString listvalores = "";
        QString queryupdate = "";
        QString separador = "";
        QString separador1 = "";
        QString separadorwhere = "";
        QString querywhere = "";
        int err = 0;
        for (int i = 0; i < m_lista.size(); ++i) {
            linea = m_lista.at(i);
            if (linea->restrictcampo() & DBCampo::DBDupPrimaryKey) {
                QString lin = linea->valorcampoprep(err);
                if (err)
                    throw (-1);
                querywhere += separadorwhere + linea->nompresentacion() + " = " + lin;
                separadorwhere = " AND ";
            } // end if
            if (!(linea->restrictcampo() & DBCampo::DBNoSave)) {
                /// Si el campo es requerido y no está entonces salimos sin dar error.
                /// No es lo mismo que los not null ya que estos si dan error
                if (linea->restrictcampo() & DBCampo::DBRequired) {
                    if (linea->valorcampo() == "")
                        return 0;
                } // end if
                if (linea->restrictcampo() & DBCampo::DBPrimaryKey) {
                    QString lin = linea->valorcampoprep(err);
                    if (err)
                        throw -1;
                    querywhere += separadorwhere + linea->nomcampo() + " = " + lin;
                    separadorwhere = " AND ";
                } // end if
                if (linea->valorcampoprep(err) != "") {
                    queryupdate += separador1 + linea->nomcampo() + "=" + linea->valorcampoprep(err);
                    separador1 = ", ";
                } // end if
                if (err)
                    throw -1;
                if ((linea->valorcampoprep(err) != "NULL") && (linea->valorcampoprep(err) != "")) {
                    listcampos += separador + linea->nomcampo();
                    listvalores += separador + linea->valorcampoprep(err);
                    if (err)
                        throw -1;
                    separador = ", ";
                } // end if
                /// Si es el id entonces lo asignamos pq ya tiene el valor correspondiente.
                if (m_campoid == linea->nomcampo()) {
                    id = linea->valorcampo();
                } // end if
            } // end if
        } // end for
        if (m_nuevoCampo) {
            QString query = "INSERT INTO " + m_tablename + " (" + listcampos + ") VALUES (" + listvalores + ")";
            m_conexionbase->ejecuta(query);
            _depura(query, 0);
            cursor2 *cur = m_conexionbase->cargacursor("SELECT " + m_campoid + " FROM " + m_tablename + " ORDER BY " + m_campoid + " DESC LIMIT 1");
            id = cur->valor(m_campoid);
            delete cur;
        } else {
            QString query = "UPDATE " + m_tablename + " SET " + queryupdate + " WHERE " + querywhere;
            _depura(query, 0);
            m_conexionbase->ejecuta(query);
        } // end if
        m_nuevoCampo = FALSE;
        return 0;
    } catch (...) {
        _depura("EXCEPTION DBRecord::DBsave", 0);
        throw (-1);
    } // end try
    _depura("END DBRecord::DBSave", 0);
}


int DBRecord::setDBvalue(QString nomb, QString valor) {
    _depura("DBRecord::setDBvalue", 0);
    DBCampo *linea;
    int error = 0;
    int i = 0;
    linea = m_lista.value(i);
    while (linea && linea->nomcampo() != nomb)
        linea = m_lista.value(++i);
    if (!linea) {
        _depura("Campo " + nomb + " no encontrado", 2);
        return -1;
    } // end if
    if (linea->nomcampo() == nomb)
        error = linea->set
                (valor);
    return error;
}


QString DBRecord::DBvalue(QString nomb) {
    _depura("DBRecord::value", 0);
    DBCampo *linea;
    int i = 0;
    linea = m_lista.value(i);
    while (linea && linea->nomcampo() != nomb)
        linea = m_lista.value(++i);
    if (!linea) {
        _depura("Campo " + nomb + " no encontrado", 2);
        return "";
    } // end if
    if (linea->nomcampo() == nomb) {
        return linea->valorcampo();
    }
    return "";
}


QString DBRecord::DBvalueprep(QString nomb) {
    _depura("DBRecord::DBvalueprep", 0);
    DBCampo *linea;
    int i = 0;
    linea = m_lista.value(i);
    while (linea && linea->nomcampo() != nomb)
        linea = m_lista.value(++i);
    if (!linea) {
        _depura("Campo " + nomb + " no encontrado", 2);
        return "";
    } // end if
    if (linea->nomcampo() == nomb) {
        int err;
        return linea->valorcampoprep(err);
    }
    return "";
}


int DBRecord::addDBCampo(QString nom, DBCampo::dbtype typ, int res, QString nomp = "") {
    _depura("DBRecord::addDBCampo", 0);
    DBCampo *camp = new DBCampo(m_conexionbase, nom, typ, res, nomp);
    camp->set("");
    m_lista.append(camp);
    _depura("END DBRecord::addDBCampo", 0);
    return 0;
}


int DBRecord::borrar() {
    _depura("DBRecord::borrar", 0);
    try {
        DBCampo *linea;
        QString separadorwhere = "";
        QString querywhere = "";

        for (int i = 0; i < m_lista.size(); ++i) {
            linea = m_lista.at(i);
            if (linea->restrictcampo() & DBCampo::DBDupPrimaryKey) {
                int err;
                QString lin = linea->valorcampoprep(err);
                if (err)
                    throw -1;
                querywhere += separadorwhere + linea->nompresentacion() + " = " + lin;
                separadorwhere = " AND ";
            } // end if

            if (!(linea->restrictcampo() & DBCampo::DBNoSave)) {
                if (linea->restrictcampo() & DBCampo::DBPrimaryKey) {
                    int err;
                    QString lin = linea->valorcampoprep(err);
                    if (err)
                        throw -1;
                    querywhere += separadorwhere + linea->nomcampo() + " = " + lin;
                    separadorwhere = " AND ";
                } // end if
            } // end if
        } // end for

        if (m_nuevoCampo == FALSE) {
            m_conexionbase->ejecuta("DELETE FROM " + m_tablename + " WHERE " + querywhere);
        } // end if

        _depura("END DBRecord::borrar", 0);
        return 0;
    } catch (...) {
        mensajeInfo("se produjo un error al borrar el elemento");
	_depura("DBRecord::borrar() Error al borrar elemento", 3);
	throw -1;
    }
}


int DBRecord::guardar() {
    _depura("DBRecord::guardar", 0);
    QString id;
    try {
        DBsave(id);
        setDBvalue(m_campoid, id);
        _depura("END DBRecord::guardar", 0);
        return 0;
    } catch (...) {
        mensajeInfo("DBRecord::guardar error en el guardado");
        throw -1;
    } // end try
}


/// Esta funcion carga un articulo.
int DBRecord::cargar(QString id) {
    _depura("DBRecord::cargar", 0);
    QString query = "SELECT * FROM " + m_tablename + " WHERE " + m_campoid + " = " + id;
    cursor2 * cur = m_conexionbase->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;
    _depura("END DBRecord::cargar", 0);
    return 0;
}

