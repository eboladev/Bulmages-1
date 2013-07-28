/***************************************************************************
 *   Copyright (C) 20049 by Tomeu Borrás Riera                             *
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

#include <QtWidgets/QLineEdit>
#include <QtCore/QTextStream>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QMessageBox>

#include "carteracobroslist.h"
#include "blmaincompany.h"
#include "blfunctions.h"
#include "blimportexport.h"
#include "blplugins.h"
#include "q19view.h"
#include "vencimientocview.h"


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
CarteraCobrosList::CarteraCobrosList ( BfCompany *comp, QWidget *parent, Qt::WindowFlags flag, edmode modo )
        : BlFormList ( comp, parent, flag ), BlImportExport ( comp )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->run ( "CarteraCobrosList_CarteraCobrosList", this );
    if ( res != 0 )
        return;

    mui_list->setMainCompany ( comp );
    setSubForm ( mui_list );


    /// Establecemos los parametros de busqueda del Cliente
    mui_idcliente->setMainCompany ( comp );
    mui_idcliente->setLabel ( _ ( "Cliente:" ) );
    mui_idcliente->setTableName ( "cliente" );
    mui_idcliente->m_valores["cifcliente"] = "";
    mui_idcliente->m_valores["nomcliente"] = "";


    mui_idforma_pago->setMainCompany (comp);
    mui_idforma_pago->setId("");
    mui_idforma_pago->setCurrentIndex ( mui_idforma_pago->findText("--") );

    mui_estadovencimientoc->setCurrentIndex ( mui_estadovencimientoc->findText("Pendiente") );

    m_modo = modo;
    hideBusqueda();
    /// Si estamos en el modo edici&oacute;n metemos la ventana en el workSpace.
    if ( m_modo == BL_EDIT_MODE ) {
        mainCompany() ->insertWindow ( windowTitle(), this );
    } else {
        setWindowTitle ( tr ( "Selector de vencimientocs" ) );
        mui_editar->setHidden ( true );
        mui_borrar->setHidden ( true );
        mui_imprimir->setHidden ( true );
    } // end if
    
    /// Cargamos los filtros guardados.
    cargaFiltrosXML();
    
    presenta();
    trataPermisos ( "vencimientoc" );
    /// Llamamos a los scripts
    blScript(this);
    
}


/** No requiere acciones especiales en el destructor de la clase.
*/
/**
**/
CarteraCobrosList::~CarteraCobrosList()
{
    BL_FUNC_DEBUG
    mainCompany() ->removeWindow ( this );
    
}


/** Todo el listado de vencimientocs lo presenta el Subformulario mui_list del tipo ListLinCliente.
*/
/// \TODO: Mejorar el sistema de filtrado incluyendo una funcion de generar Filtro.
/**
**/
void CarteraCobrosList::presenta()
{
    BL_FUNC_DEBUG
    QString where;

    if ( mui_idcliente->id() != "" ) {
        where = " AND t1.idcliente = " + mui_idcliente->id();
    } // end if

    if ( mui_fechainicial->text() != "")
	where += " AND fechavencimientoc >= '"+mui_fechainicial->text()+"'";

    if ( mui_fechafinal->text() != "")
	where += " AND fechavencimientoc <= '"+mui_fechafinal->text()+"'";

    if ( mui_estadovencimientoc->currentText() != "--" )
	where += " AND estadovencimientoc = '" + mui_estadovencimientoc->currentText() + "' ";

    if ( mui_idforma_pago->id() != "")
	where += " AND vencimientoc.idforma_pago = " + mui_idforma_pago->id();


    mui_list->load ( "SELECT * FROM vencimientoc LEFT JOIN factura AS t1 ON vencimientoc.idfactura = t1.idfactura LEFT JOIN cliente AS t2 ON t2.idcliente = vencimientoc.idcliente LEFT JOIN forma_pago  AS t3 ON t3.idforma_pago = vencimientoc.idforma_pago  WHERE nomcliente LIKE '%" + m_texto->text() + "%' " + where + " ORDER BY nomcliente" );
    
}



void CarteraCobrosList::editar ( int )
{
    BL_FUNC_DEBUG
    mdb_idvencimientoc = mui_list->dbValue ( "idvencimientoc" );
    if ( editMode() && mdb_idvencimientoc != "" ) {
        VencimientoCView *bud = new VencimientoCView( ( BfCompany * ) mainCompany(), 0 );
        if ( bud->load ( mdb_idvencimientoc ) ) {
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
    La impresion de listados esta completamente delegada a SubForm3
*/
/**
**/
void CarteraCobrosList::on_mui_imprimir_clicked()
{
    BL_FUNC_DEBUG
    printPDF ( tr ( "BlFormList de Vencimientos" ) );
    
}


/** SLOT que responde a la pulsacion del boton imprimir.
    La impresion de listados esta completamente delegada a SubForm3
*/
/**
**/
void CarteraCobrosList::on_mui_actualizar_clicked()
{
    BL_FUNC_DEBUG
    presenta();
    
}

/** SLOT que responde a la pulsacion del boton borrar.
    Instancia la clase ClienteView, lo inicializa con el vencimientoc seleccionado y le lanza el evento de borrar.
    Esta es la forma adecuada de borrar desde el listado ya que asi preservamos el tema plugins.
*/
/**
**/
void CarteraCobrosList::remove()
{
    BL_FUNC_DEBUG
    try {

                QString idvencimientoc = mui_list->dbValue("idvencimientoc");
                VencimientoCView *bud = new VencimientoCView( ( BfCompany * ) mainCompany(), 0 );
                if (bud->load(idvencimientoc)) {
                    delete bud;
                    throw -1;
                } // end if
                bud->remove();
                delete bud;
                presenta();
    } catch ( ... ) {
        blMsgInfo ( tr ( "Error al borrar un vencimientoc" ) );
    } // end try
    
}


/// Devuelve el identificador del vencimientoc seleccionado
/**
\return
**/
QString CarteraCobrosList::idvencimientoc()
{
    BL_FUNC_DEBUG
    
    return mdb_idvencimientoc;
}



/// Oculta la botonera
/**
**/
void CarteraCobrosList::hideBotonera()
{
    BL_FUNC_DEBUG
    m_botonera->hide();
    
}


/// Muestra la botonera
/**
**/
void CarteraCobrosList::showBotonera()
{
    BL_FUNC_DEBUG
    m_botonera->show();
    
}


/// Oculta el layer de busqueda
/**
**/
void CarteraCobrosList::hideBusqueda()
{
    BL_FUNC_DEBUG
    m_busqueda->hide();
    
}


/// Muestra el layer de busqueda
/**
**/
void CarteraCobrosList::showBusqueda()
{
    BL_FUNC_DEBUG
    m_busqueda->show();
    
}


/// SLOT automatico que se ejecuta al cambiar el texto del QLineEdit de filtrado general
/**
\param text
**/
void CarteraCobrosList::on_m_filtro_textChanged ( const QString &text )
{
    BL_FUNC_DEBUG
    if ( text.size() >= 3 ) {
        on_mui_actualizar_clicked();
    } // end if
    
}



/// SLOT automatico que se ejecuta al pulsar sobre el boton de crear en la botonera
/**
**/
void CarteraCobrosList::crear()
{
    BL_FUNC_DEBUG
    if ( editMode()  ) {
        VencimientoCView *bud = new VencimientoCView( ( BfCompany * ) mainCompany(), 0 );
        mainCompany() ->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if
    
}





/// SLOT automatico que se ejecuta al pulsar sobre el boton configurar en la botonera
/**
\param checked
**/
void CarteraCobrosList::on_mui_configurar_toggled ( bool checked )
{
    BL_FUNC_DEBUG
    if ( checked ) {
        mui_list->showConfig();
    } else {
        mui_list->hideConfig();
    } // end if
    
}

void CarteraCobrosList::on_mui_q34_clicked() {
    Q19View *q34 = new Q19View ( this, ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->pWorkspace() ->addSubWindow ( q34 );
    q34->show();
}


void CarteraCobrosList::on_mui_suma_clicked() {
	BlFixed a("0.00");
        /// Reseteamos los valores
        for ( int i = 0; i < mui_list->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = mui_list->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                a = a + BlFixed ( rec->dbValue ( "cantvencimientoc" ) );
            } // end if
        } // end for
	blMsgInfo ("Suma:" + a.toQString());
}

/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** PRepara el subformulario para trabajar con la tabla vencimientoc.
    Establece que no se puedan insertar ni borrar lineas.
*/
/**
\param parent
**/
CarteraCobrosListSubForm::CarteraCobrosListSubForm ( QWidget *parent, const char * ) : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    setDbTableName ( "vencimientoc" );
    setDbFieldId ( "idvencimientoc" );
    addSubFormHeader ( "idvencimientoc", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, tr ( "Id vencimiento" ) );
    addSubFormHeader ( "selector", BlDbField::DbBoolean, BlDbField::DbNoSave | BlDbField::DbNoLoad, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, "Selector" );
    addSubFormHeader ( "fechavencimientoc", BlDbField::DbDate, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, tr ( "Fecha" ) );
    addSubFormHeader ( "fechaefvencimientoc", BlDbField::DbDate, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, tr ( "Fecha efectiva" ) );
    addSubFormHeader ( "cantvencimientoc", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, tr ( "Cantidad" ) );
    addSubFormHeader ( "cifcliente", BlDbField::DbVarChar, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, tr ( "CIF" ) );
    addSubFormHeader ( "numfactura", BlDbField::DbVarChar, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, tr ( "Factura" ) );
    addSubFormHeader ( "descforma_pago", BlDbField::DbVarChar, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, tr ( "Forma de pago" ) );
    addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, tr ( "Cliente" ) );
    addSubFormHeader ( "estadovencimientoc", BlDbField::DbVarChar, BlDbField::DbNotNull , BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, tr ( "Estado" ) );
    setInsert ( false );
    setDelete ( false );
    setSortingEnabled ( true );
    
}


///
/**
**/
CarteraCobrosListSubForm::~CarteraCobrosListSubForm()
{
    BL_FUNC_DEBUG
    
}
