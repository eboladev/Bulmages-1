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

#ifndef ALBARANESPROVEEDOR_H
#define ALBARANESPROVEEDOR_H

#include <QLineEdit>
#include <Q3Table>


#include "company.h"
#include "busquedaproveedor.h"
#include "busquedaarticulo.h"
#include "funcaux.h"
#include "subform2bf.h"


class AlbaranesProveedorListSubform : public SubForm2Bf {
    Q_OBJECT
public:
    AlbaranesProveedorListSubform(QWidget *parent = 0, const char *name = 0);
    ~AlbaranesProveedorListSubform() {}
    ;
public slots:
    virtual void cargar() {
        _depura("AlbaranesProveedorListSubform::cargar\n",0);
        QString SQLQuery = "SELECT * FROM albaranp";
        cursor2 * cur= companyact()->cargacursor(SQLQuery);
        SubForm2::cargar(cur);
        delete cur;
    };
    virtual int cargar(cursor2 *cur) {
        _depura("AlbaranesProveedorListSubform::cargar\n",0);
        SubForm2::cargar(cur);
        return 0;
    };
};


#include "ui_albaranesproveedorlistbase.h"


class AlbaranesProveedor : public QWidget, public Ui_AlbaranesProveedorListBase {
    Q_OBJECT

private:
    company *m_companyact;
    /// == 0 es modo edicion
    /// == 1 es modo selector
    int m_modo;
    QString mdb_idalbaranp;

public:
    AlbaranesProveedor(QWidget *parent = 0, const char *name = 0, Qt::WFlags flag = 0);
    AlbaranesProveedor(company *comp = NULL, QWidget *parent = 0, const char *name = 0,
                       Qt::WFlags flag = 0);
    ~AlbaranesProveedor();
    void presenta();
    void modoseleccion() {
        m_modo = 1;
    };
    void modoedicion() {
        m_modo = 0;
    };
    void setcompany(company *comp) {
        m_companyact=comp;
        m_proveedor->setcompany(comp);
        m_articulo->setcompany(comp);
    };
    QString idalbaranp() {
        return mdb_idalbaranp;
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
    void hideConfiguracion() {
        m_configuracion->hide();
    };
    void showConfiguracion() {
        m_configuracion->show();
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
    void setidarticulo(QString val) {
        m_articulo->setidarticulo(val);
    };
    QString generaFiltro();

    /// Funciones que se encarga en guardar y cargar la configuracion del listado.
    void guardaconfig();
    void cargaconfig();
    void editar(int);

public slots:
    void on_mui_list_itemDoubleClicked( QTableWidgetItem *item) {
        on_mui_editar_clicked();
    };
    virtual void on_mui_editar_clicked();
    virtual void on_mui_crear_clicked()  {
        if(m_companyact != NULL)
            m_companyact->s_newAlbaranPro();
    };
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_imprimir_clicked() {
        imprimir();
    };
    virtual void on_mui_actualizar_clicked() {
        presenta();
    };
    virtual void s_configurar();
signals:
    void selected(QString);
};

#endif
