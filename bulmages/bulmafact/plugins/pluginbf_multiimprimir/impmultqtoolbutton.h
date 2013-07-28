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
#ifndef IMPMULTQTOOLBUTTON_H
#define IMPMULTQTOOLBUTTON_H

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
#include "facturaslist.h"
#include "albaranclientelist.h"
#include "cobroslist.h"
#include "blformlist.h"
#include "reciboview.h"


class ImpQToolButton : public QToolButton
{
    Q_OBJECT

private:
    BfCompany *m_companyact;
    PresupuestoList *m_presupuestoList;
    PedidosClienteList *m_pedidosClienteList;
    AlbaranClienteList *m_albaranClienteList;
    FacturasList    *m_facturasList;
    CobrosList     *m_cobrosList;
    BlFormList *m_listado;

public:
    ImpQToolButton ( PresupuestoList *pres = NULL, PedidosClienteList *ped = NULL, AlbaranClienteList *alb = NULL,  FacturasList *fac = NULL , CobrosList *cob = NULL, QWidget *parent = NULL );
    ~ImpQToolButton();
    void setBoton();
public slots:
    virtual void click();

};


class SelQToolButton : public QToolButton
{
    Q_OBJECT

private:
    BfCompany *m_companyact;
    PresupuestoList *m_presupuestoList;
    PedidosClienteList *m_pedidosClienteList;
    AlbaranClienteList *m_albaranClienteList;
    FacturasList    *m_facturasList;
    CobrosList    *m_cobrosList;
    BlFormList *m_listado;
public:
    SelQToolButton ( PresupuestoList *pres = NULL, PedidosClienteList *ped = NULL, AlbaranClienteList *alb = NULL,  FacturasList *fac = NULL , CobrosList *cob = NULL,  QWidget *parent = NULL );
    ~SelQToolButton();
    void setBoton();
public slots:
    virtual void click();

};

class EmailQToolButton : public QToolButton
{
    Q_OBJECT

private:
    BfCompany *m_companyact;
    PresupuestoList *m_presupuestoList;
    PedidosClienteList *m_pedidosClienteList;
    AlbaranClienteList *m_albaranClienteList;
    FacturasList    *m_facturasList;
    CobrosList *m_cobrosList;
    BlFormList *m_listado;

public:
    EmailQToolButton ( PresupuestoList *pres = NULL, PedidosClienteList *ped = NULL, AlbaranClienteList *alb = NULL,  FacturasList *fac = NULL, CobrosList *cob = NULL , QWidget *parent = NULL );
    ~EmailQToolButton();
    void setBoton();
public slots:
    virtual void click();

};


class ProcesarQToolButton : public QToolButton
{
    Q_OBJECT

private:
    BfCompany *m_companyact;
    PresupuestoList *m_presupuestoList;
    PedidosClienteList *m_pedidosClienteList;
    AlbaranClienteList *m_albaranClienteList;
    FacturasList    *m_facturasList;
    CobrosList *m_cobrosList;
    BlFormList *m_listado;

public:
    ProcesarQToolButton ( PresupuestoList *pres = NULL, PedidosClienteList *ped = NULL, AlbaranClienteList *alb = NULL,  FacturasList *fac = NULL, CobrosList *cob = NULL , QWidget *parent = NULL );
    ~ProcesarQToolButton();
    void setBoton();
public slots:
    virtual void click();

};



class SumarQToolButton : public QToolButton
{
    Q_OBJECT

private:
    BfCompany *m_companyact;
    PresupuestoList *m_presupuestoList;
    PedidosClienteList *m_pedidosClienteList;
    AlbaranClienteList *m_albaranClienteList;
    FacturasList    *m_facturasList;
    CobrosList *m_cobrosList;
    BlFormList *m_listado;

public:
    SumarQToolButton ( PresupuestoList *pres = NULL, PedidosClienteList *ped = NULL, AlbaranClienteList *alb = NULL,  FacturasList *fac = NULL, CobrosList *cob = NULL , QWidget *parent = NULL );
    ~SumarQToolButton();
    void setBoton();
public slots:
    virtual void click();

};



#endif
