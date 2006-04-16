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
#include "subform2bf.h"



class ListCompArticuloView : public SubForm2Bf {
Q_OBJECT
public:
	QString mdb_idarticulo;
	ListCompArticuloView(QWidget *parent = 0, const char *name = 0);
	~ListCompArticuloView() {};

public slots:
	virtual void cargar(QString idarticulo) {
    _depura("ListCompArticulo::cargaListCompArticulo\n",0);
    mdb_idarticulo = idarticulo;
    cursor2 * cur= companyact()->cargacursor("SELECT * FROM comparticulo, articulo WHERE comparticulo.idarticulo="+mdb_idarticulo+" AND articulo.idarticulo=comparticulo.idcomponente");
	SubForm2::cargar(cur);
    delete cur;
};
    virtual void editFinished(int row, int col);
    virtual void pressedAsterisk(int row, int col);
};


#endif
