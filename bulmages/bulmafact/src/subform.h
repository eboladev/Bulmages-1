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
    enum dboptions {DBNone=0, DBReadOnly=1, DBNoView=2, DBNoWrite=4};
protected:
    QString m_nomcampo;
    QString m_valorcampo;
   QString m_nompresentacion;
    unsigned int   m_restricciones;
    DBCampo::dbtype m_tipo;
    unsigned int m_options;
public:
    SHeader(QString nom, DBCampo::dbtype typ, int res, int opt, QString nomp="");
    ~SHeader() {}
    ;
    int set
        (QString val) {
        m_valorcampo=val;
        return 0;
    };
    unsigned int options() {return m_options;};
    unsigned int restricciones() {return m_restricciones;};
    DBCampo::dbtype tipo() {return m_tipo;};
    QString nompresentacion() {return m_nompresentacion;};

    int restrictcampo() {
        return m_restricciones;
    };
    QString nomcampo() {
        return m_nomcampo;
    };
};


class SDBCampo: public QTableWidgetItem2, public DBCampo {
public:
    SDBCampo(postgresiface2 *com, QString nom, dbtype typ, int res, QString nomp=""): QTableWidgetItem2(), DBCampo(com,  nom,  typ,  res,  nomp) {}
    ;
    virtual ~SDBCampo() {}
    ;
    int addDBCampo(QString nom, DBCampo::dbtype typ, int res, QString nomp="");
    virtual int set(QString val);
    void refresh();
};


class SDBRecord: public DBRecord {
public:
    SDBRecord(postgresiface2 *con) : DBRecord(con) {}
    ;
    ~SDBRecord() {}
    ;
    int addDBCampo(QString nom, DBCampo::dbtype typ, int res, QString nomp="");
    void refresh();
};


class SubForm2: public QTableWidget {
Q_OBJECT
private:
    Q3PtrList<SHeader>   m_lcabecera;
    Q3PtrList<SDBRecord> m_lista;
    postgresiface2 *m_companyact;
    QString m_tablename;
    QString m_campoid;
    bool m_insercion;
    bool m_primero;
    void guardaconfig();
    void cargaconfig();
    void pintaCabeceras();
public:
    SubForm2(QWidget *parent);
    virtual ~SubForm2() {
        guardaconfig();
    };

    void sortItems(int col, Qt::SortOrder orden);

    void setinsercion(bool b) {
        m_insercion=b;
    };

    void setcompany(postgresiface2 *c) {
        m_companyact = c;
    };

    postgresiface2 *companyact() {
        return m_companyact;
    };
    void setDBTableName(QString nom) {
        m_tablename=nom;
    };
    void setDBCampoId(QString nom) {
        m_campoid = nom;
    };

    void situarse(unsigned int, unsigned int);
    Q3PtrList<SDBRecord> *lista(){return &m_lista;};
    int addSHeader(QString nom, DBCampo::dbtype typ, int res, int opt, QString nomp);
    SDBRecord *newSDBRecord();

    void setColumnValue(QString, QString);
    QString DBvalue(QString campo, int row=-1);
    SDBRecord *SubForm2::lineaact();
    SDBRecord *SubForm2::lineaat(int row);

    virtual void nuevoRegistro();
    virtual int borrar();
    virtual int borrar(int);
    virtual int guardar();
    virtual int cargar(cursor2 *cur);
    virtual void pintar();
    virtual bool eventFilter(QObject *obj, QEvent *ev);
    virtual void pressedSlash(int row, int col);
    virtual void pressedAsterisk(int row, int col);
    virtual void pressedPlus(int row, int col);
    virtual void editFinished(int row, int col);
public slots:
	void sortByColumn(int col);
signals:
	void editFinish(int, int);
};

#endif

