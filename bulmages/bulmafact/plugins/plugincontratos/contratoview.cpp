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
#include "plugins.h"

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
    _depura ( "ContratoView::ContratoView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        /// Disparamos los plugins.
        int res = g_plugins->lanza ( "ContratoView_ContratoView", this );
        if ( res != 0 )
            return;

        subform2->setMainCompany ( comp );
        mui_lineas->setMainCompany ( comp );
        mui_idcliente->setMainCompany ( comp );
        mui_refcontrato->setMainCompany ( comp );

		/// Establecemos los parametros de busqueda del Cliente
		mui_idcliente->setLabel ( _( "Cliente:" ) );
		mui_idcliente->setTableName( "cliente" );
		mui_idcliente->m_valores["cifcliente"] = "";
		mui_idcliente->m_valores["nomcliente"] = "";
        /// Inicializamos FichaBf
        setListaLineas ( mui_lineas );
        meteWindow ( windowTitle(), this, FALSE );
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear la contrato" ) );
    }
    _depura ( "END ContratoView::ContratoView" );
}


/** Al destruir la clase provocamos que se refresque el listado de contratos.
    Para que salga actualizado.
*/
/**
**/
ContratoView::~ContratoView()
{
    _depura ( "ContratoView::~ContratoView", 0 );
    _depura ( "END ContratoView::~ContratoView", 0 );
}


/** Inicializa los subformularios y resetea el control de cambios.
*/
/**
**/
void ContratoView::inicializar()
{
    _depura ( "ContratoView::inicializar", 0 );
    subform2->inicializar();
    dialogChanges_cargaInicial();
    _depura ( "END ContratoView::inicializar", 0 );
}


/** Carga una contrato de la base de datos.
    Deleta toda la carga a la clase \ref Contrato
    Cambia el titulo de la ventana y reseta el control de cambios.
*/
/**
\param id
\return
**/
int ContratoView::cargar ( QString id )
{
    _depura ( "ContratoView::cargar", 0 );
    try {
        Contrato::cargar ( id );
        if ( DBvalue ( "idcontrato" ) != "" ) {
            setWindowTitle ( _( "Contrato" ) + " " + DBvalue ( "refcontrato" ) + " " + DBvalue ( "idcontrato" ) );
            meteWindow ( windowTitle(), this );
        } // end if
        mui_lineas->cargar ( id );
        subform2->cargar ( "SELECT * FROM factura LEFT JOIN cliente ON cliente.idcliente = factura.idcliente LEFT JOIN almacen ON factura.idalmacen = almacen.idalmacen  WHERE factura.idcliente =" + id + " AND reffactura = '" + DBvalue ( "refcontrato" ) + "'" );
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        return -1;
    } // end try
    _depura ( "END ContratoView::cargar", 0 );
    return 0;
}


/** Guardado de la ficha en la base de datos.
    Utiliza los metodos setXXX para establecer los valores de la Ficha en el BlDbRecord
    y luego llama a \ref Contrato::guardar() para el guardado en la base de datos.
*/
/// \TODO: Una vez hecho el guardado deberia hacer una carga y dejar de resetear el
/// control de cambios que deberia estar en el metodo de carga.
/**
\return
**/
int ContratoView::guardar()
{
    _depura ( "ContratoView::guardar", 0 );
    try {
        setDbValue ( "refcontrato", mui_refcontrato->text() );
        setDbValue ( "nomcontrato", mui_nomcontrato->text() );
        setDbValue ( "idcliente", mui_idcliente->id() );
        setDbValue ( "fincontrato", mui_fincontrato->text() );
        setDbValue ( "ffincontrato", mui_ffincontrato->text() );
        setDbValue ( "loccontrato", mui_loccontrato->text() );
        setDbValue ( "descontrato", mui_descontrato->toPlainText() );
        setDbValue ( "periodicidadcontrato", mui_periodicidadcontrato->periodo() );
        Contrato::guardar();
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        _depura ( "ContratoView::guardar error al guardar", 0 );
        throw - 1;
    } // end try
    _depura ( "END ContratoView::guardar", 0 );
    return 0;
}


///
/**
\param id
**/
void ContratoView::on_m_cliente_valueChanged ( QString id )
{
    _depura ( "ContratoView::on_m_cliente_valueChanged", 0 );
    subform2->setIdCliente ( id );
    _depura ( "END ContratoView::on_m_cliente_valueChanged", 0 );
}


///
/**
\param id
**/
void ContratoView::pintaidcliente ( QString id )
{
    _depura ( "ContratoView::pintaidcliente", 0 );
    mui_idcliente->setId ( id );
    _depura ( "END ContratoView::pintaidcliente", 0 );
}


///
/**
\param id
**/
void ContratoView::pintafincontrato ( QString id )
{
    _depura ( "ContratoView::pintafincontrato", 0 );
    mui_fincontrato->setText ( id );
    _depura ( "ContratoView::pintafincontrato", 0 );
}


///
/**
\param id
**/
void ContratoView::pintaffincontrato ( QString id )
{
    _depura ( "ContratoView::pintaffincontrato", 0 );
    mui_ffincontrato->setText ( id );
    _depura ( "END ContratoView::pintaffincontrato", 0 );
}


///
/**
\param id
**/
void ContratoView::pintadescontrato ( QString id )
{
    _depura ( "ContratoView::pintadescontrato", 0 );
    mui_descontrato->setText ( id );
    _depura ( "END ContratoView::pintadescontrato", 0 );
}


///
/**
\param id
**/
void ContratoView::pintarefcontrato ( QString id )
{
    _depura ( "ContratoView::pintarefcontrato", 0 );
    mui_refcontrato->setText ( id );
    _depura ( "END ContratoView::pintarefcontrato", 0 );
}


///
/**
\param id
**/
void ContratoView::pintanomcontrato ( QString id )
{
    _depura ( "ContratoView::pintanomcontrato", 0 );
    mui_nomcontrato->setText ( id );
    _depura ( "ContratoView::pintanomcontrato", 0 );
}


///
/**
\param id
**/
void ContratoView::pintaperiodicidadcontrato ( QString id )
{
    _depura ( "ContratoView::pintaperiodicidadcontrato", 0, id );
    mui_periodicidadcontrato->setperiodo ( id );
}


///
/**
\param id
**/
void ContratoView::pintaloccontrato ( QString id )
{
    _depura ( "ContratoView::pintaloccontrato", 0 );
    mui_loccontrato->setText ( id );
    _depura ( "END ContratoView::pintaloccontrato", 0 );
}


///
/**
\return
**/
void ContratoView::on_subform2_itemDoubleClicked ( QTableWidgetItem * )
{
    _depura ( "ContratoView::on_subform2_itemDoubleClicked", 0 );
    QString idfactura = subform2->DBvalue ( QString ( "idfactura" ), subform2->currentRow() );
    FacturaView *prov = new FacturaView( (BfCompany *) mainCompany(), 0);
    if ( prov->cargar ( idfactura ) ) {
        delete prov;
        return;
    } // end if
    mainCompany() ->m_pWorkspace->addWindow ( prov );
    prov->show();
    _depura ( "END ContratoView::on_subform2_itemDoubleClicked", 0 );
}


///
/**
**/
void ContratoView::on_mui_facturar_clicked()
{
    _depura ( "ContratoView::on_mui_facturar_clicked", 0 );
    int periodo = 1;
    QString query;
    bool end = FALSE;
    while ( !end ) {
        query = "SELECT ('" + DBvalue ( "fincontrato" ) + "'::DATE +" + QString::number ( periodo - 1 ) + "* '" + DBvalue ( "periodicidadcontrato" ) + "'::INTERVAL) AS finperiodo";
        query += ", ('" + DBvalue ( "fincontrato" ) + "'::DATE +" + QString::number ( periodo ) + "* '" + DBvalue ( "periodicidadcontrato" ) + "'::INTERVAL) AS ffinperiodo";
        BlDbRecordSet *cur1 = mainCompany() ->cargacursor ( query );

        query = "SELECT count(idfactura) AS cuenta FROM factura WHERE ffactura >= '" + cur1->valor ( "finperiodo" ) + "'";
        query += " AND ffactura <  '" + cur1->valor ( "ffinperiodo" ) + "'";
        query += " AND reffactura = '" + DBvalue ( "refcontrato" ) + "'";
        query += " AND idcliente = " + DBvalue ( "idcliente" );

        BlDbRecordSet *cur = mainCompany() ->cargacursor ( query );
        if ( cur->valor ( "cuenta" ) != "0" ) {
            if ( cur->valor ( "cuenta" ) != "1" ) {
                _depura ( "Detectada doble factura en un periodo", 2, cur->valor ( "cuenta" ) );
            } // end if
        } else {
            // GENERAMOS LA FACTURA
            FacturaView *fac = new FacturaView( (BfCompany *) mainCompany(), 0);
            mainCompany() ->m_pWorkspace->addWindow ( fac );
            fac->cargar ( "0" );
            fac->show();
            fac->setDbValue ( "reffactura", DBvalue ( "refcontrato" ) );
            fac->setDbValue ( "idcliente", DBvalue ( "idcliente" ) );
            fac->setDbValue ( "descfactura", DBvalue ( "nomcontrato" ) + " Periodo:  " + cur1->valor ( "finperiodo" ).left ( 10 ) + " -- " + cur1->valor ( "ffinperiodo" ).left ( 10 ) );

            BlDbRecordSet *curcliente = mainCompany() ->cargacursor ( "SELECT recargoeqcliente, regimenfiscalcliente FROM cliente WHERE idcliente = " + DBvalue ( "idcliente" ) );
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
                if ( linea->DBvalue ( "idarticulo" ) != "" ) {
                    linea1 = fac->getlistalineas() ->lineaat ( fac->getlistalineas() ->rowCount() - 1 );
                    /// Haciendo la nueva linea antes nos evitamos problemas de foco
                    fac->getlistalineas() ->nuevoRegistro();
                    fac->getlistalineas() ->setProcesarCambios ( FALSE );
                    linea1->setDbValue ( "idarticulo", linea->DBvalue ( "idarticulo" ) );
                    linea1->setDbValue ( "codigocompletoarticulo", linea->DBvalue ( "codigocompletoarticulo" ) );
                    linea1->setDbValue ( "nomarticulo", linea->DBvalue ( "nomarticulo" ) );
                    linea1->setDbValue ( "desclfactura", linea->DBvalue ( "desclcontrato" ) );
                    linea1->setDbValue ( "descuentolfactura", "0" );
                    linea1->setDbValue ( "cantlfactura", linea->DBvalue ( "cantlcontrato" ) );
                    linea1->setDbValue ( "pvplfactura", linea->DBvalue ( "pvplcontrato" ) );
                    /// Buscamos el tipo de iva que corresponde al articulo y lo ponemos.
                    BlDbRecordSet *cur = mainCompany() ->cargacursor ( "SELECT * FROM articulo WHERE idarticulo = " + linea->DBvalue ( "idarticulo" ) );
                    BlDbRecordSet *cur1 = mainCompany() ->cargacursor ( "SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor ( "idtipo_iva" ) + " ORDER BY fechatasa_iva LIMIT 1" );
                    if ( !cur->eof() ) {

                        if ( curcliente->valor ( "regimenfiscalcliente" ) == "Normal" ) {
                            linea1->setDbValue ( "ivalfactura", cur1->valor ( "porcentasa_iva" ) );
                        } // end if

                        if ( curcliente->valor ( "recargoeqcliente" ) == "t" ) {
                            linea1->setDbValue ( "reqeqlfactura", cur1->valor ( "porcentretasa_iva" ) );
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


        query = "SELECT (now() < '" + DBvalue ( "fincontrato" ) + "'::DATE + '" + DBvalue ( "periodicidadcontrato" ) + "'::INTERVAL *" + QString::number ( periodo ) + " ) AS dato";
        cur = mainCompany() ->cargacursor ( query );
        if ( cur->valor ( "dato" ) == "t" ) {
            end = TRUE;
        }// end if
        delete cur1;
        periodo++;
    } // end while
    _depura ( "END ContratoView::on_mui_facturar_clicked", 0 );
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
    _depura ( "FacturasContratoListSubForm::FacturasContratoListSubForm", 0 );
    setDbTableName ( "factura" );
    setDbFieldId ( "idfactura" );
    setFileConfig ( "contratofactura" );
    addSubFormHeader ( "numfactura", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Numero" ) );
    addSubFormHeader ( "ffactura", BlDbField::DBdate, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Fecha" ) );
    addSubFormHeader ( "nomcliente", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Nombre cliente" ) );
    addSubFormHeader ( "telfactura", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Telefono factura" ) );
    addSubFormHeader ( "bimpfactura", BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Base imponible" ) );
    addSubFormHeader ( "impfactura", BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Impuestos" ) );
    addSubFormHeader ( "totalfactura", BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Total" ) );
    addSubFormHeader ( "procesadafactura", BlDbField::DBboolean, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Procesada" ) );
    addSubFormHeader ( "idfactura", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Id factura" ) );
    addSubFormHeader ( "reffactura", BlDbField::DBint, BlDbField::DBNotNull | BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Ref factura" ) );
    addSubFormHeader ( "codigoserie_factura", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Serie" ) );
    addSubFormHeader ( "cifcliente", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "CIF cliente" ) );
    addSubFormHeader ( "codigoalmacen", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Codigo almacen" ) );
    addSubFormHeader ( "contactfactura", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Contact factura" ) );
    addSubFormHeader ( "comentfactura", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Comentario factura" ) );
    addSubFormHeader ( "idtrabajador", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Id trabajador" ) );
    addSubFormHeader ( "idcliente", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Id cliente" ) );
    addSubFormHeader ( "idalmacen", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Id almacen" ) );
    setinsercion ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    _depura ( "END FacturasContratoListSubForm::FacturasContratoListSubForm", 0 );
}


