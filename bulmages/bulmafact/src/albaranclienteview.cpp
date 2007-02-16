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

#include "albaranclienteview.h"
#include "articulolist.h"
#include "clientslist.h"
#include "cobroview.h"
#include "company.h"
#include "configuracion.h"
#include "facturaslist.h"
#include "facturaview.h"
#include "funcaux.h"
#include "informereferencia.h"
#include "listdescalbaranclienteview.h"
#include "listlinalbaranclienteview.h"
#include "pedidoclienteview.h"
#include "pedidosclientelist.h"
#include "postgresiface2.h"
#include "presupuestolist.h"
#include "presupuestoview.h"


/** Constructor de la clase corresponde a la parte visual de la ficha de cliente.
    Inicializa la ventana y todos sus componentes.
    Mete la ventana en el WorkSpace.
*/        
AlbaranClienteView::AlbaranClienteView(company *comp, QWidget *parent)
        : AlbaranCliente(comp, parent) {
    _depura("AlbaranClienteView::AlbaranClienteView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        setupUi(this);
        subform2->setcompany(comp);
        m_descuentos->setcompany(comp);
        m_almacen->setcompany(comp);
        m_forma_pago->setcompany(comp);
        m_cliente->setcompany(comp);
        m_trabajador->setcompany(comp);
        m_refalbaran->setcompany(comp);
        setListaLineas(subform2);
        setListaDescuentos(m_descuentos);
        comp->meteWindow(windowTitle(), this, FALSE);
    } catch (...) {
        mensajeInfo(tr("Error al crear el albaran a cliente"));
    } // end try
    _depura("END AlbaranClienteView::AlbaranClienteView", 0);
}


/** Destructor de la clase.
    Indica al listado que debe actualizarse.
*/
/// \TODO: Este metodo deberia mejorarse para que indicase al listado que 
/// solo debe eliminar una fila del mismo.
AlbaranClienteView::~AlbaranClienteView() {
    _depura("AlbaranClienteView::~AlbaranClienteView(", 0);
    m_companyact->refreshAlbaranesCliente();
    _depura("END AlbaranClienteView::~AlbaranClienteView(", 0);
}


/** Inicializa todos los elementos del formulario
*/
void AlbaranClienteView::inicializar() {
    _depura("AlbaranClienteView::inicializar", 0);
    subform2->inicializar();
    m_descuentos->inicializar();
    dialogChanges_cargaInicial();
    _depura("END AlbaranClienteView::inicializar", 0);
}

/** Pinta los totales en las casillas correspondientes
*/
void AlbaranClienteView::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc, Fixed irpf, Fixed reqeq) {
    _depura("AlbaranClienteView::pintatotales", 0);
    m_totalBases->setText(base.toQString());
    m_totalTaxes->setText(iva.toQString());
    m_totalalbaran->setText(total.toQString());
    m_totalDiscounts->setText(desc.toQString());
    m_totalIRPF->setText(QString(irpf.toQString()));
    m_totalReqEq->setText(QString(reqeq.toQString()));
    _depura("END AlbaranClienteView::pintatotales", 0);
}


/** Metodo que responde a la opcion de ver el presupuesto correspondiente con
    este albaran.
    
    Busca los presupuestos por referencia y abre los que tienen la misma referencia.
*/
void AlbaranClienteView::s_verpresupuesto() {
    _depura("AlbaranClienteView::s_verpresupuesto", 0);
    QString SQLQuery = "SELECT * FROM presupuesto WHERE refpresupuesto = '" +
                       DBvalue("refalbaran") + "'";
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);

    if (cur->numregistros() > 1) {
        PresupuestoList *list = new PresupuestoList(m_companyact, NULL);
        list->modoseleccion();
        list->show();

        while (!list->isHidden())  {
            theApp->processEvents();
        } // end while

        this->setEnabled(true);

        if (list->idpresupuesto() != QString("")) {
            PresupuestoView *bud = m_companyact->nuevoPresupuestoView();
            bud->cargar(list->idpresupuesto());
            bud->show();
        } // end if
    } else if (!cur->eof()) {
        PresupuestoView *bud = m_companyact->nuevoPresupuestoView();
        bud->cargar(cur->valor("idpresupuesto"));
        bud->show();
    } // end if
    delete cur;
    _depura("END AlbaranClienteView::s_verpresupuesto", 0);
}


/** SLOT de ver el pedidocliente.
    Busca los pedidos a cliente que tienen la misma referencia que el albaran
    y los abre.
*/
void AlbaranClienteView::on_mui_verpedidocliente_clicked() {
    _depura("AlbaranClienteView::on_mui_verpedidocliente_clicked", 0);
    QString SQLQuery = "SELECT * FROM pedidocliente WHERE refpedidocliente = '" + DBvalue("refalbaran") + "'";
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        while (!cur->eof()) {
            PedidoClienteView *bud = m_companyact->newPedidoClienteView();
            m_companyact->m_pWorkspace->addWindow(bud);
            bud->cargar(cur->valor("idpedidocliente"));
            bud->show();
            cur->siguienteregistro();
        } // end while
    } else {
        _depura("no hay pedidos con esta referencia", 2);
    } // end if
    delete cur;
    _depura("END AlbaranClienteView::on_mui_verpedidocliente_clicked", 0);
}


/// Se encarga de generar una factura a partir de un albar&aacute;n.
/** Primero de todo busca una factura por referencia que tenga este albaran.
    Si dicha factura existe entonces la cargamos y terminamos.
    Si no existe dicha factura el sistema avisa y permite crear una poniendo
    Todos los datos del albaran automaticamente en ella.
*/
void AlbaranClienteView::generarFactura() {
    _depura("AlbaranClienteView::generarFactura", 0);
    /// Comprobamos que existe una factura para este cliente, y en caso afirmativo lo mostramos
    /// y salimos de la funci&oacute;n.
    QString SQLQuery = "SELECT * FROM factura WHERE reffactura = '" + DBvalue("refalbaran") + "' AND idcliente = " + DBvalue("idcliente");
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);

    if (!cur->eof()) {

	/// Informamos que ya hay una factura y que la abriremos.
	/// Si no salimos de la funci&oacute;n.
	if (QMessageBox::question(this,
				tr("Factura existente"),
				tr("Existe una factura a este cliente con la misma referencia que este albaran. Desea abrirla para verificar?"),
				tr("&Si"), tr("&No"), QString::null, 0, 1)) {
		return;
	}
        FacturaView *bud = m_companyact->newFacturaView();
        m_companyact->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idfactura"));
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
    FacturaView *bud = m_companyact->newFacturaView();
    m_companyact->m_pWorkspace->addWindow(bud);

    /// Cargamos un elemento que no existe para inicializar bien la clase.
    bud->cargar("0");
    bud->setcomentfactura(DBvalue("comentalbaran"));
    bud->setidforma_pago(DBvalue("idforma_pago"));
    bud->setreffactura(DBvalue("refalbaran"));
    bud->setidcliente(DBvalue("idcliente"));
    bud->setidalmacen(DBvalue("idalmacen"));
    bud->pintaFactura();
    bud->show();

    QString l;
    SDBRecord *linea, *linea1;
    for (int i = 0; i < m_listalineas->rowCount(); ++i) {
        linea = m_listalineas->lineaat(i);
        if (linea->DBvalue( "idarticulo") != "") {
            linea1 = bud->getlistalineas()->lineaat(bud->getlistalineas()->rowCount() - 1);
            linea1->setDBvalue("desclfactura", linea->DBvalue("desclalbaran"));
            linea1->setDBvalue("cantlfactura", linea->DBvalue("cantlalbaran"));
            linea1->setDBvalue("pvplfactura", linea->DBvalue("pvplalbaran"));
            linea1->setDBvalue("descuentolfactura", linea->DBvalue("descuentolalbaran"));
            linea1->setDBvalue("idarticulo", linea->DBvalue("idarticulo"));
            linea1->setDBvalue("codigocompletoarticulo", linea->DBvalue("codigocompletoarticulo"));
            linea1->setDBvalue("nomarticulo", linea->DBvalue("nomarticulo"));
            linea1->setDBvalue("ivalfactura", linea->DBvalue("ivalalbaran"));
            bud->getlistalineas()->nuevoRegistro();
        } // end if
    } // end for
    bud->calculaypintatotales();
    m_procesadoalbaran->setChecked(TRUE);
    _depura("END AlbaranClienteView::generarFactura", 0);
}


/// Se encarga de agregar un albaran a una factura ya existente.
/// Para ello presenta un selector de factura y permite escoger a que factura
/// Agregar el albaran.
void AlbaranClienteView::agregarFactura() {
    /// Pedimos la factura a la que agregar.
    _depura("AlbaranClienteView::agregarFactura", 0);
    QDialog *diag = new QDialog(0);
    diag->setModal(true);
    FacturasList *fac = new FacturasList(m_companyact, diag, 0, FacturasList::SelectMode);
    connect(fac, SIGNAL(selected(QString)), diag, SLOT(accept()));

    /// Hacemos que las opciones de filtrado del listado ya est&eacute;n bien.
    fac->m_cliente->setidcliente(DBvalue("idcliente"));
    fac->on_mui_actualizar_clicked();

    /// Lanzamos el di&aacute;logo.
    diag->exec();
    QString idfactura = fac->idfactura();
    delete diag;

    /// Si no hay idfactura es que hemos abortado y por tanto cancelamos la operaci&oacute;n.
    if (idfactura == "")
        return;

    /// Creamos la factura.
    FacturaView *bud = m_companyact->newFacturaView();
    bud->cargar(idfactura);

    /// Agregamos en los comentarios que se ha a&ntilde;adido este albar&aacute;n.
    bud->pintaComentFactura(bud->DBvalue("comentfactura") + tr("Num. albaran") + DBvalue("numalbaran") + "\n" );

    m_companyact->m_pWorkspace->addWindow(bud);
    /// EN TEORIA SE DEBARIA COMPROBAR QUE LA FACTURA ES DEL MISMO CLIENTE,
    /// pero por ahora pasamos de hacerlo.
    QString l;
    SDBRecord *linea, *linea1;
    for (int i = 0; i < m_listalineas->rowCount(); ++i) {
        linea = m_listalineas->lineaat(i);
        if (linea->DBvalue("idarticulo") != "") {
            linea1 = bud->getlistalineas()->lineaat(bud->getlistalineas()->rowCount() - 1);
            linea1->setDBvalue("desclfactura", linea->DBvalue("desclalbaran"));
            linea1->setDBvalue("cantlfactura", linea->DBvalue("cantlalbaran"));
            linea1->setDBvalue("pvplfactura", linea->DBvalue("pvplalbaran"));
            linea1->setDBvalue("descuentolfactura", linea->DBvalue("descuentolalbaran"));
            linea1->setDBvalue("idarticulo", linea->DBvalue("idarticulo"));
            linea1->setDBvalue("codigocompletoarticulo", linea->DBvalue("codigocompletoarticulo"));
            linea1->setDBvalue("nomarticulo", linea->DBvalue("nomarticulo"));
            linea1->setDBvalue("ivalfactura", linea->DBvalue("ivalalbaran"));
            bud->getlistalineas()->nuevoRegistro();
        } // end if
    } // end for
    bud->calculaypintatotales();
    bud->show();
    m_procesadoalbaran->setChecked(TRUE);
    _depura("END AlbaranClienteView::agregarFactura", 0);
}


/// Se encarga de hacer la carga de un albaran. Para ello delega la responsabilidad
/// De base de datos a la clase \class AlbaranCliente y se encarga unicamente
/// De volver a meter la ventana en el workSpace para que esta coja adecuadamente
/// El titulo. Tambien inicializa el sistema de control de cambios.
/// Si algo falla se genera una excepcion.
int AlbaranClienteView::cargar(QString id) {
    _depura("AlbaranClienteView::cargar", 0);
    try {
        if (AlbaranCliente::cargar(id))
            throw -1;
        setWindowTitle(tr("Albaran a cliente") + " " + DBvalue("refalbaran") + " " + DBvalue("idalbaran"));
        m_companyact->meteWindow(windowTitle(), this);
        dialogChanges_cargaInicial();
    } catch(...) {
        return -1;
    } // end try
    _depura("AlbaranClienteView::cargar", 0);
    return 0;
}


/** Cuando guardamos un albaran se cogen los valores de todos los campos y
    se meten en el sistema de la base de datos.
    Luego se deja el guardado a la clase AlbaranCliente que tambien guarda
    las lineas y los descuentos.
    
    Si algo falla se genera una excepcion.
*/
int AlbaranClienteView::guardar() {
    _depura("AlbaranClienteView::guardar", 0);
    try {
    /// Cogemos todos los valores del formulario y actualizamos la clase.
    setcomentalbaran(m_comentalbaran->toPlainText());
    setcomentprivalbaran(m_comentprivalbaran->toPlainText());
    setidalmacen(m_almacen->idalmacen());
    setNumAlbaran(m_numalbaran->text());
    setidcliente(m_cliente->idcliente());
    setprocesadoalbaran(m_procesadoalbaran->isChecked() ? "TRUE" : "FALSE");
    setcontactalbaran(m_contactalbaran->text());
    settelalbaran(m_telalbaran->text());
    setfechaalbaran(m_fechaalbaran->text());
    setidforma_pago(m_forma_pago->idforma_pago());
    setidtrabajador(m_trabajador->idtrabajador());
    setrefalbaran(m_refalbaran->text());
    setdescalbaran(m_descalbaran->text());
    /// Hacemos el guardado.
    AlbaranCliente::guardar();
    dialogChanges_cargaInicial();
   } catch(...) {
	_depura("AlbaranClienteView::guardar Error al guardar ", 0);
	throw -1;
	} // end try
    _depura("END AlbaranClienteView::guardar", 0);
    return 0;
}


/// Crea un nuevo cobro para el albar&aacute;n seleccionado.
/// Util con cobros anticipados a la factura. El cobro tendra
/// la misma cantidad, referencia y cliente que el albaran.
/// Este metodo crea una pantalla de cobro y le pone los datos necesarios.
void AlbaranClienteView::on_mui_cobrar_clicked() {
    _depura("AlbaranClienteView::on_mui_cobrar_clicked", 0);
    CobroView *bud = m_companyact->newCobroView();
    bud->setidcliente(DBvalue("idcliente"));
    bud->setcantcobro(m_totalalbaran->text());
    bud->setrefcobro(DBvalue("refalbaran"));
    bud->setcomentcobro(DBvalue("descalbaran"));
    bud->pintar();
    bud->show();
    _depura("END AlbaranClienteView::on_mui_cobrar_clicked", 0);
}

void AlbaranClienteView::on_m_cliente_valueChanged(QString id) {
	_depura("AlbaranClienteView::on_m_cliente_valueChanged", 0);
	subform2->setIdCliente(id);
	m_forma_pago->setIdCliente(id);
	_depura("END AlbaranClienteView::on_m_cliente_valueChanged", 0);
}

