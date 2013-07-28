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

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtCore/QObject>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QToolButton>
#include <QtCore/QTextStream>
#include <QtWidgets/QLayout>
#include <QtWidgets/QMessageBox>
#include <QtGui/QCloseEvent>

#include <fstream>

#include "vencimientopview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "blsearchwidget.h"
#include "bldatesearch.h"

#include "blfunctions.h"


///
/**
\param comp
\param parent
**/
VencimientoPView::VencimientoPView ( BfCompany *comp, QWidget *parent )
        :  BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    try {
        setAttribute ( Qt::WA_DeleteOnClose );
        setupUi ( this );
        blCenterOnScreen ( this );

        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        mui_refvencimientop->setMainCompany ( comp );
        mui_idproveedor->setMainCompany ( comp );
	mui_idproveedor->setLabel ( _ ( "Proveedor:" ) );
	mui_idproveedor->setTableName ( "proveedor" );
	mui_idproveedor->m_valores["cifproveedor"] = "";
	mui_idproveedor->m_valores["nomproveedor"] = "";
    
    
        setTitleName ( tr ( "Vencimiento" ) );
        setDbTableName ( "vencimientop" );
        setDbFieldId ( "idvencimientop" );
        addDbField ( "idvencimientop", BlDbField::DbInt, BlDbField::DbPrimaryKey, QApplication::translate ( "Pago", "Id pago" ) );
        addDbField ( "refvencimientop", BlDbField::DbVarChar, BlDbField::DbNothing, QApplication::translate ( "Pago", "Referencia de Vencimiento" ) );
        addDbField ( "descvencimientop", BlDbField::DbVarChar, BlDbField::DbNothing, QApplication::translate ( "Pago", "Descripcion" ) );
        addDbField ( "comentvencimientop", BlDbField::DbVarChar, BlDbField::DbNothing, QApplication::translate ( "Pago", "Comentarios" ) );
        addDbField ( "fechavencimientop", BlDbField::DbVarChar, BlDbField::DbNothing, QApplication::translate ( "Pago", "Fecha de Vencimiento" ) );
        addDbField ( "fechaefvencimientop", BlDbField::DbVarChar, BlDbField::DbNothing, QApplication::translate ( "Pago", "Fecha Efectiva" ) );
        addDbField ( "estadovencimientop", BlDbField::DbVarChar, BlDbField::DbNothing, QApplication::translate ( "Vencimiento", "estado de Vencimiento" ) );
        addDbField ( "cantvencimientop", BlDbField::DbNumeric, BlDbField::DbNothing, QApplication::translate ( "Vencimiento", "cantidad de Vencimiento" ) );
        addDbField ( "idproveedor", BlDbField::DbInt, BlDbField::DbNothing, QApplication::translate ( "Vencimiento", "idproveedor de Vencimiento" ) );
        insertWindow ( windowTitle(), this, false );
        pintar();
        dialogChanges_readValues();
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( tr ( "Error al crear el pago" ), this );
    } // end try
    
}


///
/**
**/
VencimientoPView::~VencimientoPView()
{
    BL_FUNC_DEBUG
    
}


void VencimientoPView::cargarPost() {

	//SELECT * FROM vencimientop LEFT JOIN facturap AS t1 ON vencimientop.idfacturap = t1.idfacturap LEFT JOIN proveedor AS t2 ON t2.idproveedor = t1.idproveedor LEFT JOIN forma_pago  AS t3 ON t3.idforma_pago = vencimientop.idforma_pago  WHERE

}

