/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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
#include <QtWidgets/QFileDialog>

#include "promocionarclaseview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"

/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
PromocionarClaseView::PromocionarClaseView ( BfCompany *comp, QWidget *parent ) : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    
    setAttribute ( Qt::WA_DeleteOnClose );
    
    try {
    
        setupUi ( this );
        blCenterOnScreen ( this );

        mui_idclaseor->setMainCompany ( mainCompany() );
        mui_idclaseor->setQuery ( "SELECT * FROM clase ORDER BY nomclase" );
        mui_idclaseor->setTableName ( "clase" );
        mui_idclaseor->setFieldId ( "idclase" );
        mui_idclaseor->m_valores["nomclase"] = "";
        mui_idclaseor->setAllowNull ( true );
        mui_idclaseor->setId ( "" );

	
        mui_idclasedest->setMainCompany ( mainCompany() );
        mui_idclasedest->setQuery ( "SELECT * FROM clase ORDER BY nomclase" );
        mui_idclasedest->setTableName ( "clase" );
        mui_idclasedest->setFieldId ( "idclase" );
        mui_idclasedest->m_valores["nomclase"] = "";
        mui_idclasedest->setAllowNull ( true );
        mui_idclasedest->setId ( "" );
	
        insertWindow ( windowTitle(), this, false );
/*	
        pintar();
        dialogChanges_readValues();
        blScript(this);
*/
    } catch ( ... ) {
    
        blMsgInfo ( _ ( "Error al crear el alumno" ), this );
        
    } // end try
    
    
}

/** No precisa acciones adicionales en el destructor.
*/
PromocionarClaseView::~PromocionarClaseView()
{
    BL_FUNC_DEBUG
    
}


void PromocionarClaseView::on_mui_acept_released()
{
    BL_FUNC_DEBUG
    
    QString query = "UPDATE alumno set idclase = " +mui_idclasedest->id()+ " WHERE idclase = "+ mui_idclaseor->id();
    mainCompany()->runQuery(query);
    
    close();
}
