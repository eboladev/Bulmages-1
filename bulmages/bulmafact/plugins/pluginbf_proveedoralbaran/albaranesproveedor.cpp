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

#include <QCheckBox>
#include <QCheckBox>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "albaranesproveedor.h"
#include "albaranproveedorview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "blfunctions.h"


/** Constructor de AlbaranesProveedor sin inicializar con la clase company.
    Util para casos en los que aun no se haya inicializado company.
    No sera completamente operativo hasta que se haya inicializado con
    setcompany.

    Inicializa todos los componentes, se pone en modo edicion y mete la ventana en el workSpace.
*/
/**
\param parent
\param flag
**/
AlbaranesProveedor::AlbaranesProveedor ( QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    blDebug ( "AlbaranesProveedor::AlbaranesProveedor", 0 );
    setupUi ( this );
    mdb_idalbaranp = "";
    setSubForm ( mui_list );
    /// Establecemos los parametros de busqueda del Cliente
    m_proveedor->setLabel ( _ ( "Proveedor:" ) );
    m_proveedor->setTableName ( "proveedor" );
    m_proveedor->m_valores["cifproveedor"] = "";
    m_proveedor->m_valores["nomproveedor"] = "";
    hideBusqueda();
    iniciaForm();
    /// Llamamos a los scripts
    blScript(this);
    blDebug ( "END AlbaranesProveedor::AlbaranesProveedor", 0 );
}


///
/**
\param comp
**/
void AlbaranesProveedor::setMainCompany ( BfCompany *comp )
{
    blDebug ( "AlbaranesProveedor::setMainCompany", 0 );
    BlMainCompanyPointer::setMainCompany ( comp );
    m_proveedor->setMainCompany ( comp );
    m_articulo->setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    /// Establecemos los parametros de busqueda del Cliente
    m_proveedor->setLabel ( _ ( "Proveedor:" ) );
    m_proveedor->setTableName ( "proveedor" );
    m_proveedor->m_valores["cifproveedor"] = "";
    m_proveedor->m_valores["nomproveedor"] = "";

    blDebug ( "END AlbaranesProveedor::setMainCompany", 0 );
}


///
/**
**/
void AlbaranesProveedor::crear()
{
    blDebug ( "AlbaranesProveedor::crear", 0 );
    if ( mainCompany() != NULL ) {
        AlbaranProveedorView *apv = new AlbaranProveedorView ( ( BfCompany * ) mainCompany(), 0 );
        mainCompany()->pWorkspace()->addSubWindow ( apv );
        apv->inicializar();
	apv->pintar();
	
	/// Si se crea el nuevo albaran desde la lista de albaranes de un proveedor,
	/// entonces se crea el albaran para este proveedor.
	if (m_proveedor->id() != "") {
	    apv->mui_idproveedor->setId( m_proveedor->id() );
	} // end if
	
        apv->show();
        apv->mui_descalbaranp->setFocus ( Qt::OtherFocusReason );
    }// end if
    blDebug ( "END AlbaranesProveedor::crear", 0 );
}


///
/**
\param val
**/
void AlbaranesProveedor::setidproveedor ( QString val )
{
    blDebug ( "AlbaranesProveedor::setidproveedor", 0 );
    m_proveedor->setId ( val );
    blDebug ( "END AlbaranesProveedor::setidproveedor", 0 );
}


///
/**
\param val
**/
void AlbaranesProveedor::setidarticulo ( QString val )
{
    blDebug ( "AlbaranesProveedor::setidarticulo", 0 );
    m_articulo->setidarticulo ( val );
    blDebug ( "AlbaranesProveedor::setidarticulo", 0 );
}


///
/**
\return
**/
QString AlbaranesProveedor::idalbaranp()
{
    blDebug ( "AlbaranesProveedor::idalbaranp", 0 );
    blDebug ( "END AlbaranesProveedor::idalbaranp", 0 );
    return mdb_idalbaranp;
}


/** Constructor completo de la clase con el puntero a BfCompany adecuado.
    Inicializa todos los componentes con company.
    Hace una presentacion inicial.
    Mete la ventana en el workSpace.
    Oculta la parte de Busqueda.
*/
/**
\param comp
\param parent
\param flag
**/
AlbaranesProveedor::AlbaranesProveedor ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    blDebug ( "AlbaranesProveedor::AlbaranesProveedor", 0 );
    setupUi ( this );
    setMainCompany ( comp );
    setSubForm ( mui_list );
    presentar();
    mdb_idalbaranp = "";
    if ( editMode() ) {
        mainCompany() ->insertWindow ( windowTitle(), this );
    } // end if
    hideBusqueda();
    iniciaForm();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "albaranp" );
    /// Llamamos a los scripts
    blScript(this);
    blDebug ( "END AlbaranesProveedor::AlbaranesProveedor", 0 );
}


///
/**
\return
**/
void AlbaranesProveedor::iniciaForm()
{
    blDebug ( "FacturasProveedorList::iniciaForm", 0 );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "AlbaranesProveedor_iniciaForm", this );
    if ( res != 0 ) {
        blDebug ( "END AlbaranesProveedor::iniciaForm", 0, "Salida por Plugin" );
        return;
    } // end if
    mui_procesada->insertItem ( 0, _ ( "Todos los albaranes" ) );
    mui_procesada->insertItem ( 1, _ ( "Albaranes procesados" ) );
    mui_procesada->insertItem ( 2, _ ( "Albaranes no procesados" ) );
    blDebug ( "END AlbaranesProveedor::iniciaForm", 0 );
}



/** Refresca la ventana de listados de albaranes.
*/
/**
**/
AlbaranesProveedor::~AlbaranesProveedor()
{
    blDebug ( "AlbaranesProveedor::~AlbaranesProveedor", 0 );
    blDebug ( "END AlbaranesProveedor::~AlbaranesProveedor", 0 );
}


/** Hace la carga del listado,
    Calcula el total de albaranes con las opciones de filtrado descritas y
    lo presenta.
*/
/**
\return
**/
void AlbaranesProveedor::presentar()
{
    blDebug ( "AlbaranesProveedor::presentar", 0 );
    if ( mainCompany() != NULL ) {
        mui_list->load ( "SELECT *, totalalbaranp AS total, " \
                           "bimpalbaranp AS base, impalbaranp AS impuestos " \
                           "FROM albaranp LEFT " \
                           "JOIN proveedor ON albaranp.idproveedor = " \
                           "proveedor.idproveedor LEFT JOIN almacen ON " \
                           "albaranp.idalmacen = almacen.idalmacen LEFT JOIN " \
                           "forma_pago ON albaranp.idforma_pago = " \
                           "forma_pago.idforma_pago WHERE 1 = 1 " + generaFiltro() );

        /// Hacemos el calculo del total.
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT SUM(totalalbaranp) " \
                             "AS total FROM albaranp LEFT JOIN proveedor ON " \
                             "albaranp.idproveedor = proveedor.idproveedor LEFT " \
                             "JOIN almacen ON albaranp.idalmacen = almacen.idalmacen " \
                             "WHERE 1 = 1 " + generaFiltro() );
        /// En caso de que el query haya fallado salimos.
        if ( !cur ) return;
        m_total->setText ( cur->value( "total" ) );
        delete cur;
    }
    blDebug ( "END AlbaranesProveedor::presentar", 0 );
}


/** Este es un metodo auxiliar para presenta() que se encarga de generar
    la clausula WHERE de la consulta.
*/
/**
\return
**/
QString AlbaranesProveedor::generaFiltro()
{
    blDebug ( "AlbaranesProveedor::generaFiltro", 0 );
    QString filtro = "";

    if ( m_filtro->text() != "" ) {
        filtro = " AND ( lower(descalbaranp) LIKE lower('%" + m_filtro->text() + "%') ";
        filtro += " OR refalbaranp LIKE '" + m_filtro->text() + "%' ";
        filtro += " OR lower(nomproveedor) LIKE lower('%" + m_filtro->text() + "%')) ";
    }

    if ( m_proveedor->id() != "" )
        filtro += " AND albaranp.idproveedor = " + m_proveedor->id();

    /// Tratamos los elementos procesados y no procesados.
    if ( mui_procesada->currentIndex() == 1 ) {
        /// Muestra solo las procesadas.
        filtro += " AND procesadoalbaranp";
    } else if ( mui_procesada->currentIndex() == 2 ) {
        /// Muestra solo las NO procesadas.
        filtro += " AND NOT procesadoalbaranp ";
    } // end if

    if ( m_articulo->idarticulo() != "" )
        filtro += " AND idalbaranp IN (SELECT DISTINCT idalbaranp FROM lalbaranp " \
                  "WHERE idarticulo = '" + m_articulo->idarticulo() + "')";
    if ( m_fechain->text() != "" )
        filtro += " AND fechaalbaranp >= '" + m_fechain->text() + "' ";
    if ( m_fechafin->text() != "" )
        filtro += " AND fechaalbaranp <= '" + m_fechafin->text() + "' ";
    blDebug ( "END AlbaranesProveedor::generaFiltro", 0 );
    return ( filtro );
}


/** Realiza la operacion de edicion.
    Si el modo seleccionado es edicion abre la ficha de albaran proveedor y carga en ella el elemento seleccionado.
    Si el modo es seleccion lanza el signal adecuado.
*/
/**
\param row
\return
**/
void AlbaranesProveedor::editar ( int row )
{
    blDebug ( "AlbaranesProveedor::editar", 0 );
    mdb_idalbaranp = mui_list->dbValue ( QString ( "idalbaranp" ), row );
    if ( editMode() ) {
        AlbaranProveedorView * prov = new AlbaranProveedorView ( ( BfCompany * ) mainCompany(), 0 );
        if ( prov->load ( mdb_idalbaranp ) ) {
            delete prov;
            return;
        } // end if
        mainCompany() ->m_pWorkspace->addSubWindow ( prov );
        prov->show();
    } else {
        emit ( selected ( mdb_idalbaranp ) );
    } // end if
    blDebug ( "END AlbaranesProveedor::editar", 0 );
}



/** La impresion de listados esta completamente delegada a la clase BlSubForm
*/
/**
**/
void AlbaranesProveedor::imprimir()
{
    blDebug ( "AlbaranesProveedor::imprimir", 0 );
    mui_list->printPDF ( _ ( "Albaranes de proveedor" ) );
    blDebug ( "END AlbaranesProveedor::imprimir", 0 );
}


/** Al pulsar sobre el boton borrar se carga la ficha y se invoca al metodo de
    borrar en esta.
    Tambien se tratan las posibles excepciones que se hayan podido
    producir.
    Tras el borrado repinta la pantalla.
*/
/**
\return
**/
void AlbaranesProveedor::remove()
{
    blDebug ( "AlbaranesProveedor::borrar", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Debe seleccionar una linea" ), this );
        return;
    } // end if
    try {
        mdb_idalbaranp = mui_list->dbValue ( QString ( "idalbaranp" ) );
        if ( editMode() ) {
            AlbaranProveedorView * apv = new AlbaranProveedorView ( ( BfCompany * ) mainCompany(), 0 );
            if ( apv->load ( mdb_idalbaranp ) ) {
                throw - 1;
            } // end if
            apv->on_mui_borrar_clicked();
            apv->close();
        } // end if
        presentar();
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al borrar albaran de proveedor" ), this );
    } // end try
    blDebug ( "END AlbaranesProveedor::borrar", 0 );
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Configuracion del subformulario de listado.
    Prepara el subformulario para trabajar con la tabla albaranp.
*/


///
/**
**/
AlbaranesProveedorListSubform::~AlbaranesProveedorListSubform()
{
    blDebug ( "AlbaranesProveedorListSubform::~AlbaranesProveedorListSubform", 0 );
    blDebug ( "END AlbaranesProveedorListSubform::~AlbaranesProveedorListSubform", 0 );
}


///
/**
\param parent
**/
AlbaranesProveedorListSubform::AlbaranesProveedorListSubform ( QWidget *parent ) : BfSubForm ( parent )
{
    blDebug ( "AlbaranesProveedorListSubform::AlbaranesProveedorListSubform", 0 );
    setDbTableName ( "albaranp" );
    setDbFieldId ( "idalbaranp" );
    addSubFormHeader ( "idalbaranp", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID albaran" ) );
    addSubFormHeader ( "numalbaranp", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Numero de albaran" ) );
    addSubFormHeader ( "descalbaranp", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Descripcion" ) );
    addSubFormHeader ( "refalbaranp", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Referencia" ) );
    addSubFormHeader ( "fechaalbaranp", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha" ) );
    addSubFormHeader ( "comentalbaranp", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Comentario" ) );
    addSubFormHeader ( "procesadoalbaranp", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Procesado" ) );
    addSubFormHeader ( "idproveedor", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "ID proveedor" ) );
    addSubFormHeader ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "ID forma de pago" ) );
    addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "ID almacen" ) );
    addSubFormHeader ( "nomproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre del proveedor" ) );
    addSubFormHeader ( "nomalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre del almacen" ) );
    addSubFormHeader ( "descforma_pago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Descripcion de la forma de pago" ) );
    addSubFormHeader ( "base", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Base imponible" ) );
    addSubFormHeader ( "impuestos", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Impuestos" ) );
    addSubFormHeader ( "total", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Total albaran" ) );
    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    blDebug ( "END AlbaranesProveedorListSubform::AlbaranesProveedorListSubform", 0 );
}


///
/**
**/
void AlbaranesProveedorListSubform::load()
{
    blDebug ( "AlbaranesProveedorListSubform::cargar", 0 );
    QString SQLQuery = "SELECT * FROM albaranp";
    BlSubForm::load ( SQLQuery );
    blDebug ( "END AlbaranesProveedorListSubform::cargar", 0 );
}


///
/**
\param query
**/
void AlbaranesProveedorListSubform::load ( QString query )
{
    blDebug ( "AlbaranesProveedorListSubform::cargar", 0 );
    BlSubForm::load ( query );
    blDebug ( "END AlbaranesProveedorListSubform::cargar", 0 );
}

