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
#include "fichabf.h"

class company;


/// Administra el listado de proveedores.
/** */
class ProveedorListSubform : public SubForm2Bf {
    Q_OBJECT

public:
    ProveedorListSubform(QWidget *parent = 0);
    ~ProveedorListSubform() {}

public slots:
    virtual void cargar();
    virtual void cargar(QString a);
};


#include "ui_providerslistbase.h"


/// Muestra y administra la ventana con el listado de proveedores.
/** */
class ProveedorList : public FichaBf, public Ui_ProveedorListBase, public pgimportfiles {
    Q_OBJECT

public:
    enum edmode
    {
        EditMode = 0,
        SelectMode = 1
    };

private:
    edmode m_modo;
    QString m_idprovider;
    QString m_cifprovider;
    QString m_nomprovider;

public:
    ProveedorList(company *, QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmode = EditMode);
    ~ProveedorList();
    void presenta();
    void modoseleccion();
    void modoedicion();
    QString idprovider();
    QString cifprovider();
    QString nomprovider();
    void hideBotonera();
    void showBotonera();
    void hideBusqueda();
    void showBusqueda();

public slots:
    virtual void on_mui_filtro_textChanged(const QString &text);
    virtual void editar(int);
    virtual void on_mui_crear_clicked();
    virtual void s_findProvider();
    virtual void on_mui_editar_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_imprimir_clicked();
    virtual void on_mui_actualizar_clicked();
    virtual void on_mui_exportar_clicked();
    virtual void on_mui_importar_clicked();
    void on_mui_list_itemDoubleClicked(QTableWidgetItem *);
    virtual void on_mui_configurar_toggled(bool checked);

signals:
    void selected(QString);
};

#endif

