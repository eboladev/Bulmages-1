/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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

#include <QLineEdit>
#include <QTextStream>
#include <QFileDialog>
#include <QCheckBox>
#include <QMessageBox>

#include "contratoslist.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "blimportexport.h"
#include "bfinformereferencia.h"
#include "blplugins.h"
#include "contratoview.h"


/** Inicializa todos los componentes del listado.
    Hace una presentacion inicial.
    Mete la ventana en el workSpace si estamos en modo edicion.
*/
/**
\param comp
\param parent
\param flag
\param editmode
\return
**/
ContratosList::ContratosList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmode )
        : BlFormList ( comp, parent, flag ), BlImportExport ( comp )
{
    _depura ( "ContratosList::ContratosList", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ContratosList_ContratosList", this );
    if ( res != 0 )
        return;

    mui_list->setMainCompany ( comp );
    mui_idcliente->setMainCompany ( comp );
    mdb_idcontrato = "";
    mdb_nomcontrato = "";
    /// Establecemos los parametros de busqueda del Cliente
    mui_idcliente->setLabel ( _ ( "Cliente:" ) );
    mui_idcliente->setTableName ( "cliente" );
    mui_idcliente->m_valores["cifcliente"] = "";
    mui_idcliente->m_valores["nomcliente"] = "";
    hideBusqueda();
    /// Si estamos en el modo edici&oacute;n metemos la ventana en el workSpace.
    if ( modoEdicion()) {
        mainCompany() ->meteWindow ( windowTitle(), this );
    } else {
        setWindowTitle ( _ ( "Selector de contratos" ) );
        mui_editar->setHidden ( TRUE );
        mui_crear->setHidden ( TRUE );
        mui_borrar->setHidden ( TRUE );
        mui_imprimir->setHidden ( TRUE );
    } // end if
    presenta();
    trataPermisos ( "contrato" );
    _depura ( "END ContratosList::ContratosList", 0 );
}


void ContratosList::borrar() {
    ///mensajeInfo("Estoy en borrar");

    _depura ( "ContratosList::on_mui_borrar_released", 0 );
    try {
        QString idcontrato = mui_list->dbValue ( "idcontrato" );
        ContratoView *cont = new ContratoView ( ( BfCompany * ) mainCompany(), 0 ) ;
        if ( cont->cargar ( idcontrato ) ) {
            delete cont;
            throw - 1;
        } // end if
        cont->on_mui_borrar_released();
        delete cont;
        presentar();
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al borrar un contrato" ) );
    } // end try
    _depura ( "END:ContratosList::on_mui_borrar_released", 0 );

}

void ContratosList::imprimir()
{
    //mensajeInfo("Estoy en imprimir");
    
    _depura ( "ContratosList::on_mui_imprimir_released", 0 );
    mui_list->imprimirPDF ( _ ( "Listado de Contratos" ) );
    _depura ( "END ContratosList::on_mui_imprimir_released", 0 );
}

void ContratosList::presentar()
{
    _depura ("ContratosList::presenta", 0 );
    mui_list->cargar ( "SELECT * FROM contrato ORDER BY idcontrato " );
    _depura ("END ContratosList::presenta", 0 );
}

/** No requiere acciones especiales en el destructor de la clase.
*/
/**
**/
ContratosList::~ContratosList()
{
    _depura ( "ContratosList::~ContratosList", 0 );
    mainCompany() ->sacaWindow ( this );
    _depura ( "END ContratosList::~ContratosList", 0 );
}


/** Todo el listado de contratos lo presenta el Subformulario mui_list del tipo ListLinCliente.
*/
/// \TODO: Mejorar el sistema de filtrado incluyendo una funcion de generar Filtro.
/**
**/
void ContratosList::presenta()
{
    _depura ( "ContratosList::presenta", 0 );
    QString where;

    if ( mui_idcliente->id() != "" ) {
        where = " AND contrato.idcliente = " + mui_idcliente->id();
    } // end if

    mui_list->cargar ( "SELECT * FROM contrato NATURAL LEFT JOIN cliente  WHERE nomcontrato LIKE '%" + m_findClient->text() + "%' " + where + " ORDER BY nomcontrato" );
    _depura ( "END ContratosList::presenta", 0 );
}


/** LA accion de editar tiene una doble vertiente dependiendo del modo en que se ha abierto la ventana.
    Modo Edicion:
 - Crea una instancia de ClienteView, lo inicializa y lo presenta.
    Modo Seleccion:
 - Establece cual es el elemento seleccionado y se cierra la ventana para devolver el control.
*/
/**
\return
**/
void ContratosList::editar ( int row )
{

    _depura ( "ContratosList::editar", 0 );
    mdb_idcontrato = mui_list->dbValue ( "idcontrato", row );
    mdb_refcontrato = mui_list->dbValue ( "refcontrato", row );
    mdb_nomcontrato = mui_list->dbValue ( "nomcontrato", row );
    if ( modoEdicion() ) {
        ContratoView * prov = new ContratoView ( ( BfCompany * ) mainCompany() );
        if ( prov->cargar ( mdb_idcontrato ) ) {
            delete prov;
            return;
        } // end if
        mainCompany() ->m_pWorkspace->addWindow ( prov );
        prov->show();
    } else {
        emit ( selected ( mdb_idcontrato ) );
    } // end if
    _depura ( "END ContratosList::editar", 0 );

}




/// Devuelve el identificador del contrato seleccionado
/**
\return
**/
QString ContratosList::idcontrato()
{
    _depura ( "ContratosList::idcontrato", 0 );
    _depura ( "END ContratosList::idcontrato", 0 );
    return mdb_idcontrato;
}

/// Devuelve el nombre del contrato seleccionado
/**
\return
**/
QString ContratosList::nomcontrato()
{
    _depura ( "ContratosList::nomcontrato", 0 );
    _depura ( "END ContratosList::nomcontrato", 0 );
    return mdb_nomcontrato;
}


/// Devuelve el cif del contrato seleccionado
/**
\return
**/
QString ContratosList::refcontrato()
{
    _depura ( "ContratosList::refcontrato", 0 );
    _depura ( "END ContratosList::refcontrato", 0 );
    return mdb_refcontrato;
}




/// SLOT automatico que se ejecuta al cambiar el texto del QLineEdit de filtrado general
/**
\param text
**/
void ContratosList::on_m_filtro_textChanged ( const QString &text )
{
    _depura ( "ContratosList::on_m_filtro_textChanged", 0 );
    if ( text.size() >= 3 ) {
        on_mui_actualizar_released();
    } // end if
    _depura ( "END ContratosList::on_m_filtro_textChanged", 0 );
}


/// SLOT automatico que se ejecuta al pulsar sobre el boton de crear en la botonera
/**
**/
void ContratosList::on_mui_crear_released()
{
    _depura ( "ContratosList::on_mui_crear_released", 0 );
    ContratoView *prov = new ContratoView ( ( BfCompany * ) mainCompany() );
    prov->cargar ( "0" );
    mainCompany() ->m_pWorkspace->addWindow ( prov );
    prov->show();
    _depura ( "END ContratosList::on_mui_crear_released", 0 );
}


/// SLOT automatico que se ejecuta al pulsar sobre el boton de actualizar en la botonera
/**
**/
void ContratosList::on_mui_actualizar_released()
{
    _depura ( "ContratosList::on_mui_actualizar_released", 0 );
    presenta();
    _depura ( "END ContratosList::on_mui_actualizar_released", 0 );
}

/// SLOT automatico que se ejecuta al pulsar sobre el boton configurar en la botonera
/**
\param checked
**/
void ContratosList::on_mui_configurar_toggled ( bool checked )
{
    _depura ( "ContratosList::on_mui_configurar_toggled", 0 );
    if ( checked ) {
        mui_list->showConfig();
    } else {
        mui_list->hideConfig();
    } // end if
    _depura ( "END ContratosList::on_mui_configurar_toggled", 0 );
}


/// SLOT automatico que se ejecuta al pulsar sobre el boton de facturar en la botonera
/** Realiza la facturacion pendiente de todos los contratos indicados.
**/
void ContratosList::on_mui_facturar_released()
{
    _depura ( "ContratosList::on_mui_facturar", 0 );
    BlDbRecordSet *cur = NULL;
    try {
        QString where;

        if ( mui_idcliente->id() != "" ) {
            where = " AND contrato.idcliente = " + mui_idcliente->id();
        } // end if

        cur = mainCompany() ->loadQuery ( "SELECT * FROM contrato NATURAL LEFT JOIN cliente  WHERE nomcontrato LIKE '%" + m_findClient->text() + "%' " + where + " ORDER BY nomcontrato" );
        while ( !cur->eof() ) {
            ContratoView * prov = new ContratoView ( ( BfCompany * ) mainCompany() );
            if ( prov->cargar ( cur->valor ( "idcontrato" ) ) ) {
                delete prov;
            } // end if
//        mainCompany() ->m_pWorkspace->addWindow ( prov );
            prov->on_mui_facturar_released();
            delete prov;

            cur->nextRecord();
        } // end while
        delete cur;

    } catch ( ... ) {
        mensajeInfo ( "Error inesperado" );
        if ( cur ) delete cur;
        return;
    } // end try
    _depura ( "END ContratosList::on_mui_facturar", 0 );
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** PRepara el subformulario para trabajar con la tabla contrato.
    Establece que no se puedan insertar ni borrar lineas.
*/
/**
\param parent
**/
ContratosListSubform::ContratosListSubform ( QWidget *parent, const char * ) : BfSubForm ( parent )
{
    _depura ( "ContratosListSubform::ContratosListSubform", 0 );
    setDbTableName ( "contrato" );
    setDbFieldId ( "idcontrato" );
    addSubFormHeader ( "idcontrato", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID contrato" ) );
    addSubFormHeader ( "codcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Codigo" ) );
    addSubFormHeader ( "cifcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "C.I.F." ) );
    addSubFormHeader ( "refcontrato", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Referencia" ) );
    addSubFormHeader ( "nomcontrato", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre de contrato" ) );
    addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre alternativo del contrato" ) );
    addSubFormHeader ( "fincontrato", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha de alta del contrato" ) );
    addSubFormHeader ( "ffincontrato", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha de baja del contrato" ) );
    addSubFormHeader ( "periodicidadcontrato", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha de baja del contrato" ) );
    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    _depura ( "END ContratosListSubform::ContratosListSubform", 0 );
}


///
/**
**/
ContratosListSubform::~ContratosListSubform()
{
    _depura ( "ContratosListSubform::~ContratosListSubform", 0 );
    _depura ( "END ContratosListSubform::~ContratosListSubform", 0 );
}
