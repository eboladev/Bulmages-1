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
#include "fichabf.h"


/// Administra el listado de presupuestos.
/** */
class PresupuestoListSubForm : public SubForm2Bf {
    Q_OBJECT

public:
    PresupuestoListSubForm(QWidget *parent = 0, const char *name = 0);
    ~PresupuestoListSubForm() {}

public slots:
    virtual void cargar();
    virtual void cargar(QString query);
};


#include "ui_presupuestoslistbase.h"


/// Muestra y administra la ventana con el listado de presupuestos.
/** */
class PresupuestoList : public FichaBf, public Ui_PresupuestosListBase {
    Q_OBJECT

private:
    /// m_modo == 0 es modo edicion.
    /// m_modo == 1 es modo selector.
    int m_modo;
    QString m_idpresupuesto;

public:
    PresupuestoList(QWidget *parent = 0, Qt::WFlags flag = 0);
    PresupuestoList(company *comp = NULL, QWidget *parent = 0, Qt::WFlags flag = 0);
    ~PresupuestoList();
    void presenta();
    int modo();
    QString idpresupuesto();
    void modoseleccion();
    void modoedicion();
    void setEmpresaBase (company *comp);
    void hideBotonera();
    void showBotonera();
    void hideBusqueda();
    void showBusqueda();
    void imprimir();
    void meteWindow(QString nom, QObject *obj);
    void setidcliente(QString val);
    void setidarticulo(QString val);
    QString generaFiltro();
    void editar(int);
    virtual void on_mui_borrar_clicked();

public slots:
    virtual void on_m_filtro_textChanged(const QString &text);
    virtual void on_mui_list_itemDoubleClicked(QTableWidgetItem *);
    virtual void on_mui_editar_clicked();
    virtual void on_mui_crear_clicked();
    virtual void on_mui_imprimir_clicked();
    virtual void on_mui_actualizar_clicked();
    virtual void on_mui_configurar_toggled(bool checked);

signals:
    void selected(QString);
};

#endif

