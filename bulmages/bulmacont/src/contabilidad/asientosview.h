/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
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

#ifndef ASIENTOSVIEW_H
#define ASIENTOSVIEW_H

#include <QWidget>

#include "postgresiface2.h"
#include "ui_asientolistbase.h"


/** \brief Muestra un listado de todos los asientos existentes e informa de si estan abiertos o cerrados.
*/
class empresa;


class asientosview : public QWidget, public Ui_AsientoListBase {
    Q_OBJECT

public:
    empresa *m_companyact;

public:
    asientosview(empresa *emp, QWidget *parent = 0);
    ~asientosview();
    void inicializa();

public slots:
    virtual void on_mui_list_cellDoubleClicked(int, int);
    virtual void on_mui_actualizar_clicked() {
        inicializa();
    };
    virtual void on_mui_configurar_clicked() {
        mui_list->showConfig();
    };
    virtual void on_mui_imprimir_clicked();
};

#endif

