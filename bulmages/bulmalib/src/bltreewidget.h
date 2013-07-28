/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2010 by Fco. Javier M. C.                               *
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

#ifndef BLTREEWIDGET_H
#define BLTREEWIDGET_H

#include <QtWidgets/QTreeWidget>
#include "blfunctions.h"


class BL_EXPORT BlTreeWidget : public QTreeWidget {
    Q_OBJECT

public:
    BlTreeWidget ( QWidget * parent = 0 );
    ~BlTreeWidget ();

    virtual void createMenu ( QMenu * );
    virtual void execMenuAction ( QAction * );

public slots:
    virtual void contextMenuEvent ( QContextMenuEvent * );

signals:
    void pintaMenu ( QMenu * );
    void trataMenu ( QAction * );

};

#endif

