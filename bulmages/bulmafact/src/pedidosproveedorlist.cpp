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
#define COL_CONTACPEDIDOPROVEEDOR 7
#define COL_TELPEDIDOPROVEEDOR 8
#define COL_COMENTPEDIDOPROVEEDOR 9
#define COL_IDUSUARI 10
#define COL_IDPROVEEDOR 11
#define COL_IDALMACEN 12

void PedidosProveedorList::s_configurar() {
    if(mver_idpedidoproveedor->isChecked() )
        m_list->showColumn(COL_IDPEDIDOPROVEEDOR);
    else
        m_list->hideColumn(COL_IDPEDIDOPROVEEDOR);

    if(mver_codigoalmacen->isChecked() )
        m_list->showColumn(COL_CODIGOALMACEN);
    else
        m_list->hideColumn(COL_CODIGOALMACEN);

    if(mver_refpedidoproveedor->isChecked() )
        m_list->showColumn(COL_REFPEDIDOPROVEEDOR);
    else
        m_list->hideColumn(COL_REFPEDIDOPROVEEDOR);

    if(mver_numpedidoproveedor->isChecked() )
        m_list->showColumn(COL_NUMPEDIDOPROVEEDOR);
    else
        m_list->hideColumn(COL_NUMPEDIDOPROVEEDOR);

    if(mver_descpedidoproveedor->isChecked() )
        m_list->showColumn(COL_DESCPEDIDOPROVEEDOR);
    else
        m_list->hideColumn(COL_DESCPEDIDOPROVEEDOR);

    if(mver_nomproveedor->isChecked() )
        m_list->showColumn(COL_NOMPROVEEDOR);
    else
        m_list->hideColumn(COL_NOMPROVEEDOR);

    if(mver_fechapedidoproveedor->isChecked() )
        m_list->showColumn(COL_FECHAPEDIDOPROVEEDOR);
    else
        m_list->hideColumn(COL_FECHAPEDIDOPROVEEDOR);

    if(mver_contacpedidoproveedor->isChecked() )
        m_list->showColumn(COL_CONTACPEDIDOPROVEEDOR);
    else
        m_list->hideColumn(COL_CONTACPEDIDOPROVEEDOR);

    if(mver_telpedidoproveedor->isChecked() )
        m_list->showColumn(COL_TELPEDIDOPROVEEDOR);
    else
        m_list->hideColumn(COL_TELPEDIDOPROVEEDOR);

    if(mver_comentpedidoproveedor->isChecked() )
        m_list->showColumn(COL_COMENTPEDIDOPROVEEDOR);
    else
        m_list->hideColumn(COL_COMENTPEDIDOPROVEEDOR);

    if(mver_idusuari->isChecked() )
        m_list->showColumn(COL_IDUSUARI);
    else
        m_list->hideColumn(COL_IDUSUARI);

    if(mver_idproveedor->isChecked() )
        m_list->showColumn(COL_IDPROVEEDOR);
    else
        m_list->hideColumn(COL_IDPROVEEDOR);

    if(mver_idalmacen->isChecked() )
        m_list->showColumn(COL_IDALMACEN);
    else
        m_list->hideColumn(COL_IDALMACEN);
}// end s_configurar


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
    m_list->setNumCols(13);
    m_list->horizontalHeader()->setLabel( COL_IDPEDIDOPROVEEDOR, tr( "COL_IDPEDIDOPROVEEDOR" ) );
    m_list->horizontalHeader()->setLabel( COL_NOMPROVEEDOR, tr( "Proveedor" ) );
    m_list->horizontalHeader()->setLabel( COL_CODIGOALMACEN, tr( "Almacén" ) );
    m_list->horizontalHeader()->setLabel( COL_NUMPEDIDOPROVEEDOR, tr( "Num. Pedido" ) );
    m_list->horizontalHeader()->setLabel( COL_FECHAPEDIDOPROVEEDOR, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_CONTACPEDIDOPROVEEDOR, tr( "Persona Contacto" ) );
    m_list->horizontalHeader()->setLabel( COL_TELPEDIDOPROVEEDOR, tr( "Teléfono" ) );
    m_list->horizontalHeader()->setLabel( COL_COMENTPEDIDOPROVEEDOR, tr( "Comentarios" ) );
    m_list->horizontalHeader()->setLabel( COL_IDUSUARI, tr("Id. Usuari") );
    m_list->horizontalHeader()->setLabel( COL_IDPROVEEDOR, tr("Id. Proveedor") );
    m_list->horizontalHeader()->setLabel( COL_IDALMACEN, tr("Id. Almacén") );
    m_list->horizontalHeader()->setLabel( COL_DESCPEDIDOPROVEEDOR, tr("Descripción") );
    m_list->horizontalHeader()->setLabel( COL_REFPEDIDOPROVEEDOR, tr("Referencia") );
    
    m_list->setColumnWidth(COL_IDPEDIDOPROVEEDOR,75);
    m_list->setColumnWidth(COL_NUMPEDIDOPROVEEDOR,75);
    m_list->setColumnWidth(COL_FECHAPEDIDOPROVEEDOR,100);
    m_list->setColumnWidth(COL_CONTACPEDIDOPROVEEDOR,200);
    m_list->setColumnWidth(COL_TELPEDIDOPROVEEDOR,150);
    m_list->setColumnWidth(COL_COMENTPEDIDOPROVEEDOR,300);
    m_list->setColumnWidth(COL_IDUSUARI,75);
    m_list->setColumnWidth(COL_IDPROVEEDOR,75);
    m_list->setColumnWidth(COL_DESCPEDIDOPROVEEDOR, 300);
    m_list->setColumnWidth(COL_REFPEDIDOPROVEEDOR, 75);


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

	/// Hacemos el calculo del total.
	cur = companyact->cargacursor("SELECT SUM(calctotalpedpro(idpedidoproveedor)) AS total FROM pedidoproveedor LEFT JOIN  proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1  "+generarFiltro());
	m_total->setText(cur->valor("total"));
	delete cur;

	s_configurar();
}// end inicializa

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

