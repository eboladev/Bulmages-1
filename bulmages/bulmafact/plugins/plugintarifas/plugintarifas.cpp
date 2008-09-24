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
#include "listlvartarifaview.h"
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

    cli->addDBCampo ( "idtarifa", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "PluginTarifas", "plugintarifas" ) );

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
    bus->setIdTarifa ( "" );
    hboxLayout160->addWidget ( bus );

    /// 1) Comprueba si existe un 'layout' dentro de m_frameplugin
    /// 2) Si existe es porque otro 'plugin' ha metido 'widgets' y no se pueden tocar. Ponemos nuestros
    ///    'widgets' a continuacion. Si no existe lo creamos y ponemos el nuestro.
    if (cli->m_frameplugin->layout() == 0) {
	/// No existe layout creamos uno.
        cli->m_frameplugin->setLayout(hboxLayout160);
    } else {
	/// \TODO: Existe layout ponemos nuestros 'widgets' a continuacion.


    } // end if

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
    ListLTarifaView *l1 = new ListLTarifaView ( art );
    l1->setObjectName ( QString::fromUtf8 ( "ltarifas" ) );
    l1->setEmpresaBase ( art->empresaBase() );
    l1->cargar ( "0" );
    art->mui_tab->addTab ( l1, QApplication::translate ( "PluginTarifas", "Tarifas") );

    /// VARIACION DE TARIFAS

    /// Agregamos el subformulario de validaciones.
    SubForm2Bf *l = new SubForm2Bf ( art );

    /// Ponemos un delegate al subformulario para que coja los combos asignados.
    delete l->m_delegate;
    l->m_delegate = new QSubFormVarTarifaBfDelegate ( l );
    l->mui_list->setItemDelegate ( l->m_delegate );

    l->setObjectName ( QString::fromUtf8 ( "lvariaciontarifas" ) );
    l->setEmpresaBase ( art->empresaBase() );
    l->setDBTableName ( "variaciontarifa" );
    l->setDBCampoId ( "idarticulo" );
    l->addSHeader ( "idvariaciontarifa", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite , QApplication::translate ( "VariacionTarifa", "ID Variacion Tarifa" ) );
    l->addSHeader ( "idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, QApplication::translate ( "VariacionTarifa", "ID Articulo" ) );

    l->addSHeader ( "idtarifa", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNoView, QApplication::translate ( "VariacionTarifa", "ID Tarifa" ) );
    l->addSHeader ( "nomtarifa", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone, QApplication::translate ( "VariacionTarifa", "Tarifa" ) );

    l->addSHeader ( "idalmacen", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNoView, QApplication::translate ( "VariacionTarifa", "ID Almacen" ) );
    l->addSHeader ( "nomalmacen", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone, QApplication::translate ( "VariacionTarifa", "Almacen" ) );

    l->addSHeader ( "cantidadmayoroigualque", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, QApplication::translate ( "VariacionTarifa", "Cantidad Mayor o Igual que" ) );
    l->addSHeader ( "porcentajevariacion", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, QApplication::translate ( "VariacionTarifa", "Porcentaje Variacion" ) );

    l->setinsercion ( TRUE );
    l->setDelete ( TRUE );
    l->setSortingEnabled ( FALSE );
    art->dialogChanges_setQObjectExcluido ( l->mui_list );

    art->mui_tab->addTab ( l, "Variacion Tarifas" );

    /// Establece anchos de las columnas.
    l->setColumnWidth(1, 150);
    l->setColumnWidth(2, 250);
    l->setColumnWidth(3, 250);
    l->setColumnWidth(4, 200);

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
    ListLTarifaView *l1 = art->findChild<ListLTarifaView *> ( "ltarifas" );
    l1->cargar ( art->DBvalue ( "idarticulo" ) );

    /// Variacion de tarifas.
    SubForm2Bf *l = art->findChild<SubForm2Bf *> ( "lvariaciontarifas" );
    if ( l ) {
	QString SQLQuery = "SELECT * FROM variaciontarifa AS t1 LEFT JOIN (SELECT idtarifa, nomtarifa FROM tarifa) AS t2 ON t1.idtarifa = t2.idtarifa LEFT JOIN (SELECT idalmacen, nomalmacen FROM almacen) AS t3 ON t1.idalmacen = t3.idalmacen WHERE t1.idarticulo = " + art->DBvalue ( "idarticulo" ) + " ORDER BY t1.idtarifa, t1.idalmacen, t1.cantidadmayoroigualque";
        l->cargar ( SQLQuery );
    } // end if

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
        ListLTarifaView *l1 = art->findChild<ListLTarifaView *> ( "ltarifas" );
        l1->setColumnValue ( "idarticulo", art->DBvalue ( "idarticulo" ) );

    	/// Variacion de tarifas.
        SubForm2Bf *l = art->findChild<SubForm2Bf *> ( "lvariaciontarifas" );
        l->setColumnValue ( "idarticulo", art->DBvalue ( "idarticulo" ) );

        l1->guardar();
        l->guardar();

        return 0;
    } catch ( ... ) {
        _depura ( "Hubo un error al guardar las tarifas", 2 );
        return 0;
    } // end try
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
        ListLTarifaView *l1 = art->findChild<ListLTarifaView *> ( "ltarifas" );

    	/// Variacion de tarifas.
        SubForm2Bf *l = art->findChild<SubForm2Bf *> ( "lvariaciontarifas" );
        l->setColumnValue ( "idarticulo", art->DBvalue ( "idarticulo" ) );

        l1->borrar();
        l->borrar();
        return 0;
    } catch ( ... ) {
        _depura ( "Hubo un error al borrar las tarifas", 0 );
        throw - 1;
    }
}

