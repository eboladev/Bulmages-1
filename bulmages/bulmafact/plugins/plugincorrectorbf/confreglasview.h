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

#ifndef CONFREGLASVIEW_H
#define CONFREGLASVIEW_H

#include "blfunctions.h"
#include "ui_confreglasbase.h"


/** Permite configurar reglas que el corrector controle.
    Una funcionalidad extra para el corrector es permitir al usuario configurar determinadas
    reglas para que haga un control de las mismas.
    De esta forma el usuario puede controlar determinados detalles de la contabilidad que
    de otra forma le seria imposible controlar. */
class confreglasview : public QDialog, public Ui_confreglasbase
{
    Q_OBJECT

public:
    confreglasview ( QWidget *parent = 0 );
    ~confreglasview();
};

#endif

