/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
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

#include "datosview.h"


///
/**
\param parent
**/
datosView::datosView(QWidget *parent) : QDialog(parent){
    _depura("datosView::datosView", 0);

    setupUi(this);

    _depura("END datosView::datosView", 0);
}


///
/**
**/
datosView::~datosView() {
    _depura("END datosView::~datosView", 0);
    _depura("datosView::~datosView", 0);
}


///
/**
**/
void datosView::on_mui_cancelar_clicked() {
    _depura("datosView::on_mui_cancelar_clicked", 0);
    close();
    setResult(Rejected);
    _depura("END datosView::on_mui_cancelar_clicked", 0);
}


///
/**
**/
void datosView::on_mui_aceptar_clicked() {
    _depura("datosView::on_mui_aceptar_clicked", 0);
    close();
    setResult(Accepted);
    _depura("END datosView::on_mui_aceptar_clicked", 0);
}

