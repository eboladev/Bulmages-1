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
#include "company.h"
#include "blconfiguration.h"
#include "funcaux.h"


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
    _depura ( "AlbaranesProveedor::AlbaranesProveedor", 0 );
    setupUi ( this );
    mdb_idalbaranp = "";
    setSubForm ( mui_list );
	/// Establecemos los parametros de busqueda del Cliente
	m_proveedor->setLabel ( _( "Proveedor:" ) );
	m_proveedor->setTableName( "proveedor" );
	m_proveedor->m_valores["cifproveedor"] = "";
	m_proveedor->m_valores["nomproveedor"] = "";
    hideBusqueda();
    iniciaForm();
    _depura ( "END AlbaranesProveedor::AlbaranesProveedor", 0 );
}


///
/**
\param comp
**/
void AlbaranesProveedor::setEmpresaBase ( Company *comp )
{
    _depura ( "AlbaranesProveedor::setEmpresaBase", 0 );
    PEmpresaBase::setEmpresaBase ( comp );
    m_proveedor->setEmpresaBase ( comp );
    m_articulo->setEmpresaBase ( comp );
    mui_list->setEmpresaBase ( comp );
	/// Establecemos los parametros de busqueda del Cliente
	m_proveedor->setLabel ( _( "Proveedor:" ) );
	m_proveedor->setTableName( "proveedor" );
	m_proveedor->m_valores["cifproveedor"] = "";
	m_proveedor->m_valores["nomproveedor"] = "";

    _depura ( "END AlbaranesProveedor::setEmpresaBase", 0 );
}


///
/**
**/
void AlbaranesProveedor::crear()
{
    _depura ( "AlbaranesProveedor::crear", 0 );
    if ( empresaBase() != NULL ) {
        AlbaranProveedorView *albp = new AlbaranProveedorView( ( Company * ) empresaBase(), 0 );
        empresaBase()->pWorkspace()->addWindow ( albp );
	albp->inicializar();
	albp->show();
	albp->mui_descalbaranp->setFocus ( Qt::OtherFocusReason );
    }// end if
    _depura ( "END AlbaranesProveedor::crear", 0 );
}


///
/**
\param val
**/
void AlbaranesProveedor::setidproveedor ( QString val )
{
    _depura ( "AlbaranesProveedor::setidproveedor", 0 );
    m_proveedor->setId ( val );
    _depura ( "END AlbaranesProveedor::setidproveedor", 0 );
}


///
/**
\param val
**/
void AlbaranesProveedor::setidarticulo ( QString val )
{
    _depura ( "AlbaranesProveedor::setidarticulo", 0 );
    m_articulo->setidarticulo ( val );
    _depura ( "AlbaranesProveedor::setidarticulo", 0 );
}


///
/**
\return
**/
QString AlbaranesProveedor::idalbaranp()
{
    _depura ( "AlbaranesProveedor::idalbaranp", 0 );
    _depura ( "END AlbaranesProveedor::idalbaranp", 0 );
    return mdb_idalbaranp;
}


/** Constructor completo de la clase con el puntero a Company adecuado.
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
AlbaranesProveedor::AlbaranesProveedor ( Company *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    _depura ( "AlbaranesProveedor::AlbaranesProveedor", 0 );
    setupUi ( this );
    m_proveedor->setEmpresaBase ( comp );
    m_articulo->setEmpresaBase ( comp );
    mui_list->setEmpresaBase ( comp );
    setSubForm ( mui_list );
    presentar();
    mdb_idalbaranp = "";
    if (modoEdicion()) {
    	empresaBase() ->meteWindow ( windowTitle(), this );
    } // end if
    hideBusqueda();
    iniciaForm();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "albaranp" );
    _depura ( "END AlbaranesProveedor::AlbaranesProveedor", 0 );
}


///
/**
\return
**/
void AlbaranesProveedor::iniciaForm()
{
    _depura ( "FacturasProveedorList::iniciaForm", 0 );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "AlbaranesProveedor_iniciaForm", this );
    if ( res != 0 ) {
        _depura ( "END AlbaranesProveedor::iniciaForm", 0, "Salida por Plugin" );
        return;
    } // end if
    mui_procesada->insertItem ( 0, _( "Todos los albaranes" ) );
    mui_procesada->insertItem ( 1, _( "Albaranes procesados" ) );
    mui_procesada->insertItem ( 2, _( "Albaranes no procesados" ) );
    _depura ( "END AlbaranesProveedor::iniciaForm", 0 );
}



/** Refresca la ventana de listados de albaranes.
*/
/**
**/
AlbaranesProveedor::~AlbaranesProveedor()
{
    _depura ( "AlbaranesProveedor::~AlbaranesProveedor", 0 );
    _depura ( "END AlbaranesProveedor::~AlbaranesProveedor", 0 );
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
    _depura ( "AlbaranesProveedor::presentar", 0 );
    if ( empresaBase() != NULL ) {
        mui_list->cargar ( "SELECT *, totalalbaranp AS total, " \
                           "bimpalbaranp AS base, impalbaranp AS impuestos " \
                           "FROM albaranp LEFT " \
                           "JOIN proveedor ON albaranp.idproveedor = " \
                           "proveedor.idproveedor LEFT JOIN almacen ON " \
                           "albaranp.idalmacen = almacen.idalmacen LEFT JOIN " \
                           "forma_pago ON albaranp.idforma_pago = " \
                           "forma_pago.idforma_pago WHERE 1 = 1 " + generaFiltro() );

        /// Hacemos el calculo del total.
        cursor2 *cur = empresaBase() ->cargacursor ( "SELECT SUM(totalalbaranp) " \
                       "AS total FROM albaranp LEFT JOIN proveedor ON " \
                       "albaranp.idproveedor = proveedor.idproveedor LEFT " \
                       "JOIN almacen ON albaranp.idalmacen = almacen.idalmacen " \
                       "WHERE 1 = 1 " + generaFiltro() );
        /// En caso de que el query haya fallado salimos.
        if ( !cur ) return;
        m_total->setText ( cur->valor ( "total" ) );
        delete cur;
    }
    _depura ( "END AlbaranesProveedor::presentar", 0 );
}


/** Este es un metodo auxiliar para presenta() que se encarga de generar
    la clausula WHERE de la consulta.
*/
/**
\return
**/
QString AlbaranesProveedor::generaFiltro()
{
    _depura ( "AlbaranesProveedor::generaFiltro", 0 );
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
    _depura ( "END AlbaranesProveedor::generaFiltro", 0 );
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
    _depura ( "AlbaranesProveedor::editar", 0 );
    mdb_idalbaranp = mui_list->DBvalue ( QString ( "idalbaranp" ), row );
    if ( modoEdicion() ) {
        AlbaranProveedorView * prov = new AlbaranProveedorView ( ( Company * ) empresaBase(), 0 );
        if ( prov->cargar ( mdb_idalbaranp ) ) {
            delete prov;
            return;
        } // end if
        empresaBase() ->m_pWorkspace->addWindow ( prov );
        prov->show();
    } else {
        emit ( selected ( mdb_idalbaranp ) );
    } // end if
    _depura ( "END AlbaranesProveedor::editar", 0 );
}



/** La impresion de listados esta completamente delegada a la clase BlSubForm
*/
/**
**/
void AlbaranesProveedor::imprimir()
{
    _depura ( "AlbaranesProveedor::imprimir", 0 );
    mui_list->imprimirPDF ( _( "Albaranes de proveedor" ) );
    _depura ( "END AlbaranesProveedor::imprimir", 0 );
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
void AlbaranesProveedor::borrar()
{
    _depura ( "AlbaranesProveedor::borrar", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        mensajeInfo ( _( "Debe seleccionar una linea" ), this );
        return;
    } // end if
    try {
        mdb_idalbaranp = mui_list->DBvalue ( QString ( "idalbaranp" ) );
        if ( modoEdicion() ) {
            AlbaranProveedorView * apv = new AlbaranProveedorView(( Company * ) empresaBase(), 0);
            if ( apv->cargar ( mdb_idalbaranp ) ) {
                throw - 1;
            } // end if
            apv->on_mui_borrar_clicked();
            apv->close();
        } // end if
        presentar();
    } catch ( ... ) {
        mensajeInfo ( _( "Error al borrar albaran de proveedor" ), this );
    } // end try
    _depura ( "END AlbaranesProveedor::borrar", 0 );
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
    _depura ( "AlbaranesProveedorListSubform::~AlbaranesProveedorListSubform", 0 );
    _depura ( "END AlbaranesProveedorListSubform::~AlbaranesProveedorListSubform", 0 );
}


///
/**
\param parent
**/
AlbaranesProveedorListSubform::AlbaranesProveedorListSubform ( QWidget *parent ) : SubForm2Bf ( parent )
{
    _depura ( "AlbaranesProveedorListSubform::AlbaranesProveedorListSubform", 0 );
    setDBTableName ( "albaranp" );
    setDBCampoId ( "idalbaranp" );
    addSHeader ( "idalbaranp", BlDbField::DBint, BlDbField::DBNotNull | BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "ID albaran" ) );
    addSHeader ( "numalbaranp", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Numero de albaran" ) );
    addSHeader ( "descalbaranp", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descripcion" ) );
    addSHeader ( "refalbaranp", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Referencia" ) );
    addSHeader ( "fechaalbaranp", BlDbField::DBdate, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Fecha" ) );
    addSHeader ( "comentalbaranp", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Comentario" ) );
    addSHeader ( "procesadoalbaranp", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Procesado" ) );
    addSHeader ( "idproveedor", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "ID proveedor" ) );
    addSHeader ( "idforma_pago", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "ID forma de pago" ) );
    addSHeader ( "idalmacen", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "ID almacen" ) );
    addSHeader ( "nomproveedor", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Nombre del proveedor" ) );
    addSHeader ( "nomalmacen", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Nombre del almacen" ) );
    addSHeader ( "descforma_pago", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descripcion de la forma de pago" ) );
    addSHeader ( "base", BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Base imponible" ) );
    addSHeader ( "impuestos", BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Impuestos" ) );
    addSHeader ( "total", BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Total albaran" ) );
    setinsercion ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    _depura ( "END AlbaranesProveedorListSubform::AlbaranesProveedorListSubform", 0 );
}


///
/**
**/
void AlbaranesProveedorListSubform::cargar()
{
    _depura ( "AlbaranesProveedorListSubform::cargar", 0 );
    QString SQLQuery = "SELECT * FROM albaranp";
    BlSubForm::cargar ( SQLQuery );
    _depura ( "END AlbaranesProveedorListSubform::cargar", 0 );
}


///
/**
\param query
**/
void AlbaranesProveedorListSubform::cargar ( QString query )
{
    _depura ( "AlbaranesProveedorListSubform::cargar", 0 );
    BlSubForm::cargar ( query );
    _depura ( "END AlbaranesProveedorListSubform::cargar", 0 );
}

