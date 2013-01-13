/***************************************************************************
 *   Copyright (C) 2011 by Fco. Javier M. C.                               *
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


#include <QColorDialog>

#include "articulotpv.h"


ArticuloTPV::ArticuloTPV(QWidget *parent, Qt::WFlags fl) : BlWidget (parent, fl)
{
    setupUi(this);
}


ArticuloTPV::~ArticuloTPV()
{
}


void ArticuloTPV::on_mui_seleccionarcolor_clicked()
{

    QColor coloractual;

    coloractual.setNamedColor(mui_colortpvarticulo->text());
    if (!coloractual.isValid()){
        coloractual.setNamedColor("#FFFFFF");
    }

    QColorDialog colordialog;
    QColor color = colordialog.getColor(coloractual, this);
    mui_colortpvarticulo->setText(color.name());
    
}
