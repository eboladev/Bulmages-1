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
#include "bfcompany.h"
#include "pagoview.h"
#include "bltablewidget.h"
#include "blfunctions.h"
#include "blconfiguration.h"


///
/**
\param parent
\param flag
**/
PagosList::PagosList ( QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    blDebug ( "PagosList::PagosList", 0 );
    setupUi ( this );
    mdb_idpago = "";
    setSubForm ( mui_list );
    /// Establecemos los parametros de busqueda del Cliente
    m_proveedor->setLabel ( _ ( "Proveedor:" ) );
    m_proveedor->setTableName ( "proveedor" );
    m_proveedor->m_valores["cifproveedor"] = "";
    m_proveedor->m_valores["nomproveedor"] = "";
    hideBusqueda();
    /// Llamamos a los scripts
    blScript(this);
    blDebug ( "END PagosList::PagosList", 0 );
}


///
/**
\param comp
\param parent
\param flag
**/
PagosList::PagosList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    blDebug ( "PagosList::PagosList", 0 );
    setupUi ( this );
    m_proveedor->setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    mui_idbanco->setMainCompany ( comp );
    mui_idbanco->setidbanco ( "" );
    /// Establecemos los parametros de busqueda del Cliente
    m_proveedor->setLabel ( _ ( "Proveedor:" ) );
    m_proveedor->setTableName ( "proveedor" );
    m_proveedor->m_valores["cifproveedor"] = "";
    m_proveedor->m_valores["nomproveedor"] = "";

    /// Cargamos los filtros guardados.
    cargaFiltrosXML();

    presentar();
    mdb_idpago = "";
    setSubForm ( mui_list );
    if ( modoEdicion() ) {
        mainCompany() ->meteWindow ( windowTitle(), this );
    } // end if
    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "pago" );
    /// Llamamos a los scripts
    blScript(this);
    blDebug ( "END PagosList::PagosList", 0 );
}


PagosList::~PagosList()
{
    blDebug ( "PagosList::~PagosList", 0 );
    blDebug ( "END PagosList::~PagosList", 0 );
}




void PagosList::presentar()
{
    blDebug ( "PagosList::presentar()", 0 );
    if ( mainCompany() != NULL ) {
        mui_list->cargar ( "SELECT * FROM pago NATURAL LEFT JOIN proveedor NATURAL LEFT JOIN trabajador NATURAL LEFT JOIN banco WHERE 1 = 1 " + generaFiltro() );
        /// Hacemos el calculo del total.
        BlFixed total = mui_list->sumarCampo ( "cantpago" );
        m_total->setText ( total.toQString() );
    } // end if
    blDebug ( "END PagosList::presentar()", 0 );
}


QString PagosList::generaFiltro()
{
    blDebug ( "PagosList::generaFiltro", 0 );

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

    blDebug ( "END PagosList::generaFiltro", 0 );
    return ( filtro );
}





void PagosList::editar ( int )
{
    mdb_idpago = mui_list->dbValue ( "idpago" );
    if ( modoEdicion() && mdb_idpago != "" ) {
        PagoView *bud = new PagoView ( ( BfCompany * ) mainCompany(), 0 );
        if ( bud->cargar ( mdb_idpago ) ) {
            delete bud;
            return;
        } // end if
        mainCompany() ->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } else {
        close();
    } // end if
}


void PagosList::crear()
{
    blDebug ( "PagosList::crear", 0 );
    PagoView *pv = new PagoView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->m_pWorkspace->addSubWindow ( pv );
    pv->pintar();

    /// Si se crea el nuevo pago desde la lista de pagos a un proveedor,
    /// entonces se crea el pago a este proveedor.
    if (m_proveedor->id() != "") {
	pv->mui_idproveedor->setId( m_proveedor->id() );
    } // end if
    
    pv->show();
}


void PagosList::imprimir()
{
    blDebug ( "PagosList::imprimir", 0 );
    mui_list->imprimirPDF ( _ ( "Pagos a proveedores" ) );
    blDebug ( "END PagosList::imprimir", 0 );
}


void PagosList::borrar()
{
    blDebug ( "PagosList::borrar", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Debe seleccionar una linea" ), this );
        return;
    } // end if
    try {
        mdb_idpago = mui_list->dbValue ( "idpago" );
        if ( modoEdicion() && mdb_idpago != "" ) {
            PagoView * bud = new PagoView ( ( BfCompany * ) mainCompany(), NULL );
            bud->cargar ( mdb_idpago );
            bud->borrar();
        } // end if
        presentar();
    } catch ( ... )  {
        blMsgInfo ( _ ( "Error al borrar el pago" ), this );
    } // end try
    blDebug ( "END PagosList::borrar", 0 );
}

void PagosList::setMainCompany ( BfCompany *comp )
{
    blDebug ( "PagosList::setMainCompany", 0 );
    BlMainCompanyPointer::setMainCompany ( comp );
    m_proveedor->setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    mui_idbanco->setMainCompany ( comp );
    mui_idbanco->setidbanco ( "" );
    blDebug ( "END PagosList::setMainCompany", 0 );
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
PagosListSubForm::PagosListSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    setDbTableName ( "pago" );
    setDbFieldId ( "idpago" );
    addSubFormHeader ( "idpago", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id pago" ) );
    addSubFormHeader ( "idproveedor", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Id proveedor" ) );
    addSubFormHeader ( "nomproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre proveedor" ) );
    addSubFormHeader ( "cifproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "C.I.F. proveedor" ) );
    addSubFormHeader ( "telproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Telefono proveedor" ) );
    addSubFormHeader ( "emailproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Email proveedor" ) );
    addSubFormHeader ( "fechapago", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha de pago" ) );
    addSubFormHeader ( "cantpago", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Cantidad" ) );
    addSubFormHeader ( "nombanco", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Banco" ) );
    addSubFormHeader ( "refpago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Referencia de pago" ) );
    addSubFormHeader ( "previsionpago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Prevision pago" ) );
    addSubFormHeader ( "comentpago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Comentario pago" ) );
    addSubFormHeader ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Id trabajador" ) );
    addSubFormHeader ( "nomtrabajador", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre de trabajador" ) );
    addSubFormHeader ( "apellidostrabajador", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Apellidos trabajador" ) );
    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
}

PagosListSubForm::~PagosListSubForm()
{}


