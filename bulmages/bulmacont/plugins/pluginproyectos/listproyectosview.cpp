/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include "listproyectosview.h"
#include "listproyectosview.h"

#include "empresa.h"
#include "proyectoview.h"


/** Constructor de la clase del listado.
 *  Inicializa las variables y elementos del listado.
 */
/**
\param emp
\param parent
**/
ListProyectosView::ListProyectosView ( Empresa *emp, QWidget *parent,  Qt::WFlags flag, edmode editmodo )
        : BlFormList ( emp, parent, flag, editmodo )
{
    _depura ( "ListProyectosView::ListProyectosView", 0 );

    this->setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    setSubForm ( mui_listado );
    mui_listado->setEmpresaBase ( emp );
    mui_listado->setDBTableName ( "presupuestoc" );
    mui_listado->setDBCampoId ( "idpresupuestoc" );
    mui_listado->addSHeader ( "idpresupuestoc", DBCampo::DBvarchar, DBCampo::DBNothing , BlSubFormHeader::DBNoWrite, _( "Identificador" ) );
    mui_listado->addSHeader ( "idc_coste", DBCampo::DBvarchar, DBCampo::DBNothing , BlSubFormHeader::DBNoWrite, _( "C. Coste" ) );
    mui_listado->addSHeader ( "fechapresupuestoc", DBCampo::DBvarchar, DBCampo::DBNothing, BlSubFormHeader::DBNoWrite , _( "Fecha" ) );
    mui_listado->addSHeader ( "nombrepresupuestoc", DBCampo::DBvarchar, DBCampo::DBNothing, BlSubFormHeader::DBNoWrite , _( "Nombre" ) );
    mui_listado->addSHeader ( "comentpresupuestoc", DBCampo::DBvarchar, DBCampo::DBNothing, BlSubFormHeader::DBNoWrite , _( "Comentarios" ) );
    mui_listado->addSHeader ( "archpresupuestoc", DBCampo::DBvarchar, DBCampo::DBNothing, BlSubFormHeader::DBNoWrite , _( "Archivo" ) );
    mui_listado->setinsercion ( FALSE );

    presentar();
    if ( modoEdicion() ) {
        empresaBase() ->meteWindow ( windowTitle(), this );
    } else {
        setWindowTitle ( _( "Selector de Proyectos" ) );
        mui_crear->setHidden ( TRUE );
        mui_borrar->setHidden ( TRUE );
    } // end if

    _depura ( "END ListProyectosView::ListProyectosView", 0 );
}


/** Destructor de la clase que libera mermoria y saca la ventana de la lista
 * de ventanas.
 */
/**
**/
ListProyectosView::~ListProyectosView()
{
    _depura ( "ListProyectosView::~ListProyectosView", 0 );
    sacaWindow();
    _depura ( "END ListProyectosView::~ListProyectosView", 0 );
}


/** SLOT que responde a la pulsacion del boton crear una nueva amortizacion.
 * Crea una instancia de la clase AmortizacionView y la llama.
 */
/**
**/
void ListProyectosView::crear()
{
    _depura ( "ListProyectosView::on_mui_crear_clicked", 0 );
    ProyectoView *proj = new ProyectoView ( ( Empresa * ) empresaBase(), 0 );
    empresaBase() ->pWorkspace() ->addWindow ( proj );
    proj->show();
    _depura ( "END ListProyectosView::on_mui_crear_clicked", 0 );
}


/// Esta funci&oacute;n se encarga de borrar una amortizaci&oacute;n.
/// La que esta seleccionada en el listado.
/**
**/
void ListProyectosView::borrar()
{
    _depura ( "ListProyectosView::on_mui_borrar_clicked", 0 );
    try {
        QString codigo = mui_listado->DBvalue ( "idamortizacion" );
        if ( codigo != "" ) {
            QString query = "DELETE FROM linamortizacion WHERE idamortizacion = " + codigo;
            empresaBase() ->begin();
            empresaBase() ->ejecuta ( query );
            query = "DELETE FROM amortizacion WHERE idamortizacion = " + codigo;
            empresaBase() ->ejecuta ( query );
            empresaBase() ->commit();
            presentar();
        } // end if
    } catch ( ... ) {
        _depura ( _( "Error al borrar la amortizacion" ), 2 );
    } // end try
    _depura ( "END ListProyectosView::on_mui_borrar_clicked", 0 );
}



/** Se encarga de la accion preseleccionada al hacer doble click o al darle
    al boton de editar.
    Si estamos en modo seleccion actualiza el valor de los campos de seleccion y
    cierra la ventana.
    Si estamos en modo edicion abre una instancia de ArticuloView y lo carga con el
    valor seleccionado.
*/
/**
\param row
**/
void ListProyectosView::editar ( int row )
{
    _depura ( "ListProyectosView::editAmortizacion " + row, 0 );
    mdb_idpresupuestoc = mui_listado->DBvalue ( "idpresupuestoc" );
    mdb_nompresupuestoc = mui_listado->DBvalue ( "nompresupuestoc" );
    if ( modoEdicion() ) {
        /// Creamos el objeto mpatrimonialview, y lo lanzamos.
        ProyectoView * amor = new ProyectoView ( ( Empresa * ) empresaBase(), 0 );
        amor->cargar ( mdb_idpresupuestoc );
        empresaBase() ->pWorkspace() ->addWindow ( amor );
        amor->show();
    } else {
        close();
        emit ( selected ( mdb_idpresupuestoc ) );
    } // end if
    _depura ( "END ListProyectosView::editAmortizacion", 0 );
}

