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
/*
Esta clase sirve de base para todos los componentes visuales de bulmages que acaban operando con la
base de datos y les proporciona la base de datos con la que trabajar.
Deriva de BlMainCompanyPointer para obtener los metodos de trabajo con la base de datos y de
QWidget para servir de base a todos los componentes visuales de Qt.
*/
#ifndef BLWIDGET_H
#define BLWIDGET_H

#include <QtWidgets/QWidget>
#include <QtGui/QCloseEvent>
#include <QtGui/QPainter>

#include <QtGui/QHideEvent>
#include <QtGui/QShowEvent>

#include "blmaincompanypointer.h"
#include "bldialogchanges.h"
#include "blfunctions.h"
#include "blmaincompany.h"


class BL_EXPORT BlWidget : public QWidget, public BlMainCompanyPointer
{
    Q_OBJECT
    Q_PROPERTY (QString descripcion READ descripcion WRITE setDescripcion)
    
private:
    QString m_descripcion;
    
protected:
    virtual void paintEvent(QPaintEvent *event);
    bool event ( QEvent *event );


public:
    BlWidget ( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
    BlWidget ( BlMainCompany *company, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
    virtual ~BlWidget();
    QString descripcion() const;
    void setDescripcion(const QString &);

signals:
    void showed(QObject * object = 0);
    void hided(QObject * object = 0);
};


extern void qt_x11_set_global_double_buffer(bool);

#endif

