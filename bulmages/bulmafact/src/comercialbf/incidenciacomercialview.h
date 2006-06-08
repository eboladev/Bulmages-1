/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef INCIDENCIACOMERCIALVIEW_H
#define INCIDENCIACOMERCIALVIEW_H



#include <QLineEdit>
#include <Q3TextEdit>
#include <QLabel>
#include <QCheckBox>

#include "ui_incidenciacomercialbase.h"
#include "postgresiface2.h"
#include "incidenciacomercial.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include "dialogchanges.h"


class company;

class IncidenciaComercialView : public QWidget, private Ui_IncidenciaComercialBase, public IncidenciaComercial, public dialogChanges {
    Q_OBJECT

public:
    IncidenciaComercialView(QWidget *parent=NULL, const char *name="");

    ~IncidenciaComercialView();
    IncidenciaComercialView(company *, QWidget *parent=NULL, const char *name="");
    void closeEvent(QCloseEvent *);
    virtual void pintar() {}
    ;

    void setcompany(company *comp) {
        _depura("IncidenciaComercialView::setcompany",0);
        IncidenciaComercial::setcompany(comp);
        mui_idcliente->setcompany(comp);
        mui_idtrabajador->setcompany(comp);
        mui_idtrabajador->setidtrabajador("0");
        _depura("END IncidenciaComercialView::setcompany",0);

    };

    virtual int guardar() {
        _depura("IncidenciaComercialView::guardar",0);
        if (mui_fechaincidenciacomercial->text() == "")
            return 0;
        setDBvalue("fechaincidenciacomercial", mui_fechaincidenciacomercial->text());
        setDBvalue("idcliente", mui_idcliente->idcliente());
        setDBvalue("comentincidenciacomercial", mui_comentincidenciacomercial->text());
	setDBvalue("idtrabajador", mui_idtrabajador->idtrabajador());
	setDBvalue("estadoincidenciacomercial", mui_estadoincidenciacomercial->estado());
	setDBvalue("horaincidenciacomercial", mui_horaincidenciacomercial->text());
	setDBvalue("refincidenciacomercial", mui_refincidenciacomercial->text());

        int err = IncidenciaComercial::guardar();
        _depura("END IncidenciaComercialView::guardar",0);
        return err;
    };


public slots:
    virtual int cargar(QString id) {
        _depura("IncidenciaComercialView::cargar",0);
        int err = IncidenciaComercial::cargar(id);

        setCaption("IncidenciaComercial " + DBvalue("idincidenciacomercial"));

        mui_fechaincidenciacomercial->setText(DBvalue("fechaincidenciacomercial"));
        mui_idcliente->setidcliente(DBvalue("idcliente"));
        mui_comentincidenciacomercial->setText(DBvalue("comentincidenciacomercial"));
        mui_idtrabajador->setidtrabajador(DBvalue("idtrabajador"));
	mui_estadoincidenciacomercial->setestado(DBvalue("estadoincidenciacomercial"));
        mui_horaincidenciacomercial->setText(DBvalue("horaincidenciacomercial"));
        mui_refincidenciacomercial->setText(DBvalue("refincidenciacomercial"));

        dialogChanges_cargaInicial();
        _depura("IncidenciaComercialView::cargar",0);
        return err;
    };

};



#endif
