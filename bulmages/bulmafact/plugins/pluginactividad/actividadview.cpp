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
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>

#include <fstream>

#include "actividadview.h"
#include "company.h"
#include "configuracion.h"
#include "busquedafecha.h"

#include "funcaux.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
ActividadView::ActividadView ( Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "ActividadView::ActividadView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );
        centrarEnPantalla ( this );

        setTitleName ( _( "Actividad" ) );
        setDBTableName ( "actividad" );
        setDBCampoId ( "idactividad" );
        addDBCampo ( "idactividad", DBCampo::DBint, DBCampo::DBPrimaryKey, _( "ID actividad" ) );
        addDBCampo ( "nombreactividad", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Nombre del actividad" ) );
	addDBCampo ( "idprofesor", DBCampo::DBint, DBCampo::DBNotNull, _("Id profesor"));

        meteWindow ( windowTitle(), this, FALSE );

		/// Establecemos los parametros de busqueda de Profesor
	mui_idprofesor->setEmpresaBase(comp);
    mui_idprofesor->setLabel ( _( "Profesor:" ) );
	mui_idprofesor->setTableName( "profesor" );
	mui_idprofesor->m_valores["nombreprofesor"] = "";


        pintar();
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear el actividad" ), this );
    } // end try
    _depura ( "END ActividadView::ActividadView", 0 );
}


/** No precisa acciones adicionales en el destructor.
*/
ActividadView::~ActividadView()
{
    _depura ( "ActividadView::~ActividadView", 0 );
    _depura ( "END ActividadView::~ActividadView", 0 );
}


QString ActividadView::nombrePlantilla(void) 
{
   return QString("recibo");
}

void ActividadView::imprimir()
{
    _depura ( "ActividadView::imprimir", 0 );
    /// Comprobamos que se disponen de los datos minimos para imprimir el recibo.
    QString SQLQuery = "";

    if ( DBvalue ( "idcliente" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        mensajeInfo ( _( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        return;
    } 
    /// Disparamos los plugins
    int res = g_plugins->lanza ( "CoboView_on_mui_imprimir_clicked", this );
    if ( res != 0 ) {
        return;
    } // end if
    FichaBf::imprimir();

    _depura ( "END ActividadView::imprimir", 0 );
}


int ActividadView::guardarPost() {
	_depura(" ActividadView::guardarPost", 0);

	_depura("END ActividadView::guardarPost", 0);

}

