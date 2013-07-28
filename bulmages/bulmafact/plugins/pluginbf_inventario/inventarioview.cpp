/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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
#include <QtWidgets/QLayout>

#include <fstream>

#include "inventarioview.h"
#include "bfcompany.h"
#include "listcontrolstockview.h"
#include "inventario.h"
#include "blfunctions.h"


///
/**
\param comp
\param parent
**/
InventarioView::InventarioView ( BfCompany *comp, QWidget *parent )
        :  Inventario ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    subform2->setMainCompany ( comp );
    mui_idfamilia->setMainCompany ( comp );
    setListControlStock ( subform2 );
    insertWindow ( windowTitle(), this, false );
    
}


///
/**
**/
InventarioView::~InventarioView()
{
    BL_FUNC_DEBUG
}


///
/**
\return
**/
void InventarioView::on_mui_guardar2_clicked()
{
    BL_FUNC_DEBUG
    if ( mui_nominventario->text() == "" ) {
        blMsgInfo ( _ ( "Tiene que escribir una descripcion de inventario antes de guardar" ) );
        return;
    } // end if
    setFechaInventario ( mui_fechainventario->text() );
    setNomInventario ( mui_nominventario->text() );
    Inventario::save();
    on_mui_actualizar_clicked();
    blMsgInfo(_("Los campos de stock quedan bloqueados despues de guardarse."));
}


///
/**
\return
**/
int InventarioView::removeWindow()
{
    BL_FUNC_DEBUG
    companyact->removeWindow ( this );
    return 0;
}


/// Esta funci&oacute;n se ejecuta cuando se ha pulsado sobre el bot&oacute;n de borrar.
/**
**/
void InventarioView::on_mui_borrar2_clicked()
{
    BL_FUNC_DEBUG
    if ( dbValue ( "idinventario" ) != "" ) {
        if ( QMessageBox::question ( this,
                                     _ ( "Borrar inventario" ),
                                     _ ( "Esta a punto de borrar un inventario. Desea continuar?" ),
                                     _ ( "Si" ), _ ( "No" ), 0, 1, 0 ) == 0 ) {
            Inventario::remove();
        } // end if
    } // end if
    
}


///
/**
**/
void InventarioView::pintaIdInventario ( QString )
{
    BL_FUNC_DEBUG
}


///
/**
\param id
**/
void InventarioView::pintaFechaInventario ( QString id )
{
    BL_FUNC_DEBUG
    mui_fechainventario->setText ( id );
}


///
/**
\param id
**/
void InventarioView::pintaNomInventario ( QString id )
{
    BL_FUNC_DEBUG
    mui_nominventario->setText ( id );
}


///
/**
**/
void InventarioView::on_mui_aceptar_clicked()
{
    BL_FUNC_DEBUG
    setFechaInventario ( mui_fechainventario->text() );
    setNomInventario ( mui_nominventario->text() );
    if ( !Inventario::save() ) {
        close();
    } // end if
}


///
/**
**/
void InventarioView::on_mui_actualizar_clicked()
{
    BL_FUNC_DEBUG
    load ( dbValue ( "idinventario" ) );
    
}


/// Esta funcion carga un Inventario.
/**
\param idbudget
\return
**/
int InventarioView::load ( QString idbudget )
{
    QString SQLQuery;

    if ( idbudget == "" ) {
	/// Se esta creando un nuevo inventario.
	
	SQLQuery = "SELECT t1.idarticulo, t1.idalmacen, t1.nomarticulo, t1.nomalmacen, t1.codigocompletoarticulo, t1.codigoalmacen, t1.idfamilia, t2.stock AS stockantcontrolstock, t2.stock AS stocknewcontrolstock, false AS punteocontrolstock FROM (SELECT * FROM articulo, almacen) AS t1 LEFT JOIN stock_almacen AS t2 ON t1.idarticulo = t2.idarticulo AND t1.idalmacen = t2.idalmacen ";

	if ( mui_idfamilia->idfamilia() != "" ) {
	    SQLQuery += " WHERE t1.idfamilia = " + mui_idfamilia->idfamilia();
	} // end if
	
	/// Habilita la escritura en la columna.
	subform2->header("stocknewcontrolstock")->setOptions( BlSubFormHeader::DbNone );
      
    } else {
        /// Se esta cargando un inventario existente.
	QString query = "SELECT * FROM inventario WHERE idinventario = " + idbudget;
	BlDbRecordSet * cur = companyact->loadQuery ( query );
	if ( !cur->eof() ) {
	    DBload ( cur );
	} // end if
	delete cur;
	
	SQLQuery = "SELECT t1.idarticulo, t1.idalmacen, t1.nomarticulo, t1.nomalmacen, t1.codigocompletoarticulo, t1.codigoalmacen, t1.idfamilia, t2.stockantcontrolstock, t2.stocknewcontrolstock, t2.punteocontrolstock FROM (SELECT * FROM articulo, almacen) AS t1 LEFT JOIN controlstock AS t2 ON t1.idarticulo = t2.idarticulo AND t1.idalmacen = t2.idalmacen WHERE idinventario = " + idbudget + " ";

	if ( mui_idfamilia->idfamilia() != "" ) {
	    SQLQuery += " AND t1.idfamilia = " + mui_idfamilia->idfamilia();
	} // end if


	/// Deshabilita la escritura en la columna.
	subform2->header("stocknewcontrolstock")->setOptions( BlSubFormHeader::DbNoWrite );


    } // end if

    listalineas->load ( SQLQuery );
    pintaInventario();

    return 0;
}
