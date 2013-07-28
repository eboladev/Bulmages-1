/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include <QtWidgets/QMenu>

#include "balancesubform.h"


///
/**
\param parent
**/
BalanceSubForm::BalanceSubForm ( QWidget *parent, const char * ) : BcSubForm ( parent )
{
    BL_FUNC_DEBUG
    setDbTableName ( "borrador" );
    setFileConfig ( "balancesubform" );
    setDbFieldId ( "idborrador" );
    addSubFormHeader ( "idcuenta", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id cuenta" ) );
    addSubFormHeader ( "tipocuenta", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Tipo cuenta" ) );
    addSubFormHeader ( "codigo", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Codigo" ) );
    addSubFormHeader ( "descripcion", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Descripcion" ) );
    addSubFormHeader ( "asaldo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Saldo apunte" ) );
    addSubFormHeader ( "tdebe", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Total debe" ) );
    addSubFormHeader ( "thaber", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Total haber" ) );
    addSubFormHeader ( "tsaldo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Total saldo" ) );
    addSubFormHeader ( "ejdebe", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Ejercicio debe" ) );
    addSubFormHeader ( "ejhaber", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Ejercico haber" ) );
    addSubFormHeader ( "ejsaldo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Ejercicio saldo" ) );
    setInsert ( false );
    
}

