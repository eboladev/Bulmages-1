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

#include <QLineEdit>
#include <QTextStream>
#include <QFileDialog>
#include <QCheckBox>
#include <QMessageBox>

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
CarteraCobrosList::CarteraCobrosList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmode )
        : BlFormList ( comp, parent, flag ), BlImportExport ( comp )
{
    _depura ( "CarteraCobrosList::CarteraCobrosList", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "CarteraCobrosList_CarteraCobrosList", this );
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

    m_modo = editmode;
    hideBusqueda();
    /// Si estamos en el modo edici&oacute;n metemos la ventana en el workSpace.
    if ( m_modo == EditMode ) {
        mainCompany() ->meteWindow ( windowTitle(), this );
    } else {
        setWindowTitle ( tr ( "Selector de vencimientocs" ) );
        mui_editar->setHidden ( TRUE );
        mui_borrar->setHidden ( TRUE );
        mui_imprimir->setHidden ( TRUE );
    } // end if
    presenta();
    trataPermisos ( "vencimientoc" );
    _depura ( "END CarteraCobrosList::CarteraCobrosList", 0 );
}


/** No requiere acciones especiales en el destructor de la clase.
*/
/**
**/
CarteraCobrosList::~CarteraCobrosList()
{
    _depura ( "CarteraCobrosList::~CarteraCobrosList", 0 );
    mainCompany() ->sacaWindow ( this );
    _depura ( "END CarteraCobrosList::~CarteraCobrosList", 0 );
}


/** Todo el listado de vencimientocs lo presenta el Subformulario mui_list del tipo ListLinCliente.
*/
/// \TODO: Mejorar el sistema de filtrado incluyendo una funcion de generar Filtro.
/**
**/
void CarteraCobrosList::presenta()
{
    _depura ( "CarteraCobrosList::presenta", 0 );
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


    mui_list->cargar ( "SELECT * FROM vencimientoc LEFT JOIN factura AS t1 ON vencimientoc.idfactura = t1.idfactura LEFT JOIN cliente AS t2 ON t2.idcliente = vencimientoc.idcliente LEFT JOIN forma_pago  AS t3 ON t3.idforma_pago = vencimientoc.idforma_pago  WHERE nomcliente LIKE '%" + m_texto->text() + "%' " + where + " ORDER BY nomcliente" );
    _depura ( "END CarteraCobrosList::presenta", 0 );
}



void CarteraCobrosList::editar ( int )
{
    _depura ( "CarteraCobrosList::editar", 0 );
    mdb_idvencimientoc = mui_list->dbValue ( "idvencimientoc" );
    if ( modoEdicion() && mdb_idvencimientoc != "" ) {
        VencimientoCView *bud = new VencimientoCView( ( BfCompany * ) mainCompany(), 0 );
        if ( bud->cargar ( mdb_idvencimientoc ) ) {
            delete bud;
            return;
        } // end if
        mainCompany() ->m_pWorkspace->addWindow ( bud );
        bud->show();
    } else {
        close();
    } // end if
    _depura ( "END CarteraCobrosList::editar", 0 );
}



/** SLOT que responde a la pulsacion del boton imprimir.
    La impresion de listados esta completamente delegada a SubForm3
*/
/**
**/
void CarteraCobrosList::on_mui_imprimir_clicked()
{
    _depura ( "CarteraCobrosList::on_mui_imprimir_clicked", 0 );
    mui_list->imprimirPDF ( tr ( "BlFormList de Vencimientos" ) );
    _depura ( "CarteraCobrosList::on_mui_imprimir_clicked", 0 );
}


/** SLOT que responde a la pulsacion del boton imprimir.
    La impresion de listados esta completamente delegada a SubForm3
*/
/**
**/
void CarteraCobrosList::on_mui_actualizar_clicked()
{
    _depura ( "CarteraCobrosList::on_mui_imprimir_clicked", 0 );
    presenta();
    _depura ( "CarteraCobrosList::on_mui_imprimir_clicked", 0 );
}

/** SLOT que responde a la pulsacion del boton borrar.
    Instancia la clase ClienteView, lo inicializa con el vencimientoc seleccionado y le lanza el evento de borrar.
    Esta es la forma adecuada de borrar desde el listado ya que asi preservamos el tema plugins.
*/
/**
**/
void CarteraCobrosList::borrar()
{
    _depura ( "CarteraCobrosList::on_mui_borrar_clicked", 0 );
    try {

                QString idvencimientoc = mui_list->dbValue("idvencimientoc");
                VencimientoCView *bud = new VencimientoCView( ( BfCompany * ) mainCompany(), 0 );
                if (bud->cargar(idvencimientoc)) {
                    delete bud;
                    throw -1;
                } // end if
                bud->borrar();
                delete bud;
                presenta();
    } catch ( ... ) {
        mensajeInfo ( tr ( "Error al borrar un vencimientoc" ) );
    } // end try
    _depura ( "END:CarteraCobrosList::on_mui_borrar_clicked", 0 );
}


/// Establece el modo de funcionamiento como selector para esta ventana
/**
**/
void CarteraCobrosList::selectMode()
{
    _depura ( "CarteraCobrosList::selectMode", 0 );
    m_modo = SelectMode;
    _depura ( "END CarteraCobrosList::selectMode", 0 );
}

/// Establece el modo de funcionamiento como selector para edicion para esta ventana
/**
**/
void CarteraCobrosList::editMode()
{
    _depura ( "CarteraCobrosList::editMode", 0 );
    m_modo = EditMode;
    _depura ( "END CarteraCobrosList::editMode", 0 );
}

/// Devuelve el identificador del vencimientoc seleccionado
/**
\return
**/
QString CarteraCobrosList::idvencimientoc()
{
    _depura ( "CarteraCobrosList::idvencimientoc", 0 );
    _depura ( "END CarteraCobrosList::idvencimientoc", 0 );
    return mdb_idvencimientoc;
}



/// Oculta la botonera
/**
**/
void CarteraCobrosList::hideBotonera()
{
    _depura ( "CarteraCobrosList::hideBotonera", 0 );
    m_botonera->hide();
    _depura ( "END CarteraCobrosList::hideBotonera", 0 );
}


/// Muestra la botonera
/**
**/
void CarteraCobrosList::showBotonera()
{
    _depura ( "CarteraCobrosList::showBotonera", 0 );
    m_botonera->show();
    _depura ( "END CarteraCobrosList::showBotonera", 0 );
}


/// Oculta el layer de busqueda
/**
**/
void CarteraCobrosList::hideBusqueda()
{
    _depura ( "CarteraCobrosList::hideBusqueda", 0 );
    m_busqueda->hide();
    _depura ( "END CarteraCobrosList::hideBusqueda", 0 );
}


/// Muestra el layer de busqueda
/**
**/
void CarteraCobrosList::showBusqueda()
{
    _depura ( "CarteraCobrosList::showBusqueda", 0 );
    m_busqueda->show();
    _depura ( "END CarteraCobrosList::showBusqueda", 0 );
}


/// SLOT automatico que se ejecuta al cambiar el texto del QLineEdit de filtrado general
/**
\param text
**/
void CarteraCobrosList::on_m_filtro_textChanged ( const QString &text )
{
    _depura ( "CarteraCobrosList::on_m_filtro_textChanged", 0 );
    if ( text.size() >= 3 ) {
        on_mui_actualizar_clicked();
    } // end if
    _depura ( "END CarteraCobrosList::on_m_filtro_textChanged", 0 );
}



/// SLOT automatico que se ejecuta al pulsar sobre el boton de crear en la botonera
/**
**/
void CarteraCobrosList::crear()
{
    _depura ( "CarteraCobrosList::on_mui_crear_clicked", 0 );
    if ( modoEdicion()  ) {
        VencimientoCView *bud = new VencimientoCView( ( BfCompany * ) mainCompany(), 0 );
        mainCompany() ->m_pWorkspace->addWindow ( bud );
        bud->show();
    } // end if
    _depura ( "END CarteraCobrosList::on_mui_crear_clicked", 0 );
}





/// SLOT automatico que se ejecuta al pulsar sobre el boton configurar en la botonera
/**
\param checked
**/
void CarteraCobrosList::on_mui_configurar_toggled ( bool checked )
{
    _depura ( "CarteraCobrosList::on_mui_configurar_toggled", 0 );
    if ( checked ) {
        mui_list->showConfig();
    } else {
        mui_list->hideConfig();
    } // end if
    _depura ( "END CarteraCobrosList::on_mui_configurar_toggled", 0 );
}

void CarteraCobrosList::on_mui_q34_clicked() {
    Q19View *q34 = new Q19View ( this, ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->pWorkspace() ->addWindow ( q34 );
    q34->show();
}


void CarteraCobrosList::on_mui_suma_clicked() {
	BlFixed a("0.00");
        /// Reseteamos los valores
        for ( int i = 0; i < mui_list->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = mui_list->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "TRUE" ) {
                a = a + BlFixed ( rec->dbValue ( "cantvencimientoc" ) );
            } // end if
        } // end for
	mensajeInfo ("Suma:" + a.toQString());
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
    _depura ( "CarteraCobrosListSubForm::CarteraCobrosListSubForm", 0 );
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
    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    _depura ( "END CarteraCobrosListSubForm::CarteraCobrosListSubForm", 0 );
}


///
/**
**/
CarteraCobrosListSubForm::~CarteraCobrosListSubForm()
{
    _depura ( "CarteraCobrosListSubForm::~CarteraCobrosListSubForm", 0 );
    _depura ( "END CarteraCobrosListSubForm::~CarteraCobrosListSubForm", 0 );
}
