/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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

#ifndef BbloqFecha_H
#define BbloqFecha_H

#include <QWidget>

#include "blfunctions.h"
#include "ui_bbloqfechabase.h"
#include "bcform.h"


class BcCompany ;


class miQTreeWidgetItem : public QTreeWidgetItem
{
public:
    miQTreeWidgetItem ( QTreeWidget *parent );
    miQTreeWidgetItem ( QTreeWidgetItem *parent );
    ~miQTreeWidgetItem();
    QString ej;
    QString per;
};


class BbloqFecha : public BcForm, public Ui_BBloqFechaBase
{
    Q_OBJECT

private:
    void inicializa();
    QString qsbloqueado;
    QString qsabierto;

public:
    BbloqFecha ( BcCompany *emp, QWidget * parent = 0 );
    ~BbloqFecha();

public slots:
    virtual void on_mui_crear_clicked();

private slots:
    virtual void on_mui_treeWidget_itemDoubleClicked ( QTreeWidgetItem *item, int columna );
};

#endif

