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

#include "facturaslist.h"
#include "company.h"
#include "qtable1.h"
#include "busquedacliente.h"
#include "busquedaarticulo.h"
#include "busquedafecha.h"
#include "configuracion.h"
#include "facturaview.h"
#include "funcaux.h"
#include "plugins.h"


FacturasList::FacturasList(QWidget *parent, const char *name, Qt::WFlags flag, edmode editmodo)
        : QWidget(parent, name, flag) {
    setupUi(this);
    m_companyact = NULL;
    m_modo = editmodo;
    mdb_idfactura = "";
    if (m_modo == EditMode)
        meteWindow(windowTitle(), this);
    hideBusqueda();
}


FacturasList::FacturasList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag, edmode editmodo)
        : QWidget(parent, name, flag) {
    setupUi(this);
    m_companyact = comp;
    m_cliente->setcompany(m_companyact);
    m_articulo->setcompany(m_companyact);
    mui_list->setcompany(m_companyact);
    presenta();
    m_modo = editmodo;
    mdb_idfactura = "";
    if (m_modo == EditMode)
        meteWindow(windowTitle(), this);
    hideBusqueda();
}


FacturasList::~FacturasList() {
    if (m_modo == EditMode)
        m_companyact->sacaWindow(this);
}


void FacturasList::presenta() {
    _depura("FacturasList::presenta()", 1);

    cursor2 * cur = m_companyact->cargacursor("SELECT *, totalfactura AS total, bimpfactura AS base, impfactura AS impuestos FROM factura LEFT JOIN cliente ON factura.idcliente=cliente.idcliente LEFT JOIN  almacen ON  factura.idalmacen=almacen.idalmacen WHERE 1=1  " + generaFiltro());
    mui_list->cargar(cur);
    delete cur;

    /// Hacemos el calculo del total.
    cur = m_companyact->cargacursor("SELECT SUM(totalfactura) AS total FROM factura LEFT JOIN cliente ON factura.idcliente=cliente.idcliente LEFT JOIN  almacen ON  factura.idalmacen=almacen.idalmacen WHERE 1=1  " + generaFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;

    _depura("END FacturasList::presenta()", 1);
}


QString FacturasList::generaFiltro() {
    /// Tratamiento de los filtros.
    fprintf(stderr,"Tratamos el filtro \n");
    QString filtro = "";
    if (m_filtro->text() != "") {
        filtro = " AND ( descfactura LIKE '%" + m_filtro->text() + "%' ";
	filtro +=" OR reffactura LIKE '"+m_filtro->text()+"%' ";
        filtro +=" OR nomcliente LIKE '%" + m_filtro->text() + "%') ";
    } else {
        filtro = "";
    } // end if
    if (m_cliente->idcliente() != "") {
        filtro += " AND factura.idcliente=" + m_cliente->idcliente();
    } // end if
    if (!m_procesada->isChecked() ) {
        filtro += " AND NOT procesadafactura";
    } // end if
    if (m_articulo->idarticulo() != "") {
        filtro += " AND idfactura IN (SELECT DISTINCT idfactura FROM lfactura WHERE idarticulo='" + m_articulo->idarticulo() + "')";
    } // end if
    if (m_fechain->text() != "") {
        filtro += " AND ffactura >= '" + m_fechain->text() + "' ";
    } // end if
    if (m_fechafin->text() != "") {
        filtro += " AND ffactura <= '" + m_fechafin->text() + "' ";
    } // end if
    return (filtro);
}


void FacturasList::editar(int row) {
    _depura("FacturasList::editar", 0);
    mdb_idfactura = mui_list->DBvalue(QString("idfactura"), row);
    if (m_modo ==0 ) {
        FacturaView *prov = m_companyact->newFacturaView();
        if (prov->cargar(mdb_idfactura)) {
	    delete prov;
            return;
        }
        m_companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(mdb_idfactura));
    }// end if
    _depura("END FacturasList::editar", 0);
}


void FacturasList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >= 0)
        editar(a);
    else
        _depura("Debe seleccionar una linea", 2);
}


void FacturasList::on_mui_imprimir_clicked() {
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "facturas.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "facturas.rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system (archivo.toAscii().constData());

    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#endif

    system (archivologo.toAscii().constData());

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

    buff.replace("[story]", fitxersortidatxt);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    } // end if

    invocaPDF("facturas");
}


void FacturasList::on_mui_borrar_clicked() {
    _depura("FacturasList::on_mui_borrar_clicked", 0);
    try {
	mdb_idfactura = mui_list->DBvalue(QString("idfactura"));
	if (m_modo == 0) {
		FacturaView *fac = m_companyact->newFacturaView();
		if (fac->cargar(mdb_idfactura)) throw -1;
		fac->borrar();
	} // end if
	presenta();
    } catch(...) {
	mensajeInfo(tr("Error al borrar la factura cliente"));
    } // end try
    _depura("END FacturasList::on_mui_borrar_clicked", 0);
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
FacturasListSubform::FacturasListSubform(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("factura");
    setDBCampoId("idfactura");
    addSHeader("reffactura", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("Ref factura"));
    addSHeader("idfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id factura"));
    addSHeader("codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo almacen"));
    addSHeader("numfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero factura"));
    addSHeader("cifcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("CIF cliente"));
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre cliente"));
    addSHeader("ffactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Ffactura"));
    addSHeader("contactfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Contact factura"));
    addSHeader("telfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Telefono factura"));
    addSHeader("comentfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentario factura"));
    addSHeader("idtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id trabajador"));
    addSHeader("idcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id cliente"));
    addSHeader("idalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id almacen"));
    addSHeader("codigoserie_factura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo serie_factura"));
    addSHeader("total", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Total"));
    addSHeader("base", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Base imponible"));
    addSHeader("impuestos", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Impuestos"));
    setinsercion(FALSE);
}

