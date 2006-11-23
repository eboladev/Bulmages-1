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
#include <QMenu>

#include "configuracion.h"
#include "cobroslist.h"
#include "company.h"
#include "cobroview.h"
#include "funcaux.h"


CobrosList::CobrosList(QWidget *parent, Qt::WFlags flag)
        : Ficha(parent, flag) {
    setupUi(this);
    _depura("CobrosList::CobrosList", 0);
    m_companyact = NULL;
    m_modo = 0;
    mdb_idcobro = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
    _depura("END CobrosList::CobrosList", 0);

}


CobrosList::CobrosList(company *comp, QWidget *parent, Qt::WFlags flag)
        : Ficha(parent, flag) {
    setupUi(this);
    _depura("CobrosList::CobrosList",0);
    m_companyact = comp;
    m_cliente->setcompany(comp);
    mui_list->setcompany(comp);
    presentar();
    m_modo = 0;
    mdb_idcobro = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
    _depura("END CobrosList::CobrosList", 0);
}


CobrosList::~CobrosList() {}


void CobrosList::presentar() {
    _depura("CobrosList::presentar()\n", 0);
    if (m_companyact != NULL) {
        mui_list->cargar("SELECT * FROM cobro NATURAL LEFT JOIN cliente NATURAL LEFT JOIN trabajador WHERE 1=1 " + generaFiltro());

        /// Hacemos el calculo del total.
        cursor2 *cur = m_companyact->cargacursor("SELECT SUM(cantcobro) AS total FROM cobro WHERE 1=1 " + generaFiltro());
        m_total->setText(cur->valor("total"));
        delete cur;
    } // end if
    _depura("END CobrosList::presentar()\n", 0);
}


QString CobrosList::generaFiltro() {
    _depura("CobrosList::generaFiltro\n", 0);
    QString filtro = "";
    if (m_filtro->text() != "") {
        filtro = " AND ( desccobro LIKE '%" + m_filtro->text() + "%' ";
    } else {
        filtro = "";
    } // end if
    if (m_cliente->idcliente() != "") {
        filtro += " AND cobro.idcliente=" + m_cliente->idcliente();
    } // end if

    QString subfiltro = " AND ";
    if (mui_efectivos->isChecked() ) {
        filtro += " AND NOT previsioncobro";
        subfiltro = " OR ";
    } // end if

    if (mui_previsiones->isChecked() ) {
        filtro += subfiltro + " previsioncobro";
    } // end if

    if (m_fechain->text() != "")
        filtro += " AND fechacobro >= '" + m_fechain->text() + "' ";

    if (m_fechafin->text() != "")
        filtro += " AND fechacobro <= '" + m_fechafin->text() + "' ";

    _depura("END CobrosList::generaFiltro\n", 0);
    return (filtro);
}


void CobrosList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >= 0)
        on_mui_list_cellDoubleClicked(a, 0);
    else
        _depura("Debe seleccionar una linea", 2);
}


void CobrosList::on_mui_crear_clicked() {
    _depura("CobrosList::on_mui_crear_clicked", 0);
    CobroView *bud = m_companyact->newCobroView();
    m_companyact->m_pWorkspace->addWindow(bud);
    bud->show();
    bud->setidcliente(m_cliente->idcliente());
    bud->pintar();
}


void CobrosList::imprimir() {
    _depura("CobrosList::imprimir", 0);
    mui_list->imprimirPDF(tr("Listado de Cobros"));
    _depura("END CobrosList::imprimir", 0);
}


void CobrosList::on_mui_borrar_clicked() {
    _depura("CobrosList::on_mui_borrar_clicked",0);
    try {
        mdb_idcobro = mui_list->DBvalue("idcobro");
        if (m_modo == 0) {
            CobroView *bud = m_companyact->newCobroView();
            bud->cargar(mdb_idcobro);
            bud->borrar();
            delete bud;
        } // end if
        presentar();
    } catch(...) {
        mensajeInfo(tr("Error al borrar el cobro"));
    } // end try
    _depura("END:CobrosList::on_mui_borrar_clicked",0);
}


void CobrosList::on_mui_list_cellDoubleClicked(int, int) {
    _depura("CobrosList::on_mui_list_cellDoubleClicked", 0);
    try {
        mdb_idcobro = mui_list->DBvalue("idcobro");
        if (m_modo == 0) {
            CobroView *bud = m_companyact->newCobroView();
            if (bud->cargar(mdb_idcobro)) {
                delete bud;
                return;
            } // end if
            m_companyact->m_pWorkspace->addWindow(bud);
            bud->show();
        } else {
            close();
        } // end if
    } catch(...) {
        mensajeInfo(tr("Debe seleccionar una fila primero"));
    } // end try
    _depura("END CobrosList::on_mui_list_cellDoubleClicked", 0);

}


void CobrosList::on_mui_list_customContextMenuRequested(const QPoint &) {
    _depura("PagosList::on_mui_list_customContextMenuRequested", 0);
    int a = mui_list->currentRow();
    if ( a < 0)
        return;
    QMenu *popup = new QMenu(this);
    QAction *edit = popup->addAction(tr("Editar cobro"));
    QAction *del = popup->addAction(tr("Borrar cobro"));
    QAction *opcion = popup->exec(QCursor::pos());
    if (opcion == del)
        on_mui_borrar_clicked();
    if (opcion == edit)
        on_mui_editar_clicked();
    delete popup;
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
CobrosListSubForm::CobrosListSubForm(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("cobro");
    setDBCampoId("idcobro");
    addSHeader("idcobro", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID cobro"));
    addSHeader("idcliente", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID cliente"));
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre"));
    addSHeader("cifcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("C.I.F."));
    addSHeader("telcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de telefono"));
    addSHeader("mailcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Direccion de correo electronico"));
    addSHeader("fechacobro", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha de cobro"));
    addSHeader("cantcobro", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Cantidad"));
    addSHeader("refcobro", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Referencia del cobro"));
    addSHeader("previsioncobro", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Previcion del cobro"));
    addSHeader("comentcobro", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentarios"));
    addSHeader("idtrabajador", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID trabajador"));
    addSHeader("nomtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre del trabajador"));
    addSHeader("apellidostrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Apellidos del trabajador"));
    setinsercion(FALSE);
}

