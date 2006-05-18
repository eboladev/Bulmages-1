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

#include <stdio.h>
#include <QMessageBox>
#include <Q3PopupMenu>
#include <QFile>
#include <QTextStream>

#include "pedidosproveedorlist.h"
#include "funcaux.h"
#include "company.h"
#include "qtable1.h"
#include "configuracion.h"
#include "pedidoproveedorview.h"


PedidosProveedorList::PedidosProveedorList(QWidget *parent, const char *name, Qt::WFlags flag)
        : QWidget(parent, name, flag) {
    setupUi(this);
    m_companyact = NULL;
    m_modo=0;
    mdb_idpedidoproveedor="";
    meteWindow(caption(),this);
    hideBusqueda();
}


PedidosProveedorList::PedidosProveedorList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag)
        : QWidget(parent, name, flag) {
    setupUi(this);
    m_companyact = comp;
    m_proveedor->setcompany(comp);
    m_articulo->setcompany(comp);
    mui_list->setcompany(comp);
    presenta();
    m_modo=0;
    mdb_idpedidoproveedor="";
    meteWindow(caption(),this);
    hideBusqueda();
}


PedidosProveedorList::~PedidosProveedorList() {}


void PedidosProveedorList::presenta() {
    cursor2 * cur= m_companyact->cargacursor("SELECT *, calctotalpedpro(idpedidoproveedor) AS total, calcbimppedpro(idpedidoproveedor) AS base, calcimpuestospedpro(idpedidoproveedor) AS impuestos FROM pedidoproveedor LEFT JOIN  proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1  "+generarFiltro());
    mui_list->cargar(cur);
    delete cur;
    /// Hacemos el calculo del total.
    cur = m_companyact->cargacursor("SELECT SUM(calctotalpedpro(idpedidoproveedor)) AS total FROM pedidoproveedor LEFT JOIN  proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1  "+generarFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;
}




QString PedidosProveedorList::generarFiltro() {

    /// Tratamiento de los filtros.
    QString filtro="";
    if (m_filtro->text() != "") {
        filtro = " AND ( descpedidoproveedor LIKE '%"+m_filtro->text()+"%' ";
        filtro +=" OR nomproveedor LIKE '%"+m_filtro->text()+"%') ";
    } else {
        filtro = "";
    }// end if
    if (m_proveedor->idproveedor() != "") {
        filtro += " AND pedidoproveedor.idproveedor="+m_proveedor->idproveedor();
    }// end if
    if (!m_procesados->isChecked() ) {
        filtro += " AND NOT procesadopedidoproveedor";
    }// end if
    if (m_articulo->idarticulo() != "") {
        filtro += " AND idpedidoproveedor IN (SELECT DISTINCT idpedidoproveedor FROM lpedidoproveedor WHERE idarticulo='"+m_articulo->idarticulo()+"')";
    }// end if

    if (m_fechain->text() != "")
        filtro += " AND fechapedidoproveedor >= '"+m_fechain->text()+"' ";

    if (m_fechafin->text() != "")
        filtro += " AND fechapedidoproveedor <= '"+m_fechafin->text()+"' ";

    //    filtro += " ORDER BY "+orden[m_orden->currentItem()];
    return (filtro);
}


void PedidosProveedorList::imprimir() {
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"pedidosproveedor.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"pedidosproveedor.rml";
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
    invocaPDF("pedidosproveedor");

}



void PedidosProveedorList::on_mui_borrar_clicked() {
    _depura("PedidosProveedorList::on_mui_borrar_clicked",0);
    mdb_idpedidoproveedor = mui_list->DBvalue(QString("idpedidoproveedor"));
    PedidoProveedorView *prov = new PedidoProveedorView(m_companyact,0,theApp->translate("Edicion de Facturas a Cliente", "company"));
    if (prov->cargar(mdb_idpedidoproveedor)) {
        return;
    }
    prov->borrar();
    presenta();
}


void PedidosProveedorList::editar(int  row) {
    _depura("PedidosProveedorList::editar",0);
    mdb_idpedidoproveedor = mui_list->DBvalue(QString("idpedidoproveedor"),row);
    if (m_modo ==0 ) {
        PedidoProveedorView *prov = new PedidoProveedorView(m_companyact,0,theApp->translate("Edicion de Facturas a Cliente", "company"));
        if (prov->cargar(mdb_idpedidoproveedor)) {
            return;
        }
        m_companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(mdb_idpedidoproveedor));
        // close();
    }// end if
    _depura("END PedidosProveedorList::editar",0);
}

void PedidosProveedorList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >=0 )
        editar(a);
    else
        _depura("Debe seleccionar una linea",2);
}




/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================


PedidosProveedorListSubform::PedidosProveedorListSubform(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("pedidoproveedor");
    setDBCampoId("idpedidoproveedor");
    addSHeader("idpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, "reffactura");
    addSHeader("codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idfactura");
    addSHeader("refpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "codigoalmacen");
    addSHeader("numpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "numfactura");
    addSHeader("descpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomcliente");
    addSHeader("nomproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "ffactura");
    addSHeader("fechapedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "contactfactura");
    addSHeader("contactpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "telfactura");
    addSHeader("telpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "comentfactura");
    addSHeader("comentpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "comentpedidoproveedor");
    addSHeader("idtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idcliente");
    addSHeader("idproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idalmacen");
    addSHeader("idalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "codigoserie_factura");
    addSHeader("total", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "total");
    addSHeader("base", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "base");
    addSHeader("impuestos", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "impuestos");
    setinsercion(FALSE);
};





