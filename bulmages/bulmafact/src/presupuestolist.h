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

#ifndef BUDGETSLIST_H
#define BUDGETSLIST_H

#include <QLineEdit>

#include "company.h"
#include "busquedacliente.h"
#include "busquedaarticulo.h"
#include "subform2bf.h"


/// Administra el listado de presupuestos.
/** */
class PresupuestoListSubForm : public SubForm2Bf {
    Q_OBJECT

public:
    PresupuestoListSubForm(QWidget *parent = 0, const char *name = 0);
    ~PresupuestoListSubForm() {}
    ;

public slots:
    virtual void cargar() {
        _depura("PresupuestoListSubForm::cargar\n", 0);
        QString SQLQuery = "SELECT * FROM presupuesto";
        cursor2 * cur= companyact()->cargacursor(SQLQuery);
        SubForm3::cargar(cur);
        delete cur;
    };
    virtual void cargar(QString query) {
        SubForm3::cargar(query);
    };
};


#include "ui_budgetslistbase.h"


/// Muestra y administra la ventana con el listado de presupuestos.
/** */
class PresupuestoList : public QWidget, public Ui_BudgetsListBase {
    Q_OBJECT

private:
    company *m_companyact;
    /// m_modo == 0 es modo edicion.
    /// m_modo == 1 es modo selector.
    int m_modo;
    QString m_idpresupuesto;

public:
    PresupuestoList(QWidget *parent = 0, Qt::WFlags flag = 0);
    PresupuestoList(company *comp = NULL, QWidget *parent = 0, Qt::WFlags flag = 0);
    ~PresupuestoList();
    void presenta();
    int modo() {
        return m_modo;
    };
    company *getcompany() {
        return m_companyact;
    };
    QString idpresupuesto() {
        return m_idpresupuesto;
    };
    void modoseleccion() {
        m_modo = 1;
    };
    void modoedicion() {
        m_modo = 0;
    };
    void setcompany (company *comp) {
        m_companyact = comp;
        m_cliente->setcompany(comp);
        m_articulo->setcompany(comp);
        mui_list->setcompany(comp);
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
    void setidcliente(QString val) {
        m_cliente->setidcliente(val);
    };
    void setidarticulo(QString val) {
        m_articulo->setidarticulo(val);
    };
    QString generaFiltro();
    void editar(int);

public slots:
    virtual void on_m_filtro_textChanged(const QString &text) {
        if(text.size() >= 3)
            on_mui_actualizar_clicked();
    };
    virtual void on_mui_list_itemDoubleClicked(QTableWidgetItem *) {
        on_mui_editar_clicked();
    };
    virtual void on_mui_editar_clicked();
    virtual void on_mui_crear_clicked() {
        m_companyact->s_newPresupuestoCli();
    };
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_imprimir_clicked() {
        imprimir();
    };
    virtual void on_mui_actualizar_clicked() {
        presenta();
    };
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

