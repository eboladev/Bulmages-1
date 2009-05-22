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
#include <QCheckBox>
#include <QFile>
#include <QCheckBox>
#include <QTextStream>
#include <QMenu>

#include "blconfiguration.h"
#include "comisioneslist.h"
#include "bfcompany.h"
#include "comisionview.h"
#include "blfunctions.h"


/** Inicializa todos los componentes.
    Mete la ventana en el workSpace.
    Este constructor no es completo, debe inicializarse con setcompany para que la clase pueda operar.
*/
ComisionesList::ComisionesList ( QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    _depura ( "ComisionesList::ComisionesList", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ComisionesList_ComisionesList", this );
    if ( res != 0 )
        return;
    mdb_idcomision = "";
    setSubForm ( mui_list );
    hideBusqueda();

    _depura ( "END ComisionesList::ComisionesList", 0 );
}


/** Inicializa todos los componentes.
    Hace una presentacion inicial.
    Mete la ventana en el workSpace.
*/
ComisionesList::ComisionesList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    _depura ( "ComisionesList::ComisionesList", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ComisionesList_ComisionesList", this );
    if ( res != 0 )
        return;

    mui_list->setMainCompany ( comp );


    setSubForm ( mui_list );
    presentar();
    mdb_idcomision = "";
    if ( modoEdicion() )
        mainCompany() ->meteWindow ( windowTitle(), this );
    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "cobro" );
    _depura ( "END ComisionesList::ComisionesList", 0 );
}


/** No requiere acciones especiales en el destructor.
*/
ComisionesList::~ComisionesList()
{
    _depura ( "ComisionesList::~ComisionesList", 0 );
    _depura ( "END ComisionesList::~ComisionesList", 0 );
}


/** Hace la carag del listado.
    PAra ello genera los SELECTS con ayuda de generaFiltro y los pasa al SubFormulario para que los presente.
    Tambien hace un select de calculo de totales y lo presenta en el textEdit correspondiente.
*/
void ComisionesList::presentar()
{
    _depura ( "ComisionesList::presentar", 0 );
    if ( mainCompany() != NULL ) {
        mui_list->cargar ( "SELECT * FROM comision  WHERE 1 = 1 " + generaFiltro() );
    } // end if
    _depura ( "END ComisionesList::presentar", 0 );
}


/** Metodo auxiliar que genera la clausula WHERE del listado con las opciones de filtrado especificadas.
*/
QString ComisionesList::generaFiltro()
{
    _depura ( "ComisionesList::generaFiltro", 0 );
    QString filtro = "";

    _depura ( "END ComisionesList::generaFiltro", 0 );
    return ( filtro );
}



/** SLOT que responde a la pulsacion del boton de crear en el listado.
    Instancia la clase ComisionView, y la presenta.
*/
void ComisionesList::crear()
{
    _depura ( "ComisionesList::crear", 0 );
    ComisionView *bud = new ComisionView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    bud->pintar();
    _depura ( "ComisionesList::crear", 0 );
}


/** La impresion de listados esta completamente delegada a la clase SubForm3
*/
void ComisionesList::imprimir()
{
    _depura ( "ComisionesList::imprimir", 0 );
    mui_list->imprimirPDF ( _ ( "Actividades" ) );
    _depura ( "END ComisionesList::imprimir", 0 );
}


/** SLOT que responde a la pulsacion del boton borrar.
    Comprueba que exista un elemento seleccionado.
    Instancia la clase ComisionView, la inicializa y le lanza el metodo borrar.
    Esta es la forma correcta de implementar un borrado a partir de un listado
    ya que de esta forma si existen plugins que alteren el borrado tambien seran invocados.
*/
void ComisionesList::borrar()
{
    _depura ( "ComisionesList::borrar", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        mensajeInfo ( _ ( "Debe seleccionar una linea" ) );
        return;
    } // end if
    try {
        mdb_idcomision = mui_list->dbValue ( "idcomision" );
        if ( modoEdicion() ) {
            ComisionView * cv = new ComisionView ( ( BfCompany * ) mainCompany(), 0 );
            if ( cv->cargar ( mdb_idcomision ) )
                throw - 1;
            cv->on_mui_borrar_released();
            cv->close();
        } // end if
        presentar();
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al borrar el cobro a cliente" ) );
    } // end try
    _depura ( "END:ComisionesList::borrar", 0 );
}


/** SLOT que responde al doble click en el subformulario.
    Dependiendo del modo (Edicion o Seleccion) hace unas operaciones u otras.
*/
/// \TODO: Deberia crearse el metodo editar y este llamar a ese.
void ComisionesList::editar ( int )
{
    _depura ( "ComisionesList::on_mui_list_cellDoubleClicked", 0 );
    try {
        mdb_idcomision = mui_list->dbValue ( "idcomision" );
        if ( modoEdicion() ) {
            ComisionView * bud = new ComisionView ( ( BfCompany * ) mainCompany(), 0 );
            if ( bud->cargar ( mdb_idcomision ) ) {
                delete bud;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addWindow ( bud );
            bud->show();
        } else {
            emit ( selected ( mdb_idcomision ) );
        } // end if
    } catch ( ... ) {
        mensajeInfo ( _ ( "Debe seleccionar una fila primero" ) );
    } // end try
    _depura ( "END ComisionesList::on_mui_list_cellDoubleClicked", 0 );

}

/** SLOT que responde a la peticion de menu contextual en el subformulario.
*/
/// \TODO: Revisar si este metodo es util.
void ComisionesList::submenu ( const QPoint & )
{
    _depura ( "ComisionesList::on_mui_list_customContextMenuRequested", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 )
        return;
    QMenu *popup = new QMenu ( this );
    QAction *edit = popup->addAction ( _ ( "Editar Junta" ) );
    QAction *del = popup->addAction ( _ ( "Borrar Junta" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );
    if ( opcion == del )
        on_mui_borrar_released();
    if ( opcion == edit )
        on_mui_editar_released();
    delete popup;
    _depura ( "ComisionesList::on_mui_list_customContextMenuRequested", 0 );
}

/** Inicializa la clase con el puntero a la company que se esta utilizando
**/
void ComisionesList::setMainCompany ( BfCompany *comp )
{
    BlMainCompanyPointer::setMainCompany ( comp );
//    mui_list->setMainCompany ( comp );
}

/** Devuelve el identificador del cobro seleccionado
**/
QString ComisionesList::idcomision()
{
    _depura ( "ComisionesList::idcomision", 0 );
    _depura ( "END ComisionesList::idcomision", 0 );
    return mdb_idcomision;
}



/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para trabajar con la tabla cobro.
    Establece que no se puedan insertar elementos ya que es un listado.
*/
ComisionesListSubForm::ComisionesListSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ComisionesListSubForm::ComisionesListSubForm", 0 );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ComisionesListSubForm_ComisionesListSubForm", this );
    if ( res != 0 )
        return;
    setDbTableName ( "comision" );
    setDbFieldId ( "idcomision" );
    addSubFormHeader ( "idcomision", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Junta Directiva" ) );
    addSubFormHeader ( "fechaincomision", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Constitucion" ) );
    addSubFormHeader ( "fechafincomision", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Constitucion" ) );
    
    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    _depura ( "END ComisionesListSubForm::ComisionesListSubForm", 0 );
}


ComisionesListSubForm::~ComisionesListSubForm()
{
    _depura ( "ComisionesListSubForm::~ComisionesListSubForm", 0 );
    _depura ( "END ComisionesListSubForm::~ComisionesListSubForm", 0 );
}

