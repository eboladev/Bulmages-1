/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
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

#ifndef CUENTAVIEW_H
#define CUENTAVIEW_H

#include <stdlib.h>

#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QCloseEvent>

#include "ui_cuentabase.h"
#include "postgresiface2.h"
#include "funcaux.h"
#include "dialogchanges.h"
#include "fichabc.h"


class Empresa;


/// Visualiza una cuenta.
/** */
class CuentaView : public FichaBc, public Ui_CuentaBase {
    Q_OBJECT

private:
    void guardarCuenta();
    void borrarCuenta();

public:
    int idcuenta;
    int idgrupos[100];
    /// Esta variable indica el n&uacute;mero de d&iacute;gitos que tiene la
    /// contabilidad en sus cuentas.
    unsigned int numdigitos;

public:
    CuentaView(Empresa *emp, QWidget *, Qt::WFlags fl = 0);
    ~CuentaView();
    int inicializa();
    int nuevacuenta(QString, int);
    void cuentanueva(QString);
    int cargacuenta(int);
    void cambiapadre(const QString &);
    void closeEvent(QCloseEvent *);
    virtual void on_mui_borrar_clicked();

public slots:
    virtual void codigo_ret();
    virtual void on_mui_guardar_clicked();
    virtual void on_mui_aceptar_clicked();
};

#endif

