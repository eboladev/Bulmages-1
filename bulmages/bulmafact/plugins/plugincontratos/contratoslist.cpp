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
#include "company.h"
#include "funcaux.h"
#include "pgimportfiles.h"
#include "informereferencia.h"
#include "plugins.h"
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
ContratosList::ContratosList ( Company *comp, QWidget *parent, Qt::WFlags flag, edmode editmode )
        : BlFormList ( comp, parent, flag ), pgimportfiles ( comp )
{
    _depura ( "ContratosList::ContratosList", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ContratosList_ContratosList", this );
    if ( res != 0 )
        return;

    mui_list->setEmpresaBase ( comp );
    mui_idcliente->setEmpresaBase ( comp );
    mdb_idcontrato = "";
    mdb_nomcontrato = "";
    m_modo = editmode;
		/// Establecemos los parametros de busqueda del Cliente
    mui_idcliente->setLabel ( _( "Cliente:" ) );
	mui_idcliente->setTableName( "cliente" );
	mui_idcliente->m_valores["cifcliente"] = "";
	mui_idcliente->m_valores["nomcliente"] = "";
    hideBusqueda();
    /// Si estamos en el modo edici&oacute;n metemos la ventana en el workSpace.
    if ( m_modo == EditMode ) {
        empresaBase() ->meteWindow ( windowTitle(), this );
    } else {
        setWindowTitle ( _( "Selector de contratos" ) );
        mui_editar->setHidden ( TRUE );
        mui_crear->setHidden ( TRUE );
        mui_borrar->setHidden ( TRUE );
        mui_imprimir->setHidden ( TRUE );
    } // end if
    presenta();
    trataPermisos ( "contrato" );
    _depura ( "END ContratosList::ContratosList", 0 );
}


/** No requiere acciones especiales en el destructor de la clase.
*/
/**
**/
ContratosList::~ContratosList()
{
    _depura ( "ContratosList::~ContratosList", 0 );
    empresaBase() ->sacaWindow ( this );
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
    mdb_idcontrato = mui_list->DBvalue ( "idcontrato", row );
    mdb_refcontrato = mui_list->DBvalue ( "refcontrato", row );
    mdb_nomcontrato = mui_list->DBvalue ( "nomcontrato", row );
    if ( m_modo == 0 ) {
        ContratoView * prov = new ContratoView ( ( Company * ) empresaBase() );
        if ( prov->cargar ( mdb_idcontrato ) ) {
            delete prov;
            return;
        } // end if
        empresaBase() ->m_pWorkspace->addWindow ( prov );
        prov->show();
    } else {
        emit ( selected ( mdb_idcontrato ) );
    } // end if
    _depura ( "END ContratosList::editar", 0 );

}


/** SLOT que responde a la pulsacion del boton editar.
    Comprueba que existe un elemento seleccionado y llama al metodo editar().
*/
/**
\return
**/
void ContratosList::on_mui_editar_clicked()
{
    _depura ( "ContratosList::on_mui_editar_clicked", 0 );
    if ( mui_list->currentRow() < 0 ) {
        _depura ( "Debe seleccionar un elemento", 2 );
        return;
    } // end if
    editar ( mui_list->currentRow() );
    _depura ( "END ContratosList::on_mui_editar_clicked", 0 );
}


/** SLOT que responde a la pulsacion del boton imprimir.
    La impresion de listados esta completamente delegada a BlSubForm
*/
/**
**/
void ContratosList::on_mui_imprimir_clicked()
{
    _depura ( "ContratosList::on_mui_imprimir_clicked", 0 );
    mui_list->imprimirPDF ( _( "Listado de Contratos" ) );
    _depura ( "ContratosList::on_mui_imprimir_clicked", 0 );
}




/** SLOT que responde a la pulsacion del boton borrar.
    Instancia la clase ClienteView, lo inicializa con el contrato seleccionado y le lanza el evento de borrar.
    Esta es la forma adecuada de borrar desde el listado ya que asi preservamos el tema plugins.
*/
/**
**/
void ContratosList::on_mui_borrar_clicked()
{
    _depura ( "ContratosList::on_mui_borrar_clicked", 0 );
    try {
        /*
                QString idcontrato = mui_list->DBvalue("idcontrato");
                ClienteView *cli = empresaBase()->newClienteView();
                if (cli->cargar(idcontrato)) {
                    delete cli;
                    throw -1;
                } // end if
                cli->on_mui_borrar_clicked();
                delete cli;
                presenta();
        */
    } catch ( ... ) {
        mensajeInfo ( _( "Error al borrar un contrato" ) );
    } // end try
    _depura ( "END:ContratosList::on_mui_borrar_clicked", 0 );
}


/// Establece el modo de funcionamiento como selector para esta ventana
/**
**/
void ContratosList::selectMode()
{
    _depura ( "ContratosList::selectMode", 0 );
    m_modo = SelectMode;
    _depura ( "END ContratosList::selectMode", 0 );
}

/// Establece el modo de funcionamiento como selector para edicion para esta ventana
/**
**/
void ContratosList::editMode()
{
    _depura ( "ContratosList::editMode", 0 );
    m_modo = EditMode;
    _depura ( "END ContratosList::editMode", 0 );
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


/// Oculta la botonera
/**
**/
void ContratosList::hideBotonera()
{
    _depura ( "ContratosList::hideBotonera", 0 );
    m_botonera->hide();
    _depura ( "END ContratosList::hideBotonera", 0 );
}


/// Muestra la botonera
/**
**/
void ContratosList::showBotonera()
{
    _depura ( "ContratosList::showBotonera", 0 );
    m_botonera->show();
    _depura ( "END ContratosList::showBotonera", 0 );
}


/// Oculta el layer de busqueda
/**
**/
void ContratosList::hideBusqueda()
{
    _depura ( "ContratosList::hideBusqueda", 0 );
    m_busqueda->hide();
    _depura ( "END ContratosList::hideBusqueda", 0 );
}


/// Muestra el layer de busqueda
/**
**/
void ContratosList::showBusqueda()
{
    _depura ( "ContratosList::showBusqueda", 0 );
    m_busqueda->show();
    _depura ( "END ContratosList::showBusqueda", 0 );
}


/// SLOT automatico que se ejecuta al cambiar el texto del QLineEdit de filtrado general
/**
\param text
**/
void ContratosList::on_m_filtro_textChanged ( const QString &text )
{
    _depura ( "ContratosList::on_m_filtro_textChanged", 0 );
    if ( text.size() >= 3 ) {
        on_mui_actualizar_clicked();
    } // end if
    _depura ( "END ContratosList::on_m_filtro_textChanged", 0 );
}

/// SLOT automatico que se ejecuta al hacer doble click sobre un elemento determinado de la lista
/**
**/
void ContratosList::on_mui_list_itemDoubleClicked ( QTableWidgetItem * )
{
    _depura ( "ContratosList::on_mui_list_itemDoubleClicked", 0 );
    on_mui_editar_clicked();
    _depura ( "END ContratosList::on_mui_list_itemDoubleClicked", 0 );
}

/// SLOT automatico que se ejecuta al pulsar sobre el boton de crear en la botonera
/**
**/
void ContratosList::on_mui_crear_clicked()
{
    _depura ( "ContratosList::on_mui_crear_clicked", 0 );
    ContratoView *prov = new ContratoView ( ( Company * ) empresaBase() );
    prov->cargar ( "0" );
    empresaBase() ->m_pWorkspace->addWindow ( prov );
    prov->show();
    _depura ( "END ContratosList::on_mui_crear_clicked", 0 );
}


/// SLOT automatico que se ejecuta al pulsar sobre el boton de actualizar en la botonera
/**
**/
void ContratosList::on_mui_actualizar_clicked()
{
    _depura ( "ContratosList::on_mui_actualizar_clicked", 0 );
    presenta();
    _depura ( "END ContratosList::on_mui_actualizar_clicked", 0 );
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
void ContratosList::on_mui_facturar_clicked()
{
    _depura ( "ContratosList::on_mui_facturar", 0 );
    cursor2 *cur = NULL;
    try {
        QString where;

        if ( mui_idcliente->id() != "" ) {
            where = " AND contrato.idcliente = " + mui_idcliente->id();
        } // end if

        cur = empresaBase() ->cargacursor ( "SELECT * FROM contrato NATURAL LEFT JOIN cliente  WHERE nomcontrato LIKE '%" + m_findClient->text() + "%' " + where + " ORDER BY nomcontrato" );
        while ( !cur->eof() ) {
            ContratoView * prov = new ContratoView ( ( Company * ) empresaBase() );
            if ( prov->cargar ( cur->valor ( "idcontrato" ) ) ) {
                delete prov;
            } // end if
//        empresaBase() ->m_pWorkspace->addWindow ( prov );
            prov->on_mui_facturar_clicked();
            delete prov;

            cur->siguienteregistro();
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
ContratosListSubform::ContratosListSubform ( QWidget *parent, const char * ) : SubForm2Bf ( parent )
{
    _depura ( "ContratosListSubform::ContratosListSubform", 0 );
    setDBTableName ( "contrato" );
    setDBCampoId ( "idcontrato" );
    addSHeader ( "idcontrato", BlDbField::DBint, BlDbField::DBNotNull | BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "ID contrato" ) );
    addSHeader ( "codcliente", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Codigo" ) );
    addSHeader ( "cifcliente", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "C.I.F." ) );
    addSHeader ( "refcontrato", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Referencia" ) );
    addSHeader ( "nomcontrato", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Nombre de contrato" ) );
    addSHeader ( "nomcliente", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Nombre alternativo del contrato" ) );
    addSHeader ( "fincontrato", BlDbField::DBdate, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Fecha de alta del contrato" ) );
    addSHeader ( "ffincontrato", BlDbField::DBdate, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Fecha de baja del contrato" ) );
    addSHeader ( "periodicidadcontrato", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Fecha de baja del contrato" ) );
    setinsercion ( FALSE );
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
