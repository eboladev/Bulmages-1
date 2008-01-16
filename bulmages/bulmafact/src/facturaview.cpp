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

#include <QCloseEvent>
#include <QComboBox>
#include <QLayout>
#include <QMessageBox>
#include <QObject>
#include <QToolButton>
#include <QWidget>

#include <fstream>

#include "albaranclientelist.h"
#include "albaranclienteview.h"
#include "clientslist.h"
#include "cobroview.h"
#include "company.h"
#include "facturaview.h"
#include "funcaux.h"
#include "informereferencia.h"
#include "listdescfacturaview.h"
#include "listlinfacturaview.h"
#include "plugins.h"


/** Inicializa la pantalla.
    Inicializa todos los componentes y mete la pantalla en el workSpace.
*/
/**
\param comp
\param parent
**/
FacturaView::FacturaView ( Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "FacturaView::FacturaView", 0 );
    setupUi ( this );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setTitleName ( tr ( "Factura" ) );
        setDBTableName ( "factura" );
        setDBCampoId ( "idfactura" );
        addDBCampo ( "idfactura", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate ( "Factura", "Id factura" ) );
        addDBCampo ( "idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate ( "Factura", "Id cliente" ) );
        addDBCampo ( "idalmacen", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate ( "Factura", "Id almacen" ) );
        addDBCampo ( "numfactura", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "Factura", "Numero factura" ) );
        addDBCampo ( "ffactura", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate ( "Factura", "Ffactura" ) );
        addDBCampo ( "procesadafactura", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate ( "Factura", "Procesada factura" ) );
        addDBCampo ( "codigoserie_factura", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Factura", "Codigo serie factura" ) );
        addDBCampo ( "comentfactura", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Factura", "Comentario factura" ) );
        addDBCampo ( "reffactura", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Factura", "Referencia factura" ) );
        addDBCampo ( "descfactura", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Factura", "Descripcion factura" ) );
        addDBCampo ( "idtrabajador", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "Factura", "Id trabajador" ) );
        addDBCampo ( "idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "Factura", "Id formad de pago" ) );

        /// Establecemos algunos Buddies.
        mui_labelAlmacen->setText ( tr ( "Al&macen" ) );
        mui_labelAlmacen->setBuddy ( mui_idalmacen );

        /// Disparamos los plugins.
        int res = g_plugins->lanza ( "FacturaView_FacturaView", this );
        if ( res != 0 ) {
            return;
        } // end if
        subform2->setEmpresaBase ( comp );
        mui_idalmacen->setEmpresaBase ( comp );
        mui_idalmacen->setidalmacen ( "" );
        mui_idforma_pago->setEmpresaBase ( comp );
        mui_idforma_pago->setidforma_pago ( "" );
        mui_idcliente->setEmpresaBase ( comp );
        m_descuentos->setEmpresaBase ( comp );
        mui_codigoserie_factura->setEmpresaBase ( comp );
        mui_codigoserie_factura->setcodigoserie_factura ( "" );
        mui_reffactura->setEmpresaBase ( comp );

        /// Inicializamos FichaBf.
        setListaLineas ( subform2 );
        setListaDescuentos ( m_descuentos );

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
        mensajeInfo ( tr ( "Error al crear la factura" ) );
    }
    _depura ( "END FacturaView::FacturaView" );
}


/** Al destruir la clase provocamos que se refresque el listado de facturas.
    Para que salga actualizado.
*/
/**
**/
FacturaView::~FacturaView()
{
    _depura ( "FacturaView::~FacturaView", 0 );
    empresaBase() ->refreshFacturas();
    _depura ( "END FacturaView::~FacturaView", 0 );
}


/** Inicializa los subformularios y resetea el control de cambios.
*/
/**
**/
void FacturaView::inicializar()
{
    _depura ( "FacturaView::inicializar", 0 );
    subform2->inicializar();
    m_descuentos->inicializar();
    pintar();
    dialogChanges_cargaInicial();
    _depura ( "END FacturaView::inicializar", 0 );
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
void FacturaView::pintatotales ( Fixed iva, Fixed base, Fixed total, Fixed desc, Fixed irpf, Fixed reqeq )
{
    _depura ( "FacturaView::pintatotales", 0 );
    m_totalBases->setText ( base.toQString() );
    m_totalTaxes->setText ( iva.toQString() );
    m_totalfactura->setText ( total.toQString() );
    m_totalDiscounts->setText ( desc.toQString() );
    m_totalIRPF->setText ( QString ( irpf.toQString() ) );
    m_totalReqEq->setText ( QString ( reqeq.toQString() ) );
    _depura ( "END FacturaView::pintatotales", 0 );
}


/** SLOT que responde a la pulsacion del boton mui_cobrar
    Crea una instancia de la clase \ref CobroView y lo inicializa con los datos
    de la factura.
*/
/**
**/
void FacturaView::on_mui_cobrar_clicked()
{
    _depura ( "FacturaView::on_mui_cobrar_clicked", 0 );
    CobroView *bud = empresaBase() ->newCobroView();
    empresaBase() ->m_pWorkspace->addWindow ( bud );
    bud->setDBvalue ( "idcliente", DBvalue ( "idcliente" ) );
    bud->setDBvalue ( "cantcobro", m_totalfactura->text() );
    bud->setDBvalue ( "refcobro", DBvalue ( "reffactura" ) );
    bud->setDBvalue ( "comentcobro", DBvalue ( "descfactura" ) );
    bud->pintar();
    bud->show();
    _depura ( "END FacturaView::on_mui_cobrar_clicked", 0 );
}


/** SLOT que responde a la pulsacion del boton mui_agregaralbaran.
    Muestra un selector de albaranes y una vez seleccionado un albaran hace el agregado
    de todas las lineas de este.
*/
/**
**/
void FacturaView::on_mui_agregaralbaran_clicked()
{
    _depura ( "FacturaView::on_mui_agregaralbaran_clicked", 0 );
    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );
    AlbaranClienteList *fac = new AlbaranClienteList ( empresaBase(), diag, 0, AlbaranClienteList::SelectMode );
    connect ( fac, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

    /// Hacemos que las opciones de filtrado del listado ya esten bien.
    fac->m_cliente->setidcliente ( DBvalue ( "idcliente" ) );
    fac->on_mui_actualizar_clicked();

    /// Lanzamos el dialogo.
    diag->exec();
    QString idalbaran = fac->idCliDelivNote();
    delete diag;

    /// Si no hay idfactura es que hemos abortado y por tanto cancelamos la operacion.
    if ( idalbaran == "" )
        return;

    /// Creamos la factura.
    AlbaranClienteView *bud = new AlbaranClienteView ( empresaBase(), NULL );
    bud->cargar ( idalbaran );

    /// Agregamos a comentarios que albaran se corresponde.
    QString comm = DBvalue ( "comentfactura" ) + "(" + tr ( "ALBARAN: Num " ) + bud->DBvalue ( "numalbaran" ) + tr ( "Ref:" ) + " " + bud->DBvalue ( "refalbaran" ) + tr ( "Fecha:" ) + " " + bud->DBvalue ( "fechaalbaran" ) + ")\n";

    setDBvalue ( "comentfactura", comm );
    pintar();

    /// EN TEORIA SE DEBERIA COMPROBAR QUE LA FACTURA Y EL ALBARAN SON DEL MISMO CLIENTE, pero por ahora no lo hacemos.
    SDBRecord *linea, *linea1;
    for ( int i = 0; i < bud->getlistalineas() ->rowCount(); ++i ) {
        linea = bud->getlistalineas() ->lineaat ( i );
        /// Los registros vacios no se tienen en cuenta.
        if ( linea->DBvalue ( "idarticulo" ) != "" ) {
            linea1 = getlistalineas() ->lineaat ( getlistalineas() ->rowCount() - 1 );
            linea1->setDBvalue ( "desclfactura", linea->DBvalue ( "desclalbaran" ) );
            linea1->setDBvalue ( "cantlfactura", linea->DBvalue ( "cantlalbaran" ) );
            linea1->setDBvalue ( "pvplfactura", linea->DBvalue ( "pvplalbaran" ) );
            linea1->setDBvalue ( "descuentolfactura", linea->DBvalue ( "descuentolalbaran" ) );
            linea1->setDBvalue ( "idarticulo", linea->DBvalue ( "idarticulo" ) );
            linea1->setDBvalue ( "codigocompletoarticulo", linea->DBvalue ( "codigocompletoarticulo" ) );
            linea1->setDBvalue ( "nomarticulo", linea->DBvalue ( "nomarticulo" ) );
            linea1->setDBvalue ( "ivalfactura", linea->DBvalue ( "ivalalbaran" ) );
            getlistalineas() ->nuevoRegistro();
        } // end if
    } // end for

    /// Procesamos el albaran.
    bud->mui_procesadoalbaran->setChecked ( TRUE );
    bud->guardar();
    delete bud;

    /// Pintamos los totales.
    calculaypintatotales();
    _depura ( "END FacturaView::on_mui_agregaralbaran_clicked", 0 );
}


/** SLOT que responde a la pulsacion del boton mui_veralbaranes.
    Busca todos los albaranes con la misma referencia y para cada uno de ellos instancia la clase \ref AlbaranClienteView
*/
/**
**/
void FacturaView::on_mui_veralbaranes_clicked()
{
    _depura ( "FacturaView::on_mui_veralbaranes_clicked", 0 );
    QString SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + DBvalue ( "reffactura" ) + "'";
    cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );
    if ( !cur->eof() ) {
        while ( !cur->eof() ) {
            AlbaranClienteView * bud = new AlbaranClienteView ( empresaBase(), NULL );
            empresaBase() ->m_pWorkspace->addWindow ( bud );
            bud->cargar ( cur->valor ( "idalbaran" ) );
            bud->show();
            cur->siguienteregistro();
        } // end while
    } else {
        _depura ( "no hay albaranes con esta referencia", 2 );
    } // end if
    delete cur;
    _depura ( "END FacturaView::on_mui_veralbaranes_clicked", 0 );
}


///
/**
\param id
**/
void FacturaView::on_mui_idcliente_valueChanged ( QString id )
{
    _depura ( "FacturaView::on_m_cliente_valueChanged", 0 );
    subform2->setIdCliente ( id );
    mui_idforma_pago->setIdCliente ( id );
    _depura ( "END FacturaView::on_m_cliente_valueChanged", 0 );
}


///
/**
**/
void FacturaView::on_m_descuentos_editFinish ( int, int )
{
    _depura ( "FacturaView::on_m_descuentos_editFinish", 0 );
    calculaypintatotales();
    _depura ( "END FacturaView::on_m_descuentos_editFinish", 0 );
}


///
/**
**/
void FacturaView::on_subform2_editFinish ( int, int )
{
    _depura ( "FacturaView::on_subform2_editFinish", 0 );
    calculaypintatotales();
    _depura ( "END FacturaView::on_subform2_editFinish", 0 );
}


/** Se encarga del borrado de una factura.
*/
/**
\return
**/
int FacturaView::borrarPre()
{
    _depura ( "FacturaView::borrarPre", 0 );
    int error = m_listalineas->borrar();
    error = m_listadescuentos->borrar();
    _depura ( "END FacturaView::borrarPre", 0 );
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
    _depura ( "FacturaView::cargarPost", 0 );

    m_listalineas->cargar ( idbudget );
    m_listadescuentos->cargar ( idbudget );

    /// Disparamos los plugins.
    g_plugins->lanza ( "FacturaView_cargarPost_Post", this );

    calculaypintatotales();

    _depura ( "END FacturaView::cargarPost", 0 );
    return 0;
}


/** Se encarga del guardado de una factura.
*/
/**
\return
**/
int FacturaView::guardarPost()
{
    _depura ( "FacturaView::guardarPost", 0 );
    m_listadescuentos->setColumnValue ( "idfactura", DBvalue ( "idfactura" ) );
    m_listalineas->setColumnValue ( "idfactura", DBvalue ( "idfactura" ) );
    m_listalineas->guardar();
    m_listadescuentos->guardar();
    _depura ( "END FacturaView::guardarPost", 0 );
    return 0;
}



