/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef CUADRANTEDIARIOVIEW_H
#define CUADRANTEDIARIOVIEW_H

#include "ui_cuadrantediariobase.h"
#include "bldialogchanges.h"
#include "bfform.h"


class BfCompany;


/** Ventana de cuadrantes
    Se encarga de la creacion de cuadrantes en la empresa, para ubicar empleados, etc etc etc
    Deriva de BfForm para metodos comunes a todas las fichas. */
class CuadranteDiarioView : public BfForm, public Ui_CuadranteDiarioBase
{
    Q_OBJECT

public:
    CuadranteQTextDocument *m_actualcell;

public:
    CuadranteDiarioView ( BfCompany *, QWidget * );
    ~CuadranteDiarioView();
    void inicializaTrabajadores();
    void inicializaCuadrante ( const QDate &dateorig );
    virtual void on_mui_imprimir_clicked();
    void loadConfig();
    void saveConfig();

public slots:
    virtual void on_mui_calendario_clicked ( const QDate &date );
    virtual void on_mui_listtrabajadores_itemDoubleClicked ( QTreeWidgetItem *item, int column );
    virtual void on_mui_editar_clicked();
    virtual void on_mui_calendario_customContextMenuRequested ( const QPoint & pos );
    virtual void on_mui_actualizar_clicked();
    virtual void on_mui_limpiar_clicked();
    virtual void on_mui_duplicar_clicked();
};


#endif

