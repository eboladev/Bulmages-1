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

#include "convocatoriaview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
ConvocatoriaView::ConvocatoriaView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    blDebug ( "ConvocatoriaView::ConvocatoriaView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );
        blCenterOnScreen ( this );

        setTitleName ( _ ( "Convocatoria" ) );
        setDbTableName ( "actividad" );
        setDbFieldId ( "idactividad" );
        addDbField ( "idactividad", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID actividad" ) );
        addDbField ( "nombreactividad", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre del actividad" ) );
        addDbField ( "idprofesor", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id profesor" ) );

        insertWindow ( windowTitle(), this, FALSE );

        /// Establecemos los parametros de busqueda de Profesor
        mui_idprofesor->setMainCompany ( comp );
        mui_idprofesor->setLabel ( _ ( "Profesor:" ) );
        mui_idprofesor->setTableName ( "profesor" );
        mui_idprofesor->m_valores["nombreprofesor"] = "";


        pintar();
        dialogChanges_readValues();
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el actividad" ), this );
    } // end try
    blDebug ( "END ConvocatoriaView::ConvocatoriaView", 0 );
}


/** No precisa acciones adicionales en el destructor.
*/
ConvocatoriaView::~ConvocatoriaView()
{
    blDebug ( "ConvocatoriaView::~ConvocatoriaView", 0 );
    blDebug ( "END ConvocatoriaView::~ConvocatoriaView", 0 );
}


QString ConvocatoriaView::nombrePlantilla ( void )
{
    return QString ( "recibo" );
}

void ConvocatoriaView::imprimir()
{
    blDebug ( "ConvocatoriaView::imprimir", 0 );
    /// Comprobamos que se disponen de los datos minimos para imprimir el recibo.
    QString SQLQuery = "";

    if ( dbValue ( "idcliente" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        blMsgInfo ( _ ( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        return;
    }
    /// Disparamos los plugins
    int res = g_plugins->lanza ( "CoboView_on_mui_imprimir_clicked", this );
    if ( res != 0 ) {
        return;
    } // end if
    BfForm::imprimir();

    blDebug ( "END ConvocatoriaView::imprimir", 0 );
}


int ConvocatoriaView::guardarPost()
{
    blDebug ( " ConvocatoriaView::guardarPost", 0 );

    blDebug ( "END ConvocatoriaView::guardarPost", 0 );

    return 0;
}

