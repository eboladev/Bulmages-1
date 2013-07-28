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

#ifndef BCCENTROCOSTEVIEW_H
#define BCCENTROCOSTEVIEW_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtGui/QCloseEvent>

#include "ui_bccentrocostebase.h"
#include "blpostgresqlclient.h"
#include "bldialogchanges.h"
#include "bcform.h"


class BcCompany;


/// Visualiza un centro de coste.
/** */
class BC_EXPORT BcCentroCosteView : public BcForm, public Ui_BcCentroCosteBase
{
    Q_OBJECT

public:
    int idc_coste; /// Indica cual es el centro de coste que se esta visualizando.
    /// Si su valor es 0 entonces es que no se esta visualizando ning&uacute;n centro de coste.

public:
    BcCentroCosteView ( BcCompany *, QWidget *parent = 0 );
    ~BcCentroCosteView();
    void repintar();
    void mostrarPlantilla();
    virtual void on_mui_borrar_clicked();
    virtual int save();

public slots:
    virtual void on_mui_crear_clicked();
    virtual void on_mui_list_itemClicked ( QTreeWidgetItem *, int );
//    virtual void closeEvent(QCloseEvent *);
};

#endif

