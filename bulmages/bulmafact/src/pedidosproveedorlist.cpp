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

#define COL_IDPEDIDOPROVEEDOR 0
#define COL_CODIGOALMACEN 1
#define COL_REFPEDIDOPROVEEDOR 2
#define COL_NUMPEDIDOPROVEEDOR 3
#define COL_DESCPEDIDOPROVEEDOR 4
#define COL_NOMPROVEEDOR 5
#define COL_FECHAPEDIDOPROVEEDOR 6
#define COL_CONTACPEDIDOPROVEEDOR 7
#define COL_TELPEDIDOPROVEEDOR 8
#define COL_COMENTPEDIDOPROVEEDOR 9
#define COL_IDUSUARI 10
#define COL_IDPROVEEDOR 11
#define COL_IDALMACEN 12
#define COL_TOTALPEDIDOPROVEEDOR 13
#define COL_TOTALBASEIMP 14
#define COL_TOTALIMPUESTOS 15

void PedidosProveedorList::guardaconfig() {
    QString aux = "";
    mver_idpedidoproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_codigoalmacen->isChecked() ? aux += "1,":aux+="0,";
    mver_refpedidoproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_numpedidoproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_descpedidoproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_nomproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_fechapedidoproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_contacpedidoproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_telpedidoproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_comentpedidoproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_idusuari->isChecked() ? aux += "1,":aux+="0,";
    mver_idproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_idalmacen->isChecked() ? aux += "1,":aux+="0,";
    mver_totalpedidoproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_totalbaseimp->isChecked() ? aux += "1,":aux+="0,";
    mver_totalimpuestos->isChecked() ? aux += "1,":aux+="0,";

    QFile file( confpr->valor(CONF_DIR_USER)+"confpedidosproveedorlist.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << aux << "\n";
        file.close();
    }// end if
}// end guardaconfig()

void PedidosProveedorList::cargaconfig() {
    QFile file( confpr->valor(CONF_DIR_USER)+"confpedidosproveedorlist.cfn" );
    QString line;
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        line = stream.readLine(); // line of text excluding '\n'
        file.close();
    } else
        return;

    mver_idpedidoproveedor->setChecked(line.at(0)=='1');
    mver_codigoalmacen->setChecked(line.at(2)=='1');
    mver_refpedidoproveedor->setChecked(line.at(4)=='1');
    mver_numpedidoproveedor->setChecked(line.at(6)=='1');
    mver_descpedidoproveedor->setChecked(line.at(8)=='1');
    mver_nomproveedor->setChecked(line.at(10)=='1');
    mver_fechapedidoproveedor->setChecked(line.at(12)=='1');
    mver_contacpedidoproveedor->setChecked(line.at(14)=='1');
    mver_telpedidoproveedor->setChecked(line.at(16)=='1');
    mver_comentpedidoproveedor->setChecked(line.at(18)=='1');
    mver_idusuari->setChecked(line.at(20)=='1');
    mver_idproveedor->setChecked(line.at(22)=='1');
    mver_idalmacen->setChecked(line.at(24)=='1');
    mver_totalpedidoproveedor->setChecked(line.at(26)=='1');
    mver_totalbaseimp->setChecked(line.at(28)=='1');
    mver_totalimpuestos->setChecked(line.at(30)=='1');
}// end cargaconfig


void PedidosProveedorList::s_configurar()
{
  if(mver_idpedidoproveedor->isChecked())
    mui_list->showColumn(COL_IDPEDIDOPROVEEDOR);
  else
    mui_list->hideColumn(COL_IDPEDIDOPROVEEDOR);

  if(mver_codigoalmacen->isChecked())
    mui_list->showColumn(COL_CODIGOALMACEN);
  else
    mui_list->hideColumn(COL_CODIGOALMACEN);
  
  if(mver_refpedidoproveedor->isChecked())
    mui_list->showColumn(COL_REFPEDIDOPROVEEDOR);
  else
    mui_list->hideColumn(COL_REFPEDIDOPROVEEDOR);

  if(mver_numpedidoproveedor->isChecked())
    mui_list->showColumn(COL_NUMPEDIDOPROVEEDOR);
  else
    mui_list->hideColumn(COL_NUMPEDIDOPROVEEDOR);

  if(mver_descpedidoproveedor->isChecked())
    mui_list->showColumn(COL_DESCPEDIDOPROVEEDOR);
  else
    mui_list->hideColumn(COL_DESCPEDIDOPROVEEDOR);

  if(mver_nomproveedor->isChecked())
    mui_list->showColumn(COL_NOMPROVEEDOR);
  else
    mui_list->hideColumn(COL_NOMPROVEEDOR);

  if(mver_fechapedidoproveedor->isChecked())
    mui_list->showColumn(COL_FECHAPEDIDOPROVEEDOR);
  else
    mui_list->hideColumn(COL_FECHAPEDIDOPROVEEDOR);

  if(mver_contacpedidoproveedor->isChecked())
    mui_list->showColumn(COL_CONTACPEDIDOPROVEEDOR);
  else
    mui_list->hideColumn(COL_CONTACPEDIDOPROVEEDOR);

  if(mver_telpedidoproveedor->isChecked())
    mui_list->showColumn(COL_TELPEDIDOPROVEEDOR);
  else
    mui_list->hideColumn(COL_TELPEDIDOPROVEEDOR);

  if(mver_comentpedidoproveedor->isChecked())
    mui_list->showColumn(COL_COMENTPEDIDOPROVEEDOR);
  else
    mui_list->hideColumn(COL_COMENTPEDIDOPROVEEDOR);

  if(mver_idusuari->isChecked())
    mui_list->showColumn(COL_IDUSUARI);
  else
    mui_list->hideColumn(COL_IDUSUARI);

  if(mver_idproveedor->isChecked())
    mui_list->showColumn(COL_IDPROVEEDOR);
  else
    mui_list->hideColumn(COL_IDPROVEEDOR);

  if(mver_idalmacen->isChecked())
    mui_list->showColumn(COL_IDALMACEN);
  else
    mui_list->hideColumn(COL_IDALMACEN);

  if(mver_totalpedidoproveedor->isChecked())
    mui_list->showColumn(COL_TOTALPEDIDOPROVEEDOR);
  else
    mui_list->hideColumn(COL_TOTALPEDIDOPROVEEDOR);

  if(mver_totalbaseimp->isChecked())
    mui_list->showColumn(COL_TOTALBASEIMP);
  else
    mui_list->hideColumn(COL_TOTALBASEIMP);

  if(mver_totalimpuestos->isChecked())
    mui_list->showColumn(COL_TOTALIMPUESTOS);
  else
    mui_list->hideColumn(COL_TOTALIMPUESTOS);

}// end s_configurar


PedidosProveedorList::PedidosProveedorList(QWidget *parent, const char *name, Qt::WFlags flag)
        : QWidget(parent, name, flag) {
    setupUi(this);
    m_companyact = NULL;
    m_modo=0;
    mdb_idpedidoproveedor="";
    meteWindow(caption(),this);
    hideBusqueda();
    cargaconfig();
    s_configurar();
}


PedidosProveedorList::PedidosProveedorList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag) 
  : QWidget(parent, name, flag) {
    setupUi(this);
    m_companyact = comp;
    m_proveedor->setcompany(comp);
    m_articulo->setcompany(comp);
    cargaconfig();
    s_configurar();
    presenta();
    m_modo=0;
    mdb_idpedidoproveedor="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}


PedidosProveedorList::~PedidosProveedorList() {
    guardaconfig();
}



void PedidosProveedorList::presenta() {

    cursor2 * cur= m_companyact->cargacursor("SELECT *, calctotalpedpro(idpedidoproveedor) AS total, calcbimppedpro(idpedidoproveedor) AS base, calcimpuestospedpro(idpedidoproveedor) AS impuestos FROM pedidoproveedor LEFT JOIN  proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1  "+generarFiltro());
	mui_list->cargar(cur);
	delete cur;



    /// Hacemos el calculo del total.
    cur = m_companyact->cargacursor("SELECT SUM(calctotalpedpro(idpedidoproveedor)) AS total FROM pedidoproveedor LEFT JOIN  proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1  "+generarFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;


	s_configurar();
}// end presenta




QString PedidosProveedorList::generarFiltro() {

    /// Tratamiento de los filtros.
    fprintf(stderr,"Tratamos el filtro \n");

    //    QString orden[] = {"fechapedidoproveedor","cifproveedor","nomproveedor","refpedidoproveedor","numpedidoproveedor"};

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
}// end generaFiltro


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
    fitxersortidatxt += "<tr>";
    /// ---------------------------------------------------------------------
    if(mver_idpedidoproveedor->isChecked() )
        fitxersortidatxt += "	<td>Id.</td>";
    if(mver_codigoalmacen->isChecked() )
        fitxersortidatxt += "	<td>Cod. Almacen</td>";
    if(mver_refpedidoproveedor->isChecked() )
        fitxersortidatxt += "	<td>Referencia</td>";
    if(mver_numpedidoproveedor->isChecked() )
        fitxersortidatxt += "	<td>Num. Pedido</td>";
    if(mver_descpedidoproveedor->isChecked() )
        fitxersortidatxt += "	<td>Descripicon</td>";
    if(mver_nomproveedor->isChecked() )
        fitxersortidatxt += "	<td>Proveedor</td>";
    if(mver_fechapedidoproveedor->isChecked() )
        fitxersortidatxt += "	<td>Fecha</td>";
    if(mver_contacpedidoproveedor->isChecked() )
        fitxersortidatxt += "	<td>Contacto</td>";
    if(mver_telpedidoproveedor->isChecked() )
        fitxersortidatxt += "	<td>Tel.</td>";
    if(mver_comentpedidoproveedor->isChecked() )
        fitxersortidatxt += "	<td>Coment</td>";
    if(mver_idusuari->isChecked() )
        fitxersortidatxt += "	<td>Id. Usuario</td>";
    if(mver_idproveedor->isChecked() )
        fitxersortidatxt += "	<td>Id. Proveedor</td>";
    if(mver_idalmacen->isChecked() )
        fitxersortidatxt += "	<td>Id. Almacen</td>";
    if(mver_totalpedidoproveedor->isChecked() )
        fitxersortidatxt += "	<td>Total</td>";
    if(mver_totalbaseimp->isChecked() )
        fitxersortidatxt += "	<td>Base Imp</td>";
    if(mver_totalimpuestos->isChecked() )
        fitxersortidatxt += "	<td>Impuestos</td>";
    /// ---------------------------------------------------------------------
    fitxersortidatxt += "</tr>";

    cursor2 * cur= m_companyact->cargacursor("SELECT * FROM pedidoproveedor LEFT JOIN  proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1  "+generarFiltro());
    while(!cur->eof()) {
        fitxersortidatxt += "<tr>";
        /// ----------------------------------------------------------
        if(mver_idpedidoproveedor->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("idpedidoproveedor")+"</td>";
        if(mver_codigoalmacen->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("codigoalmacen")+"</td>";
        if(mver_refpedidoproveedor->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("refpedidoproveedor")+"</td>";
        if(mver_numpedidoproveedor->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("numpedidoproveedor")+"</td>";
        if(mver_descpedidoproveedor->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("descpedidoproveedor")+"</td>";
        if(mver_nomproveedor->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("nomproveedor")+"</td>";
        if(mver_fechapedidoproveedor->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("fechapedidoproveedor")+"</td>";
        if(mver_contacpedidoproveedor->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("contacpedidoproveedor")+"</td>";
        if(mver_telpedidoproveedor->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("telpedidoproveedor")+"</td>";
        if(mver_comentpedidoproveedor->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("comentpedidoproveedor")+"</td>";
        if(mver_idusuari->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("idusuari")+"</td>";
        if(mver_idproveedor->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("idproveedor")+"</td>";
        if(mver_idalmacen->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("idalmacen")+"</td>";

        /// Calculamos el total del presupuesto y lo presentamos.
        cursor2 *cur1 = m_companyact->cargacursor("SELECT calctotalpedpro("+cur->valor("idpedidoproveedor")+") AS total, calcbimppedpro("+cur->valor("idpedidoproveedor")+") AS base, calcimpuestospedpro("+cur->valor("idpedidoproveedor")+") AS impuestos");
        if(mver_totalpedidoproveedor->isChecked() )
            fitxersortidatxt += "<td>"+cur1->valor("total")+"</td>";
        if(mver_totalbaseimp->isChecked() )
            fitxersortidatxt += "<td>"+cur1->valor("base")+"</td>";
        if(mver_totalimpuestos->isChecked() )
            fitxersortidatxt += "<td>"+cur1->valor("impuestos")+"</td>";
        delete cur1;
        /// ----------------------------------------------------------
        fitxersortidatxt += "</tr>";
        cur->siguienteregistro();
    }// end if
    delete cur;
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]",fitxersortidatxt);

    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }
    invocaPDF("pedidosproveedor");

}// end imprimir



void PedidosProveedorList::on_mui_borrar_clicked() {
    _depura("PedidosProveedorList::on_mui_borrar_clicked",0);
    mdb_idpedidoproveedor = mui_list->DBvalue(QString("idpedidoproveedor"));
        PedidoProveedorView *prov = new PedidoProveedorView(m_companyact,0,theApp->translate("Edicion de Facturas a Cliente", "company"));
        if (prov->cargar(mdb_idpedidoproveedor)) {
            return;
        }
        prov->borrar();
    presenta();
}// end boton_borrar


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
    addSHeader("comentpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idusuari");
    addSHeader("idtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idcliente");
    addSHeader("idproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idalmacen");
    addSHeader("idalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "codigoserie_factura");
    addSHeader("total", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "total");
    addSHeader("base", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "base");
    addSHeader("impuestos", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "impuestos");
    setinsercion(FALSE);
};





