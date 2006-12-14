/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
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

#ifndef CUENTAVIEW_H
#define CUENTAVIEW_H

#include <stdlib.h>

#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>

#include "ui_cuentabase.h"
#include "postgresiface2.h"
#include "funcaux.h"
#include "dialogchanges.h"


class empresa;


/// Visualiza una cuenta.
/** */
class cuentaview : public QWidget, public Ui_CuentaBase, dialogChanges {
    Q_OBJECT

public:
    empresa *empresaactual;
    int idcuenta;
    int idgrupos[100];
    postgresiface2 *conexionbase;
    unsigned int numdigitos; /// Esta variable indica el n&uacute;mero de d&iacute;gitos que tiene la contabilidad en sus cuentas.

public:
    cuentaview(empresa *emp, QWidget *, Qt::WFlags fl = 0);
    ~cuentaview();
    int inicializa();
    int nuevacuenta(QString, int);
    void cuentanueva(QString);
    int cargacuenta(int);
    void cambiapadre(const QString &);
    void closeEvent(QCloseEvent *);

public slots:
    virtual void codigo_ret();
    virtual void saveAccount();
    virtual void deleteAccount();
    virtual void on_mui_aceptar_clicked() {
        saveAccount();
        close();
    };
};

#endif

