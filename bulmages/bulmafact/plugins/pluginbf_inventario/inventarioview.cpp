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

#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QLayout>

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
    insertWindow ( windowTitle(), this, FALSE );
    
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
void InventarioView::on_mui_pregenerar_clicked()
{
    BL_FUNC_DEBUG
    pregenerar();
    
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
    if ( idbudget == "" ) idbudget = "0";
    QString query = "SELECT * FROM inventario WHERE idinventario = " + idbudget;
    BlDbRecordSet * cur = companyact->loadQuery ( query );
    if ( !cur->eof() ) {
        DBload ( cur );
    } // end if
    delete cur;

    QString SQLQuery = "SELECT * FROM ";
    SQLQuery += " (SELECT idarticulo, idalmacen, nomarticulo, nomalmacen, codigocompletoarticulo, codigoalmacen, idfamilia FROM articulo, almacen) AS t1 ";
    SQLQuery += " LEFT JOIN (SELECT punteocontrolstock, COALESCE(stockantcontrolstock, 0) AS stockantcontrolstock, stocknewcontrolstock, idarticulo AS idarticulopk, idalmacen AS idalmacenpk, idinventario FROM controlstock WHERE idinventario = " + idbudget + ") AS t2 ON t1.idarticulo = t2.idarticulopk AND t1.idalmacen = t2.idalmacenpk ";

    if ( mui_idfamilia->idfamilia() != "" ) {
        SQLQuery += " WHERE t1.idfamilia = " + mui_idfamilia->idfamilia();
    } // end if

    listalineas->load ( SQLQuery );
    pintaInventario();

    return 0;
}
