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
#include "pedidoclienteview.h"
#include "pedidosclientelist.h"


PedidosClienteList::PedidosClienteList(QWidget *parent, Qt::WFlags flag)
        : Ficha(parent, flag) {
    setupUi(this);
    companyact = NULL;
    m_modo = 0;
    m_idpedidocliente = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
}


PedidosClienteList::PedidosClienteList(company *comp, QWidget *parent, Qt::WFlags flag)
        : Ficha(parent, flag) {
    setupUi(this);
    companyact = comp;
    m_cliente->setcompany(comp);
    m_articulo->setcompany(comp);
    mui_list->setcompany(comp);
    presenta();
    m_modo = 0;
    m_idpedidocliente = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
}


PedidosClienteList::~PedidosClienteList() {
    _depura("PedidosClienteList::~PedidosClienteList", 0);
}


void PedidosClienteList::presenta() {
    /// Hacemos el listado y lo presentamos.
    mui_list->cargar("SELECT *, totalpedidocliente AS total, bimppedidocliente AS base, imppedidocliente AS impuestos FROM pedidocliente LEFT JOIN  cliente ON pedidocliente.idcliente=cliente.idcliente LEFT JOIN almacen ON pedidocliente.idalmacen=almacen.idalmacen WHERE 1=1  " + generarFiltro());

    /// Hacemos el calculo del total.
    cursor2 *cur = companyact->cargacursor("SELECT SUM(totalpedidocliente) AS total FROM pedidocliente LEFT JOIN cliente ON pedidocliente.idcliente=cliente.idcliente LEFT JOIN almacen ON pedidocliente.idalmacen=almacen.idalmacen WHERE 1=1" + generarFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;
}


QString PedidosClienteList::generarFiltro() {
    /// Tratamiento de los filtros.
    _depura("PedidosClienteList::generarFiltro \n", 0);

    QString filtro = "";
    if (m_filtro->text() != "") {
        filtro = " AND ( descpedidocliente LIKE '%" + m_filtro->text() + "%' ";
        filtro +=" OR refpedidocliente LIKE '"+m_filtro->text()+"%' ";
        filtro += " OR nomcliente LIKE '%" + m_filtro->text() + "%') ";
    } else {
        filtro = "";
    } // end if
    if (m_cliente->idcliente() != "") {
        filtro += " AND pedidocliente.idcliente=" + m_cliente->idcliente();
    } // end if
    if (!m_procesados->isChecked()) {
        filtro += " AND NOT procesadopedidocliente";
    } // end if
    if (m_articulo->idarticulo() != "") {
        filtro += " AND idpedidocliente IN (SELECT DISTINCT idpedidocliente FROM lpedidocliente WHERE idarticulo='" + m_articulo->idarticulo() + "')";
    } // end if
    if (m_fechain->text() != "") {
        filtro += " AND fechapedidocliente >= '" + m_fechain->text() + "' ";
    } // end if
    if (m_fechafin->text() != "") {
        filtro += " AND fechapedidocliente <= '" + m_fechafin->text() + "' ";
    } // end if
    return (filtro);
}


void PedidosClienteList::editar(int row) {
    _depura("ProveedorList::editar", 0);
    try {
        m_idpedidocliente = mui_list->DBvalue(QString("idpedidocliente"), row);
        if (m_modo == 0) {
            PedidoClienteView *prov = new PedidoClienteView(companyact, 0);
            if (prov->cargar(m_idpedidocliente)) {
                delete prov;
                return;
            } // end if
            companyact->m_pWorkspace->addWindow(prov);
            prov->show();
        } else {
            emit(selected(m_idpedidocliente));
        } // end if
    } catch (...) {
        mensajeInfo(tr("Error al cargar el pedido cliente"));
    } // end try
    _depura("END ProveedorList::editar", 0);
}


void PedidosClienteList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >= 0) {
        editar(a);
    } else {
        _depura("Debe seleccionar una linea", 2);
    } // end if
}


void PedidosClienteList::imprimir() {
    _depura("PedidosClienteList::imprimir", 0);
    mui_list->imprimir();
    _depura("END PedidosClienteList::imprimir", 0);
}


void PedidosClienteList::on_mui_borrar_clicked() {
    _depura("PedidosClienteList::on_mui_borrar_clicked", 0);
    try {
        m_idpedidocliente = mui_list->DBvalue(QString("idpedidocliente"));
        if (m_modo == 0) {
            PedidoClienteView *prov = companyact->newPedidoClienteView();
            if (prov->cargar(m_idpedidocliente)) {
                throw -1;
            } // end if
            prov->borrar();
            delete prov;
        } // end if
        presenta();
    } catch (...) {
        mensajeInfo(tr("Error al borrar el pedido cliente"));
    } // end try
    _depura("END PedidosClienteList::on_mui_borrar_clicked", 0);
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
PedidosClienteListSubform::PedidosClienteListSubform(QWidget *parent, const char *) : SubForm2Bf(parent) {
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
}

