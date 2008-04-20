/***************************************************************************
 *   Copyright (C) 2008 by Tomeu Borras Riera                              *
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

#include "blprogressbar.h"
#include "funcaux.h"
#include <QDesktopWidget>

///
/**
**/
BLProgressBar::BLProgressBar (  ) : QWidget(0,0)
{
    _depura ( "BLProgressBar::BLProgressBar", 0 );
    setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    QDesktopWidget *pantalla = new QDesktopWidget();
    setGeometry ( ( pantalla->screenGeometry().width() / 2 ) - 150, ( pantalla->screenGeometry().height() / 2 ) -25, 300, 50 );
    mui_label->setText("");
    delete pantalla;
    _depura ( "END BLProgressBar::BLProgressBar", 0 );
}

///
/**
**/
void BLProgressBar::setText(const QString &text) {
    _depura ( "BLProgressBar::setText", 0 );
    mui_label->setText(text);
    _depura ( "END BLProgressBar::setText", 0 );
}


///
/**
**/
BLProgressBar::~BLProgressBar()
{
    _depura ( "BLProgressBar::~BLProgressBar", 0 );
    _depura ( "END BLProgressBar::~BLProgressBar", 0 );
}

///
/**
**/
void BLProgressBar::setValue(int value) { 
    _depura ( "BLProgressBar::setValue", 0 );
mui_progressBar->setValue(value);
    _depura ( "END BLProgressBar::setValue", 0 );

}

///
/**
**/
int BLProgressBar::value() {
    _depura ( "BLProgressBar::value", 0 );
    _depura ( "END BLProgressBar::value", 0 );
return mui_progressBar->value();
}

///
/**
**/
void BLProgressBar::setRange(int range,  int range1) {
    _depura ( "BLProgressBar::setRange", 0 );
mui_progressBar->setRange(range, range1);
    _depura ( "END BLProgressBar::setRange", 0 );
}

