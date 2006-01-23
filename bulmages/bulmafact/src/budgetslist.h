/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borr� Riera                              *
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

// Listado de presupuestos.

#include "budgetslistbase.h"
#include "company.h"
#include "busquedacliente.h"
#include "busquedaarticulo.h"
#include <qlineedit.h>
#include <q3table.h>


class BudgetsList : public BudgetsListBase {
    Q_OBJECT
private:
    company *companyact;
    int m_modo; // == 0 es modo edici�
    // ==1 es modo selector.
    QString m_idpresupuesto;
    void inicializa();
public:
    BudgetsList(QWidget *parent = 0, const char *name = 0, Qt::WFlags flag = 0);
    BudgetsList(company *comp = NULL, QWidget *parent = 0, const char *name = 0, Qt::WFlags flag = 0);
    ~BudgetsList();
    void presenta();
    void modoseleccion() {
        m_modo=1;
    };
    void modoedicion() {
        m_modo=0;
    };
    void setcompany (company *comp) {
        companyact=comp;
        m_cliente->setcompany(comp);
        m_articulo->setcompany(comp);
    };
    QString idpresupuesto() {
        return m_idpresupuesto;
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
        if (companyact != NULL)
            companyact->meteWindow(nom, obj);
    };
    void setidcliente(QString val) {
        m_cliente->setidcliente(val);
    };
    void setidarticulo(QString val) {
        m_articulo->setidarticulo(val);
    };
    QString generaFiltro();

    /// Estas funciones guardan y cargan la configuración de presentación del listado.
    void guardaconfig();
    void cargaconfig();


public slots:
    virtual void doubleclicked(int, int, int, const QPoint &);
    virtual void s_contextMenu(int, int, int, const QPoint &);
    virtual void s_editar();
    virtual void newBudget() {
        companyact->s_newPresupuestoCli();
    };
    virtual void s_removeBudget();
    virtual void s_imprimir() {
        imprimir();
    };
    virtual void s_filtrar() {
        presenta();
    };
    virtual void s_mostrarBusqueda() {
        if (m_busqueda->isVisible())
            hideBusqueda();
        else
            showBusqueda();
    };

    virtual void s_mostrarConfiguracion() {
        if (m_configuracion->isVisible())
            hideConfiguracion();
        else
            showConfiguracion();
    }
    ;

    virtual void s_configurar();
};

#endif
