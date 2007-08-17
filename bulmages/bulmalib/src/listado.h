/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef LISTADO_H
#define LISTADO_H

#include <QWidget>
#include <QCloseEvent>

#include "dialogchanges.h"
#include "funcaux.h"
#include "empresabase.h"
#include "subform3.h"
#include "blwidget.h"


class Listado : public BLWidget {
    Q_OBJECT
public:
    enum edmode
    {
        EditMode = 0, SelectMode = 1
    };

    SubForm3 *m_listado;

private:
    /// Indica si es modo consulta o modo edicion. (altera el comportamiento del
    /// doble click sobre la lista)
    edmode m_modo;

protected:
    void setSubForm(SubForm3 *);



public:
    Listado(QWidget *parent = 0, Qt::WFlags f = 0, edmode modo = EditMode);
    Listado(EmpresaBase *emp = NULL, QWidget *parent = 0, Qt::WFlags f = 0, edmode modo = EditMode);
    virtual void editar(int);
    virtual void imprimir();
    virtual void presentar();
    virtual ~Listado();
    virtual int sacaWindow();
    virtual int meteWindow(QString title);
    virtual void submenu(const QPoint &);
    virtual void crear();
    virtual void borrar();
    void hideBotonera();
    void showBotonera();
    void hideBusqueda();
    void showBusqueda();
    void setModoConsulta();
    void setModoEdicion();
    bool modoEdicion();
    bool modoConsulta();
    void trataPermisos(QString nomtabla);

public slots:
    virtual void on_mui_list_customContextMenuRequested(const QPoint &);
    virtual void on_mui_list_cellDoubleClicked(int, int);
    virtual void on_mui_list_toogledConfig(bool check);
    virtual void on_m_filtro_textChanged(const QString &text);
    virtual void on_m_filtro_editingFinished();
    virtual void on_mui_actualizar_clicked();
    virtual void on_mui_imprimir_clicked();
    virtual void on_mui_editar_clicked();
    virtual void on_mui_configurar_toggled(bool checked);
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_crear_clicked();

signals:
    void selected(QString);
};

#endif

