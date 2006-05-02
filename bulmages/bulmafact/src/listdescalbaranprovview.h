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

#ifndef LISTDESCALBARANPROVVIEW_H
#define LISTDESCALBARANPROVVIEW_H


#include "subform2bf.h"
#include "company.h"


class ListDescuentoAlbaranProvView : public SubForm2Bf {
    Q_OBJECT
public:
    QString mdb_idalbaranp;
    ListDescuentoAlbaranProvView(QWidget *parent = 0);
    ~ListDescuentoAlbaranProvView() {};
public slots:
    virtual void cargar(QString idalbaranp) {
        _depura("ListDescuentoAlbaranProvView::cargar\n",0);
        mdb_idalbaranp = idalbaranp;
        cursor2 * cur= companyact()->cargacursor("SELECT * FROM dalbaranp WHERE idalbaranp="+mdb_idalbaranp);
        SubForm3::cargar(cur);
        delete cur;
    };

};






/*



#include <Q3Table>

#include "listdescalbaranprov.h"
#include "company.h"
#include "descalbaranprov.h"


class ListDescuentoAlbaranProvView : public Q3Table , public ListDescuentoAlbaranProv
{
	Q_OBJECT

public:
	ListDescuentoAlbaranProvView(QWidget *parent = 0, const char *name = 0);
	~ListDescuentoAlbaranProvView();
	virtual void pintaListDescuentoAlbaranProv();
	virtual void pintadescListDescuentoAlbaranProv(int);
	DescuentoAlbaranProv *lineaat(int);
	DescuentoAlbaranProv *lineaact();

public slots:
	virtual void valueBudgetLineChanged(int row, int col);
	virtual void contextMenu (int, int, const QPoint &);
	virtual void borradescalbaranact();
};


*/


#endif
