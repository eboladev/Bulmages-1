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

#ifndef PEDIDOSCLIENTELIST_H
#define PEDIDOSCLIENTELIST_H

#include "busquedacliente.h"
#include "busquedaarticulo.h"
#include "company.h"
#include "funcaux.h"
#include "subform2bf.h"

class PedidosClienteListSubform : public SubForm2Bf {
    Q_OBJECT

public:
    PedidosClienteListSubform(QWidget *parent = 0, const char *name = 0);
    ~PedidosClienteListSubform() {}
    ;

public slots:
    virtual void cargar() {
        _depura("PedidosClienteListSubform::cargar\n", 0);
        QString SQLQuery = "SELECT * FROM pedidocliente";
        cursor2 * cur= companyact()->cargacursor(SQLQuery);
        SubForm3::cargar(cur);
        delete cur;
    };
    virtual int cargar(cursor2 *cur) {
        _depura("PedidosClienteListSubform::cargar\n", 0);
        SubForm3::cargar(cur);
        return 0;
    };
};


#include "ui_pedidosclientelistbase.h"


class PedidosClienteList : public QWidget, public Ui_PedidosClienteListBase {
    Q_OBJECT

private:
    company *companyact;
    /// m_modo == 0 es modo edicion.
    /// m_modo ==  es modo selector.
    int m_modo;
    QString m_idpedidocliente;

public:
    PedidosClienteList(QWidget *parent = 0, Qt::WFlags flag = 0);
    PedidosClienteList(company *, QWidget *parent = 0, Qt::WFlags flag = 0);
    ~PedidosClienteList();
    void presenta();
    void modoseleccion() {
        m_modo = 1;
    };
    void modoedicion() {
        m_modo = 0;
    };
    void imprimir();
    void setcompany(company *comp) {
        companyact = comp;
        m_cliente->setcompany(comp);
        mui_list->setcompany(comp);
    };
    void hideBotonera() {
        m_botonera->hide();
    };
    void showBotonera() {
        m_botonera->show();
    };
    void hideBusqueda() {
        _depura("Ocultar busqueda.", 0);
        m_busqueda->hide();
    };
    void showBusqueda() {
        m_busqueda->show();
    };
    QString idpedidocliente() {
        return m_idpedidocliente;
    };
    void setidcliente(QString val) {
        m_cliente->setidcliente(val);
    };
    void meteWindow(QString nom, QObject *obj) {
        if (companyact != NULL) {
            companyact->meteWindow(nom, obj);
        }
    };
    void editar(int);
    QString generarFiltro();

public slots:
    virtual void on_m_filtro_textChanged(const QString &text) {
	if(text.size() >=3) 
		on_mui_actualizar_clicked();
    };
    void on_mui_list_itemDoubleClicked( QTableWidgetItem *) {
        on_mui_editar_clicked();
    };
    virtual void on_mui_imprimir_clicked() {
        imprimir();
    };
    virtual void on_mui_crear_clicked() {
        companyact->s_newPedidoClienteView();
    };
    virtual void on_mui_actualizar_clicked() {
        presenta();
    };
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_editar_clicked();
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

