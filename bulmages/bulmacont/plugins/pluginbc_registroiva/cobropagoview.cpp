/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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
#include <QtWidgets/QComboBox>

#include "cobropagoview.h"
#include "blpostgresqlclient.h"
#include "bccompany.h"
#include "bcasientointeligenteview.h"

/// Inicia los encabezados de la tabla y llama a la presentacion del listado.
/**
\param emp
\param parent
**/
cobropagoview::cobropagoview ( BcCompany *emp, QWidget *parent )
        : BcForm ( emp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    m_companyact = emp;
    mui_listado->setMainCompany ( m_companyact );
    /// Inicializamos el listado.
    mui_listado->setDbTableName ( "prevcobro" );
    mui_listado->setDbFieldId ( "idprevcobro" );
    mui_listado->addSubFormHeader ( "idprevcobro", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite , _ ( "idprevcobro" ) );
    mui_listado->addSubFormHeader ( "fprevistaprevcobro", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite , _ ( "fprevistaprevcobro" ) );
    mui_listado->addSubFormHeader ( "fcobroprevcobro", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite , _ ( "fcobroprevcobro" ) );
    mui_listado->addSubFormHeader ( "idctacliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "idctacliente" ) );
    mui_listado->addSubFormHeader ( "idfpago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite , _ ( "idfpago" ) );
    mui_listado->addSubFormHeader ( "idcuenta", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbNone , _ ( "idcuenta" ) );
    mui_listado->addSubFormHeader ( "idasiento", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone , _ ( "idasiento" ) );
    mui_listado->addSubFormHeader ( "cantidadprevistaprevcobro", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone , _ ( "cantidadprevistaprevcobro" ) );
    mui_listado->addSubFormHeader ( "cantidadprevcobro", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone , _ ( "cantidadprevcobro" ) );
    mui_listado->addSubFormHeader ( "idregistroiva", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone , _ ( "idregistroiva" ) );
    mui_listado->addSubFormHeader ( "tipoprevcobro", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone , _ ( "tipoprevcobro" ) );
    mui_listado->addSubFormHeader ( "docprevcobro", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone , _ ( "docprevcobro" ) );
    mui_listado->setInsert ( false );
    /// Inicializamos el campo cuenta.
    m_cuenta->setMainCompany ( emp );
    m_cuenta->setLabel ( _ ( "Cuenta:" ) );
    m_cuenta->setTableName ( "cuenta" );
    m_cuenta->setFieldId("idcuenta");
    m_cuenta->m_valores["descripcion"] = "";
    m_cuenta->m_valores["codigo"] = "";


    on_mui_actualizar_clicked();
    m_companyact->insertWindow ( windowTitle(), this );
    
}


///
/**
**/
cobropagoview::~cobropagoview()
{
    BL_FUNC_DEBUG
    m_companyact->removeWindow ( this );
    
}


/// SLOT que responde a la pulsacion del boton de actualizar.
/**
**/
void cobropagoview::on_mui_actualizar_clicked()
{
    BL_FUNC_DEBUG

    /// Hacemos la presentacion con la nueva clase
    QString cadwhere = "";
    mui_listado->load ( "SELECT * FROM prevcobro "
                          " LEFT JOIN cuenta ON cuenta.idcuenta=prevcobro.idcuenta "
                          " LEFT JOIN (SELECT idcuenta AS idctacliente, codigo AS codigoctacliente, descripcion AS nomctacliente FROM cuenta) AS T1 ON t1.idctacliente = prevcobro.idctacliente "
                          " WHERE 1=1 " + cadwhere );
    s_recalculaSaldo();
    
}


///
/**
**/
void cobropagoview::s_guardar()
{
    BL_FUNC_DEBUG
    mui_listado->save();
    
}


///
/**
**/
void cobropagoview::s_recalculaSaldo()
{
    BL_FUNC_DEBUG
    BlFixed totalcobro ( "0" );
    BlFixed totalpago ( "0" );
    for ( int i = 0; i < mui_listado->rowCount(); i++ ) {
        BlDbSubFormRecord *rec = mui_listado->lineaat ( i );
        if ( rec ) {
            if ( rec->dbValue ( "tipoprevcobro" ) == "f" ) {
                totalcobro = totalcobro + BlFixed ( rec->dbValue ( "cantidadprevcobro" ) );
            } else {
                totalpago = totalpago + BlFixed ( rec->dbValue ( "cantidadprevcobro" ) );
            } // end if
        } // end if
    } // end for
    m_totalCobros->setText ( totalcobro.toQString() );
    m_totalPagos->setText ( totalpago.toQString() );
    
}


