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

#include "busquedareferencia.h"
#include "company.h"
#include "funcaux.h"
#include "informereferencia.h"
#include "presupuestoview.h"
#include "pedidoclienteview.h"
#include "albaranclienteview.h"
#include "facturaview.h"
#include "cobroview.h"
#include "pagoview.h"
#include "pedidoproveedorview.h"
#include "albaranproveedorview.h"
#include "facturapview.h"


/** Inicializa todos los componentes del Widget a NULL para que no haya
    confusion en determinar si un objeto ha sido creado o no.
*/
/**
\param parent
**/
BusquedaReferencia::BusquedaReferencia(QWidget *parent)
        : BLWidget(parent) {
    _depura("BusquedaReferencia::BusquedaReferencia", 0);
    setupUi(this);
    _depura("END BusquedaReferencia::BusquedaReferencia", 0);
}


/** No requiere de acciones adicionales en la destruccion de la clase.
*/
/**
**/
BusquedaReferencia::~BusquedaReferencia() {
    _depura("BusquedaReferencia::~BusquedaReferencia", 0);
    _depura("END BusquedaReferencia::~BusquedaReferencia", 0);
}


/** SLOT que responde a la peticion del informe por referencia.
    Crea una instancia de InformeReferencia, la inicializa y la lanza.
*/
/**
**/
void BusquedaReferencia::on_mui_informe_clicked() {
    _depura("BusquedaReferencia::on_mui_buscar_clicked\n", 0);
    InformeReferencia *inf = new InformeReferencia((Company *)empresaBase());
    inf->setreferencia(mui_referencia->text());
    inf->generarinforme();
    delete inf;
    _depura("END BusquedaReferencia::on_mui_buscar_clicked\n", 0);

}


/** SLOT que responde al fin de edicion de la casilla de referencia.
    Emite la se&ntilde;al valueChanged() para notificaciones.
*/
/**
**/
void BusquedaReferencia::on_mui_referencia_editingFinished() {
    _depura("BusquedaReferencia::on_m_cifcliente_editingFinished", 0);
    emit(valueChanged(mui_referencia->text()));
    _depura("END BusquedaReferencia::on_m_cifcliente_editingFinished", 0);
}


/** SLOT que responde a la pulsacion del boton abrirtodo.
    Busca en todos los documentos de la base de datos (presupuestos, pedidos, albaranes, facturas, cobros y pagos)
    Todos aquellos elementos que tengan la misma referencia que el objeto abierto.
    Instancia los objetos correspondientes y los lanza.
*/
/**
**/
void BusquedaReferencia::on_mui_abrirtodo_clicked() {
    _depura("BusquedddaReferencia::on_mui_abrirtodo_clicked", 0);
    /// Empezamos con los presupuestos.
    QString SQLQuery = "SELECT * FROM presupuesto WHERE refpresupuesto = '" + mui_referencia->text() + "'";
    cursor2 *cur = empresaBase()->cargacursor(SQLQuery);
    while (!cur->eof()) {
        PresupuestoView *bud = ((Company *)empresaBase())->nuevoPresupuestoView();
        empresaBase()->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idpresupuesto"));
        bud->show();
        cur->siguienteregistro();
    } // end while
    delete cur;

    SQLQuery = "SELECT * FROM pedidocliente WHERE refpedidocliente = '" + mui_referencia->text() + "'";
    cur = empresaBase()->cargacursor(SQLQuery);
    while (!cur->eof()) {
        PedidoClienteView *bud = ((Company *)empresaBase())->newPedidoClienteView();
        empresaBase()->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idpedidocliente"));
        bud->show();
        cur->siguienteregistro();
    } // end while
    delete cur;

    SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + mui_referencia->text() + "'";
    cur = empresaBase()->cargacursor(SQLQuery);
    while (!cur->eof()) {
        AlbaranClienteView *bud = ((Company *)empresaBase())->newAlbaranClienteView();
        empresaBase()->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idalbaran"));
        bud->show();
        cur->siguienteregistro();
    } // end while
    delete cur;

    SQLQuery = "SELECT * FROM factura WHERE reffactura = '" + mui_referencia->text() + "'";
    cur = empresaBase()->cargacursor(SQLQuery);
    while (!cur->eof()) {
        FacturaView *bud = ((Company *)empresaBase())->newFacturaView();
        empresaBase()->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idfactura"));
        bud->show();
        cur->siguienteregistro();
    } // end while
    delete cur;

    SQLQuery = "SELECT * FROM cobro WHERE refcobro = '" + mui_referencia->text() + "'";
    cur = empresaBase()->cargacursor(SQLQuery);
    while (!cur->eof()) {
        CobroView *bud = ((Company *)empresaBase())->newCobroView();
        empresaBase()->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idcobro"));
        bud->show();
        cur->siguienteregistro();
    } // end while
    delete cur;

    SQLQuery = "SELECT * FROM pago WHERE refpago = '" + mui_referencia->text() + "'";
    cur = empresaBase()->cargacursor(SQLQuery);
    while (!cur->eof()) {
        PagoView *bud = ((Company *)empresaBase())->newPagoView();
        empresaBase()->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idpago"));
        bud->show();
        cur->siguienteregistro();
    } // end while
    delete cur;

    SQLQuery = "SELECT * FROM pedidoproveedor WHERE refpedidoproveedor = '" + mui_referencia->text() + "'";
    cur = empresaBase()->cargacursor(SQLQuery);
    while (!cur->eof()) {
        PedidoProveedorView *bud = ((Company *)empresaBase())->nuevoPedidoProveedorView();
        empresaBase()->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idpedidoproveedor"));
        bud->show();
        cur->siguienteregistro();
    } // end while
    delete cur;

    SQLQuery = "SELECT * FROM albaranp WHERE refalbaranp = '" + mui_referencia->text() + "'";
    cur = empresaBase()->cargacursor(SQLQuery);
    while (!cur->eof()) {
        AlbaranProveedorView *bud = ((Company *)empresaBase())->newAlbaranProveedorView();
        empresaBase()->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idalbaranp"));
        bud->show();
        cur->siguienteregistro();
    } // end while
    delete cur;


    SQLQuery = "SELECT * FROM facturap WHERE reffacturap = '" + mui_referencia->text() + "'";
    cur = empresaBase()->cargacursor(SQLQuery);
    while (!cur->eof()) {
        FacturaProveedorView *bud = ((Company *)empresaBase())->newFacturaProveedorView();
        empresaBase()->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idfacturap"));
        bud->show();
        cur->siguienteregistro();
    } // end while
    delete cur;

    _depura("END BusquedddaReferencia::on_mui_abrirtodo_clicked", 0);
}


/** Establece la referencia que va a mostrar el control
**/
/**
\param val
**/
void BusquedaReferencia::setText(QString val) {
    _depura("BusquedaReferencia::setText", 0);
    mui_referencia->setText(val);
    _depura("END BusquedaReferencia::setText", 0);
}


/** Establece la referencia que va a mostrar el control
**/
/**
\param val
**/
void BusquedaReferencia::setValorCampo(QString val) {
    _depura("BusquedaReferencia::setText", 0);
    mui_referencia->setText(val);
    _depura("END BusquedaReferencia::setText", 0);
}


/** Devuelve el texto que esta escrito en el control
**/
/**
\return
**/
QString BusquedaReferencia::text() {
    _depura("BusquedaReferencia::text", 0);
    _depura("END BusquedaReferencia::text", 0);
    return mui_referencia->text();
}


/** Devuelve el texto que esta escrito en el control
**/
/**
\return
**/
QString BusquedaReferencia::valorCampo() {
    _depura("BusquedaReferencia::text", 0);
    _depura("END BusquedaReferencia::text", 0);
    return mui_referencia->text();
}


