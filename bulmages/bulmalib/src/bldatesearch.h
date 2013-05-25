/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#ifndef BLDATESEARCH_H
#define BLDATESEARCH_H

#include <QtWidgets/QLineEdit>

#include "blfunctions.h"
#include "blmaincompanypointer.h"
#include "blwidget.h"
#include "ui_bldatesearchbase.h"


class BL_EXPORT BlDateSearch : public BlWidget, public Ui_BlDateSearchBase
{
    Q_OBJECT

public:
    BlDateSearch ( QWidget *parent = 0 );
    ~BlDateSearch();
    virtual void setDate ( QString val );
    virtual void setText ( QString val );
    virtual void setFieldValue ( QString val );
    virtual QString fecha();
    virtual QString text();
    virtual QString fieldValue();
    virtual QDate date();

public slots:
    virtual void s_searchFecha();
    virtual void s_fechatextChanged ( const QString & );
    virtual void s_fechalostFocus();
    virtual void s_returnPressed();
    virtual void selectAll();

signals:

    /// Esta se&ntilde;al se emite siempre que el valor de la fecha sea modificada,
    /// ya sea programadamente (setText) o por el usuario.
    void valueChanged ( QString );

    void returnPressed();

    /// Esta se&ntilde;al se emite cuando el usuario cambia la fecha ya sea
    /// escribiendo o seleccionando en el calendario.
    /// No se activar&aacute; si el cambio se realiz&oacute; programadamente.
    void editingFinished();
};

#endif

