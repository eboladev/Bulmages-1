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

#ifndef BALANCEVIEW_H
#define BALANCEVIEW_H

#include <QtWidgets/QLayout>

#include "ui_balancebase.h"
#include "blpostgresqlclient.h"
#include "blconfiguration.h"
#include "bcplancontablelistview.h"
#include "bfform.h"


class BfCompany;


/// Balance. Muestra un balance de sumas y saldos.
/**
*/
class BalanceView : public BfForm, public Ui_BalanceBase
{
    Q_OBJECT

public:
    BalanceView ( BfCompany *, QWidget *parent = 0, int flags = 0 );
    void inicializa ( QString, QString, QString, QString, QString );
    ~BalanceView();
    virtual void accept();
    void imprimir();
    
private:
    /// Presenta el Balance.
    void presentar();
    /// Presenta el Balance de Sumas y Saldos.
    void presentarSyS ( QString, QString, QString, QString, int, int, bool );

public slots:
    void on_mui_actualizar_clicked();
};


#endif

