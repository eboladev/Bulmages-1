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
#include "configuracion.h"
#include "pedidoproveedorview.h"


PedidosProveedorList::PedidosProveedorList(QWidget *parent, Qt::WFlags flag)
        : FichaBf (NULL, parent, flag) {
    setupUi(this);
    m_modo = 0;
    mdb_idpedidoproveedor = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
}


PedidosProveedorList::PedidosProveedorList(company *comp, QWidget *parent, Qt::WFlags flag)
        : FichaBf(comp, parent, flag) {
    setupUi(this);
    m_proveedor->setEmpresaBase(comp);
    m_articulo->setEmpresaBase(comp);
    mui_list->setEmpresaBase(comp);
    presenta();
    m_modo = 0;
    mdb_idpedidoproveedor = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
}


PedidosProveedorList::~PedidosProveedorList() {
    _depura("PedidosProveedorList::~PedidosProveedorList", 0);
    empresaBase()->sacaWindow(this);
    _depura("END PedidosProveedorList::~PedidosProveedorList", 0);
}


void PedidosProveedorList::presenta() {
    mui_list->cargar("SELECT *, totalpedidoproveedor AS total, bimppedidoproveedor AS base, imppedidoproveedor AS impuestos FROM pedidoproveedor LEFT JOIN proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1 " + generarFiltro());
    /// Hacemos el calculo del total.
    cursor2 *cur = empresaBase()->cargacursor("SELECT SUM(totalpedidoproveedor) AS total FROM pedidoproveedor LEFT JOIN proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1 " + generarFiltro());
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
    if (!m_procesados->isChecked()) {
        filtro += " AND NOT procesadopedidoproveedor ";
    } else {
        filtro += " AND procesadopedidoproveedor ";
    }// end if
    if (m_articulo->idarticulo() != "") {
        filtro += " AND idpedidoproveedor IN (SELECT DISTINCT idpedidoproveedor FROM lpedidoproveedor WHERE idarticulo = '" + m_articulo->idarticulo() + "')";
    } // end if
    if (m_fechain->text() != "") {
        filtro += " AND fechapedidoproveedor >= '" + m_fechain->text() + "' ";
    } // end if
    if (m_fechafin->text() != "") {
        filtro += " AND fechapedidoproveedor <= '" + m_fechafin->text() + "' ";
        //filtro += " ORDER BY "+orden[m_orden->currentItem()];
    } // end if
    return (filtro);
}


void PedidosProveedorList::imprimir() {
    _depura("PedidosProveedorList::imprimir", 0);
    mui_list->imprimirPDF(tr("Pedidos a proveedores"));
    _depura("END PedidosProveedorList::imprimir", 0);
}


void PedidosProveedorList::on_mui_borrar_clicked() {
    _depura("PedidosProveedorList::on_mui_borrar_clicked", 0);
    int a = mui_list->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Debe seleccionar una linea"));
        return;
    } // end if
    try {
        mdb_idpedidoproveedor = mui_list->DBvalue(QString("idpedidoproveedor"));
        if (m_modo == 0) {
            PedidoProveedorView *ppv = empresaBase()->nuevoPedidoProveedorView();
            if (ppv->cargar(mdb_idpedidoproveedor)) {
                throw -1;
            } // end if
            ppv->on_mui_borrar_clicked();
            ppv->close();
        } // end if
        presenta();
    } catch (...) {
        mensajeInfo(tr("Error al borrar el pedido a proveedor"));
    } // end try
    _depura("END PedidosProveedorList::on_mui_borrar_clicked", 0);
}


void PedidosProveedorList::editar(int row) {
    _depura("PedidosProveedorList::editar", 0);
    try {
	   mdb_idpedidoproveedor = mui_list->DBvalue(QString("idpedidoproveedor"), row);
	   if (m_modo == 0) {
		  PedidoProveedorView *prov = new PedidoProveedorView(empresaBase(), 0);
		  if (prov->cargar(mdb_idpedidoproveedor)) {
		  delete prov;
		  return;
		  } // end if
		  empresaBase()->m_pWorkspace->addWindow(prov);
		  prov->show();
	   } else {
		  emit(selected(mdb_idpedidoproveedor));
	   } // end if
    } catch (...) {
    	mensajeInfo(tr("Error al cargar el pedido proveedor"));
    } // end try
    _depura("END PedidosProveedorList::editar", 0);
}


void PedidosProveedorList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Debe seleccionar una linea"));
        return;
    } else {
        editar(a);
    } // end if
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
PedidosProveedorListSubform::PedidosProveedorListSubform(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("pedidoproveedor");
    setDBCampoId("idpedidoproveedor");
    addSHeader("numpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de pedido"));
    addSHeader("nomproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre de proveedor"));
    addSHeader("fechapedidoproveedor", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha pedido proveedor"));
    addSHeader("refpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Referencia pedido"));
    addSHeader("base", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Base Imponible"));
    addSHeader("impuestos", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Impuestos"));
    addSHeader("total", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Total pedido"));
    addSHeader("descpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion pedido proveedor"));
    addSHeader("codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo de almacen"));
    addSHeader("contactpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Persona de contacto proveedor"));
    addSHeader("telpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Telefono proveedor"));
    addSHeader("comentpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentario pedido proveedor"));
    addSHeader("idpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("Id pedido"));
    addSHeader("idtrabajador", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id trabajador"));
    addSHeader("idproveedor", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id proveedor"));
    addSHeader("idalmacen", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id almacen"));
    setinsercion(FALSE);
    setDelete(FALSE);
    setSortingEnabled(TRUE);
}

