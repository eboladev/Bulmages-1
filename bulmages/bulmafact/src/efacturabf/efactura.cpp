/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
 *									   *
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

#include "QString"

#include "efactura.h"
#include "funcaux.h"
#include <QFileDialog>

EFactura::EFactura(QWidget *parent) : QWidget(parent) {
	setupUi(this);  // Para que el form se inicialice bien
}

EFactura::~EFactura() {}

void EFactura::on_mui_guardar_clicked() {
	_depura("EFactura::on_mui_guardar_clicked", 0);
	QString s = QFileDialog::getSaveFileName(
                    this,
                    "Choose a filename to save under",
                    "/home",
                    "Images (*.png *.xpm *.jpg)");
}