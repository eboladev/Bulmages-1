/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#include <QWidget>
#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QList>
#include <QMenu>
#include <QAction>


#include "fixed.h"
#include "empresabase.h"
#include "dbrecord.h"
#include "eqtoolbutton.h"
#include "funcaux.h"

///
/**
\param parent
**/
EQToolButton::EQToolButton(QWidget *parent) : QWidget(parent) {
    _depura("EQToolButton::EQToolButton", 0);
      connect (parent, SIGNAL(pintaMenu(QMenu *)), this, SLOT(pintaMenu(QMenu *)));

    _depura("END EQToolButton::EQToolButton", 0);
}


///
/**
**/
EQToolButton::~EQToolButton() {
    _depura("EQToolButton::~EQToolButton", 0);
    _depura("END EQToolButton::~EQToolButton", 0);
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void EQToolButton::pintaMenu(QMenu *menu) {
    _depura("EQToolButton::pintaMenu", 0);
    QAction *ajust = menu->addAction(tr("Impresion Informes Personales"));
}


