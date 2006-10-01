/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras                                    *
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

#include "msgerror.h"


msgError::msgError(QWidget *parent) : QDialog(parent) {
    setupUi(this);
    QObject::connect(pushButton1, SIGNAL(clicked(bool)), this, SLOT(close()));
    QObject::connect(pushButton2, SIGNAL(clicked(bool)), this, SLOT(s_mostarDetalles()));
}


msgError::msgError(QString mensaje, QString descripcion) {
    setupUi(this);
    hideFrame();
    m_mensaje->setText(mensaje);
    m_detalles->setPlainText(descripcion);
    exec();
}


msgError::~msgError() {}

