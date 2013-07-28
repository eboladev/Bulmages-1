/***************************************************************************
 *   Copyright (C) 2011 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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


#ifndef FAMILIASTPV_H
#define FAMILIASTPV_H

#include <QtWidgets/QTreeWidgetItem>

#include "blwidget.h"
#include "blfunctions.h"
#include "ui_familiastpvbase.h"
#include "familiasview.h"


class FamiliasTPV : public BlWidget, public Ui_FamiliasTPVBase
{
    Q_OBJECT

private:
    FamiliasView *m_familiasview;
  
public:
    FamiliasTPV(QWidget *parent, Qt::WindowFlags fl = 0);
    ~FamiliasTPV();

public slots:
    void on_mui_seleccionarcolor_clicked();
};

#endif

