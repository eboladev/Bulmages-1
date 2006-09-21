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

#ifndef FACTURASPROVEEDORLIST_H
#define FACTURASPROVEEDORLIST_H

#include "busquedaproveedor.h"
#include "busquedaarticulo.h"
#include "company.h"
#include "funcaux.h"
#include "subform2bf.h"


/// Administra el detalle del listado de facturas de proveedor.
/** */
class FacturasProveedorListSubform : public SubForm2Bf {
    Q_OBJECT

public:
    FacturasProveedorListSubform(QWidget *parent = 0);
    ~FacturasProveedorListSubform() {}
    ;

public slots:
    virtual void cargar() {
        _depura("AlbaranesProveedorListSubform::cargar\n", 0);
        QString SQLQuery = "SELECT * FROM facturap";
        cursor2 * cur= companyact()->cargacursor(SQLQuery);
        SubForm3::cargar(cur);
        delete cur;
    };
    virtual void cargar(QString query) {
        SubForm3::cargar(query);
    };
};


#include "ui_facturasplistbase.h"


/// Administra el listado de facturas de proveedor.
/** */
class FacturasProveedorList : public QWidget, public Ui_FacturasProveedorListBase {
    Q_OBJECT

private:
    company *m_companyact;
    /// m_modo == 0 es modo edicion.
    /// m_modo == 1 es modo selector.
    int m_modo;
    QString mdb_idfacturap;

public:
    FacturasProveedorList(QWidget *parent = 0, Qt::WFlags flag = 0);
    FacturasProveedorList(company *,QWidget *parent = 0);
    ~FacturasProveedorList();
    void presenta();
    void setcompany (company *comp) {
        m_companyact = comp;
        m_proveedor->setcompany(comp);
        m_articulo->setcompany(comp);
        mui_list->setcompany(comp);
    };
    void meteWindow(QString nom, QObject *obj) {
        if (m_companyact != NULL) {
            m_companyact->meteWindow(nom, obj);
        }
    };
    void modoseleccion() {
        m_modo = 1;
    };
    void modoedicion() {
        m_modo = 0;
    };
    QString idfacturap() {
        return mdb_idfacturap;
    };
    void hideBusqueda() {
        m_busqueda->hide();
    };
    void showBusqueda() {
        m_busqueda->show();
    };
    void hideBotonera() {
        m_botonera->hide();
    };
    void showBotonera() {
        m_botonera->show();
    };
    void setidproveedor(QString val) {
        m_proveedor->setidproveedor(val);
    };
    void setidarticulo(QString val) {
        m_articulo->setidarticulo(val);
    };
    QString generaFiltro();
    /// Funciones que se encargan de guardar y cargar la configuracion del listado.
    void guardaconfig();
    void cargaconfig();
    void editar(int);

public slots:
    virtual void on_m_filtro_textChanged(const QString &text) {
        if(text.size() >=3)
            on_mui_actualizar_clicked();
    };
    void on_mui_list_itemDoubleClicked(QTableWidgetItem *) {
        on_mui_editar_clicked();
    };
    virtual void on_mui_actualizar_clicked() {
        presenta();
    };
    virtual void on_mui_crear_clicked() {
        m_companyact->s_newFacturaPro();
    };
    virtual void on_mui_editar_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_imprimir_clicked();
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

