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

#include <fstream>

#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QLayout>
#include <QTextStream>

#include "facturapview.h"
#include "bfcompany.h"
#include "listlinfacturapview.h"
#include "providerslist.h"
#include "blfunctions.h"
#include "albaranproveedorview.h"


/** Inicializa todos los componentes de la clase.
    mete la ventana en el workSpace. */
/**
\param comp
\param parent
**/
FacturaProveedorView::FacturaProveedorView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        setupUi ( this );

        setTitleName ( _ ( "Factura Proveedor" ) );
        setDbTableName ( "facturap" );
        setDbFieldId ( "idfacturap" );
        addDbField ( "idfacturap", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Id facturap" ) );
        addDbField ( "idproveedor", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id proveedor" ) );
        addDbField ( "numfacturap", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero" ) );
        addDbField ( "ffacturap", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha" ) );
        addDbField ( "procesadafacturap", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Procesada facturap" ) );
        addDbField ( "comentfacturap", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentario facturap" ) );
        addDbField ( "reffacturap", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia facturap" ) );
        addDbField ( "descfacturap", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Descripcion facturap" ) );
        addDbField ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Id trabajador" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Id forma de pago" ) );

        /// Disparamos los plugins.
        int res = g_plugins->run ( "FacturaProveedorView_FacturaProveedorView", this );
        if ( res != 0 ) {
            return;
        } // end if

        subform2->setMainCompany ( comp );
        m_descuentos->setMainCompany ( comp );
        mui_idforma_pago->setMainCompany ( comp );
        mui_idforma_pago->setId ( "" );
        mui_idproveedor->setMainCompany ( comp );
        mui_idproveedor->setId ( "" );
        mui_reffacturap->setMainCompany ( comp );

        /// Establecemos los parametros de busqueda del Cliente
        mui_idproveedor->setLabel ( _ ( "Proveedor:" ) );
        mui_idproveedor->setTableName ( "proveedor" );
        mui_idproveedor->m_valores["cifproveedor"] = "";
        mui_idproveedor->m_valores["nomproveedor"] = "";

        setListaLineas ( subform2 );
        setListaDescuentos ( m_descuentos );

        m_totalBases->setReadOnly ( TRUE );
        m_totalBases->setAlignment ( Qt::AlignRight );
        m_totalTaxes->setReadOnly ( TRUE );
        m_totalTaxes->setAlignment ( Qt::AlignRight );
        m_totalDiscounts->setReadOnly ( TRUE );
        m_totalDiscounts->setAlignment ( Qt::AlignRight );
        m_totalfacturap->setReadOnly ( TRUE );
        m_totalfacturap->setAlignment ( Qt::AlignRight );
        insertWindow ( windowTitle(), this, FALSE );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear la factura proveedor" ), this );
    } // end try
    
}


/** Al destruir una factura de proveedor se hace un refresco del listado
    de facturas de proveedor. */
/**
**/
FacturaProveedorView::~FacturaProveedorView()
{
    BL_FUNC_DEBUG
    

}

///
/**
**/
void FacturaProveedorView::on_m_descuentos_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


///
/**
**/
void FacturaProveedorView::on_subform2_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


/** inicializar debe ser invocado cuando se crea una nueva ficha sin cargar ningun
    date de la base de datos (por ejemplo una nueva ficha).
    Sirve para inicializar los componenetes sin necesidad de query alguno */
/**
**/
void FacturaProveedorView::inicializar()
{
    BL_FUNC_DEBUG
    subform2->inicializar();
    m_descuentos->inicializar();
    pintar();
    dialogChanges_readValues();
    
}


/** Pinta los totales de la Factura de proveedor.
    Este metodo se llama desde FacturaProveedor. */
/**
\param iva
\param base
\param total
\param desc
\param irpf
\param reqeq
**/
void FacturaProveedorView::pintatotales ( BlFixed iva, BlFixed base, BlFixed total, BlFixed desc, BlFixed irpf, BlFixed reqeq )
{
    BL_FUNC_DEBUG
    m_totalBases->setText ( QString ( base.toQString() ) );
    m_totalTaxes->setText ( QString ( iva.toQString() ) );
    m_totalfacturap->setText ( QString ( total.toQString() ) );
    m_totalDiscounts->setText ( QString ( desc.toQString() ) );
    m_totalIRPF->setText ( QString ( irpf.toQString() ) );
    m_totalReqEq->setText ( QString ( reqeq.toQString() ) );
    
}



/** SLOT que responde  a la pulsacion del boton ver albaranes.
    Consulta todos los albaranes con la misma referencia.
    Los instancia y los carga. */
/**
**/
void FacturaProveedorView::on_mui_veralbaranes_clicked()
{
    BL_FUNC_DEBUG
    QString query = "SELECT * FROM albaranp WHERE refalbaranp='" + dbValue ( "reffacturap" ) + "'";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    while ( !cur->eof() ) {
	/// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
	int resur = g_plugins->run ( "SNewAlbaranProveedorView", ( BfCompany * ) mainCompany() );
	if ( !resur ) {
	    blMsgInfo (_( "no se pudo crear instancia de albaran proveedor" ));
	    return;
	} // end if
	AlbaranProveedorView * albpro = ( AlbaranProveedorView * ) g_plugParams;
        albpro->load ( cur->value( "idalbaranp" ) );
        mainCompany() ->m_pWorkspace->addSubWindow ( albpro );
        albpro->show();
        cur->nextRecord();
    } // end while
    delete cur;
    
}


///
/**
\param id
**/
void FacturaProveedorView::on_mui_idproveedor_valueChanged ( QString id )
{
    BL_FUNC_DEBUG
    subform2->setIdProveedor ( id );
    mui_idforma_pago->setIdProveedor ( id );
    
}


/** Hace el borrado de la factura.
    Primero borra las lineas de factura.
    Luego borra los descuentos de factura.
    Y por ultimo delega en BlDbRecord el borrado del registro.
    Si algo falla devuelve -1.
    Si todo va bien devuelve 0.
*/
/**
\return
**/
int FacturaProveedorView::beforeDelete()
{
    BL_FUNC_DEBUG
    m_listalineas->remove();
    m_listadescuentos->remove();
    
    return 0;
}



/** Carga un FacturaProveedor.
    Carga el BlDbRecord con DBload y luego llama a la carga de las lineas y los descuentos.
    Al finalizar hace un pintado para que la informacion se vea actualizada.
    Si algo falla genera una excepcion -1.
    Si todo va bien devuelve 0.
*/
/**
\param idfacturap
\return
**/
int FacturaProveedorView::cargarPost ( QString idfacturap )
{
    BL_FUNC_DEBUG
    m_listalineas->load ( idfacturap );
    m_listadescuentos->load ( idfacturap );

    /// Disparamos los plugins.
    g_plugins->run ( "FacturaProveedorView_cargarPost_Post", this );

    calculaypintatotales();
    
    return 0;
}

/** Hace el guardado de una Factura de Proveedor.
    guarda el registro a traves de BlDbRecord y luego actualiza el identificador de facturap en
    las lineas y en los descuentos para guardarlos.
    Si algo falla genera una excepcion -1.
    Si todo va bien devuelve 0.
*/
/**
\return
**/
int FacturaProveedorView::afterSave()
{
    BL_FUNC_DEBUG
    m_listalineas->setColumnValue ( "idfacturap", dbValue ( "idfacturap" ) );
    m_listadescuentos->setColumnValue ( "idfacturap", dbValue ( "idfacturap" ) );
    m_listalineas->save();
    m_listadescuentos->save();
    
    /// Disparamos los plugins.
    g_plugins->run ( "FacturaProveedorView_afterSave_Post", this );
    
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
void FacturaProveedorView::on_mui_duplicar_released()
{
    BL_FUNC_DEBUG
    FacturaProveedorView *fpv = this ;


    FacturaProveedorView *bud = NULL;
    BlDbRecordSet *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe una factura con esos datos, y en caso afirmativo lo mostramos.

        /// Creamos la factura.
        FacturaProveedorView *bud = new FacturaProveedorView ( ( BfCompany * ) fpv->mainCompany(), 0 );
        fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );

        bud->setDbValue ( "comentfacturap", fpv->dbValue ( "comentfacturap" ) );
        bud->setDbValue ( "idforma_pago", fpv->dbValue ( "idforma_pago" ) );
        bud->setDbValue ( "reffacturap", fpv->dbValue ( "reffacturap" ) );
        bud->setDbValue ( "idproveedor", fpv->dbValue ( "idproveedor" ) );
        bud->inicializar();
        bud->show();

        /// Traspasamos las lineas de factura
        QString l;
        BlDbSubFormRecord *linea, *linea1;
        for ( int i = 0; i < fpv->m_listalineas->rowCount(); ++i ) {
            linea = fpv->m_listalineas->lineaat ( i );
            if ( linea->dbValue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                bud->getlistalineas() ->newRecord();
                bud->getlistalineas() ->setProcesarCambios ( FALSE );
                linea1->setDbValue ( "desclfacturap", linea->dbValue ( "desclfacturap" ) );
                linea1->setDbValue ( "cantlfacturap", linea->dbValue ( "cantlfacturap" ) );
                linea1->setDbValue ( "pvplfacturap", linea->dbValue ( "pvplfacturap" ) );
                linea1->setDbValue ( "descuentolfacturap", linea->dbValue ( "descuentolfacturap" ) );
                linea1->setDbValue ( "idarticulo", linea->dbValue ( "idarticulo" ) );
                linea1->setDbValue ( "codigocompletoarticulo", linea->dbValue ( "codigocompletoarticulo" ) );
                linea1->setDbValue ( "nomarticulo", linea->dbValue ( "nomarticulo" ) );
                linea1->setDbValue ( "ivalfacturap", linea->dbValue ( "ivalfacturap" ) );
                linea1->setDbValue ( "reqeqlfacturap", linea->dbValue ( "reqeqlfacturap" ) );
            } // end if
        } // end for
        bud->calculaypintatotales();

    } catch ( ... ) {
        blMsgInfo ( _ ( "Error inesperado" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try

}




