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

#include <QtGui/QCloseEvent>

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
    BL_FUNC_DEBUG
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
    mui_list->setEnabled ( false );
    mui_filtro->setEnabled ( false );
    mui_actualizar->setEnabled ( false );
    mui_borrar->setEnabled ( false );

    insertWindow ( _ ( "Tarifa edicion" ), this );

    dialogChanges_readValues();
    blScript(this);
    
}


///
/**
**/
TarifaView::~TarifaView()
{
    BL_FUNC_DEBUG
    mainCompany() ->removeWindow ( this );
    
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
    BL_FUNC_DEBUG
    mui_nomtarifa->setText ( dbValue ( "nomtarifa" ) );
    setWindowTitle ( _ ( "Tarifa" ) + " " + mui_nomtarifa->text() );
    
}


///
/**
\param idtarifa
\return
**/
QString TarifaView::formaQuery ( QString idtarifa )
{
    BL_FUNC_DEBUG
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
    
    return SQLQuery;
}


///
/**
\param idtarifa
\return
**/
int TarifaView::load ( QString idtarifa )
{
    BL_FUNC_DEBUG
    int error = 0;
    m_idtarifa = idtarifa;

    /// Si se ha guardado bien entonces de habilitan los botones y demas.
    mui_list->setEnabled ( true );
    mui_filtro->setEnabled ( true );
    mui_actualizar->setEnabled ( true );
    mui_borrar->setEnabled ( true && mainCompany()->hasTablePrivilege ( tableName(), "DELETE" ) );

    setDbValue ( "idtarifa", m_idtarifa );
    BlDbRecord::load ( m_idtarifa );
    mui_list->load ( formaQuery ( m_idtarifa ) );

    setWindowTitle ( _ ( "Tarifa" ) + " " + dbValue ( "nomtarifa" ) );
    insertWindow ( windowTitle(), this );

    /// Tratamiento de excepciones.
    if ( error == 1 ) {
	
        return -1;
    } // end if
    pintar();

    dialogChanges_readValues();
    
    return 0;
}


///
/**
\return
**/
int TarifaView::save()
{
    BL_FUNC_DEBUG

    /// Al pulsar sobre 'guardar' se tiene que comprobar si existe un 'idtarifa'
    /// Si no existe se crea una nueva tarifa (INSERT) en la base de datos con el nombre proporcionado:
    /// - Se comprueba que el nombre no este vacio.
    /// - \TODO: Se comprueba que no exista un nombre igual.
    /// Si existe un 'idtarifa' se actualiza sus datos (UPDATE).
    if ( mui_nomtarifa->text().isEmpty() ) {
        blMsgWarning ( _ ( "El nombre de la tarifa no puede estar vacio." ) );
        return -1;
    } // end if

    setDbValue ( "nomtarifa", mui_nomtarifa->text() );
    BlDbRecord::save();
    /// Guardamos la lista de componentes.
    mui_list->setColumnValue ( "idtarifa", dbValue ( "idtarifa" ) );
    mui_list->save();
    dialogChanges_readValues();

    /// Se recarga el listado de articulos para poder establecer precios a esa tarifa.
    load ( dbValue ( "idtarifa" ) );

    emit guardartarifa();

    
    return 0;
}


/// Esta funci&oacute;n se ejecuta cuando se ha pulsado sobre el bot&oacute;n de nuevo.
/**
**/
void TarifaView::on_mui_crear_clicked()
{
    BL_FUNC_DEBUG
    vaciar();
    pintar();
    
}


///
/**
**/
void TarifaView::on_mui_actualizar_clicked()
{
    BL_FUNC_DEBUG
    load ( m_idtarifa );
    
}


/// Esta funci&oacute;n se ejecuta cuando se ha pulsado sobre el bot&oacute;n de borrar.
/**
**/
void TarifaView::on_mui_borrar_clicked()
{
    BL_FUNC_DEBUG
    if ( dbValue ( "idtarifa" ) != "" ) {
        if ( QMessageBox::question ( this,
                                     _ ( "Borrar tarifa" ),
                                     _ ( "Esta a punto de borrar una tarifa. Desea continuar?" ),
                                     _ ( "&Si" ), _ ( "&No" ), 0, 1, 0 ) == 0 ) {
            mainCompany() ->begin();
            int error = mui_list->remove();
            error += remove();
            if ( error ) {
                mainCompany() ->rollback();
            } else {
                mainCompany() ->commit();
                emit guardartarifa();
            } // end if
            dialogChanges_readValues();
            close();
        } // end if
    } // end if
    
}

