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

#include <QKeyEvent>
#include <QEvent>
#include <QFile>

#include "subform.h"


SDBRecord::SDBRecord(postgresiface2 *con) : DBRecord(con) {
    static int creaciones = 0;
    creaciones ++;
    _depura("SDBrecord creados:" + QString::number(creaciones), 0);
}


SDBRecord::~SDBRecord() {
    static int destrucciones = 0;
    _depura("SDBRecord::~SDBRecord", 0);
    destrucciones++;
    _depura("SDBrecord destruidos:" + QString::number(destrucciones), 0);
}


int SDBRecord::DBsave(QString &id) {
    refresh();
    return DBRecord::DBsave(id);
}


void SDBRecord::refresh() {
    _depura("SDBRecord::refresh", 0);
    SDBCampo *camp;
    for (int i = 0; i < m_lista.size(); ++i) {
        camp = (SDBCampo *) m_lista.at(i);
        camp->refresh();
    } // end for
    _depura("END SDBRecord::refresh", 0);
}


int SDBRecord::addDBCampo(QString nom, DBCampo::dbtype typ, int res, QString nomp) {
    _depura("SDBRecord::addDBCampo", 0);
    SDBCampo *camp = new SDBCampo(this, m_conexionbase, nom, typ, res, nomp);
    camp->set("");
    m_lista.append(camp);
    _depura("END SDBRecord::addDBCampo", 0);
    return 0;
}


SDBCampo::SDBCampo(SDBRecord *par, postgresiface2 *com, QString nom, dbtype typ, int res, QString nomp)
        : QTableWidgetItem2(), DBCampo(com, nom, typ, res, nomp) {
    static int creaciones = 0;
    creaciones++;
    _depura("SDBCampo creados:", 0, QString::number(creaciones));
    m_pare = par;
}


SDBCampo::~SDBCampo() {
    _depura("SDBCampo::~SDBCampo()", 0);
    static int destrucciones = 0;
    destrucciones++;
    _depura("SDBCampo destruidos:", 0, QString::number(destrucciones));
}


void SDBCampo::refresh() {
    _depura("SDBCampo::refresh", 0);
    if (tipo() == DBCampo::DBboolean) {
        DBCampo::set
            (checkState() == Qt::Checked ? "TRUE" : "FALSE");
    } else {
        DBCampo::set(text());
    } // end if
    _depura("END SDBCampo::refresh", 0);
}


int SDBCampo::set
    (QString val) {
    _depura("SDBCampo::set", 0);
    if (tipo() == DBCampo::DBboolean) {
        if (val == "TRUE" || val == "t")
            setCheckState(Qt::Checked);
        else
            setCheckState(Qt::Unchecked);
    } else {
        QTableWidgetItem2::setText(val);
    } // end if
    DBCampo::set(val);
    _depura("END SDBCampo::set", 0);
    return 0;
}


SHeader::SHeader(QString nom, DBCampo::dbtype typ, int res, int opt, QString nomp) {
    m_nomcampo = nom;
    m_valorcampo = "";
    m_nompresentacion = nomp;
    m_restricciones = res;
    m_options = opt;
    m_tipo = typ;
}

