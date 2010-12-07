/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
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

#include "pluginbf_modificadores.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "listmodificadoresview.h"




///
/**
**/
MyPlugModificadores::MyPlugModificadores()
{
    blDebug ( "MyPlugModificadores::MyPlugModificadores", 0 );
    blDebug ( "END MyPlugModificadores::MyPlugModificadores", 0 );
}


///
/**
**/
MyPlugModificadores::~MyPlugModificadores()
{
    blDebug ( "MyPlugModificadores::~MyPlugModificadores", 0 );
    blDebug ( "END MyPlugModificadores::~MyPlugModificadores", 0 );
}


///
/**
**/
void MyPlugModificadores::elslot1()
{
    blDebug ( "MyPlugModificadores::elslot1", 0 );
    ListModificadoresView * bud = new ListModificadoresView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    blDebug ( "END MyPlugModificadores::elslot1", 0 );
}



///
/**
\param bges
**/
void MyPlugModificadores::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "MyPlugModificadores::inicializa", 0 );

    if ( bges->company()->hasTablePrivilege ( "serie_factura", "SELECT" ) ) {

        /// Miramos si existe un menu Articulos
        QMenu *pPluginMenu = bges->newMenu ( "&Articulos", "menuArticulos", "menuMaestro" );
        pPluginMenu->addSeparator();
	
        m_bges = bges;
        setMainCompany ( bges->company() );
        QAction *modificadoresArticulo = new QAction ( _ ( "&Modificadores" ), 0 );
        modificadoresArticulo->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/product.png" ) ));
        modificadoresArticulo->setStatusTip ( _ ( "Modificadores" ) );
        modificadoresArticulo->setWhatsThis ( _ ( "Modificadores" ) );
        pPluginMenu->addAction ( modificadoresArticulo );
        bges->Listados->addAction ( modificadoresArticulo );
        connect ( modificadoresArticulo, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

    }// end if
    blDebug ( "END MyPlugModificadores::inicializa", 0 );
}





///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de Entrada del plugin de Modificadores\n", 0 );

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_Modificadores", "0.12.1-0002");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_modificadores", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

//    MyPlugModificadores *plug = new MyPlugModificadores();
//    plug->inicializa ( bges );
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

    /// Agregamos el subformulario de validaciones.
    BfSubForm *l = new BfSubForm ( art );
    l->setObjectName ( QString::fromUtf8 ( "lmodificadores" ) );
    l->setMainCompany ( art->mainCompany() );
    l->setDbTableName ( "modificador" );
    l->setDbFieldId ( "idmodificador" );
    
    /*
    l->addSubFormHeader ( "cadalias", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone , _ ( "Alias" ) );
    l->addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite , _ ( "Id articulo" ) );
    l->addSubFormHeader ( "idalias", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id validacion" ) );
    */
    
    l->addSubFormHeader ( "idmodificador", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbDisableView, "Id" );
    l->addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbDisableView, "Id" );
    l->addSubFormHeader ( "nombremodificador", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Nombre Modificador" ) );
    l->addSubFormHeader ( "varpreciomodificador", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Alt. Precio" ) );
    
    l->setInsert ( TRUE );
    l->setDelete ( TRUE );
    l->setSortingEnabled ( FALSE );
    art->dialogChanges_setQObjectExcluido ( l->mui_list );

    art->mui_tab->addTab ( l, "Modificadores" );

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
    BfSubForm *l = art->findChild<BfSubForm *> ( "lmodificadores" );
    if ( l ) {
        l->cargar ( "SELECT * FROM modificador WHERE idarticulo = " + art->dbValue ( "idarticulo" ) );
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
        BfSubForm *l = art->findChild<BfSubForm *> ( "lmodificadores" );
        l->setColumnValue ( "idarticulo", art->dbValue ( "idarticulo" ) );
        l->guardar();
//        invalidaEstadAlias();
        return 0;
    } catch ( ... ) {
        blDebug ( "Hubo un error al guardar los alias", 2 );
        return 0;
    }
}

