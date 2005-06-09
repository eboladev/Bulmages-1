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
#include "pedidosclientelist.h"

#include "company.h"
#include <qtable.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qfile.h>

#include "configuracion.h"
#include "pedidoclienteview.h"

#define COL_IDPEDIDOCLIENTE 0
#define COL_CODIGOALMACEN 1
#define COL_REFPEDIDOCLIENTE 2
#define COL_NUMPEDIDOCLIENTE 3
#define COL_DESCPEDIDOCLIENTE 4
#define COL_NOMCLIENTE 5
#define COL_FECHAPEDIDOCLIENTE 6
#define COL_CONTACTFACTURA 7
#define COL_TELFACTURA 8
#define COL_COMENTFACTURA 9
#define COL_IDUSUARI 10
#define COL_IDCLIENTE 11
#define COL_IDALMACEN 12
#define COL_IDSERIE_FACTURA 13



PedidosClienteList::PedidosClienteList(company *comp, QWidget *parent, const char *name) : PedidosClienteListBase(parent, name) {
    companyact = comp;
    inicializa();
    m_modo=0;
    m_idpedidocliente="";
    companyact->meteWindow(caption(),this);
}


PedidosClienteList::~PedidosClienteList() {
}


void PedidosClienteList::inicializa() {
    m_list->setNumRows( 0 );
    m_list->setNumCols( 0 );
    m_list->setSelectionMode( QTable::SingleRow );
    m_list->setSorting( TRUE );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(14);
    m_list->horizontalHeader()->setLabel( COL_IDPEDIDOCLIENTE, tr( "COL_IDPEDIDOCLIENTE" ) );
    m_list->horizontalHeader()->setLabel( COL_NOMCLIENTE, tr( "Cliente" ) );
    m_list->horizontalHeader()->setLabel( COL_CODIGOALMACEN, tr( "Almac�" ) );
    m_list->horizontalHeader()->setLabel( COL_NUMPEDIDOCLIENTE, tr( "N Presupuesto" ) );
    m_list->horizontalHeader()->setLabel( COL_FECHAPEDIDOCLIENTE, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_IDSERIE_FACTURA, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_CONTACTFACTURA, tr( "Persona Contacto" ) );
    m_list->horizontalHeader()->setLabel( COL_TELFACTURA, tr( "Tel�ono" ) );
    m_list->horizontalHeader()->setLabel( COL_COMENTFACTURA, tr( "Comentarios" ) );
    m_list->horizontalHeader()->setLabel( COL_IDUSUARI, tr("COL_IDUSUARI") );
    m_list->horizontalHeader()->setLabel( COL_IDCLIENTE, tr("COL_IDCLIENTE") );
    m_list->horizontalHeader()->setLabel( COL_IDALMACEN, tr("COL_IDALMACEN") );
    m_list->horizontalHeader()->setLabel( COL_DESCPEDIDOCLIENTE, tr("Descripcion") );
    m_list->horizontalHeader()->setLabel( COL_REFPEDIDOCLIENTE, tr("Referencia") );
    
    m_list->setColumnWidth(COL_IDPEDIDOCLIENTE,75);
    m_list->setColumnWidth(COL_NUMPEDIDOCLIENTE,75);
    m_list->setColumnWidth(COL_FECHAPEDIDOCLIENTE,100);
    m_list->setColumnWidth(COL_CONTACTFACTURA,200);
    m_list->setColumnWidth(COL_TELFACTURA,150);
    m_list->setColumnWidth(COL_COMENTFACTURA,300);
    m_list->setColumnWidth(COL_IDUSUARI,75);
    m_list->setColumnWidth(COL_IDCLIENTE,75);
    m_list->setColumnWidth(COL_DESCPEDIDOCLIENTE, 300);
    m_list->setColumnWidth(COL_REFPEDIDOCLIENTE, 75);
    
    m_list->hideColumn(COL_IDPEDIDOCLIENTE);
    m_list->hideColumn(COL_IDSERIE_FACTURA);
    m_list->hideColumn(COL_CONTACTFACTURA);
    m_list->hideColumn(COL_COMENTFACTURA);
    m_list->hideColumn(COL_IDUSUARI);
    m_list->hideColumn(COL_IDCLIENTE);
    m_list->hideColumn(COL_IDALMACEN);
    m_list->hideColumn(COL_CODIGOALMACEN);
    
    if (confpr->valor(CONF_MOSTRAR_ALMACEN)!="YES") {
        m_list->hideColumn(COL_CODIGOALMACEN);
    }// end if

    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor("#FFEEFF");
    m_list->setReadOnly(TRUE);
    
    
    QString filtro;
    if (m_filtro->text() != "") {
    	filtro = " AND ( descpedidocliente LIKE '%"+m_filtro->text()+"%' ";
	filtro +=" OR nomcliente LIKE '%"+m_filtro->text()+"%') ";
    } else {
    	filtro = "";
    }// end if    
    
    cursor2 * cur= companyact->cargacursor("SELECT * FROM pedidocliente, cliente, almacen where pedidocliente.idcliente=cliente.idcliente AND pedidocliente.idalmacen=almacen.idalmacen"+filtro);
    m_list->setNumRows( cur->numregistros() );
    int i=0;
    while (!cur->eof()) {
        m_list->setText(i,COL_IDPEDIDOCLIENTE,cur->valor("idpedidocliente"));
        m_list->setText(i,COL_NUMPEDIDOCLIENTE,cur->valor("numpedidocliente"));
        m_list->setText(i,COL_FECHAPEDIDOCLIENTE,cur->valor("fechapedidocliente"));
        m_list->setText(i,COL_IDCLIENTE,cur->valor("idcliente"));
        m_list->setText(i,COL_IDALMACEN,cur->valor("idalmacen"));
        m_list->setText(i,COL_NOMCLIENTE,cur->valor("nomcliente"));
        m_list->setText(i,COL_CODIGOALMACEN,cur->valor("codigoalmacen"));
	m_list->setText(i,COL_DESCPEDIDOCLIENTE,cur->valor("descpedidocliente"));
	m_list->setText(i,COL_REFPEDIDOCLIENTE,cur->valor("refpedidocliente"));	
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
}// end inicializa


void PedidosClienteList::doubleclicked(int a, int , int , const QPoint &) {
   fprintf(stderr,"PedidosClienteList::doubleclicked()\n");
   m_idpedidocliente = m_list->text(a,COL_IDPEDIDOCLIENTE);
   if (m_modo == 0 && m_idpedidocliente != "") {
      PedidoClienteView *bud = new PedidoClienteView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Pedidos de Clientes", "company"));
      bud->cargaPedidoCliente(m_idpedidocliente);
      bud->show();
   } else {
      close();
   }// end if
}


void PedidosClienteList::imprimir() {
    /// Copiamos el archivo
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"pedidoscliente.rml";
    archivo = "cp "+archivo+" /tmp/pedidoscliente.rml";
    system (archivo.ascii());
    
    /// Copiamos el logo
    archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";
    archivo = "cp "+archivo+" /tmp/logo.jpg";
    system (archivo.ascii());

    QFile file;
    file.setName( "/tmp/pedidoscliente.rml" );
    file.open( IO_ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;
    // L�ea de totales del presupuesto

    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"10cm, 2cm, 2cm, 3cm\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "	<td>Descripicon</td>";
    fitxersortidatxt += "	<td>Referencia</td>";
    fitxersortidatxt += "	<td>Cliente</td>";
    fitxersortidatxt += "	<td>Contacto</td>";
    fitxersortidatxt += "</tr>";    
    
    QString SQLQuery = "SELECT * FROM pedidocliente";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    while(!cur->eof()) {
    	fitxersortidatxt += "<tr>";
    	fitxersortidatxt += "<td>"+cur->valor("descpedidocliente")+"</td>";
    	fitxersortidatxt += "<td>"+cur->valor("refpedidocliente")+"</td>";
    	fitxersortidatxt += "<td>"+cur->valor("idcliente")+"</td>";
    	fitxersortidatxt += "<td>"+cur->valor("contactpedidocliente")+"</td>";
    	fitxersortidatxt += "</tr>";
	cur->siguienteregistro();
    }// end if
    delete cur;
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]",fitxersortidatxt);

    if ( file.open( IO_WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }
    system("trml2pdf.py /tmp/pedidoscliente.rml > /tmp/pedidoscliente.pdf");
    system("kpdf /tmp/pedidoscliente.pdf");

}// end imprimir



