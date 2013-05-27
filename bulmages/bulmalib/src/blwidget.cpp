/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2010 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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
BlWidget::BlWidget ( QWidget *parent, Qt::WindowFlags flags )
        : QWidget ( parent, flags), BlMainCompanyPointer()
{
    BL_FUNC_DEBUG
    setAttribute(Qt::WA_StaticContents);
    m_descripcion = "";
}


///
/**
\param emp
\param parent
\param f
**/
BlWidget::BlWidget ( BlMainCompany *company, QWidget *parent, Qt::WindowFlags flags )
        : QWidget ( parent, flags ), BlMainCompanyPointer ( company )
{
    BL_FUNC_DEBUG
    m_descripcion = "";
    
}


///
/**
**/
BlWidget::~BlWidget()
{
    BL_FUNC_DEBUG
    
}


/* Necesario para poner estilos a traves de hojas CSS.
*/
void BlWidget::paintEvent ( QPaintEvent * )
{
    QStyleOption option;
    option.initFrom ( this );
    QPainter painter ( this );
    style()->drawPrimitive ( QStyle::PE_Widget, &option, &painter, this );
}


bool BlWidget::event ( QEvent * event )
{
    if (event->type() == QEvent::HideToParent) {
        emit hided(this);
    } else if (event->type() == QEvent::ShowToParent) {
        emit showed(this);
    } // end if

    return QWidget::event(event);
}


QString BlWidget::descripcion() const {
  return m_descripcion;
}

void BlWidget::setDescripcion(const QString &d) {
  m_descripcion = d;
}
