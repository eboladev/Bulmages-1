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

#ifndef LISTLINALBARANCLIENTEVIEW_H
#define LISTLINALBARANCLIENTEVIEW_H


#include <QEvent>

#include "company.h"
#include "subform2bf.h"
#include "fixed.h"


class ListLinAlbaranClienteView : public SubForm2Bf {
    Q_OBJECT
public:
    QString mdb_idalbaran;
    ListLinAlbaranClienteView(QWidget *parent = 0);
    ~ListLinAlbaranClienteView() {}
    ;
public slots:
    virtual void cargar(QString idalbaran);
    Fixed calculabase();
    Fixed calculaiva();
	virtual void editFinished(int, int);

};






/*

#include <Q3Table>
#include <QEvent>

#include "linalbarancliente.h"
#include "listlinalbarancliente.h"
#include "company.h"

class ListLinAlbaranClienteView : public Q3Table , public ListLinAlbaranCliente
{
	Q_OBJECT

public:
	ListLinAlbaranClienteView(QWidget *parent = 0, const char *name = 0);
	~ListLinAlbaranClienteView();
	virtual void pintaListLinAlbaranCliente();
	virtual void pintalinListLinAlbaranCliente(int);
	virtual bool eventFilter(QObject *obj, QEvent *ev);
	LinAlbaranCliente *lineaat(int);
	LinAlbaranCliente *lineaact();
	void cargaconfig();
	void guardaconfig();

public slots:
	virtual void valueBudgetLineChanged(int row, int col);
	virtual QString searchArticle();
	virtual void manageArticle(int row);
	virtual void contextMenu (int, int, const QPoint &);
	virtual void borraLinAlbaranClienteact();
};

*/
#endif
