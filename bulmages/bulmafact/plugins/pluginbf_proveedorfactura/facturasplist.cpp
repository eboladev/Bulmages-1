/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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
#include <QFile>
#include <QTextStream>
#include <QString>

#include "facturasplist.h"
#include "bfcompany.h"
#include "bfbuscararticulo.h"
#include "blconfiguration.h"
#include "facturapview.h"
#include "blplugins.h"


/** Inicializa todos los componentes del listado a NULL para que no exista consusion sobre
    si un elemento ha sido inicializado o no.
    Al construir la clase con este constructor debe ser inicializada con setcompany().
    Mete la ventana en el workSpace. */
/**
\param parent
\param flag
\return
**/
FacturasProveedorList::FacturasProveedorList ( QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->run ( "FacturasProveedorList_FacturasProveedorList", this );
    if ( res != 0 ) {
        return;
    } // end if
    mdb_idfacturap = "";
    setSubForm ( mui_list );
    /// Establecemos los parametros de busqueda del Cliente
    m_proveedor->setLabel ( _ ( "Proveedor:" ) );
    m_proveedor->setTableName ( "proveedor" );
    m_proveedor->m_valores["cifproveedor"] = "";
    m_proveedor->m_valores["nomproveedor"] = "";
    hideBusqueda();
    iniciaForm();
    g_plugins->run ( "FacturasProveedorList_FacturasProveedorList_Post", this );
    /// Llamamos a los scripts
    blScript(this);
    
}


/** Inicializa todos los componentes, hace la carga inicial con \ref presenta()
    mete la ventana en el workSpace(). */
/**
\param comp
\param parent
\return
**/
FacturasProveedorList::FacturasProveedorList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->run ( "FacturasProveedorList_FacturasProveedorList", this );
    if ( res != 0 ) {
        return;
    } // end if
    m_proveedor->setMainCompany ( mainCompany() );
    m_articulo->setMainCompany ( mainCompany() );
    mui_list->setMainCompany ( comp );
    

    setSubForm ( mui_list );
    mdb_idfacturap = "";
    /// Establecemos los parametros de busqueda del Cliente
    m_proveedor->setLabel ( _ ( "Proveedor:" ) );
    m_proveedor->setTableName ( "proveedor" );
    m_proveedor->m_valores["cifproveedor"] = "";
    m_proveedor->m_valores["nomproveedor"] = "";
    if ( editMode() ) {
        mainCompany() ->insertWindow ( windowTitle(), this );
    } // end if
    hideBusqueda();
    iniciaForm();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "facturap" );

    /// Cargamos los filtros guardados.
    cargaFiltrosXML();
    
    presentar();
    
    /// Disparamos los plugins.
    g_plugins->run ( "FacturasProveedorList_FacturasProveedorList_Post", this );
    /// Llamamos a los scripts
    blScript(this);
    
}


///
/**
\return
**/
void FacturasProveedorList::iniciaForm()
{
    BL_FUNC_DEBUG
    /// Disparamos los plugins.
    int res = g_plugins->run ( "FacturasProveedorList_iniciaForm", this );
    if ( res != 0 )
        return;
    mui_procesada->insertItem ( 0, _ ( "Todas las facturas" ) );
    mui_procesada->insertItem ( 1, _ ( "Facturas procesadas" ) );
    mui_procesada->insertItem ( 2, _ ( "Facturas no procesadas" ) );
    
}


/** No precisa de acciones especiales en el destructor. */
/**
**/
FacturasProveedorList::~FacturasProveedorList()
{
    BL_FUNC_DEBUG
    
}


/** Hace la carga inicial del listado.
    Tambien hace el calculo de totales y lo presenta. */
/**
\return
**/
void FacturasProveedorList::presentar()
{
    BL_FUNC_DEBUG
    mui_list->load ( "SELECT *, totalfacturap AS total, bimpfacturap AS base, impfacturap AS impuestos  FROM facturap LEFT JOIN proveedor ON facturap.idproveedor=proveedor.idproveedor WHERE 1=1  " + generaFiltro() );

    /// Hacemos el calculo del total.
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT SUM(totalfacturap) AS total FROM facturap LEFT JOIN proveedor ON facturap.idproveedor=proveedor.idproveedor WHERE 1=1  " + generaFiltro() );
    /// Si ha habido un error con el query salimos
    if ( !cur ) return;
    m_total->setText ( cur->value( "total" ) );
    delete cur;
    
}


/** Metodo auxiliar que crea la clausula WHERE del query de carga  \ref presenta()
    La clausula WHERE utiliza todas las opciones de filtrado para crearse. */
/**
\return
**/
const QString FacturasProveedorList::generaFiltro()
{
    BL_FUNC_DEBUG
    /// Tratamiento de los filtros.
    QString filtro = "";

    /// Hacemos el filtrado like del campo m_filtro
    filtro += mui_list->likeFilterSQL(m_filtro->text());
    
    if ( m_proveedor->id() != "" ) {
        filtro += " AND facturap.idproveedor = " + m_proveedor->id();
    } // end if

    /// Tratamos los elementos procesados y no procesados.
    if ( mui_procesada->currentIndex() == 1 ) {
        /// Muestra solo las procesadas.
        filtro += " AND procesadafacturap";
    } else if ( mui_procesada->currentIndex() == 2 ) {
        /// Muestra solo las NO procesadas.
        filtro += " AND NOT procesadafacturap ";
    } // end if

    if ( m_articulo->idarticulo() != "" ) {
        filtro += " AND idfacturap IN (SELECT DISTINCT idfacturap FROM lfacturap WHERE idarticulo = '" + m_articulo->idarticulo() + "')";
    } // end if

    if ( m_fechain->text() != "" )
        filtro += " AND ffacturap >= '" + m_fechain->text() + "' ";

    if ( m_fechafin->text() != "" )
        filtro += " AND ffacturap <= '" + m_fechafin->text() + "' ";

    
    return ( filtro );
}


/// Este metodo responmde a la accion predeterminada sobre el listado
/// Normalmente consiste en abrir la ficha del elemento seleccionado,
/// en este caso, una factura de proveedor
/**
\return
**/
void FacturasProveedorList::editar ( int row )
{
    BL_FUNC_DEBUG
    try {
        mdb_idfacturap = mui_list->dbValue ( QString ( "idfacturap" ), row );
        if ( editMode() ) {
            FacturaProveedorView * prov = new FacturaProveedorView ( ( BfCompany * ) mainCompany(), 0 );
            if ( prov->load ( mdb_idfacturap ) ) {
                delete prov;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addSubWindow ( prov );
            prov->show();
        } else {
            emit ( selected ( mdb_idfacturap ) );
        } // end if
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al cargar la factura proveedor" ), this );
    } // end try
    
}


/** SLOT que responde a la pulsacion del boton mui_borrar en el formulario.
    Crea una instancia de \ref FacturaProveedorView carga el elemento seleccionado
    y lanza el metodo \ref FacturaProveedorView::on_mui_borrar_clicked()
    Una vez borrado recarga el listado para que se actualicen los cambios. */
///
/**
\return
**/
void FacturasProveedorList::remove()
{
    BL_FUNC_DEBUG
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Debe seleccionar una linea" ), this );
        return;
    } // end if
    try {
        mdb_idfacturap = mui_list->dbValue ( "idfacturap" );
        FacturaProveedorView *bud = new FacturaProveedorView ( ( BfCompany * ) mainCompany(), 0 );
        bud->load ( mdb_idfacturap );
        bud->on_mui_borrar_clicked();
        delete bud;
        presentar();
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al borrar la factura de proveedor" ), this );
    } // end try
    
}


/** SLOT que responde a la pulsacion del boton mui_imprimir.
    La impresion de listados esta completamente delegada en BlSubForm. */
/**
**/
void FacturasProveedorList::imprimir()
{
    BL_FUNC_DEBUG
    printPDF ( _ ( "Facturas de proveedores" ) );
    
}


///
/**
\param comp
**/
void FacturasProveedorList::setMainCompany ( BfCompany *comp )
{
    BL_FUNC_DEBUG
    BlMainCompanyPointer::setMainCompany ( comp );
    m_proveedor->setMainCompany ( comp );
    m_articulo->setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    
}


///
/**
\return
**/
QString FacturasProveedorList::idfacturap()
{
    BL_FUNC_DEBUG
    return mdb_idfacturap;
    
}


///
/**
\param val
**/
void FacturasProveedorList::setidproveedor ( QString val )
{
    BL_FUNC_DEBUG
    m_proveedor->setId ( val );
    
}


///
/**
\param val
**/
void FacturasProveedorList::setidarticulo ( QString val )
{
    BL_FUNC_DEBUG
    m_articulo->setidarticulo ( val );
    
}


///
/**
**/

void FacturasProveedorList::crear()
{
    BL_FUNC_DEBUG
    FacturaProveedorView *fpv = new FacturaProveedorView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany()->pWorkspace()->addSubWindow ( fpv );
    fpv->inicializar();
    fpv->pintar();

    /// Si se crea el nueva factura desde la lista de facturas de un proveedor,
    /// entonces se crea la factura para este proveedor.
    if (m_proveedor->id() != "") {
	fpv->mui_idproveedor->setId( m_proveedor->id() );
    } // end if
    
    fpv->show();
    fpv->mui_descfacturap->setFocus ( Qt::OtherFocusReason );
    
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para que trabaje con la tabla facturap. */
/**
\param parent
**/
FacturasProveedorListSubform::FacturasProveedorListSubform ( QWidget *parent ) : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    setDbTableName ( "facturap" );
    setDbFieldId ( "idfacturap" );
    addSubFormHeader ( "reffacturap", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Referencia factura" ) );
    addSubFormHeader ( "idfacturap", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id factura" ) );
    addSubFormHeader ( "numfacturap", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Numero factura" ) );
    addSubFormHeader ( "nomproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre proveedor" ) );
    addSubFormHeader ( "ffacturap", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha factura" ) );
    addSubFormHeader ( "contactfacturap", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Persona de contacto" ) );
    addSubFormHeader ( "telfacturap", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Telefono factura" ) );
    addSubFormHeader ( "comentfacturap", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Comentario factura" ) );
    addSubFormHeader ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Id trabajador" ) );
    addSubFormHeader ( "idproveedor", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Id proveedor" ) );
    addSubFormHeader ( "total", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Total" ) );
    addSubFormHeader ( "base", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Base imponible" ) );
    addSubFormHeader ( "impuestos", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Impuestos" ) );
    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    
}


///
/**
**/
FacturasProveedorListSubform::~FacturasProveedorListSubform()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void FacturasProveedorListSubform::load()
{
    BL_FUNC_DEBUG
    QString SQLQuery = "SELECT * FROM facturap";
    BlSubForm::load ( SQLQuery );
    
}


///
/**
\param query
**/
void FacturasProveedorListSubform::load ( QString query )
{
    BL_FUNC_DEBUG
    BlSubForm::load ( query );
    
}

