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

#include "vehiculoview.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "bfsubform.h"


/// Constructor de Clase
/**
\param emp
\param parent
**/
VehiculoView::VehiculoView ( BfCompany *emp, QWidget *parent )
        : FichaBf ( emp, parent )
{
    _depura ( "VehiculoView::VehiculoView", 0 );
    this->setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_listadomantvehiculo->setMainCompany ( emp );

    /// Inicializamos la clase para la base de datos.
    setTitleName ( _( "Vehiculo" ) );
    setDbTableName ( "vehiculo" );
    setDbFieldId ( "idvehiculo" );
    addDbField ( "idvehiculo", BlDbField::DbInt, BlDbField::DbPrimaryKey, _( "idvehiculo" ) );
    addDbField ( "matriculavehiculo", BlDbField::DbVarChar, BlDbField::DbNotNull, _( "matriculavehiculo" ) );
    addDbField ( "marcavehiculo", BlDbField::DbVarChar, BlDbField::DbNothing, _( "marcavehiculo" ) );
    addDbField ( "modelovehiculo", BlDbField::DbVarChar, BlDbField::DbNothing, _( "modelovehiculo" ) );
    addDbField ( "bastidorvehiculo", BlDbField::DbVarChar, BlDbField::DbNothing, _( "bastidorvehiculo" ) );
    addDbField ( "colorvehiculo", BlDbField::DbVarChar, BlDbField::DbNothing, _( "colorvehiculo" ) );
    addDbField ( "fechacompravehiculo", BlDbField::DbDate, BlDbField::DbNothing, _( "fechavehiculo" ) );
    addDbField ( "preciovehiculo", BlDbField::DbNumeric, BlDbField::DbNothing, _( "preciovehiculo" ) );

    /// Inicializamos el listado.
    mui_listadomantvehiculo->setDbTableName ( "mantvehiculo" );
    mui_listadomantvehiculo->setDbFieldId ( "idmantvehiculo" );
    mui_listadomantvehiculo->addSubFormHeader ( "idmantvehiculo", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite, _( "idmantvehiculo" ) );
    mui_listadomantvehiculo->addSubFormHeader ( "fechamantvehiculo", BlDbField::DbDate, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _( "fechamantvehiculo" ) );
    mui_listadomantvehiculo->addSubFormHeader ( "preciomantvehiculo", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone, _( "preciomantvehiculo" ) );
    mui_listadomantvehiculo->addSubFormHeader ( "idvehiculo", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbNoView, _( "idvehiculo" ) );
    mui_listadomantvehiculo->setinsercion ( TRUE );

    /// Fin de nuevas amortizaciones
    mainCompany() ->meteWindow ( windowTitle(), this );
    cargar ( "1" );
    _depura ( "END VehiculoView::VehiculoView", 0 );
}


///
/**
**/
VehiculoView::~VehiculoView()
{
    _depura ( "VehiculoView::~VehiculoView", 0 );
    mainCompany() ->sacaWindow ( this );
    _depura ( "END VehiculoView::~VehiculoView", 0 );
}


///
/**
\return
**/
int VehiculoView::guardar()
{
    _depura ( "VehiculoView::guardar", 0 );
    try {
        setDbValue ( "matriculavehiculo", mui_matriculavehiculo->text() );
        setDbValue ( "marcavehiculo", mui_marcavehiculo->text() );
        setDbValue ( "modelovehiculo", mui_modelovehiculo->text() );
        setDbValue ( "fechacompravehiculo", mui_fechacompravehiculo->text() );
        setDbValue ( "preciovehiculo", mui_preciovehiculo->text() );

        QString id = "";
        BlDbRecord::DBsave ( id );
        mui_listadomantvehiculo->setColumnValue ( "idvehiculo", id );
        mui_listadomantvehiculo->guardar();
        dialogChanges_cargaInicial();
        return 0;
    } catch ( ... ) {
        mensajeInfo ( _( "Error al guardar la ficha" ) );
        return -1;
    } // end try
    _depura ( "END VehiculoView::guardar", 0 );
}


///
/**
\param idvehiculo
\return
**/
int VehiculoView::cargar ( QString idvehiculo )
{
    _depura ( "VehiculoView::cargar", 0 );
    try {
        BlDbRecord::cargar ( idvehiculo );

        // Se pinta.
        mui_matriculavehiculo->setText ( dbValue ( "matriculavehiculo" ) );
        mui_marcavehiculo->setText ( dbValue ( "marcavehiculo" ) );
        mui_modelovehiculo->setText ( dbValue ( "modelovehiculo" ) );
        mui_fechacompravehiculo->setText ( dbValue ( "fechacompravehiculo" ) );
        mui_preciovehiculo->setText ( dbValue ( "preciovehiculo" ) );

        QString mantenimientos = "SELECT * FROM mantvehiculo WHERE idvehiculo = '" + idvehiculo + "'";
        mui_listadomantvehiculo->cargar ( mantenimientos );

        dialogChanges_cargaInicial();
        mainCompany() ->meteWindow ( windowTitle(), this );
        _depura ( "END VehiculoView::cargar", 0 );
        return 0;
    } catch ( ... ) {
        mensajeInfo ( _( "Error al cargar los datos en la ficha." ) );
        return -1;
    }
}

