/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef BLERRORDIALOG_H
#define BLERRORDIALOG_H

#include <QtWidgets/QDialog>

#include "blfunctions.h"
#include "ui_blerrordialogbase.h"


class BL_EXPORT BlErrorDialog : public QDialog, public Ui_BlErrorDialogBase
{
    Q_OBJECT

public:
    BlErrorDialog ( QWidget *parent = 0 );
    BlErrorDialog ( QString, QString );
    virtual ~BlErrorDialog();
    void hideFrame();
    void showFrame();

public slots:
    virtual void on_pushButton2_released();
};

#endif

