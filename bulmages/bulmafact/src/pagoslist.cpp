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
#include <QMenu>
#include <QTextStream>

#include "pagoslist.h"
#include "company.h"
#include "pagoview.h"
#include "qtable2.h"
#include "funcaux.h"
#include "configuracion.h"


PagosList::PagosList(QWidget *parent, Qt::WFlags flag)
        : QWidget(parent, flag) {
    m_companyact = NULL;
    setupUi(this);
    m_modo = 0;
    mdb_idpago = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
}


PagosList::PagosList(company *comp, QWidget *parent, Qt::WFlags flag)
        : QWidget(parent, flag) {
    m_companyact = comp;
    setupUi(this);
    m_proveedor->setcompany(comp);
    mui_list->setcompany(comp);
    presentar();
    m_modo = 0;
    mdb_idpago = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
}


PagosList::~PagosList() {
    m_companyact->sacaWindow(this);
}


void PagosList::presentar() {
    _depura("PagosList::presentar()\n", 0);
    if (m_companyact != NULL ) {
        mui_list->cargar("SELECT * FROM pago NATURAL LEFT JOIN proveedor NATURAL LEFT JOIN trabajador WHERE 1 = 1 " + generaFiltro());

        /// Hacemos el calculo del total.
        cursor2 *cur = m_companyact->cargacursor("SELECT SUM(cantpago) AS total FROM pago WHERE 1 = 1 " + generaFiltro());
        m_total->setText(cur->valor("total"));
        delete cur;
    } // end if
    _depura("END PagosList::presentar()\n", 0);
}


QString PagosList::generaFiltro() {
    _depura("PagosList::generaFiltro", 0);
    QString filtro = "";
    if (m_filtro->text() != "") {
        filtro = " AND ( descpago LIKE '%" + m_filtro->text() + "%' ";
    } else {
        filtro = "";
    } // end if
    if (m_proveedor->idproveedor() != "") {
        filtro += " AND pago.idproveedor = " + m_proveedor->idproveedor();
    } // end if

    QString subfiltro = " AND ";
    if (mui_efectivos->isChecked() ) {
        filtro += " AND NOT previsionpago";
	subfiltro = " OR ";
    } // end if

    if (mui_previsiones->isChecked() ) {
        filtro += subfiltro + " previsionpago";
    } // end if



    if (m_fechain->text() != "")
        filtro += " AND fechapago >= '" + m_fechain->text() + "' ";
    if (m_fechafin->text() != "")
        filtro += " AND fechapago <= '" + m_fechafin->text() + "' ";
    _depura("END PagosList::generaFiltro", 0);
    return (filtro);
}


void PagosList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >= 0)
        on_mui_list_cellDoubleClicked(a, 0);
    else
        _depura("Debe seleccionar una linea", 2);
}


void PagosList::on_mui_list_cellDoubleClicked(int, int) {
    mdb_idpago = mui_list->DBvalue("idpago");
    if (m_modo == 0 && mdb_idpago != "") {
        PagoView *bud = m_companyact->newPagoView();
        if (bud->cargar(mdb_idpago)) {
		delete bud;
		return;
	} // end if
	m_companyact->m_pWorkspace->addWindow(bud);
        bud->show();
    } else {
        close();
    } // end if
}


void PagosList::on_mui_list_customContextMenuRequested(const QPoint &) {
    _depura("PagosList::on_mui_list_customContextMenuRequested", 0);
    int a = mui_list->currentRow();
    if (a < 0)
        return;
    QMenu *popup = new QMenu(this);
    QAction *edit = popup->addAction(tr("Editar pago"));
    QAction *del = popup->addAction(tr("Borrar pago"));
    QAction *opcion = popup->exec(QCursor::pos());
    if (opcion == del)
        on_mui_borrar_clicked();
    if (opcion == edit)
        on_mui_editar_clicked();
    delete popup;
}


void PagosList::on_mui_crear_clicked() {
    fprintf(stderr, "Iniciamos el boton_crear\n");
    PagoView *bud = m_companyact->newPagoView();
    m_companyact->m_pWorkspace->addWindow(bud);
    bud->show();
    bud->setidproveedor(m_proveedor->idproveedor());
    bud->pintar();
}


void PagosList::imprimir() {
    _depura("PagosList::imprimir", 0);
     mui_list->imprimirPDF(tr("Listado de Pagos"));
    _depura("END PagosList::imprimir", 0);
}


void PagosList::on_mui_borrar_clicked() {
    _depura("PagosList::on_mui_borrar_clicked", 0);
    try {
	mdb_idpago = mui_list->DBvalue("idpago");
	if (m_modo == 0 && mdb_idpago != "") {
		PagoView *bud = new PagoView(m_companyact, NULL);
		bud->cargar(mdb_idpago);
		bud->borraPago();
	} // end if
	presentar();
    } catch (...)  {
	mensajeInfo(tr("Error al borrar el pago"));
    } // end try
    _depura("END PagosList::on_mui_borrar_clicked", 0);
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
PagosListSubForm::PagosListSubForm(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("pago");
    setDBCampoId("idpago");
    addSHeader("idpago", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("Id pago"));
    addSHeader("idproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id proveedor"));
    addSHeader("nomproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre proveedor"));
    addSHeader("cifproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("C.I.F. proveedor"));
    addSHeader("telproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Telefono proveedor"));
    addSHeader("emailproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Email proveedor"));
    addSHeader("fechapago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha de pago"));
    addSHeader("cantpago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Cantidad"));
    addSHeader("refpago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Referencia de pago"));
    addSHeader("previsionpago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Prevision pago"));
    addSHeader("comentpago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentario pago"));
    addSHeader("idtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id trabajador"));
    addSHeader("nomtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre de trabajador"));
    addSHeader("apellidostrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Apellidos trabajador"));
    setinsercion(FALSE);
}

