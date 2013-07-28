/***************************************************************************
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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

#include <QtGui/QCloseEvent>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>
#include <QtCore/QObject>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

#include "compraventaview.h"
#include "clientslist.h"
#include "cobroview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "facturaslist.h"
#include "facturaview.h"
#include "facturapview.h"
#include "blfunctions.h"
#include "bfinformereferencia.h"
#include "bfclientealbarandescuentosubform.h"
#include "bfclientealbaransubform.h"
#include "blpostgresqlclient.h"
#include "blplugins.h"


/** Constructor de la clase corresponde a la parte visual de la ficha de cliente.
    Inicializa la ventana y todos sus componentes.
    Mete la ventana en el WorkSpace.
*/
/**
\param comp
\param parent
\return
**/
CompraVentaView::CompraVentaView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        m_albaranp = new BlDbRecord ( comp );
        m_albaranp->setDbTableName ( "albaranp" );
        m_albaranp->setDbFieldId ( "idalbaranp" );
        m_albaranp->addDbField ( "idalbaranp", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Id albaran proveedor" ) );
        m_albaranp->addDbField ( "numalbaranp", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Numero albaran proveedor" ) );
        m_albaranp->addDbField ( "fechaalbaranp", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha albaran proveedor" ) );
        m_albaranp->addDbField ( "comentalbaranp", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentario albaran proveedor" ) );
        m_albaranp->addDbField ( "idproveedor", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id proveedor" ) );
        m_albaranp->addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Id forma de pago" ) );
        m_albaranp->addDbField ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id almacen" ) );
        m_albaranp->addDbField ( "refalbaranp", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia albaran proveedor" ) );
        m_albaranp->addDbField ( "descalbaranp", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Descripcion albaran proveedor" ) );

        subform3->setMainCompany ( comp );
        m_descuentos3->setMainCompany ( comp );

        setTitleName ( _ ( "Compra Venta" ) );
        setDbTableName ( "albaran" );
        setDbFieldId ( "idalbaran" );
        addDbField ( "idalbaran", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Id albaran" ) );
        addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Cliente" ) );
        addDbField ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Almacen" ) );
        addDbField ( "numalbaran", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Numero de albaran" ) );
        addDbField ( "fechaalbaran", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha de creacion" ) );
        addDbField ( "contactalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Persona de contacto" ) );
        addDbField ( "telalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Telefono de contacto" ) );
        addDbField ( "comentalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentario" ) );
        addDbField ( "comentprivalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentario privado" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Forma de pago" ) );
        addDbField ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Trabajador" ) );
        addDbField ( "procesadoalbaran", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Procesado" ) );
        addDbField ( "descalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Descripcion" ) );
        addDbField ( "refalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia" ) );

        /// Disparamos los plugins.
        int res = g_plugins->run ( "CompraVentaView_CompraVentaView", this );
        if ( res != 0 )
            return;

        subform2->setMainCompany ( comp );
        m_descuentos->setMainCompany ( comp );
        mui_idalmacen->setMainCompany ( comp );
        mui_idforma_pago->setMainCompany ( comp );
        mui_idcliente->setMainCompany ( comp );
        mui_idtrabajador->setMainCompany ( comp );
        mui_refalbaran->setMainCompany ( comp );

        /// Establecemos los parametros de busqueda del Cliente
        mui_idcliente->setLabel ( _ ( "Cliente:" ) );
        mui_idcliente->setTableName ( "cliente" );
        mui_idcliente->m_valores["cifcliente"] = "";
        mui_idcliente->m_valores["nomcliente"] = "";

        setListaLineas ( subform2 );
        setListaDescuentos ( m_descuentos );

        /// Inicializamos para que no se queden sin ser pintada.
        mui_idforma_pago->setFieldValue ( "0" );
        mui_idalmacen->setFieldValue ( "0" );
        mui_idtrabajador->setFieldValue ( "0" );


        subform3->load ( "0" );
        subform2->load ( "0" );


        /// Conectamos algunos elementos para que funcionen.
//     connect ( mui_idalmacen, SIGNAL ( valueChanged ( QString ) ), this, SLOT ( on_mui_idalmacen_valueChanged ( QString ) ) );

        insertWindow ( windowTitle(), this, false );

        /// Disparamos los plugins por flanco descendente.
        g_plugins->run ( "CompraVentaView_CompraVentaView_Post", this );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el albaran a cliente" ), this );
    } // end try
    
}


/** Destructor de la clase.
    Indica al listado que debe actualizarse.
*/
/// \TODO: Este metodo deberia mejorarse para que indicase al listado que
/// solo debe eliminar una fila del mismo.
/**
**/
CompraVentaView::~CompraVentaView()
{
    BL_FUNC_DEBUG
    delete m_albaranp;
    
}


/// Inicializa todos los elementos del formulario
/**
**/
void CompraVentaView::inicializar()
{
    BL_FUNC_DEBUG
    subform2->inicializar();
    m_descuentos->inicializar();
    dialogChanges_readValues();
    
}



/// Pinta los totales en las casillas correspondientes
/**
\param iva
\param base
\param total
\param desc
\param irpf
\param reqeq
**/
void CompraVentaView::pintatotales ( BlFixed iva, BlFixed base, BlFixed total, BlFixed desc, BlFixed irpf, BlFixed reqeq )
{
    BL_FUNC_DEBUG
    m_totalBases->setText ( base.toQString() );
    m_totalTaxes->setText ( iva.toQString() );
    m_totalalbaran->setText ( total.toQString() );
    m_totalDiscounts->setText ( desc.toQString() );
    m_totalIRPF->setText ( QString ( irpf.toQString() ) );
    m_totalReqEq->setText ( QString ( reqeq.toQString() ) );
    
}




/// Crea un nuevo cobro para el albar&aacute;n seleccionado.
/// Util con cobros anticipados a la factura. El cobro tendra
/// la misma cantidad, referencia y cliente que el albaran.
/// Este metodo crea una pantalla de cobro y le pone los datos necesarios.
/**
**/
void CompraVentaView::on_mui_cobrar_clicked()
{
    BL_FUNC_DEBUG
    recogeValores();
    CobroView *bud = new CobroView ( mainCompany(), 0 );
    mainCompany()->pWorkspace()->addSubWindow ( bud );
    bud->setDbValue ( "idcliente", dbValue ( "idcliente" ) );
    bud->setDbValue ( "cantcobro", m_totalalbaran->text() );
    bud->setDbValue ( "refcobro", dbValue ( "refalbaran" ) );
    bud->setDbValue ( "comentcobro", dbValue ( "descalbaran" ) );
    bud->pintar();
    bud->show();
    
}


///
/**
\param id
**/
void CompraVentaView::on_mui_idcliente_valueChanged ( QString id )
{
    BL_FUNC_DEBUG
    subform2->setIdCliente ( id );
    subform3->setIdCliente ( id );

    mui_idforma_pago->setIdCliente ( id );
    setDbValue ( "idcliente", id );

    if ( ! id.isEmpty() ) {
        QString query = "SELECT idproveedor FROM proveedor WHERE cifproveedor IN (SELECT cifcliente FROM cliente WHERE idcliente= " + id + ")";
        BlDbRecordSet *cur = mainCompany()->loadQuery ( query );
        if ( !cur->eof() ) {
            m_albaranp->setDbValue ( "idproveedor", cur->value( "idproveedor" ) );
            subform3->setIdProveedor ( cur->value( "idproveedor" ) );
            m_descuentos3->setIdProveedor ( cur->value( "idproveedor" ) );
        } else {
            blMsgInfo ( "No hay proveedor para este cliente" );
        } // end if
        delete cur;
    } else {
        m_albaranp->setDbValue ( "idproveedor", "" );
        subform3->setIdProveedor ( "" );
        m_descuentos3->setIdProveedor ( "" );
    } // end if

    
}



///
/**
\param id
**/
void CompraVentaView::on_mui_idalmacen_valueChanged ( QString id )
{
    BL_FUNC_DEBUG
    subform2->setIdAlmacen ( id );
    subform3->setIdAlmacen ( id );
    
}


/// Este slot se activa cuando hay cambios en los subformularios.
/**
**/
void CompraVentaView::s_pintaTotales()
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


///
/**
**/
void CompraVentaView::on_m_descuentos_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


///
/**
**/
void CompraVentaView::on_subform2_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}



/** Metodo de borrar un albaran. Se encarga de mandar a la base de datos
    la instruccion necesaria para el borrado de un albaran y controlar
    los posibles errores que se produzcan.
    Tambi&eacute;n borra todas las lineas y las lineas de descuento que se refieren
    a el albaran que se pretende borrar.
*/
/// \todo: Este metodo deberia poderse delegar en BlDbRecord, o por lo menos la parte del borrado del registro.
/// \todo: Hace falta meter el metodo dentro del sistema de excepciones try catch.
/**
\return
**/
int CompraVentaView::beforeDelete()
{
    BL_FUNC_DEBUG
    m_listalineas->remove();
    m_listadescuentos->remove();
    
    return 0;
}





/** Este m&eacute;todo carga un AlbaranCliente. Tambi&eacute;n carga las lineas
    de albar&aacute;n y los descuentos de albar&aacute;n.
    Tras la carga tambi&eacute;n invoca un repintado del albaran para que se vea
    correctamente la pantalla.
*/
/**
\param idalbaran
\return
**/
int CompraVentaView::cargarPost ( QString idalbaran )
{
    BL_FUNC_DEBUG
    try {
        m_listalineas->load ( idalbaran );
        m_listadescuentos->load ( idalbaran );

        /// Establecemos los valores del albaran de proveedor por si acaso el albaran de proveedor no existiese.
        m_albaranp->setDbValue ( "refalbaranp", dbValue ( "refalbaran" ) );


        /// Buscamos si hay algun albaran de proveedor y lo cargamos.
        QString query = "SELECT * FROM albaranp WHERE refalbaranp='" + dbValue ( "refalbaran" ) + "'";
        BlDbRecordSet *cur = mainCompany()->loadQuery ( query );
        if ( !cur->eof() ) {
            m_albaranp->load ( cur->value( "idalbaranp" ) );
            subform3->load ( cur->value( "idalbaranp" ) );
            m_descuentos3->load ( cur->value( "idalbaranp" ) );
        } // end if
        delete cur;

        /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
        g_plugins->run ( "AlbaranCliente_cargarPost_Post", this );

        calculaypintatotales();
        
        return 0;
    } catch ( ... ) {
	
        return 0;
    } // end try
}


/** Este m&eacute;todo se encarga de hacer el guardado del albar&aacute; en la
    base de datos. Una vez guardado se guardan las lineas de albar&aacute;n y los
    descuentos de albar&aacute;n. Tras hacer el guardado se hace una carga del albaran
    para recuperar los datos que haya podido escribir la base de datos automaticamente.
    Dichos datos son la referencia y el n&uacute;mero de albar&aacute;n.

    Si todo funciona bien este m&eacute;todo devuelve 0. Si se produce algun error
    se genera una excepcion -1.
*/
/**
\return
**/
int CompraVentaView::afterSave()
{
    BL_FUNC_DEBUG
    try {
// setDbValue("idalmacen" , mdb_idalmacen->text());
// BlDbRecord::save();
// save();
// load(id);
        m_listalineas->setColumnValue ( "idalbaran", dbValue ( "idalbaran" ) );
        m_listalineas->save();
        m_listadescuentos->setColumnValue ( "idalbaran", dbValue ( "idalbaran" ) );
        m_listadescuentos->save();

        /// Cargamos el albaran para que se refresquen los valores puestos por la base de datos.
        load ( dbValue ( "idalbaran" ) );
        generarFactura();


        if ( subform3->rowCount() > 1 ) {
            m_albaranp->setDbValue ( "refalbaranp", dbValue ( "refalbaran" ) );
            m_albaranp->setDbValue ( "idalmacen", dbValue ( "idalmacen" ) );
            m_albaranp->BlDbRecord::save();
            subform3->setColumnValue ( "idalbaranp", m_albaranp->dbValue ( "idalbaranp" ) );
            m_descuentos3->setColumnValue ( "idalbaranp", m_albaranp->dbValue ( "idalbaranp" ) );
            subform3->save();
            m_descuentos3->save();
            m_albaranp->BlDbRecord::load ( m_albaranp->dbValue ( "idalbaranp" ) );

            generarFacturaProveedor();
        } // end if

        
    } catch ( ... ) {
	
    } // end try
    return 0;
}




/// Se encarga de generar una factura a partir de un albar&aacute;n.
/** Primero de todo busca una factura por referencia que tenga este albaran.
    Si dicha factura existe entonces la cargamos y terminamos.
    Si no existe dicha factura el sistema avisa y permite crear una poniendo
    Todos los datos del albaran automaticamente en ella.
*/
/**
\return
**/
void CompraVentaView::generarFacturaProveedor()
{
    BL_FUNC_DEBUG

    FacturaProveedorView *bud = NULL;
    BlDbRecordSet *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe una factura con esos datos, y en caso afirmativo lo mostramos.

        QString SQLQuery = "";

        if ( m_albaranp->dbValue ( "refalbaranp" ).isEmpty() || m_albaranp->dbValue ( "idproveedor" ).isEmpty() ) {
            /// El albaran no se ha guardado y no se dispone en la base de datos
            /// de estos datos. Se utilizan en su lugar los del formulario.
            /// Verifica que exista, por lo menos, un cliente seleccionado.
            if ( m_albaranp->dbValue ( "idproveedor" ).isEmpty() ) {
                blMsgInfo ( _ ( "Tiene que seleccionar un proveedor" ), this );
                return;
            } else {
                SQLQuery = "SELECT * FROM facturap WHERE reffacturap = '" + m_albaranp->dbValue ( "refalbaranp" ) + "' AND idproveedor = " + m_albaranp->dbValue ( "idproveedor" );
            } // end if
        } else {
            SQLQuery = "SELECT * FROM facturap WHERE reffacturap = '" + m_albaranp->dbValue ( "refalbaranp" ) + "' AND idproveedor = " + m_albaranp->dbValue ( "idproveedor" );
        } // end if

        cur = mainCompany() ->loadQuery ( SQLQuery );

        if ( !cur->eof() ) {
            /// Informamos que ya hay una factura y que la abriremos.
            /// Si no salimos de la funci&oacute;n.
            if ( QMessageBox::question ( this,
                                         _ ( "Factura de proveedor existente" ),
                                         _ ( "Existe una factura de este proveedor con la misma referencia que este albaran. Desea rehacerla ?" ),
                                         _ ( "&Si" ), _ ( "&No" ), QString::null, 0, 1 ) ) {
                return;
            } // end if
            SQLQuery = "DELETE FROM lfacturap WHERE idfacturap=" + cur->value( "idfacturap" );
            mainCompany()->runQuery ( SQLQuery );
            SQLQuery = "DELETE FROM facturap WHERE idfacturap = " + cur->value( "idfacturap" );
            mainCompany()->runQuery ( SQLQuery );

        } // end if
        delete cur;

        /// Si el albaran no tiene lineas salimos sin crear la factura. Teniendo en cuenta que la primera linea esta en blanco.
        if ( subform3->rowCount() <= 1 )
            return;

        /// Creamos la factura de proveedor.
        FacturaProveedorView *bud = new FacturaProveedorView ( ( BfCompany * ) mainCompany(), 0 );
        mainCompany() ->m_pWorkspace->addSubWindow ( bud );

        /// Cargamos un elemento que no existe para inicializar bien la clase.
        bud->inicializar();

        bud->pintar();
        /// Traspasamos los datos a la factura.
        bud->setDbValue ( "comentfacturap", m_albaranp->dbValue ( "comentalbaranp" ) );
        bud->setDbValue ( "idforma_pago", m_albaranp->dbValue ( "idforma_pago" ) );
        bud->setDbValue ( "reffacturap", dbValue ( "refalbaran" ) );
        bud->setDbValue ( "numfacturap", dbValue ( "refalbaran" ) );
        bud->setDbValue ( "idproveedor", m_albaranp->dbValue ( "idproveedor" ) );
        bud->setDbValue ( "idalmacen", m_albaranp->dbValue ( "idalmacen" ) );
        bud->pintar();
        bud->show();

        bud->getlistalineas()->setIdProveedor ( dbValue ( "idproveedor" ) );
        bud->getlistalineas()->setIdAlmacen ( dbValue ( "idalmacen" ) );

        /// Traspasamos las lineas
        QString l;
        BlDbSubFormRecord *linea, *linea1;
        for ( int i = 0; i < subform3->rowCount(); ++i ) {
            linea = subform3->lineaat ( i );
            if ( linea->dbValue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                bud->getlistalineas() ->newRecord();
                bud->getlistalineas() ->setProcesarCambios ( false );
                linea1->setDbValue ( "desclfacturap", linea->dbValue ( "desclalbaranp" ) );
                linea1->setDbValue ( "cantlfacturap", linea->dbValue ( "cantlalbaranp" ) );
                linea1->setDbValue ( "pvplfacturap", linea->dbValue ( "pvplalbaranp" ) );
                linea1->setDbValue ( "descuentolfacturap", linea->dbValue ( "descuentolalbaranp" ) );
                linea1->setDbValue ( "idarticulo", linea->dbValue ( "idarticulo" ) );
                linea1->setDbValue ( "codigocompletoarticulo", linea->dbValue ( "codigocompletoarticulo" ) );
                linea1->setDbValue ( "nomarticulo", linea->dbValue ( "nomarticulo" ) );
                linea1->setDbValue ( "ivalfacturap", linea->dbValue ( "ivalalbaranp" ) );
                /// Estos traspasos no son para un plugin generico pero ahora no tengo tiempo de arreglarlo.
                linea1->setDbValue ( "impesplfacturap", linea->dbValue ( "impesplalbaranp" ) );
                linea1->setDbValue ( "kiloolivalfacturap", linea->dbValue ( "kiloolivalalbaranp" ) );
                linea1->setDbValue ( "rendimientolfacturap", linea->dbValue ( "rendimientolalbaranp" ) );
                bud->getlistalineas() ->setProcesarCambios ( true );
            } // end if
        } // end for

        bud->pintar();
        bud->save();
        bud->close();

    } catch ( ... ) {
        blMsgInfo ( _ ( "Error inesperado" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try

    
}


/// Se encarga de generar una factura a partir de un albar&aacute;n.
/** Primero de todo busca una factura por referencia que tenga este albaran.
    Si dicha factura existe entonces la cargamos y terminamos.
    Si no existe dicha factura el sistema avisa y permite crear una poniendo
    Todos los datos del albaran automaticamente en ella.
*/
/**
\return
**/
void CompraVentaView::generarFactura()
{
    BL_FUNC_DEBUG

    FacturaView *bud = NULL;
    BlDbRecordSet *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe una factura con esos datos, y en caso afirmativo lo mostramos.

        QString SQLQuery = "";

        if ( dbValue ( "refalbaran" ).isEmpty() || dbValue ( "idcliente" ).isEmpty() ) {
            /// El albaran no se ha guardado y no se dispone en la base de datos
            /// de estos datos. Se utilizan en su lugar los del formulario.
            /// Verifica que exista, por lo menos, un cliente seleccionado.
            if ( mui_idcliente->id().isEmpty() ) {
                blMsgInfo ( _ ( "Tiene que seleccionar un cliente" ), this );
                return;
            } else {
                SQLQuery = "SELECT * FROM factura WHERE reffactura = '" + mui_refalbaran->text() + "' AND idcliente = " + mui_idcliente->id();
            } // end if
        } else {
            SQLQuery = "SELECT * FROM factura WHERE reffactura = '" + dbValue ( "refalbaran" ) + "' AND idcliente = " + dbValue ( "idcliente" );
        } // end if

        cur = mainCompany() ->loadQuery ( SQLQuery );

        if ( !cur->eof() ) {
            /// Informamos que ya hay una factura y que la abriremos.
            /// Si no salimos de la funci&oacute;n.
            if ( QMessageBox::question ( this,
                                         _ ( "Factura existente" ),
                                         _ ( "Existe una factura a este cliente con la misma referencia que este albaran. Desea abrirla para verificar?" ),
                                         _ ( "&Si" ), _ ( "&No" ), QString::null, 0, 1 ) ) {
                return;
            }


            int resur = g_plugins->run ( "SNewFacturaView", ( BfCompany * ) mainCompany() );
            if ( !resur ) {
                blMsgInfo ( "No se pudo crear instancia de factura" );
                return;
            } // end if
            bud = ( FacturaView * ) g_plugParams;
            mainCompany() ->m_pWorkspace->addSubWindow ( bud );
            bud->load ( cur->value( "idfactura" ) );
            bud->show();
            return;
        } // end if
        delete cur;

        /// Si el albaran no tiene lineas salimos sin crear la factura. Teniendo en cuenta que la primera linea esta en blanco.
        if ( m_listalineas->rowCount() <= 1 )
            return;

        /// Creamos la factura.
        int resur = g_plugins->run ( "SNewFacturaView", ( BfCompany * ) mainCompany() );
        if ( !resur ) {
            blMsgInfo ( "No se pudo crear instancia de factura" );
            return;
        } // end if
        bud = ( FacturaView * ) g_plugParams;
        mainCompany() ->m_pWorkspace->addSubWindow ( bud );

        /// Cargamos un elemento que no existe para inicializar bien la clase.
        bud->load ( "0" );

        /// Traspasamos los datos a la factura.
        recogeValores();
        bud->setDbValue ( "comentfactura", dbValue ( "comentalbaran" ) );
        bud->setDbValue ( "idforma_pago", dbValue ( "idforma_pago" ) );
        bud->setDbValue ( "reffactura", dbValue ( "refalbaran" ) );
        bud->setDbValue ( "idcliente", dbValue ( "idcliente" ) );
        bud->setDbValue ( "idalmacen", dbValue ( "idalmacen" ) );

        bud->getlistalineas()->setIdCliente ( dbValue ( "idcliente" ) );
        bud->getlistalineas()->setIdAlmacen ( dbValue ( "idalmacen" ) );

        /// Traspasamos las lineas
        QString l;
        BlDbSubFormRecord *linea, *linea1;
        for ( int i = 0; i < m_listalineas->rowCount(); ++i ) {
            linea = m_listalineas->lineaat ( i );
            if ( linea->dbValue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                /// Haciendo el nuevo registro antes nos evitamos problemas de foco.
                bud->getlistalineas() ->newRecord();
                bud->getlistalineas() ->setProcesarCambios ( false );
                linea1->setDbValue ( "codigocompletoarticulo", linea->dbValue ( "codigocompletoarticulo" ) );
                linea1->setDbValue ( "desclfactura", linea->dbValue ( "desclalbaran" ) );
                linea1->setDbValue ( "cantlfactura", linea->dbValue ( "cantlalbaran" ) );
                linea1->setDbValue ( "pvplfactura", linea->dbValue ( "pvplalbaran" ) );
                linea1->setDbValue ( "reqeqlfactura", linea->dbValue ( "reqeqlalbaran" ) );
                linea1->setDbValue ( "ivalfactura", linea->dbValue ( "ivalalbaran" ) );
                linea1->setDbValue ( "descuentolfactura", linea->dbValue ( "descuentolalbaran" ) );
                linea1->setDbValue ( "idarticulo", linea->dbValue ( "idarticulo" ) );
                linea1->setDbValue ( "nomarticulo", linea->dbValue ( "nomarticulo" ) );

                /// Estos traspasos no deberian ir en un plugin generico. Pero no tengo tiempo ahora de arreglarlo.
                linea1->setDbValue ( "kiloolivalfactura", linea->dbValue ( "kiloolivalalbaran" ) );
                linea1->setDbValue ( "litroolivalfactura", linea->dbValue ( "litroolivalalbaran" ) );
                linea1->setDbValue ( "rendimientolfactura", linea->dbValue ( "rendimientolalbaran" ) );
                bud->getlistalineas() ->setProcesarCambios ( true );
                linea1->refresh();
            } // end if
        } // end for

        /// Traspasamos los descuentos.
        if ( m_listadescuentos->rowCount() > 1 ) {
            for ( int i = 0; i < m_listadescuentos->rowCount(); ++i ) {
                linea1 = m_listadescuentos->lineaat ( i );
                if ( linea1->dbValue ( "proporciondalbaran" ) != "" ) {
                    linea = bud->getlistadescuentos() ->lineaat ( bud->getlistadescuentos() ->rowCount() - 1 );
                    bud->getlistadescuentos() ->setProcesarCambios ( false );
                    linea->setDbValue ( "conceptdfactura", linea1->dbValue ( "conceptdalbaran" ) );
                    linea->setDbValue ( "proporciondfactura", linea1->dbValue ( "proporciondalbaran" ) );
                    bud->getlistadescuentos() ->setProcesarCambios ( true );
                    bud->getlistadescuentos() ->newRecord();
                } // end if
            } // end for
        } // end if

        bud->pintar();
        bud->save();
        bud->close();

        mui_procesadoalbaran->setChecked ( true );

    } catch ( ... ) {
        blMsgInfo ( _ ( "Error inesperado" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try

    
}



void CompraVentaView::imprimir()
{
    BL_FUNC_DEBUG
    try {

        /// Si devuelve 0 significa que el archivo RML se ha generado mal
        /// el PDF correspondiente.
        if ( generateRML ( "compraventa.rml" ) ) {
            blCreateAndLoadPDF ( "compraventa" );
        } // end if

        

    } catch ( int e ) {
        if ( e == 100 ) {
            /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
            blMsgInfo ( _ ( "Tiene que guardar el documento antes de poder procesarlo." ), this );
            throw ( -1 );
        } // end if
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error inesperado en la impresion" ), this );
    } // end try

}

