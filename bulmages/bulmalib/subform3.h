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
#include "fixed.h"


/// SubForm3, constructor de la clase base para subformularios.
class SubForm3: public QWidget, public Ui_SubForm3Base {
    Q_OBJECT

private:
    QString m_tablename; /// Nombre de la tabla en la BD.

protected:
    QList<SHeader *> m_lcabecera; /// Lista de encabezados del listado. Coordenadas X de la tabla.
    QList<SDBRecord *> m_lista; /// Filas y contenido. Coordenadas Y de la tabla.
    QList<SDBRecord *> m_listaborrar; /// Lista de subformularios marcados para ser borrados.
    postgresiface2 *m_companyact; /// Clase padre y acceso a base de datos.
    QString m_fileconfig; /// Fichero de configuracion del subformulario.
    QString m_campoid; /// Campo Identificador de la tabla en la BD.
    bool m_insercion; /// Indica si se pueden insertar nuevas filas o no.
    bool m_primero; /// Indica si ya se ha cargado la configuracion o no.
    bool m_delete;

protected:
    void guardaconfig();
    void cargaconfig();
    void pintaCabeceras();
    QList<SDBRecord *> *lista() {
        return &m_lista;
    };

public:
    bool campoCompleto(int row);
    void setDelete(bool f) {
        m_delete = f;
    };
    SubForm3(QWidget *parent);
    virtual ~SubForm3();
    SDBCampo *item(int row, int col) {
        return (SDBCampo *) mui_list->item(row, col);
    };
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
        mui_list->setColumnWidth(i, j);
    };
    void hideColumn(int i) {
        mui_list->hideColumn(i);
    };
    int currentRow() {
        return mui_list->currentRow();
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
    void sortItems(int col, Qt::SortOrder orden);
    void setinsercion(bool b) {
        m_insercion = b;
    };
    void setcompany(postgresiface2 *c) {
        m_companyact = c;
    };
    postgresiface2 *companyact() {
        return m_companyact;
    };
    void setDBTableName(QString nom) {
        m_tablename = nom;
        m_fileconfig = nom;
    };
    void setFileConfig(QString nom) {
        m_fileconfig = nom;
    };
    void setDBCampoId(QString nom) {
        m_campoid = nom;
    };
    void clear() {
        mui_list->clear();
    };
    void setRowCount(int i) {
        mui_list->setRowCount(i);
    };
    void setColumnCount(int i) {
        mui_list->setColumnCount(i);
    };
    void situarse(unsigned int, unsigned int);
    void situarse1(unsigned int, unsigned int);
    int addSHeader(QString nom, DBCampo::dbtype typ, int res, int opt, QString nomp);
    SDBRecord *newSDBRecord();
    void setColumnValue(QString, QString);
    QString DBvalue(QString campo, int row = -1);
    SDBRecord *lineaact();
    SDBRecord *lineaat(int row);
    virtual void imprimirPDF(const QString &);
    virtual void nuevoRegistro();
    virtual int borrar();
    virtual int borrar(int);
    virtual int guardar();
    virtual void cargar(cursor2 *cur);
    virtual void cargar(QString query);
    virtual int inicializar();
    virtual void pintar();
    virtual void creaMenu(QMenu *) {
        _depura("SubForm3:: CreaMenu, funcion para ser sobreescrita", 0);
    };
    virtual void procesaMenu(QAction *) {
        _depura("SubForm3:: procesaMenu, funcion para ser sobreescrita", 0);
    };
    virtual Fixed sumarCampo(QString);

public slots:
    virtual void contextMenuEvent(QContextMenuEvent *);
    virtual void on_mui_confcol_clicked();
    virtual void on_mui_confquery_clicked();
    virtual void on_mui_appag_clicked() {
        on_mui_confquery_clicked();
    };
    virtual void on_mui_list_editFinished(int row, int col, int key);
    virtual void on_mui_list_pressedSlash(int row, int col);
    virtual void on_mui_list_pressedAsterisk(int row, int col);
    virtual void on_mui_list_pressedPlus(int row, int col);
    virtual void on_mui_list_pressedMinus(int row, int col);
    virtual void on_mui_list_ctrlSubir(int row, int col);
    virtual void on_mui_list_ctrlBajar(int row, int col);
    virtual void on_mui_pagsiguiente_clicked();
    virtual void on_mui_paganterior_clicked();
    virtual void on_mui_list_itemDoubleClicked(QTableWidgetItem *item);
    virtual void on_mui_list_cellDoubleClicked(int row, int col);
    virtual void toogleConfig() {
        mui_configurador->setVisible(mui_configurador->isHidden());
        emit toogledConfig(mui_configurador->isVisible());
    };
    virtual void on_mui_botonCerrar_clicked() {
        toogleConfig();
    };
    virtual void on_mui_list_cellChanged(int, int);
    virtual void on_mui_list_itemChanged(QTableWidgetItem *it);
    virtual void on_mui_list_currentCellChanged(int, int, int, int );


signals:
    void toogledConfig(bool);
    void editFinish(int, int);
    void itemDoubleClicked(QTableWidgetItem *);
    void cellDoubleClicked(int row, int col);
    void pintaMenu(QMenu *);
    void trataMenu(QAction *);
};

#endif

