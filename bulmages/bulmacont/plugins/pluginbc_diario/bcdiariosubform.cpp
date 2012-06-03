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
#include "bcdiariosubform.h"


///
/**
\param parent
**/
BcDiarioSubForm::BcDiarioSubForm ( QWidget *parent, const char * ) : BcSubForm ( parent )
{
    BL_FUNC_DEBUG
    setDbTableName ( "borrador" );
    setFileConfig ( "BcDiarioSubForm" );
    setDbFieldId ( "idborrador" );
    addSubFormHeader ( "idasiento", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "idasiento" ) );
//     addSubFormHeader ( "ordenasientoconfecha", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Asiento" ) );
    addSubFormHeader ( "ordenasiento", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Asiento" ) );
    addSubFormHeader ( "fecha", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha" ) );
    addSubFormHeader ( "codigo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Cuenta" ) );
    addSubFormHeader ( "descripcion", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Descripcion" ) );
    addSubFormHeader ( "descripcioncuenta", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Descripcion de Cuenta" ) );
    addSubFormHeader ( "conceptocontable", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Concepto contable" ) );
    addSubFormHeader ( "debe", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Debe" ) );
    addSubFormHeader ( "haber", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Haber" ) );
    addSubFormHeader ( "comentario", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbNoWrite, _ ( "Comentario" ) );
    addSubFormHeader ( "idborrador", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "idborrador" ) );
    addSubFormHeader ( "codigoborrador", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "codigoborrador" ) );
    addSubFormHeader ( "tipocuenta", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "tipocuenta" ) );
    addSubFormHeader ( "contrapartida", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Contrapartida" ) );
    addSubFormHeader ( "codcontrapartida", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Contrapartida" ) );
    addSubFormHeader ( "nombrecanal", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Canal" ) );
    addSubFormHeader ( "marcaconciliacion", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "MarcaConciliacion" ) );
    addSubFormHeader ( "nombrec_coste", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Centro Coste" ) );
    addSubFormHeader ( "factura", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Factura" ) );
    addSubFormHeader ( "idapunte", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "idapunte" ) );
    addSubFormHeader ( "idtipoiva", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "idtipoiva" ) );
    addSubFormHeader ( "idregistroiva", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbHideView, _ ( "idregistroiva" ) );
    addSubFormHeader ( "idcanal", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbHideView , _ ( "idcanal" ) );
    addSubFormHeader ( "idc_coste", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "idc_coste" ) );
    addSubFormHeader ( "iddiario", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "iddiario" ) );
    addSubFormHeader ( "idcuenta", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "idcuenta" ) );
    /// Van juntas. Desactivas un tipo de pijama y se activa otro personalizado al hacer 'rowspan'.
    setListadoPijama ( FALSE );
    setColumnToRowSpan ( "ordenasiento" );
    setColorFondo1 ( g_confpr->value( CONF_BG_DIARIO ) );
    setColorFondo2 ( g_confpr->value( CONF_BG_ALT_DIARIO ) );
    setInsert ( FALSE );
    setOrdenEnabled ( FALSE );
    setOrdenPorQuery ( TRUE );
    setDelete ( FALSE );
    
}


///
/**
**/
BcDiarioSubForm::~BcDiarioSubForm()
{
    BL_FUNC_DEBUG
    
}




