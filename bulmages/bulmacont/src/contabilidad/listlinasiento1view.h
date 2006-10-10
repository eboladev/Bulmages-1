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

#ifndef LISTLINASIENTO1VIEW_H
#define LISTLINASIENTO1VIEW_H

#include "subform2bc.h"
#include "fixed.h"


class ListLinAsiento1View : public SubForm2Bc {
    Q_OBJECT

public:
    ListLinAsiento1View(QWidget *parent = 0, const char *name = 0);
    ~ListLinAsiento1View() {}
    ;
    virtual void cargar(QString);
    Fixed totaldebe(QString);
    Fixed totalhaber(QString);

public slots:
    virtual void contextMenuEvent(QContextMenuEvent *);
    virtual void boton_iva();
};

#endif

