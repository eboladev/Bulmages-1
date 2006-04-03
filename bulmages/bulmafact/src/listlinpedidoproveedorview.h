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

#ifndef LISTLINPEDIDOPROVEEDORVIEW_H
#define LISTLINPEDIDOPROVEEDORVIEW_H

#include <Q3Table>
#include <QEvent>


#include "company.h"
#include "subform2bf.h"
#include "fixed.h"

class ListLinPedidoProveedorView : public SubForm2Bf {
    Q_OBJECT
public:
    QString mdb_idpedidoproveedor;
    ListLinPedidoProveedorView(QWidget *parent = 0);
    ~ListLinPedidoProveedorView() {}
    ;
public slots:
    virtual void cargar(QString idpedidoproveedor) {
        _depura("ListLinPedidoProveedorView::cargar\n",0);
        mdb_idpedidoproveedor = idpedidoproveedor;
        cursor2 * cur= companyact()->cargacursor("SELECT * FROM lpedidoproveedor LEFT JOIN articulo ON lpedidoproveedor.idarticulo = articulo.idarticulo WHERE idpedidoproveedor="+mdb_idpedidoproveedor);
        SubForm2::cargar(cur);
        delete cur;
    };

    Fixed calculabase() {
	Fixed base("0.0");
        for (int i=0; i < rowCount()-1; i++) {
		Fixed totpar = Fixed(DBvalue("pvplpedidoproveedor",i)) * Fixed(DBvalue("cantlpedidoproveedor",i));
		base = base + totpar;
        }// end for
	return base;
    };


    Fixed calculaiva() {
	Fixed base("0.0");
        for (int i=0; i < rowCount()-1; i++) {
		Fixed totpar = Fixed(DBvalue("pvplpedidoproveedor",i)) * Fixed(DBvalue("ivalpedidoproveedor",i));
		base = base + totpar;
        }// end for
	return base;
};


	virtual void editFinished(int, int);

};



#endif
