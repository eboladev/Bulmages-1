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


#ifndef SUBFORM3_H
#define SUBFORM3_H

#include <Qt>
#include <QWidget>

#include "ui_subform3base.h"
#include "postgresiface2.h"
#include "funcaux.h"
#include "qtable2.h"
#include "subform.h"


class SubForm3: public QWidget, public Ui_SubForm3Base {
    Q_OBJECT
private:
    QList<SHeader *>   m_lcabecera;
    QList<SDBRecord *> m_lista;
    postgresiface2 *m_companyact;
    QString m_tablename;
    QString m_campoid;
    bool m_insercion;
    bool m_primero;

private:
    void guardaconfig();
    void cargaconfig();
    void pintaCabeceras();
    QList<SDBRecord *> *lista() {
        return &m_lista;
    };

public:

    int columnCount() {
        return mui_list->columnCount();
    };
    void showColumn(int i) {
        mui_list->showColumn(i);
    };
    int columnWidth(int i) {
        return mui_list->columnWidth(i);
    };
    void setColumnWidth(int i, int j) {
        mui_list->setColumnWidth(i,j);
    };
    void hideColumn(int i) {
        mui_list->hideColumn(i);
    };
    int  currentRow() {
        return mui_list->currentRow();
    };
    QTableWidgetItem *item(int i, int j) {
        return item(i,j);
    };
    int rowCount() {
        return mui_list->rowCount();
    };
    int currentColumn() {
        return mui_list->currentColumn();
    };
    void resizeColumnsToContents() {
        mui_list->resizeColumnsToContents();
    };
    void resizeRowsToContents() {
        mui_list->resizeRowsToContents();
    };
    void resizeColumnToContents(int i) {
        mui_list->resizeColumnToContents(i);
    };
    void resizeRowToContents(int i) {
        mui_list->resizeRowToContents(i);
    };
    void hideConfig() {
        mui_configurador->hide();
    };
    void showConfig() {
        mui_configurador->show();
    };

    QString imprimir();
    SubForm3(QWidget *parent);
    virtual ~SubForm3() {
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
    int addSHeader(QString nom, DBCampo::dbtype typ, int res, int opt, QString nomp);
    SDBRecord *newSDBRecord();

    void setColumnValue(QString, QString);
    QString DBvalue(QString campo, int row=-1);
    SDBRecord *lineaact();
    SDBRecord *lineaat(int row);

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
    virtual void on_mui_confcol_clicked();
    virtual void on_mui_confquery_clicked();
signals:
    void editFinish(int, int);
};

#endif

