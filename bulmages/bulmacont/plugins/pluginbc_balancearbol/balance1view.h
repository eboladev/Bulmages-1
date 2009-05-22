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

#ifndef BALANCE1VIEW_H
#define BALANCE1VIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QToolButton>
#include <QTreeWidgetItem>

#include "qlayout.h"
#include "bcplancontablelistview.h"
#include "bcdiarioview.h"
#include "bcextractoview.h"
#include "blpostgresqlclient.h"
#include "blconfiguration.h"
#include "ui_balancetreebase.h"
#include "bcform.h"
#include "bcplancontablearbol.h"


class intapunts3view;
class diarioview1;
class BcExtractoView;
class BcCompany ;


/// Pantalla de presentación del balnace de sumas y saldos (jerarquica)
/** \todo Hay que eliminar el array de ccostes y sustituirlo por un BlDbRecordSet.
    \todo Hay que eliminar los punteros de extracto - diario - introaputnes y hacerlo pasar
    todo por la clase empresa.
    Esta clase sirve para presentar en balance de sumas y saldos en forma de tabla. Permite
    presentar el balance entre unos rangos definidos, incluyendo las cuentas de un nivel
    determinado. */
class BalanceTreeView : public BcForm, public Ui_BalanceTreeBase
{
    Q_OBJECT

private:
    int m_ccuenta;
    int m_cdenominacion;
    int m_csaldo_ant;
    int m_cdebe;
    int m_chaber;
    int m_csaldo;
    int m_cdebeej;
    int m_chaberej;
    int m_csaldoej;
//     int m_cidcuenta;
//     int m_cnivel;
//     int m_cpadre;
    BcPlanContableArbol *arbol; /// el arbol de cuentas

public:
    BalanceTreeView ( BcCompany *, QWidget *parent = 0, int flags = 0 );
    void inicializa1 ( QString, QString, QString, QString, int );
    ~BalanceTreeView();
    virtual void imprimir();

private:
    void presentar();
    bool generaBalance(); /// Genera el balance con un árbol de cuentas y devuelve 1 si se consigue con éxito

private slots:
    virtual void contextmenu ( const QPoint & );
//     virtual void nivelactivated ( int );
//     virtual void nivelactivated1 ( int, QTreeWidgetItem * );
    virtual void boton_extracto1 ( int );
    virtual void boton_asiento();
    virtual void boton_diario1 ( int );

public slots:
    virtual void on_mui_actualizar_released();
    virtual void on_mui_hojacalculo_released();
};

#endif
