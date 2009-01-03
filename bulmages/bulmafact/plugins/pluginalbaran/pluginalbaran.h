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

#ifdef Q_WS_WIN
# define MY_EXPORT __declspec(dllexport)
#else
# define MY_EXPORT
#endif

#include "postgresiface2.h"
#include "bulmafact.h"
#include "blwidget.h"
#include "articuloview.h"
#include "busquedareferencia.h"
#include "clienteview.h"
#include "facturaview.h"
#include "albaranclienteview.h"
#include "pedidoclienteview.h"
#include "presupuestoview.h"
#include "company.h"


extern "C" MY_EXPORT int SNewAlbaranClienteView (Company *);

extern "C" MY_EXPORT int entryPoint ( Bulmafact * );
extern "C" MY_EXPORT int Company_createMainWindows_Post(Company *);
extern "C" MY_EXPORT int ClienteView_ClienteView_Post (ClienteView *);

extern "C" MY_EXPORT int ClienteView_cargarPost_Post (ClienteView *);
extern "C" MY_EXPORT int BusquedaReferencia_on_mui_abrirtodo_clicked_Post (BusquedaReferencia *);
extern "C" MY_EXPORT int PedidoClienteView_PedidoClienteView ( PedidoClienteView * );
extern "C" MY_EXPORT int PresupuestoView_PresupuestoView ( PresupuestoView * );




// 
class mypluginalb : public QObject, PEmpresaBase
{
    Q_OBJECT

public:
    Bulmafact *m_bges;

public:
    mypluginalb();
    ~mypluginalb();
    void inicializa ( Bulmafact * );

public slots:
    void elslot();
    void elslot1();
};
