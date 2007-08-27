/***************************************************************************
 *   Copyright (C) 2007 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
 *   http://www.iglues.org Asociacion Iglues -- Contabilidad Linux         *
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

#ifndef CPAISVIEW_H
#define CPAISVIEW_H

#include <QWidget>
//#include <QComboBox>
#include <QLineEdit>
#include <QCloseEvent>

#include "ui_paisbase.h"
#include "postgresiface2.h"
#include "dialogchanges.h"
#include "ficha.h"


class EmpresaBase;


/// Visualiza un centro de coste.
/** */
class PaisView : public Ficha, public Ui_paisbase {
    Q_OBJECT

public:
    int idpais; /// Indica cual es el centro de coste que se esta visualizando.
    /// Si su valor es 0 entonces es que no se esta visualizando ning&uacute;n centro de coste.


public:
    PaisView(EmpresaBase *, QWidget *parent = 0);
    ~PaisView();
    void pintar();
    void mostrarplantilla();
    virtual void on_mui_borrar_clicked();

public slots:
    virtual void on_mui_guardar_clicked();
    virtual void on_mui_crear_clicked();
    virtual void on_mui_list_itemSelectionChanged();
    //virtual void on_mui_list_itemChanged(QTableWidgetItem *);
    virtual void closeEvent(QCloseEvent *);
};

#endif

