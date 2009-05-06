/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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
#include "reciboslist.h"
#include "bfcompany.h"
#include "reciboview.h"
#include "blfunctions.h"

/** Inicializa todos los componentes.
    Mete la ventana en el workSpace.
    Este constructor no es completo, debe inicializarse con setcompany para que la clase pueda operar.
*/
RecibosList::RecibosList ( QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    _depura ( "RecibosList::RecibosList", 0 );
    
    setupUi ( this );
    
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "RecibosList_RecibosList", this );
    if ( res != 0 )
        return;
        
    mdb_idrecibo = "";
    setSubForm ( mui_list );
    hideBusqueda();
    iniciaForm();

    _depura ( "END RecibosList::RecibosList", 0 );
}

/** Inicializa todos los componentes.
    Hace una presentacion inicial.
    Mete la ventana en el workSpace.
*/
RecibosList::RecibosList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    _depura ( "RecibosList::RecibosList", 0 );
    
    setupUi ( this );
    
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "RecibosList_RecibosList", this );
    if ( res != 0 )
        return;

    mui_list->setMainCompany ( comp );

    setSubForm ( mui_list );
    presentar();
    mdb_idrecibo = "";
    
    if ( modoEdicion() )
        mainCompany()->meteWindow ( windowTitle(), this );
        
    hideBusqueda();
    iniciaForm();
    
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "recibo" );
    
    _depura ( "END RecibosList::RecibosList", 0 );
}

/** No requiere acciones especiales en el destructor.
*/
RecibosList::~RecibosList()
{
    _depura ( "RecibosList::~RecibosList", 0 );
    _depura ( "END RecibosList::~RecibosList", 0 );
}

void RecibosList::iniciaForm()
{
    _depura ( "RecibosList::iniciaForm" );
    
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "RecibosList_iniciaForm", this );
    if ( res != 0 )
        return;
        
    mui_procesada->insertItem ( 0, _ ( "Todos los recibos" ) );
    mui_procesada->insertItem ( 1, _ ( "Recibos pagados" ) );
    mui_procesada->insertItem ( 2, _ ( "Recibos no pagados" ) );
    mui_procesada->setCurrentIndex ( 0 );
    
    _depura ( "END RecibosList::iniciaForm" );
}

/** Hace la carga del listado.
    Para ello genera los SELECTS con ayuda de generaFiltro y los pasa al SubFormulario para que los presente.
    Tambien hace un select de calculo de totales y lo presenta en el textEdit correspondiente.
*/
void RecibosList::presentar()
{
    _depura ( "RecibosList::presentar", 0 );
    
    if ( mainCompany() != NULL ) {
        mui_list->cargar ( "SELECT * FROM recibo LEFT JOIN forma_pago ON recibo.idforma_pago = forma_pago.idforma_pago LEFT JOIN cliente ON recibo.idcliente = cliente.idcliente WHERE 1 = 1 " + generaFiltro() );
    } // end if
    
    _depura ( "END RecibosList::presentar", 0 );
}


/** Metodo auxiliar que genera la clausula WHERE del listado con las opciones de filtrado especificadas.
*/
QString RecibosList::generaFiltro()
{
    _depura ( "RecibosList::generaFiltro", 0 );
    
    QString filtro = "";
    
    if ( m_filtro->text() != "" ) {
        filtro += " AND ( lower(cliente.nomcliente) LIKE lower('%" + m_filtro->text() + "%') OR lower(recibo.descrecibo) LIKE lower('%" + m_filtro->text() + "%') )";
    } // end if
    
    /// Tratamos los elementos procesados y no procesados.
    if ( mui_procesada->currentIndex() == 1 ) {
        /// Muestra solo las procesadas.
        filtro += " AND pagadorecibo";
    } else if ( mui_procesada->currentIndex() == 2 ) {
        /// Muestra solo las NO procesadas.
        filtro += " AND NOT pagadorecibo ";
    } // end if

    if ( m_fechain->text() != "" )
        filtro += " AND fecharecibo >= '" + m_fechain->text() + "' ";

    if ( m_fechafin->text() != "" )
        filtro += " AND fecharecibo <= '" + m_fechafin->text() + "' ";

    _depura ( "END RecibosList::generaFiltro", 0 );
    
    return ( filtro );
}

/** SLOT que responde a la pulsacion del boton de crear en el listado.
    Instancia la clase ReciboView, y la presenta.
*/
void RecibosList::crear()
{
    _depura ( "RecibosList::crear", 0 );
    
    ReciboView *bud = new ReciboView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    bud->pintar();
    
    _depura ( "END RecibosList::crear", 0 );
}

/** La impresion de listados esta completamente delegada a la clase SubForm3
*/
void RecibosList::imprimir()
{
    _depura ( "RecibosList::imprimir", 0 );
    
    mui_list->imprimirPDF ( _ ( "Recibos" ) );
    
    _depura ( "END RecibosList::imprimir", 0 );
}

/** SLOT que responde a la pulsacion del boton borrar.
    Comprueba que exista un elemento seleccionado.
    Instancia la clase ReciboView, la inicializa y le lanza el metodo borrar.
    Esta es la forma correcta de implementar un borrado a partir de un listado
    ya que de esta forma si existen plugins que alteren el borrado tambien seran invocados.
*/
void RecibosList::borrar()
{
    _depura ( "RecibosList::borrar", 0 );
    
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        mensajeInfo ( _ ( "Debe seleccionar una linea" ) );
        return;
    } // end if
    
    try {
        mdb_idrecibo = mui_list->dbValue ( "idrecibo" );
        if ( modoEdicion() ) {
            ReciboView * cv = new ReciboView ( ( BfCompany * ) mainCompany(), 0 );
            if ( cv->cargar ( mdb_idrecibo ) )
                throw - 1;
            cv->on_mui_borrar_clicked();
            cv->close();
        } // end if
        presentar();
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al borrar el cobro a cliente" ) );
    } // end try
    
    _depura ( "END:RecibosList::borrar", 0 );
}

/** SLOT que responde al doble click en el subformulario.
    Dependiendo del modo (Edicion o Seleccion) hace unas operaciones u otras.
*/
/// \TODO: Deberia crearse el metodo editar y este llamar a ese.
void RecibosList::editar ( int )
{
    _depura ( "RecibosList::editar", 0 );
    
    try {
        mdb_idrecibo = mui_list->dbValue ( "idrecibo" );
        if ( modoEdicion() ) {
            ReciboView * bud = new ReciboView ( ( BfCompany * ) mainCompany(), 0 );
            if ( bud->cargar ( mdb_idrecibo ) ) {
                delete bud;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addWindow ( bud );
            bud->show();
        } else {
            emit ( selected ( mdb_idrecibo ) );
        } // end if
    } catch ( ... ) {
        mensajeInfo ( _ ( "Debe seleccionar una fila primero" ) );
    } // end try
    
    _depura ( "END RecibosList::editar", 0 );
}

/** SLOT que responde a la peticion de menu contextual en el subformulario.
*/
/// \TODO: Revisar si este metodo es util.
void RecibosList::submenu ( const QPoint & )
{
    _depura ( "RecibosList::submenu", 0 );
    
    int a = mui_list->currentRow();
    if ( a < 0 )
        return;
        
    QMenu *popup = new QMenu ( this );
    QAction *edit = popup->addAction ( _ ( "Editar recibo" ) );
    QAction *del = popup->addAction ( _ ( "Borrar recibo" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );
    
    if ( opcion == del )
        on_mui_borrar_clicked();
        
    if ( opcion == edit )
        on_mui_editar_clicked();
        
    delete popup;
    
    _depura ( "END RecibosList::submenu", 0 );
}

/** Inicializa la clase con el puntero a la company que se esta utilizando
**/
void RecibosList::setMainCompany ( BfCompany *comp )
{
    _depura ( "RecibosList::setMainCompany", 0 );
    
    BlMainCompanyPointer::setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    
    _depura ( "END RecibosList::setMainCompany", 0 );
}

/** Devuelve el identificador del cobro seleccionado
**/
QString RecibosList::idrecibo()
{
    _depura ( "RecibosList::idrecibo", 0 );
    _depura ( "END RecibosList::idrecibo", 0 );
    
    return mdb_idrecibo;
}

/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para trabajar con la tabla cobro.
    Establece que no se puedan insertar elementos ya que es un listado.
*/
RecibosListSubForm::RecibosListSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "RecibosListSubForm::RecibosListSubForm", 0 );
    
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "RecibosListSubForm_RecibosListSubForm", this );
    if ( res != 0 )
        return;
        
    setDbTableName ( "recibo" );
    setDbFieldId ( "idrecibo" );
    addSubFormHeader ( "idrecibo", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Recibo" ) );
    addSubFormHeader ( "cantrecibo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Cantidad" ) );
    addSubFormHeader ( "fecharecibo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha" ) );
    addSubFormHeader ( "idcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "ID Tutor" ) );
    addSubFormHeader ( "descforma_pago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Forma de Pago" ) );
    addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Tutor" ) );

    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    
    _depura ( "END RecibosListSubForm::RecibosListSubForm", 0 );
}

RecibosListSubForm::~RecibosListSubForm()
{
    _depura ( "RecibosListSubForm::~RecibosListSubForm", 0 );
    _depura ( "END RecibosListSubForm::~RecibosListSubForm", 0 );
}
