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

#ifndef BUSQUEDATRABAJADOR_H
#define BUSQUEDATRABAJADOR_H

#include "blcombobox.h"
#include "company.h"
#include "blpostgresqlclient.h"
#include "blfunctions.h"
#include "blwidget.h"


/** Convierte en los SubForms los datos del tipo desctipo_iva en selectores del tipo QComboBox
    de esta forma la introduccion de tipos de IVA es sencilla.
    Esta clase se usa conjuntamente con SubForm2Bf para el cambio del editor
    estandar por un ComboBox */
class BusquedaTrabajadorDelegate : public BlComboBox
{
    Q_OBJECT

public:
    BusquedaTrabajadorDelegate ( QWidget *parent = 0 );
    ~BusquedaTrabajadorDelegate();
    virtual void set ( const QString & );
};


/// Permite buscar y seleccionar un trabajador.
/** Este comboBox permite buscar y seleccionar un
    trabajador. Aparece en forma de desplegable.
    Antes de utilizarlo debe ser inicializado con setcompany().
    Cuando se cambia el valor del Widget se emite un SIGNAL valueChanged().
*/
class BusquedaTrabajador : public BlComboBox
{
    Q_OBJECT

public:
    BusquedaTrabajador ( QWidget *parent = 0 );
    ~BusquedaTrabajador();
    virtual void setId ( QString idtrabajador );
    QString id();
    void m_activated ( int index );
};

#endif
