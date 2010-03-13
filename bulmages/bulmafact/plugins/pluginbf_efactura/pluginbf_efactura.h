/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#ifndef PLUGINBF_EFACTURA_H
#define PLUGINBF_EFACTURA_H

#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QObject>

#include "blapplication.h"
#include "bfbulmafact.h"
#include "bfcompany.h"
#include "facturasplist.h"
#include "blwidget.h"
#include "pdefs_pluginbf_efactura.h"


extern "C" PLUGINBF_EFACTURA_EXPORT int entryPoint ( BfBulmaFact * );
extern "C" PLUGINBF_EFACTURA_EXPORT int FacturaView_FacturaView ( FacturaView * );
extern "C" PLUGINBF_EFACTURA_EXPORT int FacturasProveedorList_FacturasProveedorList_Post ( FacturasProveedorList * );


class efacturabf : public QObject, BlMainCompanyPointer
{
    Q_OBJECT

public:
    efacturabf();
    ~efacturabf();
    void inicializa ( BfBulmaFact * );
    int FacturaView_FacturaView ( FacturaView * );
    int FacturasProveedorList_FacturasProveedorList_Post ( FacturasProveedorList * );

public slots:
    void elslot();
    void elslot1();
};


#endif

