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

#include <QMessageBox>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>

#include "comisionview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
ComisionView::ComisionView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );
        blCenterOnScreen ( this );

        setTitleName ( _ ( "Comision" ) );
        setDbTableName ( "comision" );
        setDbFieldId ( "idcomision" );
        addDbField ( "idcomision", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID comision" ) );
        addDbField ( "fechaincomision", BlDbField::DbDate, BlDbField::DbNotNull, _ ( "Fecha Constitucion" ) );
        addDbField ( "fechafincomision", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha Cese" ) );
        addDbField ( "nomcomision", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre" ) );
        addDbField ( "comentcomision", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentarios" ) );

        mui_list->setMainCompany( comp );
        mui_list->setDbTableName ( "miembrocomision" );
        mui_list->setDbFieldId ( "idmiembrocomision" );
        mui_list->addSubFormHeader ( "idmiembrocomision", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Miembro" ) );
        mui_list->addSubFormHeader ( "idcomision", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Junta Directiva" ) );
        mui_list->addSubFormHeader ( "fechainmiembrocomision", BlDbField::DbDate, BlDbField::DbNothing, BlSubFormHeader::DbNone , _ ( "Fecha posesion" ) );
        mui_list->addSubFormHeader ( "fechafinmiembrocomision", BlDbField::DbDate, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Fecha Cese" ) );
        mui_list->addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Socio" ) );
        mui_list->addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Socio" ) );
        mui_list->addSubFormHeader ( "cargomiembrocomision", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Cargo" ) );

        mui_list->setInsert ( TRUE );
        mui_list->setDelete ( TRUE );
        mui_list->setSortingEnabled ( FALSE );
        
        mui_list->load("SELECT * from miembrocomision NATURAL LEFT JOIN cliente WHERE 1 = 2");
        
        insertWindow ( windowTitle(), this, FALSE );
        pintar();
        dialogChanges_readValues();
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el comision" ), this );
    } // end try
    
}


/** No precisa acciones adicionales en el destructor.
*/
ComisionView::~ComisionView()
{
    BL_FUNC_DEBUG
    
}


QString ComisionView::templateName ( void )
{
    return QString ( "comision" );
}

void ComisionView::imprimir()
{
    BL_FUNC_DEBUG
    /// Comprobamos que se disponen de los datos minimos para imprimir el comision.
    QString SQLQuery = "";

    if ( dbValue ( "idcomision" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        blMsgInfo ( _ ( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        return;
    }
    /// Disparamos los plugins
    int res = g_plugins->run ( "ComisionView_on_mui_imprimir_clicked", this );
    if ( res != 0 ) {
        return;
    } // end if
    BfForm::imprimir();

    
}


int ComisionView::afterSave()
{
    BL_FUNC_DEBUG
    mui_list->setColumnValue("idcomision", dbValue("idcomision") );
    mui_list->save();
    
    return 0;
}


int ComisionView::beforeDelete()
{

    return 0;
}



int ComisionView::cargarPost ( QString id )
{
    BL_FUNC_DEBUG
    mui_list->load("SELECT * FROM miembrocomision NATURAL LEFT JOIN cliente WHERE idcomision = " + id);
    
    return 0;
}




