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

#ifndef INVENTARIOSVIEW_H
#define INVENTARIOSVIEW_H

#include <Q3Frame>

#include "inventariosbase.h"
#include "company.h"


class InventariosView : public InventariosBase
{
	Q_OBJECT

private:
	company *companyact;

public:
	InventariosView(QWidget *parent = 0, const char *name = 0, Qt::WFlags flag = 0);
	InventariosView(company *,QWidget *parent = 0, const char *name = 0);
	~InventariosView();
	void inicializa();
	void setcompany (company *comp)
	{
		companyact = comp;
	};
	void meteWindow(QString nom, QObject *obj)
	{
		if (companyact != NULL)
		{
			companyact->meteWindow(nom, obj);
		}
	};

public slots:
	virtual void doubleclicked(int, int , int , const QPoint &);
	virtual void s_new()
	{
		companyact->s_newInventario();
	};
    	virtual void s_edit();
	virtual void s_refresh()
	{
		inicializa();
	};
	virtual void s_delete();
};

#endif
