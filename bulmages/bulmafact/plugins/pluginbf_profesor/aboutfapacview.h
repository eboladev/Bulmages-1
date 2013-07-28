/***************************************************************************
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#ifndef ABOUTFAPACVIEW_H
#define ABOUTFAPACVIEW_H

#include <QtWidgets/QDialog>

#include "blfunctions.h"
#include <ui_aboutfapacbase.h>


class AboutFapacView : public QDialog, private Ui::AboutFapacBase
{
    Q_OBJECT

public:
    /// Constructor de la clase que hace todas las inicializaciones y muestra el dise&ntilde;o hecho en aboutfapacbase.ui.
    AboutFapacView ( QDialog *parent = 0 );
    /// Destructor de la clase. Destruye todas las reservas de memoria din&aacute;mica que se hayan podido realizar.
    ~AboutFapacView();
};

#endif

