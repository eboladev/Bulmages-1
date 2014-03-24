/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#ifndef COMERCIALCLIENTEVIEW_H
#define COMERCIALCLIENTEVIEW_H

#include "bfcompany.h"
#include "ui_comercialclientebase.h"
#include "bfform.h"


class ComercialClienteView : public BfForm, public Ui_ComercialClienteBase
{
    Q_OBJECT

public:
    void setMainCompany ( BfCompany *comp );
    ComercialClienteView ( BfCompany *comp, QWidget *parent = 0 );
    ~ComercialClienteView();
};

#endif

