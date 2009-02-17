/***************************************************************************
 *   Copyright (C) 2004 by Alvaro de Miguel                                *
 *   alvaro_demiguel@gmail.com                                             *
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

#ifndef PROMEDIOVIEW_H
#define PROMEDIOVIEW_H

#include "blfunctions.h"
#include "ui_promediobase.h"
#include "blpostgresqlclient.h"
#include "dialogchanges.h"
#include "bldb.h"


class BfCompany;


class PromedioView : public QWidget, public Ui_PromedioBase, public dialogChanges, public BlDbRecord
{
    Q_OBJECT

private:
    BfCompany *m_companyact;

public:
    PromedioView ( BfCompany *emp, QWidget *parent = 0 );
    ~PromedioView();
    BfCompany *companyact() {
        return m_companyact;
    };

public:
    int cargar ( QString );
};

#endif
