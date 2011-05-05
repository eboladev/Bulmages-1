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

#include "pluginbf_tarifa.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "busquedatarifa.h"
#include "listltarifaview.h"
#include "listlvartarifaview.h"
#include "tarifalistview.h"


///
/**
**/
PluginBf_Tarifa::PluginBf_Tarifa() : BlMainCompanyPointer()
{
    blDebug ( "PluginBf_Tarifa::PluginBf_Tarifa", 0 );
    blDebug ( "END PluginBf_Tarifa::PluginBf_Tarifa", 0 );
}


///
/**
**/
PluginBf_Tarifa::~PluginBf_Tarifa()
{
    blDebug ( "PluginBf_Tarifa::~PluginBf_Tarifa", 0 );
    blDebug ( "END PluginBf_Tarifa::~PluginBf_Tarifa", 0 );
}


///
/**
**/
void PluginBf_Tarifa::elslot()
{
    blDebug ( "PluginBf_Tarifa::elslot", 0 );
    TarifaListView *tar = new TarifaListView ( ( ( BfCompany * ) mainCompany() ), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( tar );
    tar->show();
    blDebug ( "END PluginBf_Tarifa::elslot", 0 );
}


///
/**
\param bges
**/
void PluginBf_Tarifa::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "PluginBf_Tarifa::inicializa", 0 );
    /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
    m_bges = bges;
    setMainCompany ( bges->company() );

    /// Miramos si existe un menu Articulos
    QMenu *pPluginMenu = bges->newMenu ( _("&Articulos"), "menuArticulos", "menuMaestro" );
    pPluginMenu->addSeparator();


    QAction *planCuentas = new QAction ( _ ( "&Tarifas" ), 0 );
    planCuentas->setStatusTip ( _ ( "Tarifas" ) );
    planCuentas->setWhatsThis ( _ ( "Tarifas" ) );

    pPluginMenu->addAction ( planCuentas );
    connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    blDebug ( "END PluginBf_Tarifa::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de Entrada del plugin de tarifas\n", 0 );

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_VariacionTarifa", "0.11.1-0001");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_tarifa", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    PluginBf_Tarifa *plug = new PluginBf_Tarifa();
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
    blDebug ( "dentro del plugin", 0 );

    cli->addDbField ( "idtarifa", BlDbField::DbInt, BlDbField::DbNothing, _ ( "plugintarifas" ) );

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
    bus->setMainCompany ( cli->mainCompany() );
    bus->setIdTarifa ( "" );
    hboxLayout160->addWidget ( bus );

    /// 1) Comprueba si existe un 'layout' dentro de m_frameplugin
    /// 2) Si existe es porque otro 'plugin' ha metido 'widgets' y no se pueden tocar. Ponemos nuestros
    ///    'widgets' a continuacion. Si no existe lo creamos y ponemos el nuestro.
    if ( cli->m_frameplugin->layout() == 0 ) {
        /// No existe layout creamos uno.
        cli->m_frameplugin->setLayout ( hboxLayout160 );
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
    blDebug ( "ArticuloView_ArticuloView", 0 );
    ListLTarifaView *l1 = new ListLTarifaView ( art );
    l1->setObjectName ( QString::fromUtf8 ( "ltarifas" ) );
    l1->setMainCompany ( art->mainCompany() );
    l1->cargar ( "0" );
    art->mui_tab->addTab ( l1, _ ( "Tarifas" ) );

    /// VARIACION DE TARIFAS

    /// Agregamos el subformulario de validaciones.
    BfSubForm *l = new BfSubForm ( art );

    /// Ponemos un delegate al subformulario para que coja los combos asignados.
    delete l->m_delegate;
    l->m_delegate = new QSubFormVarTarifaBfDelegate ( l );
    l->mui_list->setItemDelegate ( l->m_delegate );

    l->setObjectName ( QString::fromUtf8 ( "lvariaciontarifas" ) );
    l->setMainCompany ( art->mainCompany() );
    l->setDbTableName ( "variaciontarifa" );
    l->setDbFieldId ( "idarticulo" );
    l->addSubFormHeader ( "idvariaciontarifa", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite , _ ( "ID variacion tarifa" ) );
    l->addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView, _ ( "ID articulo" ) );
    l->addSubFormHeader ( "idtarifa", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView, _ ( "ID tarifa" ) );
    l->addSubFormHeader ( "nomtarifa", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Tarifa" ) );
    l->addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView, _ ( "ID almacen" ) );
    l->addSubFormHeader ( "nomalmacen", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Almacen" ) );
    l->addSubFormHeader ( "cantidadmayoroigualque", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Cantidad mayor o igual que" ) );
    l->addSubFormHeader ( "porcentajevariacion", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Porcentaje variacion" ) );

    l->setInsert ( TRUE );
    l->setDelete ( TRUE );
    l->setSortingEnabled ( FALSE );
    art->dialogChanges_setQObjectExcluido ( l->mui_list );

    art->mui_tab->addTab ( l, _("Variacion de tarifas") );

    /// Establece anchos de las columnas.
    l->setColumnWidth ( 1, 150 );
    l->setColumnWidth ( 2, 250 );
    l->setColumnWidth ( 3, 250 );
    l->setColumnWidth ( 4, 200 );

    blDebug ( "END ArticuloView_ArticuloView", 0 );
    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_cargar ( ArticuloView *art )
{
    blDebug ( "ArticuloView_cargar", 0 );
    ListLTarifaView *l1 = art->findChild<ListLTarifaView *> ( "ltarifas" );
    l1->cargar ( art->dbValue ( "idarticulo" ) );

    /// Variacion de tarifas.
    BfSubForm *l = art->findChild<BfSubForm *> ( "lvariaciontarifas" );
    if ( l ) {
        QString SQLQuery = "SELECT * FROM variaciontarifa AS t1 LEFT JOIN (SELECT idtarifa, nomtarifa FROM tarifa) AS t2 ON t1.idtarifa = t2.idtarifa LEFT JOIN (SELECT idalmacen, nomalmacen FROM almacen) AS t3 ON t1.idalmacen = t3.idalmacen WHERE t1.idarticulo = " + art->dbValue ( "idarticulo" ) + " ORDER BY t1.idtarifa, t1.idalmacen, t1.cantidadmayoroigualque";
        l->cargar ( SQLQuery );
    } // end if

    blDebug ( "END ArticuloView_cargar", 0 );
    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_guardar_post ( ArticuloView *art )
{
    blDebug ( "ArticuloView_guardar_post", 0 );
    try {
        ListLTarifaView *l1 = art->findChild<ListLTarifaView *> ( "ltarifas" );
        l1->setColumnValue ( "idarticulo", art->dbValue ( "idarticulo" ) );

        /// Variacion de tarifas.
        BfSubForm *l = art->findChild<BfSubForm *> ( "lvariaciontarifas" );
        l->setColumnValue ( "idarticulo", art->dbValue ( "idarticulo" ) );

        l1->guardar();
        l->guardar();

        return 0;
    } catch ( ... ) {
        blDebug ( "Hubo un error al guardar las tarifas", 2 );
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
    blDebug ( "ArticuloView_borrar", 0 );
    try {
        ListLTarifaView *l1 = art->findChild<ListLTarifaView *> ( "ltarifas" );

        /// Variacion de tarifas.
        BfSubForm *l = art->findChild<BfSubForm *> ( "lvariaciontarifas" );
        l->setColumnValue ( "idarticulo", art->dbValue ( "idarticulo" ) );

        l1->borrar();
        l->borrar();
        return 0;
    } catch ( ... ) {
        blDebug ( "Hubo un error al borrar las tarifas", 0 );
        throw - 1;
    } // end try
}


///
/**
**/
int BfSubForm_BfSubForm ( BfSubForm *sub )
{
    blDebug ( "PluginTarifas BfSubForm_BfSubForm", 0 );
    /// Este codigo hace que cuando se cambie el campo cantidad de articulo de una linea salte el
    /// calculo del PVP en funcion del cliente y otros parametros.
    QObject::connect ( sub->m_delegate, SIGNAL ( cant_changed ( BlDbSubFormRecord * ) ), sub, SLOT ( calculaPVP ( BlDbSubFormRecord * ) ) );
    blDebug ( "END PluginTarifas BfSubForm_BfSubForm", 0 );
    return 0;
}


/// Busca el porcentaje de variacion de tarifa en funcion de la cantidad.
/**
**/
int BfSubForm_calculaPVP ( BfSubForm *sub )
{
    blDebug ( "PluginTarifas BfSubForm_calculaPVP", 0 );

    BlDbRecordSet *cur = NULL;

    QString cantactual = sub->m_registrolinea->dbValue ( "cant" + sub->tableName() ).replace ( ",", "." );
    QString pvpactual = sub->m_registrolinea->dbValue ( "pvp" + sub->tableName() ).replace ( ",", "." );
    QString variacionpvp;

    /// Comprueba que se tengan todos los datos para aplicar variacion de tarifas.
    if ( sub->idArticulo().isEmpty() || sub->idTarifa().isEmpty() || sub->idAlmacen().isEmpty() ) {
        blDebug ( "END PluginTarifas BfSubForm_calculaPVP -sin suficientes datos-", 0 );
        return 0;
    } else {
        cur = sub->mainCompany()->loadQuery ( "SELECT * FROM variaciontarifa WHERE idarticulo = " + sub->idArticulo() + " AND idtarifa = " + sub->idTarifa() + " AND idalmacen = " + sub->idAlmacen() + " AND cantidadmayoroigualque <= " + cantactual + " ORDER BY cantidadmayoroigualque DESC LIMIT 1" );
    } // end if

    /// Si no se devuelve ningun resultado no se aplica variacion a la tarifa.
    if ( cur->numregistros() > 0 ) {
        variacionpvp = cur->valor ( "porcentajevariacion" );

        /// Aplica al precio la variacion correspondiente.
        QString res = sub->mainCompany()->PGEval ( pvpactual + " * (1 + " + variacionpvp + " / 100)", 2 );
        sub->m_registrolinea->setDbValue ( "pvp" + sub->tableName(), res );
    } // end if

    blDebug ( "END PluginTarifas BfSubForm_calculaPVP", 0 );
    return 0;
}
