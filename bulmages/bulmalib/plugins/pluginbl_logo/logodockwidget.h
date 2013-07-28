/***************************************************************************
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


#ifndef LOGODOCK_H
#define LOGODOCK_H

#include <QtWidgets/QWidget>
#include <QtCore/QString>
#include <QtGui/QPixmap>

#include "bldockwidget.h"


class LogoDockWidget : public BlDockWidget
{
  private:
    QLabel *m_label;
    QPixmap m_originalPixmap;
    
  public:
    LogoDockWidget( const QString &title, QWidget * parent = 0, const QString &name = "", Qt::WindowFlags flags = 0 );
    ~LogoDockWidget();
    void setPixmap(QPixmap pixmap);
  
  protected:
    void resizeEvent(QResizeEvent *event);
};

#endif