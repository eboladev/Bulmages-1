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

#ifndef PLUGINBC_BALANCEARBOL_H
#define PLUGINBC_BALANCEARBOL_H

#include "bcbulmacont.h"
#include "blpostgresqlclient.h"
#include "bccompany.h"
#include "blmaincompanypointer.h"
#include "pdefs_pluginbc_balancearbol.h"


extern "C" PLUGINBC_BALANCEARBOL_EXPORT int entryPoint ( BcBulmaCont * );


class myplugin4 : public QObject, BlMainCompanyPointer
{
    Q_OBJECT

public:
    BcBulmaCont *m_bulmacont;

public:
    myplugin4();
    ~myplugin4();
    void inicializa ( BcBulmaCont * );

public slots:
    void elslot();
};


#endif

