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


PagosList::PagosList(QWidget *parent, const char *name, Qt::WFlags flag)
        : QWidget(parent, name, flag) {
    m_companyact = NULL;
    setupUi(this);
    m_modo=0;
    mdb_idpago="";
    meteWindow(caption(),this);
    hideBusqueda();
}

PagosList::PagosList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag)
        : QWidget(parent, name, flag) {
    m_companyact = comp;
    setupUi(this);
    m_proveedor->setcompany(comp);
    mui_list->setcompany(comp);
    presentar();
    m_modo=0;
    mdb_idpago="";
    meteWindow(caption(),this);
    hideBusqueda();
}



PagosList::~PagosList() {
    m_companyact->sacaWindow(this);
}




void PagosList::presentar() {
    _depura("PagosList::presentar()\n",0);
    if (m_companyact != NULL ) {
        cursor2 * cur= m_companyact->cargacursor("SELECT * FROM pago NATURAL LEFT JOIN proveedor NATURAL LEFT JOIN trabajador WHERE 1=1"+generaFiltro());
        mui_list->cargar(cur);
	delete cur;

        /// Hacemos el calculo del total.
        cur = m_companyact->cargacursor("SELECT SUM(cantpago) AS total FROM pago WHERE 1=1"+generaFiltro());
        m_total->setText(cur->valor("total"));
        delete cur;
    }// end if
    _depura("END PagosList::presentar()\n",0);
}



QString PagosList::generaFiltro() {
    _depura("PagosList::generaFiltro",0);
    QString filtro="";
    if (m_filtro->text() != "") {
        filtro = " AND ( descpago LIKE '%"+m_filtro->text()+"%' ";
    } else {
        filtro = "";
    }// end if
    if (m_proveedor->idproveedor() != "") {
        filtro += " AND pago.idproveedor="+m_proveedor->idproveedor();
    }// end if
    if (!m_procesados->isChecked() ) {
        filtro += " AND NOT previsionpago";
    }// end if
    if (m_fechain->text() != "")
        filtro += " AND fechapago >= '"+m_fechain->text()+"' ";
    if (m_fechafin->text() != "")
        filtro += " AND fechapago <= '"+m_fechafin->text()+"' ";
    _depura("END PagosList::generaFiltro",0);
    return (filtro);
}



void PagosList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >=0 )
        on_mui_list_cellDoubleClicked(a,0);
    else
        _depura("Debe seleccionar una linea",2);
}


void PagosList::on_mui_list_cellDoubleClicked(int , int ) {
    mdb_idpago = mui_list->DBvalue("idpago");
    if (m_modo ==0 && mdb_idpago != "") {
        PagoView *bud = new PagoView(m_companyact,NULL,theApp->translate("Edicion de pagos", "company"));
        bud->cargar(mdb_idpago);
        bud->show();
    } else {
        close();
    }// end if
}

void PagosList::on_mui_list_customContextMenuRequested(const QPoint &) {
    _depura("PagosList::on_mui_list_customContextMenuRequested",0);
    int a = mui_list->currentRow();
    if ( a < 0)
        return;
    QMenu *popup = new QMenu(this);
    QAction *edit = popup->addAction(tr("Editar Pago"));
    QAction *del = popup->addAction(tr("Borrar Pago"));
    QAction *opcion = popup->exec(QCursor::pos());
    if (opcion == del)
        on_mui_borrar_clicked();
    if (opcion == edit)
	on_mui_editar_clicked();
    delete popup;
}




void PagosList::on_mui_crear_clicked() {
    fprintf(stderr,"Iniciamos el boton_crear\n");
    PagoView *bud = new PagoView(m_companyact,NULL,theApp->translate("Edicion de pagos", "company"));
    bud->show();
    bud->setidproveedor(m_proveedor->idproveedor());
    bud->pintar();
}


void PagosList::imprimir() {
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"pagos.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"pagos.rml";
    QString archivologo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";

    /// Copiamos el archivo
#ifdef WINDOWS
    archivo = "copy "+archivo+" "+archivod;
#else
    archivo = "cp "+archivo+" "+archivod;
#endif

    system (archivo.ascii());

    /// Copiamos el logo

#ifdef WINDOWS
    archivologo = "copy "+archivologo+" "+confpr->valor(CONF_DIR_USER)+"logo.jpg";
#else
    archivologo = "cp "+archivologo+" "+confpr->valor(CONF_DIR_USER)+"logo.jpg";
#endif

    system (archivologo.ascii());

    QFile file;
    file.setName( archivod );
    file.open( QIODevice::ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;
    // Linea de totales del presupuesto
    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += mui_list->imprimir();
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]",fitxersortidatxt);

    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }

    // Crea el pdf  y lo muestra.
    _depura("Vamos a imprimir e listado de pagos",0);
    invocaPDF("pagos");
}


void PagosList::on_mui_borrar_clicked() {
    mdb_idpago = mui_list->DBvalue("idpago");
    if (m_modo ==0 && mdb_idpago != "") {
        PagoView *bud = new PagoView(m_companyact,NULL,theApp->translate("Edicion de presupuestos", "company"));
        bud->cargar(mdb_idpago);
        bud->borraPago();
    }// end if
    presentar();
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================


PagosListSubForm::PagosListSubForm(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("pago");
    setDBCampoId("idpago");
    addSHeader("idpago", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, "idpago");
    addSHeader("idproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idproveedor");
    addSHeader("nomproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomproveedor");
    addSHeader("cifproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "cifproveedor");
    addSHeader("telproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "telproveedor");
    addSHeader("emailproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "emailproveedor");
    addSHeader("fechapago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "fechapago");
    addSHeader("cantpago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "cantpago");
    addSHeader("refpago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "refpago");
    addSHeader("previsionpago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "previsionpago");
    addSHeader("comentpago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "comentpago");
    addSHeader("idtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idtrabajador");
    addSHeader("nomtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomtrabajador");
    addSHeader("apellidostrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "apellidostrabajador");


    setinsercion(FALSE);
}




