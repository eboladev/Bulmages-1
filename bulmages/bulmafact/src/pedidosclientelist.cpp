//
// C++ Implementation: pedidosclientelist
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <QMessageBox>
#include <Q3PopupMenu>
#include <QFile>
#include <QTextStream>

#include "pedidosclientelist.h"
#include "company.h"
#include "qtable1.h"
#include "funcaux.h"
#include "configuracion.h"
#include "pedidoclienteview.h"



PedidosClienteList::PedidosClienteList(QWidget *parent, const char *name, Qt::WFlags flag)
: QWidget(parent, name, flag) {
	setupUi(this);
    companyact = NULL;
    m_modo=0;
    m_idpedidocliente="";
    meteWindow(caption(),this);
    hideBusqueda();
}


PedidosClienteList::PedidosClienteList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag) : QWidget(parent, name, flag) {	
	setupUi(this);
    companyact = comp;
    m_cliente->setcompany(comp);
    m_articulo->setcompany(comp);
	mui_list->setcompany(comp);
	presenta();
    m_modo=0;
    m_idpedidocliente="";
    meteWindow(caption(),this);
    hideBusqueda();
}


PedidosClienteList::~PedidosClienteList() {
}




void PedidosClienteList::presenta() {

    /// Hacemos el listado y lo presentamos.
    cursor2 * cur= companyact->cargacursor("SELECT *, calctotalpedcli(idpedidocliente) AS total, calcbimppedcli(idpedidocliente) AS base, calcimpuestospedcli(idpedidocliente) AS impuestos FROM pedidocliente LEFT JOIN  cliente ON pedidocliente.idcliente=cliente.idcliente LEFT JOIN almacen ON pedidocliente.idalmacen=almacen.idalmacen WHERE 1=1  "+generarFiltro());
	mui_list->cargar(cur);
	delete cur;

	/// Hacemos el calculo del total.
	cur = companyact->cargacursor("SELECT SUM(calctotalpedcli(idpedidocliente)) AS total FROM pedidocliente LEFT JOIN cliente ON pedidocliente.idcliente=cliente.idcliente LEFT JOIN almacen ON pedidocliente.idalmacen=almacen.idalmacen WHERE 1=1"+generarFiltro());
	m_total->setText(cur->valor("total"));
	delete cur;
}// end presenta




QString PedidosClienteList::generarFiltro() {

    /// Tratamiento de los filtros.
    fprintf(stderr,"Tratamos el filtro \n");
    
    QString filtro="";
    if (m_filtro->text() != "") {
        filtro = " AND ( descpedidocliente LIKE '%"+m_filtro->text()+"%' ";
        filtro +=" OR nomcliente LIKE '%"+m_filtro->text()+"%') ";
    } else {
        filtro = "";
    }// end if
    if (m_cliente->idcliente() != "") {
        filtro += " AND pedidocliente.idcliente="+m_cliente->idcliente();
    }// end if
    if (!m_procesados->isChecked() ) {
        filtro += " AND NOT procesadopedidocliente";
    }// end if
    if (m_articulo->idarticulo() != "") {
        filtro += " AND idpedidocliente IN (SELECT DISTINCT idpedidocliente FROM lpedidocliente WHERE idarticulo='"+m_articulo->idarticulo()+"')";
    }// end if

    if (m_fechain->text() != "")
	filtro += " AND fechapedidocliente >= '"+m_fechain->text()+"' ";

    if (m_fechafin->text() != "") 
	filtro += " AND fechapedidocliente <= '"+m_fechafin->text()+"' ";

    return (filtro);
}





void PedidosClienteList::editar(int  row) {
    _depura("ProveedorList::editar",0);
    m_idpedidocliente = mui_list->DBvalue(QString("idpedidocliente"),row);
    if (m_modo ==0 ) {
        PedidoClienteView *prov = new PedidoClienteView(companyact,0,theApp->translate("Edicion de Pedidos a Cliente", "company"));
        if (prov->cargar(m_idpedidocliente)) {
            return;
        }
        companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(m_idpedidocliente));
        // close();
    }// end if
    _depura("END ProveedorList::editar",0);
}

void PedidosClienteList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
	if (a >=0 ) 
    	editar(a);
	else
	_depura("Debe seleccionar una linea",2);
}



void PedidosClienteList::imprimir() {
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"pedidoscliente.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"pedidoscliente.rml";
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

    QFile file;
    file.setName( archivod );
    file.open( QIODevice::ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;
    // L�ea de totales del presupuesto

    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += mui_list->imprimir();
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]",fitxersortidatxt);

    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }
    invocaPDF("pedidoscliente");

}// end imprimir



void PedidosClienteList::on_mui_borrar_clicked() {
    _depura("PedidosClienteList::on_mui_borrar_clicked",0);
    m_idpedidocliente = mui_list->DBvalue(QString("idpedidocliente"));
    if (m_modo ==0 ) {
        PedidoClienteView *prov = new PedidoClienteView(companyact,0,theApp->translate("Edicion de Pedidos a Cliente", "company"));
        if (prov->cargar(m_idpedidocliente))
            return;
	prov->borrar();
	delete prov;
    }// end if
    _depura("END PedidosClienteList::on_mui_borrar_clicked",0);
    presenta();
}// end boton_borrar






/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================


PedidosClienteListSubform::PedidosClienteListSubform(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("pedidocliente");
    setDBCampoId("idpedidocliente");
    addSHeader("idpedidocliente", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, "idpedidocliente");
    addSHeader("codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "codigoalmacen");
    addSHeader("refpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "refpedidocliente");
    addSHeader("numpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "numpedidocliente");
    addSHeader("descpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "descpedidocliente");
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomcliente");
    addSHeader("fechapedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "fechapedidocliente");
    addSHeader("contactpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "contactpedidocliente");
    addSHeader("telpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "telpedidocliente");
    addSHeader("comentpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "comentpedidocliente");
    addSHeader("idusuari", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idusuari");
    addSHeader("idcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idcliente");
    addSHeader("idalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idalmacen");
    addSHeader("total", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "totalpedidocliente");
    addSHeader("base", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "totalbaseimp");
    addSHeader("impuestos", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "totalimpuestos");
    setinsercion(FALSE);
};





