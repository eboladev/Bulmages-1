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

#include "pedidosclientelist.h"
#include "company.h"
#include "qtable1.h"
#include "funcaux.h"
#include "configuracion.h"
#include "pedidoclienteview.h"


PedidosClienteList::PedidosClienteList(QWidget *parent, const char *name, Qt::WFlags flag)
        : QWidget(parent, name, flag) {
    setupUi(this);
    companyact = NULL;
    m_modo = 0;
    m_idpedidocliente = "";
    meteWindow(caption(), this);
    hideBusqueda();
}


PedidosClienteList::PedidosClienteList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag)
        : QWidget(parent, name, flag) {
    setupUi(this);
    companyact = comp;
    m_cliente->setcompany(comp);
    m_articulo->setcompany(comp);
    mui_list->setcompany(comp);
    presenta();
    m_modo = 0;
    m_idpedidocliente = "";
    meteWindow(caption(), this);
    hideBusqueda();
}


PedidosClienteList::~PedidosClienteList() {
    _depura("PedidosClienteList::~PedidosClienteList", 0);
    companyact->sacaWindow(this);
}


void PedidosClienteList::presenta() {
    /// Hacemos el listado y lo presentamos.
    cursor2 * cur= companyact->cargacursor("SELECT *, calctotalpedcli(idpedidocliente) AS total, calcbimppedcli(idpedidocliente) AS base, calcimpuestospedcli(idpedidocliente) AS impuestos FROM pedidocliente LEFT JOIN  cliente ON pedidocliente.idcliente=cliente.idcliente LEFT JOIN almacen ON pedidocliente.idalmacen=almacen.idalmacen WHERE 1=1  " + generarFiltro());
    mui_list->cargar(cur);
    delete cur;

    /// Hacemos el calculo del total.
    cur = companyact->cargacursor("SELECT SUM(calctotalpedcli(idpedidocliente)) AS total FROM pedidocliente LEFT JOIN cliente ON pedidocliente.idcliente=cliente.idcliente LEFT JOIN almacen ON pedidocliente.idalmacen=almacen.idalmacen WHERE 1=1" + generarFiltro());
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

    if (m_fechain->text() != "")
        filtro += " AND fechapedidocliente >= '" + m_fechain->text() + "' ";

    if (m_fechafin->text() != "")
        filtro += " AND fechapedidocliente <= '" + m_fechafin->text() + "' ";

    return (filtro);
}


void PedidosClienteList::editar(int row) {
    _depura("ProveedorList::editar", 0);
    m_idpedidocliente = mui_list->DBvalue(QString("idpedidocliente"), row);
    if (m_modo == 0) {
        PedidoClienteView *prov = new PedidoClienteView(companyact, 0, QApplication::translate("PedidosClienteList", "Edicion de pedidos a cliente"));
        if (prov->cargar(m_idpedidocliente)) {
            return;
        }
        companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(m_idpedidocliente));
    } // end if
    _depura("END ProveedorList::editar", 0);
}


void PedidosClienteList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >= 0)
        editar(a);
    else
        _depura("Debe seleccionar una linea", 2);
}


void PedidosClienteList::imprimir() {
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "pedidoscliente.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "pedidoscliente.rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS
    archivo = "copy " + archivo + " " + archivod;
#else
    archivo = "cp " + archivo + " " + archivod;
#endif

    system (archivo.ascii());

    /// Copiamos el logo.
#ifdef WINDOWS
    archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#else
    archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#endif

    QFile file;
    file.setName(archivod);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;

    /// Linea de totales del presupuesto.
    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += mui_list->imprimir();
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]",fitxersortidatxt);

    if (file.open( QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    }
    invocaPDF("pedidoscliente");
}


void PedidosClienteList::on_mui_borrar_clicked() {
    _depura("PedidosClienteList::on_mui_borrar_clicked", 0);
    m_idpedidocliente = mui_list->DBvalue(QString("idpedidocliente"));
    if (m_modo == 0) {
        PedidoClienteView *prov = new PedidoClienteView(companyact, 0, QApplication::translate("PedidosClienteList", "Edicion de pedidos a cliente"));
        if (prov->cargar(m_idpedidocliente))
            return;
        prov->borrar();
        delete prov;
    } // end if
    _depura("END PedidosClienteList::on_mui_borrar_clicked", 0);
    presenta();
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
PedidosClienteListSubform::PedidosClienteListSubform(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("pedidocliente");
    setDBCampoId("idpedidocliente");
    addSHeader("idpedidocliente", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("Id pedido cliente"));
    addSHeader("codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo de almacen"));
    addSHeader("refpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Referencia pedido cliente"));
    addSHeader("numpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero pedido cliente"));
    addSHeader("descpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion pedido cliente"));
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre cliente"));
    addSHeader("fechapedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha pedido cliente"));
    addSHeader("contactpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Persona de contacto"));
    addSHeader("telpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Telefono pedido cliente"));
    addSHeader("comentpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentario pedido"));
    addSHeader("idtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id trabajador"));
    addSHeader("idcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id cliente"));
    addSHeader("idalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id almacen"));
    addSHeader("total", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Total pedido"));
    addSHeader("base", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Base imponible"));
    addSHeader("impuestos", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Impuestos"));
    setinsercion(FALSE);
}

