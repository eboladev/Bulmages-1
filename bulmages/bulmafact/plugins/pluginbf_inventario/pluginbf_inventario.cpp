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

#include "pluginbf_inventario.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "inventariosview.h"


///
/**
**/
myplugininv::myplugininv()
{
    _depura ( "myplugininv::myplugininv", 0 );
    _depura ( "END myplugininv::myplugininv", 0 );
}


///
/**
**/
myplugininv::~myplugininv()
{
    _depura ( "myplugininv::~myplugininv", 0 );
    _depura ( "END myplugininv::~myplugininv", 0 );
}


///
/**
**/
void myplugininv::elslot()
{
    _depura ( "myplugininv::elslot", 0 );
    InventariosView *tar = new InventariosView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( tar );
    tar->show();
    _depura ( "END myplugininv::elslot", 0 );
}


///
/**
\param bges
**/
void myplugininv::inicializa ( Bulmafact *bges )
{
    _depura ( "myplugininv::inicializa", 0 );
    /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
    m_bges = bges;
    setMainCompany ( bges->getcompany() );

    /// Miramos si existe un menu Articulos
    QMenu *pPluginMenu = bges->newMenu ( "&Articulos", "menuArticulos", "menuMaestro" );
    pPluginMenu->addSeparator();


    QAction *planCuentas = new QAction ( _ ( "&Inventarios" ), 0 );
    planCuentas->setStatusTip ( _ ( "Inventarios" ) );
    planCuentas->setWhatsThis ( _ ( "Inventarios" ) );

    pPluginMenu->addAction ( planCuentas );
    connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    _depura ( "END myplugininv::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de Entrada del plugin de Tarifas\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_inventario", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    myplugininv *plug = new myplugininv();
    plug->inicializa ( bges );
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
    BfSubForm *l = new BfSubForm ( art );
    l->setMainCompany ( art->mainCompany() );
    l->setObjectName ( QString::fromUtf8 ( "m_lmin" ) );
    art->mui_tab->addTab ( l, _ ( "Minimos almacen" ) );

    l->setDbTableName ( "minimsalmacen" );
    l->setDbFieldId ( "idminimsalmacen" );
    l->addSubFormHeader ( "nomalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Almacen" ) );
    l->addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite, _ ( "Id Almacen" ) );
    l->addSubFormHeader ( "valminimsalmacen", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Minimo" ) );
    l->addSubFormHeader ( "idminimsalmacen", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Validacion" ) );
    l->addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Trabajador" ) );
    l->setInsert ( TRUE );
    l->setDelete ( TRUE );
    l->setSortingEnabled ( FALSE );
    art->dialogChanges_setQObjectExcluido ( l->mui_list );

    _depura ( "END ArticuloView_ArticuloView", 0 );
    return 0;
}


///
/**
\param fich
\return
**/
int BlForm_cargar ( BlForm *fich )
{
    BlSubForm * form = fich->findChild<BlSubForm *> ( "m_lmin" );
    if ( form )
        form->cargar ( "SELECT * FROM minimsalmacen NATURAL LEFT JOIN almacen WHERE idarticulo = " + fich->dbValue ( "idarticulo" ) );
    return 0;
}



int BlForm_guardar_Post ( BlForm *fich )
{
    BlSubForm * form = fich->findChild<BlSubForm *> ( "m_lmin" );
    if ( form ) {
        form->setColumnValue ( "idarticulo", fich->dbValue ( "idarticulo" ) );
        form->guardar();
    } // end if
    return 0;
}


int BfSubForm_on_mui_list_editFinished ( BfSubForm *subform )
{
    _depura ( "BfSubForm_on_mui_list_editFinished", 0 );

    if ( subform->currentColumn() < 1 ) {
        return 0;
    } // end if

    BlDbSubFormField *camp = ( BlDbSubFormField * ) subform->item ( subform->currentRow(), subform->currentColumn() - 1 );
    camp->refresh();

    if ( camp->nomcampo() == "cant" + subform->tableName() ) {
        BlDbSubFormRecord * rec = subform->lineaat ( subform->currentRow() );
        QObject *wid = subform->parent();
        while ( wid
                && ( wid->objectName() != "PresupuestoClienteBase" )
                && ( wid->objectName() != "PedidoClienteBase" )
                && ( wid->objectName() != "AlbaranClienteBase" )
              ) {
            wid = wid->parent();
        } // end if

        if ( !wid ) return 0;

        BlForm *fich = ( BlForm * ) wid;
        QString idalmacen = fich->dbValue ( "idalmacen" );
        if ( idalmacen == "" ) return 0;

        if ( rec->dbValue ( "idarticulo" ) == "" ) return 0;

        QString query1 = "SELECT * FROM stock_almacen where idarticulo=" + rec->dbValue ( "idarticulo" ) + " AND idalmacen = " + idalmacen;
        BlDbRecordSet *cur1 = subform->mainCompany() ->loadQuery ( query1 );
        BlFixed stock ( "0" );
        if ( !cur1 ) return 0;
        if ( !cur1->eof() ) {
            stock = BlFixed ( cur1->valor ( "stock" ) );
        } // end if
        delete cur1;



        QString query = "SELECT * FROM minimsalmacen where idarticulo=" + rec->dbValue ( "idarticulo" ) + " AND idalmacen = " + idalmacen;
        BlDbRecordSet *cur = subform->mainCompany() ->loadQuery ( query );
        if ( !cur ) return 0;
        if ( !cur->eof() ) {
            BlFixed val = BlFixed ( cur->valor ( "valminimsalmacen" ) );
            BlFixed valb = BlFixed ( camp->valorcampo() );
            if ( stock - valb <= val )
                mensajeAviso ( _ ( "Stock minimo superado" ) );
        } // end if
        delete cur;
    } // end if
    return 0;
}

