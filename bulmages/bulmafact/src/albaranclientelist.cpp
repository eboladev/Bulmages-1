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

#include <QCheckBox>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "albaranclientelist.h"
#include "albaranclienteview.h"
#include "company.h"
#include "funcaux.h"


AlbaranClienteList::AlbaranClienteList(QWidget *parent, Qt::WFlags flag, edmode editmodo)
        : QWidget(parent, flag) {
    setupUi(this);
    m_companyact = NULL;
    m_modo = editmodo;
    mdb_idalbaran = "";
    if (m_modo == EditMode)
        meteWindow(windowTitle(), this);
    hideBusqueda();
}


AlbaranClienteList::AlbaranClienteList(company *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo)
        : QWidget(parent, flag) {
    setupUi(this);
    m_companyact = comp;
    m_cliente->setcompany(comp);
    m_articulo->setcompany(comp);
    mui_list->setcompany(comp);
    presenta();
    m_modo = editmodo;
    mdb_idalbaran = "";
    if (m_modo == EditMode)
        m_companyact->meteWindow(windowTitle(), this);
    hideBusqueda();
}


AlbaranClienteList::~AlbaranClienteList() {
    if (m_modo == EditMode)
        m_companyact->sacaWindow(this);
}


void AlbaranClienteList::presenta() {
    _depura("AlbaranClienteList::presenta\n");

    mui_list->cargar("SELECT *, totalalbaran AS total, bimpalbaran AS base, impalbaran AS impuestos FROM albaran LEFT JOIN  cliente ON albaran.idcliente=cliente.idcliente LEFT JOIN almacen ON albaran.idalmacen=almacen.idalmacen LEFT JOIN forma_pago ON albaran.idforma_pago = forma_pago.idforma_pago WHERE 1=1  "+generarFiltro());


    /// Hacemos el calculo del total.
    cursor2 *cur = m_companyact->cargacursor("SELECT SUM(totalalbaran) AS total FROM albaran LEFT JOIN cliente ON albaran.idcliente=cliente.idcliente LEFT JOIN almacen ON almacen.idalmacen=albaran.idalmacen where 1=1 "+generarFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;

    _depura("End AlbaranClienteList::presenta");
}


void AlbaranClienteList::editar(int row) {
    _depura("AlbaranClienteList::editar", 0);
    mdb_idalbaran = mui_list->DBvalue(QString("idalbaran"), row);
    if (m_modo == 0) {
        AlbaranClienteView *prov = m_companyact->newAlbaranClienteView();
        if (prov->cargar(mdb_idalbaran)) {
            delete prov;
            return;
        } // end if
        m_companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(mdb_idalbaran));
    } // end if
    _depura("END AlbaranClienteList::editar", 0);
}


void AlbaranClienteList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >= 0)
        editar(a);
    else
        _depura("Debe seleccionar una linea", 2);
}



void AlbaranClienteList::on_mui_borrar_clicked() {
    _depura("AlbaranClienteList::on_mui_borrar_clicked", 0);
    try {
        mdb_idalbaran = mui_list->DBvalue(QString("idalbaran"));
        if (m_modo == 0) {
            AlbaranClienteView *prov = m_companyact->newAlbaranClienteView();
            if (prov->cargar(mdb_idalbaran))
                throw -1;
            prov->borrar();
        } // end if
        presenta();
    } catch(...) {
        mensajeInfo(tr("Error al borrar el albaran cliente"));
    } // end try
    _depura("END AlbaranClienteList::on_mui_borrar_clicked", 0);
}


void AlbaranClienteList::imprimir() {
    _depura("AlbaranClienteList::imprimir", 0);
    mui_list->imprimirPDF(tr("Albaranes Cliente"));
    _depura("END AlbaranClienteList::imprimir", 0);
}


QString AlbaranClienteList::generarFiltro() {
    /// Tratamiento de los filtros.
    _depura("AlbaranClienteList::generarFiltro", 0);
    QString filtro = "";

    if (m_filtro->text() != "") {
        filtro = " AND ( descalbaran LIKE '%" + m_filtro->text() + "%' ";
        filtro +=" OR refalbaran LIKE '"+m_filtro->text()+"%' ";
        filtro +=" OR nomcliente LIKE '%" + m_filtro->text() + "%') ";
    } else {
        filtro = "";
    } // end if

    if (m_cliente->idcliente() != "")
        filtro += " AND albaran.idcliente='" + m_cliente->idcliente() + "'";

    if (m_articulo->idarticulo() != "")
        filtro += " AND idalbaran IN (SELECT DISTINCT idalbaran FROM lalbaran WHERE idarticulo='" + m_articulo->idarticulo() + "')";

    if (!m_procesados->isChecked() )
        filtro += " AND NOT procesadoalbaran";

    if (m_fechain->text() != "")
        filtro += " AND fechaalbaran >= '" + m_fechain->text() + "' ";

    if (m_fechafin->text() != "")
        filtro += " AND fechaalbaran <= '" + m_fechafin->text() + "' ";

    _depura("END AlbaranClienteList::generarFiltro", 0);
    return (filtro);
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
AlbaranClienteListSubform::AlbaranClienteListSubform(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("albaran");
    setDBCampoId("idalbaran");
    addSHeader("refalbaran", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("Referencia de albaran"));
    addSHeader("codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo de almacen"));
    addSHeader("numalbaran", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de albaran"));
    addSHeader("descalbaran", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion"));
    addSHeader("fechaalbaran", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha de albaran"));
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre de cliente"));
    addSHeader("idforma_pago", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID forma de pago"));
    addSHeader("descforma_pago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion de forma de pago"));
    addSHeader("idtrabajador", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID trabajador"));
    addSHeader("idcliente", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID cliente"));
    addSHeader("idalbaran", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID albaran"));
    addSHeader("comentalbaran", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentarios del albaran"));
    addSHeader("idalmacen", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID almacen"));
    addSHeader("base", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Base imponible"));
    addSHeader("impuestos", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Impuestos"));
    addSHeader("total", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Total"));
    setinsercion(FALSE);
    setDelete(FALSE);
}

