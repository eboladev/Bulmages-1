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
#include "cobroslist.h"
#include "bfcompany.h"
#include "cobroview.h"
#include "blfunctions.h"


/** Inicializa todos los componentes.
    Mete la ventana en el workSpace.
    Este constructor no es completo, debe inicializarse con setcompany para que la clase pueda operar.
*/
CobrosList::CobrosList ( QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    _depura ( "CobrosList::CobrosList", 0 );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "CobrosList_CobrosList", this );
    if ( res != 0 )
        return;
    mdb_idcobro = "";
    setSubForm ( mui_list );
    /// Establecemos los parametros de busqueda del Cliente
    m_cliente->setLabel ( _ ( "Cliente:" ) );
    m_cliente->setTableName ( "cliente" );
    m_cliente->m_valores["cifcliente"] = "";
    m_cliente->m_valores["nomcliente"] = "";    hideBusqueda();

    _depura ( "END CobrosList::CobrosList", 0 );
}


/** Inicializa todos los componentes.
    Hace una presentacion inicial.
    Mete la ventana en el workSpace.
*/
CobrosList::CobrosList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    _depura ( "CobrosList::CobrosList", 0 );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "CobrosList_CobrosList", this );
    if ( res != 0 )
        return;
    m_cliente->setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    mui_idbanco->setMainCompany ( comp );
    mui_idbanco->setidbanco ( "" );
    setSubForm ( mui_list );
    /// Establecemos los parametros de busqueda del Cliente
    m_cliente->setLabel ( _ ( "Cliente:" ) );
    m_cliente->setTableName ( "cliente" );
    m_cliente->m_valores["cifcliente"] = "";
    m_cliente->m_valores["nomcliente"] = "";
    presentar();
    mdb_idcobro = "";
    if ( modoEdicion() )
        mainCompany() ->meteWindow ( windowTitle(), this );
    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "cobro" );

    _depura ( "END CobrosList::CobrosList", 0 );
}


/** No requiere acciones especiales en el destructor.
*/
CobrosList::~CobrosList()
{
    _depura ( "CobrosList::~CobrosList", 0 );
    _depura ( "END CobrosList::~CobrosList", 0 );
}


/** Hace la carga del listado.
    Para ello genera los SELECTS con ayuda de generaFiltro y los pasa al SubFormulario para que los presente.
    Tambien hace un select de calculo de totales y lo presenta en el textEdit correspondiente.
*/
void CobrosList::presentar()
{
    _depura ( "CobrosList::presentar", 0 );
    if ( mainCompany() != NULL ) {
        mui_list->cargar ( "SELECT * FROM cobro NATURAL LEFT JOIN cliente LEFT JOIN trabajador ON trabajador.idtrabajador = cobro.idtrabajador NATURAL LEFT JOIN banco WHERE 1 = 1 " + generaFiltro() );
        /// Hacemos el calculo del total.
        BlFixed total = mui_list->sumarCampo ( "cantcobro" );
        m_total->setText ( total.toQString() );
    } // end if
    _depura ( "END CobrosList::presentar", 0 );
}


/** Metodo auxiliar que genera la clausula WHERE del listado con las opciones de filtrado especificadas.
*/
QString CobrosList::generaFiltro()
{
    _depura ( "CobrosList::generaFiltro", 0 );
    QString filtro = "";
    if ( m_filtro->text() != "" ) {
        filtro = " AND ( lower(comentcobro) LIKE lower('%" + m_filtro->text() + "%') ";
        filtro += " OR refcobro LIKE '" + m_filtro->text() + "%' ";
        filtro += " OR lower(nomcliente) LIKE lower('%" + m_filtro->text() + "%')) ";
    } // end if

    if ( m_cliente->id() != "" ) {
        filtro += " AND cobro.idcliente = " + m_cliente->id();
    } // end if

    QString subfiltro = " AND ";
    if ( mui_efectivos->isChecked() ) {
        filtro += " AND NOT previsioncobro";
        subfiltro = " OR ";
    } // end if

    if ( mui_previsiones->isChecked() ) {
        filtro += subfiltro + " previsioncobro";
    } // end if

    if ( m_fechain->text() != "" )
        filtro += " AND fechacobro >= '" + m_fechain->text() + "' ";

    if ( m_fechafin->text() != "" )
        filtro += " AND fechacobro <= '" + m_fechafin->text() + "' ";

    if ( mui_idbanco->idbanco() != "" )
        filtro += " AND idbanco = " + mui_idbanco->idbanco();

    _depura ( "END CobrosList::generaFiltro", 0 );
    return ( filtro );
}



/** SLOT que responde a la pulsacion del boton de crear en el listado.
    Instancia la clase CobroView, y la presenta.
*/
void CobrosList::crear()
{
    _depura ( "CobrosList::crear", 0 );
    CobroView *bud = new CobroView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    bud->setDbValue ( "idcliente", m_cliente->id() );
    bud->pintar();
    _depura ( "CobrosList::crear", 0 );
}


/** La impresion de listados esta completamente delegada a la clase BlSubForm
*/
void CobrosList::imprimir()
{
    _depura ( "CobrosList::imprimir", 0 );
    mui_list->imprimirPDF ( _ ( "Cobros a clientes" ) );
    _depura ( "END CobrosList::imprimir", 0 );
}


/** SLOT que responde a la pulsacion del boton borrar.
    Comprueba que exista un elemento seleccionado.
    Instancia la clase CobroView, la inicializa y le lanza el metodo borrar.
    Esta es la forma correcta de implementar un borrado a partir de un listado
    ya que de esta forma si existen plugins que alteren el borrado tambien seran invocados.
*/
void CobrosList::borrar()
{
    _depura ( "CobrosList::borrar", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        mensajeInfo ( _ ( "Debe seleccionar una linea" ) );
        return;
    } // end if
    try {
        mdb_idcobro = mui_list->dbValue ( "idcobro" );
        if ( modoEdicion() ) {
            CobroView * cv = new CobroView ( ( BfCompany * ) mainCompany(), 0 );
            if ( cv->cargar ( mdb_idcobro ) )
                throw - 1;
            cv->on_mui_borrar_clicked();
            cv->close();
        } // end if
        presentar();
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al borrar el cobro a cliente" ) );
    } // end try
    _depura ( "END:CobrosList::borrar", 0 );
}


/** SLOT que responde al doble click en el subformulario.
    Dependiendo del modo (Edicion o Seleccion) hace unas operaciones u otras.
*/
/// \TODO: Deberia crearse el metodo editar y este llamar a ese.
void CobrosList::editar ( int )
{
    _depura ( "CobrosList::on_mui_list_cellDoubleClicked", 0 );
    try {
        mdb_idcobro = mui_list->dbValue ( "idcobro" );
        if ( modoEdicion() ) {
            CobroView * bud = new CobroView ( ( BfCompany * ) mainCompany(), 0 );
            if ( bud->cargar ( mdb_idcobro ) ) {
                delete bud;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addWindow ( bud );
            bud->show();
        } else {
            emit ( selected ( mdb_idcobro ) );
        } // end if
    } catch ( ... ) {
        mensajeInfo ( _ ( "Debe seleccionar una fila primero" ) );
    } // end try
    _depura ( "END CobrosList::on_mui_list_cellDoubleClicked", 0 );

}

/** SLOT que responde a la peticion de menu contextual en el subformulario.
*/
/// \TODO: Revisar si este metodo es util.
void CobrosList::submenu ( const QPoint & )
{
    _depura ( "CobrosList::submenu", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 )
        return;
    QMenu *popup = new QMenu ( this );
    QAction *edit = popup->addAction ( _ ( "Editar cobro" ) );
    QAction *del = popup->addAction ( _ ( "Borrar cobro" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );
    if ( opcion == del )
        on_mui_borrar_clicked();
    if ( opcion == edit )
        on_mui_editar_clicked();
    delete popup;
    _depura ( "END CobrosList::submenu", 0 );
}

/** Inicializa la clase con el puntero a la company que se esta utilizando
**/
void CobrosList::setMainCompany ( BfCompany *comp )
{
    BlMainCompanyPointer::setMainCompany ( comp );
    m_cliente->setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    mui_idbanco->setMainCompany ( comp );
    mui_idbanco->setidbanco ( "" );
}

/** Devuelve el identificador del cobro seleccionado
**/
QString CobrosList::idcobro()
{
    _depura ( "CobrosList::idcobro", 0 );
    _depura ( "END CobrosList::idcobro", 0 );
    return mdb_idcobro;
}

/** Inicializa la la case con un cliente determiando. Lo establece en las opciones de filtrado
    no actualiza el listado
**/
void CobrosList::setidcliente ( QString val )
{
    _depura ( "CobrosList::setidcliente", 0 );
    m_cliente->setId ( val );
    _depura ( "END CobrosList::setidcliente", 0 );
}

/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para trabajar con la tabla cobro.
    Establece que no se puedan insertar elementos ya que es un listado.
*/
CobrosListSubForm::CobrosListSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "CobrosListSubForm::CobrosListSubForm", 0 );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "CobrosListSubForm_CobrosListSubForm", this );
    if ( res != 0 )
        return;
    setDbTableName ( "cobro" );
    setDbFieldId ( "idcobro" );
    addSubFormHeader ( "idcobro", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID cobro" ) );
    addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "ID cliente" ) );
    addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre" ) );
    addSubFormHeader ( "cifcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "C.I.F." ) );
    addSubFormHeader ( "telcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Numero de telefono" ) );
    addSubFormHeader ( "mailcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Direccion de correo electronico" ) );
    addSubFormHeader ( "fechacobro", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha de cobro" ) );
    addSubFormHeader ( "fechavenccobro", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha de vencimiento" ) );
    addSubFormHeader ( "cantcobro", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Cantidad" ) );
    addSubFormHeader ( "nombanco", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Banco" ) );
    addSubFormHeader ( "refcobro", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Referencia del cobro" ) );
    addSubFormHeader ( "previsioncobro", BlDbField::DbBoolean, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Prevision de cobro" ) );
    addSubFormHeader ( "comentcobro", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Comentarios" ) );
    addSubFormHeader ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "ID trabajador" ) );
    addSubFormHeader ( "nomtrabajador", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre del trabajador" ) );
    addSubFormHeader ( "apellidostrabajador", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Apellidos del trabajador" ) );
    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    _depura ( "END CobrosListSubForm::CobrosListSubForm", 0 );
}


CobrosListSubForm::~CobrosListSubForm()
{
    _depura ( "CobrosListSubForm::~CobrosListSubForm", 0 );
    _depura ( "END CobrosListSubForm::~CobrosListSubForm", 0 );
}

