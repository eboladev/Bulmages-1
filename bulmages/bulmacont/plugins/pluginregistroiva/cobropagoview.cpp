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

#include <QComboBox>

#include "cobropagoview.h"
#include "blpostgresqlclient.h"
#include "bccompany.h"
#include "aplinteligentesview.h"
#include "busquedacuenta.h"


/// Inicia los encabezados de la tabla y llama a la presentacion del listado.
/**
\param emp
\param parent
**/
cobropagoview::cobropagoview ( BcCompany *emp, QWidget *parent )
        : BlForm ( parent )
{
    _depura ( "cobropagoview::cobropagoview", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    m_companyact = emp;
    mui_listado->setMainCompany ( m_companyact );
    /// Inicializamos el listado.
    mui_listado->setDBTableName ( "prevcobro" );
    mui_listado->setDBCampoId ( "idprevcobro" );
    mui_listado->addSHeader ( "idprevcobro", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoWrite , _( "idprevcobro" ) );
    mui_listado->addSHeader ( "fprevistaprevcobro", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoWrite , _( "fprevistaprevcobro" ) );
    mui_listado->addSHeader ( "fcobroprevcobro", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite , _( "fcobroprevcobro" ) );
    mui_listado->addSHeader ( "idctacliente", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite | BlSubFormHeader::DBNoView, _( "idctacliente" ) );
    mui_listado->addSHeader ( "idfpago", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite , _( "idfpago" ) );
    mui_listado->addSHeader ( "idcuenta", BlDbField::DBint, BlDbField::DBNothing, BlSubFormHeader::DBNone , _( "idcuenta" ) );
    mui_listado->addSHeader ( "idasiento", BlDbField::DBnumeric, BlDbField::DBNothing, BlSubFormHeader::DBNone , _( "idasiento" ) );
    mui_listado->addSHeader ( "cantidadprevistaprevcobro", BlDbField::DBnumeric, BlDbField::DBNothing, BlSubFormHeader::DBNone , _( "cantidadprevistaprevcobro" ) );
    mui_listado->addSHeader ( "cantidadprevcobro", BlDbField::DBnumeric, BlDbField::DBNothing, BlSubFormHeader::DBNone , _( "cantidadprevcobro" ) );
    mui_listado->addSHeader ( "idregistroiva", BlDbField::DBnumeric, BlDbField::DBNothing, BlSubFormHeader::DBNone , _( "idregistroiva" ) );
    mui_listado->addSHeader ( "tipoprevcobro", BlDbField::DBnumeric, BlDbField::DBNothing, BlSubFormHeader::DBNone , _( "tipoprevcobro" ) );
    mui_listado->addSHeader ( "docprevcobro", BlDbField::DBnumeric, BlDbField::DBNothing, BlSubFormHeader::DBNone , _( "docprevcobro" ) );
    mui_listado->setinsercion ( FALSE );
    /// Dejamos de inicializar el listado.
    m_cuenta->setMainCompany ( emp );
    on_mui_actualizar_clicked();
    m_companyact->meteWindow ( windowTitle(), this );
    _depura ( "END cobropagoview::cobropagoview", 0 );
}


///
/**
**/
cobropagoview::~cobropagoview()
{
    _depura ( "cobropagoview::~cobropagoview", 0 );
    m_companyact->sacaWindow ( this );
    _depura ( "END cobropagoview::~cobropagoview", 0 );
}


/// SLOT que responde a la pulsacion del boton de actualizar.
/**
**/
void cobropagoview::on_mui_actualizar_clicked()
{
    _depura ( "cobropagoview::s_actualizar", 0 );

    /// Hacemos la presentacion con la nueva clase
    QString cadwhere = "";
    mui_listado->cargar ( "SELECT * FROM prevcobro "
                          " LEFT JOIN cuenta ON cuenta.idcuenta=prevcobro.idcuenta "
                          " LEFT JOIN (SELECT idcuenta AS idctacliente, codigo AS codigoctacliente, descripcion AS nomctacliente FROM cuenta) AS T1 ON t1.idctacliente = prevcobro.idctacliente "
                          " WHERE 1=1 " + cadwhere );
    s_recalculaSaldo();
    _depura ( "END cobropagoview::s_actualizar", 0 );
}


///
/**
**/
void cobropagoview::s_guardar()
{
    _depura ( "cobropagoview::s_guardar", 0 );
    mui_listado->guardar();
    _depura ( "END cobropagoview::s_guardar", 0 );
}


///
/**
**/
void cobropagoview::s_recalculaSaldo()
{
    _depura ( "s_recalculaSaldo()", 0 );
    BlFixed totalcobro ( "0" );
    BlFixed totalpago ( "0" );
    for ( int i = 0; i < mui_listado->rowCount(); i++ ) {
        BlDbSubFormRecord *rec = mui_listado->lineaat ( i );
        if ( rec ) {
            if ( rec->DBvalue ( "tipoprevcobro" ) == "f" ) {
                totalcobro = totalcobro + BlFixed ( rec->DBvalue ( "cantidadprevcobro" ) );
            } else {
                totalpago = totalpago + BlFixed ( rec->DBvalue ( "cantidadprevcobro" ) );
            } // end if
        } // end if
    } // end for
    m_totalCobros->setText ( totalcobro.toQString() );
    m_totalPagos->setText ( totalpago.toQString() );
    _depura ( "END s_recalculaSaldo()", 0 );
}

