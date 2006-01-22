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
#include "funcaux.h"
#include "company.h"
#include "qtable1.h"
#include <qmessagebox.h>
#include <q3popupmenu.h>
#include <qfile.h>
//Added by qt3to4:
#include <QTextStream>

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

    QFile file( confpr->valor(CONF_DIR_USER)+"/confpedidosproveedorlist.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << aux << "\n";
        for (int i = 0; i < m_list->numCols(); i++) {
            stream << m_list->columnWidth(i) << "\n";
        }// end for
        file.close();
    }// end if
}// end guardaconfig()

void PedidosProveedorList::cargaconfig() {
    QFile file( confpr->valor(CONF_DIR_USER)+"/confpedidosproveedorlist.cfn" );
    QString line;
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        line = stream.readLine(); // line of text excluding '\n'
        for (int i = 0; i < m_list->numCols(); i++) {
            QString linea = stream.readLine();
            m_list->setColumnWidth(i, linea.toInt());
        }// end for
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

    if(mver_totalpedidoproveedor->isChecked() )
        m_list->showColumn(COL_TOTALPEDIDOPROVEEDOR);
    else
        m_list->hideColumn(COL_TOTALPEDIDOPROVEEDOR);

    if(mver_totalbaseimp->isChecked() )
        m_list->showColumn(COL_TOTALBASEIMP);
    else
        m_list->hideColumn(COL_TOTALBASEIMP);

    if(mver_totalimpuestos->isChecked() )
        m_list->showColumn(COL_TOTALIMPUESTOS);
    else
        m_list->hideColumn(COL_TOTALIMPUESTOS);

}// end s_configurar


PedidosProveedorList::PedidosProveedorList(QWidget *parent, const char *name, Qt::WFlags flag)
        : PedidosProveedorListBase(parent, name, flag) {
    cargaconfig();
    s_configurar();
    companyact = NULL;
    m_modo=0;
    m_idpedidoproveedor="";
    meteWindow(caption(),this);
    hideBusqueda();
}// end providerslist


PedidosProveedorList::PedidosProveedorList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag) : PedidosProveedorListBase(parent, name, flag) {
    companyact = comp;
    m_proveedor->setcompany(comp);
    m_articulo->setcompany(comp);
    inicializa();
    cargaconfig();
    s_configurar();
    m_modo=0;
    m_idpedidoproveedor="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}


PedidosProveedorList::~PedidosProveedorList() {
    guardaconfig();
}


void PedidosProveedorList::inicializa() {
    m_list->setNumRows( 0 );
    m_list->setSelectionMode( Q3Table::SingleRow );
    m_list->setSorting( TRUE );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(16);
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
    m_list->horizontalHeader()->setLabel( COL_TOTALPEDIDOPROVEEDOR, tr("Total") );
    m_list->horizontalHeader()->setLabel( COL_TOTALBASEIMP, tr("Base Imponible") );
    m_list->horizontalHeader()->setLabel( COL_TOTALIMPUESTOS, tr("Impuestos") );

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


        /// Calculamos el total del presupuesto y lo presentamos.
        cursor2 *cur1 = companyact->cargacursor("SELECT calctotalpedpro("+cur->valor("idpedidoproveedor")+") AS total, calcbimppedpro("+cur->valor("idpedidoproveedor")+") AS base, calcimpuestospedpro("+cur->valor("idpedidoproveedor")+") AS impuestos");
        m_list->setText(i,COL_TOTALPEDIDOPROVEEDOR,cur1->valor("total"));
        m_list->setText(i,COL_TOTALBASEIMP, cur1->valor("base"));
        m_list->setText(i,COL_TOTALIMPUESTOS, cur1->valor("impuestos"));
        delete cur1;
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;

    /// Hacemos el calculo del total.
    cur = companyact->cargacursor("SELECT SUM(calctotalpedpro(idpedidoproveedor)) AS total FROM pedidoproveedor LEFT JOIN  proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1  "+generarFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;
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


void PedidosProveedorList::imprimir() {
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"pedidosproveedor.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"/pedidosproveedor.rml";
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

    archivologo = "copy "+archivologo+" "+confpr->valor(CONF_DIR_USER)+"/logo.jpg";
#else

    archivologo = "cp "+archivologo+" "+confpr->valor(CONF_DIR_USER)+"/logo.jpg";
#endif

    system (archivologo.ascii());

    QFile file;
    file.setName( archivod );
    file.open( QIODevice::ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;
    // L�ea de totales del presupuesto

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

    cursor2 * cur= companyact->cargacursor("SELECT * FROM pedidoproveedor LEFT JOIN  proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1  "+generarFiltro());
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
        cursor2 *cur1 = companyact->cargacursor("SELECT calctotalpedpro("+cur->valor("idpedidoproveedor")+") AS total, calcbimppedpro("+cur->valor("idpedidoproveedor")+") AS base, calcimpuestospedpro("+cur->valor("idpedidoproveedor")+") AS impuestos");
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
        PedidoProveedorView *bud = new PedidoProveedorView(companyact,0,theApp->translate("Edicion de Presupuestos", "company"));
        companyact->m_pWorkspace->addWindow(bud);
        bud->cargaPedidoProveedor(m_idpedidoproveedor);
        bud->show();
    } else {
        close();
    }// end if
}

void PedidosProveedorList::doubleclicked(int a, int , int , const QPoint &) {
    _depura("PedidosProveedorList::doubleclicked()\n",0);
    m_idpedidoproveedor = m_list->text(a,COL_IDPEDIDOPROVEEDOR);
    if (m_modo == 0 && m_idpedidoproveedor != "") {
        PedidoProveedorView *bud = new PedidoProveedorView(companyact,0,theApp->translate("Edicion de Pedidos de Proveedors", "company"));
        companyact->m_pWorkspace->addWindow(bud);
        bud->cargaPedidoProveedor(m_idpedidoproveedor);
        bud->show();
    } else {
        close();
    }// end if
}
