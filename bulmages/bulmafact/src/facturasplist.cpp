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

#include "facturasplist.h"
#include "company.h"
#include "qtable1.h"
#include "busquedaproveedor.h"
#include "busquedaarticulo.h"
#include "configuracion.h"
#include "facturapview.h"


FacturasProveedorList::FacturasProveedorList(QWidget *parent, Qt::WFlags flag)
        : QWidget(parent, flag) {
    setupUi(this);
    m_companyact = NULL;
    m_modo = 0;
    mdb_idfacturap = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
}


FacturasProveedorList::FacturasProveedorList(company *comp, QWidget *parent)
        : QWidget(parent) {
    setupUi(this);
    m_companyact = comp;
    m_proveedor->setcompany(m_companyact);
    m_articulo->setcompany(m_companyact);
    mui_list->setcompany(comp);
    presenta();
    m_modo = 0;
    mdb_idfacturap = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
}


FacturasProveedorList::~FacturasProveedorList() {
    _depura("FacturasProveedorList::~FacturasProveedorList", 0);
    m_companyact->sacaWindow(this);
}


void FacturasProveedorList::presenta() {
    _depura("FacturasProveedorList::presenta", 0);
    cursor2 * cur= m_companyact->cargacursor("SELECT *, totalfacturap AS total, bimpfacturap AS base, impfacturap AS impuestos  FROM facturap LEFT JOIN proveedor ON facturap.idproveedor=proveedor.idproveedor WHERE 1=1  " + generaFiltro());

    mui_list->cargar(cur);
    delete cur;

    /// Hacemos el calculo del total.
    cur = m_companyact->cargacursor("SELECT SUM(totalfacturap) AS total FROM facturap LEFT JOIN proveedor ON facturap.idproveedor=proveedor.idproveedor WHERE 1=1  " + generaFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;
    _depura("END FacturasProveedorList::presenta", 0);
}


QString FacturasProveedorList::generaFiltro() {
    /// Tratamiento de los filtros.
    _depura("Tratamos el filtro \n", 0);
    QString filtro = "";
    if (m_filtro->text() != "") {
        filtro = " AND ( descfacturap LIKE '%" + m_filtro->text() + "%' ";
        filtro +=" OR nomproveedor LIKE '%" + m_filtro->text() + "%') ";
    } else {
        filtro = "";
    } // end if
    if (m_proveedor->idproveedor() != "") {
        filtro += " AND facturap.idproveedor=" + m_proveedor->idproveedor();
    } // end if
    if (!m_procesada->isChecked() ) {
        filtro += " AND NOT procesadafacturap";
    } // end if
    if (m_articulo->idarticulo() != "") {
        filtro += " AND idfacturap IN (SELECT DISTINCT idfacturap FROM lfacturap WHERE idarticulo='" + m_articulo->idarticulo() + "')";
    } // end if

    if (m_fechain->text() != "")
        filtro += " AND ffacturap >= '" + m_fechain->text() + "' ";

    if (m_fechafin->text() != "")
        filtro += " AND ffacturap <= '" + m_fechafin->text() + "' ";

    return (filtro);
}


void FacturasProveedorList::editar(int row) {
    _depura("FacturasProveedorList::editar", 0);
    try {
	mdb_idfacturap = mui_list->DBvalue(QString("idfacturap"), row);
	if (m_modo == 0) {
		FacturaProveedorView *prov = m_companyact->newFacturaProveedorView();
		if (prov->cargar(mdb_idfacturap)) {
			delete prov;
			return;
		} // end if
		m_companyact->m_pWorkspace->addWindow(prov);
		prov->show();
	} else {
		emit(selected(mdb_idfacturap));
	} // end if
    } catch(...) {
	mensajeInfo(tr("Error al cargar la factura proveedor"));
    } // end try
    _depura("END FacturasProveedorList::editar", 0);
}


void FacturasProveedorList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >= 0)
        editar(a);
    else
        _depura("Debe seleccionar una linea", 2);
}


void FacturasProveedorList::on_mui_borrar_clicked() {
    _depura("FacturasProveedorList::on_mui_borrar_clicked", 0);
    try {
	mdb_idfacturap = mui_list->DBvalue("idfacturap");
	FacturaProveedorView *bud = m_companyact->newFacturaProveedorView();
	bud->cargar(mdb_idfacturap);
	bud->on_mui_borrar_clicked();
	delete bud;
	presenta();
    } catch(...) {
	mensajeInfo(tr("Error al borrar factura de proveedor"));
    }
    _depura("END FacturasProveedorList::on_mui_borrar_clicked", 0);
}


void FacturasProveedorList::on_mui_imprimir_clicked() {
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "facturasproveedor.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "facturasproveedor.rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system(archivo.toAscii().constData());

    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#endif

    system(archivologo.toAscii().constData());

    QFile file;
    file.setFileName(archivod);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt;
    /// Linea de totales del presupuesto.
    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += mui_list->imprimir();
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]", fitxersortidatxt);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    }
    invocaPDF("facturasproveedor");
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
FacturasProveedorListSubform::FacturasProveedorListSubform(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("facturap");
    setDBCampoId("idfacturap");
    addSHeader("reffacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Referencia factura"));
    addSHeader("idfacturap", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("Id factura"));
    addSHeader("numfacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero factura"));
    addSHeader("nomproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre proveedor"));
    addSHeader("ffacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha factura"));
    addSHeader("contactfacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Persona de contacto"));
    addSHeader("telfacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Telefono factura"));
    addSHeader("comentfacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentario factura"));
    addSHeader("idtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id trabajador"));
    addSHeader("idproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id proveedor"));
    addSHeader("total", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Total"));
    addSHeader("base", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Base imponible"));
    addSHeader("impuestos", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Impuestos"));
    setinsercion(FALSE);
}

