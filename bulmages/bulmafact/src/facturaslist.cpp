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

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "busquedaarticulo.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include "company.h"
#include "configuracion.h"
#include "facturaslist.h"
#include "facturaview.h"
#include "funcaux.h"
#include "plugins.h"


/** Prepara todos los componentes i los inicializa a NULL para que no haya dudas sobre si
    ya han sido inicializados o no.
    Inicializando la clase con este constructor precisa que sea establecido el company con setcompany.
*/
/**
\param parent
\param flag
\param editmodo
**/
FacturasList::FacturasList ( QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : Listado ( NULL, parent, flag, editmodo )
{
    _depura ( "FacturasList::FacturasList", 0 );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "FacturasList_FacturasList", this );
    if ( res != 0 )
        return;
    iniciaForm();
    mdb_idfactura = "";
    setSubForm ( mui_list );
    hideBusqueda();
    _depura ( "END FacturasList::FacturasList", 0 );
}


/** Inicializa todos los componentes y prepara la ventana para funcionar.
*/
/**
\param comp
\param parent
\param flag
\param editmodo
**/
FacturasList::FacturasList ( Company *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : Listado ( comp, parent, flag, editmodo )
{
    _depura ( "FacturasList::FacturasList", 0 );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "FacturasList_FacturasList", this );
    if ( res != 0 )
        return;
    iniciaForm();
    m_cliente->setEmpresaBase ( empresaBase() );
    m_articulo->setEmpresaBase ( empresaBase() );
    mui_list->setEmpresaBase ( empresaBase() );
    setSubForm ( mui_list );
    presentar();
    mdb_idfactura = "";
    if ( modoEdicion() ) {
        empresaBase() ->meteWindow ( windowTitle(), this );
    } // end if
    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "factura" );
    _depura ( "END FacturasList::FacturasList", 0 );
}


///
/**
\return
**/
void FacturasList::iniciaForm()
{
    _depura ( "FacturasList::iniciaForm" );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "FacturasList_iniciaForm", this );
    if ( res != 0 )
        return;
    mui_procesada->insertItem ( 0, tr ( "Todas las facturas" ) );
    mui_procesada->insertItem ( 1, tr ( "Facturas procesadas" ) );
    mui_procesada->insertItem ( 2, tr ( "Facturas no procesadas" ) );
    _depura ( "END FacturasList::iniciaForm" );
}


/** No precisa de acciones especiales en el destructor de la clase.
*/
/**
**/
FacturasList::~FacturasList()
{
    _depura ( "FacturasList::~FacturasList", 0 );
    _depura ( "END FacturasList::~FacturasList", 0 );
}


/** Hace la carga del subformulario y el calculo de los totales.
*/
/**
**/
void FacturasList::presentar()
{
    _depura ( "FacturasList::presentar", 0 );

    mui_list->cargar ( "SELECT *, totalfactura AS total, bimpfactura AS base, impfactura AS impuestos FROM factura LEFT JOIN cliente ON factura.idcliente = cliente.idcliente LEFT JOIN almacen ON factura.idalmacen = almacen.idalmacen WHERE 1 = 1 " + generaFiltro() );

    /// Hacemos el calculo del total.
    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT SUM(totalfactura) AS total, SUM(bimpfactura) AS base, SUM(impfactura) AS impuestos FROM factura LEFT JOIN cliente ON factura.idcliente = cliente.idcliente LEFT JOIN almacen ON factura.idalmacen = almacen.idalmacen WHERE 1 = 1 " + generaFiltro() );
    /// Esta consulta podria resultar en NULL y por tanto debe tratarse el caso
    if ( cur ) {
        mui_totalbimponible->setText ( cur->valor ( "base" ) );
        mui_totalimpuestos->setText ( cur->valor ( "impuestos" ) );
        mui_totalfacturas->setText ( cur->valor ( "total" ) );
        delete cur;
    } // end if

    _depura ( "END FacturasList::presentar", 0 );
}


/** Funcion auxiliar que genera la clausula WHERE de la consulta del listado a partir de las opciones de filtrado que el usuario haya especificado.
*/
/**
\return
**/
QString FacturasList::generaFiltro()
{
    _depura ( "FacturasList::generaFiltro", 0 );
    /// Tratamiento de los filtros.
    QString filtro = "";
    if ( m_filtro->text() != "" ) {
        filtro = " AND ( lower(descfactura) LIKE lower('%" + m_filtro->text() + "%') ";
        filtro += " OR reffactura LIKE '" + m_filtro->text() + "%' ";
        filtro += " OR lower(nomcliente) LIKE lower('%" + m_filtro->text() + "%')) ";
    } else {
        filtro = "";
    } // end if
    if ( m_cliente->idcliente() != "" ) {
        filtro += " AND factura.idcliente = " + m_cliente->idcliente();
    } // end if

    /// Tratamos los procesados y no procesados en las facturas para el filtraje.
    if ( mui_procesada->currentIndex() == 1 ) {
        /// Muestra solo las procesadas.
        filtro += " AND procesadafactura ";
    } else if ( mui_procesada->currentIndex() == 2 ) {
        /// Muestra solo las NO procesadas.
        filtro += " AND NOT procesadafactura ";
    } // end if

    if ( m_articulo->idarticulo() != "" ) {
        filtro += " AND idfactura IN (SELECT DISTINCT idfactura FROM lfactura WHERE idarticulo = '" + m_articulo->idarticulo() + "') ";
    } // end if
    if ( m_fechain->text() != "" ) {
        filtro += " AND ffactura >= '" + m_fechain->text() + "' ";
    } // end if
    if ( m_fechafin->text() != "" ) {
        filtro += " AND ffactura <= '" + m_fechafin->text() + "' ";
    } // end if
    _depura ( "END FacturasList::generaFiltro", 0 );
    return ( filtro );
}


/** Metodo que responde a la accion por defecto sobre el listado (doble click o boton de editar)
    Si estamos en modo edicion abre la ficha de Factura y carga el elemento seleccionado.
    Si estamos en modo seleccion cierra la ventana y emite el SIGNAL de que se ha seleccionado una factura.
*/
/**
\param row
\return
**/
void FacturasList::editar ( int row )
{
    _depura ( "FacturasList::editar", 0 );
    mdb_idfactura = mui_list->DBvalue ( QString ( "idfactura" ), row );
    if ( modoEdicion() ) {
        FacturaView * prov = ( ( Company * ) empresaBase() ) ->newFacturaView();
        if ( prov->cargar ( mdb_idfactura ) ) {
            delete prov;
            return;
        } // end if
        empresaBase() ->m_pWorkspace->addWindow ( prov );
        prov->show();
    } else {
        emit ( selected ( mdb_idfactura ) );
    } // end if
    _depura ( "END FacturasList::editar", 0 );
}



/** SLOT que responde a la pulsacion del boton de imprimir.
    La impresion de listados esta completamente delegada en SubForm2Bf
*/
/**
**/
void FacturasList::imprimir()
{
    _depura ( "FacturasList::on_mui_imprimir_clicked", 0 );
    mui_list->imprimirPDF ( tr ( "Facturas a clientes" ) );
    _depura ( "FacturasList::on_mui_imprimir_clicked", 0 );
}


/** SLOT que responde a la pulsacion del boton borrar.
    Crea una instancia de FacuturaView, carga el elemento seleccionado e invoca al
    metodo de borrar.
    La ventaja de hacerlo de esta forma es que si hay plugins en la factura, estos tambien se ejecutaran.
*/
///
/**
\return
**/
void FacturasList::borrar()
{
    _depura ( "FacturasList::borrar", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        mensajeInfo ( tr ( "Debe seleccionar una linea" ) );
        return;
    } // end if
    try {
        mdb_idfactura = mui_list->DBvalue ( QString ( "idfactura" ) );
        if ( modoEdicion() ) {
            FacturaView * fv = ( ( Company * ) empresaBase() ) ->newFacturaView();
            if ( fv->cargar ( mdb_idfactura ) )
                throw - 1;
            fv->on_mui_borrar_clicked();
            fv->close();
        } // end if
        presentar();
    } catch ( ... ) {
        mensajeInfo ( tr ( "Error al borrar la factura a cliente" ) );
    } // end try
    _depura ( "END FacturasList::borrar", 0 );
}


///
/**
\param comp
**/
void FacturasList::setEmpresaBase ( Company *comp )
{
    _depura ( "FacturasList::setEmpresaBase", 0 );
    PEmpresaBase::setEmpresaBase ( comp );
    m_cliente->setEmpresaBase ( comp );
    m_articulo->setEmpresaBase ( comp );
    mui_list->setEmpresaBase ( comp );
    _depura ( "END FacturasList::setEmpresaBase", 0 );
}


///
/**
\return
**/
QString FacturasList::idfactura()
{
    _depura ( "FacturasList::idfacturae", 0 );
    _depura ( "END FacturasList::idfactura", 0 );
    return mdb_idfactura;
}


///
/**
\param val
**/
void FacturasList::setidcliente ( QString val )
{
    _depura ( "FacturasList::setidcliente", 0 );
    m_cliente->setidcliente ( val );
    _depura ( "END FacturasList::setidcliente", 0 );
}


///
/**
\param val
**/
void FacturasList::setidarticulo ( QString val )
{
    _depura ( "FacturasList::setidarticulo", 0 );
    m_articulo->setidarticulo ( val );
    _depura ( "END FacturasList::setidarticulo", 0 );
}


///
/**
**/
void FacturasList::crear()
{
    _depura ( "FacturasList::crear", 0 );
    ( ( Company * ) empresaBase() ) ->s_newFacturaCli();
    _depura ( "END FacturasList::crear", 0 );
}

/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para trabajar con la tabla factura.
*/
/**
\param parent
\return
**/
FacturasListSubform::FacturasListSubform ( QWidget *parent, const char * ) : SubForm2Bf ( parent )
{
    _depura ( "FacturasListSubform::FacturasListSubform", 0 );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "FacturasListSubform_FacturasListSubform", this );
    if ( res != 0 )
        return;
    setDBTableName ( "factura" );
    setDBCampoId ( "idfactura" );
    addSHeader ( "numfactura", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Numero" ) );
    addSHeader ( "ffactura", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Fecha" ) );
    addSHeader ( "nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Nombre cliente" ) );
    addSHeader ( "telfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Telefono factura" ) );
    addSHeader ( "base", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Base imponible" ) );
    addSHeader ( "impuestos", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Impuestos" ) );
    addSHeader ( "total", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Total" ) );
    addSHeader ( "procesadafactura", DBCampo::DBboolean, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Procesada" ) );
    addSHeader ( "descfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Descripcion" ) );
    addSHeader ( "idfactura", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Id factura" ) );
    addSHeader ( "reffactura", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr ( "Ref factura" ) );
    addSHeader ( "codigoserie_factura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Serie" ) );
    addSHeader ( "cifcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "CIF cliente" ) );
    addSHeader ( "codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Codigo almacen" ) );
    addSHeader ( "contactfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Contact factura" ) );
    addSHeader ( "comentfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Comentario factura" ) );
    addSHeader ( "idtrabajador", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Id trabajador" ) );
    addSHeader ( "idcliente", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Id cliente" ) );
    addSHeader ( "idalmacen", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Id almacen" ) );
    setinsercion ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    _depura ( "END FacturasListSubform::FacturasListSubform", 0 );
}


///
/**
**/
FacturasListSubform::~FacturasListSubform()
{
    _depura ( "FacturasListSubform::~FacturasListSubform", 0 );
    _depura ( "END FacturasListSubform::~FacturasListSubform", 0 );
}


///
/**
**/
void FacturasListSubform::cargar()
{
    _depura ( "FacturasListSubform::cargar\n", 0 );
    QString SQLQuery = "SELECT * FROM factura";
    SubForm3::cargar ( SQLQuery );
}


///
/**
\param query
**/
void FacturasListSubform::cargar ( QString query )
{
    _depura ( "FacturasListSubform::cargar", 0 );
    SubForm3::cargar ( query );
    _depura ( "END FacturasListSubform::cargar", 0 );
}

