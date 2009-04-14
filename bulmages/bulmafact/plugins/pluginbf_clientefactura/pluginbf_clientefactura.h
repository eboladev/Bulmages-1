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
 
#ifndef PLUGINBF_CLIENTEFACTURA_H
#define PLUGINBF_CLIENTEFACTURA_H
 

#include "pdefs_clientefactura.h"
#include "blpostgresqlclient.h"
#include "bfbulmafact.h"
#include "blwidget.h"
#include "bfbuscarreferencia.h"
#include "clienteview.h"
#include "facturaview.h"
#include "albaranclienteview.h"
#include "pedidoclienteview.h"
#include "presupuestoview.h"
#include "bfcompany.h"

extern "C" MY_EXPORT_CLIENTEFACTURA int SNewFacturaView (BfCompany *);
extern "C" MY_EXPORT_CLIENTEFACTURA int entryPoint ( BfBulmaFact * );
extern "C" MY_EXPORT_CLIENTEFACTURA int BfCompany_createMainWindows_Post(BfCompany *);
extern "C" MY_EXPORT_CLIENTEFACTURA int ClienteView_ClienteView_Post (ClienteView *);
extern "C" MY_EXPORT_CLIENTEFACTURA int ClienteView_cargarPost_Post (ClienteView *);
extern "C" MY_EXPORT_CLIENTEFACTURA int BfBuscarReferencia_on_mui_abrirtodo_clicked_Post (BfBuscarReferencia *);
extern "C" MY_EXPORT_CLIENTEFACTURA int AlbaranClienteView_AlbaranClienteView ( AlbaranClienteView * );
extern "C" MY_EXPORT_CLIENTEFACTURA int PedidoClienteView_PedidoClienteView ( PedidoClienteView * );
extern "C" MY_EXPORT_CLIENTEFACTURA int PresupuestoView_PresupuestoView ( PresupuestoView * );

 
class mypluginfact : public QObject, BlMainCompanyPointer
{
    Q_OBJECT

public:
    BfBulmaFact *m_bges;

public:
    mypluginfact();
    ~mypluginfact();
    void inicializa ( BfBulmaFact * );

public slots:
    void elslot();
    void elslot1();
};


#endif
