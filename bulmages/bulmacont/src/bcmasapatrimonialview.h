/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociación Iglues -- Contabilidad Linux         *
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

#ifndef MPATRIMONIALVIEW_H
#define MPATRIMONIALVIEW_H

#include <QtWidgets/QWidget>

#include "blfunctions.h"
#include "ui_bcmasapatrimonialbase.h"
#include "blwidget.h"


class BcCompany;


class BC_EXPORT BcMasaPatrimonialView : public QDialog, public Ui_BcMasaPatrimonialBase, public BlMainCompanyPointer
{
    Q_OBJECT

public:
    /// El identificador de masa patrimonial. (Lo guardamos en forma de string porque es una
    /// prueba en la que no hay que hacer convesiones de tipos).
    QString idmpatrimonial;

public:
    BcMasaPatrimonialView ( BcCompany *, QWidget *parent = 0, Qt::WindowFlags fl = 0 );
    ~BcMasaPatrimonialView();
    void inicializa1 ( QString );
    QString getIdMasa();
    QString getNomMasa();

public slots:
    virtual void on_mui_nuevasuma_clicked();
    virtual void on_mui_nuevaresta_clicked();
    virtual void on_mui_borrasuma_clicked();
    virtual void on_mui_borraresta_clicked();
    virtual void on_mui_aceptar_clicked();
    virtual void on_mui_buscampatrimonial_clicked();
    virtual void on_mui_cancelar_clicked();
};

#endif
