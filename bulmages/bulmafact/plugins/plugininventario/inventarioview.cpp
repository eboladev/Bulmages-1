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
    _depura ( "InventarioView::InventarioView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    subform2->setMainCompany ( comp );
    mui_idfamilia->setMainCompany ( comp );
    setListControlStock ( subform2 );
    meteWindow ( windowTitle(), this, FALSE );
    _depura ( "END InventarioView::InventarioView", 0 );
}


///
/**
**/
InventarioView::~InventarioView()
{
    _depura ( "InventarioView::~InventarioView", 0 );
    _depura ( "END InventarioView::~InventarioView", 0 );
}


///
/**
\return
**/
void InventarioView::on_mui_guardar2_clicked()
{
    _depura ( "InventarioView::on_mui_guardar2_clicked", 0 );
    if ( mui_nominventario->text() == "" ) {
        mensajeInfo ( _( "Tiene que escribir una descripcion de inventario antes de guardar" ) );
        return;
    } // end if
    setfechainventario ( mui_fechainventario->text() );
    setnominventario ( mui_nominventario->text() );
    Inventario::guardar();
    _depura ( "END InventarioView::on_mui_guardar2_clicked", 0 );
}


///
/**
\return
**/
int InventarioView::sacaWindow()
{
    _depura ( "InventarioView::sacaWindow", 0 );
    companyact->sacaWindow ( this );
    return 0;
    _depura ( "END InventarioView::sacaWindow", 0 );
}


/// Esta funci&oacute;n se ejecuta cuando se ha pulsado sobre el bot&oacute;n de borrar.
/**
**/
void InventarioView::on_mui_borrar2_clicked()
{
    _depura ( "InventarioView::on_mui_borrar2_clicked", 0 );
    if ( DBvalue ( "idinventario" ) != "" ) {
        if ( QMessageBox::question ( this,
                                     _( "Borrar inventario" ),
                                     _( "Esta a punto de borrar un inventario. Desea continuar?" ),
                                     _( "Si" ), _( "No" ), 0, 1, 0 ) == 0 ) {
            Inventario::borrar();
        } // end if
    } // end if
    _depura ( "END InventarioView::on_mui_borrar2_clicked", 0 );
}


///
/**
**/
void InventarioView::pintaidinventario ( QString )
{
    _depura ( "InventarioView::pintaidinventario", 0 );
    _depura ( "END InventarioView::pintaidinventario", 0 );
}


///
/**
\param id
**/
void InventarioView::pintafechainventario ( QString id )
{
    _depura ( "InventarioView::pintaidinventario", 0 );
    mui_fechainventario->setText ( id );
    _depura ( "InventarioView::pintaidinventario", 0 );
}


///
/**
\param id
**/
void InventarioView::pintanominventario ( QString id )
{
    _depura ( "InventarioView::pintaidinventario", 0 );
    mui_nominventario->setText ( id );
    _depura ( "InventarioView::pintaidinventario", 0 );
}


///
/**
**/
void InventarioView::on_mui_aceptar_clicked()
{
    _depura ( "InventarioView::on_mui_aceptar_clicked", 0 );
    setfechainventario ( mui_fechainventario->text() );
    setnominventario ( mui_nominventario->text() );
    if ( !Inventario::guardar() ) {
        close();
    } // end if
    _depura ( "END InventarioView::on_mui_aceptar_clicked", 0 );
}


///
/**
**/
void InventarioView::on_mui_pregenerar_clicked()
{
    _depura ( "InventarioView::on_mui_pregenerar_clicked", 0 );
    pregenerar();
    _depura ( "END InventarioView::on_mui_pregenerar_clicked", 0 );
}

///
/**
**/
void InventarioView::on_mui_actualizar_released()
{
    _depura ( "InventarioView::on_mui_actualizar_released", 0 );
    cargar ( DBvalue ( "idinventario" ) );
    _depura ( "END InventarioView::on_mui_actualizar_released", 0 );
}


/// Esta funcion carga un Inventario.
/**
\param idbudget
\return
**/
int InventarioView::cargar ( QString idbudget )
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
    SQLQuery += " LEFT JOIN (SELECT punteocontrolstock,stockantcontrolstock, stocknewcontrolstock, idarticulo AS idarticulopk, idalmacen AS idalmacenpk, idinventario FROM controlstock WHERE idinventario = " + idbudget + ") AS t2 ON t1.idarticulo = t2.idarticulopk AND t1.idalmacen = t2.idalmacenpk ";

    if ( mui_idfamilia->idfamilia() != "" ) {
        SQLQuery += " WHERE t1.idfamilia = " + mui_idfamilia->idfamilia();
    } // end if

    listalineas->cargar ( SQLQuery );
    pintaInventario();

    return 0;
}
