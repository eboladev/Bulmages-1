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
CarteraPagosList::CarteraPagosList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode modo )
        : BlFormList ( comp, parent, flag ), BlImportExport ( comp )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->run ( "CarteraPagosList_CarteraPagosList", this );
    if ( res != 0 )
        return;

    mui_list->setMainCompany ( comp );
    setSubForm ( mui_list );
    mui_idproveedor->setMainCompany ( comp );
    mui_idproveedor->setLabel ( _ ( "Proveedor:" ) );
    mui_idproveedor->setTableName ( "proveedor" );
    mui_idproveedor->m_valores["cifproveedor"] = "";
    mui_idproveedor->m_valores["nomproveedor"] = "";
    
    
    mui_idforma_pago->setMainCompany (comp);
    mui_idforma_pago->setId("");
    mui_idforma_pago->setCurrentIndex ( mui_idforma_pago->findText("--") );

    mui_estadovencimientop->setCurrentIndex ( mui_estadovencimientop->findText("Pendiente") );

    m_modo = modo;
    hideBusqueda();
    /// Si estamos en el modo edici&oacute;n metemos la ventana en el workSpace.
    if ( m_modo == BL_EDIT_MODE ) {
        mainCompany() ->insertWindow ( windowTitle(), this );
    } else {
        setWindowTitle ( tr ( "Selector de vencimientops" ) );
        mui_editar->setHidden ( TRUE );
        mui_borrar->setHidden ( TRUE );
        mui_imprimir->setHidden ( TRUE );
    } // end if
    
    /// Cargamos los filtros guardados.
    cargaFiltrosXML();
    
    presenta();
    trataPermisos ( "vencimientop" );
    /// Llamamos a los scripts
    blScript(this);
    
}


/** No requiere acciones especiales en el destructor de la clase.
*/
/**
**/
CarteraPagosList::~CarteraPagosList()
{
    BL_FUNC_DEBUG
    mainCompany() ->removeWindow ( this );
    
}


/** Todo el blformlist de vencimientops lo presenta el Subformulario mui_list del tipo ListLinCliente.
*/
/// \TODO: Mejorar el sistema de filtrado incluyendo una funcion de generar Filtro.
/**
**/
void CarteraPagosList::presenta()
{
    BL_FUNC_DEBUG
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


    mui_list->load ( "SELECT * FROM vencimientop LEFT JOIN facturap AS t1 ON vencimientop.idfacturap = t1.idfacturap LEFT JOIN proveedor AS t2 ON t2.idproveedor = vencimientop.idproveedor LEFT JOIN forma_pago  AS t3 ON t3.idforma_pago = vencimientop.idforma_pago  WHERE nomproveedor LIKE '%" + m_texto->text() + "%' " + where + " ORDER BY nomproveedor" );
    
}



void CarteraPagosList::editar ( int )
{
    BL_FUNC_DEBUG
    mdb_idvencimientop = mui_list->dbValue ( "idvencimientop" );
    if ( editMode() && mdb_idvencimientop != "" ) {
        VencimientoPView *bud = new VencimientoPView( ( BfCompany * ) mainCompany(), 0 );
        if ( bud->load ( mdb_idvencimientop ) ) {
            delete bud;
            return;
        } // end if
        mainCompany() ->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } else {
        close();
    } // end if
    
}



/** SLOT que responde a la pulsacion del boton imprimir.
    La impresion de blformlists esta completamente delegada a runQueryBlSubForm
*/
/**
**/
void CarteraPagosList::on_mui_imprimir_clicked()
{
    BL_FUNC_DEBUG
    mui_list->printPDF ( tr ( "Listado de Vencimientos" ) );
    
}

/** SLOT que responde a la pulsacion del boton imprimir.
    La impresion de blformlists esta completamente delegada a runQueryBlSubForm
*/
/**
**/
void CarteraPagosList::on_mui_actualizar_clicked()
{
    BL_FUNC_DEBUG
    presenta();
    
}


/** SLOT que responde a la pulsacion del boton borrar.
    Instancia la clase ClienteView, lo inicializa con el vencimientop seleccionado y le lanza el evento de borrar.
    Esta es la forma adecuada de borrar desde el blformlist ya que asi preservamos el tema plugins.
*/
/**
**/
void CarteraPagosList::remove()
{
    BL_FUNC_DEBUG
    try {

                QString idvencimientop = mui_list->dbValue("idvencimientop");
	        VencimientoPView *bud = new VencimientoPView( ( BfCompany * ) mainCompany(), 0 );
                if (bud->load(idvencimientop)) {
                    delete bud;
                    throw -1;
                } // end if
                bud->remove();
                delete bud;
                presenta();
    } catch ( ... ) {
        blMsgInfo ( tr ( "Error al borrar un vencimientop" ) );
    } // end try
    
}


/// Devuelve el identificador del vencimientop seleccionado
/**
\return
**/
QString CarteraPagosList::idvencimientop()
{
    BL_FUNC_DEBUG
    
    return mdb_idvencimientop;
}



/// Oculta la botonera
/**
**/
void CarteraPagosList::hideBotonera()
{
    BL_FUNC_DEBUG
    m_botonera->hide();
    
}


/// Muestra la botonera
/**
**/
void CarteraPagosList::showBotonera()
{
    BL_FUNC_DEBUG
    m_botonera->show();
    
}


/// Oculta el layer de busqueda
/**
**/
void CarteraPagosList::hideBusqueda()
{
    BL_FUNC_DEBUG
    m_busqueda->hide();
    
}


/// Muestra el layer de busqueda
/**
**/
void CarteraPagosList::showBusqueda()
{
    BL_FUNC_DEBUG
    m_busqueda->show();
    
}


/// SLOT automatico que se ejecuta al cambiar el texto del QLineEdit de filtrado general
/**
\param text
**/
void CarteraPagosList::on_m_filtro_textChanged ( const QString &text )
{
    BL_FUNC_DEBUG
    if ( text.size() >= 3 ) {
        on_mui_actualizar_clicked();
    } // end if
    
}



/// SLOT automatico que se ejecuta al pulsar sobre el boton de crear en la botonera
/**
**/
void CarteraPagosList::crear()
{
    BL_FUNC_DEBUG
    if ( editMode()  ) {
        VencimientoPView *bud = new VencimientoPView( ( BfCompany * ) mainCompany(), 0 );
        mainCompany() ->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if
    
}





/// SLOT automatico que se ejecuta al pulsar sobre el boton configurar en la botonera
/**
\param checked
**/
void CarteraPagosList::on_mui_configurar_toggled ( bool checked )
{
    BL_FUNC_DEBUG
    if ( checked ) {
        mui_list->showConfig();
    } else {
        mui_list->hideConfig();
    } // end if
    
}

void CarteraPagosList::on_mui_q34_clicked() {
    Q34View *q34 = new Q34View ( this, ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->pWorkspace() ->addSubWindow ( q34 );
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
	blMsgInfo ("Suma:" + a.toQString());
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
    BL_FUNC_DEBUG
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
    
}


///
/**
**/
CarteraPagosListSubForm::~CarteraPagosListSubForm()
{
    BL_FUNC_DEBUG
    
}
