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

/** Inicializa todos los componentes de la ventana.
    Mete la ventana en el workSpace.
*/    
AlbaranProveedorView::AlbaranProveedorView(company *comp, QWidget *parent)
        : AlbaranProveedor(comp, parent) {
    _depura("AlbaranProveedorView::AlbaranProveedorView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        setupUi(this);
        subform2->setcompany(comp);
        m_almacen->setcompany(comp);
        m_forma_pago->setcompany(comp);
        m_proveedor->setcompany(comp);
        m_descuentos->setcompany(comp);
        m_refalbaranp->setcompany(comp);

	setListaLineas( subform2);
	setListaDescuentos( m_descuentos);


        m_totalBases->setReadOnly(TRUE);
        m_totalBases->setAlignment(Qt::AlignRight);
        m_totalTaxes->setReadOnly(TRUE);
        m_totalTaxes->setAlignment(Qt::AlignRight);
        m_totalDiscounts->setReadOnly(TRUE);
        m_totalDiscounts->setAlignment(Qt::AlignRight);
        m_totalalbaranp->setReadOnly(TRUE);
        m_totalalbaranp->setAlignment(Qt::AlignRight);
        pintaidforma_pago("0");
        pintaidalmacen("0");
        if (m_companyact != NULL)
            m_companyact->meteWindow(windowTitle(), this, FALSE);
    } catch (...) {
        mensajeInfo(tr("Error al crear el albaran proveedor"));
    } // end try
    _depura("END AlbaranProveedorView::AlbaranProveedorView", 0);
}


/** Hace que el listado de Albaranes se refresque
*/
AlbaranProveedorView::~AlbaranProveedorView() {
    _depura("AlbaranProveedorView::~AlbaranProveedorView", 0);
    m_companyact->refreshAlbaranesProveedor();
    _depura("END AlbaranProveedorView::~AlbaranProveedorView", 0);
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
void AlbaranProveedorView::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc, Fixed irpf, Fixed reqeq) {
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
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);

    if (!cur->eof()) {
        FacturaProveedorView *bud = m_companyact->newFacturaProveedorView();
        m_companyact->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idfacturap"));
        bud->show();
        return;
    } // end if
    delete cur;

    /// Informamos de que no existe el pedido y a ver si lo queremos realizar.
    /// Si no salimos de la funcion.
    if (QMessageBox::question(this,
                              tr("Factura inexistente"),
                              tr("No existe una factura asociada a este albaran."
                                 "Desea crearla?"),
                              tr("&Si"), tr("&No"), QString::null, 0, 1)) {
        return;
    } // end if

    /// Creamos la factura.
    FacturaProveedorView *bud = m_companyact->newFacturaProveedorView();
    m_companyact->m_pWorkspace->addWindow(bud);

    bud->setcomentfacturap(DBvalue("comentalbaranp"));
    bud->setidforma_pago(DBvalue("idforma_pago"));
    bud->setreffacturap(DBvalue("refalbaranp"));
    bud->setidproveedor(DBvalue("idproveedor"));
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

/** Se encarga de hacer la carga de un Albaran de Proveedor.
    Delega toda la funcionalidad a AlbaranProveedor excepto el cambiar el
    titulo dela ventana y hacer que este se refresque en el listado de ventanas.
    Inicializa la ventana para que considere que no hay cambios realizados.    
    Si algo falla devuelve -1.
*/
int AlbaranProveedorView::cargar(QString id) {
    _depura("AlbaranProveedorView::cargar", 0);
    try {
        AlbaranProveedor::cargar(id);
        if (DBvalue("idalbaranp") != "") {
            setWindowTitle(tr("Albaran de proveedor") + " " + DBvalue("refalbaranp") + " " + DBvalue("idalbaranp"));
            m_companyact->meteWindow(windowTitle(), this);
            dialogChanges_cargaInicial();
        } // end if
    } catch (...) {
        return -1;
    } // end try
    _depura("END AlbaranProveedorView::cargar", 0);
    return 0;
}


/** Refresca los datos de DBRecord con los valores puestos en los componentes
    Luego invoca los metodos de guardado en AlbaranProveedor.

    Induce una carga con el identificador de ventana para que se refresquen posibles
    datos que haya introducido la base de datos.

    Si algo falla devuelve una excepcion -1.
*/
int AlbaranProveedorView::guardar() {
    _depura("AlbaranProveedorView::guardar", 0);
    try {
        setcomentalbaranp(m_comentalbaranp->toPlainText());
        setnumalbaranp(m_numalbaranp->text());
        setidproveedor(m_proveedor->idproveedor());
        setfechaalbaranp(m_fechaalbaranp->text());
        setidalmacen(m_almacen->idalmacen());
        setidforma_pago(m_forma_pago->idforma_pago());
        setrefalbaranp(m_refalbaranp->text());
        setdescalbaranp(m_descalbaranp->text());
        AlbaranProveedor::guardar();
        cargar(DBvalue("idalbaranp"));
        _depura("END AlbaranProveedorView::guardar", 0);
        return 0;
    } catch (...) {
        _depura("AlbaranProveedorView::guardar Error inesperado al guardar", 2);
        throw -1;
    } // end try
}


/** SLOT que responde a la pulsacion del boton guardar.
    Llama al metodo de guardar.
*/
void AlbaranProveedorView::on_mui_guardar_clicked() {
    _depura("AlbaranProveedorView::on_mui_guardar_clicked", 0);
    guardar();
    _depura("END AlbaranProveedorView::on_mui_guardar_clicked", 0);
}


/** SLOT que responde a la creacion de un pago.
    Crea una instancia de la ventana de pagos y la rellena con los datos
    del AlbaranProveedor y lo muestra.
*/
///  \TODO: Actualmente no esta comprobando que el pago ya exista.
void AlbaranProveedorView::on_mui_pagar_clicked() {
    _depura("AlbaranProveedorView::on_mui_pagar_clicked", 0);
    PagoView *bud = m_companyact->newPagoView();
    m_companyact->m_pWorkspace->addWindow(bud);
    bud->setidproveedor(DBvalue("idproveedor"));
    bud->setcantpago(m_totalalbaranp->text());
    bud->setrefpago(DBvalue("refalbaranp"));
    bud->setcomentpago(DBvalue("descalbaranp"));
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
    cursor2 *cur = m_companyact->cargacursor(query);
    while (!cur->eof()) {
        PedidoProveedorView *pedpro = m_companyact->newPedidoProveedorView();
        pedpro->cargar(cur->valor("idpedidoproveedor"));
        m_companyact->m_pWorkspace->addWindow( pedpro);
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
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);

    if (!cur->eof()) {

	/// Informamos que ya hay una factura y que la abriremos.
	/// Si no salimos de la funci&oacute;n.
	if (QMessageBox::question(this,
				tr("Factura de Proveedor existente"),
				tr("Existe una factura a este proveedor con la misma referencia que este albaran. Desea abrirla para verificar?"),
				tr("&Si"), tr("&No"), QString::null, 0, 1)) {
		return;
	}
        FacturaProveedorView *bud = m_companyact->newFacturaProveedorView();
        m_companyact->m_pWorkspace->addWindow(bud);
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
    FacturaProveedorView *bud = m_companyact->newFacturaProveedorView();
    m_companyact->m_pWorkspace->addWindow(bud);

    /// Cargamos un elemento que no existe para inicializar bien la clase.
    bud->inicializar();
    bud->setcomentfacturap(DBvalue("comentalbaranp"));
    bud->setidforma_pago(DBvalue("idforma_pago"));
    bud->setreffacturap(DBvalue("refalbaranp"));
    bud->setidproveedor(DBvalue("idproveedor"));
    bud->pintar();
    bud->show();

    QString l;
    SDBRecord *linea, *linea1;
    for (int i = 0; i < m_listalineas->rowCount(); ++i) {
        linea = m_listalineas->lineaat(i);
        if (linea->DBvalue( "idarticulo") != "") {
            linea1 = bud->getlistalineas()->lineaat(bud->getlistalineas()->rowCount() - 1);
            linea1->setDBvalue("desclfacturap", linea->DBvalue("desclalbaranp"));
            linea1->setDBvalue("cantlfacturap", linea->DBvalue("cantlalbaranp"));
            linea1->setDBvalue("pvplfacturap", linea->DBvalue("pvplalbaranp"));
            linea1->setDBvalue("descuentolfacturap", linea->DBvalue("descuentolalbaranp"));
            linea1->setDBvalue("idarticulo", linea->DBvalue("idarticulo"));
            linea1->setDBvalue("codigocompletoarticulo", linea->DBvalue("codigocompletoarticulo"));
            linea1->setDBvalue("nomarticulo", linea->DBvalue("nomarticulo"));
            linea1->setDBvalue("ivalfacturap", linea->DBvalue("ivalalbaranp"));
            bud->getlistalineas()->nuevoRegistro();
        } // end if
    } // end for
    bud->calculaypintatotales();
//    m_procesadoalbaranp->setChecked(TRUE);
    _depura("END AlbaranClienteView::generarFactura", 0);
}


void AlbaranProveedorView::on_m_proveedor_valueChanged(QString id) {
	_depura("AlbaranProveedorView::on_m_proveedor_valueChanged", 0);
	subform2->setIdProveedor(id);
	m_forma_pago->setIdProveedor(id);
	_depura("END AlbaranProveedorView::on_m_proveedor_valueChanged", 0);
}

