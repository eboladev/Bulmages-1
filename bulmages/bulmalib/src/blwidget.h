/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#ifndef BLWIDGET_H
#define BLWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include <QPainter>

#include <QHideEvent>
#include <QShowEvent>

#include "blmaincompanypointer.h"
#include "bldialogchanges.h"
#include "blfunctions.h"
#include "blmaincompany.h"


class BL_EXPORT BlWidget : public QWidget, public BlMainCompanyPointer
{
    Q_OBJECT

protected:
    virtual void paintEvent(QPaintEvent *);

#ifdef AREA_QMDI
    bool event ( QEvent * event );
#endif

public:
    BlWidget ( QWidget *parent = 0, Qt::WFlags f = 0 );
    BlWidget ( BlMainCompany *, QWidget *parent = 0, Qt::WFlags f = 0 );
    virtual ~BlWidget();

signals:
    void showed(QObject * obj = 0);
    void hided(QObject * obj = 0);
};


extern void qt_x11_set_global_double_buffer(bool);

#endif

