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

#include "almacenview.h"
#include "company.h"
#include "blconfiguration.h"
#include "plugins.h"
#include "funcaux.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
/**
\param comp
\param parent
**/
AlmacenView::AlmacenView ( Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "AlmacenView::AlmacenView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        /// Lanzamos los plugins.
        if ( g_plugins->lanza ( "AlmacenView_AlmacenView", this ) ) return;

        setTitleName ( _( "Almacen" ) );
        setDBTableName ( "almacen" );
        setDBCampoId ( "idalmacen" );
        addDBCampo ( "idalmacen", DBCampo::DBint, DBCampo::DBPrimaryKey, _( "ID almacen" ) );
        addDBCampo ( "codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNotNull, _( "Codigo almacen" ) );
        addDBCampo ( "nomalmacen", DBCampo::DBvarchar, DBCampo::DBNotNull  , _( "Nombre almacen" ) );
        addDBCampo ( "diralmacen", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Direccion almacen" ) );
        addDBCampo ( "poblalmacen", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Poblacion almacen" ) );
        addDBCampo ( "cpalmacen", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Codigo postal" ) );
        addDBCampo ( "telalmacen", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Telefono almacen" ) );
        addDBCampo ( "faxalmacen", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Fax almacen" ) );
        addDBCampo ( "emailalmacen", DBCampo::DBvarchar, DBCampo::DBNothing, _( "email almacen" ) );
        addDBCampo ( "fechaalmacen", DBCampo::DBdate, DBCampo::DBNoSave, _( "Fecha almacen" ) );


        addDBCampo ( "tipoalmacen", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Tipo almacen" ) );
        mui_tipoalmacen_t->setValorCampo ( "Tienda" );
        mui_tipoalmacen_a->setValorCampo ( "Almacen" );

        dialogChanges_cargaInicial();
        meteWindow ( windowTitle(), this, FALSE );
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear el almacen" ) );
    } // end try
    _depura ( "END AlmacenView::AlmacenView", 0 );
}


/// No precisa acciones adicionales en el destructor.

/**
**/
AlmacenView::~AlmacenView()
{
    _depura ( "AlmacenView::~AlmacenView", 0 );
    _depura ( "END AlmacenView::~AlmacenView", 0 );
}






