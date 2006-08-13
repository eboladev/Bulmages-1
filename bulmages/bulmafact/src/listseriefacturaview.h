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

#ifndef LISTSERIEVIEW_H
#define LISTSERIEVIEW_H

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "postgresiface2.h"
#include "cobro.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include "subform2bf.h"



class company;


class ListSerieFacturaSubForm : public SubForm2Bf {
    Q_OBJECT

public:
    ListSerieFacturaSubForm(QWidget *parent = 0);
    ~ListSerieFacturaSubForm() {}
    ;

public slots:
    virtual void cargar() {
        _depura("ListSerieFacturaSubForm::cargar\n",0);
        cursor2 * cur= companyact()->cargacursor("SELECT *, codigoserie_factura AS codigoserie_facturaorig FROM serie_factura");
        SubForm3::cargar(cur);
        delete cur;
    };
};


#include "ui_listseriefacturabase.h"


class ListSerieFacturaView : public QDialog, public Ui_ListSerieFacturaBase {
    Q_OBJECT

public:
    company *companyact;

public:
    ListSerieFacturaView(company *, QWidget *);
    ~ListSerieFacturaView();

public slots:
    virtual void on_mui_aceptar_clicked() {
        mui_listado->guardar();
        close();
    };
};

#endif

