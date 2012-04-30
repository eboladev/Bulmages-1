/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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
#include <QCheckBox>
#include <QFile>
#include <QCheckBox>
#include <QTextStream>
#include <QMenu>

#include "blconfiguration.h"
#include "actividadeslist.h"
#include "bfcompany.h"
#include "actividadview.h"
#include "blfunctions.h"

/** Inicializa todos los componentes.
    Mete la ventana en el workSpace.
    Este constructor no es completo, debe inicializarse con setcompany para que la clase pueda operar.
*/
ActividadesList::ActividadesList ( QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    BL_FUNC_DEBUG
    
    setupUi ( this );
    
    /// Disparamos los plugins.
    int res = g_plugins->run ( "ActividadesList_ActividadesList", this );
    if ( res != 0 )
        return;
        
    mdb_idactividad = "";
    setSubForm ( mui_list );
    hideBusqueda();

    /// Llamamos a los scripts
    blScript(this);
    
}

/** Inicializa todos los componentes.
    Hace una presentacion inicial.
    Mete la ventana en el workSpace.
*/
ActividadesList::ActividadesList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    BL_FUNC_DEBUG
    
    setupUi ( this );
    
    /// Disparamos los plugins.
    int res = g_plugins->run ( "ActividadesList_ActividadesList", this );
    if ( res != 0 )
        return;

    mui_list->setMainCompany ( comp );

    setSubForm ( mui_list );
    
    /// Cargamos los filtros guardados.
    cargaFiltrosXML();
    
    presentar();
    mdb_idactividad = "";
    
    if ( editMode() )
        mainCompany() ->insertWindow ( windowTitle(), this );
        
    hideBusqueda();
    
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "actividad" );
    /// Llamamos a los scripts
    blScript(this);
    
}

/** No requiere acciones especiales en el destructor.
*/
ActividadesList::~ActividadesList()
{
    BL_FUNC_DEBUG
    
}

/** Hace la carag del listado.
    PAra ello genera los SELECTS con ayuda de generaFiltro y los pasa al SubFormulario para que los presente.
    Tambien hace un select de calculo de totales y lo presenta en el textEdit correspondiente.
*/
void ActividadesList::presentar()
{
    BL_FUNC_DEBUG
    
    if ( mainCompany() != NULL ) {
        mui_list->load ( "SELECT * FROM actividad LEFT JOIN tipoactividad ON tipoactividad.idtipoactividad = actividad.idtipoactividad LEFT JOIN profesor ON profesor.idprofesor = actividad.idprofesor WHERE 1 = 1 " + generaFiltro() );
    } // end if
    
    
}

/** Metodo auxiliar que genera la clausula WHERE del listado con las opciones de filtrado especificadas.
*/
const QString ActividadesList::generaFiltro()
{
    BL_FUNC_DEBUG
    
    QString filtro = "";
    
    /// Hacemos el filtrado like del campo m_filtro
    filtro += mui_list->likeFilterSQL(m_filtro->text());
    
    return ( filtro );
}

/** SLOT que responde a la pulsacion del boton de crear en el listado.
    Instancia la clase ActividadView, y la presenta.
*/
void ActividadesList::crear()
{
    BL_FUNC_DEBUG
    
    ActividadView *bud = new ActividadView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    
}

/** La impresion de listados esta completamente delegada a la clase SubForm3
*/
void ActividadesList::imprimir()
{
    BL_FUNC_DEBUG
    
    printPDF ( _ ( "Actividades" ) );
    
    
}

/** SLOT que responde a la pulsacion del boton borrar.
    Comprueba que exista un elemento seleccionado.
    Instancia la clase ActividadView, la inicializa y le lanza el metodo borrar.
    Esta es la forma correcta de implementar un borrado a partir de un listado
    ya que de esta forma si existen plugins que alteren el borrado tambien seran invocados.
*/
void ActividadesList::remove()
{
    BL_FUNC_DEBUG
    
    int a = mui_list->currentRow();
    
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Debe seleccionar una linea" ) );
        return;
    } // end if
    
    try {
        mdb_idactividad = mui_list->dbValue ( "idactividad" );
        if ( editMode() ) {
            ActividadView * cv = new ActividadView ( ( BfCompany * ) mainCompany(), 0 );
            if ( cv->load ( mdb_idactividad ) )
                throw - 1;
            cv->on_mui_borrar_clicked();
            cv->close();
        } // end if
        presentar();
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al borrar el cobro a cliente" ) );
    } // end try
    
    
}

/** SLOT que responde al doble click en el subformulario.
    Dependiendo del modo (Edicion o Seleccion) hace unas operaciones u otras.
*/
/// \TODO: Deberia crearse el metodo editar y este llamar a ese.
void ActividadesList::editar ( int )
{
    BL_FUNC_DEBUG
    
    try {
        mdb_idactividad = mui_list->dbValue ( "idactividad" );
        if ( editMode() ) {
            ActividadView * bud = new ActividadView ( ( BfCompany * ) mainCompany(), 0 );
            if ( bud->load ( mdb_idactividad ) ) {
                delete bud;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addSubWindow ( bud );
            bud->show();
        } else {
	    close();
            emit ( selected ( mdb_idactividad ) );
        } // end if
    } catch ( ... ) {
        blMsgInfo ( _ ( "Debe seleccionar una fila primero" ) );
    } // end try
    
    

}

/** SLOT que responde a la peticion de menu contextual en el subformulario.
*/
/// \TODO: Revisar si este metodo es util.
void ActividadesList::submenu ( const QPoint & )
{
    BL_FUNC_DEBUG
    
    int a = mui_list->currentRow();
    
    if ( a < 0 )
        return;
        
    QMenu *popup = new QMenu ( this );
    QAction *edit = popup->addAction ( _ ( "Editar Actividad" ) );
    QAction *del = popup->addAction ( _ ( "Borrar Actividad" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );
    
    if ( opcion == del )
        on_mui_borrar_clicked();
        
    if ( opcion == edit )
        on_mui_editar_clicked();
        
    delete popup;
    
    
}

/** Inicializa la clase con el puntero a la company que se esta utilizando
**/
void ActividadesList::setMainCompany ( BfCompany *comp )
{
    BL_FUNC_DEBUG
    
    BlMainCompanyPointer::setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    
    
}

/** Devuelve el identificador del cobro seleccionado
**/
QString ActividadesList::idactividad()
{
    BL_FUNC_DEBUG
    
    
    return mdb_idactividad;
}

/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para trabajar con la tabla cobro.
    Establece que no se puedan insertar elementos ya que es un listado.
*/
ActividadesListSubForm::ActividadesListSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    
    /// Disparamos los plugins.
    int res = g_plugins->run ( "ActividadesListSubForm_ActividadesListSubForm", this );
    if ( res != 0 )
        return;
        
    setDbTableName ( "actividad" );
    setDbFieldId ( "idactividad" );
    addSubFormHeader ( "idactividad", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Actividad" ) );
    addSubFormHeader ( "nombreactividad", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre" ) );
    addSubFormHeader ( "nombreprofesor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Profesor" ) );
    addSubFormHeader ( "nombretipoactividad", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Tipo" ) );
    addSubFormHeader ( "precioactividad", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Precio" ) );
    addSubFormHeader ( "finicialactividad", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha Inicial" ) );
    addSubFormHeader ( "ffinalactividad", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha Final" ) );
    addSubFormHeader ( "hinicialactividad", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Hora Inicial" ) );
    addSubFormHeader ( "duracionactividad", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Duracion" ) );
    
    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    
    
}

ActividadesListSubForm::~ActividadesListSubForm()
{
    BL_FUNC_DEBUG
    
}

