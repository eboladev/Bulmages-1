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
#include <QTextStream>

#include "presupuestolist.h"
#include "bfcompany.h"
#include "presupuestoview.h"
#include "blfunctions.h"
#include "blplugins.h"
#include "blconfiguration.h"


///
/**
\param parent
\param flag
\return
**/
PresupuestoList::PresupuestoList ( QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    blDebug ( Q_FUNC_INFO, 0 );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "PresupuestoList_PresupuestoList", this );
    if ( res != 0 )
        return;
    m_idpresupuesto = "";
    setSubForm ( mui_list );
    /// Establecemos los parametros de busqueda del Cliente
    m_cliente->setLabel ( _ ( "Cliente:" ) );
    m_cliente->setTableName ( "cliente" );
    m_cliente->m_valores["cifcliente"] = "";
    m_cliente->m_valores["nomcliente"] = "";

    hideBusqueda();
    iniciaForm();
    /// Llamamos a los scripts
    blScript(this);
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param comp
\param parent
\param flag
\return
**/
PresupuestoList::PresupuestoList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    blDebug ( Q_FUNC_INFO, 0 );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "PresupuestoList_PresupuestoList", this );
    if ( res != 0 )
        return;
    m_cliente->setMainCompany ( comp );
    m_articulo->setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    setSubForm ( mui_list );
    /// Establecemos los parametros de busqueda del Cliente
    m_cliente->setLabel ( _ ( "Cliente:" ) );
    m_cliente->setTableName ( "cliente" );
    m_cliente->m_valores["cifcliente"] = "";
    m_cliente->m_valores["nomcliente"] = "";
    iniciaForm();
    
    /// Cargamos los filtros guardados.
    cargaFiltrosXML();
    
    presentar();
    m_idpresupuesto = "";
    if ( editMode() ) {
        mainCompany() ->insertWindow ( windowTitle(), this );
    } // end if
    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "presupuesto" );
    /// Llamamos a los scripts
    blScript(this);
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\return
**/
void PresupuestoList::iniciaForm()
{
    blDebug ( Q_FUNC_INFO, 0 );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "PresupuestoList_iniciaForm", this );
    if ( res != 0 )
        return;
    mui_procesada->insertItem ( 0, _ ( "Todos los presupuestos" ) );
    mui_procesada->insertItem ( 1, _ ( "Presupuestos procesados" ) );
    mui_procesada->insertItem ( 2, _ ( "Presupuestos no procesados" ) );
    mui_procesada->setCurrentIndex ( 2 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
PresupuestoList::~PresupuestoList()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\return
**/
QString PresupuestoList::idpresupuesto()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return m_idpresupuesto;
}


///
/**
\param comp
**/
void PresupuestoList::setMainCompany ( BfCompany *comp )
{
    blDebug ( Q_FUNC_INFO, 0 );
    BlMainCompanyPointer::setMainCompany ( comp );
    m_cliente->setMainCompany ( comp );
    m_articulo->setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param val
**/
void PresupuestoList::setidcliente ( QString val )
{
    blDebug ( Q_FUNC_INFO, 0 );
    m_cliente->setId ( val );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param val
**/
void PresupuestoList::setidarticulo ( QString val )
{
    blDebug ( Q_FUNC_INFO, 0 );
    m_articulo->setidarticulo ( val );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
void PresupuestoList::crear()
{
    blDebug ( Q_FUNC_INFO, 0 );
    PresupuestoView *pv = new PresupuestoView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany()->m_pWorkspace->addSubWindow ( pv );
    pv->inicializar();
    pv->pintar();
    
    /// Si se crea el nuevo presupuesto desde la lista de presupuestos de un cliente,
    /// entonces se crea el presupuesto para este cliente.
    if (m_cliente->id() != "") {
	pv->mui_idcliente->setId( m_cliente->id() );
    } // end if
    
    pv->show();
    pv->mui_descpresupuesto->setFocus ( Qt::OtherFocusReason );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
void PresupuestoList::presentar()
{
    blDebug ( Q_FUNC_INFO, 0 );

    /// Hacemos el listado y lo presentamos.
    mui_list->load ( "SELECT *, totalpresupuesto AS total, bimppresupuesto AS base, imppresupuesto AS impuestos FROM presupuesto LEFT JOIN  cliente ON presupuesto.idcliente=cliente.idcliente LEFT JOIN almacen ON presupuesto.idalmacen=almacen.idalmacen WHERE 1=1 " + generaFiltro() );

    /// Hacemos el calculo del total.
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT SUM(totalpresupuesto) AS total FROM presupuesto LEFT JOIN cliente ON presupuesto.idcliente=cliente.idcliente LEFT JOIN almacen ON presupuesto.idalmacen=almacen.idalmacen WHERE 1=1 " + generaFiltro() );
    /// Si por un problema de permisos este query devuelve NULL debe contemplarse el caso.
    if ( cur ) {
        m_total->setText ( cur->value( "total" ) );
        delete cur;
    } // end if

    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\return
**/
QString PresupuestoList::generaFiltro()
{
    blDebug ( Q_FUNC_INFO, 0 );
    /// Tratamiento de los filtros.
    QString filtro = "";

    if ( m_filtro->text() != "" ) {
        filtro = " AND ( lower(descpresupuesto) LIKE lower('%" + m_filtro->text() + "%') ";
        filtro += " OR refpresupuesto LIKE '" + m_filtro->text() + "%' ";
        filtro += " OR lower(nomcliente) LIKE lower('%" + m_filtro->text() + "%')) ";
    } // end if

    if ( m_cliente->id() != "" ) {
        filtro += " AND presupuesto.idcliente = " + m_cliente->id();
    } // end if

    /// Tratamos los elementos procesados y no procesados.
    if ( mui_procesada->currentIndex() == 1 ) {
        /// Muestra solo las procesadas.
        filtro += " AND procesadopresupuesto";
    } else if ( mui_procesada->currentIndex() == 2 ) {
        /// Muestra solo las NO procesadas.
        filtro += " AND NOT procesadopresupuesto ";
    } // end if

    if ( m_articulo->idarticulo() != "" ) {
        filtro += " AND idpresupuesto IN (SELECT DISTINCT idpresupuesto FROM lpresupuesto WHERE idarticulo='" + m_articulo->idarticulo() + "')";
    }// end if

    if ( m_fechain->text() != "" )
        filtro += " AND fpresupuesto >= '" + m_fechain->text() + "' ";
    if ( m_fechafin->text() != "" )
        filtro += " AND fpresupuesto <= '" + m_fechafin->text() + "' ";
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return ( filtro );
}


///
/**
\param row
\return
**/
void PresupuestoList::editar ( int row )
{
    blDebug ( Q_FUNC_INFO, 0 );
    try {
        m_idpresupuesto = mui_list->dbValue ( QString ( "idpresupuesto" ), row );
        if ( editMode() ) {
            PresupuestoView * prov = new PresupuestoView ( ( BfCompany * ) mainCompany() , 0 );
            if ( prov->load ( m_idpresupuesto ) ) {
                delete prov;
                return;
            }
            mainCompany() ->m_pWorkspace->addSubWindow ( prov );
            prov->show();
        } else {
            emit ( selected ( m_idpresupuesto ) );
        } // end if
        blDebug ( ("END ", Q_FUNC_INFO), 0 );
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al editar el presupuesto" ), this );
    } // end try
}


///
/**
\return
**/
void PresupuestoList::imprimir()
{
    blDebug ( Q_FUNC_INFO, 0 );
    mui_list->printPDF ( _ ( "Presupuestos a clientes" ) );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\return
**/
void PresupuestoList::remove()
{
    blDebug ( Q_FUNC_INFO, 0 );
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Debe seleccionar una linea" ), this );
        return;
    } // end if
    try {
        m_idpresupuesto = mui_list->dbValue ( QString ( "idpresupuesto" ) );
        if ( editMode() ) {
            PresupuestoView * pv = new PresupuestoView ( ( BfCompany * ) mainCompany(), 0 );
            if ( pv->load ( m_idpresupuesto ) )
                throw - 1;
            pv->on_mui_borrar_clicked();
            pv->close();
        } // end if
        presentar();
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al borrar el presupuesto" ), this );
    } // end try
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
///
/**
\param parent
\return
**/
PresupuestoListSubForm::PresupuestoListSubForm ( QWidget *parent, const char * ) : BfSubForm ( parent )
{
    blDebug ( Q_FUNC_INFO, 0 );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "PresupuestoListSubForm_PresupuestoListSubForm", this );
    if ( res != 0 ) {
        blDebug ( ("END ", Q_FUNC_INFO), 0 );
        return;
    } // end if
    setDbTableName ( "presupuesto" );
    setDbFieldId ( "idpresupuesto" );
    addSubFormHeader ( "idpresupuesto", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID presupuesto" ) );
    addSubFormHeader ( "numpresupuesto", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Presupuesto" ) );
    addSubFormHeader ( "fpresupuesto", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha" ) );
    addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Cliente" ) );
    addSubFormHeader ( "refpresupuesto", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Referencia" ) );
    addSubFormHeader ( "base", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Base Imponible" ) );
    addSubFormHeader ( "impuestos", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Impuestos" ) );
    addSubFormHeader ( "total", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Total" ) );
    addSubFormHeader ( "descpresupuesto", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Descripcion" ) );
    addSubFormHeader ( "contactpresupuesto", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Persona de contacto" ) );
    addSubFormHeader ( "telpresupuesto", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Telefono" ) );
    addSubFormHeader ( "comentpresupuesto", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Comentario" ) );
    addSubFormHeader ( "codigoalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Almacen" ) );
    addSubFormHeader ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "ID trabajador" ) );
    addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "ID cliente" ) );
    addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "ID almacen" ) );
    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    /// Disparamos los plugins.
    g_plugins->lanza ( "PresupuestoListSubForm_PresupuestoListSubForm_Post", this );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return;
}


///
/**
**/
void PresupuestoListSubForm::load()
{
    blDebug ( Q_FUNC_INFO, 0 );
    QString SQLQuery = "SELECT * FROM presupuesto";
    BlSubForm::load ( SQLQuery );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param query
**/
void PresupuestoListSubForm::load ( QString query )
{
    blDebug ( Q_FUNC_INFO, 0, QString("Consulta: '$1'").arg(query) );
    BlSubForm::load ( query );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}
