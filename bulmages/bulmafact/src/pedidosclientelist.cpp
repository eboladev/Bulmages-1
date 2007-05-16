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

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "company.h"
#include "configuracion.h"
#include "funcaux.h"
#include "plugins.h"
#include "pedidoclienteview.h"
#include "pedidosclientelist.h"


PedidosClienteList::PedidosClienteList(QWidget *parent, Qt::WFlags flag)
        : Listado(NULL, parent, flag) {
    _depura("PedidosClienteList::PedidosClienteList", 0);
    setupUi(this);
    /// Disparamos los plugins.
    int res = g_plugins->lanza("PedidosClienteList_PedidosClienteList", this);
    if (res != 0)
        return;
    m_idpedidocliente = "";
    setSubForm(mui_list);
    hideBusqueda();
    _depura("END PedidosClienteList::PedidosClienteList", 0);
}


PedidosClienteList::PedidosClienteList(company *comp, QWidget *parent, Qt::WFlags flag): Listado(comp, parent, flag) {
    _depura("PedidosClienteList::PedidosClienteList", 0);
    setupUi(this);
    /// Disparamos los plugins.
    int res = g_plugins->lanza("PedidosClienteList_PedidosClienteList", this);
    if (res != 0)
        return;
    m_cliente->setEmpresaBase(comp);
    m_articulo->setEmpresaBase(comp);
    mui_list->setEmpresaBase(comp);
    setSubForm(mui_list);
    presentar();
    m_idpedidocliente = "";
    empresaBase()->meteWindow(windowTitle(), this);
    hideBusqueda();
    _depura("END PedidosClienteList::PedidosClienteList", 0);
}


PedidosClienteList::~PedidosClienteList() {
    _depura("PedidosClienteList::~PedidosClienteList", 0);
    _depura("END PedidosClienteList::~PedidosClienteList", 0);
}


void PedidosClienteList::presentar() {
    _depura("PedidosClienteList::presenta", 0);
    /// Hacemos el listado y lo presentamos.
    mui_list->cargar("SELECT *, totalpedidocliente AS total, bimppedidocliente AS base, imppedidocliente AS impuestos FROM pedidocliente LEFT JOIN  cliente ON pedidocliente.idcliente = cliente.idcliente LEFT JOIN almacen ON pedidocliente.idalmacen=almacen.idalmacen WHERE 1 = 1 " + generarFiltro());

    /// Hacemos el calculo del total.
    cursor2 *cur = empresaBase()->cargacursor("SELECT SUM(totalpedidocliente) AS total FROM pedidocliente LEFT JOIN cliente ON pedidocliente.idcliente=cliente.idcliente LEFT JOIN almacen ON pedidocliente.idalmacen = almacen.idalmacen WHERE 1 = 1 " + generarFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;
    _depura("END PedidosClienteList::presenta", 0);
}


QString PedidosClienteList::generarFiltro() {
    /// Tratamiento de los filtros.
    _depura("PedidosClienteList::generarFiltro", 0);

    QString filtro = "";
    if (m_filtro->text() != "") {
        filtro = " AND ( descpedidocliente LIKE '%" + m_filtro->text() + "%' ";
        filtro +=" OR refpedidocliente LIKE '" + m_filtro->text() + "%' ";
        filtro += " OR nomcliente LIKE '%" + m_filtro->text() + "%') ";
    } else {
        filtro = "";
    } // end if
    if (m_cliente->idcliente() != "") {
        filtro += " AND pedidocliente.idcliente = " + m_cliente->idcliente();
    } // end if
    if (!m_procesados->isChecked()) {
        filtro += " AND NOT procesadopedidocliente ";
    } else {
        filtro += " AND procesadopedidocliente ";
    }// end if
    if (m_articulo->idarticulo() != "") {
        filtro += " AND idpedidocliente IN (SELECT DISTINCT idpedidocliente FROM lpedidocliente WHERE idarticulo = '" + m_articulo->idarticulo() + "')";
    } // end if
    if (m_fechain->text() != "") {
        filtro += " AND fechapedidocliente >= '" + m_fechain->text() + "' ";
    } // end if
    if (m_fechafin->text() != "") {
        filtro += " AND fechapedidocliente <= '" + m_fechafin->text() + "' ";
    } // end if
    return (filtro);
}


void PedidosClienteList::crear() {
    _depura("PedidosClienteList:crear", 0);
    PedidoClienteView *prov = new PedidoClienteView((company *)empresaBase(), 0);
    empresaBase()->m_pWorkspace->addWindow(prov);
    prov->show();
    _depura("END PedidosClienteList:crear", 0);
}


/// \TODO: Company debe instanciar la clase y no hacerse asi como esta ahora.
void PedidosClienteList::editar(int row) {
    _depura("ProveedorList::editar", 0);
    try {
        m_idpedidocliente = mui_list->DBvalue(QString("idpedidocliente"), row);
        if (modoEdicion()) {
            PedidoClienteView *prov = new PedidoClienteView((company *)empresaBase(), 0);
            if (prov->cargar(m_idpedidocliente)) {
                delete prov;
                return;
            } // end if
            empresaBase()->m_pWorkspace->addWindow(prov);
            prov->show();
        } else {
            emit(selected(m_idpedidocliente));
        } // end if
    } catch (...) {
        mensajeInfo(tr("Error al cargar el pedido cliente"));
    } // end try
    _depura("END ProveedorList::editar", 0);
}




void PedidosClienteList::imprimir() {
    _depura("PedidosClienteList::imprimir", 0);
    mui_list->imprimirPDF(tr("Pedidos de clientes"));
    _depura("END PedidosClienteList::imprimir", 0);
}


void PedidosClienteList::borrar() {
    _depura("PedidosClienteList::borrar", 0);
    int a = mui_list->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Debe seleccionar una linea"));
        return;
    } // end if
    try {
        m_idpedidocliente = mui_list->DBvalue(QString("idpedidocliente"));
        if (modoEdicion()) {
            PedidoClienteView *pcv = ((company *)empresaBase())->newPedidoClienteView();
            if (pcv->cargar(m_idpedidocliente)) {
                throw -1;
            } // end if
            pcv->on_mui_borrar_clicked();
            pcv->close();
        } // end if
        presentar();
    } catch (...) {
        mensajeInfo(tr("Error al borrar el pedido de cliente"));
    } // end try
    _depura("END PedidosClienteList::borrar", 0);
}



void PedidosClienteList::setEmpresaBase(company *comp) {
    PEmpresaBase::setEmpresaBase( comp);
    m_cliente->setEmpresaBase(comp);
    mui_list->setEmpresaBase(comp);
}

QString PedidosClienteList::idpedidocliente() {
    return m_idpedidocliente;
}

void PedidosClienteList::setidcliente(QString val) {
    m_cliente->setidcliente(val);
}

/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
PedidosClienteListSubform::PedidosClienteListSubform(QWidget *parent, const char *) : SubForm2Bf(parent) {
    _depura("PedidosClienteListSubform::PedidosClienteListSubform", 0);
    /// Disparamos los plugins.
    int res = g_plugins->lanza("PedidosClienteListSubform_PedidosClienteListSubform", this);
    if (res != 0)
        return;
    setDBTableName("pedidocliente");
    setDBCampoId("idpedidocliente");
    addSHeader("numpedidocliente", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero pedido cliente"));
    addSHeader("refpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Referencia pedido cliente"));
    addSHeader("codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo de almacen"));
    addSHeader("descpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion pedido cliente"));
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre cliente"));
    addSHeader("fechapedidocliente", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha pedido cliente"));
    addSHeader("contactpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Persona de contacto"));
    addSHeader("telpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Telefono pedido cliente"));
    addSHeader("comentpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentario pedido"));
    addSHeader("total", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Total pedido"));
    addSHeader("base", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Base imponible"));
    addSHeader("impuestos", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Impuestos"));
    addSHeader("idtrabajador", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id trabajador"));
    addSHeader("idcliente", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id cliente"));
    addSHeader("idalmacen", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id almacen"));
    addSHeader("idpedidocliente", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("Id pedido cliente"));
    setinsercion(FALSE);
    setDelete(FALSE);
    setSortingEnabled(TRUE);
    _depura("END PedidosClienteListSubform::PedidosClienteListSubform", 0);
}

void PedidosClienteListSubform::cargar() {
    _depura("PedidosClienteListSubform::cargar", 0);
    QString SQLQuery = "SELECT * FROM pedidocliente";
    cursor2 *cur = empresaBase()->cargacursor(SQLQuery);
    SubForm3::cargar(cur);
    delete cur;
}


void PedidosClienteListSubform::cargar(QString query) {
    SubForm3::cargar(query);
}


