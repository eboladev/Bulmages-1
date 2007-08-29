/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef BUSQUEDAALMACEN_H
#define BUSQUEDAALMACEN_H

#include "qcombobox2.h"

#include "blwidget.h"
#include "company.h"
#include "postgresiface2.h"


/// Clase que sirve para seleccionar un almac&eacute;n.
/** Creamos un QComboBox que sirve para presentar la lista de almacenes
    disponibles para poder seleccionar uno de ellos.*/
class BusquedaAlmacen : public QComboBox2 {
    Q_OBJECT

private:
    /// El puntero a company para que se pueda trabajar con la base de datos.
    cursor2 *m_cursorcombo;
    /// Indica cual es el codigo de almacen por defecto.
    QString m_codigoalmacen;

public:
    BusquedaAlmacen(QWidget *parent = 0, const char *name = 0);
    ~BusquedaAlmacen();
    virtual void setidalmacen(QString idalmacen);
    virtual void setValorCampo(QString idalmacen);
    QString idalmacen();
    void setEmpresaBase(Company *);
    virtual QString valorCampo();

public slots:
    void m_activated(int index);

signals:
    void valueChanged(QString);
};

#endif

