/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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
#include "blwidget.h"


class company;

/** Convierte en los SubForms los datos del tipo desctipo_iva en selectores del tipo QComboBox
    de esta forma la introduccion de tipos de IVA es sencilla.
    Esta clase se usa conjuntamente con SubForm2Bf para el cambio del editor
    estandar por un ComboBox */
class BusquedaTipoIVADelegate : public QComboBox, public PEmpresaBase {
    Q_OBJECT

private:
    /// Este cursor almacena el listado de series de factura para poder trabajar con ellas.
    cursor2 *m_cursorcombo;

public:
    BusquedaTipoIVADelegate(QWidget *parent = 0);
    ~BusquedaTipoIVADelegate();
    virtual void set(const QString &);
};

#endif

