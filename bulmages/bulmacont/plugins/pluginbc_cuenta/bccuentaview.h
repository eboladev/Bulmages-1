/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
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

#ifndef BCCUENTAVIEW_H
#define BCCUENTAVIEW_H

#include <stdlib.h>

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QRadioButton>
#include <QtGui/QCloseEvent>

#include "ui_bccuentabase.h"
#include "blpostgresqlclient.h"
#include "blfunctions.h"
#include "bldialogchanges.h"
#include "bcform.h"


class BcCompany;


/// Visualiza una cuenta.
/** */
class BcCuentaView : public BcForm, public Ui_BcCuentaBase
{
    Q_OBJECT

public:
    BcCuentaView ( BcCompany *emp, QWidget *, Qt::WindowFlags fl = 0 );
    ~BcCuentaView();
    int nuevacuenta ( QString );
};

#endif

