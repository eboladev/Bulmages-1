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
#include "fichabf.h"

/// Administra el detalle del listado de pagos.
/** */
class PagosListSubForm : public SubForm2Bf {
    Q_OBJECT

public:
    PagosListSubForm(QWidget *parent = 0);
    ~PagosListSubForm() {}
    ;
};


#include "ui_pagoslistbase.h"


/// Muestra y administra el listado de pagos.
/** */
class PagosList : public FichaBf, private Ui_PagosListBase {
    Q_OBJECT

private:
    /// m_modo == 0 es modo edicion.
    /// m_modo == 1 es modo selector.
    int m_modo;
    QString mdb_idpago;

public:
    PagosList(QWidget *parent = 0, Qt::WFlags flag = 0);
    PagosList(company *comp = NULL, QWidget *parent = 0, Qt::WFlags flag = 0);
    ~PagosList();
    void presentar();
    void modoseleccion();
    void modoedicion();
    void setEmpresaBase (company *comp);
    QString idpago();
    void hideBotonera();
    void showBotonera();
    void hideBusqueda();
    void showBusqueda();
    void imprimir();
    void setidproveedor(QString val);
    QString generaFiltro();


public slots:
    virtual void on_mui_list_cellDoubleClicked(int, int);
    virtual void on_mui_list_customContextMenuRequested(const QPoint &);
    virtual void on_mui_editar_clicked();
    virtual void on_mui_crear_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_imprimir_clicked();
    virtual void on_mui_actualizar_clicked();
    virtual void on_mui_configurar_toggled(bool checked);
};

#endif

