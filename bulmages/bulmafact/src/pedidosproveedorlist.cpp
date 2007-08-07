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
        : Listado (NULL, parent, flag) {
    _depura("PedidosProveedorList::PedidosProveedorList", 0);
    setupUi(this);
    mdb_idpedidoproveedor = "";
    setSubForm(mui_list);
    hideBusqueda();
    iniciaForm();
    _depura("END PedidosProveedorList::PedidosProveedorList", 0);
}


PedidosProveedorList::PedidosProveedorList(company *comp, QWidget *parent, Qt::WFlags flag)
        : Listado(comp, parent, flag) {
    _depura("PedidosProveedorList::PedidosProveedorList", 0);
    setupUi(this);
    m_proveedor->setEmpresaBase(comp);
    m_articulo->setEmpresaBase(comp);
    mui_list->setEmpresaBase(comp);
    presentar();
    setSubForm(mui_list);
    mdb_idpedidoproveedor = "";
    empresaBase()->meteWindow(windowTitle(), this);
    hideBusqueda();
    iniciaForm();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos("pedidoproveedor");
    _depura("END PedidosProveedorList::PedidosProveedorList", 0);

}

PedidosProveedorList::~PedidosProveedorList() {
    _depura("PedidosProveedorList::~PedidosProveedorList", 0);
    _depura("END PedidosProveedorList::~PedidosProveedorList", 0);
}

void PedidosProveedorList::iniciaForm() {
    _depura("PedidosProveedorList::iniciaForm");
    /// Disparamos los plugins.
    int res = g_plugins->lanza("PedidosProveedorList_iniciaForm", this);
    if (res != 0)
        return;
    mui_procesada->insertItem(0, tr("Todos los pedidos"));
    mui_procesada->insertItem(1, tr("Pedidos procesados"));
    mui_procesada->insertItem(2, tr("Pedidos no procesados"));
    _depura("END PedidosProveedorList::iniciaForm");
}


void PedidosProveedorList::presentar() {
    mui_list->cargar("SELECT *, totalpedidoproveedor AS total, bimppedidoproveedor AS base, imppedidoproveedor AS impuestos FROM pedidoproveedor LEFT JOIN proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1 " + generarFiltro());
    /// Hacemos el calculo del total.
    cursor2 *cur = empresaBase()->cargacursor("SELECT SUM(totalpedidoproveedor) AS total FROM pedidoproveedor LEFT JOIN proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1 " + generarFiltro());
    /// En caso de que el query haya fallado (por problemas de permisos p.ej.) salimos.
    if(!cur) return;
    m_total->setText(cur->valor("total"));
    delete cur;
}


QString PedidosProveedorList::generarFiltro() {
    /// Tratamiento de los filtros.
    QString filtro = "";

    if (m_filtro->text() != "") {
        filtro = " AND ( lower(descpedidoproveedor) LIKE lower('%" + m_filtro->text() + "%') ";
        filtro +=" OR refpedidoproveedor LIKE '" + m_filtro->text() + "%' ";
        filtro += " OR lower(nomproveedor) LIKE lower('%" + m_filtro->text() + "%')) ";
    } // end if
    if (m_proveedor->idproveedor() != "") {
        filtro += " AND pedidoproveedor.idproveedor = " + m_proveedor->idproveedor();
    } // end if


    /// Tratamos los elementos procesados y no procesados.
    if (mui_procesada->currentIndex() == 1) {
        /// Muestra solo las procesadas.
        filtro += " AND procesadopedidoproveedor";
    } else if (mui_procesada->currentIndex() == 2) {
        /// Muestra solo las NO procesadas.
        filtro += " AND NOT procesadopedidoproveedor ";
    } // end if


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


void PedidosProveedorList::borrar() {
    _depura("PedidosProveedorList::borrar", 0);
    int a = mui_list->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Debe seleccionar una linea"));
        return;
    } // end if
    try {
        mdb_idpedidoproveedor = mui_list->DBvalue(QString("idpedidoproveedor"));
        if (modoEdicion()) {
            PedidoProveedorView *ppv = ((company *)empresaBase())->nuevoPedidoProveedorView();
            if (ppv->cargar(mdb_idpedidoproveedor)) {
                throw -1;
            } // end if
            ppv->on_mui_borrar_clicked();
            ppv->close();
        } // end if
        presentar();
    } catch (...) {
        mensajeInfo(tr("Error al borrar el pedido a proveedor"));
    } // end try
    _depura("END PedidosProveedorList::borrar", 0);
}


void PedidosProveedorList::editar(int row) {
    _depura("PedidosProveedorList::editar", 0);
    try {
        mdb_idpedidoproveedor = mui_list->DBvalue(QString("idpedidoproveedor"), row);
        if (modoEdicion()) {
            PedidoProveedorView *prov = new PedidoProveedorView((company *)empresaBase(), 0);
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


void PedidosProveedorList::crear() {
    _depura("PedidosProveedorList::crear", 0);
    PedidoProveedorView *prov = new PedidoProveedorView((company *)empresaBase(), 0);
    empresaBase()->m_pWorkspace->addWindow(prov);
    prov->show();
    _depura("END PedidosProveedorList::crear", 0);
}


void PedidosProveedorList::setEmpresaBase(company *comp) {
    PEmpresaBase::setEmpresaBase(comp);
    m_proveedor->setEmpresaBase(comp);
    mui_list->setEmpresaBase(comp);
}

QString PedidosProveedorList::idpedidoproveedor() {
    return mdb_idpedidoproveedor;
}

void PedidosProveedorList::setidproveedor(QString val) {
    m_proveedor->setidproveedor(val);
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

void PedidosProveedorListSubform::cargar() {
    _depura("PedidosProveedorListSubform::cargar", 0);
    QString SQLQuery = "SELECT * FROM pedidoproveedor";
    cursor2 *cur = empresaBase()->cargacursor(SQLQuery);
    SubForm3::cargar(cur);
    delete cur;
}


void PedidosProveedorListSubform::cargar(QString query) {
    SubForm3::cargar(query);
}

PedidosProveedorListSubform::~PedidosProveedorListSubform() {}
