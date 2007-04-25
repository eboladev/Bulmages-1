/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#ifndef BALANCEVIEW_H
#define BALANCEVIEW_H

#include "qlayout.h"

#include "ui_balancebase.h"
#include "diarioview.h"
#include "extractoview1.h"
#include "postgresiface2.h"
#include "configuracion.h"
#include "listcuentasview1.h"
#include "ficha.h"


class empresa;


/// Balance de Sumas y Saldos plano.
/** \todo Hay que eliminar el ccostes[200] y cambiarlo por un cursor.
    Esta clase muestra un balance de sumas y saldos de la empresa abierta.
    Esta clase genera el balance de sumas y saldos y lo muestra por pantalla en una
    ventana dise&ntilde;ada espec&iacute;ficamente para ello.
*/
class BalanceView : public Ficha, public Ui_BalanceBase {
    Q_OBJECT

private:
    /// Empresa con la que se ha inicializado la clase.
    empresa *m_companyact;
    /// Cursor para recorrer las cuentas.
    cursor2 *m_cursorcta;
    /// El n&uacute;mero de d&iacute;gitos que tienen las cuentas de la empresa.
    int numdigitos;
    /// Array que almacena los identificadores de los centros de coste.

public:
    BalanceView(empresa *, QWidget *parent = 0, int flags = 0);
    void inicializa1(QString, QString, QString, QString, QString);
    ~BalanceView();

private:
    /// Presenta el Balance.
    void presentar();
    /// Presenta el Balance de Sumas y Saldos.
    void presentarSyS(QString, QString, QString, QString, int, int, bool);

public slots:
    virtual void accept();
    virtual void on_mui_imprimir_clicked();

private slots:
    virtual void nivelactivated(int);
};

#endif

