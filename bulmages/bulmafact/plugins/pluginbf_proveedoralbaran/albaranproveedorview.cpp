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

#include "albaranproveedorview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "blfunctions.h"
#include "bfproveedoralbaransubform.h"
#include "pedidoproveedorview.h"
#include "blplugins.h"
#include "bltoolbutton.h"


/** Inicializa todos los componentes de la ventana.
    Mete la ventana en el workSpace.
*/
/**
\param comp
\param parent
\return
**/
AlbaranProveedorView::AlbaranProveedorView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );
        setTitleName ( _ ( "Albaran de proveedor" ) );
        setDbTableName ( "albaranp" );
        setDbFieldId ( "idalbaranp" );
        addDbField ( "idalbaranp", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Id albaran proveedor" ) );
        addDbField ( "numalbaranp", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Numero albaran proveedor" ) );
        addDbField ( "fechaalbaranp", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha albaran proveedor" ) );
        addDbField ( "comentalbaranp", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentario albaran proveedor" ) );
        addDbField ( "idproveedor", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id proveedor" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Id forma de pago" ) );
        addDbField ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id almacen" ) );
        addDbField ( "procesadoalbaranp", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Albaran procesado" ) );
        addDbField ( "refalbaranp", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia albaran proveedor" ) );
        addDbField ( "descalbaranp", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Descripcion albaran proveedor" ) );
        addDbField ( "irpfalbaranp", BlDbField::DbNumeric, BlDbField::DbNothing, _ ( "IRPF albaran" ) );

        /// Disparamos los plugins.
        int res = g_plugins->run ( "AlbaranProveedorView_AlbaranProveedorView", this );
        if ( res != 0 ) {
            return;
        } // end if
        subform2->setMainCompany ( comp );
        mui_idalmacen->setMainCompany ( comp );
        mui_idforma_pago->setMainCompany ( comp );
        mui_idproveedor->setMainCompany ( comp );
        m_descuentos->setMainCompany ( comp );
        mui_refalbaranp->setMainCompany ( comp );

        /// Inicializamos BfForm.
        setListaLineas ( subform2 );
        setListaDescuentos ( m_descuentos );

        /// Establecemos los parametros de busqueda del Cliente
        mui_idproveedor->setLabel ( _ ( "Proveedor:" ) );
        mui_idproveedor->setTableName ( "proveedor" );
        mui_idproveedor->m_valores["cifproveedor"] = "";
        mui_idproveedor->m_valores["nomproveedor"] = "";

        m_totalBases->setReadOnly ( true );
        m_totalBases->setAlignment ( Qt::AlignRight );
        m_totalTaxes->setReadOnly ( true );
        m_totalTaxes->setAlignment ( Qt::AlignRight );
        m_totalDiscounts->setReadOnly ( true );
        m_totalDiscounts->setAlignment ( Qt::AlignRight );
        m_totalalbaranp->setReadOnly ( true );
        m_totalalbaranp->setAlignment ( Qt::AlignRight );
        mui_idforma_pago->setId ( "0" );
        mui_idalmacen->setId ( "0" );
	
	/// Calculamos el IRPF y lo ponemos
        QString query = "SELECT tasairpf FROM irpf WHERE fechairpf <= now()::DATE ORDER BY fechairpf DESC LIMIT 1";
	BlDbRecordSet *cur = mainCompany() ->loadQuery ( query);
	if ( cur ) {
	    if ( !cur->eof() ) {
		mui_irpfalbaranp -> setText( cur->value( "tasairpf" ) );
		setDbValue("irpfalbaranp", cur->value("tasairpf") );
	    } // end if
	    delete cur;
	} // end if
	
        insertWindow ( windowTitle(), this, false );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el albaran proveedor" ), this );
    } // end try
    
}


/** Hace que el listado de Albaranes se refresque
*/
/**
**/
AlbaranProveedorView::~AlbaranProveedorView()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void AlbaranProveedorView::on_m_descuentos_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


///
/**
**/
void AlbaranProveedorView::on_subform2_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


/** Inicializa los elementos de la ventana.
    Este metodo es invocado al crear un nuevo Albaran de Proveedor sin
    tener que hacer una carga del mismo.
*/
/**
**/
void AlbaranProveedorView::inicializar()
{
    BL_FUNC_DEBUG
    subform2->inicializar();
    m_descuentos->inicializar();
    pintar();
    dialogChanges_readValues();
    
}


/// Pinta los totales indicados en sus correspondientes textEdit.
/**
\param iva
\param base
\param desc
\param irpf
\param reqeq
**/
void AlbaranProveedorView::pintatotales ( BlFixed iva, BlFixed base, BlFixed total, BlFixed desc, BlFixed irpf, BlFixed reqeq )
{
    BL_FUNC_DEBUG
    m_totalBases->setText ( QString ( base.toQString() ) );
    m_totalTaxes->setText ( QString ( iva.toQString() ) );
    m_totalDiscounts->setText ( QString ( desc.toQString() ) );
    m_totalIRPF->setText ( QString ( irpf.toQString() ) );
    m_totalReqEq->setText ( QString ( reqeq.toQString() ) );
    m_totalalbaranp->setText ( QString ( total.toQString() ) );
    
}


/** SLOT que responde a la solicitud de ver todos los pedidos de proveedor
    relacionados con el albaran abierto.
    Realiza una consulta sobre los pedidos de proveedor que tienen la misma
    referencia que este pedido de proveedor y los instancia y muestra.
    NOTA: Este plugin debe ser trasladado al plugin de pedidos de proveedor.
*/
/**
**/
void AlbaranProveedorView::on_mui_verpedidosproveedor_clicked()
{
    BL_FUNC_DEBUG
    QString query = "SELECT * FROM pedidoproveedor WHERE refpedidoproveedor = '" + dbValue ( "refalbaranp" ) + "'";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    while ( !cur->eof() ) {
	/// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
	int resur = g_plugins->run ( "SNewPedidoProveedorView", ( BfCompany * ) mainCompany() );
	if ( !resur ) {
	      blMsgInfo (_( "no se pudo crear instancia de pedido proveedor" ));
	      return;
	} // end if
	PedidoProveedorView * pedpro = ( PedidoProveedorView * ) g_plugParams;
        pedpro->load ( cur->value( "idpedidoproveedor" ) );
        mainCompany() ->m_pWorkspace->addSubWindow ( pedpro );
        pedpro->show();
        cur->nextRecord();
    } // end while
    delete cur;
    
}


///
/**
\param id
**/
void AlbaranProveedorView::on_mui_idproveedor_valueChanged ( QString id )
{
    BL_FUNC_DEBUG
    subform2->setIdProveedor ( id );
    mui_idforma_pago->setIdProveedor ( id );
    
}


/** Se encarga del borrado en la base de datos de una albaran de proveedor.
    Si se produce algun error devuelve una excepcion.
    Primero llama al borrado de las lineas y luego a los descuentos.
    Por ultimo llama al borrado del registro correspondiente en albaranp.
*/
/**
\return
**/
int AlbaranProveedorView::beforeDelete()
{
    BL_FUNC_DEBUG
    m_listalineas->remove();
    m_listadescuentos->remove();
    
    return 0;
}


/// Esta funcion carga un AlbaranProveedor.
/// Hace el query adecuado, carga el registro a traves de BlDbRecord.
/// Hace la carga de las lineas y de los descuentos.
/// Invoca al pintado.
/**
\param idbudget
\return
**/
int AlbaranProveedorView::cargarPost ( QString idbudget )
{
    BL_FUNC_DEBUG

    m_listalineas->load ( idbudget );
    m_listadescuentos->load ( idbudget );

    /// Disparamos los plugins.
    g_plugins->run ( "AlbaranProveedorView_cargarPost_Post", this );

    calculaypintatotales();
    
    return 0;
}


/** Guarda el albaran de proveedor en la base de datos.
    Para ello hace el guardado de BlDbRecord y luego guarda las lineas y los descuentos.
    Una vez hecho el guardado se hace una carga para recuperar posibles datos que haya
    introducido la base de datos como la referencia.

    Si algo falla devuelve una excepcion -1.
*/
/**
\return
**/
int AlbaranProveedorView::afterSave()
{
    BL_FUNC_DEBUG

    m_listalineas->setColumnValue ( "idalbaranp", dbValue ( "idalbaranp" ) );
    m_listadescuentos->setColumnValue ( "idalbaranp", dbValue ( "idalbaranp" ) );

    m_listalineas->save();
    m_listadescuentos->save();

    
    return 0;
}

/** Pintar un albaran de proveedor
*/
void AlbaranProveedorView::pintarPost ( )
{
    BL_FUNC_DEBUG

    /// Escribimos como descripcion el nombre del proveedor para que aparezca en el titulo y en el dockwidget
    setDescripcion( mui_idproveedor->fieldValue("nomproveedor") + "\n" + mui_fechaalbaranp->text());
}


/// Se encarga de generar una albaran a partir de un albar&aacute;n.
/** Primero de todo busca una albaran por referencia que tenga este albaran.
    Si dicha albaran existe entonces la cargamos y terminamos.
    Si no existe dicha albaran el sistema avisa y permite crear una poniendo
    Todos los datos del albaran automaticamente en ella.
*/
/**
\return
**/
void AlbaranProveedorView::on_mui_duplicar_released()
{
    BL_FUNC_DEBUG
    AlbaranProveedorView *fpv = this ;


    AlbaranProveedorView *bud = NULL;
    BlDbRecordSet *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe una albaran con esos datos, y en caso afirmativo lo mostramos.

        /// Creamos la albaran.
        AlbaranProveedorView *bud = new AlbaranProveedorView ( ( BfCompany * ) fpv->mainCompany(), 0 );
        fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
	bud->inicializar();

        bud->setDbValue ( "comentalbaranp", fpv->dbValue ( "comentalbaranp" ) );
        bud->setDbValue ( "idforma_pago", fpv->dbValue ( "idforma_pago" ) );
        bud->setDbValue ( "refalbaranp", fpv->dbValue ( "refalbaranp" ) );
        bud->setDbValue ( "idproveedor", fpv->dbValue ( "idproveedor" ) );
        bud->pintar();
        bud->show();

        /// Traspasamos las lineas de albaran
        QString l;
        BlDbSubFormRecord *linea, *linea1;
        for ( int i = 0; i < fpv->m_listalineas->rowCount(); ++i ) {
            linea = fpv->m_listalineas->lineaat ( i );
            if ( linea->dbValue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                bud->getlistalineas() ->newRecord();
                bud->getlistalineas() ->setProcesarCambios ( false );
                linea1->setDbValue ( "desclalbaranp", linea->dbValue ( "desclalbaranp" ) );
                linea1->setDbValue ( "cantlalbaranp", linea->dbValue ( "cantlalbaranp" ) );
                linea1->setDbValue ( "pvplalbaranp", linea->dbValue ( "pvplalbaranp" ) );
                linea1->setDbValue ( "descuentolalbaranp", linea->dbValue ( "descuentolalbaranp" ) );
                linea1->setDbValue ( "idarticulo", linea->dbValue ( "idarticulo" ) );
                linea1->setDbValue ( "codigocompletoarticulo", linea->dbValue ( "codigocompletoarticulo" ) );
                linea1->setDbValue ( "nomarticulo", linea->dbValue ( "nomarticulo" ) );
                linea1->setDbValue ( "ivalalbaranp", linea->dbValue ( "ivalalbaranp" ) );
                linea1->setDbValue ( "reqeqlalbaranp", linea->dbValue ( "reqeqlalbaranp" ) );
            } // end if
        } // end for
        bud->calculaypintatotales();

    } catch ( ... ) {
        blMsgInfo ( _ ( "Error inesperado" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try

}







