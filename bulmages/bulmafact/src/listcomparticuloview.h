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

#include "listcomparticulo.h"
#include "company.h"
#include "comparticulo.h"


class ListCompArticuloView : public Q3Table , public ListCompArticulo
{
	Q_OBJECT

public:
	ListCompArticuloView(QWidget *parent = 0, const char *name = 0);
	~ListCompArticuloView();
	virtual void pintaListCompArticulo();
	virtual void pintalinListCompArticulo(int);
	virtual bool eventFilter(QObject *obj, QEvent *ev);
	CompArticulo *lineaat(int);
	CompArticulo *lineaact();

public slots:
	virtual void valueListCompArticuloViewChanged(int row, int col);
	virtual QString searchArticle();
	virtual void manageArticle(int row);
	virtual void contextMenu (int, int, const QPoint &);
	virtual void borraCompArticuloact();
};

#endif
