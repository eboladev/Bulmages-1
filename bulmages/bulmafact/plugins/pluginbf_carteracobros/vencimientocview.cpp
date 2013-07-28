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

#include "vencimientocview.h"
#include "blmaincompany.h"
#include "blconfiguration.h"
#include "blsearchwidget.h"
#include "bldatesearch.h"
#include "blfunctions.h"


///
/**
\param comp
\param parent
**/
VencimientoCView::VencimientoCView ( BfCompany *comp, QWidget *parent )
        :  BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    try {
        setAttribute ( Qt::WA_DeleteOnClose );
        setupUi ( this );
        blCenterOnScreen ( this );

        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        mui_refvencimientoc->setMainCompany ( comp );


        /// Establecemos los parametros de busqueda del Cliente
        mui_idcliente->setMainCompany ( comp );
        mui_idcliente->setLabel ( _ ( "Cliente:" ) );
        mui_idcliente->setTableName ( "cliente" );
        mui_idcliente->m_valores["cifcliente"] = "";
        mui_idcliente->m_valores["nomcliente"] = "";
 
        setTitleName ( tr ( "Vencimiento" ) );
        setDbTableName ( "vencimientoc" );
        setDbFieldId ( "idvencimientoc" );
        addDbField ( "idvencimientoc", BlDbField::DbInt, BlDbField::DbPrimaryKey, QApplication::translate ( "Cobro", "Id cobro" ) );
        addDbField ( "refvencimientoc", BlDbField::DbVarChar, BlDbField::DbNothing, QApplication::translate ( "Cobro", "Referencia de vencimiento" ) );
        addDbField ( "descvencimientoc", BlDbField::DbVarChar, BlDbField::DbNothing, QApplication::translate ( "Cobro", "Descripcion" ) );
        addDbField ( "comentvencimientoc", BlDbField::DbVarChar, BlDbField::DbNothing, QApplication::translate ( "Cobro", "Comentarios" ) );
        addDbField ( "fechavencimientoc", BlDbField::DbVarChar, BlDbField::DbNothing, QApplication::translate ( "Cobro", "Fecha de vencimiento" ) );
        addDbField ( "fechaefvencimientoc", BlDbField::DbVarChar, BlDbField::DbNothing, QApplication::translate ( "Cobro", "Fecha efectiva" ) );
        addDbField ( "estadovencimientoc", BlDbField::DbVarChar, BlDbField::DbNothing, QApplication::translate ( "Vencimiento", "Estado de vencimiento" ) );
        addDbField ( "cantvencimientoc", BlDbField::DbNumeric, BlDbField::DbNothing, QApplication::translate ( "Vencimiento", "Cantidad de vencimiento" ) );
        addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbNothing, QApplication::translate ( "Vencimiento", "idcliente de Vencimiento" ) );
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
VencimientoCView::~VencimientoCView()
{
    BL_FUNC_DEBUG
    
}


void VencimientoCView::cargarPost() {

	//SELECT * FROM vencimientoc LEFT JOIN facturap AS t1 ON vencimientoc.idfacturap = t1.idfacturap LEFT JOIN cliente AS t2 ON t2.idcliente = t1.idcliente LEFT JOIN forma_pago  AS t3 ON t3.idforma_pago = vencimientoc.idforma_pago  WHERE

}

