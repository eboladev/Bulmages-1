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

#include <stdio.h>

#include "plugintarifas.h"
#include "company.h"
#include "funcaux.h"
#include "busquedatarifa.h"
#include "listltarifaview.h"
#include "tarifalistview.h"


///
/**
**/
myplugin1::myplugin1() : PEmpresaBase()
{
    _depura ( "myplugin1::myplugin1", 0 );
    _depura ( "END myplugin1::myplugin1", 0 );
}


///
/**
**/
myplugin1::~myplugin1()
{
    _depura ( "myplugin1::~myplugin1", 0 );
    _depura ( "END myplugin1::~myplugin1", 0 );
}


///
/**
**/
void myplugin1::elslot()
{
    _depura ( "myplugin1::elslot", 0 );
    TarifaListView *tar = new TarifaListView ( ( ( Company * ) empresaBase() ), NULL );
    empresaBase() ->m_pWorkspace->addWindow ( tar );
    tar->show();
    _depura ( "END myplugin1::elslot", 0 );
}


///
/**
\param bges
**/
void myplugin1::inicializa ( Bulmafact *bges )
{
    _depura ( "myplugin1::inicializa", 0 );
    /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
    m_bges = bges;
    setEmpresaBase ( bges->getcompany() );

    QAction *planCuentas = new QAction ( tr ( "&Tarifas" ), 0 );
    planCuentas->setStatusTip ( tr ( "Tarifas" ) );
    planCuentas->setWhatsThis ( tr ( "Tarifas" ) );
    bges->menuArticulos->addSeparator();
    bges->menuArticulos->addAction ( planCuentas );
    connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    _depura ( "END myplugin1::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de Entrada del plugin de Tarifas\n", 0 );
    myplugin1 *plug = new myplugin1();
    plug->inicializa ( bges );
    return 0;
}


///
/**
\param cli
\return
**/
int ClienteView_ClienteView ( ClienteView *cli )
{
    _depura ( "dentro del plugin", 0 );

    cli->addDBCampo ( "idtarifa", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "Identificador", "plugintarifas" ) );

    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing ( 2 );
    hboxLayout160->setMargin ( 0 );
    hboxLayout160->setObjectName ( QString::fromUtf8 ( "hboxLayout16" ) );

    QLabel *textLabel2_9_26 = new QLabel ( cli->m_frameplugin );
    textLabel2_9_26->setObjectName ( QString::fromUtf8 ( "textLabel2_9_2" ) );
    hboxLayout160->addWidget ( textLabel2_9_26 );
    textLabel2_9_26->setText ( "Tarifa" );

    BusquedaTarifa *bus = new BusquedaTarifa ( cli->m_frameplugin );
    bus->setObjectName ( QString::fromUtf8 ( "mui_idtarifa" ) );
    bus->setEmpresaBase ( cli->empresaBase() );
    bus->setidtarifa ( "" );
    hboxLayout160->addWidget ( bus );

    cli->vboxLayout2->addLayout ( hboxLayout160 );
    return 0;
}



///
/**
\param art
\return
**/
int ArticuloView_ArticuloView ( ArticuloView *art )
{
    _depura ( "ArticuloView_ArticuloView", 0 );
    ListLTarifaView *l = new ListLTarifaView ( art );
    l->setObjectName ( QString::fromUtf8 ( "ltarifas" ) );
    l->setEmpresaBase ( art->empresaBase() );
    l->cargar ( "0" );
    art->mui_tab->addTab ( l, "Tarifas" );
    _depura ( "END ArticuloView_ArticuloView", 0 );
    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_cargar ( ArticuloView *art )
{
    _depura ( "ArticuloView_cargar", 0 );
    ListLTarifaView *l = art->findChild<ListLTarifaView *> ( "ltarifas" );
    l->cargar ( art->DBvalue ( "idarticulo" ) );
    _depura ( "END ArticuloView_cargar", 0 );
    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_guardar_post ( ArticuloView *art )
{
    _depura ( "ArticuloView_guardar_post", 0 );
    try {
        ListLTarifaView *l = art->findChild<ListLTarifaView *> ( "ltarifas" );
        l->setColumnValue ( "idarticulo", art->DBvalue ( "idarticulo" ) );
        l->guardar();
        return 0;
    } catch ( ... ) {
        _depura ( "Hubo un error al guardar las tarifas", 2 );
        return 0;
    }
}


///
/**
\param art
\return
**/
int ArticuloView_borrar ( ArticuloView *art )
{
    _depura ( "ArticuloView_borrar", 0 );
    try {
        ListLTarifaView *l = art->findChild<ListLTarifaView *> ( "ltarifas" );
        l->borrar();
        return 0;
    } catch ( ... ) {
        _depura ( "Hubo un error al borrar las tarifas", 0 );
        throw - 1;
    }
}

