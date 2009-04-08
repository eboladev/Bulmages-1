/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#include "blwidget.h"


///
/**
\param parent
\param f
**/
BlWidget::BlWidget ( QWidget *parent, Qt::WFlags f )
        : QWidget ( parent, f ), BlMainCompanyPointer()
{
    _depura ( "BlWidget::BlWidget", 0 );
    _depura ( "END BlWidget::BlWidget", 0 );
}


///
/**
\param emp
\param parent
\param f
**/
BlWidget::BlWidget ( BlMainCompany *emp, QWidget *parent, Qt::WFlags f )
        : QWidget ( parent, f ), BlMainCompanyPointer ( emp )
{
    _depura ( "BlWidget::BlWidget", 0 );
    _depura ( "END BlWidget::BlWidget", 0 );
}


///
/**
**/
BlWidget::~BlWidget()
{
    _depura ( "BlWidget::~BlWidget", 0 );
    _depura ( "END BlWidget::~BlWidget", 0 );
}


/* Necesario para poner estilos a traves de hojas CSS.
*/
void BlWidget::paintEvent ( QPaintEvent * )
{
    QStyleOption opt;
    opt.initFrom ( this );
    QPainter p ( this );
    style()->drawPrimitive ( QStyle::PE_Widget, &opt, &p, this );
}

