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

#include "carterapagoslist.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "blimportexport.h"
#include "blplugins.h"
#include "q34view.h"
#include "vencimientopview.h"


/** Inicializa todos los componentes del blformlist.
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
CarteraPagosList::CarteraPagosList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmode )
        : BlFormList ( comp, parent, flag ), BlImportExport ( comp )
{
    _depura ( "CarteraPagosList::CarteraPagosList", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "CarteraPagosList_CarteraPagosList", this );
    if ( res != 0 )
        return;

    mui_list->setMainCompany ( comp );
    setSubForm ( mui_list );
    mui_idproveedor->setMainCompany ( comp );
    mui_idforma_pago->setMainCompany (comp);
    mui_idforma_pago->setId("");
    mui_idforma_pago->setCurrentIndex ( mui_idforma_pago->findText("--") );

    mui_estadovencimientop->setCurrentIndex ( mui_estadovencimientop->findText("Pendiente") );

    m_modo = editmode;
    hideBusqueda();
    /// Si estamos en el modo edici&oacute;n metemos la ventana en el workSpace.
    if ( m_modo == EditMode ) {
        mainCompany() ->meteWindow ( windowTitle(), this );
    } else {
        setWindowTitle ( tr ( "Selector de vencimientops" ) );
        mui_editar->setHidden ( TRUE );
        mui_borrar->setHidden ( TRUE );
        mui_imprimir->setHidden ( TRUE );
    } // end if
    presenta();
    trataPermisos ( "vencimientop" );
    _depura ( "END CarteraPagosList::CarteraPagosList", 0 );
}


/** No requiere acciones especiales en el destructor de la clase.
*/
/**
**/
CarteraPagosList::~CarteraPagosList()
{
    _depura ( "CarteraPagosList::~CarteraPagosList", 0 );
    mainCompany() ->sacaWindow ( this );
    _depura ( "END CarteraPagosList::~CarteraPagosList", 0 );
}


/** Todo el blformlist de vencimientops lo presenta el Subformulario mui_list del tipo ListLinCliente.
*/
/// \TODO: Mejorar el sistema de filtrado incluyendo una funcion de generar Filtro.
/**
**/
void CarteraPagosList::presenta()
{
    _depura ( "CarteraPagosList::presenta", 0 );
    QString where;

    if ( mui_idproveedor->id() != "" ) {
        where = " AND t1.idproveedor = " + mui_idproveedor->id();
    } // end if

    if ( mui_fechainicial->text() != "")
	where += " AND fechavencimientop >= '"+mui_fechainicial->text()+"'";

    if ( mui_fechafinal->text() != "")
	where += " AND fechavencimientop <= '"+mui_fechafinal->text()+"'";

    if ( mui_estadovencimientop->currentText() != "--" )
	where += " AND estadovencimientop = '" + mui_estadovencimientop->currentText() + "' ";

    if ( mui_idforma_pago->id() != "")
	where += " AND vencimientop.idforma_pago = " + mui_idforma_pago->id();


    mui_list->cargar ( "SELECT * FROM vencimientop LEFT JOIN facturap AS t1 ON vencimientop.idfacturap = t1.idfacturap LEFT JOIN proveedor AS t2 ON t2.idproveedor = vencimientop.idproveedor LEFT JOIN forma_pago  AS t3 ON t3.idforma_pago = vencimientop.idforma_pago  WHERE nomproveedor LIKE '%" + m_texto->text() + "%' " + where + " ORDER BY nomproveedor" );
    _depura ( "END CarteraPagosList::presenta", 0 );
}



void CarteraPagosList::editar ( int )
{
    _depura ( "CarteraPagosList::editar", 0 );
    mdb_idvencimientop = mui_list->dbValue ( "idvencimientop" );
    if ( modoEdicion() && mdb_idvencimientop != "" ) {
        VencimientoPView *bud = new VencimientoPView( ( BfCompany * ) mainCompany(), 0 );
        if ( bud->cargar ( mdb_idvencimientop ) ) {
            delete bud;
            return;
        } // end if
        mainCompany() ->m_pWorkspace->addWindow ( bud );
        bud->show();
    } else {
        close();
    } // end if
    _depura ( "END CarteraPagosList::editar", 0 );
}



/** SLOT que responde a la pulsacion del boton imprimir.
    La impresion de blformlists esta completamente delegada a runQueryBlSubForm
*/
/**
**/
void CarteraPagosList::on_mui_imprimir_clicked()
{
    _depura ( "CarteraPagosList::on_mui_imprimir_clicked", 0 );
    mui_list->imprimirPDF ( tr ( "Listado de Vencimientos" ) );
    _depura ( "CarteraPagosList::on_mui_imprimir_clicked", 0 );
}

/** SLOT que responde a la pulsacion del boton imprimir.
    La impresion de blformlists esta completamente delegada a runQueryBlSubForm
*/
/**
**/
void CarteraPagosList::on_mui_actualizar_clicked()
{
    _depura ( "CarteraCobrosList::on_mui_imprimir_clicked", 0 );
    presenta();
    _depura ( "CarteraCobrosList::on_mui_imprimir_clicked", 0 );
}


/** SLOT que responde a la pulsacion del boton borrar.
    Instancia la clase ClienteView, lo inicializa con el vencimientop seleccionado y le lanza el evento de borrar.
    Esta es la forma adecuada de borrar desde el blformlist ya que asi preservamos el tema plugins.
*/
/**
**/
void CarteraPagosList::borrar()
{
    _depura ( "CarteraPagosList::on_mui_borrar_clicked", 0 );
    try {

                QString idvencimientop = mui_list->dbValue("idvencimientop");
	        VencimientoPView *bud = new VencimientoPView( ( BfCompany * ) mainCompany(), 0 );
                if (bud->cargar(idvencimientop)) {
                    delete bud;
                    throw -1;
                } // end if
                bud->borrar();
                delete bud;
                presenta();
    } catch ( ... ) {
        mensajeInfo ( tr ( "Error al borrar un vencimientop" ) );
    } // end try
    _depura ( "END:CarteraPagosList::on_mui_borrar_clicked", 0 );
}


/// Establece el modo de funcionamiento como selector para esta ventana
/**
**/
void CarteraPagosList::selectMode()
{
    _depura ( "CarteraPagosList::selectMode", 0 );
    m_modo = SelectMode;
    _depura ( "END CarteraPagosList::selectMode", 0 );
}

/// Establece el modo de funcionamiento como selector para edicion para esta ventana
/**
**/
void CarteraPagosList::editMode()
{
    _depura ( "CarteraPagosList::editMode", 0 );
    m_modo = EditMode;
    _depura ( "END CarteraPagosList::editMode", 0 );
}

/// Devuelve el identificador del vencimientop seleccionado
/**
\return
**/
QString CarteraPagosList::idvencimientop()
{
    _depura ( "CarteraPagosList::idvencimientop", 0 );
    _depura ( "END CarteraPagosList::idvencimientop", 0 );
    return mdb_idvencimientop;
}



/// Oculta la botonera
/**
**/
void CarteraPagosList::hideBotonera()
{
    _depura ( "CarteraPagosList::hideBotonera", 0 );
    m_botonera->hide();
    _depura ( "END CarteraPagosList::hideBotonera", 0 );
}


/// Muestra la botonera
/**
**/
void CarteraPagosList::showBotonera()
{
    _depura ( "CarteraPagosList::showBotonera", 0 );
    m_botonera->show();
    _depura ( "END CarteraPagosList::showBotonera", 0 );
}


/// Oculta el layer de busqueda
/**
**/
void CarteraPagosList::hideBusqueda()
{
    _depura ( "CarteraPagosList::hideBusqueda", 0 );
    m_busqueda->hide();
    _depura ( "END CarteraPagosList::hideBusqueda", 0 );
}


/// Muestra el layer de busqueda
/**
**/
void CarteraPagosList::showBusqueda()
{
    _depura ( "CarteraPagosList::showBusqueda", 0 );
    m_busqueda->show();
    _depura ( "END CarteraPagosList::showBusqueda", 0 );
}


/// SLOT automatico que se ejecuta al cambiar el texto del QLineEdit de filtrado general
/**
\param text
**/
void CarteraPagosList::on_m_filtro_textChanged ( const QString &text )
{
    _depura ( "CarteraPagosList::on_m_filtro_textChanged", 0 );
    if ( text.size() >= 3 ) {
        on_mui_actualizar_clicked();
    } // end if
    _depura ( "END CarteraPagosList::on_m_filtro_textChanged", 0 );
}



/// SLOT automatico que se ejecuta al pulsar sobre el boton de crear en la botonera
/**
**/
void CarteraPagosList::crear()
{
    _depura ( "CarteraPagosList::on_mui_crear_clicked", 0 );
    if ( modoEdicion()  ) {
        VencimientoPView *bud = new VencimientoPView( ( BfCompany * ) mainCompany(), 0 );
        mainCompany() ->m_pWorkspace->addWindow ( bud );
        bud->show();
    } // end if
    _depura ( "END CarteraPagosList::on_mui_crear_clicked", 0 );
}





/// SLOT automatico que se ejecuta al pulsar sobre el boton configurar en la botonera
/**
\param checked
**/
void CarteraPagosList::on_mui_configurar_toggled ( bool checked )
{
    _depura ( "CarteraPagosList::on_mui_configurar_toggled", 0 );
    if ( checked ) {
        mui_list->showConfig();
    } else {
        mui_list->hideConfig();
    } // end if
    _depura ( "END CarteraPagosList::on_mui_configurar_toggled", 0 );
}

void CarteraPagosList::on_mui_q34_clicked() {
    Q34View *q34 = new Q34View ( this, ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->pWorkspace() ->addWindow ( q34 );
    q34->show();
}


void CarteraPagosList::on_mui_suma_clicked() {
	BlFixed a("0.00");
        /// Reseteamos los valores
        for ( int i = 0; i < mui_list->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = mui_list->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "TRUE" ) {
                a = a + BlFixed ( rec->dbValue ( "cantvencimientop" ) );
            } // end if
        } // end for
	mensajeInfo ("Suma:" + a.toQString());
}

/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** PRepara el subformulario para trabajar con la tabla vencimientop.
    Establece que no se puedan insertar ni borrar lineas.
*/
/**
\param parent
**/
CarteraPagosListSubForm::CarteraPagosListSubForm ( QWidget *parent, const char * ) : BfSubForm ( parent )
{
    _depura ( "CarteraPagosListSubForm::CarteraPagosListSubForm", 0 );
    setDbTableName ( "vencimientop" );
    setDbFieldId ( "idvencimientop" );
    addSubFormHeader ( "idvencimientop", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, tr ( "ID vencimientop" ) );
    addSubFormHeader ( "selector", BlDbField::DbBoolean, BlDbField::DbNoSave | BlDbField::DbNoLoad, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, "selector" );
    addSubFormHeader ( "fechavencimientop", BlDbField::DbDate, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, tr ( "Fecha" ) );
    addSubFormHeader ( "fechaefvencimientop", BlDbField::DbDate, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, tr ( "Fecha Efectiva" ) );
    addSubFormHeader ( "cantvencimientop", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, tr ( "Cantidad" ) );
    addSubFormHeader ( "cifproveedor", BlDbField::DbVarChar, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, tr ( "CIF" ) );
    addSubFormHeader ( "numfacturap", BlDbField::DbVarChar, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, tr ( "Factura" ) );
    addSubFormHeader ( "descforma_pago", BlDbField::DbVarChar, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, tr ( "Forma de Pago" ) );
    addSubFormHeader ( "nomproveedor", BlDbField::DbVarChar, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, tr ( "Proveedor" ) );
    addSubFormHeader ( "estadovencimientop", BlDbField::DbVarChar, BlDbField::DbNotNull , BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, tr ( "Estado" ) );
    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    _depura ( "END CarteraPagosListSubForm::CarteraPagosListSubForm", 0 );
}


///
/**
**/
CarteraPagosListSubForm::~CarteraPagosListSubForm()
{
    _depura ( "CarteraPagosListSubForm::~CarteraPagosListSubForm", 0 );
    _depura ( "END CarteraPagosListSubForm::~CarteraPagosListSubForm", 0 );
}
