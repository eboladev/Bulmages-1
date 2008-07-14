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
#include "configuracion.h"
#include <QDesktopWidget>

///
/**
**/
BLProgressBar::BLProgressBar (  ) : QDialog ( 0, Qt::SplashScreen )
{
    _depura ( "BLProgressBar::BLProgressBar", 0 );
    if ( confpr->valor ( CONF_SHOW_PROGRESS_BAR ) == "TRUE" ) {
        setupUi ( this );
        QDesktopWidget *pantalla = new QDesktopWidget();
        setGeometry ( ( pantalla->screenGeometry().width() / 2 ) - this->width() / 2, ( pantalla->screenGeometry().height() / 2 ) - this->height() / 2, this->width(), this->height() );
        delete pantalla;
    } // end if
    _depura ( "END BLProgressBar::BLProgressBar", 0 );
}

///
/**
**/
void BLProgressBar::setText ( const QString &tex )
{
    _depura ( "BLProgressBar::setText", 0 );

    if ( confpr->valor ( CONF_SHOW_PROGRESS_BAR ) == "TRUE" ) {
        mui_label1->setText ( tex );
        /// Si no hacemos una espera no se pinta bien el setText.
        for ( int i = 0; i < 20; i++ )
            repaint();
    } // end if
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
void BLProgressBar::setValue ( int value )
{
    _depura ( "BLProgressBar::setValue", 0 );
    if ( confpr->valor ( CONF_SHOW_PROGRESS_BAR ) == "TRUE" )
        mui_progressBar->setValue ( value );
    _depura ( "END BLProgressBar::setValue", 0 );

}

///
/**
**/
int BLProgressBar::value()
{
    _depura ( "BLProgressBar::value", 0 );
    _depura ( "END BLProgressBar::value", 0 );
    if ( confpr->valor ( CONF_SHOW_PROGRESS_BAR ) == "TRUE" ) {
        return mui_progressBar->value();
    } else {
        return 0;
    } // end if
}

///
/**
**/
void BLProgressBar::setRange ( int range,  int range1 )
{
    _depura ( "BLProgressBar::setRange", 0 );
    if ( confpr->valor ( CONF_SHOW_PROGRESS_BAR ) == "TRUE" )
        mui_progressBar->setRange ( range, range1 );
    _depura ( "END BLProgressBar::setRange", 0 );
}

///
/**
**/
void BLProgressBar::show (  )
{
    _depura ( "BLProgressBar::show", 0 );
    if ( confpr->valor ( CONF_SHOW_PROGRESS_BAR ) == "TRUE" ) {
        QWidget::show();
    } // end if
    _depura ( "END BLProgressBar::show", 0 );
}
