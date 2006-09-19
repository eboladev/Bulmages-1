/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#ifndef BALANCESVIEW_H
#define BALANCESVIEW_H

#include <QWidget>

#include "ui_canualesbase.h"


/**
 * \author Tomeu Borras Riera
 * \class balancesview balancesview.h
 * \brief Listado de Balances Disponibles
 * 
 * Clase que muestra un listado de todos los balances introducidos en el sistema.
 * Sirve como punto de entrada a la generacion de los balances permitiendo la
 * importacion/exportacion de los mismos, su modificacion y su impresion.
 */
class empresa;


class CAnualesView : public QWidget, public Ui_CAnualesBase {
    Q_OBJECT

private:
    /// Empresa con la que se trabaja (inicializada con el constructor de clase)
    empresa *m_companyact;
    /// Esta variable indica si se abre para busqueda o para edicion.
    /// (edicion = 0; seleccion = 1)
    int m_modo;
    /// Indica el nombre del balance seleccionado
    QString m_nomBalance;
    /// Indica el identificador del balance seleccionado.
    QString m_idBalance;

public:
    CAnualesView(empresa *, QWidget *parent = 0, const char *name = 0);
    ~CAnualesView();
    void inicializatabla();
    /// Establece el funcionamiento de la ventana como selector (afecta sobre el dobleclick en el listado)
    void setmodoselector() {
        m_modo = 1;
    }
    /// Establece el funcionamiento de la ventana como editor (afecta sobre el dobleclick en el listado)
    void setmodoeditor() {
        m_modo = 0;
    }
    /// Devuelve el nombre del Balance seleccionado por la clase.
    QString nomBalance() {
        return m_nomBalance;
    }
    /// Devuelve el identificador del Balance que se ha seleccionado.
    QString idBalance() {
        return m_idBalance;
    }

private slots:
    /// SLOT que responde a la pulsacion del boton de imprimir Balance.
    virtual void imprimir();
    /// SLOT que responde a la accion de hacer doble click sobre el listado.
    virtual void on_mui_listado_itemDoubleClicked(QTableWidgetItem *);
};

#endif
