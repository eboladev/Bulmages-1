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
#include "company.h"
#include "blfunctions.h"
#include "bfsubform.h"


/// Constructor de Clase
/**
\param emp
\param parent
**/
VehiculoView::VehiculoView ( Company *emp, QWidget *parent )
        : FichaBf ( emp, parent )
{
    _depura ( "VehiculoView::VehiculoView", 0 );
    this->setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_listadomantvehiculo->setEmpresaBase ( emp );

    /// Inicializamos la clase para la base de datos.
    setTitleName ( _( "Vehiculo" ) );
    setDBTableName ( "vehiculo" );
    setDBCampoId ( "idvehiculo" );
    addDBCampo ( "idvehiculo", BlDbField::DBint, BlDbField::DBPrimaryKey, _( "idvehiculo" ) );
    addDBCampo ( "matriculavehiculo", BlDbField::DBvarchar, BlDbField::DBNotNull, _( "matriculavehiculo" ) );
    addDBCampo ( "marcavehiculo", BlDbField::DBvarchar, BlDbField::DBNothing, _( "marcavehiculo" ) );
    addDBCampo ( "modelovehiculo", BlDbField::DBvarchar, BlDbField::DBNothing, _( "modelovehiculo" ) );
    addDBCampo ( "bastidorvehiculo", BlDbField::DBvarchar, BlDbField::DBNothing, _( "bastidorvehiculo" ) );
    addDBCampo ( "colorvehiculo", BlDbField::DBvarchar, BlDbField::DBNothing, _( "colorvehiculo" ) );
    addDBCampo ( "fechacompravehiculo", BlDbField::DBdate, BlDbField::DBNothing, _( "fechavehiculo" ) );
    addDBCampo ( "preciovehiculo", BlDbField::DBnumeric, BlDbField::DBNothing, _( "preciovehiculo" ) );

    /// Inicializamos el listado.
    mui_listadomantvehiculo->setDBTableName ( "mantvehiculo" );
    mui_listadomantvehiculo->setDBCampoId ( "idmantvehiculo" );
    mui_listadomantvehiculo->addSHeader ( "idmantvehiculo", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoWrite, _( "idmantvehiculo" ) );
    mui_listadomantvehiculo->addSHeader ( "fechamantvehiculo", BlDbField::DBdate, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "fechamantvehiculo" ) );
    mui_listadomantvehiculo->addSHeader ( "preciomantvehiculo", BlDbField::DBnumeric, BlDbField::DBNothing, BlSubFormHeader::DBNone, _( "preciomantvehiculo" ) );
    mui_listadomantvehiculo->addSHeader ( "idvehiculo", BlDbField::DBint, BlDbField::DBNothing, BlSubFormHeader::DBNoWrite | BlSubFormHeader::DBNoView, _( "idvehiculo" ) );
    mui_listadomantvehiculo->setinsercion ( TRUE );

    /// Fin de nuevas amortizaciones
    empresaBase() ->meteWindow ( windowTitle(), this );
    cargar ( "1" );
    _depura ( "END VehiculoView::VehiculoView", 0 );
}


///
/**
**/
VehiculoView::~VehiculoView()
{
    _depura ( "VehiculoView::~VehiculoView", 0 );
    empresaBase() ->sacaWindow ( this );
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
        setDBvalue ( "matriculavehiculo", mui_matriculavehiculo->text() );
        setDBvalue ( "marcavehiculo", mui_marcavehiculo->text() );
        setDBvalue ( "modelovehiculo", mui_modelovehiculo->text() );
        setDBvalue ( "fechacompravehiculo", mui_fechacompravehiculo->text() );
        setDBvalue ( "preciovehiculo", mui_preciovehiculo->text() );

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
        mui_matriculavehiculo->setText ( DBvalue ( "matriculavehiculo" ) );
        mui_marcavehiculo->setText ( DBvalue ( "marcavehiculo" ) );
        mui_modelovehiculo->setText ( DBvalue ( "modelovehiculo" ) );
        mui_fechacompravehiculo->setText ( DBvalue ( "fechacompravehiculo" ) );
        mui_preciovehiculo->setText ( DBvalue ( "preciovehiculo" ) );

        QString mantenimientos = "SELECT * FROM mantvehiculo WHERE idvehiculo = '" + idvehiculo + "'";
        mui_listadomantvehiculo->cargar ( mantenimientos );

        dialogChanges_cargaInicial();
        empresaBase() ->meteWindow ( windowTitle(), this );
        _depura ( "END VehiculoView::cargar", 0 );
        return 0;
    } catch ( ... ) {
        mensajeInfo ( _( "Error al cargar los datos en la ficha." ) );
        return -1;
    }
}

