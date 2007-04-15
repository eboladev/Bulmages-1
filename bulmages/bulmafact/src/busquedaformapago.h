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

#ifndef BUSQUEDAFORMA_PAGO_H
#define BUSQUEDAFORMA_PAGO_H

#include <QComboBox>

#include "company.h"
#include "postgresiface2.h"
#include "funcaux.h"


/// Permite buscar y seleccionar una forma de pago.
/** Widget que permite buscar y seleccionar una
    forma de pago. Aparece en forma de comboBox. */
class BusquedaFormaPago : public QComboBox {
    Q_OBJECT

private:
    /// Puntero a la clase company para poder trabajar con la Base de Datos y pasar mensajes.
    company *companyact;
    /// Cursor de la base de datos que contiene las formas de pago disponibles.
    cursor2 *m_cursorcombo;

public:
    BusquedaFormaPago(QWidget *parent = 0);
    ~BusquedaFormaPago();
    void setcompany(company *comp);
    virtual void setidforma_pago(QString idforma_pago);
    QString idforma_pago();
    virtual void setIdCliente(QString);
    virtual void setIdProveedor(QString);
public slots:
    virtual void m_activated(int index);

signals:
    void valueChanged(QString);
};

#endif

