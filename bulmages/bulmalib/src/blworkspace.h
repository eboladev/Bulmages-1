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


//#define AREA_QMDI

#ifdef AREA_QMDI
  #include <QMdiArea>
  #include <QMdiSubWindow>
#else
  #include <QWorkspace>
#endif



#include "blfunctions.h"
#include "blwindowlistdock.h"


#ifdef AREA_QMDI
  class BL_EXPORT BlWorkspace : public QMdiArea
#else
  class BL_EXPORT BlWorkspace : public QWorkspace
#endif
{
    Q_OBJECT

public:
    BlWorkspace ( QWidget * );
    ~BlWorkspace();
    
#ifdef AREA_QMDI    
    void addSubWindow ( QWidget * );
    QWidget *activeWindow () const;
#else
    void addWindow ( QWidget * );
    void addSubWindow ( QWidget * );
#endif

#ifdef AREA_QMDI    
  signals:
    void deselectDockAll();
#endif
  
};

#endif

