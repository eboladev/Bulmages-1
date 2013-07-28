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



///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_Modificadores", "0.12.1-0002");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_modificadores", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_ArticuloView ( ArticuloView *art )
{
    BL_FUNC_DEBUG

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
    
    l->addSubFormHeader ( "idmodificador", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbDisableView, "Id modificador" );
    l->addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbDisableView, "Id articulo" );
    l->addSubFormHeader ( "nombremodificador", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Nombre modificador" ) );
    l->addSubFormHeader ( "varpreciomodificador", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Alt. precio" ) );
    
    l->setInsert ( true );
    l->setDelete ( true );
    l->setSortingEnabled ( false );
    art->dialogChanges_setExcludedObject ( l->mui_list );

    art->mui_tab->addTab ( l, "Modificadores" );

    
    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_load ( ArticuloView *art )
{
    BL_FUNC_DEBUG
    BfSubForm *l = art->findChild<BfSubForm *> ( "lmodificadores" );
    if ( l ) {
        l->load ( "SELECT * FROM modificador WHERE idarticulo = " + art->dbValue ( "idarticulo" ) );
    } // end if
    
    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_guardar_post ( ArticuloView *art )
{
    BL_FUNC_DEBUG
    try {
        BfSubForm *l = art->findChild<BfSubForm *> ( "lmodificadores" );
        l->setColumnValue ( "idarticulo", art->dbValue ( "idarticulo" ) );
        l->save();
//        invalidaEstadAlias();
        return 0;
    } catch ( ... ) {
        blMsgError ( _("Hubo un error al guardar los alias") );
        return 0;
    }
}

