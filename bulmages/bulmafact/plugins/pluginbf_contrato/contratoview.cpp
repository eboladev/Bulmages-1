/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include <QCloseEvent>
#include <QComboBox>
#include <QLayout>
#include <QMessageBox>
#include <QObject>
#include <QToolButton>
#include <QWidget>

#include <fstream>

#include "facturaview.h"
#include "bfcompany.h"
#include "contrato.h"
#include "contratoview.h"
#include "blfunctions.h"
#include "blplugins.h"

/** Inicializa la pantalla.
    Inicializa todos los componentes y mete la pantalla en el workSpace.
*/
/**
\param comp
\param parent
\return
**/
ContratoView::ContratoView ( BfCompany *comp, QWidget *parent )
        : Contrato ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        /// Disparamos los plugins.
        int res = g_plugins->run ( "ContratoView_ContratoView", this );
        if ( res != 0 )
            return;

        subform2->setMainCompany ( comp );
        mui_lineas->setMainCompany ( comp );
        mui_idcliente->setMainCompany ( comp );
        mui_refcontrato->setMainCompany ( comp );

        /// Establecemos los parametros de busqueda del Cliente
        mui_idcliente->setLabel ( _ ( "Cliente:" ) );
        mui_idcliente->setTableName ( "cliente" );
        mui_idcliente->m_valores["cifcliente"] = "";
        mui_idcliente->m_valores["nomcliente"] = "";
        /// Inicializamos BfForm
        setListaLineas ( mui_lineas );
        insertWindow ( windowTitle(), this, FALSE );
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear la contrato" ) );
    }
    
}


/** Al destruir la clase provocamos que se refresque el listado de contratos.
    Para que salga actualizado.
*/
/**
**/
ContratoView::~ContratoView()
{
    BL_FUNC_DEBUG
    
}


/** Inicializa los subformularios y resetea el control de cambios.
*/
/**
**/
void ContratoView::inicializar()
{
    BL_FUNC_DEBUG
    subform2->inicializar();
    dialogChanges_readValues();
    
}


/** Carga una contrato de la base de datos.
    Deleta toda la carga a la clase \ref Contrato
    Cambia el titulo de la ventana y reseta el control de cambios.
*/
/**
\param id
\return
**/
int ContratoView::load ( QString id )
{
    BL_FUNC_DEBUG
    try {
        Contrato::load ( id );
        if ( dbValue ( "idcontrato" ) != "" ) {
            setWindowTitle ( _ ( "Contrato" ) + " " + dbValue ( "refcontrato" ) + " " + dbValue ( "idcontrato" ) );
            insertWindow ( windowTitle(), this );
        } // end if
        mui_lineas->load ( id );
        subform2->load ( "SELECT * FROM factura LEFT JOIN cliente ON cliente.idcliente = factura.idcliente LEFT JOIN almacen ON factura.idalmacen = almacen.idalmacen  WHERE factura.idcliente =" + id + " AND reffactura = '" + dbValue ( "refcontrato" ) + "'" );
        dialogChanges_readValues();
    } catch ( ... ) {
        return -1;
    } // end try
    
    return 0;
}


/** Guardado de la ficha en la base de datos.
    Utiliza los metodos setXXX para establecer los valores de la Ficha en el BlDbRecord
    y luego llama a \ref Contrato::save() para el guardado en la base de datos.
*/
/// \TODO: Una vez hecho el guardado deberia hacer una carga y dejar de resetear el
/// control de cambios que deberia estar en el metodo de carga.
/**
\return
**/
int ContratoView::save()
{
    BL_FUNC_DEBUG
    try {
        setDbValue ( "refcontrato", mui_refcontrato->text() );
        setDbValue ( "nomcontrato", mui_nomcontrato->text() );
        setDbValue ( "idcliente", mui_idcliente->id() );
        setDbValue ( "fincontrato", mui_fincontrato->text() );
        setDbValue ( "ffincontrato", mui_ffincontrato->text() );
        setDbValue ( "loccontrato", mui_loccontrato->text() );
        setDbValue ( "descontrato", mui_descontrato->toPlainText() );
        setDbValue ( "periodicidadcontrato", mui_periodicidadcontrato->periodo() );
        Contrato::save();
        dialogChanges_readValues();
    } catch ( ... ) {
	
        throw - 1;
    } // end try
    
    return 0;
}


///
/**
\param id
**/
void ContratoView::on_m_cliente_valueChanged ( QString id )
{
    BL_FUNC_DEBUG
    subform2->setIdCliente ( id );
    
}


///
/**
\param id
**/
void ContratoView::pintaidcliente ( QString id )
{
    BL_FUNC_DEBUG
    mui_idcliente->setId ( id );
    
}


///
/**
\param id
**/
void ContratoView::pintafincontrato ( QString id )
{
    BL_FUNC_DEBUG
    mui_fincontrato->setText ( id );
    
}


///
/**
\param id
**/
void ContratoView::pintaffincontrato ( QString id )
{
    BL_FUNC_DEBUG
    mui_ffincontrato->setText ( id );
    
}


///
/**
\param id
**/
void ContratoView::pintadescontrato ( QString id )
{
    BL_FUNC_DEBUG
    mui_descontrato->setText ( id );
    
}


///
/**
\param id
**/
void ContratoView::pintarefcontrato ( QString id )
{
    BL_FUNC_DEBUG
    mui_refcontrato->setText ( id );
    
}


///
/**
\param id
**/
void ContratoView::pintanomcontrato ( QString id )
{
    BL_FUNC_DEBUG
    mui_nomcontrato->setText ( id );
    
}


///
/**
\param id
**/
void ContratoView::pintaperiodicidadcontrato ( QString id )
{
    BL_FUNC_DEBUG
    mui_periodicidadcontrato->setperiodo ( id );
    
}


///
/**
\param id
**/
void ContratoView::pintaloccontrato ( QString id )
{
    BL_FUNC_DEBUG
    mui_loccontrato->setText ( id );
    
}


///
/**
\return
**/
void ContratoView::on_subform2_itemDoubleClicked ( QTableWidgetItem * )
{
    BL_FUNC_DEBUG
    QString idfactura = subform2->dbValue ( QString ( "idfactura" ), subform2->currentRow() );
    FacturaView *prov = new FacturaView ( ( BfCompany * ) mainCompany(), 0 );
    if ( prov->load ( idfactura ) ) {
        delete prov;
        return;
    } // end if
    mainCompany() ->m_pWorkspace->addSubWindow ( prov );
    prov->show();
    
}


///
/**
**/
void ContratoView::on_mui_facturar_clicked()
{
    BL_FUNC_DEBUG
    int periodo = 1;
    QString query;
    bool end = FALSE;
    while ( !end ) {
        query = "SELECT ('" + dbValue ( "fincontrato" ) + "'::DATE +" + QString::number ( periodo - 1 ) + "* '" + dbValue ( "periodicidadcontrato" ) + "'::INTERVAL) AS finperiodo";
        query += ", ('" + dbValue ( "fincontrato" ) + "'::DATE +" + QString::number ( periodo ) + "* '" + dbValue ( "periodicidadcontrato" ) + "'::INTERVAL) AS ffinperiodo";
        BlDbRecordSet *cur1 = mainCompany() ->loadQuery ( query );

        query = "SELECT count(idfactura) AS cuenta FROM factura WHERE ffactura >= '" + cur1->value( "finperiodo" ) + "'";
        query += " AND ffactura <  '" + cur1->value( "ffinperiodo" ) + "'";
        query += " AND reffactura = '" + dbValue ( "refcontrato" ) + "'";
        query += " AND idcliente = " + dbValue ( "idcliente" );

        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
        if ( cur->value( "cuenta" ) != "0" ) {
            if ( cur->value( "cuenta" ) != "1" ) {
		BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Detectada doble factura en un periodo.") );
            } // end if
        } else {
            // GENERAMOS LA FACTURA
            /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
            int resur = g_plugins->run ( "SNewFacturaView", ( BfCompany * ) mainCompany() );
            if ( !resur ) {
                blMsgInfo ( _( "no se pudo crear instancia de factura") );
                return;
            } // end if
            FacturaView *fac = ( FacturaView * ) g_plugParams;	    
	    
            mainCompany() ->m_pWorkspace->addSubWindow ( fac );
            fac->load ( "0" );
            fac->show();
            fac->setDbValue ( "reffactura", dbValue ( "refcontrato" ) );
            fac->setDbValue ( "idcliente", dbValue ( "idcliente" ) );
            fac->setDbValue ( "descfactura", dbValue ( "nomcontrato" ) + " Periodo:  " + cur1->value( "finperiodo" ).left ( 10 ) + " -- " + cur1->value( "ffinperiodo" ).left ( 10 ) );

            BlDbRecordSet *curcliente = mainCompany() ->loadQuery ( "SELECT recargoeqcliente, regimenfiscalcliente FROM cliente WHERE idcliente = " + dbValue ( "idcliente" ) );
            if ( ! curcliente ) {
                return;
            } // end if
            if ( curcliente->eof() ) {
                return;
            } // end if

            QString l;
            BlDbSubFormRecord *linea, *linea1;
            for ( int i = 0; i < m_listalineas->rowCount(); ++i ) {
                linea = m_listalineas->lineaat ( i );
                if ( linea->dbValue ( "idarticulo" ) != "" ) {
                    linea1 = fac->getlistalineas() ->lineaat ( fac->getlistalineas() ->rowCount() - 1 );
                    /// Haciendo la nueva linea antes nos evitamos problemas de foco
                    fac->getlistalineas() ->newRecord();
                    fac->getlistalineas() ->setProcesarCambios ( FALSE );
                    linea1->setDbValue ( "idarticulo", linea->dbValue ( "idarticulo" ) );
                    linea1->setDbValue ( "codigocompletoarticulo", linea->dbValue ( "codigocompletoarticulo" ) );
                    linea1->setDbValue ( "nomarticulo", linea->dbValue ( "nomarticulo" ) );
                    linea1->setDbValue ( "desclfactura", linea->dbValue ( "desclcontrato" ) );
                    linea1->setDbValue ( "descuentolfactura", "0" );
                    linea1->setDbValue ( "cantlfactura", linea->dbValue ( "cantlcontrato" ) );
                    linea1->setDbValue ( "pvplfactura", linea->dbValue ( "pvplcontrato" ) );
                    /// Buscamos el tipo de iva que corresponde al articulo y lo ponemos.
                    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM articulo WHERE idarticulo = " + linea->dbValue ( "idarticulo" ) );
                    BlDbRecordSet *cur1 = mainCompany() ->loadQuery ( "SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->value( "idtipo_iva" ) + " ORDER BY fechatasa_iva DESC LIMIT 1" );
                    if ( !cur->eof() ) {

                        if ( curcliente->value( "regimenfiscalcliente" ) == "Normal" ) {
                            linea1->setDbValue ( "ivalfactura", cur1->value( "porcentasa_iva" ) );
                        } // end if

                        if ( curcliente->value( "recargoeqcliente" ) == "t" ) {
                            linea1->setDbValue ( "reqeqlfactura", cur1->value( "porcentretasa_iva" ) );
                        } // end if

                    } // end if
                    delete cur1;
                    delete cur;


                    fac->getlistalineas() ->setProcesarCambios ( TRUE );
                    linea1->refresh();
                } // end if
            } // end for
            fac->calculaypintatotales();
            fac->pintar();
            delete curcliente;
        } // end if
        delete cur;


        query = "SELECT (now() < '" + dbValue ( "fincontrato" ) + "'::DATE + '" + dbValue ( "periodicidadcontrato" ) + "'::INTERVAL *" + QString::number ( periodo ) + " ) AS dato";
        cur = mainCompany() ->loadQuery ( query );
        if ( cur->value( "dato" ) == "t" ) {
            end = TRUE;
        }// end if
        delete cur1;
        periodo++;
    } // end while
    
}

/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para trabajar con la tabla factura.
*/
/**
\param parent
**/
FacturasContratoListSubForm::FacturasContratoListSubForm ( QWidget *parent, const char * ) : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    setDbTableName ( "factura" );
    setDbFieldId ( "idfactura" );
    setFileConfig ( "contratofactura" );
    addSubFormHeader ( "numfactura", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Numero" ) );
    addSubFormHeader ( "ffactura", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha" ) );
    addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre cliente" ) );
    addSubFormHeader ( "telfactura", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Telefono factura" ) );
    addSubFormHeader ( "bimpfactura", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Base imponible" ) );
    addSubFormHeader ( "impfactura", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Impuestos" ) );
    addSubFormHeader ( "totalfactura", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Total" ) );
    addSubFormHeader ( "procesadafactura", BlDbField::DbBoolean, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Procesada" ) );
    addSubFormHeader ( "idfactura", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Id factura" ) );
    addSubFormHeader ( "reffactura", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Ref factura" ) );
    addSubFormHeader ( "codigoserie_factura", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Serie" ) );
    addSubFormHeader ( "cifcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "CIF cliente" ) );
    addSubFormHeader ( "codigoalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Codigo almacen" ) );
    addSubFormHeader ( "contactfactura", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Contact factura" ) );
    addSubFormHeader ( "comentfactura", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Comentario factura" ) );
    addSubFormHeader ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Id trabajador" ) );
    addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Id cliente" ) );
    addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Id almacen" ) );
    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    
}


