/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef PAGOSLIST_H
#define PAGOSLIST_H


#include <QLineEdit>

#include "company.h"
#include "busquedaproveedor.h"
#include "funcaux.h"
#include "subform2bf.h"


class PagosListSubForm : public SubForm2Bf {
    Q_OBJECT

public:
    PagosListSubForm(QWidget *parent = 0, const char *name = 0);
    ~PagosListSubForm() {}
    ;
};


#include "ui_pagoslistbase.h"

class PagosList : public QWidget, private Ui_PagosListBase {
    Q_OBJECT

private:
    company *m_companyact;
    /// m_modo == 0 es modo edicion.
    /// m_modo == 1 es modo selector.
    int m_modo;
    QString mdb_idpago;

public:
    PagosList(QWidget *parent = 0, const char *name = 0, Qt::WFlags flag = 0);
    PagosList(company *comp = NULL, QWidget *parent = 0, const char *name = 0,
              Qt::WFlags flag = 0);
    ~PagosList();
    void presentar();
    void modoseleccion() {
        m_modo = 1;
    };
    void modoedicion() {
        m_modo = 0;
    };
    void setcompany (company *comp)	{
        m_companyact = comp;
        m_proveedor->setcompany(comp);
	mui_list->setcompany(comp);
    };
    QString idpago() {
        return mdb_idpago;
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
    void imprimir();
    void meteWindow(QString nom, QObject *obj) {
        if (m_companyact != NULL) {
            m_companyact->meteWindow(nom, obj);
        }
    };
    void setidproveedor(QString val) {
        m_proveedor->setidproveedor(val);
    };
    QString generaFiltro();

public slots:
    virtual void on_mui_list_cellDoubleClicked(int , int);
    virtual void on_mui_list_customContextMenuRequested(const QPoint &);
    virtual void on_mui_editar_clicked();
    virtual void on_mui_crear_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_imprimir_clicked() {
        imprimir();
    };
    virtual void on_mui_actualizar_clicked() {
        presentar();
    };
    virtual void on_mui_configurar_toggled(bool checked) {
        if (checked)
            mui_list->showConfig();
        else
            mui_list->hideConfig();
    };
};

#endif

