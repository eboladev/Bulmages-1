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

#ifndef INVENTARIOVIEW_H
#define INVENTARIOVIEW_H

#include <QLineEdit>
#include <Q3TextEdit>
#include <QLabel>
#include <QCheckBox>

#include "ui_inventariobase.h"
#include "inventario.h"
#include "busquedafecha.h"
#include "postgresiface2.h"


class InventarioView : public QWidget, public Ui_InventarioBase, public Inventario {
    Q_OBJECT

public:
    InventarioView(company *, QWidget *parent = 0, const char *name = 0);
    ~InventarioView();
    void pintaidinventario(QString) {}
    ;
    void pintafechainventario(QString id) {
        m_fechainventario->setText(id);
    };
    void pintanominventario(QString id) {
        m_nominventario->setText(id);
    };

public slots:
    virtual void on_mui_guardar_clicked() {
        setfechainventario(m_fechainventario->text());
        setnominventario(m_nominventario->text());
	guardar();
    };
    virtual void on_mui_aceptar_clicked() {
        setfechainventario(m_fechainventario->text());
        setnominventario(m_nominventario->text());
        if(!guardar()) close();
    };
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_pregenerar_clicked() {
        pregenerar();
    };
};

#endif

