/***************************************************************************
 *   Copyright (C) 2011 by Daniel Ripoll Osma                              *
 *   dripoll.public@gmail.com                                              *
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

/// Para asegurarnos la compilacion correcta, si no esta definido EXAMPLE_H lo definimos
#ifndef EXAMPLE_H
#define EXAMPLE_H

/// Depende de BlWidget y del archivo de ui autogenerado al compilar
#include "blwidget.h"
#include "ui_examplebase.h"


class BtCompany;


class ExampleView : public BlWidget, public Ui_ExampleBase
{
    /// Cargamos QObject, obligatorio para cualquier clase que herede de este
    Q_OBJECT

public:
    // Constructors:
    ExampleView ( BlMainCompany *emp, QWidget *parent );
    // Destructors:
    virtual ~ExampleView();
    
public slots:
    // Slots:
    int on_mui_button_example_clicked ( bool state );
};

#endif
