/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <fstream>

#include <QtWidgets/QMessageBox>
#include <QtGui/QCloseEvent>
#include "blfile.h"
#include <QtCore/QTextStream>

#include "almacenview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "blplugins.h"
#include "blfunctions.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
/**
\param comp
\param parent
**/
AlmacenView::AlmacenView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        /// Lanzamos los plugins.
        if ( g_plugins->run ( "AlmacenView_AlmacenView", this ) ) return;

        setTitleName ( _ ( "Almacen" ) );
        setDbTableName ( "almacen" );
        setDbFieldId ( "idalmacen" );
        addDbField ( "idalmacen", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID almacen" ) );
        addDbField ( "codigoalmacen", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Codigo almacen" ) );
        addDbField ( "nomalmacen", BlDbField::DbVarChar, BlDbField::DbNotNull  , _ ( "Nombre almacen" ) );
        addDbField ( "diralmacen", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Direccion almacen" ) );
        addDbField ( "poblalmacen", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Poblacion almacen" ) );
        addDbField ( "cpalmacen", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Codigo postal" ) );
        addDbField ( "telalmacen", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Telefono almacen" ) );
        addDbField ( "faxalmacen", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Fax almacen" ) );
        addDbField ( "emailalmacen", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "email almacen" ) );
        addDbField ( "fechaalmacen", BlDbField::DbDate, BlDbField::DbNoSave, _ ( "Fecha almacen" ) );


        addDbField ( "tipoalmacen", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Tipo almacen" ) );
        mui_tipoalmacen_t->setFieldValue ( "Tienda" );
        mui_tipoalmacen_a->setFieldValue ( "Almacen" );

        dialogChanges_readValues();
        insertWindow ( windowTitle(), this, false );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el almacen." ) );
    } // end try
    
}


/// No precisa acciones adicionales en el destructor.

/**
**/
AlmacenView::~AlmacenView()
{
    BL_FUNC_DEBUG
    
}






