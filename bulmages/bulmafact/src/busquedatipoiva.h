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

#ifndef BUSQUEDATIPO_IVA_H
#define BUSQUEDATIPO_IVA_H

#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include "postgresiface2.h"



class company;


class BusquedaTipoIVADelegate : public QComboBox {
Q_OBJECT
private:
	company *m_companyact;
    /// Este cursor almacena el listado de series de factura para poder trabajar con ellas.
    cursor2 *m_cursorcombo;

public:
    BusquedaTipoIVADelegate(QWidget *parent = 0);
    ~BusquedaTipoIVADelegate();
    void setcompany(company *comp) {
        m_companyact = comp;
    };
    virtual void set(const QString &);
};



#endif

