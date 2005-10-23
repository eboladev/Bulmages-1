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
#include "qtable1.h"
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
#define COL_CONTACTPEDIDOCLIENTE 7
#define COL_TELPEDIDOCLIENTE 8
#define COL_COMENTPEDIDOCLIENTE 9
#define COL_IDUSUARI 10
#define COL_IDCLIENTE 11
#define COL_IDALMACEN 12

void PedidosClienteList::s_configurar() {

    if(mver_idpedidocliente->isChecked() )
        m_list->showColumn(COL_IDPEDIDOCLIENTE);
    else
        m_list->hideColumn(COL_IDPEDIDOCLIENTE);

    if(mver_codigoalmacen->isChecked() )
        m_list->showColumn(COL_CODIGOALMACEN);
    else
        m_list->hideColumn(COL_CODIGOALMACEN);

    if(mver_refpedidocliente->isChecked() )
        m_list->showColumn(COL_REFPEDIDOCLIENTE);
    else
        m_list->hideColumn(COL_REFPEDIDOCLIENTE);

    if(mver_numpedidocliente->isChecked() )
        m_list->showColumn(COL_NUMPEDIDOCLIENTE);
    else
        m_list->hideColumn(COL_NUMPEDIDOCLIENTE);

    if(mver_descpedidocliente->isChecked() )
        m_list->showColumn(COL_DESCPEDIDOCLIENTE);
    else
        m_list->hideColumn(COL_DESCPEDIDOCLIENTE);

    if(mver_nomcliente->isChecked() )
        m_list->showColumn(COL_NOMCLIENTE);
    else
        m_list->hideColumn(COL_NOMCLIENTE);

    if(mver_fechapedidocliente->isChecked() )
        m_list->showColumn(COL_FECHAPEDIDOCLIENTE);
    else
        m_list->hideColumn(COL_FECHAPEDIDOCLIENTE);

    if(mver_contactpedidocliente->isChecked() )
        m_list->showColumn(COL_CONTACTPEDIDOCLIENTE);
    else
        m_list->hideColumn(COL_CONTACTPEDIDOCLIENTE);

    if(mver_telpedidocliente->isChecked() )
        m_list->showColumn(COL_TELPEDIDOCLIENTE);
    else
        m_list->hideColumn(COL_TELPEDIDOCLIENTE);

    if(mver_comentpedidocliente->isChecked() )
        m_list->showColumn(COL_COMENTPEDIDOCLIENTE);
    else
        m_list->hideColumn(COL_COMENTPEDIDOCLIENTE);

    if(mver_idusuari->isChecked() )
        m_list->showColumn(COL_IDUSUARI);
    else
        m_list->hideColumn(COL_IDUSUARI);

    if(mver_idcliente->isChecked() )
        m_list->showColumn(COL_IDCLIENTE);
    else
        m_list->hideColumn(COL_IDCLIENTE);

    if(mver_idalmacen->isChecked() )
        m_list->showColumn(COL_IDALMACEN);
    else
        m_list->hideColumn(COL_IDALMACEN);
}// end s_configurar


PedidosClienteList::PedidosClienteList(QWidget *parent, const char *name, int flag)
: PedidosClienteListBase(parent, name, flag) {
    companyact = NULL;
    m_modo=0;
    m_idpedidocliente="";
    meteWindow(caption(),this);
    hideBusqueda();
}// end providerslist


PedidosClienteList::PedidosClienteList(company *comp, QWidget *parent, const char *name, int flag) : PedidosClienteListBase(parent, name, flag) {
    companyact = comp;
    m_cliente->setcompany(comp);
    m_articulo->setcompany(comp);
    inicializa();
    m_modo=0;
    m_idpedidocliente="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}


PedidosClienteList::~PedidosClienteList() {
}


void PedidosClienteList::inicializa() {
    m_list->setNumRows( 0 );
    m_list->setSelectionMode( QTable::SingleRow );
    m_list->setSorting( TRUE );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(13);
    m_list->horizontalHeader()->setLabel( COL_IDPEDIDOCLIENTE, tr( "COL_IDPEDIDOCLIENTE" ) );
    m_list->horizontalHeader()->setLabel( COL_NOMCLIENTE, tr( "Cliente" ) );
    m_list->horizontalHeader()->setLabel( COL_CODIGOALMACEN, tr( "Almacén" ) );
    m_list->horizontalHeader()->setLabel( COL_NUMPEDIDOCLIENTE, tr( "Num. Pedido" ) );
    m_list->horizontalHeader()->setLabel( COL_FECHAPEDIDOCLIENTE, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_CONTACTPEDIDOCLIENTE, tr( "Persona Contacto" ) );
    m_list->horizontalHeader()->setLabel( COL_TELPEDIDOCLIENTE, tr( "Teléfono" ) );
    m_list->horizontalHeader()->setLabel( COL_COMENTPEDIDOCLIENTE, tr( "Comentarios" ) );
    m_list->horizontalHeader()->setLabel( COL_IDUSUARI, tr("Id. Usuari") );
    m_list->horizontalHeader()->setLabel( COL_IDCLIENTE, tr("Id. Cliente") );
    m_list->horizontalHeader()->setLabel( COL_IDALMACEN, tr("Id. Almacén") );
    m_list->horizontalHeader()->setLabel( COL_DESCPEDIDOCLIENTE, tr("Descripción") );
    m_list->horizontalHeader()->setLabel( COL_REFPEDIDOCLIENTE, tr("Referencia") );
    
    m_list->setColumnWidth(COL_IDPEDIDOCLIENTE,75);
    m_list->setColumnWidth(COL_NUMPEDIDOCLIENTE,75);
    m_list->setColumnWidth(COL_FECHAPEDIDOCLIENTE,100);
    m_list->setColumnWidth(COL_CONTACTPEDIDOCLIENTE,200);
    m_list->setColumnWidth(COL_TELPEDIDOCLIENTE,150);
    m_list->setColumnWidth(COL_COMENTPEDIDOCLIENTE,300);
    m_list->setColumnWidth(COL_IDUSUARI,75);
    m_list->setColumnWidth(COL_IDCLIENTE,75);
    m_list->setColumnWidth(COL_DESCPEDIDOCLIENTE, 300);
    m_list->setColumnWidth(COL_REFPEDIDOCLIENTE, 75);
    


    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor(confpr->valor(CONF_BG_LISTPEDIDOSCLIENTE));
    m_list->setReadOnly(TRUE);
     
    /// Hacemos el listado y lo presentamos.
    cursor2 * cur= companyact->cargacursor("SELECT * FROM pedidocliente LEFT JOIN  cliente ON pedidocliente.idcliente=cliente.idcliente LEFT JOIN almacen ON pedidocliente.idalmacen=almacen.idalmacen WHERE 1=1  "+generarFiltro());
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


	/// Hacemos el calculo del total.
	cur = companyact->cargacursor("SELECT SUM(calctotalpedcli(idpedidocliente)) AS total FROM pedidocliente LEFT JOIN cliente ON pedidocliente.idcliente=cliente.idcliente LEFT JOIN almacen ON pedidocliente.idalmacen=almacen.idalmacen WHERE 1=1"+generarFiltro());
	m_total->setText(cur->valor("total"));
	delete cur;


	s_configurar();
}// end inicializa

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
}// end generaFiltro



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



void PedidosClienteList::s_borrarPedidosCliente() {
    fprintf(stderr,"Iniciamos el boton_borrar\n");
    if (m_list->currentRow() >= 0) {
        if (QMessageBox::warning( this, "BulmaFact - Pedidos Cliente", "Desea borrar el pedido seleccionado", "Si", "No") == 0) {
            companyact->begin();
            QString SQLQuery = "DELETE FROM lpedidocliente WHERE idpedidocliente ="+m_list->text(m_list->currentRow(),COL_IDPEDIDOCLIENTE);
            if (companyact->ejecuta(SQLQuery)==0) {
                QString SQLQuery = "DELETE FROM dpedidocliente WHERE idpedidocliente ="+m_list->text(m_list->currentRow(),COL_IDPEDIDOCLIENTE);
                if (companyact->ejecuta(SQLQuery)==0) {
                        QString SQLQuery = "DELETE FROM pedidocliente WHERE idpedidocliente ="+m_list->text(m_list->currentRow(),COL_IDPEDIDOCLIENTE);
                        if (companyact->ejecuta(SQLQuery)==0) {
                            companyact->commit();
                        } else {
                            companyact->rollback();
                        }

                } else {
                    companyact->rollback();
                }
            } else {
                companyact->rollback();
            }
        }
    }
    inicializa();
}// end boton_borrar


void PedidosClienteList::s_editarPedidosCliente() {
    int a = m_list->currentRow();
    m_idpedidocliente = m_list->text(a,COL_IDPEDIDOCLIENTE);
    if (m_modo ==0 && m_idpedidocliente != "") {
        PedidoClienteView *bud = new PedidoClienteView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Presupuestos", "company"));
        bud->cargaPedidoCliente(m_idpedidocliente);
        bud->show();
    } else {
        close();
    }// end if
}

