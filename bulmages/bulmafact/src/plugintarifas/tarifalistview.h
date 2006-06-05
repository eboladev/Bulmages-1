/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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

#ifndef TARIFALISTVIEW_H
#define TARIFALISTVIEW_H

#include "ui_tarifalistbase.h"
#include "company.h"
#include "pgimportfiles.h"


class TarifaListView : public QWidget, private Ui_TarifaListBase, public pgimportfiles {
    Q_OBJECT

public:
    enum edmode
    {
        EditMode = 0, SelectMode = 1
    };

private:
    company *m_companyact;
    edmode m_modo;

public:
    TarifaListView(company *, QWidget *parent = 0, const char *name = 0,
                   Qt::WFlags flag = 0, edmode editmodo = EditMode);
    ~TarifaListView();
    void editar(int);
public slots:
    void on_mui_crear_clicked();
    void on_mui_editar_clicked();
    void on_mui_borrar_clicked();
    void on_mui_list_itemDoubleClicked( QTableWidgetItem *item) {
        on_mui_editar_clicked();
    };
};

#endif

