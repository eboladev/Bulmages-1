/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borrás Riera                              *
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


class company;


class BudgetsList : public BudgetsListBase {
Q_OBJECT
private:
   company *companyact;
   int m_modo; // == 0 es modo edición
            // ==1 es modo selector.
   QString m_idpresupuesto;
   
public:
    BudgetsList(company *, QWidget *parent = 0, const char *name = 0, int flag = 0);
    ~BudgetsList();
    void inicializa();
    void modoseleccion() {m_modo=1;};
    void modoedicion() {m_modo=0;};
    QString idpresupuesto() {return m_idpresupuesto;};
    
public slots:
    virtual void doubleclicked(int, int, int, const QPoint &);
    virtual void s_contextMenu(int, int, int, const QPoint &);
    virtual void newBudget();
	 virtual void s_removeBudget();
    
    /*
    virtual void boton_editar();
    
    virtual void boton_duplicar();
    virtual void boton_borrar();
    virtual void boton_imprimir();
    virtual void boton_filtrar();
*/
};

#endif
