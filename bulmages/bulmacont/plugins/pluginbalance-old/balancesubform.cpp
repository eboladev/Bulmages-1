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

#include <QMenu>

#include "balancesubform.h"


BalanceSubForm::BalanceSubForm ( QWidget *parent, const char * ) : BcSubForm ( parent )
{
    _depura ( "BalanceSubForm::BalanceSubForm", 0 );
    setDbTableName ( "borrador" );
    setFileConfig ( "balancesubform" );
    setDbFieldId ( "idborrador" );
    addSubFormHeader ( "idcuenta", BlDbField::DBint, BlDbField::DBNotNull | BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "idcuenta" ) );
    addSubFormHeader ( "tipocuenta", BlDbField::DBint, BlDbField::DBNotNull | BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "tipocuenta" ) );
    addSubFormHeader ( "codigo", BlDbField::DBint, BlDbField::DBNotNull | BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "codigo" ) );
    addSubFormHeader ( "descripcion", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "descripcion" ) );
    addSubFormHeader ( "asaldo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "asaldo" ) );
    addSubFormHeader ( "tdebe", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "tdebe" ) );
    addSubFormHeader ( "thaber", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "thaber" ) );
    addSubFormHeader ( "tsaldo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "tsaldo" ) );
    addSubFormHeader ( "ejdebe", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "ejdebe" ) );
    addSubFormHeader ( "ejhaber", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "ejhaber" ) );
    addSubFormHeader ( "ejsaldo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "ejsaldo" ) );
    setinsercion ( FALSE );
    _depura ( "END BalanceSubForm::BalanceSubForm", 0 );
}

