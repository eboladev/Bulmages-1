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

#ifndef BCBLOQUEAFECHAVIEW_H
#define BCBLOQUEAFECHAVIEW_H

#include <QWidget>

#include "blfunctions.h"
#include "ui_bcbloqueafechabase.h"
#include "bcform.h"
#include "pdefs_pluginbc_bloqueofechas.h"


class BcCompany ;


class PLUGINBC_BLOQUEOFECHAS_EXPORT BcTreeWidgetItem : public QTreeWidgetItem
{
public:
    BcTreeWidgetItem ( QTreeWidget *parent );
    BcTreeWidgetItem ( QTreeWidgetItem *parent );
    ~BcTreeWidgetItem();
    QString ej;
    QString per;
};


class PLUGINBC_BLOQUEOFECHAS_EXPORT BcBloqueaFechaView : public BcForm, public Ui_BcBloqueaFechaBase
{
    Q_OBJECT

private:
    void inicializa();
    QString qsbloqueado;
    QString qsabierto;

public:
    BcBloqueaFechaView ( BcCompany *emp, QWidget * parent = 0 );
    ~BcBloqueaFechaView();

public slots:
    virtual void on_mui_crear_clicked();
    void on_mui_cerrar_clicked();

private slots:
    virtual void on_mui_treeWidget_itemDoubleClicked ( QTreeWidgetItem *item, int columna );
};

#endif

