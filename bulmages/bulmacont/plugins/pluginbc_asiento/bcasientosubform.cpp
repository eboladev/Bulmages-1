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

#include <QMenu>

#include "bcasientosubform.h"
#include "blplugins.h"
#include "bccompany.h"


/// Construye la cabecera del asiento (nombra cada columna de la tabla)
/**
\param parent
**/
BcAsientoSubForm::BcAsientoSubForm ( QWidget *parent, const char * )
        : BcSubForm ( parent )
{
    BL_FUNC_DEBUG
    setDbTableName ( "borrador" );
    setFileConfig ( "asientotabla" );
    setDbFieldId ( "idborrador" );
    addSubFormHeader ( "idapunte", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Id apunte" ) );
    addSubFormHeader ( "fecha", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Fecha" ) );
    addSubFormHeader ( "codigo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Codigo" ) );
    addSubFormHeader ( "descripcioncuenta", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "Nombre de la cuenta" ) );
    addSubFormHeader ( "descripcion", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Descripcion del apunte" ) );
    addSubFormHeader ( "conceptocontable", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Concepto contable" ) );
    addSubFormHeader ( "debe", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Debe" ) );
    addSubFormHeader ( "haber", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Haber" ) );
    addSubFormHeader ( "contrapartida", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Contrapartida" ) );
    addSubFormHeader ( "comentario", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Comentario" ) );
    addSubFormHeader ( "idcanal", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "ID Canal" ) );
    addSubFormHeader ( "nomcanal", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Canal" ) );
    addSubFormHeader ( "marcaconciliacion", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Conciliacion" ) );
    addSubFormHeader ( "idc_coste", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Id centro de coste" ) );
    addSubFormHeader ( "nomc_coste", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Centro de coste" ) );
    addSubFormHeader ( "idtipoiva", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Id tipo de IVA" ) );
    addSubFormHeader ( "orden", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Orden" ) );
    addSubFormHeader ( "idborrador", BlDbField::DbInt,  BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Id borrador" ) );
    addSubFormHeader ( "idasiento", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Id asiento" ) );
    addSubFormHeader ( "idcuenta", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Id cuenta" ) );
    addSubFormHeader ( "tipocuenta", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Tipo de cuenta" ) );
    addSubFormHeader ( "idregistroiva", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Id. Registro IVA" ) );
    addSubFormHeader ( "factura", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Fecha Factura" ) );
    addSubFormHeader ( "ffactura", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Num. Factura" ) );
    setInsert ( TRUE );
    setOrdenEnabled ( FALSE );
    connect ( this, SIGNAL ( pintaMenu ( QMenu * ) ), this, SLOT ( s_pintaMenu ( QMenu * ) ) );
    connect ( this, SIGNAL ( trataMenu ( QAction * ) ), this, SLOT ( s_trataMenu ( QAction * ) ) );
    
}


/// Destructor de la clase. No hace nada especial.
/**
**/
BcAsientoSubForm::~BcAsientoSubForm()
{
    BL_FUNC_DEBUG
    
}


/// Slot que trata la solicitud de pintar el menu contextual sobre el subformulario.
/**
\param menu
**/
void BcAsientoSubForm::s_pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
    menu->addSeparator();
    menu->addAction ( _ ( "Mostrar asiento" ) );
    menu->addSeparator();
    menu->addAction ( _ ( "Mostrar extracto (dia)" ) );
    menu->addAction ( _ ( "Mostrar extracto (mes)" ) );
    menu->addAction ( _ ( "Mostrar extracto (ano)" ) );
    
}

/// Slot que trata la activacion de un elemento en el menu contextual.
/**
\param action
/return
**/
void BcAsientoSubForm::s_trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
    if ( !action ) return;
    if ( action->text() == _ ( "Mostrar asiento" ) )
        boton_asiento();
    if ( action->text() == _ ( "Mostrar extracto (dia)" ) )
        boton_extracto1 ( 0 );
    if ( action->text() == _ ( "Mostrar extracto (mes)" ) )
        boton_extracto1 ( 1 );
    if ( action->text() == _ ( "Mostrar extracto (ano)" ) )
        boton_extracto1 ( 2 );

    
}

/// Carga lineas de asiento (apuntes).
/**
\param idasiento
**/
void BcAsientoSubForm::load ( QString idasiento )
{
    BL_FUNC_DEBUG
    QString SQLQuery = "SELECT * FROM borrador ";
    SQLQuery += " LEFT JOIN (SELECT codigo, descripcion AS descripcioncuenta, idcuenta, tipocuenta FROM cuenta) AS t1 ON t1.idcuenta = borrador.idcuenta ";
    SQLQuery += " LEFT JOIN (SELECT idcanal, nombre AS nomcanal, descripcion AS descanal FROM canal) AS t2 ON borrador.idcanal = t2.idcanal ";
    SQLQuery += " LEFT JOIN (SELECT idc_coste, nombre AS nomc_coste, descripcion AS descc_coste FROM c_coste) AS t3 ON borrador.idc_coste = t3.idc_coste ";
    SQLQuery += " LEFT JOIN (SELECT idregistroiva, factura, ffactura, idborrador AS idborriva FROM registroiva) AS t4 ON borrador.idborrador = t4.idborriva ";
    SQLQuery += "WHERE idasiento = " + idasiento + " ORDER BY orden";
    BcSubForm::load ( SQLQuery );
    
}


/// Devuelve el DEBE de un asiento.
/// Si el asiento no existe devuelve 0
/**
\param idasiento
\return
**/
BlFixed BcAsientoSubForm::totalDebe ( QString idasiento )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, QString("idasiento = '%1'").arg(idasiento) );
    if ( idasiento == "" ) return BlFixed ( "0" );
    QString SQLQuery = "SELECT sum(debe) FROM borrador LEFT JOIN (SELECT codigo, descripcion AS descripcioncuenta, idcuenta, tipocuenta FROM cuenta) AS t1 ON t1.idcuenta = borrador.idcuenta LEFT JOIN (SELECT idcanal, nombre AS nombrecanal, descripcion AS descripcioncanal FROM canal) AS t2 ON borrador.idcanal = t2.idcanal LEFT JOIN (SELECT idc_coste, nombre AS nombrec_coste, descripcion AS descripcionc_coste FROM c_coste) AS t3 ON borrador.idc_coste = t3.idc_coste LEFT JOIN (SELECT idregistroiva, factura, ffactura, idborrador FROM registroiva) AS t4 ON borrador.idborrador = t4.idborrador WHERE idasiento = " + idasiento;
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery );
    if ( !cur ) return BlFixed ( "0" );
    QString resultado = cur->value( 0 );
    delete cur;
    
    return BlFixed ( resultado );
}


/// Devuelve el HABER de un asiento.
/**
\param idasiento
\return
**/
BlFixed BcAsientoSubForm::totalHaber ( QString idasiento )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, QString("idasiento = '%1'").arg(idasiento) );
    if ( idasiento == "" ) return BlFixed ( "0" );
    QString SQLQuery = "SELECT sum(haber) FROM borrador LEFT JOIN (SELECT codigo, descripcion AS descripcioncuenta, idcuenta, tipocuenta FROM cuenta) AS t1 ON t1.idcuenta=borrador.idcuenta LEFT JOIN (SELECT idcanal, nombre AS nombrecanal, descripcion AS descripcioncanal FROM canal) AS t2 ON borrador.idcanal = t2.idcanal LEFT JOIN (SELECT idc_coste, nombre AS nombrec_coste, descripcion AS descripcionc_coste FROM c_coste) AS t3 ON borrador.idc_coste = t3.idc_coste LEFT JOIN (SELECT idregistroiva, factura, ffactura, idborrador FROM registroiva) AS t4 ON borrador.idborrador = t4.idborrador WHERE idasiento = " + idasiento;
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery );
    if ( !cur ) return BlFixed ( "0" );
    QString resultado = cur->value( 0 );
    delete cur;
    
    return BlFixed ( resultado );
}


