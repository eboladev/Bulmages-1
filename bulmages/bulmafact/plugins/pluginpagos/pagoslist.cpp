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
#include <QMenu>
#include <QTextStream>

#include "pagoslist.h"
#include "company.h"
#include "pagoview.h"
#include "qtable2.h"
#include "funcaux.h"
#include "configuracion.h"


///
/**
\param parent
\param flag
**/
PagosList::PagosList ( QWidget *parent, Qt::WFlags flag, edmode editmodo)
        : BlFormList ( NULL, parent, flag, editmodo )
{
    _depura ( "PagosList::PagosList", 0 );
    setupUi ( this );
    mdb_idpago = "";
    setSubForm ( mui_list );
	/// Establecemos los parametros de busqueda del Cliente
	m_proveedor->setLabel ( _( "Proveedor:" ) );
	m_proveedor->setTableName( "proveedor" );
	m_proveedor->m_valores["cifproveedor"] = "";
	m_proveedor->m_valores["nomproveedor"] = "";
    hideBusqueda();
    _depura ( "END PagosList::PagosList", 0 );
}


///
/**
\param comp
\param parent
\param flag
**/
PagosList::PagosList ( Company *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    _depura ( "PagosList::PagosList", 0 );
    setupUi ( this );
    m_proveedor->setEmpresaBase ( comp );
    mui_list->setEmpresaBase ( comp );
    mui_idbanco->setEmpresaBase ( comp );
    mui_idbanco->setidbanco ( "" );
	/// Establecemos los parametros de busqueda del Cliente
	m_proveedor->setLabel ( _( "Proveedor:" ) );
	m_proveedor->setTableName( "proveedor" );
	m_proveedor->m_valores["cifproveedor"] = "";
	m_proveedor->m_valores["nomproveedor"] = "";

    presentar();
    mdb_idpago = "";
    setSubForm ( mui_list );
    if (modoEdicion()) {
    	empresaBase() ->meteWindow ( windowTitle(), this );
    } // end if
    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "pago" );
    _depura ( "END PagosList::PagosList", 0 );
}


PagosList::~PagosList()
{
    _depura ( "PagosList::~PagosList", 0 );
    _depura ( "END PagosList::~PagosList", 0 );
}




void PagosList::presentar()
{
    _depura ( "PagosList::presentar()", 0 );
    if ( empresaBase() != NULL ) {
        mui_list->cargar ( "SELECT * FROM pago NATURAL LEFT JOIN proveedor NATURAL LEFT JOIN trabajador NATURAL LEFT JOIN banco WHERE 1 = 1 " + generaFiltro() );
        /// Hacemos el calculo del total.
        BlFixed total = mui_list->sumarCampo ( "cantpago" );
        m_total->setText ( total.toQString() );
    } // end if
    _depura ( "END PagosList::presentar()", 0 );
}


QString PagosList::generaFiltro()
{
    _depura ( "PagosList::generaFiltro", 0 );

    QString filtro = "";
    if ( m_filtro->text() != "" ) {
        filtro = " AND ( lower(descpago) LIKE lower('%" + m_filtro->text() + "%') ";
        filtro += " OR refpago LIKE '" + m_filtro->text() + "%' ";
        filtro += " OR lower(nomproveedor) LIKE lower('%" + m_filtro->text() + "%')) ";
    } else {
        filtro = "";
    } // end if

    if ( m_proveedor->id() != "" ) {
        filtro += " AND pago.idproveedor = " + m_proveedor->id();
    } // end if

    QString subfiltro = " AND ";
    if ( mui_efectivos->isChecked() ) {
        filtro += " AND NOT previsionpago";
        subfiltro = " OR ";
    } // end if
    if ( mui_previsiones->isChecked() ) {
        filtro += subfiltro + " previsionpago";
    } // end if
    if ( m_fechain->text() != "" ) {
        filtro += " AND fechapago >= '" + m_fechain->text() + "' ";
    } // end if
    if ( m_fechafin->text() != "" ) {
        filtro += " AND fechapago <= '" + m_fechafin->text() + "' ";
    } // end if
    if ( mui_idbanco->idbanco() != "" )
        filtro += " AND idbanco = " + mui_idbanco->idbanco();

    _depura ( "END PagosList::generaFiltro", 0 );
    return ( filtro );
}





void PagosList::editar ( int )
{
    mdb_idpago = mui_list->DBvalue ( "idpago" );
    if ( modoEdicion() && mdb_idpago != "" ) {
        PagoView *bud = new PagoView( ( Company * ) empresaBase(), 0 );
        if ( bud->cargar ( mdb_idpago ) ) {
            delete bud;
            return;
        } // end if
        empresaBase() ->m_pWorkspace->addWindow ( bud );
        bud->show();
    } else {
        close();
    } // end if
}


void PagosList::crear()
{
    _depura ( "PagosList::crear", 0 );
    PagoView *bud = new PagoView( ( Company * ) empresaBase(), 0 );
    empresaBase() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    bud->setDBvalue ( "idproveedor", m_proveedor->id() );
    bud->pintar();
}


void PagosList::imprimir()
{
    _depura ( "PagosList::imprimir", 0 );
    mui_list->imprimirPDF ( _( "Pagos a proveedores" ) );
    _depura ( "END PagosList::imprimir", 0 );
}


void PagosList::borrar()
{
    _depura ( "PagosList::borrar", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        mensajeInfo ( _( "Debe seleccionar una linea" ), this );
        return;
    } // end if
    try {
        mdb_idpago = mui_list->DBvalue ( "idpago" );
        if ( modoEdicion() && mdb_idpago != "" ) {
            PagoView * bud = new PagoView ( ( Company * ) empresaBase(), NULL );
            bud->cargar ( mdb_idpago );
            bud->borrar();
        } // end if
        presentar();
    } catch ( ... )  {
        mensajeInfo ( _( "Error al borrar el pago" ), this );
    } // end try
    _depura ( "END PagosList::borrar", 0 );
}

void PagosList::setEmpresaBase ( Company *comp )
{
    _depura ( "PagosList::setEmpresaBase", 0 );
    PEmpresaBase::setEmpresaBase ( comp );
    m_proveedor->setEmpresaBase ( comp );
    mui_list->setEmpresaBase ( comp );
    mui_idbanco->setEmpresaBase ( comp );
    mui_idbanco->setidbanco ( "" );
    _depura ( "END PagosList::setEmpresaBase", 0 );
}


QString PagosList::idpago()
{
    return mdb_idpago;
}

void PagosList::setidproveedor ( QString val )
{
    m_proveedor->setId ( val );
}




/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
PagosListSubForm::PagosListSubForm ( QWidget *parent ) : SubForm2Bf ( parent )
{
    setDBTableName ( "pago" );
    setDBCampoId ( "idpago" );
    addSHeader ( "idpago", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, _( "Id pago" ) );
    addSHeader ( "idproveedor", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Id proveedor" ) );
    addSHeader ( "nomproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Nombre proveedor" ) );
    addSHeader ( "cifproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "C.I.F. proveedor" ) );
    addSHeader ( "telproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Telefono proveedor" ) );
    addSHeader ( "emailproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Email proveedor" ) );
    addSHeader ( "fechapago", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Fecha de pago" ) );
    addSHeader ( "cantpago", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Cantidad" ) );
    addSHeader ( "nombanco", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Banco" ) );
    addSHeader ( "refpago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Referencia de pago" ) );
    addSHeader ( "previsionpago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Prevision pago" ) );
    addSHeader ( "comentpago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Comentario pago" ) );
    addSHeader ( "idtrabajador", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Id trabajador" ) );
    addSHeader ( "nomtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Nombre de trabajador" ) );
    addSHeader ( "apellidostrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Apellidos trabajador" ) );
    setinsercion ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
}

PagosListSubForm::~PagosListSubForm()
{}


