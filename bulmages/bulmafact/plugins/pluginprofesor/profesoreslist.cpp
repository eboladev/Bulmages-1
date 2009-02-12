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
#include "profesoreslist.h"
#include "company.h"
#include "profesorview.h"
#include "funcaux.h"


/** Inicializa todos los componentes.
    Mete la ventana en el workSpace.
    Este constructor no es completo, debe inicializarse con setcompany para que la clase pueda operar.
*/
ProfesoresList::ProfesoresList ( QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    _depura ( "ProfesoresList::ProfesoresList", 0 );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ProfesoresList_ProfesoresList", this );
    if ( res != 0 )
        return;
    mdb_idprofesor = "";
    setSubForm ( mui_list );
    hideBusqueda();

    _depura ( "END ProfesoresList::ProfesoresList", 0 );
}


/** Inicializa todos los componentes.
    Hace una presentacion inicial.
    Mete la ventana en el workSpace.
*/
ProfesoresList::ProfesoresList ( Company *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    _depura ( "ProfesoresList::ProfesoresList", 0 );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ProfesoresList_ProfesoresList", this );
    if ( res != 0 )
        return;

    mui_list->setEmpresaBase ( comp );


    setSubForm ( mui_list );
    presentar();
    mdb_idprofesor = "";
    if ( modoEdicion() )
        empresaBase() ->meteWindow ( windowTitle(), this );
    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "cobro" );
    _depura ( "END ProfesoresList::ProfesoresList", 0 );
}


/** No requiere acciones especiales en el destructor.
*/
ProfesoresList::~ProfesoresList()
{
    _depura ( "ProfesoresList::~ProfesoresList", 0 );
    _depura ( "END ProfesoresList::~ProfesoresList", 0 );
}


/** Hace la carag del listado.
    PAra ello genera los SELECTS con ayuda de generaFiltro y los pasa al SubFormulario para que los presente.
    Tambien hace un select de calculo de totales y lo presenta en el textEdit correspondiente.
*/
void ProfesoresList::presentar()
{
    _depura ( "ProfesoresList::presentar", 0 );
    if ( empresaBase() != NULL ) {
        mui_list->cargar ( "SELECT * FROM profesor WHERE 1 = 1 " + generaFiltro() );
    } // end if
    _depura ( "END ProfesoresList::presentar", 0 );
}


/** Metodo auxiliar que genera la clausula WHERE del listado con las opciones de filtrado especificadas.
*/
QString ProfesoresList::generaFiltro()
{
    _depura ( "ProfesoresList::generaFiltro", 0 );
    QString filtro = "";
    if ( m_filtro->text() != "" ) {
        filtro = " AND ( lower(nombreprofesor) LIKE lower('%" + m_filtro->text() + "%') ";
        filtro += " ) ";
    } // end if

    _depura ( "END ProfesoresList::generaFiltro", 0 );
    return ( filtro );
}



/** SLOT que responde a la pulsacion del boton de crear en el listado.
    Instancia la clase ProfesorView, y la presenta.
*/
void ProfesoresList::crear()
{
    _depura ( "ProfesoresList::crear", 0 );
    ProfesorView *bud = new ProfesorView( (Company *) empresaBase(), 0);
    empresaBase() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    bud->pintar();
    _depura ( "ProfesoresList::crear", 0 );
}


/** La impresion de listados esta completamente delegada a la clase SubForm3
*/
void ProfesoresList::imprimir()
{
    _depura ( "ProfesoresList::imprimir", 0 );
    mui_list->imprimirPDF ( _( "Profesores" ) );
    _depura ( "END ProfesoresList::imprimir", 0 );
}


/** SLOT que responde a la pulsacion del boton borrar.
    Comprueba que exista un elemento seleccionado.
    Instancia la clase ProfesorView, la inicializa y le lanza el metodo borrar.
    Esta es la forma correcta de implementar un borrado a partir de un listado
    ya que de esta forma si existen plugins que alteren el borrado tambien seran invocados.
*/
void ProfesoresList::borrar()
{
    _depura ( "ProfesoresList::borrar", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        mensajeInfo ( _( "Debe seleccionar una linea" ) );
        return;
    } // end if
    try {
        mdb_idprofesor = mui_list->DBvalue ( "idprofesor" );
        if ( modoEdicion() ) {
            ProfesorView * cv = new ProfesorView( (Company *) empresaBase(), 0);
            if ( cv->cargar ( mdb_idprofesor ) )
                throw - 1;
            cv->on_mui_borrar_clicked();
            cv->close();
        } // end if
        presentar();
    } catch ( ... ) {
        mensajeInfo ( _( "Error al borrar el cobro a cliente" ) );
    } // end try
    _depura ( "END:ProfesoresList::borrar", 0 );
}


/** SLOT que responde al doble click en el subformulario.
    Dependiendo del modo (Edicion o Seleccion) hace unas operaciones u otras.
*/
/// \TODO: Deberia crearse el metodo editar y este llamar a ese.
void ProfesoresList::editar ( int )
{
    _depura ( "ProfesoresList::on_mui_list_cellDoubleClicked", 0 );
    try {
        mdb_idprofesor = mui_list->DBvalue ( "idprofesor" );
        if ( modoEdicion() ) {
            ProfesorView * bud = new ProfesorView( (Company *) empresaBase(), 0);
            if ( bud->cargar ( mdb_idprofesor ) ) {
                delete bud;
                return;
            } // end if
            empresaBase() ->m_pWorkspace->addWindow ( bud );
            bud->show();
        } else {
            emit ( selected ( mdb_idprofesor ) );
        } // end if
    } catch ( ... ) {
        mensajeInfo ( _( "Debe seleccionar una fila primero" ) );
    } // end try
    _depura ( "END ProfesoresList::on_mui_list_cellDoubleClicked", 0 );

}

/** SLOT que responde a la peticion de menu contextual en el subformulario.
*/
/// \TODO: Revisar si este metodo es util.
void ProfesoresList::submenu ( const QPoint & )
{
    _depura ( "ProfesoresList::on_mui_list_customContextMenuRequested", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 )
        return;
    QMenu *popup = new QMenu ( this );
    QAction *edit = popup->addAction ( _( "Editar profesor" ) );
    QAction *del = popup->addAction ( _( "Borrar profesor" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );
    if ( opcion == del )
        on_mui_borrar_clicked();
    if ( opcion == edit )
        on_mui_editar_clicked();
    delete popup;
    _depura ( "ProfesoresList::on_mui_list_customContextMenuRequested", 0 );
}

/** Inicializa la clase con el puntero a la company que se esta utilizando
**/
void ProfesoresList::setEmpresaBase ( Company *comp )
{
    PEmpresaBase::setEmpresaBase ( comp );
    mui_list->setEmpresaBase ( comp );
}

/** Devuelve el identificador del cobro seleccionado
**/
QString ProfesoresList::idprofesor()
{
    _depura ( "ProfesoresList::idprofesor", 0 );
    _depura ( "END ProfesoresList::idprofesor", 0 );
    return mdb_idprofesor;
}



/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para trabajar con la tabla cobro.
    Establece que no se puedan insertar elementos ya que es un listado.
*/
ProfesoresListSubForm::ProfesoresListSubForm ( QWidget *parent ) : SubForm2Bf ( parent )
{
    _depura ( "ProfesoresListSubForm::ProfesoresListSubForm", 0 );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ProfesoresListSubForm_ProfesoresListSubForm", this );
    if ( res != 0 )
        return;
    setDBTableName ( "profesor" );
    setDBCampoId ( "idprofesor" );
    addSHeader ( "idprofesor", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Id profesor" ) );
    addSHeader ( "nombreprofesor", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Nombre" ) );

    setinsercion ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    _depura ( "END ProfesoresListSubForm::ProfesoresListSubForm", 0 );
}


ProfesoresListSubForm::~ProfesoresListSubForm()
{
    _depura ( "ProfesoresListSubForm::~ProfesoresListSubForm", 0 );
    _depura ( "END ProfesoresListSubForm::~ProfesoresListSubForm", 0 );
}

