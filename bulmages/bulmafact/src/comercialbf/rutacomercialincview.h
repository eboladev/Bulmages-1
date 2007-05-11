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

#ifndef RUTACOMERCIALINCVIEW_H
#define RUTACOMERCIALINCVIEW_H

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "ui_rutacomercialincbase.h"
#include "postgresiface2.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include "dialogchanges.h"
#include "funcaux.h"




class RutaComercialIncView : public FichaBf, public Ui_RutaComercialIncBase {
    Q_OBJECT

public:
    RutaComercialIncView(company *, QWidget *);
    ~RutaComercialIncView();
    void closeEvent(QCloseEvent *);
    virtual void pintar() {}
    ;

public slots:
    virtual void on_mui_guardar_clicked() {
        _depura("RutaComercialIncView::on_mui_guardar_clicked", 0);
        mui_rutacomercial->guardar();
        mui_incidenciacomercial->guardar();
        _depura("END RutaComercialIncView::on_mui_guardar_clicked", 0);
    };
    virtual int cargar(QString id, QString id1) {
        _depura("RutaComercialIncView::cargar", 0);
        int err = mui_rutacomercial->cargar(id);
        err = mui_incidenciacomercial->cargar(id1);
        setWindowTitle(tr("Accion comercial") + " " + mui_rutacomercial->DBvalue("idrutacomercial") + " + " + mui_incidenciacomercial->DBvalue("idincidenciacomercial"));
        empresaBase()->meteWindow(windowTitle(), this);
        _depura("END RutaComercialIncView::cargar", 0);
        return err;
    };
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_aceptar_clicked() {
        on_mui_guardar_clicked();
        close();
    };
};

#endif

