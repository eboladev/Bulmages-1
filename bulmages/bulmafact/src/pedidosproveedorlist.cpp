//
// C++ Implementation: pedidosproveedorlist
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pedidosproveedorlist.h"

#include "company.h"
#include "qtable1.h"
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qfile.h>

#include "configuracion.h"
#include "pedidoproveedorview.h"

#define COL_IDPEDIDOPROVEEDOR 0
#define COL_CODIGOALMACEN 1
#define COL_REFPEDIDOPROVEEDOR 2
#define COL_NUMPEDIDOPROVEEDOR 3
#define COL_DESCPEDIDOPROVEEDOR 4
#define COL_NOMPROVEEDOR 5
#define COL_FECHAPEDIDOPROVEEDOR 6
#define COL_CONTACTFACTURA 7
#define COL_TELFACTURA 8
#define COL_COMENTFACTURA 9
#define COL_IDUSUARI 10
#define COL_IDPROVEEDOR 11
#define COL_IDALMACEN 12
#define COL_IDSERIE_FACTURA 13


PedidosProveedorList::PedidosProveedorList(QWidget *parent, const char *name, int flag)
: PedidosProveedorListBase(parent, name, flag) {
    companyact = NULL;
    m_modo=0;
    m_idpedidoproveedor="";
    meteWindow(caption(),this);
    hideBusqueda();
}// end providerslist


PedidosProveedorList::PedidosProveedorList(company *comp, QWidget *parent, const char *name, int flag) : PedidosProveedorListBase(parent, name, flag) {
    companyact = comp;
    m_proveedor->setcompany(comp);
    m_articulo->setcompany(comp);
    inicializa();
    m_modo=0;
    m_idpedidoproveedor="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}


PedidosProveedorList::~PedidosProveedorList() {
}


void PedidosProveedorList::inicializa() {
    m_list->setNumRows( 0 );
    m_list->setSelectionMode( QTable::SingleRow );
    m_list->setSorting( TRUE );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(14);
    m_list->horizontalHeader()->setLabel( COL_IDPEDIDOPROVEEDOR, tr( "COL_IDPEDIDOPROVEEDOR" ) );
    m_list->horizontalHeader()->setLabel( COL_NOMPROVEEDOR, tr( "Proveedor" ) );
    m_list->horizontalHeader()->setLabel( COL_CODIGOALMACEN, tr( "Almac�" ) );
    m_list->horizontalHeader()->setLabel( COL_NUMPEDIDOPROVEEDOR, tr( "Num. Pedido" ) );
    m_list->horizontalHeader()->setLabel( COL_FECHAPEDIDOPROVEEDOR, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_IDSERIE_FACTURA, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_CONTACTFACTURA, tr( "Persona Contacto" ) );
    m_list->horizontalHeader()->setLabel( COL_TELFACTURA, tr( "Tel�ono" ) );
    m_list->horizontalHeader()->setLabel( COL_COMENTFACTURA, tr( "Comentarios" ) );
    m_list->horizontalHeader()->setLabel( COL_IDUSUARI, tr("Id. Usuari") );
    m_list->horizontalHeader()->setLabel( COL_IDPROVEEDOR, tr("Id. Proveedor") );
    m_list->horizontalHeader()->setLabel( COL_IDALMACEN, tr("Id. Almac�") );
    m_list->horizontalHeader()->setLabel( COL_DESCPEDIDOPROVEEDOR, tr("Descripci�") );
    m_list->horizontalHeader()->setLabel( COL_REFPEDIDOPROVEEDOR, tr("Referencia") );
    
    m_list->setColumnWidth(COL_IDPEDIDOPROVEEDOR,75);
    m_list->setColumnWidth(COL_NUMPEDIDOPROVEEDOR,75);
    m_list->setColumnWidth(COL_FECHAPEDIDOPROVEEDOR,100);
    m_list->setColumnWidth(COL_CONTACTFACTURA,200);
    m_list->setColumnWidth(COL_TELFACTURA,150);
    m_list->setColumnWidth(COL_COMENTFACTURA,300);
    m_list->setColumnWidth(COL_IDUSUARI,75);
    m_list->setColumnWidth(COL_IDPROVEEDOR,75);
    m_list->setColumnWidth(COL_DESCPEDIDOPROVEEDOR, 300);
    m_list->setColumnWidth(COL_REFPEDIDOPROVEEDOR, 75);
    
    m_list->hideColumn(COL_IDPEDIDOPROVEEDOR);
    m_list->hideColumn(COL_IDSERIE_FACTURA);
    m_list->hideColumn(COL_CONTACTFACTURA);
    m_list->hideColumn(COL_COMENTFACTURA);
    m_list->hideColumn(COL_IDUSUARI);
    m_list->hideColumn(COL_IDPROVEEDOR);
    m_list->hideColumn(COL_IDALMACEN);
    m_list->hideColumn(COL_CODIGOALMACEN);

    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor(confpr->valor(CONF_BG_LISTPEDIDOSCLIENTE));
    m_list->setReadOnly(TRUE);
     
    
    cursor2 * cur= companyact->cargacursor("SELECT * FROM pedidoproveedor LEFT JOIN  proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1  "+generarFiltro());
    m_list->setNumRows( cur->numregistros() );
    int i=0;
    while (!cur->eof()) {
        m_list->setText(i,COL_IDPEDIDOPROVEEDOR,cur->valor("idpedidoproveedor"));
        m_list->setText(i,COL_NUMPEDIDOPROVEEDOR,cur->valor("numpedidoproveedor"));
        m_list->setText(i,COL_FECHAPEDIDOPROVEEDOR,cur->valor("fechapedidoproveedor"));
        m_list->setText(i,COL_IDPROVEEDOR,cur->valor("idproveedor"));
        m_list->setText(i,COL_IDALMACEN,cur->valor("idalmacen"));
        m_list->setText(i,COL_NOMPROVEEDOR,cur->valor("nomproveedor"));
        m_list->setText(i,COL_CODIGOALMACEN,cur->valor("codigoalmacen"));
	m_list->setText(i,COL_DESCPEDIDOPROVEEDOR,cur->valor("descpedidoproveedor"));
	m_list->setText(i,COL_REFPEDIDOPROVEEDOR,cur->valor("refpedidoproveedor"));	
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
}// end inicializa

QString PedidosProveedorList::generarFiltro() {

    /// Tratamiento de los filtros.
    fprintf(stderr,"Tratamos el filtro \n");
    
    QString orden[] = {"fechapedidoproveedor","cifproveedor","nomproveedor","refpedidoproveedor","numpedidoproveedor"};
    
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
    filtro += " ORDER BY "+orden[m_orden->currentItem()];
    return (filtro);
}// end generaFiltro



void PedidosProveedorList::doubleclicked(int a, int , int , const QPoint &) {
   fprintf(stderr,"PedidosProveedorList::doubleclicked()\n");
   m_idpedidoproveedor = m_list->text(a,COL_IDPEDIDOPROVEEDOR);
   if (m_modo == 0 && m_idpedidoproveedor != "") {
      PedidoProveedorView *bud = new PedidoProveedorView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Pedidos de Proveedors", "company"));
      bud->cargaPedidoProveedor(m_idpedidoproveedor);
      bud->show();
   } else {
      close();
   }// end if
}


void PedidosProveedorList::imprimir() {
    /// Copiamos el archivo
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"pedidosproveedor.rml";
    archivo = "cp "+archivo+" /tmp/pedidosproveedor.rml";
    system (archivo.ascii());
    
    /// Copiamos el logo
    archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";
    archivo = "cp "+archivo+" /tmp/logo.jpg";
    system (archivo.ascii());

    QFile file;
    file.setName( "/tmp/pedidosproveedor.rml" );
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
    fitxersortidatxt += "	<td>Proveedor</td>";
    fitxersortidatxt += "	<td>Contacto</td>";
    fitxersortidatxt += "</tr>";    
    
    QString SQLQuery = "SELECT * FROM pedidoproveedor";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    while(!cur->eof()) {
    	fitxersortidatxt += "<tr>";
    	fitxersortidatxt += "<td>"+cur->valor("descpedidoproveedor")+"</td>";
    	fitxersortidatxt += "<td>"+cur->valor("refpedidoproveedor")+"</td>";
    	fitxersortidatxt += "<td>"+cur->valor("idproveedor")+"</td>";
    	fitxersortidatxt += "<td>"+cur->valor("contactpedidoproveedor")+"</td>";
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
    system("trml2pdf.py /tmp/pedidosproveedor.rml > /tmp/pedidosproveedor.pdf");
    system("kpdf /tmp/pedidosproveedor.pdf");

}// end imprimir



void PedidosProveedorList::s_borrarPedidosProveedor() {
    fprintf(stderr,"Iniciamos el boton_borrar\n");
    if (m_list->currentRow() >= 0) {
        if (QMessageBox::warning( this, "BulmaFact - Pedidos Proveedor", "Desea borrar el pedido seleccionado", "Si", "No") == 0) {
            companyact->begin();
            QString SQLQuery = "DELETE FROM lpedidoproveedor WHERE idpedidoproveedor ="+m_list->text(m_list->currentRow(),COL_IDPEDIDOPROVEEDOR);
            if (companyact->ejecuta(SQLQuery)==0) {
                QString SQLQuery = "DELETE FROM dpedidoproveedor WHERE idpedidoproveedor ="+m_list->text(m_list->currentRow(),COL_IDPEDIDOPROVEEDOR);
                if (companyact->ejecuta(SQLQuery)==0) {
                        QString SQLQuery = "DELETE FROM pedidoproveedor WHERE idpedidoproveedor ="+m_list->text(m_list->currentRow(),COL_IDPEDIDOPROVEEDOR);
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


void PedidosProveedorList::s_editarPedidosProveedor() {
    int a = m_list->currentRow();
    m_idpedidoproveedor = m_list->text(a,COL_IDPEDIDOPROVEEDOR);
    if (m_modo ==0 && m_idpedidoproveedor != "") {
        PedidoProveedorView *bud = new PedidoProveedorView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Presupuestos", "company"));
        bud->cargaPedidoProveedor(m_idpedidoproveedor);
        bud->show();
    } else {
        close();
    }// end if
}

