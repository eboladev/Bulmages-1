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

#ifndef LISTCOMPARTICULOVIEW_H
#define LISTCOMPARTICULOVIEW_H

#include <Q3Table>
#include <QEvent>

#include "comparticulolist.h"
#include "company.h"
#include "comparticulo.h"
#include "qtable2.h"

class ListCompArticuloView : public QTableWidget2 , public ListCompArticulo
{
Q_OBJECT

public:
	ListCompArticuloView(QWidget *parent = 0, const char *name = 0);
	~ListCompArticuloView();
	virtual void pintar();
	virtual void pintar(int);
	virtual bool eventFilter(QObject *obj, QEvent *ev);
	CompArticulo *lineaat(int);
	CompArticulo *lineaact();
	void generar();

public slots:
	virtual QString searchArticle();
	virtual void manageArticle(int row);
        virtual void contextMenuEvent (QContextMenuEvent *);
	virtual void s_cellChanged(int row, int col);
	virtual void s_currentCellChanged(int row, int col, int prow, int pcol);
};

#endif
