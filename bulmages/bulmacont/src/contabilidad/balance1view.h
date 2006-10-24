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

#ifndef BALANCE1VIEW_H
#define BALANCE1VIEW_H

#include <Q3DateTimeEdit>
#include <Q3FileDialog>
#include <Q3Table>

#include <QWidget>
#include <QLineEdit>
#include <QToolButton>

#include "qtable1.h"
#include "qlayout.h"

#include "listcuentasview1.h"
#include "diarioview.h"
#include "extractoview1.h"
#include "postgresiface2.h"
#include "configuracion.h"
#include "ui_balancetreebase.h"


class intapunts3view;
class diarioview1;
class extractoview1;
class empresa;



/// Pantalla de presentación del balnace de sumas y saldos (jerarquica)
/** \todo Hay que eliminar el array de ccostes y sustituirlo por un cursor2.
    \todo Hay que eliminar los punteros de extracto - diario - introaputnes y hacerlo pasar todo por la clase empresa.
    Esta clase sirve para presentar en balance de sumas y saldos en forma de tabla. Permite
    presentar el balance entre unos rangos definidos, incluyendo las cuentas de un nivel
    determinado.
*/
class BalanceTreeView : public QWidget, public Ui_BalanceTreeBase {
    Q_OBJECT

private:
    /// Empresa con la que trabaja la clase.
    empresa *m_companyact;
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
    BalanceTreeView(empresa *, QWidget *parent = 0, int flags = 0);
    void inicializa1(QString, QString, QString, QString, int);
    ~BalanceTreeView();

private:
    void presentar();

private slots:
    virtual void contextmenu(Q3ListViewItem *, const QPoint &, int);
    virtual void nivelactivated(int);
    virtual void nivelactivated1(int, Q3ListViewItem *);
    virtual void boton_extracto1(int);
    virtual void boton_asiento();
    virtual void boton_diario1(int);

public slots:
    virtual void on_mui_actualizar_clicked() {
        presentar();
    };
    virtual void on_mui_imprimir_clicked() {
        boton_imprimir();
    };
    virtual void boton_imprimir();
    virtual void accept() {
        presentar();
    };
};

#endif

