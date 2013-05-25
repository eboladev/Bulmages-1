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
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "cuadrante1view.h"
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
Cuadrante1View::Cuadrante1View ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        /// Lanzamos los plugins.
        if ( g_plugins->run ( "Cuadrante1View_Cuadrante1View", this ) ) return;

        setTitleName ( _ ( "Cuadrante" ) );
        setDbTableName ( "cuadrante" );
        setDbFieldId ( "idcuadrante" );
        addDbField ( "idcuadrante", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID cuadrante" ) );
        addDbField ( "idalmacen", BlDbField::DbInt, BlDbField::DbNoSave, _ ( "Almacen" ) );
        addDbField ( "fechacuadrante", BlDbField::DbDate, BlDbField::DbNoSave  , _ ( "Fecha" ) );
        addDbField ( "comentcuadrante", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentarios" ) );
        addDbField ( "aperturacuadrante", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Apertura manyana" ) );
        addDbField ( "cierrecuadrante", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Cierre manyana" ) );
        addDbField ( "apertura1cuadrante", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Apartura tarde" ) );
        addDbField ( "cierre1cuadrante", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Cierre tarde" ) );
        addDbField ( "fiestacuadrante", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Festivo" ) );

        mui_idalmacen->setMainCompany ( comp );

        mui_list->setMainCompany ( comp );
        mui_list->setDbTableName ( "horario" );
        mui_list->setDbFieldId ( "idhorario" );
        mui_list->addSubFormHeader ( "nomtrabajador",  BlDbField::DbVarChar, BlDbField::DbNoSave,     BlSubFormHeader::DbNone,                         _ ( "Nombre Trabajador" ) );
        mui_list->addSubFormHeader ( "horainhorario",  BlDbField::DbVarChar, BlDbField::DbNotNull,    BlSubFormHeader::DbNone,                         _ ( "Hora Inicio" ) );
        mui_list->addSubFormHeader ( "horafinhorario", BlDbField::DbVarChar, BlDbField::DbNotNull,    BlSubFormHeader::DbNone,                         _ ( "Hora Fin" ) );
        mui_list->addSubFormHeader ( "idcuadrante",    BlDbField::DbInt,     BlDbField::DbNotNull,    BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite,  _ ( "ID Cuadrante" ) );
        mui_list->addSubFormHeader ( "idtrabajador",   BlDbField::DbInt,     BlDbField::DbNotNull,    BlSubFormHeader::DbNone,  _ ( "ID trabajador" ) );
        mui_list->addSubFormHeader ( "idhorario",      BlDbField::DbInt,     BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite,  _ ( "ID horario" ) );
        mui_list->setInsert ( true );
        mui_list->setDelete ( true );
        mui_list->setSortingEnabled ( false );

        dialogChanges_readValues();
        insertWindow ( windowTitle(), this, false );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el almacen" ) );
    } // end try
    
}


/** No precisa acciones adicionales en el destructor.
*/
/**
**/
Cuadrante1View::~Cuadrante1View()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param idcuadrante
\return
**/
int Cuadrante1View::cargarPost ( QString idcuadrante )
{
    mui_list->load ( "SELECT * FROM horario LEFT JOIN trabajador ON horario.idtrabajador = trabajador.idtrabajador WHERE idcuadrante=" + idcuadrante );
    return 0;
}


///
/**
\return
**/
int Cuadrante1View::afterSave()
{
    mui_list->setColumnValue ( "idcuadrante", dbValue ( "idcuadrante" ) );
    mui_list->save();
    emit ( saved() );
    return 0;
}



