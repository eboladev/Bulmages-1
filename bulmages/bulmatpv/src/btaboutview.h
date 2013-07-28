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

#ifndef BTABOUTVIEW_H
#define BTABOUTVIEW_H

#include <QtWidgets/QDialog>
#include <QtCore/QString>

#include "blfunctions.h"
#include "ui_btaboutbase.h"


/// Clase que muestra la ventana de informaci&oacute;n sobre el programa.
/** Es invocada desde el menu About y presenta la licencia, autores y entidades
    que dan soporte al programa.
    */
class BT_EXPORT BtAboutView : public QDialog, private Ui::BtAboutBase
{
    Q_OBJECT

public:
    /// Constructor de la clase que hace todas las inicializaciones y muestra el dise&ntilde;o hecho en about.ui.
    BtAboutView ( QDialog *parent = 0 );
    /// Destructor de la clase. Destruye todas las reservas de memoria din&aacute; dinamica que se hayan podido realizar.
    ~BtAboutView();
};

#endif

