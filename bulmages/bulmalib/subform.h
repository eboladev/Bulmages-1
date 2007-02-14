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

#ifndef SUBFORM_H
#define SUBFORM_H

#include <Qt>

#include "postgresiface2.h"
#include "funcaux.h"
#include "qtable2.h"
#include "dbrecord.h"


class SHeader {
public:
    enum dboptions {DBNone = 0, DBReadOnly = 1, DBNoView = 2, DBNoWrite = 4, DBBlockView = 8};

protected:
    QString m_nomcampo;
    QString m_valorcampo;
    QString m_nompresentacion;
    unsigned int m_restricciones;
    DBCampo::dbtype m_tipo;
    unsigned int m_options;

public:
    SHeader(QString nom, DBCampo::dbtype typ, int res, int opt, QString nomp = "");
    ~SHeader() {
        _depura("SHeader::~SHeader", 0);
    };
    int set(QString val) {
        m_valorcampo = val;
        return 0;
    };
    unsigned int options() {
        return m_options;
    };
    unsigned int restricciones() {
        return m_restricciones;
    };
    DBCampo::dbtype tipo() {
        return m_tipo;
    };
    QString nompresentacion() {
        return m_nompresentacion;
    };
    int restrictcampo() {
        return m_restricciones;
    };
    QString nomcampo() {
        return m_nomcampo;
    };
};


class SDBRecord: public DBRecord {
public:
    SDBRecord(postgresiface2 *con);
    ~SDBRecord();
    int addDBCampo(QString nom, DBCampo::dbtype typ, int res, QString nomp = "");
    void refresh();
    virtual int DBsave(QString &);
};


class SDBCampo: public QTableWidgetItem2, public DBCampo {
private:
    SDBRecord *m_pare;

public:
    SDBRecord *pare() {
        return m_pare;
    };
    SDBCampo(SDBRecord *par, postgresiface2 *com, QString nom, dbtype typ, int res, QString nomp = "");
    virtual ~SDBCampo();
    int addDBCampo(QString nom, DBCampo::dbtype typ, int res, QString nomp = "");
    virtual int set(QString val);
    void refresh();
    virtual bool operator< (const QTableWidgetItem &other) const;
};

#endif

