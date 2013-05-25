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

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QCheckBox>
#include <QtCore/QFile>
#include <QtWidgets/QCheckBox>
#include <QtCore/QTextStream>
#include <QtWidgets/QMenu>

#include "blconfiguration.h"
#include "jdirectivalist.h"
#include "bfcompany.h"
#include "jdirectivaview.h"
#include "blfunctions.h"

/** Inicializa todos los componentes.
    Mete la ventana en el workSpace.
    Este constructor no es completo, debe inicializarse con setcompany para que la clase pueda operar.
*/
JDirectivaList::JDirectivaList ( QWidget *parent, Qt::WindowFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    BL_FUNC_DEBUG
    
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    
    /// Disparamos los plugins.
    int res = g_plugins->run ( "JDirectivaList_JDirectivaList", this );
    if ( res != 0 )
        return;
        
    mdb_idjdirectiva = "";
    setSubForm ( mui_list );
    hideBusqueda();
    /// Llamamos a los scripts
    blScript(this);
    
}

/** Inicializa todos los componentes.
    Hace una presentacion inicial.
    Mete la ventana en el workSpace.
*/
JDirectivaList::JDirectivaList ( BfCompany *comp, QWidget *parent, Qt::WindowFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    BL_FUNC_DEBUG
    
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    
    /// Disparamos los plugins.
    int res = g_plugins->run ( "JDirectivaList_JDirectivaList", this );
    if ( res != 0 )
        return;

    mui_list->setMainCompany ( comp );

    setSubForm ( mui_list );
    
    /// Cargamos los filtros guardados.
    cargaFiltrosXML();
    
    presentar();
    mdb_idjdirectiva = "";
    
    if ( editMode() )
        mainCompany() ->insertWindow ( windowTitle(), this );
        
    hideBusqueda();
    
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "jdirectiva" );

    /// Llamamos a los scripts
    blScript(this);
    
}

/** No requiere acciones especiales en el destructor.
*/
JDirectivaList::~JDirectivaList()
{
    BL_FUNC_DEBUG
    
}

/** Hace la carag del listado.
    PAra ello genera los SELECTS con ayuda de generaFiltro y los pasa al SubFormulario para que los presente.
    Tambien hace un select de calculo de totales y lo presenta en el textEdit correspondiente.
*/
void JDirectivaList::presentar()
{
    BL_FUNC_DEBUG
    
    if ( mainCompany() != NULL ) {
        mui_list->load ( "SELECT * FROM jdirectiva  WHERE 1 = 1 " + generaFiltro() );
    } // end if
    
    
}

/** Metodo auxiliar que genera la clausula WHERE del listado con las opciones de filtrado especificadas.
*/
const QString JDirectivaList::generaFiltro()
{
    BL_FUNC_DEBUG
    
    QString filtro = "";

    /// Hacemos el filtrado like del campo m_filtro
    filtro += mui_list->likeFilterSQL(m_filtro->text());
    
    return ( filtro );
}


/** SLOT que responde a la pulsacion del boton de crear en el listado.
    Instancia la clase JDirectivaView, y la presenta.
*/
void JDirectivaList::crear()
{
    BL_FUNC_DEBUG
    
    JDirectivaView *bud = new JDirectivaView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    bud->pintar();

        
}


/** La impresion de listados esta completamente delegada a la clase SubForm3
*/
void JDirectivaList::imprimir()
{
    BL_FUNC_DEBUG
    
    printPDF ( _ ( "Actividades" ) );
    
    
}

/** SLOT que responde a la pulsacion del boton borrar.
    Comprueba que exista un elemento seleccionado.
    Instancia la clase JDirectivaView, la inicializa y le lanza el metodo borrar.
    Esta es la forma correcta de implementar un borrado a partir de un listado
    ya que de esta forma si existen plugins que alteren el borrado tambien seran invocados.
*/
void JDirectivaList::remove()
{
    BL_FUNC_DEBUG
    
    int a = mui_list->currentRow();
    
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Debe seleccionar una linea" ) );
        return;
    } // end if
    
    try {
        mdb_idjdirectiva = mui_list->dbValue ( "idjdirectiva" );
        if ( editMode() ) {
            JDirectivaView * cv = new JDirectivaView ( ( BfCompany * ) mainCompany(), 0 );
            if ( cv->load ( mdb_idjdirectiva ) )
                throw - 1;
            cv->on_mui_borrar_clicked();
            cv->close();
        } // end if
        presentar();
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al borrar el cobro a cliente" ) );
    } // end try
    
    
}

void JDirectivaList::editar ( int )
{
    BL_FUNC_DEBUG
    
    try {
        mdb_idjdirectiva = mui_list->dbValue ( "idjdirectiva" );
        if ( editMode() ) {
            JDirectivaView * bud = new JDirectivaView ( ( BfCompany * ) mainCompany(), 0 );
            if ( bud->load ( mdb_idjdirectiva ) ) {
                delete bud;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addSubWindow ( bud );
            bud->show();
        } else {
            emit ( selected ( mdb_idjdirectiva ) );
        } // end if
    } catch ( ... ) {
        blMsgInfo ( _ ( "Debe seleccionar una fila primero" ) );
    } // end try
    
    
}

/** SLOT que responde a la peticion de menu contextual en el subformulario.
*/
/// \TODO: Revisar si este metodo es util.
void JDirectivaList::submenu ( const QPoint & )
{
    BL_FUNC_DEBUG
    
    int a = mui_list->currentRow();
    
    if ( a < 0 )
        return;
        
    QMenu *popup = new QMenu ( this );
    QAction *edit = popup->addAction ( _ ( "Editar Junta" ) );
    QAction *del = popup->addAction ( _ ( "Borrar Junta" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );
    
    if ( opcion == del )
        on_mui_borrar_clicked();
        
    if ( opcion == edit )
        on_mui_editar_clicked();
        
    delete popup;
    
    
}

/** Inicializa la clase con el puntero a la company que se esta utilizando
**/
void JDirectivaList::setMainCompany ( BfCompany *comp )
{
    BlMainCompanyPointer::setMainCompany ( comp );
//    mui_list->setMainCompany ( comp );
}

/** Devuelve el identificador del cobro seleccionado
**/
QString JDirectivaList::idjdirectiva()
{
    BL_FUNC_DEBUG
    
    
    return mdb_idjdirectiva;
}

/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para trabajar con la tabla cobro.
    Establece que no se puedan insertar elementos ya que es un listado.
*/
JDirectivaListSubForm::JDirectivaListSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    
    /// Disparamos los plugins.
    int res = g_plugins->run ( "JDirectivaListSubForm_JDirectivaListSubForm", this );
    if ( res != 0 )
        return;
        
    setDbTableName ( "jdirectiva" );
    setDbFieldId ( "idjdirectiva" );
    addSubFormHeader ( "idjdirectiva", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Junta Directiva" ) );
    addSubFormHeader ( "fechainjdirectiva", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha de constitucion" ) );
    addSubFormHeader ( "fechafinjdirectiva", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha de disolucion" ) );
    
    setInsert ( false );
    setDelete ( false );
    setSortingEnabled ( true );
    
    
}

JDirectivaListSubForm::~JDirectivaListSubForm()
{
    BL_FUNC_DEBUG
    
}
