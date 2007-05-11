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

#ifndef LISTDESCFACTURAPROVVIEW_H
#define LISTDESCFACTURAPROVVIEW_H

#include "subform2bf.h"
#include "company.h"


/// Muestra y administra la ventana de lista de descuentos por factura de proveedor.
/** */
class ListDescuentoFacturaProvView : public SubForm2Bf {
    Q_OBJECT

public:
    QString mdb_idfacturap;
    ListDescuentoFacturaProvView(QWidget *parent = 0);
    ~ListDescuentoFacturaProvView() {}
    ;

public slots:
    virtual void cargar(QString idfacturap) {
        _depura("ListCompArticulo::cargaListCompArticulo", 0);
        mdb_idfacturap = idfacturap;
        cursor2 * cur= empresaBase()->cargacursor("SELECT * FROM dfacturap WHERE idfacturap = " + mdb_idfacturap);
        SubForm3::cargar(cur);
        delete cur;
    };

};

#endif

