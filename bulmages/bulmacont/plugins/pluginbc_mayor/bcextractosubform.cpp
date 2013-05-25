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

#include "bcextractosubform.h"


///
/**
**/
BcExtractoSubForm::~BcExtractoSubForm()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param parent
**/
BcExtractoSubForm::BcExtractoSubForm ( QWidget *parent, const char * ) : BcSubForm ( parent )
{
    BL_FUNC_DEBUG
    setDbTableName ( "apunte" );
    setFileConfig ( "BcExtractoSubForm" );
    setDbFieldId ( "idapunte" );
    addSubFormHeader ( "punteo", BlDbField::DbBoolean, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Punteo" ) );
    addSubFormHeader ( "fecha", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbDisableView, _ ( "Fecha" ) );
    addSubFormHeader ( "codigo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Cuenta" ) );
    addSubFormHeader ( "ordenasiento", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbDisableView, _ ( "Asiento" ) );
    addSubFormHeader ( "descripcioncuenta", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Descripcion" ) );
    addSubFormHeader ( "descripcionapunte", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Descripcion Apunte" ) );
    addSubFormHeader ( "descripcionasiento", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Descripcion Asiento" ) );
    addSubFormHeader ( "conceptocontable", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Concepto" ) );
    addSubFormHeader ( "debe", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbDisableView, _ ( "Debe" ) );
    addSubFormHeader ( "haber", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbDisableView, _ ( "Haber" ) );
    addSubFormHeader ( "saldo", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Saldo" ) );

    addSubFormHeader ( "idapunte", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "idapunte" ) );
    addSubFormHeader ( "codigoborrador", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "codigoborrador" ) );
    addSubFormHeader ( "idasiento", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "idasiento" ) );
    addSubFormHeader ( "iddiario", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "iddiario" ) );
    addSubFormHeader ( "idcuenta", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "idcuenta" ) );
    addSubFormHeader ( "tipocuenta", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "tipocuenta" ) );
    addSubFormHeader ( "contrapartida", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Id contrapartida" ) );
    addSubFormHeader ( "codcontrapartida", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "contrapartida" ) );
    addSubFormHeader ( "comentario", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Comentario" ) );
    addSubFormHeader ( "idcanal", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "idcanal" ) );
    addSubFormHeader ( "nombrecanal", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Canal" ) );
    addSubFormHeader ( "marcaconciliacion", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "marcaconciliacion" ) );
    addSubFormHeader ( "idc_coste", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "idc_coste" ) );
    addSubFormHeader ( "nombrec_coste", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Centro Coste" ) );
    addSubFormHeader ( "idtipoiva", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "idtipoiva" ) );
    addSubFormHeader ( "orden", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Orden" ) );
    setInsert ( false );
    setOrdenEnabled ( false );
    setOrdenPorQuery ( false );
    setDelete ( false );
 
}


