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

#ifndef FACTURASLIST_H
#define FACTURASLIST_H

#include "busquedacliente.h"
#include "busquedaarticulo.h"
#include "company.h"
#include "funcaux.h"
#include "subform2bf.h"
#include "fichabf.h"


/// Administra el detalle del listado de facturas a clientes.
/** */
class FacturasListSubform : public SubForm2Bf {
    Q_OBJECT

public:
    FacturasListSubform(QWidget *parent = 0, const char *name = 0);
    ~FacturasListSubform() {}

public slots:
    virtual void cargar() {
        _depura("FacturasListSubform::cargar\n", 0);
        QString SQLQuery = "SELECT * FROM factura";
        cursor2 *cur= empresaBase()->cargacursor(SQLQuery);
        SubForm3::cargar(cur);
        delete cur;
    }
    virtual void cargar(QString query) {
	SubForm3::cargar(query);
    }
};


#include "ui_facturaslistbase.h"


/// Administra el listado de facturas a clientes.
/** */
class FacturasList : public FichaBf, public Ui_FacturasListBase {
    Q_OBJECT

public:
    enum edmode
    {
        EditMode = 0,
        SelectMode = 1
    };

private:
    edmode m_modo;
    QString mdb_idfactura;

public:
    FacturasList(QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmodo = EditMode);
    FacturasList(company *,QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmodo = EditMode);
    ~FacturasList();
    void iniciaForm();
    void setEmpresaBase (company *comp) {
	PEmpresaBase::setEmpresaBase(comp);
        m_cliente->setEmpresaBase(comp);
        m_articulo->setEmpresaBase(comp);
        mui_list->setEmpresaBase(comp);
    };

    void meteWindow(QString nom, QObject *obj) {
        if (empresaBase() != NULL) {
            empresaBase()->meteWindow(nom, obj);
        }
    };
    company *getcompany() {
        return empresaBase();
    };
    int modo() {
        return m_modo;
    };
    void modoseleccion() {
        m_modo = SelectMode;
    };
    void modoedicion() {
        m_modo = EditMode;
    };
    QString idfactura() {
        return mdb_idfactura;
    };
    void hideBusqueda() {
        m_busqueda->hide();
    };
    void showBusqueda() {
        m_busqueda->show();
    };
    void setidcliente(QString val) {
        m_cliente->setidcliente(val);
    };
    void setidarticulo(QString val) {
        m_articulo->setidarticulo(val);
    };
    QString generaFiltro();

    void presenta();
    void editar(int);
    virtual void on_mui_borrar_clicked();

public slots:
    virtual void on_m_filtro_textChanged(const QString &text) {
    if (text.size() >= 3) 
		on_mui_actualizar_clicked();
    };
    void on_mui_list_itemDoubleClicked( QTableWidgetItem *) {
        on_mui_editar_clicked();
    };
    virtual void on_mui_actualizar_clicked() {
        presenta();
    }
    virtual void on_mui_crear_clicked() {
        empresaBase()->s_newFacturaCli();
    };
    virtual void on_mui_editar_clicked();
    virtual void on_mui_imprimir_clicked();
    virtual void on_mui_configurar_toggled(bool checked) {
        if (checked) {
            mui_list->showConfig();
        } else {
            mui_list->hideConfig();
        } // end if
    };

signals:
    void selected(QString);
};

#endif

