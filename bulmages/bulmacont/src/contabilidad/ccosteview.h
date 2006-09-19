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

#ifndef CCOSTEVIEW_H
#define CCOSTEVIEW_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <Q3TextEdit>

#include "ui_ccostebase.h"
#include "postgresiface2.h"
#include "dialogchanges.h"


class empresa;


class ccosteview : public QWidget, public Ui_ccostedlg, public dialogChanges {
    Q_OBJECT

public:
    postgresiface2 *conexionbase;
    int ccostes[100];
    int idc_coste; /// Indica cual es el centro de coste que se esta visualizando.
    /// Si su valor es 0 entonces es que no se esta visualizando ningun centro de coste.
    empresa *empresaactual;

public:
    ccosteview(empresa *, QWidget *parent = 0);
    ~ccosteview();
    void pintar();
    void mostrarplantilla();

public slots:
    virtual void on_mui_guardar_clicked();
    virtual void on_mui_crear_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_list_itemClicked(QTreeWidgetItem *, int);
    virtual void closeEvent(QCloseEvent *);
};

#endif

