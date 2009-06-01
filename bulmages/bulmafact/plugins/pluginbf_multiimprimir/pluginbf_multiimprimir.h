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

#include "facturaslist.h"
#include "presupuestolist.h"
#include "pedidosclientelist.h"
#include "albaranclientelist.h"
#include "cobroslist.h"
#include "reciboslist.h"
#include "bfbulmafact.h"


extern "C" MY_EXPORT int PresupuestoList_PresupuestoList ( PresupuestoList * );
extern "C" MY_EXPORT int PresupuestoListSubForm_PresupuestoListSubForm ( PresupuestoListSubForm * );
extern "C" MY_EXPORT int PedidosClienteList_PedidosClienteList ( PedidosClienteList * );
extern "C" MY_EXPORT int PedidosClienteListSubform_PedidosClienteListSubform ( PedidosClienteListSubform * );
extern "C" MY_EXPORT int AlbaranClienteList_AlbaranClienteList ( AlbaranClienteList * );
extern "C" MY_EXPORT int AlbaranClienteListSubform_AlbaranClienteListSubform ( AlbaranClienteListSubform * );
extern "C" MY_EXPORT int FacturasList_FacturasList ( FacturasList * );
extern "C" MY_EXPORT int FacturasListSubform_FacturasListSubform ( FacturasListSubform * );
extern "C" MY_EXPORT int CobrosList_CobrosList ( CobrosList * );
extern "C" MY_EXPORT int CobrosListSubForm_CobrosListSubForm ( CobrosListSubForm * );
extern "C" MY_EXPORT int RecibosListSubForm_RecibosListSubForm ( RecibosListSubForm * );
extern "C" MY_EXPORT int RecibosList_RecibosList ( RecibosList * );
extern "C" MY_EXPORT int entryPoint ( BfBulmaFact * );


// 

/*
class myplugin : public QObject {
    Q_OBJECT

public:
    BlPostgreSqlClient *dbConnection;

public:
    myplugin();
    ~myplugin();
    void inicializa(bulmafact *);

public slots:
    void elslot();
};

*/
