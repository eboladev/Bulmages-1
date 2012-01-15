/***************************************************************************
 *   Copyright (C) 2010 by Tomeu Borras Riera                              *
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

#include "bltoolbutton.h"
#include "blfunctions.h"
#include "blplugins.h"

///
/**
\param parent
\param f
**/
BlToolButton::BlToolButton ( QWidget *parent )        : QToolButton ( parent ), BlMainCompanyPointer()
{
    BL_FUNC_DEBUG
    connect (this, SIGNAL(released()), this, SLOT(buttonReleased()));
    
}


///
/**
\param emp
\param parent
\param f
**/
BlToolButton::BlToolButton ( BlMainCompany *emp, QWidget *parent )        : QToolButton ( parent ), BlMainCompanyPointer ( emp )
{
    BL_FUNC_DEBUG
    connect (this, SIGNAL(released()), this, SLOT(buttonReleased()));
    
}


///
/**
**/
BlToolButton::~BlToolButton()
{
    BL_FUNC_DEBUG
    
}

void BlToolButton::buttonReleased() {
    BL_FUNC_DEBUG
    g_plugins->run("BlToolButton_released", this);
    
}



/*
/// Desde los plugins (aun no se bien pq) no se pueden crear objetos definidos en la libreria. De hecho compila y linka pero acaba dando un error
/// De procedimiento no encontrado. El problema parecer ser debido a la reserva de memoria asi que si se hace el new dentro de la libreria llamante no
/// da problemas. Por eso este procedimiento.
/// Sino que se llama desde multiples partes del sistema.
BlToolButton * newBlToolButton ( BlMainCompany *emp, QWidget *parent, Qt::WFlags f )
{
    BlToolButton *h = new BlTooloButton ( emp, parent, f );
    return h;
}

*/