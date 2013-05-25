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

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QCheckBox>
#include <QtCore/QFile>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QMenu>
#include <QtCore/QTextStream>

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
PagosList::PagosList ( QWidget *parent, Qt::WindowFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    BL_FUNC_DEBUG
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
    
}


///
/**
\param comp
\param parent
\param flag
**/
PagosList::PagosList ( BfCompany *comp, QWidget *parent, Qt::WindowFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    BL_FUNC_DEBUG
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
    if ( editMode() ) {
        mainCompany() ->insertWindow ( windowTitle(), this );
    } // end if
    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "pago" );
    /// Llamamos a los scripts
    blScript(this);
    
}


PagosList::~PagosList()
{
    BL_FUNC_DEBUG
    
}




void PagosList::presentar()
{
    BL_FUNC_DEBUG
    if ( mainCompany() != NULL ) {
        mui_list->load ( "SELECT * FROM pago NATURAL LEFT JOIN proveedor NATURAL LEFT JOIN trabajador NATURAL LEFT JOIN banco WHERE 1 = 1 " + generaFiltro() );
        /// Hacemos el calculo del total.
        BlFixed total = mui_list->sumarCampo ( "cantpago" );
        m_total->setText ( total.toQString() );
    } // end if
    
}


const QString PagosList::generaFiltro()
{
    BL_FUNC_DEBUG

    QString filtro = "";

    /// Hacemos el filtrado like del campo m_filtro
    filtro += mui_list->likeFilterSQL(m_filtro->text());

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

    return ( filtro );
}





void PagosList::editar ( int )
{
    mdb_idpago = mui_list->dbValue ( "idpago" );
    if ( editMode() && mdb_idpago != "" ) {
        PagoView *bud = new PagoView ( ( BfCompany * ) mainCompany(), 0 );
        if ( bud->load ( mdb_idpago ) ) {
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
    BL_FUNC_DEBUG
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
    BL_FUNC_DEBUG
    printPDF ( _ ( "Pagos a proveedores" ) );
    
}


void PagosList::remove()
{
    BL_FUNC_DEBUG
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Debe seleccionar una linea" ), this );
        return;
    } // end if
    try {
        mdb_idpago = mui_list->dbValue ( "idpago" );
        if ( editMode() && mdb_idpago != "" ) {
            PagoView * bud = new PagoView ( ( BfCompany * ) mainCompany(), NULL );
            bud->load ( mdb_idpago );
            bud->remove();
        } // end if
        presentar();
    } catch ( ... )  {
        blMsgInfo ( _ ( "Error al borrar el pago" ), this );
    } // end try
    
}

void PagosList::setMainCompany ( BfCompany *comp )
{
    BL_FUNC_DEBUG
    BlMainCompanyPointer::setMainCompany ( comp );
    m_proveedor->setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    mui_idbanco->setMainCompany ( comp );
    mui_idbanco->setidbanco ( "" );
    
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
    setInsert ( false );
    setDelete ( false );
    setSortingEnabled ( true );
}

PagosListSubForm::~PagosListSubForm()
{}


