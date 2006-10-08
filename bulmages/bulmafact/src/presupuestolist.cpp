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

#include <QMessageBox>
#include <QCheckBox>
#include <QFile>
#include <QCheckBox>
#include <QTextStream>

#include "presupuestolist.h"
#include "company.h"
#include "presupuestoview.h"
#include "funcaux.h"
#include "plugins.h"
#include "configuracion.h"


PresupuestoList::PresupuestoList(QWidget *parent, Qt::WFlags flag)
        : QWidget(parent, flag) {
    setupUi(this);
    m_companyact = NULL;
    m_modo = 0;
    m_idpresupuesto = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
}


PresupuestoList::PresupuestoList(company *comp, QWidget *parent, Qt::WFlags flag)
        : QWidget(parent, flag) {
    setupUi(this);
    m_companyact = comp;
    m_cliente->setcompany(comp);
    m_articulo->setcompany(comp);
    mui_list->setcompany(comp);
    presenta();
    m_modo = 0;
    m_idpresupuesto = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
}


PresupuestoList::~PresupuestoList() {
    _depura("PresupuestoList::~PresupuestoList", 0);
    m_companyact->sacaWindow(this);
}


void PresupuestoList::presenta() {
    _depura("PresupuestoList::presenta()\n");

    /// Hacemos el listado y lo presentamos.
    mui_list->cargar("SELECT *, totalpresupuesto AS total, bimppresupuesto AS base, imppresupuesto AS impuestos FROM presupuesto LEFT JOIN  cliente ON presupuesto.idcliente=cliente.idcliente LEFT JOIN almacen ON presupuesto.idalmacen=almacen.idalmacen WHERE 1=1 " + generaFiltro());

    /// Hacemos el calculo del total.
    cursor2 *cur = m_companyact->cargacursor("SELECT SUM(totalpresupuesto) AS total FROM presupuesto LEFT JOIN cliente ON presupuesto.idcliente=cliente.idcliente LEFT JOIN almacen ON presupuesto.idalmacen=almacen.idalmacen WHERE 1=1 " + generaFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;

    _depura("end PresupuestoList::presenta()\n");
}


QString PresupuestoList::generaFiltro() {
    /// Tratamiento de los filtros.
    QString filtro = "";

    if (m_filtro->text() != "") {
        filtro = " AND (descpresupuesto LIKE '%" + m_filtro->text() + "%' ";
        filtro +=" OR refpresupuesto LIKE '"+m_filtro->text()+"%' ";
        filtro +=" OR nomcliente LIKE '%" + m_filtro->text() + "%') ";
    } else {
        filtro = "";
    } // end if

    if (m_cliente->idcliente() != "") {
        filtro += " AND presupuesto.idcliente=" + m_cliente->idcliente();
    } // end if

    if (!m_procesados->isChecked() ) {
        filtro += " AND NOT procesadopresupuesto";
    } // end if

    if (m_articulo->idarticulo() != "") {
        filtro += " AND idpresupuesto IN (SELECT DISTINCT idpresupuesto FROM lpresupuesto WHERE idarticulo='" + m_articulo->idarticulo() + "')";
    }// end if

    if (m_fechain->text() != "")
        filtro += " AND fpresupuesto >= '"+m_fechain->text() + "' ";
    if (m_fechafin->text() != "")
        filtro += " AND fpresupuesto <= '"+m_fechafin->text() + "' ";
    return (filtro);
}


void PresupuestoList::editar(int row) {
    _depura("PresupuestoList::editar", 0);
    try {
	m_idpresupuesto = mui_list->DBvalue(QString("idpresupuesto"), row);
	if (m_modo == 0) {
		PresupuestoView *prov = m_companyact->newBudget();
		if (prov->cargar(m_idpresupuesto)) {
		delete prov;
		return;
		}
		m_companyact->m_pWorkspace->addWindow(prov);
		prov->show();
	} else {
		emit(selected(m_idpresupuesto));
	} // end if
	_depura("END PresupuestoList::editar", 0);
    } catch(...) {
	mensajeInfo(tr("Error al editar el presupuesto"));
    } // end try
}


void PresupuestoList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >= 0)
        editar(a);
    else
        _depura("Debe seleccionar una linea", 2);
}


void PresupuestoList::imprimir() {
    _depura("PresupuestoList::imprimir", 0);
    mui_list->imprimirPDF(tr("Listado de Presupuestos"));
    _depura("END PresupuestoList::imprimir", 0);
}


void PresupuestoList::on_mui_borrar_clicked() {
    _depura("PresupuestoList::on_mui_borrar_clicked", 0);
    try {
        m_idpresupuesto = mui_list->DBvalue(QString("idpresupuesto"));
        if (m_modo == 0) {
            PresupuestoView *prov = m_companyact->newBudget();
            if (prov->cargar(m_idpresupuesto))
                return;
            prov->borrar();
            delete prov;
        } // end if
        presenta();
    } catch(...) {
        mensajeInfo(tr("Error al borrar el presupuesto"));
    } // end try
    _depura("END PresupuestoList::on_mui_borrar_clicked", 0);
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
PresupuestoListSubForm::PresupuestoListSubForm(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("presupuesto");
    setDBCampoId("idpresupuesto");
    addSHeader("idpresupuesto", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID presupuesto"));
    addSHeader("codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo de almacen"));
    addSHeader("numpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de presupuesto"));
    addSHeader("refpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Referencia"));
    addSHeader("descpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion"));
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre del cliente"));
    addSHeader("fpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha del presupuesto"));
    addSHeader("contactpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Persona de contacto"));
    addSHeader("telpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de telefono"));
    addSHeader("comentpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentarios"));
    addSHeader("idtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID trabajador"));
    addSHeader("idcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID cliente"));
    addSHeader("idalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID almacen"));
    addSHeader("base", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Total Base imponible"));
    addSHeader("impuestos", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Total impuestos"));
    addSHeader("total", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Total presupuesto"));
    setinsercion(FALSE);
    setDelete(FALSE);
}

