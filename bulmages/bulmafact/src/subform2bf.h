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

#ifndef SUBFORM2BF_H
#define SUBFORM2BF_H

#include <Q3Table>
#include <QEvent>

#include "comparticulolist.h"
#include "company.h"
#include "comparticulo.h"
#include "qtable2.h"
#include "subform.h"



class SubForm2Bf : public SubForm2 {
Q_OBJECT
public:
	SubForm2Bf(QWidget *parent = 0);
	virtual ~SubForm2Bf() {};

public slots:
        virtual void contextMenuEvent (QContextMenuEvent *);
    virtual void editFinished(int row, int col);
    virtual void pressedSlash(int row, int col);
    virtual void pressedAsterisk(int row, int col);
};


#endif
