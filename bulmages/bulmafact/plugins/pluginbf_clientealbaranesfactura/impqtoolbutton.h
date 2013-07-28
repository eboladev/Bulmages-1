/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#include <QtWidgets/QWidget>
#include <QtWidgets/QToolButton>
#include <QtCore/QString>
#include "bfcompany.h"
#include "blfunctions.h"
#include <QtXml/QDomDocument>
#include <QtCore/QMap>
#include <QtCore/QString>

#include "pedidosclientelist.h"
#include "presupuestolist.h"
#include "albaranclientelist.h"
#include "facturaslist.h"


class ImpQToolButton : public QToolButton
{
    Q_OBJECT

private:
    BfCompany *m_companyact;
    PresupuestoList *m_presupuestoList;
    PedidosClienteList *m_pedidosClienteList;
    AlbaranClienteList *m_albaranClienteList;
    FacturasList    *m_facturasList;

public:
    ImpQToolButton ( PresupuestoList *pres = NULL, PedidosClienteList *ped = NULL, AlbaranClienteList *alb = NULL,  FacturasList *fac = NULL , QWidget *parent = NULL );
    ~ImpQToolButton();
    void setBoton();

public slots:
    virtual void click();

};

