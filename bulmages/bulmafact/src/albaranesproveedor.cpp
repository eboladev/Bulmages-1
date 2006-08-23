/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <QCheckBox>
#include <QCheckBox>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "albaranesproveedor.h"
#include "albaranproveedorview.h"
#include "company.h"
#include "configuracion.h"
#include "funcaux.h"
#include "presupuestoview.h"
#include "qtable1.h"


AlbaranesProveedor::AlbaranesProveedor(QWidget *parent, Qt::WFlags flag)
        : QWidget(parent, flag) {
    setupUi(this);
    m_companyact = NULL;
    m_modo = 0;
    mdb_idalbaranp = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
}


AlbaranesProveedor::AlbaranesProveedor(company *comp, QWidget *parent, Qt::WFlags flag)
        : QWidget(parent, flag) {
    setupUi(this);
    m_companyact = comp;
    m_proveedor->setcompany(comp);
    m_articulo->setcompany(comp);
    mui_list->setcompany(comp);
    presenta();
    m_modo = 0;
    mdb_idalbaranp = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
}


AlbaranesProveedor::~AlbaranesProveedor() {
    m_companyact->refreshAlbaranesProveedor();
    m_companyact->sacaWindow(this);
}


void AlbaranesProveedor::presenta() {
    _depura("AlbaranesProveedor::presenta().", 1);
    if (m_companyact != NULL ) {
        cursor2 * cur = m_companyact->cargacursor("SELECT *, totalalbaranp AS total, " \
                        "bimpalbaranp AS base, impalbaranp AS impuestos " \
                        "FROM albaranp LEFT " \
                        "JOIN proveedor ON albaranp.idproveedor = " \
                        "proveedor.idproveedor LEFT JOIN almacen ON " \
                        "albaranp.idalmacen = almacen.idalmacen LEFT JOIN " \
                        "forma_pago ON albaranp.idforma_pago = " \
                        "forma_pago.idforma_pago WHERE 1 = 1 " + generaFiltro());
        mui_list->cargar(cur);
        delete cur;

        /// Hacemos el calculo del total.
        cur = m_companyact->cargacursor("SELECT SUM(totalalbaranp) " \
                                        "AS total FROM albaranp LEFT JOIN proveedor ON " \
                                        "albaranp.idproveedor = proveedor.idproveedor LEFT " \
                                        "JOIN almacen ON albaranp.idalmacen = almacen.idalmacen " \
                                        "WHERE 1 = 1 " + generaFiltro());
        m_total->setText(cur->valor("total"));
        delete cur;
    }
    _depura("END AlbaranesProveedor::presenta().", 0);
}


QString AlbaranesProveedor::generaFiltro() {
    /// Tratamiento de los filtros.
    _depura("Tratamos el filtro.", 0);
    QString filtro = "";

    if (m_filtro->text() != "") {
        filtro = " AND (descalbaranp LIKE '%" + m_filtro->text() + "%' ";
        filtro +=" OR nomproveedor LIKE '%" + m_filtro->text() + "%') ";
    } else {
        filtro = "";
    }

    if (m_proveedor->idproveedor() != "")
        filtro += " AND albaranp.idproveedor = " + m_proveedor->idproveedor();
    if (!m_procesados->isChecked())
        filtro += " AND NOT procesadoalbaranp";
    if (m_articulo->idarticulo() != "")
        filtro += " AND idalbaranp IN (SELECT DISTINCT idalbaranp FROM lalbaranp " \
                  "WHERE idarticulo = '" + m_articulo->idarticulo() + "')";
    if (m_fechain->text() != "")
        filtro += " AND fechaalbaranp >= '" + m_fechain->text() + "' ";
    if (m_fechafin->text() != "")
        filtro += " AND fechaalbaranp <= '" + m_fechafin->text() + "' ";
    return (filtro);
}


void AlbaranesProveedor::editar(int row) {
    _depura("AlbaranesProveedor::editar", 0);
    mdb_idalbaranp = mui_list->DBvalue(QString("idalbaranp"), row);
    if (m_modo == 0) {
        AlbaranProveedorView *prov = new AlbaranProveedorView(m_companyact, 0);
        if (prov->cargar(mdb_idalbaranp)) {
            delete prov;
            return;
        } // end if
        m_companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(mdb_idalbaranp));
    } // end if
    _depura("END AlbaranesProveedor::editar", 0);
}


void AlbaranesProveedor::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >= 0)
        editar(a);
    else
        _depura("Debe seleccionar una linea", 2);
}


void AlbaranesProveedor::imprimir() {
    _depura("AlbaranesProveedor::imprimir", 0);
	mui_list->imprimirPDF(tr("Albaranes Proveedor"));
    _depura("END AlbaranesProveedor::imprimir", 0);
}


void AlbaranesProveedor::on_mui_borrar_clicked() {
    _depura("AlbaranesProveedor::on_mui_borrar_clicked", 0);
    try {
        mdb_idalbaranp = mui_list->DBvalue(QString("idalbaranp"));
        if (m_modo == 0) {
            AlbaranProveedorView *bud = m_companyact->newAlbaranProveedorView();
            bud->cargar(mdb_idalbaranp);
            bud->borrar();
            delete bud;
        } // end if
        presenta();
    } catch(...) {
        mensajeInfo(tr("Error al borrar albaran de proveedor"));
    } // end try
    _depura("END AlbaranesProveedor::on_mui_borrar_clicked", 0);

}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
AlbaranesProveedorListSubform::AlbaranesProveedorListSubform(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("albaranp");
    setDBCampoId("idalbaranp");
    addSHeader("idalbaranp", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID albaran de proveedor"));
    addSHeader("numalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de albaran de proveedor"));
    addSHeader("descalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion del albaran de proveedor"));
    addSHeader("refalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Referencia del albaran de proveedor"));
    addSHeader("fechaalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha del albaran de proveedor"));
    addSHeader("comentalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentario del albaran de proveedor"));
    addSHeader("procesadoalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Albaran de proveedor procesado"));
    addSHeader("idproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID proveedor"));
    addSHeader("idforma_pago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID forma de pago"));
    addSHeader("idalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID almacen"));
    addSHeader("nomproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nommbre del proveedor"));
    addSHeader("nomalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre del almacen"));
    addSHeader("descforma_pago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion de la forma de pago"));
    addSHeader("base", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Base imponible"));
    addSHeader("impuestos", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Impuestos"));
    addSHeader("total", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Total albaran"));
    setinsercion(FALSE);
    setDelete(FALSE);
}

