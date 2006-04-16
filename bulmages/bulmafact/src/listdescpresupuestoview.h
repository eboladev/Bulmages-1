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

#ifndef LISTDESCPRESUPUESTOVIEW_H
#define LISTDESCPRESUPUESTOVIEW_H

#include "subform2bf.h"
#include "company.h"


class ListDescuentoPresupuestoView : public SubForm2Bf {
    Q_OBJECT
public:
    QString mdb_idpresupuesto;
    ListDescuentoPresupuestoView(QWidget *parent = 0);
    ~ListDescuentoPresupuestoView() {};

public slots:
    virtual void cargar(QString idpresupuesto) {
        _depura("ListDescuentoPresupuestoView::cargar\n",0);
        mdb_idpresupuesto = idpresupuesto;
        cursor2 * cur= companyact()->cargacursor("SELECT * FROM dpresupuesto WHERE idpresupuesto="+mdb_idpresupuesto);
        SubForm2::cargar(cur);
        delete cur;
    };

};

#endif
