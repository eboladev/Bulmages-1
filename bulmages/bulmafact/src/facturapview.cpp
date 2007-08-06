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

#include "facturapview.h"
#include "company.h"
#include "listlinfacturapview.h"
#include "facturap.h"
#include "providerslist.h"
#include "pagoview.h"
#include "funcaux.h"
#include "albaranproveedorview.h"


/** Inicializa todos los componentes de la clase.
    mete la ventana en el workSpace. */
FacturaProveedorView::FacturaProveedorView(company *comp, QWidget *parent)
        : FacturaProveedor(comp, parent) {
    _depura("FacturaProveedorView::FacturaProveedorView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        setupUi(this);
        subform2->setEmpresaBase(comp);
        m_descuentos->setEmpresaBase(comp);
        m_forma_pago->setEmpresaBase(comp);
        m_forma_pago->setidforma_pago("");
        m_proveedor->setEmpresaBase(comp);
        m_proveedor->setidproveedor("");
        m_reffacturap->setEmpresaBase(comp);

        setListaLineas(subform2);
        setListaDescuentos(m_descuentos);

        m_totalBases->setReadOnly(TRUE);
        m_totalBases->setAlignment(Qt::AlignRight);
        m_totalTaxes->setReadOnly(TRUE);
        m_totalTaxes->setAlignment(Qt::AlignRight);
        m_totalDiscounts->setReadOnly(TRUE);
        m_totalDiscounts->setAlignment(Qt::AlignRight);
        m_totalfacturap->setReadOnly(TRUE);
        m_totalfacturap->setAlignment(Qt::AlignRight);
        meteWindow(windowTitle(), this, FALSE);
    } catch (...) {
        mensajeInfo(tr("Error al crear la factura proveedor"));
    } // end try
    _depura("END FacturaProveedorView::FacturaProveedorView", 0);
}


/** Al destruir una factura de proveedor se hace un refresco del listado
    de facturas de proveedor. */
FacturaProveedorView::~FacturaProveedorView() {
    _depura("FacturaProveedorView::~FacturaProveedorView", 0);
    empresaBase()->refreshFacturasProveedor();
    _depura("END FacturaProveedorView::~FacturaProveedorView", 0);

}


void FacturaProveedorView::pintaidproveedor(QString id) {
    _depura("FacturaProveedorView::pintaidproveedor", 0);
    m_proveedor->setidproveedor(id);
    _depura("END FacturaProveedorView::pintaidproveedor", 0);
}


void FacturaProveedorView::pintanumfacturap(QString id) {
    _depura("FacturaProveedorView::pintanumfacturap", 0);
    m_numfacturap->setText(id);
    _depura("END FacturaProveedorView::pintanumfacturap", 0);
}


void FacturaProveedorView::pintafechafacturap(QString id) {
    _depura("FacturaProveedorView::pintafechafacturap", 0);
    m_fechafacturap->setText(id);
    _depura("END FacturaProveedorView::pintafechafacturap", 0);
}


void FacturaProveedorView::pintadescfacturap(QString id) {
    _depura("FacturaProveedorView::pintadescfacturap", 0);
    m_descfacturap->setText(id);
    _depura("END FacturaProveedorView::pintadescfacturap", 0);
}


void FacturaProveedorView::pintaComentFacturaProveedor(QString id) {
    _depura("FacturaProveedorView::pintaComentFacturaProveedor", 0);
    m_comentfacturap->setPlainText(id);
    _depura("END FacturaProveedorView::pintaComentFacturaProveedor", 0);
}


void FacturaProveedorView::pintareffacturap(QString id) {
    _depura("FacturaProveedorView::pintareffacturap", 0);
    m_reffacturap->setText(id);
    _depura("END FacturaProveedorView::pintareffacturap", 0);
}


void FacturaProveedorView::pintaidforma_pago(QString id) {
    _depura("FacturaProveedorView::pintaidforma_pago", 0);
    m_forma_pago->setidforma_pago(id);
    _depura("END FacturaProveedorView::pintaidforma_pago", 0);
}


void FacturaProveedorView::pintaprocesadafacturap(QString id) {
    _depura("FacturaProveedorView::pintaprocesadafacturap", 0);
    if (id == "t" || id == "TRUE") {
        m_procesadafacturap->setChecked(TRUE);
    } else {
        m_procesadafacturap->setChecked(FALSE);
    } // end if
    _depura("END FacturaProveedorView::pintaprocesadafacturap", 0);
}


void FacturaProveedorView::on_mui_pagar_clicked() {
    _depura("FacturaProveedorView::on_mui_pagar_clicked", 0);
    s_nuevoPago();
    _depura("END FacturaProveedorView::on_mui_pagar_clicked", 0);
}


void FacturaProveedorView::on_m_descuentos_editFinish(int, int) {
    _depura("FacturaProveedorView::on_m_descuentos_editFinish", 0);
    calculaypintatotales();
    _depura("END FacturaProveedorView::on_m_descuentos_editFinish", 0);
}


void FacturaProveedorView::on_subform2_editFinish(int, int) {
    _depura("FacturaProveedorView::on_subform2_editFinish", 0);
    calculaypintatotales();
    _depura("END FacturaProveedorView::on_subform2_editFinish", 0);
}


/** inicializar debe ser invocado cuando se crea una nueva ficha sin cargar ningun
    date de la base de datos (por ejemplo una nueva ficha).
    Sirve para inicializar los componenetes sin necesidad de query alguno */
void FacturaProveedorView::inicializar() {
    _depura("FacturaProveedorView::inicializar", 0);
    subform2->inicializar();
    m_descuentos->inicializar();
    pintar();
    dialogChanges_cargaInicial();
    _depura("END FacturaProveedorView::inicializar", 0);
}


/** Pinta los totales de la Factura de proveedor.
    Este metodo se llama desde FacturaProveedor. */
void FacturaProveedorView::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc, Fixed irpf, Fixed reqeq) {
    _depura("PresupuestoView::pintatotales", 0);
    m_totalBases->setText(QString(base.toQString()));
    m_totalTaxes->setText(QString(iva.toQString()));
    m_totalfacturap->setText(QString(total.toQString()));
    m_totalDiscounts->setText(QString(desc.toQString()));
    m_totalIRPF->setText(QString(irpf.toQString()));
    m_totalReqEq->setText(QString(reqeq.toQString()));
    _depura("END PresupuestoView::pintatotales", 0);
}


/** Metodo que responde al hacer un nuevo pago desde la factura.
    Crea una instancia de \ref PagoView y la lanza poniendole los parametros
    del pago iguales a los de la factura de Proveedor.
*/
/// \TODO: Deberia ser directamente on_mui_pagar_clicked()
void FacturaProveedorView::s_nuevoPago() {
    _depura("FacturaProveedorView::s_nuevoPago", 0);
    PagoView *bud = empresaBase()->newPagoView();
    empresaBase()->m_pWorkspace->addWindow(bud);
    bud->setidproveedor(DBvalue("idproveedor"));
    bud->setcantpago(m_totalfacturap->text());
    bud->setrefpago(DBvalue("reffacturap"));
    bud->setcomentpago(DBvalue("descfacturap"));
    bud->pintar();
    bud->show();
    _depura("END FacturaProveedorView::s_nuevoPago", 0);
}


/** Carga una factura de proveedor desde la base de datos.
    Deleta toda la funcionalidad a la clase FacturaProveedor excepto el cambio de titulo
    de la ventana y el reseteo del control de cambios.
    Si todo va bien devuelve 0
    Si algo falla devuelve -1 */
int FacturaProveedorView::cargar(QString id) {
    _depura("FacturaProveedorView::cargar", 0);
    try {
        FacturaProveedor::cargar(id);
        setWindowTitle(tr("Factura de proveedor") + " " + DBvalue("reffacturap") + " - " + DBvalue("numfacturap"));
        meteWindow(windowTitle(), this);
        dialogChanges_cargaInicial();
    } catch (...) {
        return -1;
    } // end try
    _depura("END FacturaProveedorView::cargar");
    return 0;
}


/** Se encarga del guardado de la ficha en la base de datos.
     Pone los valores de los cambios del formulario en el DBRecord mediante los metodos setXXX
     Llama al metodo guardar() de FacturaProveedor quien hace el guardado en la base de datos.
     Si algo falla saca un mensaje de error y produce una excepcion -1.
     Si todo va bien devuelve 0. */
int FacturaProveedorView::guardar() {
    _depura("FacturaProveedorView::guardar", 0);
    try {
        setidproveedor(m_proveedor->idproveedor());
        setnumfacturap(m_numfacturap->text());
        setfechafacturap(m_fechafacturap->text());
        setdescfacturap(m_descfacturap->text());
        setcomentfacturap(m_comentfacturap->toPlainText());
        setreffacturap(m_reffacturap->text());
        setidforma_pago(m_forma_pago->idforma_pago());
        setprocesadafacturap(m_procesadafacturap->isChecked() ? "TRUE" : "FALSE");
        FacturaProveedor::guardar();
        cargar(DBvalue("idfacturap"));
    } catch (...) {
        mensajeInfo(tr("Error al guardar la factura proveedor"));
        throw -1;
    } // end try
    _depura("END FacturaProveedorView::guardar", 0);
    return 0;
}

/** SLOT que responde  a la pulsacion del boton ver albaranes.
    Consulta todos los albaranes con la misma referencia.
    Los instancia y los carga. */
void FacturaProveedorView::on_mui_veralbaranes_clicked() {
    _depura("FacturaProveedorView::on_mui_veralbaranes_clicked", 0);
    QString query = "SELECT * FROM albaranp WHERE refalbaranp='"+DBvalue("reffacturap")+"'";
    cursor2 *cur = empresaBase()->cargacursor(query);
    while (!cur->eof()) {
        AlbaranProveedorView *albpro = empresaBase()->newAlbaranProveedorView();
        albpro->cargar(cur->valor("idalbaranp"));
        empresaBase()->m_pWorkspace->addWindow( albpro);
        albpro->show();
        cur->siguienteregistro();
    } // end while
    delete cur;
    _depura("END FacturaProveedorView::on_mui_veralbaranes_clicked", 0);
}


void FacturaProveedorView::on_m_proveedor_valueChanged(QString id) {
    _depura("FacturaProveedorView::on_m_proveedor_valueChanged", 0);
    subform2->setIdProveedor(id);
    m_forma_pago->setIdProveedor(id);
    _depura("END FacturaProveedorView::on_m_proveedor_valueChanged", 0);
}

