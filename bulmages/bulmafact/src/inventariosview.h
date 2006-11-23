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

#ifndef INVENTARIOSVIEW_H
#define INVENTARIOSVIEW_H

#include "company.h"
#include "subform2bf.h"
#include "ficha.h"


/// Muestra y administra las l&iacute;neas de detalle del listado de inventarios.
/** */
class InventariosSubForm : public SubForm2Bf {
    Q_OBJECT

public:
    InventariosSubForm(QWidget *parent = 0);
    ~InventariosSubForm() {}
    ;

public slots:
    virtual void cargar() {
        _depura("InventariosSubform::cargar\n", 0);
        QString SQLQuery = "SELECT * FROM inventario";
        cursor2 * cur= companyact()->cargacursor(SQLQuery);
        SubForm3::cargar(cur);
        delete cur;
    };
};


#include "ui_inventariosbase.h"


/// Muestra y administra el listado de inventarios.
/** */
class InventariosView : public Ficha, public Ui_InventariosBase {
    Q_OBJECT

private:
    company *companyact;

public:
    InventariosView(QWidget *parent = 0, Qt::WFlags flag = 0);
    InventariosView(company *,QWidget *parent = 0);
    ~InventariosView();
    void inicializa();
    void setcompany(company *comp) {
        companyact = comp;
        mui_listado->setcompany(comp);
    }
    void meteWindow(QString nom, QObject *obj) {
        if (companyact != NULL) {
            companyact->meteWindow(nom, obj);
        }
    }
    virtual int sacaWindow();

public slots:
    virtual void on_mui_listado_itemDoubleClicked(QTableWidgetItem *) {
        on_mui_editar_clicked();
    }
    virtual void on_mui_crear_clicked() {
        companyact->s_newInventario();
    }
    virtual void on_mui_editar_clicked();
    virtual void on_mui_listado_itemDoubleClicked() {
        on_mui_editar_clicked();
    }
    virtual void on_mui_borrar_clicked();
};

#endif

