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
        : BfForm ( emp, parent )
{
    BL_FUNC_DEBUG
    this->setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_listadomantvehiculo->setMainCompany ( emp );

    /// Inicializamos la clase para la base de datos.
    setTitleName ( _ ( "Vehiculo" ) );
    setDbTableName ( "vehiculo" );
    setDbFieldId ( "idvehiculo" );
    addDbField ( "idvehiculo", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "idvehiculo" ) );
    addDbField ( "matriculavehiculo", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "matriculavehiculo" ) );
    addDbField ( "marcavehiculo", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "marcavehiculo" ) );
    addDbField ( "modelovehiculo", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "modelovehiculo" ) );
    addDbField ( "bastidorvehiculo", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "bastidorvehiculo" ) );
    addDbField ( "colorvehiculo", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "colorvehiculo" ) );
    addDbField ( "fechacompravehiculo", BlDbField::DbDate, BlDbField::DbNothing, _ ( "fechavehiculo" ) );
    addDbField ( "preciovehiculo", BlDbField::DbNumeric, BlDbField::DbNothing, _ ( "preciovehiculo" ) );

    /// Inicializamos el listado.
    mui_listadomantvehiculo->setDbTableName ( "mantvehiculo" );
    mui_listadomantvehiculo->setDbFieldId ( "idmantvehiculo" );
    mui_listadomantvehiculo->addSubFormHeader ( "idmantvehiculo", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite, _ ( "idmantvehiculo" ) );
    mui_listadomantvehiculo->addSubFormHeader ( "fechamantvehiculo", BlDbField::DbDate, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "fechamantvehiculo" ) );
    mui_listadomantvehiculo->addSubFormHeader ( "preciomantvehiculo", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "preciomantvehiculo" ) );
    mui_listadomantvehiculo->addSubFormHeader ( "idvehiculo", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "idvehiculo" ) );
    mui_listadomantvehiculo->setInsert ( TRUE );

    /// Fin de nuevas amortizaciones
    mainCompany() ->insertWindow ( windowTitle(), this );
    load ( "1" );
    blScript(this);
    
}


///
/**
**/
VehiculoView::~VehiculoView()
{
    BL_FUNC_DEBUG
    mainCompany() ->removeWindow ( this );
    
}


///
/**
\return
**/
int VehiculoView::save()
{
    BL_FUNC_DEBUG
    try {
        setDbValue ( "matriculavehiculo", mui_matriculavehiculo->text() );
        setDbValue ( "marcavehiculo", mui_marcavehiculo->text() );
        setDbValue ( "modelovehiculo", mui_modelovehiculo->text() );
        setDbValue ( "fechacompravehiculo", mui_fechacompravehiculo->text() );
        setDbValue ( "preciovehiculo", mui_preciovehiculo->text() );

        QString id = "";
        BlDbRecord::dbSave ( id );
        mui_listadomantvehiculo->setColumnValue ( "idvehiculo", id );
        mui_listadomantvehiculo->save();
        dialogChanges_readValues();
        return 0;
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al guardar la ficha" ) );
        return -1;
    } // end try
    
}


///
/**
\param idvehiculo
\return
**/
int VehiculoView::load ( QString idvehiculo )
{
    BL_FUNC_DEBUG
    try {
        BlDbRecord::load ( idvehiculo );

        // Se pinta.
        mui_matriculavehiculo->setText ( dbValue ( "matriculavehiculo" ) );
        mui_marcavehiculo->setText ( dbValue ( "marcavehiculo" ) );
        mui_modelovehiculo->setText ( dbValue ( "modelovehiculo" ) );
        mui_fechacompravehiculo->setText ( dbValue ( "fechacompravehiculo" ) );
        mui_preciovehiculo->setText ( dbValue ( "preciovehiculo" ) );

        QString mantenimientos = "SELECT * FROM mantvehiculo WHERE idvehiculo = '" + idvehiculo + "'";
        mui_listadomantvehiculo->load ( mantenimientos );

        dialogChanges_readValues();
        mainCompany() ->insertWindow ( windowTitle(), this );
        
        return 0;
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al cargar los datos en la ficha." ) );
        return -1;
    }
}

