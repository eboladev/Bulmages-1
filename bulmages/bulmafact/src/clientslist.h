/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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

#ifndef clientslist_H
#define clientslist_H

#include "pgimportfiles.h"
#include "company.h"
#include "subform2bf.h"


class ClienteListSubform : public SubForm2Bf {
    Q_OBJECT

public:
    ClienteListSubform(QWidget *parent = 0, const char *name = 0);
    ~ClienteListSubform() {}
    ;
};

#include "ui_clientslistbase.h"


class ClientsList : public QWidget, public Ui_ClientsListBase, public pgimportfiles {
    Q_OBJECT

public:
    enum edmode
    {
        EditMode = 0,
        SelectMode = 1
    };

private:
    edmode m_modo;
    QString mdb_idcliente;
    QString mdb_nomcliente;
    QString mdb_cifcliente;

public:
    company *m_companyact;

public:
    ClientsList(company *, QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmode = EditMode);
    ~ClientsList();
    void selectMode() {
        m_modo = SelectMode;
    };
    void editMode() {
        m_modo = EditMode;
    };
    void presenta();
    QString idclient() {
        return mdb_idcliente;
    };
    QString nomclient() {
        return mdb_nomcliente;
    };
    QString cifclient() {
        return mdb_cifcliente;
    };
    void hideBotonera() {
        m_botonera->hide();
    };
    void showBotonera() {
        m_botonera->show();
    };
    void hideBusqueda() {
        m_busqueda->hide();
    };
    void showBusqueda() {
        m_busqueda->show();
    };
    void editar(int);

public slots:
    virtual void on_m_filtro_textChanged(const QString &text) {
        if (text.size() >= 3)
            on_mui_actualizar_clicked();
    };
    void on_mui_list_itemDoubleClicked(QTableWidgetItem *) {
        on_mui_editar_clicked();
    };
    virtual void on_mui_crear_clicked() {
        m_companyact->s_newClienteView();
    };
    virtual void on_mui_editar_clicked();
    virtual void on_mui_imprimir_clicked();
    virtual void on_mui_informeclientes_clicked();
    virtual void on_mui_actualizar_clicked() {
        presenta();
    };
    virtual void on_mui_exportar_clicked();
    virtual void on_mui_importar_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_configurar_toggled(bool checked) {
        if (checked)
            mui_list->showConfig();
        else
            mui_list->hideConfig();
    };

signals:
    void selected(QString);
};

#endif

