/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#include <QWidget>
#include <QLineEdit>
#include <QToolButton>
#include <QTreeWidgetItem>

#include "qlayout.h"
#include "listcuentasview1.h"
#include "diarioview.h"
#include "extractoview1.h"
#include "postgresiface2.h"
#include "blconfiguration.h"
#include "ui_balancebase.h"
#include "fichabc.h"


class intapunts3view;
class diarioview1;
class extractoview1;
class Empresa;


/// Pantalla de presentación del balnace de sumas y saldos (jerarquica)
/** \todo Hay que eliminar el array de ccostes y sustituirlo por un cursor2.
    \todo Hay que eliminar los punteros de extracto - diario - introaputnes y hacerlo pasar
    todo por la clase empresa.
    Esta clase sirve para presentar en balance de sumas y saldos en forma de tabla. Permite
    presentar el balance entre unos rangos definidos, incluyendo las cuentas de un nivel
    determinado. */
class BalanceView : public FichaBc, public Ui_BalanceBase
{
    Q_OBJECT

private:
    /// Indica el n&uacute;mero de d&iacute;gitos que tienen por defecto las cuentas.
    int numdigitos;
    int m_ccuenta;
    int m_cdenominacion;
    int m_csaldo_ant;
    int m_cdebe;
    int m_chaber;
    int m_csaldo;
    int m_cdebeej;
    int m_chaberej;
    int m_csaldoej;
    int m_cidcuenta;
    int m_cnivel;
    int m_cpadre;

public:
    BalanceView ( Empresa *, QWidget *parent = 0, int flags = 0 );
    void inicializa1 ( QString, QString, QString, QString, int );
    ~BalanceView();
    virtual void imprimir();

private:
    void presentar();
    void generarBalance();

private slots:
    virtual void contextmenu ( const QPoint & );
    virtual void boton_extracto1 ( int );
    virtual void boton_asiento();
    virtual void boton_diario1 ( int );

public slots:
    virtual void on_mui_actualizar_clicked();
    virtual void on_mui_hojacalculo_clicked();
};

#endif

