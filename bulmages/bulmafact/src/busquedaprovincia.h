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

#ifndef BUSQUEDAPROVINCIA_H
#define BUSQUEDAPROVINCIA_H

#include <QComboBox>

#include "company.h"
#include "postgresiface2.h"


/// Permite buscar y seleccionar una provincia.
/** Este Widget  permite buscar y seleccionar una  provincia. 
    Antes de usar el Widget debe ser inicializa con setcompany()
*/
/// \TODO: Comprobar que libere correctamente la memoria.
class BusquedaProvincia : public QComboBox {
    Q_OBJECT

private:
    /// Puntero a la clase company para poder trabajar con la base de datos. Debe inicializarse con setcompany().
    company *companyact;
    /// Cursor que almacena en memoria la lista de provincias.
    cursor2 *m_cursorcombo;

public:
    BusquedaProvincia(QWidget *parent = 0);
    ~BusquedaProvincia();
    void setcompany(company *comp);
    virtual void setProvincia(QString provincia);

public slots:
    void m_activated(int index);

signals:
    void valueChanged(QString);
};

#endif

