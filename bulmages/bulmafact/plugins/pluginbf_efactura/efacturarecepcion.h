/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#ifndef EFACTURARECEPCION_H
#define EFACTURARECEPCION_H

#include <QtWidgets/QWidget>

#include <ui_efacturarecepcionbase.h>
#include "bfcompany.h"
#include "bfform.h"

class EFacturaRecepcion : public BfForm, public Ui_EFacturaRecepcionBase
{
    Q_OBJECT
public:
    EFacturaRecepcion ( BfCompany *emp, QWidget *parent = 0 );
    ~EFacturaRecepcion();

public slots:
//  virtual void on_mui_examinaECertificado_clicked();
    virtual void on_mui_recibir_clicked();
    virtual void on_mui_abrir_clicked();
    virtual void on_mui_salir_clicked();
};

#endif
