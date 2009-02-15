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
#include "company.h"
#include "busquedaarticulo.h"
#include "blconfiguration.h"
#include "facturapview.h"
#include "plugins.h"


/** Inicializa todos los componentes del listado a NULL para que no exista consusion sobre
    si un elemento ha sido inicializado o no.
    Al construir la clase con este constructor debe ser inicializada con setcompany().
    Mete la ventana en el workSpace. */
/**
\param parent
\param flag
\return
**/
FacturasProveedorList::FacturasProveedorList ( QWidget *parent, Qt::WFlags flag, edmode editmodo)
        : BlFormList ( NULL, parent, flag, editmodo )
{
    _depura ( "FacturasProveedorList::FacturasProveedorList", 0 );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "FacturasProveedorList_FacturasProveedorList", this );
    if ( res != 0 ) {
        return;
    } // end if
    mdb_idfacturap = "";
    setSubForm ( mui_list );
	/// Establecemos los parametros de busqueda del Cliente
	m_proveedor->setLabel ( _( "Proveedor:" ) );
	m_proveedor->setTableName( "proveedor" );
	m_proveedor->m_valores["cifproveedor"] = "";
	m_proveedor->m_valores["nomproveedor"] = "";
    hideBusqueda();
    iniciaForm();
    g_plugins->lanza ( "FacturasProveedorList_FacturasProveedorList_Post", this );
    _depura ( "FacturasProveedorList::FacturasProveedorList", 0 );
}


/** Inicializa todos los componentes, hace la carga inicial con \ref presenta()
    mete la ventana en el workSpace(). */
/**
\param comp
\param parent
\return
**/
FacturasProveedorList::FacturasProveedorList ( Company *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    _depura ( "FacturasProveedorList::FacturasProveedorList", 0 );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "FacturasProveedorList_FacturasProveedorList", this );
    if ( res != 0 ) {
        return;
    } // end if
    m_proveedor->setEmpresaBase ( empresaBase() );
    m_articulo->setEmpresaBase ( empresaBase() );
    mui_list->setEmpresaBase ( comp );
    presentar();
    setSubForm ( mui_list );
    mdb_idfacturap = "";
	/// Establecemos los parametros de busqueda del Cliente
	m_proveedor->setLabel ( _( "Proveedor:" ) );
	m_proveedor->setTableName( "proveedor" );
	m_proveedor->m_valores["cifproveedor"] = "";
	m_proveedor->m_valores["nomproveedor"] = "";
    if(modoEdicion()) {
    	empresaBase() ->meteWindow ( windowTitle(), this );
    } // end if
    hideBusqueda();
    iniciaForm();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "facturap" );

    /// Disparamos los plugins.
    g_plugins->lanza ( "FacturasProveedorList_FacturasProveedorList_Post", this );

    _depura ( "END FacturasProveedorList::FacturasProveedorList", 0 );
}


///
/**
\return
**/
void FacturasProveedorList::iniciaForm()
{
    _depura ( "FacturasProveedorList::iniciaForm" );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "FacturasProveedorList_iniciaForm", this );
    if ( res != 0 )
        return;
    mui_procesada->insertItem ( 0, _( "Todas las facturas" ) );
    mui_procesada->insertItem ( 1, _( "Facturas procesadas" ) );
    mui_procesada->insertItem ( 2, _( "Facturas no procesadas" ) );
    _depura ( "END FacturasProveedorList::iniciaForm" );
}


/** No precisa de acciones especiales en el destructor. */
/**
**/
FacturasProveedorList::~FacturasProveedorList()
{
    _depura ( "FacturasProveedorList::~FacturasProveedorList", 0 );
    _depura ( "END FacturasProveedorList::~FacturasProveedorList", 0 );
}


/** Hace la carga inicial del listado.
    Tambien hace el calculo de totales y lo presenta. */
/**
\return
**/
void FacturasProveedorList::presentar()
{
    _depura ( "FacturasProveedorList::presentar", 0 );
    mui_list->cargar ( "SELECT *, totalfacturap AS total, bimpfacturap AS base, impfacturap AS impuestos  FROM facturap LEFT JOIN proveedor ON facturap.idproveedor=proveedor.idproveedor WHERE 1=1  " + generaFiltro() );

    /// Hacemos el calculo del total.
    BlDbRecordSet *cur = empresaBase() ->cargacursor ( "SELECT SUM(totalfacturap) AS total FROM facturap LEFT JOIN proveedor ON facturap.idproveedor=proveedor.idproveedor WHERE 1=1  " + generaFiltro() );
    /// Si ha habido un error con el query salimos
    if ( !cur ) return;
    m_total->setText ( cur->valor ( "total" ) );
    delete cur;
    _depura ( "END FacturasProveedorList::presentar", 0 );
}


/** Metodo auxiliar que crea la clausula WHERE del query de carga  \ref presenta()
    La clausula WHERE utiliza todas las opciones de filtrado para crearse. */
/**
\return
**/
QString FacturasProveedorList::generaFiltro()
{
    _depura ( "FacturasProveedorList::generaFiltro", 0 );
    /// Tratamiento de los filtros.
    QString filtro = "";
    if ( m_filtro->text() != "" ) {
        filtro = " AND ( lower(descfacturap) LIKE lower('%" + m_filtro->text() + "%') ";
        filtro += " OR reffacturap LIKE '" + m_filtro->text() + "%' ";
        filtro += " OR lower(nomproveedor) LIKE lower('%" + m_filtro->text() + "%')) ";
    } // end if
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

    _depura ( "END FacturasProveedorList::generaFiltro", 0 );
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
    _depura ( "FacturasProveedorList::editar", 0 );
    try {
        mdb_idfacturap = mui_list->DBvalue ( QString ( "idfacturap" ), row );
        if ( modoEdicion() ) {
            FacturaProveedorView * prov = new FacturaProveedorView ( ( Company * ) empresaBase(), 0 );
            if ( prov->cargar ( mdb_idfacturap ) ) {
                delete prov;
                return;
            } // end if
            empresaBase() ->m_pWorkspace->addWindow ( prov );
            prov->show();
        } else {
            emit ( selected ( mdb_idfacturap ) );
        } // end if
    } catch ( ... ) {
        mensajeInfo ( _( "Error al cargar la factura proveedor" ), this );
    } // end try
    _depura ( "END FacturasProveedorList::editar", 0 );
}


/** SLOT que responde a la pulsacion del boton mui_borrar en el formulario.
    Crea una instancia de \ref FacturaProveedorView carga el elemento seleccionado
    y lanza el metodo \ref FacturaProveedorView::on_mui_borrar_clicked()
    Una vez borrado recarga el listado para que se actualicen los cambios. */
///
/**
\return
**/
void FacturasProveedorList::borrar()
{
    _depura ( "FacturasProveedorList::borrar", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        mensajeInfo ( _( "Debe seleccionar una linea" ), this );
        return;
    } // end if
    try {
        mdb_idfacturap = mui_list->DBvalue ( "idfacturap" );
        FacturaProveedorView *bud = new FacturaProveedorView( ( Company * ) empresaBase(), 0 );
        bud->cargar ( mdb_idfacturap );
        bud->on_mui_borrar_clicked();
        delete bud;
        presentar();
    } catch ( ... ) {
        mensajeInfo ( _( "Error al borrar la factura de proveedor" ), this );
    } // end try
    _depura ( "END FacturasProveedorList::borrar", 0 );
}


/** SLOT que responde a la pulsacion del boton mui_imprimir.
    La impresion de listados esta completamente delegada en BlSubForm. */
/**
**/
void FacturasProveedorList::imprimir()
{
    _depura ( "FacturasProveedorList::on_mui_imprimir_clicked", 0 );
    mui_list->imprimirPDF ( _( "Facturas de proveedores" ) );
    _depura ( "END FacturasProveedorList::on_mui_imprimir_clicked", 0 );
}


///
/**
\param comp
**/
void FacturasProveedorList::setEmpresaBase ( Company *comp )
{
    _depura ( "FacturasProveedorList::setEmpresaBase", 0 );
    PEmpresaBase::setEmpresaBase ( comp );
    m_proveedor->setEmpresaBase ( comp );
    m_articulo->setEmpresaBase ( comp );
    mui_list->setEmpresaBase ( comp );
    _depura ( "END FacturasProveedorList::setEmpresaBase", 0 );
}


///
/**
\return
**/
QString FacturasProveedorList::idfacturap()
{
    _depura ( "FacturasProveedorList::idfacturap", 0 );
    return mdb_idfacturap;
    _depura ( "END FacturasProveedorList::idfacturap", 0 );
}


///
/**
\param val
**/
void FacturasProveedorList::setidproveedor ( QString val )
{
    _depura ( "FacturasProveedorList::setidproveedor", 0 );
    m_proveedor->setId ( val );
    _depura ( "END FacturasProveedorList::setidproveedor", 0 );
}


///
/**
\param val
**/
void FacturasProveedorList::setidarticulo ( QString val )
{
    _depura ( "FacturasProveedorList::setidarticulo", 0 );
    m_articulo->setidarticulo ( val );
    _depura ( "END FacturasProveedorList::setidarticulo", 0 );
}


///
/**
**/

void FacturasProveedorList::crear()
{
    _depura ( "FacturasProveedorList::crear", 0 );
    FacturaProveedorView *fact = new FacturaProveedorView( ( Company * ) empresaBase(), 0 );
    empresaBase()->pWorkspace()->addWindow ( fact );
    fact->inicializar();
    fact->show();
    fact->mui_descfacturap->setFocus ( Qt::OtherFocusReason );
    _depura ( "END FacturasProveedorList::crear", 0 );
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
    _depura ( "FacturasProveedorListSubform::FacturasProveedorListSubform", 0 );
    setDBTableName ( "facturap" );
    setDBCampoId ( "idfacturap" );
    addSHeader ( "reffacturap", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Referencia factura" ) );
    addSHeader ( "idfacturap", BlDbField::DBint, BlDbField::DBNotNull | BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Id factura" ) );
    addSHeader ( "numfacturap", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Numero factura" ) );
    addSHeader ( "nomproveedor", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Nombre proveedor" ) );
    addSHeader ( "ffacturap", BlDbField::DBdate, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Fecha factura" ) );
    addSHeader ( "contactfacturap", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Persona de contacto" ) );
    addSHeader ( "telfacturap", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Telefono factura" ) );
    addSHeader ( "comentfacturap", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Comentario factura" ) );
    addSHeader ( "idtrabajador", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Id trabajador" ) );
    addSHeader ( "idproveedor", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Id proveedor" ) );
    addSHeader ( "total", BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Total" ) );
    addSHeader ( "base", BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Base imponible" ) );
    addSHeader ( "impuestos", BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Impuestos" ) );
    setinsercion ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    _depura ( "END FacturasProveedorListSubform::FacturasProveedorListSubform", 0 );
}


///
/**
**/
FacturasProveedorListSubform::~FacturasProveedorListSubform()
{
    _depura ( "FacturasProveedorListSubform::~FacturasProveedorListSubform", 0 );
    _depura ( "END FacturasProveedorListSubform::~FacturasProveedorListSubform", 0 );
}


///
/**
**/
void FacturasProveedorListSubform::cargar()
{
    _depura ( "FacturasProveedorListSubform::cargar", 0 );
    QString SQLQuery = "SELECT * FROM facturap";
    BlSubForm::cargar ( SQLQuery );
    _depura ( "END FacturasProveedorListSubform::cargar", 0 );
}


///
/**
\param query
**/
void FacturasProveedorListSubform::cargar ( QString query )
{
    _depura ( "FacturasProveedorListSubform::cargar", 0 );
    BlSubForm::cargar ( query );
    _depura ( "END FacturasProveedorListSubform::cargar", 0 );
}

