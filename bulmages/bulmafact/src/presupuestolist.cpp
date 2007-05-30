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
        : Listado(NULL, parent, flag) {
    _depura("PresupuestoList::PresupuestoList(1)", 0);
    setupUi(this);
    /// Disparamos los plugins.
    int res = g_plugins->lanza("PresupuestoList_PresupuestoList", this);
    if (res != 0)
        return;
    m_idpresupuesto = "";
    setSubForm(mui_list);
    hideBusqueda();
    iniciaForm();
    _depura("END PresupuestoList::PresupuestoList(1)", 0);
}


PresupuestoList::PresupuestoList(company *comp, QWidget *parent, Qt::WFlags flag)
        : Listado(comp, parent, flag) {
    _depura("PresupuestoList::PresupuestoList(2)", 0);
    setupUi(this);
    /// Disparamos los plugins.
    int res = g_plugins->lanza("PresupuestoList_PresupuestoList", this);
    if (res != 0)
        return;
    m_cliente->setEmpresaBase(comp);
    m_articulo->setEmpresaBase(comp);
    mui_list->setEmpresaBase(comp);
    setSubForm(mui_list);
    presentar();
    m_idpresupuesto = "";
    empresaBase()->meteWindow(windowTitle(), this);
    hideBusqueda();
    iniciaForm();
    _depura("END PresupuestoList::PresupuestoList(2)", 0);
}


void PresupuestoList::iniciaForm() {
    _depura("PresupuestoList::iniciaForm");
    /// Disparamos los plugins.
    int res = g_plugins->lanza("PresupuestoList_iniciaForm", this);
    if (res != 0)
        return;
    mui_procesada->insertItem(0, tr("Todos los presupuestos"));
    mui_procesada->insertItem(1, tr("Presupuestos procesados"));
    mui_procesada->insertItem(2, tr("Presupuestos no procesados"));
    _depura("END PresupuestoList::iniciaForm");
}


PresupuestoList::~PresupuestoList() {
    _depura("PresupuestoList::~PresupuestoList", 0);
    _depura("END PresupuestoList::~PresupuestoList", 0);
}





QString PresupuestoList::idpresupuesto() {
    return m_idpresupuesto;
}



void PresupuestoList::setEmpresaBase(company *comp) {
    PEmpresaBase::setEmpresaBase(comp);
    m_cliente->setEmpresaBase(comp);
    m_articulo->setEmpresaBase(comp);
    mui_list->setEmpresaBase(comp);
}



void PresupuestoList::setidcliente(QString val) {
    m_cliente->setidcliente(val);
}


void PresupuestoList::setidarticulo(QString val) {
    m_articulo->setidarticulo(val);
}


void PresupuestoList::crear() {
    ((company *)empresaBase())->s_newPresupuestoCli();
}



void PresupuestoList::presentar() {
    _depura("PresupuestoList::presentar", 0);

    /// Hacemos el listado y lo presentamos.
    mui_list->cargar("SELECT *, totalpresupuesto AS total, bimppresupuesto AS base, imppresupuesto AS impuestos FROM presupuesto LEFT JOIN  cliente ON presupuesto.idcliente=cliente.idcliente LEFT JOIN almacen ON presupuesto.idalmacen=almacen.idalmacen WHERE 1=1 " + generaFiltro());

    /// Hacemos el calculo del total.
    cursor2 *cur = empresaBase()->cargacursor("SELECT SUM(totalpresupuesto) AS total FROM presupuesto LEFT JOIN cliente ON presupuesto.idcliente=cliente.idcliente LEFT JOIN almacen ON presupuesto.idalmacen=almacen.idalmacen WHERE 1=1 " + generaFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;

    _depura("END PresupuestoList::presentar", 0);
}


QString PresupuestoList::generaFiltro() {
    _depura("PresupuestoList::generaFiltro", 0);
    /// Tratamiento de los filtros.
    QString filtro = "";

    if (m_filtro->text() != "") {
        filtro = " AND ( lower(descpresupuesto) LIKE lower('%" + m_filtro->text() + "%') ";
        filtro += " OR refpresupuesto LIKE '" + m_filtro->text() + "%' ";
        filtro += " OR lower(nomcliente) LIKE lower('%" + m_filtro->text() + "%')) ";
    } // end if

    if (m_cliente->idcliente() != "") {
        filtro += " AND presupuesto.idcliente = " + m_cliente->idcliente();
    } // end if

    /// Tratamos los elementos procesados y no procesados.
    if (mui_procesada->currentIndex() == 1) {
        /// Muestra solo las procesadas.
        filtro += " AND procesadopresupuesto";
    } else if (mui_procesada->currentIndex() == 2) {
        /// Muestra solo las NO procesadas.
        filtro += " AND NOT procesadopresupuesto ";
    } // end if

    if (m_articulo->idarticulo() != "") {
        filtro += " AND idpresupuesto IN (SELECT DISTINCT idpresupuesto FROM lpresupuesto WHERE idarticulo='" + m_articulo->idarticulo() + "')";
    }// end if

    if (m_fechain->text() != "")
        filtro += " AND fpresupuesto >= '" + m_fechain->text() + "' ";
    if (m_fechafin->text() != "")
        filtro += " AND fpresupuesto <= '" + m_fechafin->text() + "' ";
    return (filtro);
    _depura("END PresupuestoList::generaFiltro", 0);
}


void PresupuestoList::editar(int row) {
    _depura("PresupuestoList::editar", 0);
    try {
        m_idpresupuesto = mui_list->DBvalue(QString("idpresupuesto"), row);
        if (modoEdicion()) {
            PresupuestoView *prov = ((company *)empresaBase())->nuevoPresupuestoView();
            if (prov->cargar(m_idpresupuesto)) {
                delete prov;
                return;
            }
            empresaBase()->m_pWorkspace->addWindow(prov);
            prov->show();
        } else {
            emit(selected(m_idpresupuesto));
        } // end if
        _depura("END PresupuestoList::editar", 0);
    } catch(...) {
        mensajeInfo(tr("Error al editar el presupuesto"));
    } // end try
}



void PresupuestoList::imprimir() {
    _depura("PresupuestoList::imprimir", 0);
    mui_list->imprimirPDF(tr("Presupuestos a clientes"));
    _depura("END PresupuestoList::imprimir", 0);
}


void PresupuestoList::borrar() {
    _depura("PresupuestoList::borrar", 0);
    int a = mui_list->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Debe seleccionar una linea"));
        return;
    } // end if
    try {
        m_idpresupuesto = mui_list->DBvalue(QString("idpresupuesto"));
        if (modoEdicion()) {
            PresupuestoView *pv = ((company *)empresaBase())->nuevoPresupuestoView();
            if (pv->cargar(m_idpresupuesto))
                throw -1;
            pv->on_mui_borrar_clicked();
            pv->close();
        } // end if
        presentar();
    } catch (...) {
        mensajeInfo(tr("Error al borrar el presupuesto"));
    } // end try
    _depura("END PresupuestoList::borrar", 0);
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
PresupuestoListSubForm::PresupuestoListSubForm(QWidget *parent, const char *) : SubForm2Bf(parent) {
    _depura("PresupuestoListSubForm::PresupuestoListSubForm", 0);
    /// Disparamos los plugins.
    int res = g_plugins->lanza("PresupuestoListSubForm_PresupuestoListSubForm", this);
    if (res != 0)
        return;
    setDBTableName("presupuesto");
    setDBCampoId("idpresupuesto");
    addSHeader("idpresupuesto", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID presupuesto"));
    addSHeader("codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo de almacen"));
    addSHeader("numpresupuesto", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de presupuesto"));
    addSHeader("refpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Referencia"));
    addSHeader("descpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion"));
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre del cliente"));
    addSHeader("fpresupuesto", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha del presupuesto"));
    addSHeader("contactpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Persona de contacto"));
    addSHeader("telpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de telefono"));
    addSHeader("comentpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentarios"));
    addSHeader("idtrabajador", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID trabajador"));
    addSHeader("idcliente", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID cliente"));
    addSHeader("idalmacen", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID almacen"));
    addSHeader("base", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Total Base imponible"));
    addSHeader("impuestos", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Total impuestos"));
    addSHeader("total", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Total presupuesto"));
    setinsercion(FALSE);
    setDelete(FALSE);
    setSortingEnabled(TRUE);
    /// Disparamos los plugins.
    res = g_plugins->lanza("PresupuestoListSubForm_PresupuestoListSubForm_Post", this);
    if (res != 0)
        return;
    _depura("PresupuestoListSubForm::PresupuestoListSubForm", 0);
}


void PresupuestoListSubForm::cargar() {
        _depura("PresupuestoListSubForm::cargar", 0);
        QString SQLQuery = "SELECT * FROM presupuesto";
        cursor2 *cur= empresaBase()->cargacursor(SQLQuery);
        SubForm3::cargar(cur);
        delete cur;
        _depura("END PresupuestoListSubForm::cargar", 0);
}

void PresupuestoListSubForm::cargar(QString query) {
        _depura("PresupuestoListSubForm::cargar", 0, query);
        SubForm3::cargar(query);
    _depura("PresupuestoListSubForm::cargar", 0);
}
