/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#include <QtGui/QCloseEvent>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLayout>
#include <QtWidgets/QMessageBox>
#include <QtCore/QObject>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

#include "blplugins.h"
#include "albaranclientelist.h"
#include "albaranclienteview.h"
#include "clientslist.h"
#include "bfcompany.h"
#include "facturaview.h"
#include "bfinformereferencia.h"
#include "listdescfacturaview.h"
#include "listlinfacturaview.h"


/** Inicializa la pantalla.
    Inicializa todos los componentes y mete la pantalla en el workSpace.
*/
/**
\param comp
\param parent
**/
FacturaView::FacturaView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setTitleName ( _ ( "Factura" ) );
        setDbTableName ( "factura" );
        setDbFieldId ( "idfactura" );
        addDbField ( "idfactura", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Id factura" ) );
        addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Cliente" ) );
        addDbField ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Almacen" ) );
        addDbField ( "numfactura", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Numero factura" ) );
        addDbField ( "ffactura", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Ffactura" ) );
        addDbField ( "procesadafactura", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Procesada factura" ) );
        addDbField ( "codigoserie_factura", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Serie" ) );
        addDbField ( "comentfactura", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentario" ) );
        addDbField ( "reffactura", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia" ) );
        addDbField ( "descfactura", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Descripcion" ) );
        addDbField ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Trabajador" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Forma de pago" ) );

        /// Establecemos algunos Buddies.
        mui_labelAlmacen->setText ( _ ( "Al&macen" ) );
        mui_labelAlmacen->setBuddy ( mui_idalmacen );

        /// Disparamos los plugins.
        int res = g_plugins->run ( "FacturaView_FacturaView", this );
        if ( res != 0 ) {
            return;
        } // end if
        subform2->setMainCompany ( comp );
        mui_idalmacen->setMainCompany ( comp );
        mui_idforma_pago->setMainCompany ( comp );
        mui_idcliente->setMainCompany ( comp );
        m_descuentos->setMainCompany ( comp );
        mui_codigoserie_factura->setMainCompany ( comp );
        mui_reffactura->setMainCompany ( comp );
        mui_idtrabajador->setMainCompany ( comp );
      
        /// Inicializamos BfForm.
        setListaLineas ( subform2 );
        setListaDescuentos ( m_descuentos );

        /// Establecemos los parametros de busqueda del Cliente
        mui_idcliente->setLabel ( _ ( "Cliente:" ) );
        mui_idcliente->setTableName ( "cliente" );
        mui_idcliente->m_valores["cifcliente"] = "";
        mui_idcliente->m_valores["nomcliente"] = "";

        /// Establecemos valores por defecto en los combo boxes para que no se queden sin inicializar.
        mui_idalmacen->setId ( "" );
        mui_codigoserie_factura->setId ( "" );
        mui_idforma_pago->setId ( "" );
        mui_idtrabajador->setId ( "0" );

        m_totalBases->setReadOnly ( true );
        m_totalBases->setAlignment ( Qt::AlignRight );
        m_totalTaxes->setReadOnly ( true );
        m_totalTaxes->setAlignment ( Qt::AlignRight );
        m_totalDiscounts->setReadOnly ( true );
        m_totalDiscounts->setAlignment ( Qt::AlignRight );
        m_totalfactura->setReadOnly ( true );
        m_totalfactura->setAlignment ( Qt::AlignRight );
        insertWindow ( windowTitle(), this, false );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear la factura" ), this );
    }
    
}


/** Al destruir la clase provocamos que se refresque el listado de facturas.
    Para que salga actualizado.
*/
/**
**/
FacturaView::~FacturaView()
{
    BL_FUNC_DEBUG
    
}


/** Inicializa los subformularios y resetea el control de cambios.
*/
/**
**/
void FacturaView::inicializar()
{
    BL_FUNC_DEBUG
    subform2->inicializar();
    m_descuentos->inicializar();
    pintar();
    dialogChanges_readValues();
    
}


/** Pinta los campos de totales, que al no estar en la base de datos son tratados
    de forma distinta.
*/
/**
\param iva
\param base
\param total
\param desc
\param irpf
\param reqeq
**/
void FacturaView::pintatotales ( BlFixed iva, BlFixed base, BlFixed total, BlFixed desc, BlFixed irpf, BlFixed reqeq )
{
    BL_FUNC_DEBUG
    m_totalBases->setText ( base.toQString() );
    m_totalTaxes->setText ( iva.toQString() );
    m_totalfactura->setText ( total.toQString() );
    m_totalDiscounts->setText ( desc.toQString() );
    m_totalIRPF->setText ( QString ( irpf.toQString() ) );
    m_totalReqEq->setText ( QString ( reqeq.toQString() ) );
    
}




/** SLOT que responde a la pulsacion del boton mui_veralbaranes.
    Busca todos los albaranes con la misma referencia y para cada uno de ellos instancia la clase \ref AlbaranClienteView
*/
/**
**/
void FacturaView::on_mui_veralbaranes_clicked()
{
    BL_FUNC_DEBUG
    AlbaranClienteView *bud = NULL;
    BlDbRecordSet *cur = NULL;
    try {
        QString SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + dbValue ( "reffactura" ) + "'";
        cur = mainCompany() ->loadQuery ( SQLQuery );
        if ( !cur->eof() ) {
            while ( !cur->eof() ) {
	      
		/// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
		int resur = g_plugins->run ( "SNewAlbaranClienteView", mainCompany() );
		if ( !resur ) {
		    blMsgInfo ( _ ( "No se pudo crear instancia de albaran" ) );
		    return;
		} // end if
                bud = ( AlbaranClienteView * ) g_plugParams;
                mainCompany() ->m_pWorkspace->addSubWindow ( bud );
                bud->load ( cur->value( "idalbaran" ) );
                bud->show();
                cur->nextRecord();
            } // end while
        } else {
            blMsgInfo ( _ ( "No hay albaranes con esta referencia" ), this );
        } // end if
        delete cur;
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error inesperado" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try
    
}


///
/**
\param id
**/
void FacturaView::on_mui_idcliente_valueChanged ( QString id )
{
    BL_FUNC_DEBUG
    subform2->setIdCliente ( id );
    mui_idforma_pago->setIdCliente ( id );
    
}


///
/**
**/
void FacturaView::on_m_descuentos_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


///
/**
**/
void FacturaView::on_subform2_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


/** Se encarga del borrado de una factura.
*/
/**
\return
**/
int FacturaView::beforeDelete()
{
    BL_FUNC_DEBUG
    int error = m_listalineas->remove();
    error = m_listadescuentos->remove();
    
    return 0;
}


/** Carga una factura.
*/
/**
\param idbudget
\return
**/
int FacturaView::cargarPost ( QString idbudget )
{
    BL_FUNC_DEBUG

    m_listalineas->load ( idbudget );
    m_listadescuentos->load ( idbudget );

    /// Disparamos los plugins.
    g_plugins->run ( "FacturaView_cargarPost_Post", this );

    calculaypintatotales();
    
    return 0;
}

/** Pintar una factura.
*/
/**
\param idbudget
\return
**/
void FacturaView::pintarPost ( )
{
    BL_FUNC_DEBUG

    /// Escribimos como descripcion el nombre del cliente para que aparezca en el titulo y en el dockwidget
    setDescripcion( mui_idcliente->fieldValue("nomcliente") + "\n" + mui_ffactura->text());
}


/** Se encarga del guardado de una factura.
*/
/**
\return
**/
int FacturaView::afterSave()
{
    BL_FUNC_DEBUG
    m_listadescuentos->setColumnValue ( "idfactura", dbValue ( "idfactura" ) );
    m_listalineas->setColumnValue ( "idfactura", dbValue ( "idfactura" ) );
    m_listalineas->save();
    m_listadescuentos->save();
    
    /// Disparamos los plugins.
    g_plugins->run ( "FacturaView_afterSave_Post", this );    
    return 0;
}


///
/**
\param id
**/
void FacturaView::on_mui_idalmacen_valueChanged ( QString id )
{
    BL_FUNC_DEBUG
    m_listalineas->setIdAlmacen ( id );
    
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
void FacturaView::on_mui_duplicar_released()
{
    BL_FUNC_DEBUG
    FacturaView *fpv = this ;


    FacturaView *bud = NULL;
    BlDbRecordSet *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe una factura con esos datos, y en caso afirmativo lo mostramos.

        /// Creamos la factura.
        FacturaView *bud = new FacturaView ( ( BfCompany * ) fpv->mainCompany(), 0 );
        fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
	bud->inicializar();

        bud->setDbValue ( "comentfactura", fpv->dbValue ( "comentfactura" ) );
        bud->setDbValue ( "idforma_pago", fpv->dbValue ( "idforma_pago" ) );
        bud->setDbValue ( "reffactura", fpv->dbValue ( "reffactura" ) );
        bud->setDbValue ( "idcliente", fpv->dbValue ( "idcliente" ) );
        bud->pintar();
        bud->show();

        /// Traspasamos las lineas de factura
        QString l;
        BlDbSubFormRecord *linea, *linea1;
        for ( int i = 0; i < fpv->m_listalineas->rowCount(); ++i ) {
            linea = fpv->m_listalineas->lineaat ( i );
            if ( linea->dbValue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                bud->getlistalineas() ->newRecord();
                bud->getlistalineas() ->setProcesarCambios ( false );
                linea1->setDbValue ( "desclfactura", linea->dbValue ( "desclfactura" ) );
                linea1->setDbValue ( "cantlfactura", linea->dbValue ( "cantlfactura" ) );
                linea1->setDbValue ( "pvplfactura", linea->dbValue ( "pvplfactura" ) );
                linea1->setDbValue ( "descuentolfactura", linea->dbValue ( "descuentolfactura" ) );
                linea1->setDbValue ( "idarticulo", linea->dbValue ( "idarticulo" ) );
                linea1->setDbValue ( "codigocompletoarticulo", linea->dbValue ( "codigocompletoarticulo" ) );
                linea1->setDbValue ( "nomarticulo", linea->dbValue ( "nomarticulo" ) );
                linea1->setDbValue ( "ivalfactura", linea->dbValue ( "ivalfactura" ) );
                linea1->setDbValue ( "reqeqlfactura", linea->dbValue ( "reqeqlfactura" ) );
            } // end if
        } // end for
        bud->calculaypintatotales();

    } catch ( ... ) {
        blMsgInfo ( _ ( "Error inesperado duplicando la factura" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try

}




