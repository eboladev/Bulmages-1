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

#include "bccompany.h"
#include "proyectoview.h"


/** Constructor de la clase del listado.
 *  Inicializa las variables y elementos del listado.
 */
/**
\param emp
\param parent
**/
ListProyectosView::ListProyectosView ( BcCompany *emp, QWidget *parent,  Qt::WFlags flag, edmode editmodo )
        : BlFormList ( emp, parent, flag, editmodo )
{
    BL_FUNC_DEBUG

    this->setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    setSubForm ( mui_listado );
    mui_listado->setMainCompany ( emp );
    mui_listado->setDbTableName ( "presupuestoc" );
    mui_listado->setDbFieldId ( "idpresupuestoc" );
    mui_listado->addSubFormHeader ( "idpresupuestoc", BlDbField::DbVarChar, BlDbField::DbNothing , BlSubFormHeader::DbNoWrite, _ ( "Identificador" ) );
    mui_listado->addSubFormHeader ( "idc_coste", BlDbField::DbVarChar, BlDbField::DbNothing , BlSubFormHeader::DbNoWrite, _ ( "C. Coste" ) );
    mui_listado->addSubFormHeader ( "fechapresupuestoc", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite , _ ( "Fecha" ) );
    mui_listado->addSubFormHeader ( "nombrepresupuestoc", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite , _ ( "Nombre" ) );
    mui_listado->addSubFormHeader ( "comentpresupuestoc", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite , _ ( "Comentarios" ) );
    mui_listado->addSubFormHeader ( "archpresupuestoc", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite , _ ( "Archivo" ) );
    mui_listado->setInsert ( FALSE );

    presentar();
    if ( editMode() ) {
        mainCompany() ->insertWindow ( windowTitle(), this );
    } else {
        setWindowTitle ( _ ( "Selector de Proyectos" ) );
        mui_crear->setHidden ( TRUE );
        mui_borrar->setHidden ( TRUE );
    } // end if

    /// Lanzamos los posibles scripts
    blScript(this);

    
}


/** Destructor de la clase que libera mermoria y saca la ventana de la lista
 * de ventanas.
 */
/**
**/
ListProyectosView::~ListProyectosView()
{
    BL_FUNC_DEBUG
    removeWindow();
    
}


/** SLOT que responde a la pulsacion del boton crear una nueva amortizacion.
 * Crea una instancia de la clase AmortizacionView y la llama.
 */
/**
**/
void ListProyectosView::crear()
{
    BL_FUNC_DEBUG
    ProyectoView *proj = new ProyectoView ( ( BcCompany * ) mainCompany(), 0 );
    mainCompany() ->pWorkspace() ->addSubWindow ( proj );
    proj->show();
    
}


/// Esta funci&oacute;n se encarga de borrar una amortizaci&oacute;n.
/// La que esta seleccionada en el listado.
/**
**/
void ListProyectosView::remove()
{
    BL_FUNC_DEBUG
    try {
        QString codigo = mui_listado->dbValue ( "idamortizacion" );
        if ( codigo != "" ) {
            QString query = "DELETE FROM linamortizacion WHERE idamortizacion = " + codigo;
            mainCompany() ->begin();
            mainCompany() ->runQuery ( query );
            query = "DELETE FROM amortizacion WHERE idamortizacion = " + codigo;
            mainCompany() ->runQuery ( query );
            mainCompany() ->commit();
            presentar();
        } // end if
    } catch ( ... ) {
        BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error al borrar la amortizacion.") );
    } // end try
    
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
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("Fila: '%1'.")).arg(QString::number(row)) );
    mdb_idpresupuestoc = mui_listado->dbValue ( "idpresupuestoc" );
    mdb_nompresupuestoc = mui_listado->dbValue ( "nompresupuestoc" );
    if ( editMode() ) {
        /// Creamos el objeto BcMasaPatrimonialView, y lo lanzamos.
        ProyectoView * amor = new ProyectoView ( ( BcCompany * ) mainCompany(), 0 );
        amor->load ( mdb_idpresupuestoc );
        mainCompany() ->pWorkspace() ->addSubWindow ( amor );
        amor->show();
    } else {
        close();
        emit ( selected ( mdb_idpresupuestoc ) );
    } // end if
    
}

