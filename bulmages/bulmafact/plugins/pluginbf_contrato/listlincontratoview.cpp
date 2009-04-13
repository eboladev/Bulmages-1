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

#include <QMessageBox>
#include <QKeyEvent>
#include <QEvent>

#include "listlincontratoview.h"
#include "blfunctions.h"
#include "blfixed.h"
#include "blplugins.h"


///
/**
\param parent
**/
ListLinContratoView::ListLinContratoView ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListLinContratoView::ListLinContratoView", 0 );
    setDbTableName ( "lcontrato" );
    setDbFieldId ( "idlcontrato" );
    addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView, _ ( "Id articulo" ) );
    addSubFormHeader ( "codigocompletoarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Codigo completo articulo" ) );
    addSubFormHeader ( "nomarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "Nombre articulo" ) );
    addSubFormHeader ( "idlcontrato", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView, _ ( "Idl contrato" ) );
    addSubFormHeader ( "desclcontrato", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Descripcionl contrato" ) );
    addSubFormHeader ( "cantlcontrato", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Cantidadl contrato" ) );
    addSubFormHeader ( "pvplcontrato", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "PVPl contrato" ) );
    addSubFormHeader ( "idcontrato", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView, _ ( "Id contrato" ) );
    addSubFormHeader ( "ordenlcontrato", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView, _ ( "Orden" ) );
    setInsert ( TRUE );
    setOrdenEnabled ( TRUE );
    _depura ( "END ListLinContratoView::ListLinContratoView", 0 );
}


///
/**
\param idcontrato
**/
void ListLinContratoView::cargar ( QString idcontrato )
{
    _depura ( "ListLinContratoView::cargar", 0 );
    mdb_idcontrato = idcontrato;
    BlSubForm::cargar ( "SELECT * FROM lcontrato LEFT JOIN articulo ON lcontrato.idarticulo = articulo.idarticulo WHERE idcontrato=" + mdb_idcontrato + " ORDER BY ordenlcontrato" );
    _depura ( "END ListLinContratoView::cargar", 0 );
}



