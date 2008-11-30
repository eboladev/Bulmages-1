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
PagosList::PagosList ( QWidget *parent, Qt::WFlags flag )
        : Listado ( NULL, parent, flag )
{
    _depura ( "PagosList::PagosList", 0 );
    setupUi ( this );
    mdb_idpago = "";
    setSubForm ( mui_list );
    hideBusqueda();
    _depura ( "END PagosList::PagosList", 0 );
}


///
/**
\param comp
\param parent
\param flag
**/
PagosList::PagosList ( Company *comp, QWidget *parent, Qt::WFlags flag )
        : Listado ( comp, parent, flag )
{
    _depura ( "PagosList::PagosList", 0 );
    setupUi ( this );
    m_proveedor->setEmpresaBase ( comp );
    mui_list->setEmpresaBase ( comp );
    mui_idbanco->setEmpresaBase ( comp );
    mui_idbanco->setidbanco ( "" );
    presentar();
    mdb_idpago = "";
    setSubForm ( mui_list );
    empresaBase() ->meteWindow ( windowTitle(), this );
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
        Fixed total = mui_list->sumarCampo ( "cantpago" );
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

    if ( m_proveedor->idproveedor() != "" ) {
        filtro += " AND pago.idproveedor = " + m_proveedor->idproveedor();
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
        PagoView * bud = ( ( Company * ) empresaBase() ) ->newPagoView();
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
    PagoView *bud = ( ( Company * ) empresaBase() ) ->newPagoView();
    empresaBase() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    bud->setDBvalue ( "idproveedor", m_proveedor->idproveedor() );
    bud->pintar();
}


void PagosList::imprimir()
{
    _depura ( "PagosList::imprimir", 0 );
    mui_list->imprimirPDF ( tr ( "Pagos a proveedores" ) );
    _depura ( "END PagosList::imprimir", 0 );
}


void PagosList::borrar()
{
    _depura ( "PagosList::borrar", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        mensajeInfo ( tr ( "Debe seleccionar una linea" ), this );
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
        mensajeInfo ( tr ( "Error al borrar el pago" ), this );
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
    m_proveedor->setidproveedor ( val );
}




/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
PagosListSubForm::PagosListSubForm ( QWidget *parent ) : SubForm2Bf ( parent )
{
    setDBTableName ( "pago" );
    setDBCampoId ( "idpago" );
    addSHeader ( "idpago", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr ( "Id pago" ) );
    addSHeader ( "idproveedor", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Id proveedor" ) );
    addSHeader ( "nomproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Nombre proveedor" ) );
    addSHeader ( "cifproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "C.I.F. proveedor" ) );
    addSHeader ( "telproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Telefono proveedor" ) );
    addSHeader ( "emailproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Email proveedor" ) );
    addSHeader ( "fechapago", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Fecha de pago" ) );
    addSHeader ( "cantpago", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Cantidad" ) );
    addSHeader ( "nombanco", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Banco" ) );
    addSHeader ( "refpago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Referencia de pago" ) );
    addSHeader ( "previsionpago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Prevision pago" ) );
    addSHeader ( "comentpago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Comentario pago" ) );
    addSHeader ( "idtrabajador", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Id trabajador" ) );
    addSHeader ( "nomtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Nombre de trabajador" ) );
    addSHeader ( "apellidostrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Apellidos trabajador" ) );
    setinsercion ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
}

PagosListSubForm::~PagosListSubForm()
{}


