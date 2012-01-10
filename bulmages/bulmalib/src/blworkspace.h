/***************************************************************************
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
 *   http://www.iglues.org                                                 *
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

#ifndef BLWORKSPACE_H
#define BLWORKSPACE_H

#include <QObject>
#include <QWidget>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QWorkspace>

#include "blfunctions.h"
#include "blwindowlistdock.h"


#ifdef AREA_QMDI
class BL_EXPORT BlWorkspace : public QMdiArea
{

//    Q_OBJECT

public:
    BlWorkspace ( QWidget * );
    ~BlWorkspace();

    void addSubWindow ( QWidget * );
    QWidget *activeWindow () const;

//signals:
//    void deselectDockAll();
};


#else

class BL_EXPORT BlWorkspace : public QWorkspace
{
//    Q_OBJECT

public:
    BlWorkspace ( QWidget * );
    ~BlWorkspace();

    void addWindow ( QWidget * );
    void addSubWindow ( QWidget * );
};

#endif

#endif

