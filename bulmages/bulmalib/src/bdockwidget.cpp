/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include <QObject>
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QVBoxLayout>

#include <map>

#include "funcaux.h"
#include "bdockwidget.h"

///
/**
\param w
**/
void BDockWidget::setWorkspace ( QWorkspace2 *w )
{
    _depura ( "BDockWidget::setWorkspace", 0 );
    m_pWorkspace = w;
    _depura ( "END BDockWidget::setWorkspace", 0 );
}


///
/**
\param a
**/
BDockWidget::BDockWidget ( const QString & title, QWidget * parent, Qt::WindowFlags flags ) : QDockWidget ( title, parent, flags )
{
    _depura ( "BDockWidget::BDockWidget", 0 );
    setFocusPolicy ( Qt::StrongFocus );

    _depura ( "END BDockWidget::BDockWidget", 0 );
}


///
/**
**/
BDockWidget::~BDockWidget()
{
    _depura ( "BDockWidget::~BDockWidget", 0 );
    _depura ( "END BDockWidget::~BDockWidget", 0 );
}



///
/**
\param visible
**/
void BDockWidget::cambiaVisible ( bool visible )
{
    _depura ( "BDockWidget::cambiaVisible", 0 );
    if ( visible == TRUE ) {
        this->show();
    } else {
        this->hide();
    } // end if
    _depura ( "END BDockWidget::cambiaVisible", 0 );
}


///
/**
**/
void BDockWidget::closeEvent ( QCloseEvent * )
{
    _depura ( "BDockWidget::closeEvent", 0 );
    emit ( cambiaEstadoVisible ( FALSE ) );
    _depura ( "END BDockWidget::closeEvent", 0 );
}

