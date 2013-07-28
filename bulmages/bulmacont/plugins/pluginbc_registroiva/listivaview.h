/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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
#ifdef TOMEU


#ifndef LISTIVAVIEW_H
#define LISTIVAVIEW_H

#include <QtWidgets/QTableWidget>
#include <QtWidgets/QMenu>
#include <QtCore/QEvent>

#include "listiva.h"
#include "iva.h"


class BcCompany;


class ListIvaView : public QTableWidget, public ListIva
{
    Q_OBJECT

public:
    ListIvaView ( QWidget *parent = 0 );
    ~ListIvaView();
    virtual void pintaListIva();
    virtual void pintaIva ( int );
    virtual bool eventFilter ( QObject *obj, QEvent *ev );
    Iva *lineaat ( int );
    Iva *lineaact();
    void saveConfig();
    void loadConfig();

public slots:
    virtual void valueBudgetLineChanged ( int row, int col );
    virtual void contextMenu ( int, int, const QPoint & );
    virtual void borraIvaAct();
};

#endif

#endif