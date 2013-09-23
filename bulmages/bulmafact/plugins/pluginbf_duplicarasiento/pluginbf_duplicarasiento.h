/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifndef PLUGINBF_DUPLICARASIENTO_H
#define PLUGINBF_DUPLICARASIENTO_H

#include "bfbulmafact.h"
#include "blpostgresqlclient.h"
#include "bfcompany.h"
#include "bcasientoview.h"
#include "pdefs_pluginbf_duplicarasiento.h"


extern "C" PLUGINBF_DUPLICARASIENTO_EXPORT int entryPoint ( BfBulmaFact * );
extern "C" PLUGINBF_DUPLICARASIENTO_EXPORT int BcAsientoView_BcAsientoView ( BcAsientoView * );


class PluginBc_DuplicarAsiento : public QToolButton, BlMainCompanyPointer
{
    Q_OBJECT

public:
    BcAsientoView *m_asiento;

public:
    PluginBc_DuplicarAsiento(BcAsientoView *as, BfCompany *comp, QWidget *parent = NULL);
    ~PluginBc_DuplicarAsiento();

public slots:
    void elslot();
};


#endif

