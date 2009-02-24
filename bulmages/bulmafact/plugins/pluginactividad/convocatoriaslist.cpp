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
#include "convocatoriaslist.h"
#include "bfcompany.h"
#include "convocatoriaview.h"
#include "blfunctions.h"


/** Inicializa todos los componentes.
    Mete la ventana en el workSpace.
    Este constructor no es completo, debe inicializarse con setcompany para que la clase pueda operar.
*/
ConvocatoriasList::ConvocatoriasList ( QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    _depura ( "ConvocatoriasList::ConvocatoriasList", 0 );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ConvocatoriasList_ConvocatoriasList", this );
    if ( res != 0 )
        return;
    mdb_idactividad = "";
    setSubForm ( mui_list );
    hideBusqueda();

    _depura ( "END ConvocatoriasList::ConvocatoriasList", 0 );
}


/** Inicializa todos los componentes.
    Hace una presentacion inicial.
    Mete la ventana en el workSpace.
*/
ConvocatoriasList::ConvocatoriasList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    _depura ( "ConvocatoriasList::ConvocatoriasList", 0 );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ConvocatoriasList_ConvocatoriasList", this );
    if ( res != 0 )
        return;

    mui_list->setMainCompany ( comp );


    setSubForm ( mui_list );
    presentar();
    mdb_idactividad = "";
    if ( modoEdicion() )
        mainCompany() ->meteWindow ( windowTitle(), this );
    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "cobro" );
    _depura ( "END ConvocatoriasList::ConvocatoriasList", 0 );
}


/** No requiere acciones especiales en el destructor.
*/
ConvocatoriasList::~ConvocatoriasList()
{
    _depura ( "ConvocatoriasList::~ConvocatoriasList", 0 );
    _depura ( "END ConvocatoriasList::~ConvocatoriasList", 0 );
}


/** Hace la carag del listado.
    PAra ello genera los SELECTS con ayuda de generaFiltro y los pasa al SubFormulario para que los presente.
    Tambien hace un select de calculo de totales y lo presenta en el textEdit correspondiente.
*/
void ConvocatoriasList::presentar()
{
    _depura ( "ConvocatoriasList::presentar", 0 );
    if ( mainCompany() != NULL ) {
        mui_list->cargar ( "SELECT * FROM actividad WHERE 1 = 1 " + generaFiltro() );
    } // end if
    _depura ( "END ConvocatoriasList::presentar", 0 );
}


/** Metodo auxiliar que genera la clausula WHERE del listado con las opciones de filtrado especificadas.
*/
QString ConvocatoriasList::generaFiltro()
{
    _depura ( "ConvocatoriasList::generaFiltro", 0 );
    QString filtro = "";
    if ( m_filtro->text() != "" ) {
        filtro = " AND ( lower(nombreactividad) LIKE lower('%" + m_filtro->text() + "%') ";
        filtro += " ) ";
    } // end if

    _depura ( "END ConvocatoriasList::generaFiltro", 0 );
    return ( filtro );
}



/** SLOT que responde a la pulsacion del boton de crear en el listado.
    Instancia la clase ConvocatoriaView, y la presenta.
*/
void ConvocatoriasList::crear()
{
    _depura ( "ConvocatoriasList::crear", 0 );
    ConvocatoriaView *bud = new ConvocatoriaView( (BfCompany *) mainCompany(), 0);
    mainCompany() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    bud->pintar();
    _depura ( "ConvocatoriasList::crear", 0 );
}


/** La impresion de listados esta completamente delegada a la clase SubForm3
*/
void ConvocatoriasList::imprimir()
{
    _depura ( "ConvocatoriasList::imprimir", 0 );
    mui_list->imprimirPDF ( _( "Convocatorias" ) );
    _depura ( "END ConvocatoriasList::imprimir", 0 );
}


/** SLOT que responde a la pulsacion del boton borrar.
    Comprueba que exista un elemento seleccionado.
    Instancia la clase ConvocatoriaView, la inicializa y le lanza el metodo borrar.
    Esta es la forma correcta de implementar un borrado a partir de un listado
    ya que de esta forma si existen plugins que alteren el borrado tambien seran invocados.
*/
void ConvocatoriasList::borrar()
{
    _depura ( "ConvocatoriasList::borrar", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        mensajeInfo ( _( "Debe seleccionar una linea" ) );
        return;
    } // end if
    try {
        mdb_idactividad = mui_list->dbValue ( "idactividad" );
        if ( modoEdicion() ) {
            ConvocatoriaView * cv = new ConvocatoriaView( (BfCompany *) mainCompany(), 0);
            if ( cv->cargar ( mdb_idactividad ) )
                throw - 1;
            cv->on_mui_borrar_clicked();
            cv->close();
        } // end if
        presentar();
    } catch ( ... ) {
        mensajeInfo ( _( "Error al borrar el cobro a cliente" ) );
    } // end try
    _depura ( "END:ConvocatoriasList::borrar", 0 );
}


/** SLOT que responde al doble click en el subformulario.
    Dependiendo del modo (Edicion o Seleccion) hace unas operaciones u otras.
*/
/// \TODO: Deberia crearse el metodo editar y este llamar a ese.
void ConvocatoriasList::editar ( int )
{
    _depura ( "ConvocatoriasList::on_mui_list_cellDoubleClicked", 0 );
    try {
        mdb_idactividad = mui_list->dbValue ( "idactividad" );
        if ( modoEdicion() ) {
            ConvocatoriaView * bud = new ConvocatoriaView( (BfCompany *) mainCompany(), 0);
            if ( bud->cargar ( mdb_idactividad ) ) {
                delete bud;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addWindow ( bud );
            bud->show();
        } else {
            emit ( selected ( mdb_idactividad ) );
        } // end if
    } catch ( ... ) {
        mensajeInfo ( _( "Debe seleccionar una fila primero" ) );
    } // end try
    _depura ( "END ConvocatoriasList::on_mui_list_cellDoubleClicked", 0 );

}

/** SLOT que responde a la peticion de menu contextual en el subformulario.
*/
/// \TODO: Revisar si este metodo es util.
void ConvocatoriasList::submenu ( const QPoint & )
{
    _depura ( "ConvocatoriasList::on_mui_list_customContextMenuRequested", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 )
        return;
    QMenu *popup = new QMenu ( this );
    QAction *edit = popup->addAction ( _( "Editar Convocatoria" ) );
    QAction *del = popup->addAction ( _( "Borrar Convocatoria" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );
    if ( opcion == del )
        on_mui_borrar_clicked();
    if ( opcion == edit )
        on_mui_editar_clicked();
    delete popup;
    _depura ( "ConvocatoriasList::on_mui_list_customContextMenuRequested", 0 );
}

/** Inicializa la clase con el puntero a la company que se esta utilizando
**/
void ConvocatoriasList::setMainCompany ( BfCompany *comp )
{
    BlMainCompanyPointer::setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
}

/** Devuelve el identificador del cobro seleccionado
**/
QString ConvocatoriasList::idactividad()
{
    _depura ( "ConvocatoriasList::idactividad", 0 );
    _depura ( "END ConvocatoriasList::idactividad", 0 );
    return mdb_idactividad;
}



/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para trabajar con la tabla cobro.
    Establece que no se puedan insertar elementos ya que es un listado.
*/
ConvocatoriasListSubForm::ConvocatoriasListSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ConvocatoriasListSubForm::ConvocatoriasListSubForm", 0 );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ConvocatoriasListSubForm_ConvocatoriasListSubForm", this );
    if ( res != 0 )
        return;
    setDbTableName ( "actividad" );
    setDbFieldId ( "idactividad" );
    addSubFormHeader ( "idactividad", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoView | BlSubFormHeader::DbNoWrite, _( "ID Convocatoria" ) );
    addSubFormHeader ( "nombreactividad", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Nombre" ) );

    setinsercion ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    _depura ( "END ConvocatoriasListSubForm::ConvocatoriasListSubForm", 0 );
}


ConvocatoriasListSubForm::~ConvocatoriasListSubForm()
{
    _depura ( "ConvocatoriasListSubForm::~ConvocatoriasListSubForm", 0 );
    _depura ( "END ConvocatoriasListSubForm::~ConvocatoriasListSubForm", 0 );
}

