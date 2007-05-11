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

#ifndef PEDIDOSPROVEEDORLIST_H
#define PEDIDOSPROVEEDORLIST_H

#include "busquedaproveedor.h"
#include "busquedaarticulo.h"
#include "company.h"
#include "funcaux.h"
#include "subform2bf.h"
#include "fichabf.h"


/// Administra las l&iacute;neas de detalle de pedidos a proveedor.
/** */
class PedidosProveedorListSubform : public SubForm2Bf {
    Q_OBJECT

public:
    PedidosProveedorListSubform(QWidget *parent = 0);
    ~PedidosProveedorListSubform() {}
    virtual void cargar() {
        _depura("PedidosProveedorListSubform::cargar", 0);
        QString SQLQuery = "SELECT * FROM pedidoproveedor";
        cursor2 *cur = empresaBase()->cargacursor(SQLQuery);
        SubForm3::cargar(cur);
        delete cur;
    }
    virtual void cargar(QString query) {
        SubForm3::cargar(query);
    }
};


#include "ui_pedidosproveedorlistbase.h"


/// Muestra y administra la ventana con la informaci&oacute;n de los pedidos a proveedor.
/** */
class PedidosProveedorList : public FichaBf, public Ui_PedidosProveedorListBase {
    Q_OBJECT

private:
    /// m_modo == 0 es modo edicion.
    /// m_modo == 1 es modo selector.
    int m_modo;
    QString mdb_idpedidoproveedor;

public:
    PedidosProveedorList(QWidget *parent = 0, Qt::WFlags flag = 0);
    PedidosProveedorList(company *, QWidget *parent = 0, Qt::WFlags flag = 0);
    ~PedidosProveedorList();
    void presenta();
    void modoseleccion() {
        m_modo = 1;
    }
    void modoedicion() {
        m_modo = 0;
    }
    void imprimir();
    void setEmpresaBase(company *comp) {
        FichaBf::setEmpresaBase(comp);
        m_proveedor->setEmpresaBase(comp);
        mui_list->setEmpresaBase(comp);
    }
    void hideBotonera() {
        m_botonera->hide();
    }
    void showBotonera() {
        m_botonera->show();
    }
    void hideBusqueda() {
        m_busqueda->hide();
    }
    void showBusqueda() {
        m_busqueda->show();
    }
    QString idpedidoproveedor() {
        return mdb_idpedidoproveedor;
    }
    void setidproveedor(QString val) {
        m_proveedor->setidproveedor(val);
    }
    void meteWindow(QString nom, QObject *obj) {
        if (empresaBase() != NULL) {
            empresaBase()->meteWindow(nom, obj);
        } // end if
    }
    QString generarFiltro();
    void editar(int);
    virtual void on_mui_borrar_clicked();

public slots:
    virtual void on_m_filtro_textChanged(const QString &text) {
        if (text.size() >= 3) {
            on_mui_actualizar_clicked();
        } // end if
    }
    void on_mui_list_itemDoubleClicked(QTableWidgetItem *) {
        on_mui_editar_clicked();
    }
    virtual void on_mui_imprimir_clicked() {
        imprimir();
    }
    virtual void on_mui_actualizar_clicked() {
        presenta();
    }
    virtual void on_mui_crear_clicked() {
        empresaBase()->s_newPedidoPro();
    }
    virtual void s_filtrar() {
        presenta();
    }
    virtual void on_mui_editar_clicked();
    virtual void on_mui_configurar_toggled(bool checked) {
        if (checked) {
            mui_list->showConfig();
        } else {
            mui_list->hideConfig();
        } // end if
    }

signals:
    void selected(QString);
};

#endif

