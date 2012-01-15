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
    BL_FUNC_DEBUG
    
    setupUi ( this );
    
    /// Disparamos los plugins.
    int res = g_plugins->run ( "RecibosList_RecibosList", this );
    if ( res != 0 )
        return;
        
    mdb_idrecibo = "";
    setSubForm ( mui_list );
    hideBusqueda();
    iniciaForm();
    /// Llamamos a los scripts
    blScript(this);
    
}

/** Inicializa todos los componentes.
    Hace una presentacion inicial.
    Mete la ventana en el workSpace.
*/
RecibosList::RecibosList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    BL_FUNC_DEBUG
    
    setupUi ( this );
    
    /// Disparamos los plugins.
    int res = g_plugins->run ( "RecibosList_RecibosList", this );
    if ( res != 0 )
        return;

    mui_list->setMainCompany ( comp );

    /// Establecemos los parametros de busqueda de Profesor
    mui_idactividad->setMainCompany ( comp );
    mui_idactividad->setLabel ( _ ( "Actividad:" ) );
    mui_idactividad->setTableName ( "actividad" );
    mui_idactividad->m_valores["nombreactividad"] = "";


    setSubForm ( mui_list );
    presentar();
    mdb_idrecibo = "";
    
    if ( editMode() )
        mainCompany()->insertWindow ( windowTitle(), this );
        
    hideBusqueda();
    iniciaForm();
    
    mui_idbanco->setMainCompany ( comp );
    mui_idbanco->setidbanco ( "" );
    
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "recibo" );
    
    /// Cargamos los filtros guardados.
    cargaFiltrosXML();
    
    presentar();
    /// Llamamos a los scripts
    blScript(this);
    
}


/** No requiere acciones especiales en el destructor.
*/
RecibosList::~RecibosList()
{
    BL_FUNC_DEBUG
    
}


void RecibosList::iniciaForm()
{
    BL_FUNC_DEBUG
    
    /// Disparamos los plugins.
    int res = g_plugins->run ( "RecibosList_iniciaForm", this );
    if ( res != 0 )
        return;
        
    mui_procesada->insertItem ( 0, _ ( "Todos los recibos" ) );
    mui_procesada->insertItem ( 1, _ ( "Recibos pagados" ) );
    mui_procesada->insertItem ( 2, _ ( "Recibos no pagados" ) );
    mui_procesada->setCurrentIndex ( 0 );
    
    
}


/** Hace la carga del listado.
    Para ello genera los SELECTS con ayuda de generaFiltro y los pasa al SubFormulario para que los presente.
    Tambien hace un select de calculo de totales y lo presenta en el textEdit correspondiente.
*/
void RecibosList::presentar()
{
    BL_FUNC_DEBUG
    
    if ( mainCompany() != NULL ) {
	QString query = "SELECT * FROM recibo LEFT JOIN forma_pago ON recibo.idforma_pago = forma_pago.idforma_pago LEFT JOIN cliente ON recibo.idcliente = cliente.idcliente LEFT JOIN banco ON recibo.idbanco = banco.idbanco WHERE 1 = 1 " + generaFiltro() ;
        mui_list->load ( query );
    } // end if
    
    
}


/** Metodo auxiliar que genera la clausula WHERE del listado con las opciones de filtrado especificadas.
*/
const QString RecibosList::generaFiltro()
{
    BL_FUNC_DEBUG
    
    QString filtro = "";
    
    /// Hacemos el filtrado like del campo m_filtro
    filtro += mui_list->likeFilterSQL(m_filtro->text());
    
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
            
    if ( mui_idbanco->idbanco() != "" )
        filtro += " AND banco.idbanco = " + mui_idbanco->idbanco();

    if (mui_idactividad->id() != "") {
	QString nombreactividad = mui_idactividad->fieldValue("nombreactividad");
	filtro += " AND idrecibo IN (SELECT idrecibo FROM lrecibo WHERE conceptolrecibo LIKE '%" + nombreactividad + "%')";
    } // end if
    
    
    return ( filtro );
}

/** SLOT que responde a la pulsacion del boton de crear en el listado.
    Instancia la clase ReciboView, y la presenta.
*/
void RecibosList::crear()
{
    BL_FUNC_DEBUG
    
    ReciboView *bud = new ReciboView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    bud->pintar();
    
    
}

/** La impresion de listados esta completamente delegada a la clase SubForm3
*/
void RecibosList::imprimir()
{
    BL_FUNC_DEBUG
    
    mui_list->printPDF ( _ ( "Recibos" ) );
    
    
}

/** SLOT que responde a la pulsacion del boton borrar.
    Comprueba que exista un elemento seleccionado.
    Instancia la clase ReciboView, la inicializa y le lanza el metodo borrar.
    Esta es la forma correcta de implementar un borrado a partir de un listado
    ya que de esta forma si existen plugins que alteren el borrado tambien seran invocados.
*/
void RecibosList::remove()
{
    BL_FUNC_DEBUG
    
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Debe seleccionar una linea" ) );
        return;
    } // end if
    
    try {
        mdb_idrecibo = mui_list->dbValue ( "idrecibo" );
        if ( editMode() ) {
            ReciboView * cv = new ReciboView ( ( BfCompany * ) mainCompany(), 0 );
            if ( cv->load ( mdb_idrecibo ) )
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
void RecibosList::editar ( int )
{
    BL_FUNC_DEBUG
    
    try {
        mdb_idrecibo = mui_list->dbValue ( "idrecibo" );
        if ( editMode() ) {
            ReciboView * bud = new ReciboView ( ( BfCompany * ) mainCompany(), 0 );
            if ( bud->load ( mdb_idrecibo ) ) {
                delete bud;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addSubWindow ( bud );
            bud->show();
        } else {
            emit ( selected ( mdb_idrecibo ) );
        } // end if
    } catch ( ... ) {
        blMsgInfo ( _ ( "Debe seleccionar una fila primero" ) );
    } // end try
    
    
}


/** SLOT que responde a la peticion de menu contextual en el subformulario.
*/
/// \TODO: Revisar si este metodo es util.
void RecibosList::submenu ( const QPoint & )
{
    BL_FUNC_DEBUG
    
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
    
    
}

/** Inicializa la clase con el puntero a la company que se esta utilizando
**/
void RecibosList::setMainCompany ( BfCompany *comp )
{
    BL_FUNC_DEBUG
    
    BlMainCompanyPointer::setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    
    
}

/** Devuelve el identificador del cobro seleccionado
**/
QString RecibosList::idrecibo()
{
    BL_FUNC_DEBUG
    
    
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
    BL_FUNC_DEBUG
    
    /// Disparamos los plugins.
    int res = g_plugins->run ( "RecibosListSubForm_RecibosListSubForm", this );
    if ( res != 0 )
        return;
        
    setDbTableName ( "recibo" );
    setDbFieldId ( "idrecibo" );
    addSubFormHeader ( "idrecibo", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Recibo" ) );
    addSubFormHeader ( "cantrecibo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Cantidad" ) );
    addSubFormHeader ( "pagadorecibo", BlDbField::DbBoolean, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Pagado" ) );
    addSubFormHeader ( "devueltorecibo", BlDbField::DbBoolean, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Devuelto" ) );
    addSubFormHeader ( "fecharecibo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha" ) );
    addSubFormHeader ( "idcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "ID Tutor" ) );
    addSubFormHeader ( "descforma_pago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Forma de Pago" ) );
    addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Tutor" ) );

    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    
    
}


RecibosListSubForm::~RecibosListSubForm()
{
    BL_FUNC_DEBUG
    
}

