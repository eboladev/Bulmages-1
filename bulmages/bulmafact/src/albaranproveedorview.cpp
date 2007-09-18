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

#include <QCloseEvent>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QToolButton>
#include <QWidget>

#include "albaranproveedorview.h"
#include "articulolist.h"
#include "company.h"
#include "configuracion.h"
#include "facturapview.h"
#include "facturaview.h"
#include "funcaux.h"
#include "listlinalbaranproveedorview.h"
#include "pagoview.h"
#include "pedidoproveedorview.h"
#include "postgresiface2.h"
#include "presupuestolist.h"
#include "presupuestoview.h"
#include "plugins.h"


/** Inicializa todos los componentes de la ventana.
    Mete la ventana en el workSpace.
*/
AlbaranProveedorView::AlbaranProveedorView(Company *comp, QWidget *parent)
        : FichaBf(comp, parent) {
    _depura("AlbaranProveedorView::AlbaranProveedorView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        setupUi(this);
    setTitleName(tr("Albaran Proveedor"));
    setDBTableName("albaranp");
    setDBCampoId("idalbaranp");
    addDBCampo("idalbaranp", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("AlbaranProveedor", "Id albaran proveedor"));
    addDBCampo("numalbaranp", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("AlbaranProveedor", "Numero albaran proveedor"));
    addDBCampo("fechaalbaranp", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("AlbaranProveedor", "Fecha albaran proveedor"));
    addDBCampo("comentalbaranp", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranProveedor", "Comentario albaran proveedor"));
    addDBCampo("idproveedor", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("AlbaranProveedor", "Id proveedor"));
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("AlbaranProveedor", "Id forma de pago"));
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("AlbaranProveedor", "Id almacen"));
    addDBCampo("refalbaranp", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranProveedor", "Referencia albaran proveedor"));
    addDBCampo("descalbaranp", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranProveedor", "Descripcion albaran proveedor"));


        /// Disparamos los plugins.
        int res = g_plugins->lanza("AlbaranProveedorView_AlbaranProveedorView", this);
        if (res != 0)
            return;
        subform2->setEmpresaBase(comp);
        mui_idalmacen->setEmpresaBase(comp);
        mui_idforma_pago->setEmpresaBase(comp);
        mui_idproveedor->setEmpresaBase(comp);
        m_descuentos->setEmpresaBase(comp);
        mui_refalbaranp->setEmpresaBase(comp);

        setListaLineas(subform2);
        setListaDescuentos(m_descuentos);

        m_totalBases->setReadOnly(TRUE);
        m_totalBases->setAlignment(Qt::AlignRight);
        m_totalTaxes->setReadOnly(TRUE);
        m_totalTaxes->setAlignment(Qt::AlignRight);
        m_totalDiscounts->setReadOnly(TRUE);
        m_totalDiscounts->setAlignment(Qt::AlignRight);
        m_totalalbaranp->setReadOnly(TRUE);
        m_totalalbaranp->setAlignment(Qt::AlignRight);
	mui_idforma_pago->setidforma_pago("0");
	mui_idalmacen->setidalmacen("0");
        meteWindow(windowTitle(), this, FALSE);
    } catch (...) {
        mensajeInfo(tr("Error al crear el albaran proveedor"));
    } // end try
    _depura("END AlbaranProveedorView::AlbaranProveedorView", 0);
}


/** Hace que el listado de Albaranes se refresque
*/
AlbaranProveedorView::~AlbaranProveedorView() {
    _depura("AlbaranProveedorView::~AlbaranProveedorView", 0);
    empresaBase()->refreshAlbaranesProveedor();
    _depura("END AlbaranProveedorView::~AlbaranProveedorView", 0);
}




void AlbaranProveedorView::on_mui_facturar_clicked() {
    _depura("AlbaranProveedorView::on_mui_facturar_clicked", 0);
    generarFacturaProveedor();
    _depura("END AlbaranProveedorView::on_mui_facturar_clicked", 0);
}



void AlbaranProveedorView::s_generarFactura() {
    _depura("AlbaranProveedorView::s_generarFactura", 0);
    generarFactura();
    _depura("END AlbaranProveedorView::s_generarFactura", 0);
}


void AlbaranProveedorView::on_m_descuentos_editFinish(int, int) {
    _depura("AlbaranProveedorView::on_m_descuentos_editFinish", 0);
    calculaypintatotales();
    _depura("END AlbaranProveedorView::on_m_descuentos_editFinish", 0);
}


void AlbaranProveedorView::on_subform2_editFinish(int, int) {
    _depura("AlbaranProveedorView::on_subform2_editFinish", 0);
    calculaypintatotales();
    _depura("END AlbaranProveedorView::on_subform2_editFinish", 0);
}


/** Inicializa los elementos de la ventana.
    Este metodo es invocado al crear un nuevo Albaran de Proveedor sin
    tener que hacer una carga del mismo.
*/
void AlbaranProveedorView::inicializar() {
    _depura("AlbaranProveedorView::inicializar", 0);
    subform2->inicializar();
    m_descuentos->inicializar();
    dialogChanges_cargaInicial();
    _depura("END AlbaranProveedorView::inicializar", 0);
}


/** Pinta los totales indicados en sus correspondientes textEdit.
*/
void AlbaranProveedorView::pintatotales(Fixed iva, Fixed base, Fixed, Fixed desc, Fixed irpf, Fixed reqeq) {
    _depura("AlbaranProveedorView::pintatotales", 0);
    m_totalalbaranp->setText((iva + base).toQString());
    m_totalBases->setText(base.toQString());
    m_totalTaxes->setText(iva.toQString());
    m_totalDiscounts->setText(desc.toQString());
    m_totalIRPF->setText(QString(irpf.toQString()));
    m_totalReqEq->setText(QString(reqeq.toQString()));
    _depura("END AlbaranProveedorView::pintatotales", 0);
}


/** FUNCION AUN NO IMPLEMENTADA. */
void AlbaranProveedorView::s_verpedidoproveedor()  {
    _depura("Funcion aun no implementada", 2);
}


/** Se encarga de generar una factura de proveedor a partir del albaranp.
    Mira si existe una factura de proveedor con la misma referencia y si es asi
    la abre y la muestra.
    En caso contrario crea una instancia de facturap y rellena todos los campos de esta con
    los datos del albaran y la muestra.
*/
void AlbaranProveedorView::generarFactura()  {
    _depura("AlbaranProveedorView::generarFactura", 0);
    /// Comprobamos que existe el elemento, y en caso afirmativo lo mostramos
    /// y salimos de la funcion.
    QString SQLQuery = "SELECT * FROM facturap WHERE reffacturap = '" + DBvalue("refalbaranp") + "'";
    cursor2 *cur = empresaBase()->cargacursor(SQLQuery);

    if (!cur->eof()) {
        FacturaProveedorView *bud = empresaBase()->newFacturaProveedorView();
        empresaBase()->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idfacturap"));
        bud->show();
        return;
    } // end if
    delete cur;

    /// Informamos de que no existe el pedido y a ver si lo queremos realizar.
    /// Si no salimos de la funcion.
    if (QMessageBox::question(this,
                              tr("Factura inexistente"),
                              tr("No existe una factura asociada a este albaran.\nDesea crearla?"),
                              tr("&Si"), tr("&No"), QString::null, 0, 1)) {
        return;
    } // end if

    /// Creamos la factura.
    FacturaProveedorView *bud = empresaBase()->newFacturaProveedorView();
    empresaBase()->m_pWorkspace->addWindow(bud);

    bud->setDBvalue("comentfacturap", DBvalue("comentalbaranp"));
    bud->setDBvalue("idforma_pago", DBvalue("idforma_pago"));
    bud->setDBvalue("reffacturap", DBvalue("refalbaranp"));
    bud->setDBvalue("idproveedor", DBvalue("idproveedor"));
    SDBRecord *linea, *linea1;

    for (int i  = 0; i < m_listalineas->rowCount(); ++i) {
        linea = getlistalineas()->lineaat(i);
        linea1 = bud->getlistalineas()->newSDBRecord();
        linea1->setDBvalue("desclfacturap", linea->DBvalue("desclalbaranp"));
        linea1->setDBvalue("cantlfacturap", linea->DBvalue("cantlalbaranp"));
        linea1->setDBvalue("pvplfacturap", linea->DBvalue("pvplalbaranp"));
        linea1->setDBvalue("descuentolfacturap", linea->DBvalue("descuentolalbaranp"));
        linea1->setDBvalue("idarticulo", linea->DBvalue("idarticulo"));
        linea1->setDBvalue("codigocompletoarticulo", linea->DBvalue("codigocompletoarticulo"));
        linea1->setDBvalue("nomarticulo", linea->DBvalue("nomarticulo"));
        linea1->setDBvalue("ivalfacturap", linea->DBvalue("ivalalbaranp"));
    } // end for
    bud->pintar();
    bud->show();
    _depura("END AlbaranProveedorView::generarFactura", 0);
}






/** SLOT que responde a la creacion de un pago.
    Crea una instancia de la ventana de pagos y la rellena con los datos
    del AlbaranProveedor y lo muestra.
*/
///  \TODO: Actualmente no esta comprobando que el pago ya exista.
void AlbaranProveedorView::on_mui_pagar_clicked() {
    _depura("AlbaranProveedorView::on_mui_pagar_clicked", 0);
    PagoView *bud = empresaBase()->newPagoView();
    empresaBase()->m_pWorkspace->addWindow(bud);
    bud->setDBvalue("idproveedor", DBvalue("idproveedor"));
    bud->setDBvalue("cantpago", m_totalalbaranp->text());
    bud->setDBvalue("refpago", DBvalue("refalbaranp"));
    bud->setDBvalue("comentpago", DBvalue("descalbaranp"));
    bud->pintar();
    bud->show();
    _depura("END AlbaranProveedorView::on_mui_pagar_clicked", 0);
}

/** SLOT que responde a la solicitud de ver todos los pedidos de proveedor
    relacionados con el albaran abierto.
    Realiza una consulta sobre los pedidos de proveedor que tienen la misma
    referencia que este pedido de proveedor y los instancia y muestra.
*/
void AlbaranProveedorView::on_mui_verpedidosproveedor_clicked() {
    _depura("AlbaranProveedorView::on_mui_verpedidos_clicked", 0);
    QString query = "SELECT * FROM pedidoproveedor WHERE refpedidoproveedor = '" + DBvalue("refalbaranp") + "'";
    cursor2 *cur = empresaBase()->cargacursor(query);
    while (!cur->eof()) {
        PedidoProveedorView *pedpro = empresaBase()->nuevoPedidoProveedorView();
        pedpro->cargar(cur->valor("idpedidoproveedor"));
        empresaBase()->m_pWorkspace->addWindow( pedpro);
        pedpro->show();
        cur->siguienteregistro();
    } // end while
    delete cur;
    _depura("END AlbaranProveedorView::on_mui_verpedidos_clicked", 0);
}


/// Se encarga de generar una factura a partir de un albar&aacute;n.
/** Primero de todo busca una factura por referencia que tenga este albaran.
    Si dicha factura existe entonces la cargamos y terminamos.
    Si no existe dicha factura el sistema avisa y permite crear una poniendo
    Todos los datos del albaran automaticamente en ella.
*/
void AlbaranProveedorView::generarFacturaProveedor() {
    _depura("AlbaranProveedorView::generarFacturaProveedor", 0);
    /// Comprobamos que existe una factura para este cliente, y en caso afirmativo lo mostramos
    /// y salimos de la funci&oacute;n.
    QString SQLQuery = "SELECT * FROM facturap WHERE reffacturap = '" + DBvalue("refalbaranp") + "' AND idproveedor ="+DBvalue("idproveedor");
    cursor2 *cur = empresaBase()->cargacursor(SQLQuery);

    if (!cur->eof()) {

    /// Informamos que ya hay una factura y que la abriremos.
    /// Si no salimos de la funci&oacute;n.
    if (QMessageBox::question(this,
                tr("Factura de proveedor existente"),
                tr("Existe una factura de este proveedor con la misma referencia que este albaran. Desea abrirla para verificar?"),
                tr("&Si"), tr("&No"), QString::null, 0, 1)) {
        return;
    }
        FacturaProveedorView *bud = empresaBase()->newFacturaProveedorView();
        empresaBase()->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idfacturap"));
        bud->show();
        return;
    } // end if
    delete cur;

    /// Informamos de que no existe la factura y a ver si lo queremos realizar.
    /// Si no salimos de la funci&oacute;n.
//    if (QMessageBox::question(this,
//                              tr("Factura inexistente"),
//                              tr("No existe una factura asociada a este albaran. Desea crearla?"),
//                              tr("&Si"), tr("&No"), QString::null, 0, 1)) {
//        return;
//    }

    /// Creamos la factura.
    FacturaProveedorView *bud = empresaBase()->newFacturaProveedorView();
    empresaBase()->m_pWorkspace->addWindow(bud);

    /// Cargamos un elemento que no existe para inicializar bien la clase.
    bud->inicializar();

    bud->setDBvalue("comentfacturap", DBvalue("comentalbaranp"));
    bud->setDBvalue("idforma_pago", DBvalue("idforma_pago"));
    bud->setDBvalue("reffacturap", DBvalue("refalbaranp"));
    bud->setDBvalue("idproveedor", DBvalue("idproveedor"));
    bud->pintar();
    bud->show();

    QString l;
    SDBRecord *linea, *linea1;
    for (int i = 0; i < m_listalineas->rowCount(); ++i) {
        linea = m_listalineas->lineaat(i);
        if (linea->DBvalue( "idarticulo") != "") {
            linea1 = bud->getlistalineas()->lineaat(bud->getlistalineas()->rowCount() - 1);
            bud->getlistalineas()->nuevoRegistro();
            linea1->setDBvalue("desclfacturap", linea->DBvalue("desclalbaranp"));
            linea1->setDBvalue("cantlfacturap", linea->DBvalue("cantlalbaranp"));
            linea1->setDBvalue("pvplfacturap", linea->DBvalue("pvplalbaranp"));
            linea1->setDBvalue("descuentolfacturap", linea->DBvalue("descuentolalbaranp"));
            linea1->setDBvalue("idarticulo", linea->DBvalue("idarticulo"));
            linea1->setDBvalue("codigocompletoarticulo", linea->DBvalue("codigocompletoarticulo"));
            linea1->setDBvalue("nomarticulo", linea->DBvalue("nomarticulo"));
            linea1->setDBvalue("ivalfacturap", linea->DBvalue("ivalalbaranp"));
        } // end if
    } // end for
    bud->calculaypintatotales();
    _depura("END AlbaranClienteView::generarFactura", 0);
}


void AlbaranProveedorView::on_mui_idproveedor_valueChanged(QString id) {
    _depura("AlbaranProveedorView::on_m_proveedor_valueChanged", 0);
    subform2->setIdProveedor(id);
    mui_idforma_pago->setIdProveedor(id);
    _depura("END AlbaranProveedorView::on_m_proveedor_valueChanged", 0);
}

/** Se encarga del borrado en la base de datos de una albaran de proveedor.
    Si se produce algun error devuelve una excepcion.
    Primero llama al borrado de las lineas y luego a los descuentos.
    Por ultimo llama al borrado del registro correspondiente en albaranp.
*/
int AlbaranProveedorView::borrarPre() {
    _depura("AlbaranProveedorView::borrar", 0);
            m_listalineas->borrar();
            m_listadescuentos->borrar();
        _depura("END AlbaranProveedorView::borrar", 0);
        return 0;
}


/// Esta funcion carga un AlbaranProveedor.
/// Hace el query adecuado, carga el registro a traves de DBRecord.
/// Hace la carga de las lineas y de los descuentos.
/// Invoca al pintado.
int AlbaranProveedorView::cargarPost(QString idbudget) {
    _depura("AlbaranProveedorView::cargar", 0);

    m_listalineas->cargar(idbudget);
    m_listadescuentos->cargar(idbudget);
    _depura("END AlbaranProveedorView::cargar", 0);
    return 0;
}


/** Guarda el albaran de proveedor en la base de datos.
    Para ello hace el guardado de DBRecord y luego guarda las lineas y los descuentos.
    Una vez hecho el guardado se hace una carga para recuperar posibles datos que haya
    introducido la base de datos como la referencia.

    Si algo falla devuelve una excepcion -1.
*/
int AlbaranProveedorView::guardarPost() {
    _depura("AlbaranProveedorView::guardar", 0);

        m_listalineas->guardar();
        m_listadescuentos->guardar();

        _depura("END AlbaranProveedorView::guardar", 0);
        return 0;

}



