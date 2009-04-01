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

#include "facturaview.h"
#include "presupuestoview.h"
#include "pedidoclienteview.h"
#include "albaranclienteview.h"
#include "bulmafact.h"


extern "C" MY_EXPORT int AlbaranClienteView_AlbaranClienteView ( AlbaranClienteView * );
/*
extern "C" MY_EXPORT int PedidoClienteView_PedidoClienteView(PedidoClienteView *);
extern "C" MY_EXPORT int PresupuestoView_PresupuestoView(PresupuestoView *);
*/
extern "C" MY_EXPORT int FacturaView_FacturaView ( FacturaView * );
extern "C" MY_EXPORT int entryPoint ( Bulmafact * );


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
