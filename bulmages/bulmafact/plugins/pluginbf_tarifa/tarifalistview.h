/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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

#ifndef TARIFALISTVIEW_H
#define TARIFALISTVIEW_H

#include "ui_tarifalistbase.h"
#include "bfcompany.h"
#include "blimportexport.h"
#include "blformlist.h"


class TarifaListView : public BlFormList, private Ui_TarifaListBase, public BlImportExport
{
    Q_OBJECT

public:
    TarifaListView ( BfCompany *comp, QWidget *parent = 0, Qt::WindowFlags flag = 0, edmode editmodo = BL_EDIT_MODE );
    ~TarifaListView();
    void editar(int);
    void remove();
    void crear();

public slots:
    void actualizar();
};

#endif

