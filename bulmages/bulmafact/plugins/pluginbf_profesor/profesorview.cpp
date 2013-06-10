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

#include "profesorview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"

/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
ProfesorView::ProfesorView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    
    setAttribute ( Qt::WA_DeleteOnClose );
    
    try {
        setupUi ( this );
        blCenterOnScreen ( this );

        setTitleName ( _ ( "Monitor" ) );
        setDbTableName ( "profesor" );
        setDbFieldId ( "idprofesor" );
        addDbField ( "idprofesor", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID profesor" ) );
        addDbField ( "nombreprofesor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre del profesor" ) );
        addDbField ( "dniprofesor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "DNI del profesor" ) );
        addDbField ( "dirprofesor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Direccion del profesor" ) );
        addDbField ( "cpprofesor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "CP del profesor" ) );
        addDbField ( "poblprofesor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Poblacion del profesor" ) );
        addDbField ( "telprofesor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Telefono del profesor" ) );
        addDbField ( "faxprofesor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Fax del profesor" ) );
        addDbField ( "movilprofesor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Movil del profesor" ) );
        addDbField ( "mailprofesor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "eMail del profesor" ) );
        addDbField ( "urlprofesor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Web del profesor" ) );
        addDbField ( "estudiosprofesor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Estudios del profesor" ) );
        addDbField ( "comentprofesor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentarios del profesor" ) );

        /// Datos por defecto.
        mui_idprovincia->setMainCompany ( mainCompany() );
        mui_idprovincia->setQuery ( "SELECT * FROM provincia LEFT JOIN pais ON provincia.idpais = pais.idpais ORDER BY descpais, provincia" );
        mui_idprovincia->setTableName ( "provincia" );
        mui_idprovincia->setFieldId ( "idprovincia" );
        mui_idprovincia->m_valores["provincia"] = "";
        mui_idprovincia->m_valores["descpais"] = "";
        mui_idprovincia->setAllowNull ( true );
        mui_idprovincia->setId ( "" );

        m_actividades->setMainCompany ( mainCompany() );
	m_actividades->setDbTableName ( "actividad" );
	m_actividades->setDbFieldId ( "idactividad" );
	m_actividades->addSubFormHeader ( "idactividad", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Actividad" ) );
	m_actividades->addSubFormHeader ( "nombreactividad", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre" ) );
	m_actividades->addSubFormHeader ( "nombretipoactividad", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Tipo" ) );
	m_actividades->addSubFormHeader ( "precioactividad", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Precio" ) );
	m_actividades->addSubFormHeader ( "finicialactividad", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha Inicial" ) );
	m_actividades->addSubFormHeader ( "ffinalactividad", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha Final" ) );
	
	m_actividades->setInsert ( false );
	m_actividades->setSortingEnabled ( true );

        insertWindow ( windowTitle(), this, false );
        pintar();
        dialogChanges_readValues();
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el profesor" ), this );
    } // end try
    
    
}

/** No precisa acciones adicionales en el destructor.
*/
ProfesorView::~ProfesorView()
{
    BL_FUNC_DEBUG
    
}

QString ProfesorView::templateName ( void )
{
    return QString ( "profesor" );
}

void ProfesorView::imprimir()
{
    BL_FUNC_DEBUG
    
    /// Comprobamos que se disponen de los datos minimos para imprimir el recibo.
    QString SQLQuery = "";

    if ( dbValue ( "idprofesor" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        blMsgInfo ( _ ( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        return;
    }
    
    /// Disparamos los plugins
    int res = g_plugins->run ( "ProfesorView_on_mui_imprimir_clicked", this );
    if ( res != 0 ) {
        return;
    } // end if
    
    BfForm::imprimir();

    
}

int ProfesorView::afterSave()
{
    BL_FUNC_DEBUG

    
    return 0;
}

int ProfesorView::cargarPost(QString id)
{
    BL_FUNC_DEBUG

    m_actividades->load("SELECT * FROM actividad NATURAL LEFT JOIN tipoactividad WHERE idprofesor = " + id);
    
    return 0;
}
