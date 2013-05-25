/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef ALMACENESLISTVIEW_H
#define ALMACENESLISTVIEW_H

#include <QtWidgets/QLineEdit>

#include "bfcompany.h"
#include "blformlist.h"
#include "ui_almaceneslistbase.h"
#include "pdefs_pluginbf_almacenamplia.h"


/// Muestra y administra la ventana con el listado de almacenes.
/** */
class PLUGINBF_ALMACENAMPLIA_EXPORT AlmacenesListView : public BlFormList, public Ui_AlmacenesListBase
{
    Q_OBJECT

private:
    QString m_idalmacen;

public:
    AlmacenesListView ( QWidget *parent = 0, Qt::WindowFlags flag = 0 );
    AlmacenesListView ( BfCompany *comp = NULL, QWidget *parent = 0, Qt::WindowFlags flag = 0 );
    ~AlmacenesListView();
    void presentar();
    void editar ( int );
    void remove();
    void crear();
    QString idalmacen();
    void setMainCompany ( BfCompany *comp );
    void imprimir();
    const QString generaFiltro();
    void iniciaForm();

signals:
    void selected ( QString );
};

#endif

