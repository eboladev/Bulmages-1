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

#include <QMessageBox>
#include <QFile>
#include <QTextStream>

#include <stdio.h>

#include "pedidosproveedorlist.h"
#include "funcaux.h"
#include "company.h"
#include "qtable1.h"
#include "configuracion.h"
#include "pedidoproveedorview.h"


PedidosProveedorList::PedidosProveedorList(QWidget *parent, Qt::WFlags flag)
        : QWidget(parent, flag) {
    setupUi(this);
    m_companyact = NULL;
    m_modo = 0;
    mdb_idpedidoproveedor = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
}


PedidosProveedorList::PedidosProveedorList(company *comp, QWidget *parent, Qt::WFlags flag)
        : QWidget(parent, flag) {
    setupUi(this);
    m_companyact = comp;
    m_proveedor->setcompany(comp);
    m_articulo->setcompany(comp);
    mui_list->setcompany(comp);
    presenta();
    m_modo = 0;
    mdb_idpedidoproveedor = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
}


PedidosProveedorList::~PedidosProveedorList() {
    _depura("PedidosProveedorList::~PedidosProveedorList", 0);
    m_companyact->sacaWindow(this);
}


void PedidosProveedorList::presenta() {
    cursor2 * cur= m_companyact->cargacursor("SELECT *, totalpedidoproveedor AS total, bimppedidoproveedor AS base, imppedidoproveedor AS impuestos FROM pedidoproveedor LEFT JOIN proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1 " + generarFiltro());
    mui_list->cargar(cur);
    delete cur;
    /// Hacemos el calculo del total.
    cur = m_companyact->cargacursor("SELECT SUM(totalpedidoproveedor) AS total FROM pedidoproveedor LEFT JOIN proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1 " + generarFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;
}


QString PedidosProveedorList::generarFiltro() {
    /// Tratamiento de los filtros.
    QString filtro = "";

    if (m_filtro->text() != "") {
        filtro = " AND ( descpedidoproveedor LIKE '%" + m_filtro->text() + "%' ";
        filtro += " OR nomproveedor LIKE '%" + m_filtro->text() + "%') ";
    } else {
        filtro = "";
    } // end if

    if (m_proveedor->idproveedor() != "") {
        filtro += " AND pedidoproveedor.idproveedor = " + m_proveedor->idproveedor();
    } // end if

    if (!m_procesados->isChecked() ) {
        filtro += " AND NOT procesadopedidoproveedor";
    } // end if

    if (m_articulo->idarticulo() != "") {
        filtro += " AND idpedidoproveedor IN (SELECT DISTINCT idpedidoproveedor FROM lpedidoproveedor WHERE idarticulo = '" + m_articulo->idarticulo() + "')";
    } // end if

    if (m_fechain->text() != "")
        filtro += " AND fechapedidoproveedor >= '" + m_fechain->text() + "' ";

    if (m_fechafin->text() != "")
        filtro += " AND fechapedidoproveedor <= '" + m_fechafin->text() + "' ";
    //filtro += " ORDER BY "+orden[m_orden->currentItem()];

    return (filtro);
}


void PedidosProveedorList::imprimir() {
    _depura("PedidosProveedorList::imprimir", 0);
    mui_list->imprimirPDF(tr("Listado de Pedidos a Proveedores"));
    _depura("END PedidosProveedorList::imprimir", 0);
}


void PedidosProveedorList::on_mui_borrar_clicked() {
    _depura("PedidosProveedorList::on_mui_borrar_clicked", 0);
    try {
	mdb_idpedidoproveedor = mui_list->DBvalue(QString("idpedidoproveedor"));
	if (mdb_idpedidoproveedor == "") return;
	PedidoProveedorView *prov = new PedidoProveedorView(m_companyact, 0);
	if (prov->cargar(mdb_idpedidoproveedor)) {
		throw -1;
	} // end if
	prov->borrar();
	presenta();
    } catch(...) {
	mensajeInfo(tr("No se pudo borrar el pedido proveedor"));
    } // end try
    _depura("END PedidosProveedorList::on_mui_borrar_clicked", 0);
}


void PedidosProveedorList::editar(int row) {
    _depura("PedidosProveedorList::editar", 0);
    try {
	mdb_idpedidoproveedor = mui_list->DBvalue(QString("idpedidoproveedor"), row);
	if (m_modo == 0) {
		PedidoProveedorView *prov = new PedidoProveedorView(m_companyact, 0);
		if (prov->cargar(mdb_idpedidoproveedor)) {
		delete prov;
		return;
		} // end if
		m_companyact->m_pWorkspace->addWindow(prov);
		prov->show();
	} else {
		emit(selected(mdb_idpedidoproveedor));
	} // end if
    } catch(...) {
	mensajeInfo(tr("Error al cargar el pedido proveedor"));
    } // end try
    _depura("END PedidosProveedorList::editar", 0);
}


void PedidosProveedorList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >= 0)
        editar(a);
    else
        _depura("Debe seleccionar una linea", 2);
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
PedidosProveedorListSubform::PedidosProveedorListSubform(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("pedidoproveedor");
    setDBCampoId("idpedidoproveedor");
    addSHeader("idpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("Id pedido proveedor"));
    addSHeader("codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo de almacen"));
    addSHeader("refpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Referencia pedido proveedor"));
    addSHeader("numpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero pedido proveedor"));
    addSHeader("descpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion pedido proveedor"));
    addSHeader("nomproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre de proveedor"));
    addSHeader("fechapedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha pedido proveedor"));
    addSHeader("contactpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Persona de contacto proveedor"));
    addSHeader("telpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Telefono proveedor"));
    addSHeader("comentpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentario pedido proveedor"));
    addSHeader("idtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id trabajador"));
    addSHeader("idproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id proveedor"));
    addSHeader("idalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id almacen"));
    addSHeader("total", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Total"));
    addSHeader("base", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Base imponible"));
    addSHeader("impuestos", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Impuestos"));
    setinsercion(FALSE);
    setDelete(FALSE);
}

