/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2012 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
 *                                                                         *
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

#ifndef BLBANKEDIT_H
#define BLBANKEDIT_H

#include <QtWidgets/QLineEdit>
#include "blfunctions.h"
#include "blwidget.h"
#include "ui_blbankeditbase.h"


class BL_EXPORT BlBankEdit : public BlWidget, public Ui_BlBankEditBase
{
    Q_OBJECT

public:
    BlBankEdit ( QWidget *parent = 0 );
    ~BlBankEdit();
    virtual void setText ( QString text );
    virtual void setFieldValue ( QString value );
    virtual QString text();
    virtual QString fieldValue();
    void checkControlDigit();

public slots:
    virtual void bankAccountChanged ( const QString & );
    virtual void bankAccountLostFocus();
    virtual void returnKeyPressed();
    virtual void selectAll();
    virtual void setFocus();

signals:
    void valueChanged ( QString );
    void returnPressed();
    void editingFinished();
};

#endif

