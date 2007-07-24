/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef DBRECORD_H
#define DBRECORD_H

#include <QString>
#include <QList>

#include "postgresiface2.h"
#include "funcaux.h"


class DBCampo {
public:
    enum dbtype {DBint = 1, DBvarchar = 2, DBdate = 3, DBnumeric = 4, DBboolean = 5};
    enum dbrestrict {DBNothing = 0, DBNotNull = 1, DBPrimaryKey = 2,
                     DBNoSave = 4, DBAuto = 8, DBDupPrimaryKey = 16, DBRequired = 32, DBNoLoad = 64};

private:
    QString m_nomcampo;
    QString m_valorcampo;
    QString m_nompresentacion;
    int m_restrict;
    dbtype m_tipo;
    postgresiface2 *m_conexionbase;
    /// indica el valor del campo en un estado anterior para determinar si ha habido cambios.
    QString m_valorcampoorig;

public:
    bool cambiado();
    void resetCambio();
    DBCampo(postgresiface2 *com, QString nom, dbtype typ, int res, QString nomp = "");
    virtual ~DBCampo();
    postgresiface2 *conexionbase();
    void setconexionbase(postgresiface2 *comp);
    dbtype tipo();
    virtual int set(QString val);
    int restrictcampo();
    QString nomcampo();
    QString nompresentacion();
    QString valorcampo();
    QString valorcampoprep(int &error);
};


class DBRecord {
protected:
    QList<DBCampo *> m_lista;
    postgresiface2 *m_conexionbase;
    QString m_tablename;
    QString m_campoid;
    bool m_nuevoCampo;

public:
    DBRecord(postgresiface2 *);
    virtual ~DBRecord();
    void setconexionbase(postgresiface2 *comp);
    postgresiface2 *conexionbase();
    int DBload(cursor2 *);
    virtual int DBsave(QString &id);
    virtual int setDBvalue(QString, QString);
    QString DBvalue(QString);
    bool exists(QString);
    QString DBvalueprep(QString);
    void setDBTableName(QString nom);
    void setNuevo(bool n);
    QString tableName();
    QString campoId();
    void setDBCampoId(QString nom);
    int addDBCampo(QString, DBCampo::dbtype, int, QString);
    void DBclear();
    QList<DBCampo *> *lista();
    virtual int borrar();
    virtual int guardar();
    virtual void vaciar();
    virtual void imprimir();
    virtual int cargar(QString);
};

#endif

