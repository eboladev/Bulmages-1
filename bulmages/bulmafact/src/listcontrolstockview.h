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

#ifndef LISTCONTROLSTOCKVIEW_H
#define LISTCONTROLSTOCKVIEW_H

#include <Q3Table>
#include <QEvent>

#include "listcontrolstock.h"
#include "company.h"
#include "controlstock.h"

#include "subform2bf.h"




class ListControlStockView : public SubForm2Bf {
    Q_OBJECT
public:
    QString mdb_idinventario;
    ListControlStockView(QWidget *parent = 0, const char *name = 0);
    ~ListControlStockView() {}
    ;

public slots:
    virtual void cargar(QString idinventario) {
        _depura("ListCompArticulo::cargaListCompArticulo\n",0);

        mdb_idinventario=idinventario;
	QString SQLQuery = "SELECT * FROM ";

	SQLQuery += " (SELECT * FROM articulo, almacen) AS t1 ";

	SQLQuery +=" LEFT JOIN (SELECT *, idarticulo AS idarticulopk, idalmacen AS idalmacenpk, idinventario AS idinventariopk FROM controlstock WHERE idinventario = "+idinventario+") AS t2 ON t1.idarticulo = t2.idarticulopk AND t1.idalmacen = t2.idalmacenpk ";
	SQLQuery += " ORDER BY codigoalmacen, codigocompletoarticulo";

        cursor2 * cur= companyact()->cargacursor(SQLQuery);
        SubForm2Bf::cargar(cur);
        delete cur;
    };
    virtual int guardar() {
	SubForm2Bf::guardar();
	cargar(mdb_idinventario);
	return 0;
    }
};



/*
 
class ListControlStockView : public Q3Table , public ListControlStock
{
	Q_OBJECT
 
public:
	ListControlStockView(QWidget *parent = 0, const char *name = 0);
	~ListControlStockView();
	virtual void pintaListControlStock();
	virtual void pintalinListControlStock(int);
	virtual bool eventFilter(QObject *obj, QEvent *ev);
	ControlStock *lineaat(int);
	ControlStock *lineaact();
 
public slots:
	virtual void valueBudgetLineChanged(int row, int col);
	virtual QString searchArticle();
	virtual void manageArticle(int row);
	virtual void manageAlmacen(int row);
	virtual void contextMenu (int, int ,const QPoint &);
	//virtual void borrar();
};
 
*/


#endif
