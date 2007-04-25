/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                               *
 *   josep@burcion.com                                                     *
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

#ifndef LISTADO347_H
#define LISTADO347_H

#include "ui_listado347base.h"
#include "postgresiface2.h"


class Listado347 : public QDialog, public Ui_Listado347Base {
   Q_OBJECT

private:
    postgresiface2 * DBConn;

public:
    Listado347(postgresiface2 *DBConn = 0, QString ejerActual = "", QWidget *parent = 0, Qt::WFlags f = Qt::Window);
    ~Listado347();

private slots:
    void on_m_boton_recalcular_clicked();
    void on_m_boton_imprimir_clicked();
    void on_finicial_editingFinished();
    void on_ffinal_editingFinished();
};

#endif

