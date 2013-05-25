/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociación Iglues -- Contabilidad Linux         *
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

#ifndef BCCAMBIACUENTAVIEW_H
#define BCCAMBIACUENTAVIEW_H

#include "ui_bccambiacuentabase.h"
#include "blwidget.h"

class BcCompany;


class BC_EXPORT BcCambiaCuentaView : public QDialog, public Ui_BcCambiaCuentaBase, public BlMainCompanyPointer
{
    Q_OBJECT
public:
    BcCambiaCuentaView ( BcCompany *, QWidget *, Qt::WindowFlags flag = 0 );
    ~BcCambiaCuentaView();

public slots:
    virtual void accept();

};

#endif

