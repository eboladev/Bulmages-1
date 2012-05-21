/***************************************************************************
 *   Copyright (C) 2009 by Xavier Rusiñol Escriu                           *
 *   russi7@gmail.com                                                      *
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
#include <stdio.h>

#include "blconfiguration.h"
#include "sociolist.h"
#include "bfcompany.h"
#include "socioview.h"
#include "blfunctions.h"


/** Inicializa todos los componentes.
    Mete la ventana en el workSpace.
    Este constructor no es completo, debe inicializarse con setcompany para que la clase pueda operar.
*/
SociosList::SociosList ( QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->run ( "SociosList_SociosList", this );
    if ( res != 0 )
        return;
    mdb_idcliente = "";
    setSubForm ( mui_list );
    hideBusqueda();
    /// Llamamos a los scripts
    blScript(this);
    
}


/** Inicializa todos los componentes.
    Hace una presentacion inicial.
    Mete la ventana en el workSpace.
*/
SociosList::SociosList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->run ( "SociosList_SociosList", this );
    if ( res != 0 )
        return;

    mui_list->setMainCompany ( comp );

    setSubForm ( mui_list );
    
    mdb_idcliente = "";
    if ( editMode() )
        mainCompany() ->insertWindow ( windowTitle(), this );
    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "cliente" );
    
    /// Cargamos los filtros guardados.
    cargaFiltrosXML();
    
    presentar();
    
    /// Llamamos a los scripts
    blScript(this);
}


/** No requiere acciones especiales en el destructor.
*/
SociosList::~SociosList()
{
    BL_FUNC_DEBUG
}


/** Hace la carga del listado.
    PAra ello genera los SELECTS con ayuda de generaFiltro y los pasa al SubFormulario para que los presente.
    Por defecto salen los clientes
*/
void SociosList::presentar()
{
    BL_FUNC_DEBUG
    if ( mainCompany() != NULL ) {
        mui_list->load ( "SELECT * FROM cliente WHERE sociocliente = false " + generaFiltro() );
    } // end if
}

/** Seleccionar si queremos ver clientes o socios
*/
void SociosList::on_mui_sociocliente_activated( int index )
{
    BL_FUNC_DEBUG

    if(index){
      mui_list->load ( "SELECT * FROM cliente WHERE sociocliente = true " + generaFiltro() );
    }
    else{
      mui_list->load ( "SELECT * FROM cliente WHERE sociocliente = false " + generaFiltro() );
    }
}


/** Metodo auxiliar que genera la clausula WHERE del listado con las opciones de filtrado especificadas.
*/
const QString SociosList::generaFiltro()
{
    BL_FUNC_DEBUG
    QString filtro = "";

    /// Hacemos el filtrado like del campo m_filtro
    filtro += mui_list->likeFilterSQL(m_filtro->text());
    
    return ( filtro );
}



/** SLOT que responde a la pulsacion del boton de crear en el listado.
    Instancia la clase SocioView, y la presenta.
*/
void SociosList::crear()
{
    BL_FUNC_DEBUG
    SocioView *bud = new SocioView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    bud->pintar();
    
}


/** La impresion de listados esta completamente delegada a la clase BlSubForm
*/
void SociosList::imprimir()
{
    BL_FUNC_DEBUG
    printPDF ( _ ( "Padres" ) );
    
}


/** SLOT que responde a la pulsacion del boton borrar.
    Comprueba que exista un elemento seleccionado.
    Instancia la clase SocioView, la inicializa y le lanza el metodo borrar.
    Esta es la forma correcta de implementar un borrado a partir de un listado
    ya que de esta forma si existen plugins que alteren el borrado tambien seran invocados.
*/
void SociosList::remove()
{
    BL_FUNC_DEBUG
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Debe seleccionar una linea" ) );
        return;
    } // end if
    try {
        mdb_idcliente = mui_list->dbValue ( "idcliente" );
        if ( editMode() ) {
            SocioView * cv = new SocioView ( ( BfCompany * ) mainCompany(), 0 );
            if ( cv->load ( mdb_idcliente ) )
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
void SociosList::editar ( int )
{
    BL_FUNC_DEBUG
    try {
        mdb_idcliente = mui_list->dbValue ( "idcliente" );
        if ( editMode() ) {
            SocioView * bud = new SocioView ( ( BfCompany * ) mainCompany(), 0 );
            if ( bud->load ( mdb_idcliente ) ) {
                delete bud;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addSubWindow ( bud );
            bud->show();
        } else {
	    close();
            emit ( selected ( mdb_idcliente ) );
        } // end if
    } catch ( ... ) {
        blMsgInfo ( _ ( "Debe seleccionar una fila primero" ) );
    } // end try
}

/** SLOT que responde a la peticion de menu contextual en el subformulario.
*/
/// \TODO: Revisar si este metodo es util.
void SociosList::submenu ( const QPoint & )
{
    BL_FUNC_DEBUG
    int a = mui_list->currentRow();
    if ( a < 0 )
        return;
    QMenu *popup = new QMenu ( this );
    QAction *edit = popup->addAction ( _ ( "Editar Tutor" ) );
    QAction *del = popup->addAction ( _ ( "Borrar Tutor" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );
    if ( opcion == del )
        on_mui_borrar_clicked();
    if ( opcion == edit )
        on_mui_editar_clicked();
    delete popup;
    
}

/** Inicializa la clase con el puntero a la company que se esta utilizando
**/
void SociosList::setMainCompany ( BfCompany *comp )
{
    BlMainCompanyPointer::setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
}

/** Devuelve el identificador del cobro seleccionado
**/
QString SociosList::idcliente()
{
    BL_FUNC_DEBUG
    
    return mdb_idcliente;
}



/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para trabajar con la tabla cobro.
    Establece que no se puedan insertar elementos ya que es un listado.
*/
SociosListSubForm::SociosListSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    /// Disparamos los plugins.
    int res = g_plugins->run ( "SociosListSubForm_SociosListSubForm", this );
    if ( res != 0 )
        return;
    setDbTableName ( "cliente" );
    setDbFieldId ( "idcliente" );
    setFileConfig("tutor");
    addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID padre" ) );
    addSubFormHeader ( "selector", BlDbField::DbBoolean, BlDbField::DbNoSave | BlDbField::DbNoLoad, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, "selector" );
    addSubFormHeader ( "numsociocliente", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Numero Socio" ) );
    addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre" ) );
    addSubFormHeader ( "codcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Codigo" ) );
    addSubFormHeader ( "cifcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "C.I.F." ) );
    addSubFormHeader ( "nomaltcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre alternativo del cliente" ) );
    addSubFormHeader ( "bancocliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Numero cuenta corriente" ) );
    addSubFormHeader ( "dircliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Direccion" ) );
    addSubFormHeader ( "poblcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Poblacion" ) );
    addSubFormHeader ( "cpcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Codigo postal" ) );
    addSubFormHeader ( "telcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Numero de telefono" ) );
    addSubFormHeader ( "faxcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Numero de fax" ) );
    addSubFormHeader ( "mailcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Direccion de correo electronica" ) );
    addSubFormHeader ( "urlcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Direccion URL" ) );
    addSubFormHeader ( "corpcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Empresa" ) );
    addSubFormHeader ( "faltacliente", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha de alta del cliente" ) );
    addSubFormHeader ( "fbajacliente", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha de baja del cliente" ) );
    addSubFormHeader ( "comentcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Comentarios" ) );

    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    
}


SociosListSubForm::~SociosListSubForm()
{
    BL_FUNC_DEBUG
    
}


