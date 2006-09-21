/***************************************************************************
 *   Copyright (C) 2004 by J. M. Torres Rigo                               *
 *   joanmi@bulma.net                                                      *
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

#ifndef PROVIDERSLIST_H
#define PROVIDERSLIST_H

#include "pgimportfiles.h"
#include "subform2bf.h"


class company;


/// Administra el listado de proveedores.
/** */
class ProveedorListSubform : public SubForm2Bf {
    Q_OBJECT

public:
    ProveedorListSubform(QWidget *parent = 0);
    ~ProveedorListSubform() {}
    ;

public slots:
    virtual void cargar() {
        _depura("ProveedorListSubform::cargar\n", 0);
        QString SQLQuery = "SELECT * FROM proveedor";
        cursor2 * cur= companyact()->cargacursor(SQLQuery);
        SubForm3::cargar(cur);
        delete cur;
    };
    virtual void cargar(QString a) {
        SubForm3::cargar(a);
    };
};


#include "ui_providerslistbase.h"


/// Muestra y administra la ventana con el listado de proveedores.
/** */
class ProveedorList : public QWidget, public Ui_ProveedorListBase, public pgimportfiles {
    Q_OBJECT

public:
    enum edmode
    {
        EditMode = 0,
        SelectMode = 1
    };

private:
    company *m_companyact;
    edmode m_modo;
    QString m_idprovider;
    QString m_cifprovider;
    QString m_nomprovider;

public:
    ProveedorList(company *, QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmode = EditMode);
    ~ProveedorList();
    void presenta();
    void modoseleccion() {
        m_modo = SelectMode;
    };
    void modoedicion() {
        m_modo = EditMode;
    };
    QString idprovider() {
        return m_idprovider;
    };
    QString cifprovider() {
        return m_cifprovider;
    };
    QString nomprovider() {
        return m_nomprovider;
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

public slots:
    virtual void on_mui_filtro_textChanged(const QString &text) {
        if (text.size() >= 3)
            on_mui_actualizar_clicked();
    };
    virtual void editar(int);
    virtual void on_mui_crear_clicked();
    virtual void s_findProvider();
    virtual void on_mui_editar_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_imprimir_clicked();
    virtual void on_mui_actualizar_clicked() {
        presenta();
    };
    virtual void on_mui_exportar_clicked();
    virtual void on_mui_importar_clicked();
    void on_mui_list_itemDoubleClicked( QTableWidgetItem *) {
        on_mui_editar_clicked();
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

