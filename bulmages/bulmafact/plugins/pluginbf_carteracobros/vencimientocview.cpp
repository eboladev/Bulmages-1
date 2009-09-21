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

#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>
#include <QCloseEvent>

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
    _depura ( "VencimientoCView::VencimientoCView", 0 );
    try {
        setAttribute ( Qt::WA_DeleteOnClose );
        setupUi ( this );
        centrarEnPantalla ( this );

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
        addDbField ( "idvencimientoc", BlDbField::DbInt, BlDbField::DbPrimaryKey, QApplication::translate ( "Pago", "Id pago" ) );
        addDbField ( "refvencimientoc", BlDbField::DbVarChar, BlDbField::DbNothing, QApplication::translate ( "Pago", "Referencia de Vencimiento" ) );
        addDbField ( "descvencimientoc", BlDbField::DbVarChar, BlDbField::DbNothing, QApplication::translate ( "Pago", "Descripcion" ) );
        addDbField ( "comentvencimientoc", BlDbField::DbVarChar, BlDbField::DbNothing, QApplication::translate ( "Pago", "Comentarios" ) );
        addDbField ( "fechavencimientoc", BlDbField::DbVarChar, BlDbField::DbNothing, QApplication::translate ( "Pago", "Fecha de Vencimiento" ) );
        addDbField ( "fechaefvencimientoc", BlDbField::DbVarChar, BlDbField::DbNothing, QApplication::translate ( "Pago", "Fecha Efectiva" ) );
        addDbField ( "estadovencimientoc", BlDbField::DbVarChar, BlDbField::DbNothing, QApplication::translate ( "Vencimiento", "estado de Vencimiento" ) );
        addDbField ( "cantvencimientoc", BlDbField::DbNumeric, BlDbField::DbNothing, QApplication::translate ( "Vencimiento", "cantidad de Vencimiento" ) );
        addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbNothing, QApplication::translate ( "Vencimiento", "idcliente de Vencimiento" ) );
        meteWindow ( windowTitle(), this, FALSE );
        pintar();
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        mensajeInfo ( tr ( "Error al crear el pago" ), this );
    } // end try
    _depura ( "END VencimientoCView::VencimientoCView", 0 );
}


///
/**
**/
VencimientoCView::~VencimientoCView()
{
    _depura ( "VencimientoCView::~VencimientoCView", 0 );
    _depura ( "END VencimientoCView::~VencimientoCView", 0 );
}


void VencimientoCView::cargarPost() {

	//SELECT * FROM vencimientoc LEFT JOIN facturap AS t1 ON vencimientoc.idfacturap = t1.idfacturap LEFT JOIN cliente AS t2 ON t2.idcliente = t1.idcliente LEFT JOIN forma_pago  AS t3 ON t3.idforma_pago = vencimientoc.idforma_pago  WHERE

}

