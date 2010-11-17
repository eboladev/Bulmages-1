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

#include <QCloseEvent>
#include <QComboBox>
#include <QLayout>
#include <QMessageBox>
#include <QObject>
#include <QToolButton>
#include <QWidget>

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
    blDebug ( "FacturaView::FacturaView", 0 );
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
        int res = g_plugins->lanza ( "FacturaView_FacturaView", this );
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

        m_totalBases->setReadOnly ( TRUE );
        m_totalBases->setAlignment ( Qt::AlignRight );
        m_totalTaxes->setReadOnly ( TRUE );
        m_totalTaxes->setAlignment ( Qt::AlignRight );
        m_totalDiscounts->setReadOnly ( TRUE );
        m_totalDiscounts->setAlignment ( Qt::AlignRight );
        m_totalfactura->setReadOnly ( TRUE );
        m_totalfactura->setAlignment ( Qt::AlignRight );
        meteWindow ( windowTitle(), this, FALSE );
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear la factura" ), this );
    }
    blDebug ( "END FacturaView::FacturaView" );
}


/** Al destruir la clase provocamos que se refresque el listado de facturas.
    Para que salga actualizado.
*/
/**
**/
FacturaView::~FacturaView()
{
    blDebug ( "FacturaView::~FacturaView", 0 );
    blDebug ( "END FacturaView::~FacturaView", 0 );
}


/** Inicializa los subformularios y resetea el control de cambios.
*/
/**
**/
void FacturaView::inicializar()
{
    blDebug ( "FacturaView::inicializar", 0 );
    subform2->inicializar();
    m_descuentos->inicializar();
    pintar();
    dialogChanges_cargaInicial();
    blDebug ( "END FacturaView::inicializar", 0 );
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
    blDebug ( "FacturaView::pintatotales", 0 );
    m_totalBases->setText ( base.toQString() );
    m_totalTaxes->setText ( iva.toQString() );
    m_totalfactura->setText ( total.toQString() );
    m_totalDiscounts->setText ( desc.toQString() );
    m_totalIRPF->setText ( QString ( irpf.toQString() ) );
    m_totalReqEq->setText ( QString ( reqeq.toQString() ) );
    blDebug ( "END FacturaView::pintatotales", 0 );
}



/** SLOT que responde a la pulsacion del boton mui_agregaralbaran.
    Muestra un selector de albaranes y una vez seleccionado un albaran hace el agregado
    de todas las lineas de este.
*/
/**
**/
void FacturaView::on_mui_agregaralbaran_clicked()
{
    blDebug ( "FacturaView::on_mui_agregaralbaran_clicked", 0 );

    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );
    AlbaranClienteList *fac = new AlbaranClienteList ( mainCompany(), diag, 0, BL_SELECT_MODE );
    connect ( fac, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

    /// Hacemos que las opciones de filtrado del listado ya esten bien.
    fac->m_cliente->setId ( dbValue ( "idcliente" ) );
    fac->on_mui_actualizar_clicked();

    /// Lanzamos el dialogo.
    diag->exec();
    QString idalbaran = fac->idCliDelivNote();
    delete diag;

    /// Si no hay idfactura es que hemos abortado y por tanto cancelamos la operacion.
    if ( idalbaran == "" )
        return;

    /// Creamos la factura.
    AlbaranClienteView *bud = new AlbaranClienteView ( mainCompany(), NULL );
    bud->cargar ( idalbaran );

    /// Agregamos a comentarios que albaran se corresponde.
    QString comm = dbValue ( "comentfactura" ) + "(" + _ ( "ALBARAN: Num " ) + bud->dbValue ( "numalbaran" ) + _ ( "Ref:" ) + " " + bud->dbValue ( "refalbaran" ) + _ ( "Fecha:" ) + " " + bud->dbValue ( "fechaalbaran" ) + ")\n";

    setDbValue ( "comentfactura", comm );
    pintar();

    /// EN TEORIA SE DEBERIA COMPROBAR QUE LA FACTURA Y EL ALBARAN SON DEL MISMO CLIENTE, pero por ahora no lo hacemos.
    BlDbSubFormRecord *linea, *linea1;
    for ( int i = 0; i < bud->getlistalineas() ->rowCount(); ++i ) {
        linea = bud->getlistalineas() ->lineaat ( i );
        /// Los registros vacios no se tienen en cuenta.
        if ( linea->dbValue ( "idarticulo" ) != "" ) {
            linea1 = getlistalineas() ->lineaat ( getlistalineas() ->rowCount() - 1 );
            linea1->setDbValue ( "codigocompletoarticulo", linea->dbValue ( "codigocompletoarticulo" ) );
            linea1->setDbValue ( "desclfactura", linea->dbValue ( "desclalbaran" ) );
            linea1->setDbValue ( "cantlfactura", linea->dbValue ( "cantlalbaran" ) );
            linea1->setDbValue ( "pvplfactura", linea->dbValue ( "pvplalbaran" ) );
            linea1->setDbValue ( "descuentolfactura", linea->dbValue ( "descuentolalbaran" ) );
            linea1->setDbValue ( "idarticulo", linea->dbValue ( "idarticulo" ) );
            linea1->setDbValue ( "nomarticulo", linea->dbValue ( "nomarticulo" ) );
            linea1->setDbValue ( "ivalfactura", linea->dbValue ( "ivalalbaran" ) );
            getlistalineas() ->nuevoRegistro();
        } // end if
    } // end for

    /// Procesamos el albaran.
    bud->mui_procesadoalbaran->setChecked ( TRUE );
    bud->guardar();
    delete bud;

    /// Pintamos los totales.
    calculaypintatotales();
    blDebug ( "END FacturaView::on_mui_agregaralbaran_clicked", 0 );
}


/** SLOT que responde a la pulsacion del boton mui_veralbaranes.
    Busca todos los albaranes con la misma referencia y para cada uno de ellos instancia la clase \ref AlbaranClienteView
*/
/**
**/
void FacturaView::on_mui_veralbaranes_clicked()
{
    blDebug ( "FacturaView::on_mui_veralbaranes_clicked", 0 );
    AlbaranClienteView *bud = NULL;
    BlDbRecordSet *cur = NULL;
    try {
        QString SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + dbValue ( "reffactura" ) + "'";
        cur = mainCompany() ->loadQuery ( SQLQuery );
        if ( !cur->eof() ) {
            while ( !cur->eof() ) {
                bud = new AlbaranClienteView ( mainCompany(), NULL );
                mainCompany() ->m_pWorkspace->addSubWindow ( bud );
                bud->cargar ( cur->valor ( "idalbaran" ) );
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
    blDebug ( "END FacturaView::on_mui_veralbaranes_clicked", 0 );
}


///
/**
\param id
**/
void FacturaView::on_mui_idcliente_valueChanged ( QString id )
{
    blDebug ( "FacturaView::on_m_cliente_valueChanged", 0 );
    subform2->setIdCliente ( id );
    mui_idforma_pago->setIdCliente ( id );
    blDebug ( "END FacturaView::on_m_cliente_valueChanged", 0 );
}


///
/**
**/
void FacturaView::on_m_descuentos_editFinish ( int, int )
{
    blDebug ( "FacturaView::on_m_descuentos_editFinish", 0 );
    calculaypintatotales();
    blDebug ( "END FacturaView::on_m_descuentos_editFinish", 0 );
}


///
/**
**/
void FacturaView::on_subform2_editFinish ( int, int )
{
    blDebug ( "FacturaView::on_subform2_editFinish", 0 );
    calculaypintatotales();
    blDebug ( "END FacturaView::on_subform2_editFinish", 0 );
}


/** Se encarga del borrado de una factura.
*/
/**
\return
**/
int FacturaView::borrarPre()
{
    blDebug ( "FacturaView::borrarPre", 0 );
    int error = m_listalineas->borrar();
    error = m_listadescuentos->borrar();
    blDebug ( "END FacturaView::borrarPre", 0 );
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
    blDebug ( "FacturaView::cargarPost", 0 );

    m_listalineas->cargar ( idbudget );
    m_listadescuentos->cargar ( idbudget );

    /// Disparamos los plugins.
    g_plugins->lanza ( "FacturaView_cargarPost_Post", this );

    calculaypintatotales();

    blDebug ( "END FacturaView::cargarPost", 0 );
    return 0;
}


/** Se encarga del guardado de una factura.
*/
/**
\return
**/
int FacturaView::guardarPost()
{
    blDebug ( "FacturaView::guardarPost", 0 );
    m_listadescuentos->setColumnValue ( "idfactura", dbValue ( "idfactura" ) );
    m_listalineas->setColumnValue ( "idfactura", dbValue ( "idfactura" ) );
    m_listalineas->guardar();
    m_listadescuentos->guardar();
    blDebug ( "END FacturaView::guardarPost", 0 );
    return 0;
}


///
/**
\param id
**/
void FacturaView::on_mui_idalmacen_valueChanged ( QString id )
{
    blDebug ( "FacturaView::on_mui_idalmacen_valueChanged", 0 );
    m_listalineas->setIdAlmacen ( id );
    blDebug ( "END FacturaView::on_mui_idalmacen_valueChanged", 0 );
}



