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

#include "qlayout.h"

#include "ui_balancebase.h"
#include "blpostgresqlclient.h"
#include "blconfiguration.h"
#include "bcplancontablelistview.h"
#include "bcform.h"


class BcCompany;


/// Balance de Sumas y Saldos plano.
/**
    Esta clase muestra un balance de sumas y saldos de la empresa abierta.
    Esta clase genera el balance de sumas y saldos y lo muestra por pantalla en una
    ventana dise&ntilde;ada espec&iacute;ficamente para ello.
*/
class BalanceView : public BcForm, public Ui_BalanceBase
{
    Q_OBJECT

private:
    /// Cursor para recorrer las cuentas.
    BlDbRecordSet *m_cursorcta;
    /// El n&uacute;mero de d&iacute;gitos que tienen las cuentas de la empresa.
    int numDigitos;
    /// Array que almacena los identificadores de los centros de coste.

public:
    BalanceView ( BcCompany *, QWidget *parent = 0, int flags = 0 );
    void inicializa1 ( QString, QString, QString, QString, QString );
    ~BalanceView();
    void imprimir();
    virtual void accept();

private:
    /// Presenta el Balance.
    void presentar();
    /// Presenta el Balance de Sumas y Saldos.
    void presentarSyS ( QString, QString, QString, QString, int, int, bool );

public slots:
    void on_mui_actualizar_clicked();
};


#endif

