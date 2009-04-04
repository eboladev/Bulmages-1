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

#include <QDesktopWidget>

#include "blprogressbar.h"
#include "blconfiguration.h"


///
/**
**/
BlProgressBar::BlProgressBar (  ) : QDialog ( 0, Qt::SplashScreen )
{
    _depura ( "BlProgressBar::BlProgressBar", 0 );
    if ( g_confpr->valor ( CONF_SHOW_PROGRESS_BAR ) == "TRUE" ) {
        setupUi ( this );
        QDesktopWidget *pantalla = new QDesktopWidget();
        setGeometry ( ( pantalla->screenGeometry().width() / 2 ) - this->width() / 2, ( pantalla->screenGeometry().height() / 2 ) - this->height() / 2, this->width(), this->height() );
        delete pantalla;
    } // end if
    _depura ( "END BlProgressBar::BlProgressBar", 0 );
}


///
/**
**/
void BlProgressBar::setText ( const QString &tex )
{
    _depura ( "BlProgressBar::setText", 0 );

    if ( g_confpr->valor ( CONF_SHOW_PROGRESS_BAR ) == "TRUE" ) {
        mui_label1->setText ( tex );
        /// Si no hacemos una espera no se pinta bien el setText.
        for ( int i = 0; i < 20; i++ )
            repaint();
    } // end if
    _depura ( "END BlProgressBar::setText", 0 );
}


///
/**
**/
BlProgressBar::~BlProgressBar()
{
    _depura ( "BlProgressBar::~BlProgressBar", 0 );
    _depura ( "END BlProgressBar::~BlProgressBar", 0 );
}


///
/**
**/
void BlProgressBar::setValue ( int value )
{
    _depura ( "BlProgressBar::setValue", 0 );
    if ( g_confpr->valor ( CONF_SHOW_PROGRESS_BAR ) == "TRUE" )
        mui_progressBar->setValue ( value );
    _depura ( "END BlProgressBar::setValue", 0 );

}


///
/**
**/
int BlProgressBar::value()
{
    _depura ( "BlProgressBar::value", 0 );
    _depura ( "END BlProgressBar::value", 0 );
    if ( g_confpr->valor ( CONF_SHOW_PROGRESS_BAR ) == "TRUE" ) {
        return mui_progressBar->value();
    } else {
        return 0;
    } // end if
}


///
/**
**/
void BlProgressBar::setRange ( int range,  int range1 )
{
    _depura ( "BlProgressBar::setRange", 0 );
    if ( g_confpr->valor ( CONF_SHOW_PROGRESS_BAR ) == "TRUE" )
        mui_progressBar->setRange ( range, range1 );
    _depura ( "END BlProgressBar::setRange", 0 );
}


///
/**
**/
void BlProgressBar::show (  )
{
    _depura ( "BlProgressBar::show", 0 );
    if ( g_confpr->valor ( CONF_SHOW_PROGRESS_BAR ) == "TRUE" ) {
        QWidget::show();
    } // end if
    _depura ( "END BlProgressBar::show", 0 );
}

