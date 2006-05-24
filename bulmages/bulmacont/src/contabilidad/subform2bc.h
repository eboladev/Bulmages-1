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

#ifndef SUBFORM2BC1_H
#define SUBFORM2BC1_H

//#include <Q3Table>
#include <QEvent>

#include "qtable2.h"
#include "subform.h"
#include "subform3.h"


class SubForm2Bc : public SubForm3 {
Q_OBJECT
protected:
    bool m_delete;

public:
    SubForm2Bc(QWidget *parent = 0);
    virtual ~SubForm2Bc() {}
    ;
	virtual int cargar(cursor2 *);

    void setDelete(bool f) {
        m_delete=f;
    };

   virtual void boton_asiento();
   virtual void boton_extracto1(int);
   virtual void boton_balance1(int);
   virtual void boton_balancetree(int);
   virtual void boton_diario1(int);

public slots:
    virtual void contextMenuEvent (QContextMenuEvent *);
    virtual void on_mui_list_editFinished(int row, int col);
    virtual void on_mui_list_pressedSlash(int row, int col);
    virtual void on_mui_list_pressedAsterisk(int row, int col);
};

#endif

