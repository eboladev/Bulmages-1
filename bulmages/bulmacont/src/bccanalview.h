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

#ifndef BCCANALVIEW_H
#define BCCANALVIEW_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QCloseEvent>
#include <QTextStream>

#include "ui_bccanalbase.h"
#include "bldialogchanges.h"
#include "bcform.h"


class BcCompany;


class BC_EXPORT BcCanalView : public BcForm, public Ui_BcCanalBase
{
    Q_OBJECT

private:
    QLineEdit *m_nomCanal;
    QTextEdit *m_descCanal;

public:
    int canales[100];
    /// Indica cual es el canal que se esta visualizando.
    int idcanal;
    /// Si su valor es 0 entonces es que no se esta visualizando ningun centro de coste.


public:
    BcCanalView ( BcCompany *, QWidget *parent = 0 );
    ~BcCanalView();
    void mostrarPlantilla();
    void pintar();
    void on_mui_borrar_clicked();

public slots:
    virtual void on_mui_idCanal_valueChanged ( QString );
    virtual void on_mui_guardar_clicked();
    virtual void on_mui_crear_clicked();
    virtual void closeEvent ( QCloseEvent * );
};

#endif

