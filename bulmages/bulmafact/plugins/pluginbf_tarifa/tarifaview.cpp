/***************************************************************************
 *   Copyright (C) 2004 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
 *   Copyright (C) 2008 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include <QCloseEvent>

#include "tarifaview.h"
#include "bfcompany.h"
#include "familiasview.h"
#include "tiposarticuloview.h"
#include "bfbuscarfamilia.h"
#include "bfbuscaralmacen.h"
#include "comparticulolistview.h"
#include "blfunctions.h"
#include "blplugins.h"


///
/**
\param comp
\param parent
**/
TarifaView::TarifaView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    _depura ( "TarifaView::INIT_constructor()\n", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    setTitleName ( _ ( "Tarifa" ) );
    setDbTableName ( "tarifa" );
    setDbFieldId ( "idtarifa" );
    addDbField ( "idtarifa", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID tarifa" ) );
    addDbField ( "nomtarifa", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Nombre de la tarifa" ) );
    ///\TODO: Existen en la base de datos 2 campos mas para establecer fecha de inicio y de
    ///       fin de aplicacion de una tarifa. Actualmente no se usan estos valores.
    mui_idfamilia->setMainCompany ( comp );
    mui_almacen->setMainCompany ( comp );
    mui_almacen->setId ( "" );
    mui_list->setMainCompany ( comp );

    /// Desactivamos los campos que solo se utilizan si existe un 'idtarifa',
    /// es decir, si estamos creando una nueva tarifa no tenemos que emplear
    /// estos campos.
    mui_list->setEnabled ( FALSE );
    mui_filtro->setEnabled ( FALSE );
    mui_actualizar->setEnabled ( FALSE );
    mui_borrar->setEnabled ( FALSE );

    meteWindow ( _ ( "Tarifa edicion" ), this );

    dialogChanges_cargaInicial();
    _depura ( "TarifaView::END_constructor()\n", 0 );
}


///
/**
**/
TarifaView::~TarifaView()
{
    _depura ( "TarifaView::INIT_destructor()\n", 0 );
    mainCompany() ->sacaWindow ( this );
    _depura ( "TarifaView::END_destructor()\n", 0 );
}


///
/**
**/
BfCompany *TarifaView::companyact()
{
    return mainCompany();
}


///
/**
**/
void TarifaView::pintar()
{
    _depura ( "TarifaView::pintar", 0 );
    mui_nomtarifa->setText ( dbValue ( "nomtarifa" ) );
    setWindowTitle ( _ ( "Tarifa" ) + " " + mui_nomtarifa->text() );
    _depura ( "END TarifaView::pintar", 1 );
}


///
/**
\param idtarifa
\return
**/
QString TarifaView::formaQuery ( QString idtarifa )
{
    _depura ( "TarifaView::formaQuery", 0 );
    QString wherearticulo = "";
    QString warticulo = " WHERE ";

    QString wherealmacen = "";
    QString walmacen = " WHERE ";

    QString idfamilia = mui_idfamilia->idfamilia();
    if ( idfamilia != "" ) {
        wherearticulo += warticulo + " idfamilia = " + idfamilia;
        warticulo = " AND ";
    } // end if

    QString idalmacen = mui_almacen->id();
    if ( idalmacen != "" ) {
        wherealmacen += walmacen + " idalmacen = " + idalmacen;
        walmacen = " AND ";
    } // end if

    QString SQLQuery = "SELECT * FROM (SELECT * FROM (SELECT * FROM almacen " + wherealmacen + ") AS t6, (SELECT * FROM articulo " + wherearticulo + ") AS t5, (SELECT * FROM tarifa WHERE idtarifa = " + idtarifa + ") AS t2 ) AS t3 ";
    SQLQuery += " LEFT JOIN (SELECT * FROM ltarifa WHERE idtarifa = " + idtarifa + ") as t1 ON t1.idtarifa = t3.idtarifa AND t1.idalmacen = t3.idalmacen AND t1.idarticulo = t3.idarticulo";
    _depura ( "END TarifaView::formaQuery", 0 );
    return SQLQuery;
}


///
/**
\param idtarifa
\return
**/
int TarifaView::cargar ( QString idtarifa )
{
    _depura ( "TarifaView::cargar(" + idtarifa + ")\n", 0 );
    int error = 0;
    m_idtarifa = idtarifa;

    /// Si se ha guardado bien entonces de habilitan los botones y demas.
    mui_list->setEnabled ( TRUE );
    mui_filtro->setEnabled ( TRUE );
    mui_actualizar->setEnabled ( TRUE );
    mui_borrar->setEnabled ( TRUE );

    setDbValue ( "idtarifa", m_idtarifa );
    BlDbRecord::cargar ( m_idtarifa );
    mui_list->cargar ( formaQuery ( m_idtarifa ) );

    setWindowTitle ( _ ( "Tarifa" ) + " " + dbValue ( "nomtarifa" ) );
    meteWindow ( windowTitle(), this );

    /// Tratamiento de excepciones.
    if ( error == 1 ) {
        _depura ( "TarifaView::cargar Error en la carga del articulo.\n", 0 );
        return -1;
    } // end if
    pintar();

    dialogChanges_cargaInicial();
    _depura ( "END TarifaView::cargar\n", 0 );
    return 0;
}


///
/**
\return
**/
int TarifaView::guardar()
{
    _depura ( "TarifaView::INIT_s_grabarClicked()\n", 0 );

    /// Al pulsar sobre 'guardar' se tiene que comprobar si existe un 'idtarifa'
    /// Si no existe se crea una nueva tarifa (INSERT) en la base de datos con el nombre proporcionado:
    /// - Se comprueba que el nombre no este vacio.
    /// - \TODO: Se comprueba que no exista un nombre igual.
    /// Si existe un 'idtarifa' se actualiza sus datos (UPDATE).
    if ( mui_nomtarifa->text().isEmpty() ) {
        mensajeAviso ( tr ( "El nombre de la tarifa no puede estar vacio." ) );
        return -1;
    } // end if

    setDbValue ( "nomtarifa", mui_nomtarifa->text() );
    BlDbRecord::guardar();
    /// Guardamos la lista de componentes.
    mui_list->setColumnValue ( "idtarifa", dbValue ( "idtarifa" ) );
    mui_list->guardar();
    dialogChanges_cargaInicial();

    /// Se recarga el listado de articulos para poder establecer precios a esa tarifa.
    cargar ( dbValue ( "idtarifa" ) );

    emit guardartarifa();

    _depura ( "TarifaView::END_s_grabarClicked()\n", 0 );
    return 0;
}


/// Esta funci&oacute;n se ejecuta cuando se ha pulsado sobre el bot&oacute;n de nuevo.
/**
**/
void TarifaView::on_mui_crear_clicked()
{
    _depura ( "TarifaView::INIT_boton_nuevo()\n", 0 );
    vaciar();
    pintar();
    _depura ( "TarifaView::END_boton_nuevo()\n", 0 );
}


///
/**
**/
void TarifaView::on_mui_actualizar_clicked()
{
    _depura ( "TarifaView::INIT_boton_nuevo()\n", 0 );
    cargar ( m_idtarifa );
    _depura ( "TarifaView::END_boton_nuevo()\n", 0 );
}


/// Esta funci&oacute;n se ejecuta cuando se ha pulsado sobre el bot&oacute;n de borrar.
/**
**/
void TarifaView::on_mui_borrar_clicked()
{
    _depura ( "TarifaView::INIT_boton_borrar()\n", 0 );
    if ( dbValue ( "idtarifa" ) != "" ) {
        if ( QMessageBox::question ( this,
                                     _ ( "Borrar tarifa" ),
                                     _ ( "Esta a punto de borrar una tarifa. Desea continuar?" ),
                                     _ ( "&Si" ), _ ( "&No" ), 0, 1, 0 ) == 0 ) {
            mainCompany() ->begin();
            int error = mui_list->borrar();
            error += borrar();
            if ( error ) {
                mainCompany() ->rollback();
            } else {
                mainCompany() ->commit();
                emit guardartarifa();
            } // end if
            dialogChanges_cargaInicial();
            close();
        } // end if
    } // end if
    _depura ( "TarifaView::END_boton_borrar()\n", 0 );
}

