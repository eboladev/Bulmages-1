/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C. (Porting to QT4)              *
 *   fcojavmc@todo-redes.com                                               *
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

#ifndef LISTALMACENVIEW_H
#define LISTALMACENVIEW_H

#include <QLineEdit>
#include <Q3TextEdit>
#include <QLabel>
#include <QCheckBox>

#include "postgresiface2.h"
#include "cobro.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include "subform2bf.h"

class company;

class ListAlmacenSubForm : public SubForm2Bf {
    Q_OBJECT
public:
    ListAlmacenSubForm(QWidget *parent = 0, const char *name = 0);
    ~ListAlmacenSubForm() {}
    ;

public slots:
    virtual void cargar() {
        _depura("ListAlmacenSubForm::cargar\n",0);
        cursor2 * cur= companyact()->cargacursor("SELECT * FROM almacen");
        SubForm2::cargar(cur);
        delete cur;
    };
};




// ALMACENES
#include "ui_listalmacenbase.h"


class ListAlmacenView : public QDialog, public Ui_ListAlmacenBase {
    Q_OBJECT

public:
    company *companyact;

public:
    ListAlmacenView(company *comp , QWidget *parent, const char *name="");
    ~ListAlmacenView();
public slots:

    virtual void on_mui_aceptar_clicked() {
        mui_listado->guardar();
        close();
    };
};

#endif
