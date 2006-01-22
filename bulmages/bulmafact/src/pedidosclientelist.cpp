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
#include "funcaux.h"
#include <qmessagebox.h>
#include <q3popupmenu.h>
#include <qfile.h>
//Added by qt3to4:
#include <QTextStream>

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
#define COL_TOTALPEDIDOCLIENTE 13
#define COL_TOTALBASEIMP 14
#define COL_TOTALIMPUESTOS 15


void PedidosClienteList::guardaconfig() {
    QString aux = "";
    mver_idpedidocliente->isChecked() ? aux += "1,":aux+="0,";
    mver_codigoalmacen->isChecked() ? aux += "1,":aux+="0,";
    mver_refpedidocliente->isChecked() ? aux += "1,":aux+="0,";
    mver_numpedidocliente->isChecked() ? aux += "1,":aux+="0,";
    mver_descpedidocliente->isChecked() ? aux += "1,":aux+="0,";
    mver_nomcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_fechapedidocliente->isChecked() ? aux += "1,":aux+="0,";
    mver_contactpedidocliente->isChecked() ? aux += "1,":aux+="0,";
    mver_telpedidocliente->isChecked() ? aux += "1,":aux+="0,";
    mver_comentpedidocliente->isChecked() ? aux += "1,":aux+="0,";
    mver_idusuari->isChecked() ? aux += "1,":aux+="0,";
    mver_idcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_idalmacen->isChecked() ? aux += "1,":aux+="0,";
    mver_totalpedidocliente->isChecked() ? aux += "1,":aux+="0,";
    mver_totalbaseimp->isChecked() ? aux += "1,":aux+="0,";
    mver_totalimpuestos->isChecked() ? aux += "1,":aux+="0,";

    QFile file( confpr->valor(CONF_DIR_USER)+"/confpedidosclientelist.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << aux << "\n";
        for (int i = 0; i < m_list->numCols(); i++) {
            stream << m_list->columnWidth(i) << "\n";
        }// end for
        file.close();
    }// end if
}// end guardaconfig()

void PedidosClienteList::cargaconfig() {
    QFile file( confpr->valor(CONF_DIR_USER)+"/confpedidosclientelist.cfn" );
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

    mver_idpedidocliente->setChecked(line.at(0)=='1');
    mver_codigoalmacen->setChecked(line.at(2)=='1');
    mver_refpedidocliente->setChecked(line.at(4)=='1');
    mver_numpedidocliente->setChecked(line.at(6)=='1');
    mver_descpedidocliente->setChecked(line.at(8)=='1');
    mver_nomcliente->setChecked(line.at(10)=='1');
    mver_fechapedidocliente->setChecked(line.at(12)=='1');
    mver_contactpedidocliente->setChecked(line.at(14)=='1');
    mver_telpedidocliente->setChecked(line.at(16)=='1');
    mver_comentpedidocliente->setChecked(line.at(18)=='1');
    mver_idusuari->setChecked(line.at(20)=='1');
    mver_idcliente->setChecked(line.at(22)=='1');
    mver_idalmacen->setChecked(line.at(24)=='1');
    mver_totalpedidocliente->setChecked(line.at(26)=='1');
    mver_totalbaseimp->setChecked(line.at(28)=='1');
    mver_totalimpuestos->setChecked(line.at(30)=='1');
}// end cargaconfig


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


    if(mver_totalpedidocliente->isChecked() )
        m_list->showColumn(COL_TOTALPEDIDOCLIENTE);
    else
        m_list->hideColumn(COL_TOTALPEDIDOCLIENTE);

    if(mver_totalbaseimp->isChecked() )
        m_list->showColumn(COL_TOTALBASEIMP);
    else
        m_list->hideColumn(COL_TOTALBASEIMP);

    if(mver_totalimpuestos->isChecked() )
        m_list->showColumn(COL_TOTALIMPUESTOS);
    else
        m_list->hideColumn(COL_TOTALIMPUESTOS);



}// end s_configurar


PedidosClienteList::PedidosClienteList(QWidget *parent, const char *name, Qt::WFlags flag)
: PedidosClienteListBase(parent, name, flag) {
    companyact = NULL;
    m_modo=0;
    m_idpedidocliente="";
    meteWindow(caption(),this);
    hideBusqueda();
    cargaconfig();
    s_configurar();
}// end providerslist


PedidosClienteList::PedidosClienteList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag) : PedidosClienteListBase(parent, name, flag) {
    companyact = comp;
    m_cliente->setcompany(comp);
    m_articulo->setcompany(comp);
    inicializa();
    cargaconfig();
    s_configurar();
    m_modo=0;
    m_idpedidocliente="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}


PedidosClienteList::~PedidosClienteList() {
   guardaconfig();
}


void PedidosClienteList::inicializa() {
    m_list->setNumRows( 0 );
    m_list->setSelectionMode( Q3Table::SingleRow );
    m_list->setSorting( TRUE );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(16);
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
    m_list->horizontalHeader()->setLabel( COL_TOTALPEDIDOCLIENTE, tr("Total") );
    m_list->horizontalHeader()->setLabel( COL_TOTALBASEIMP, tr("Base Imponible") );
    m_list->horizontalHeader()->setLabel( COL_TOTALIMPUESTOS, tr("Impuestos") );

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

            /// Calculamos el total del presupuesto y lo presentamos.
            cursor2 *cur1 = companyact->cargacursor("SELECT calctotalpedcli("+cur->valor("idpedidocliente")+") AS total, calcbimppedcli("+cur->valor("idpedidocliente")+") AS base, calcimpuestospedcli("+cur->valor("idpedidocliente")+") AS impuestos");
            m_list->setText(i,COL_TOTALPEDIDOCLIENTE,cur1->valor("total"));
            m_list->setText(i,COL_TOTALBASEIMP, cur1->valor("base"));
            m_list->setText(i,COL_TOTALIMPUESTOS, cur1->valor("impuestos"));
            delete cur1;

        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;


	/// Hacemos el calculo del total.
	cur = companyact->cargacursor("SELECT SUM(calctotalpedcli(idpedidocliente)) AS total FROM pedidocliente LEFT JOIN cliente ON pedidocliente.idcliente=cliente.idcliente LEFT JOIN almacen ON pedidocliente.idalmacen=almacen.idalmacen WHERE 1=1"+generarFiltro());
	m_total->setText(cur->valor("total"));
	delete cur;

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
      PedidoClienteView *bud = new PedidoClienteView(companyact,0,theApp->translate("Edicion de Pedidos de Clientes", "company"));
	companyact->m_pWorkspace->addWindow(bud);
      bud->cargaPedidoCliente(m_idpedidocliente);
      bud->show();
   } else {
      close();
   }// end if
}


void PedidosClienteList::imprimir() {
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"pedidoscliente.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"/pedidoscliente.rml";
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
/// ---------------------------------------------------------
    if(mver_idpedidocliente->isChecked() )
	    fitxersortidatxt += "	<td>Id.</td>";
    if(mver_codigoalmacen->isChecked() )
	    fitxersortidatxt += "	<td>Cod. Almacen</td>";
    if(mver_refpedidocliente->isChecked() )
	    fitxersortidatxt += "	<td>Referencia</td>";
    if(mver_numpedidocliente->isChecked() )
	    fitxersortidatxt += "	<td>Num. Pedido</td>";
    if(mver_descpedidocliente->isChecked() )
	    fitxersortidatxt += "	<td>Descripicon</td>";
    if(mver_nomcliente->isChecked() )
	    fitxersortidatxt += "	<td>Cliente</td>";
    if(mver_fechapedidocliente->isChecked() )
	    fitxersortidatxt += "	<td>Fecha</td>";
    if(mver_contactpedidocliente->isChecked() )
	    fitxersortidatxt += "	<td>Contacto</td>";
    if(mver_telpedidocliente->isChecked() )
	    fitxersortidatxt += "	<td>Telefono</td>";
    if(mver_comentpedidocliente->isChecked() )
	    fitxersortidatxt += "	<td>Comentarios</td>";
    if(mver_idusuari->isChecked() )
	    fitxersortidatxt += "	<td>Id. Usuario</td>";
    if(mver_idcliente->isChecked() )
	    fitxersortidatxt += "	<td>Id. Cliente</td>";
    if(mver_idalmacen->isChecked() )
	    fitxersortidatxt += "	<td>Id. Almacen</td>";
    if(mver_totalpedidocliente->isChecked() )
	    fitxersortidatxt += "	<td>Total</td>";
    if(mver_totalbaseimp->isChecked() )
	    fitxersortidatxt += "	<td>Base Imp.</td>";
    if(mver_totalimpuestos->isChecked() )
	    fitxersortidatxt += "	<td>Impuestos</td>";
/// ----------------------------------------------------------
    fitxersortidatxt += "</tr>";    
    
    cursor2 * cur= companyact->cargacursor("SELECT * FROM pedidocliente LEFT JOIN  cliente ON pedidocliente.idcliente=cliente.idcliente LEFT JOIN almacen ON pedidocliente.idalmacen=almacen.idalmacen WHERE 1=1  "+generarFiltro());
    while(!cur->eof()) {
    	fitxersortidatxt += "<tr>";
/// ---------------------------------------------------------
    if(mver_idpedidocliente->isChecked() )
    	fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idpedidocliente"))+"</td>";
    if(mver_codigoalmacen->isChecked() )
    	fitxersortidatxt += "<td>"+XMLProtect(cur->valor("codigoalmacen"))+"</td>";
    if(mver_refpedidocliente->isChecked() )
    	fitxersortidatxt += "<td>"+XMLProtect(cur->valor("refpedidocliente"))+"</td>";
    if(mver_numpedidocliente->isChecked() )
    	fitxersortidatxt += "<td>"+XMLProtect(cur->valor("numpedidocliente"))+"</td>";
    if(mver_descpedidocliente->isChecked() )
    	fitxersortidatxt += "<td>"+XMLProtect(cur->valor("descpedidocliente"))+"</td>";
    if(mver_nomcliente->isChecked() )
    	fitxersortidatxt += "<td>"+XMLProtect(cur->valor("nomcliente"))+"</td>";
    if(mver_fechapedidocliente->isChecked() )
    	fitxersortidatxt += "<td>"+XMLProtect(cur->valor("fechapedidocliente"))+"</td>";
    if(mver_contactpedidocliente->isChecked() )
    	fitxersortidatxt += "<td>"+XMLProtect(cur->valor("contactpedidocliente"))+"</td>";
    if(mver_telpedidocliente->isChecked() )
    	fitxersortidatxt += "<td>"+XMLProtect(cur->valor("telpedidocliente"))+"</td>";
    if(mver_comentpedidocliente->isChecked() )
    	fitxersortidatxt += "<td>"+XMLProtect(cur->valor("comentpedidocliente"))+"</td>";
    if(mver_idusuari->isChecked() )
    	fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idusuari"))+"</td>";
    if(mver_idcliente->isChecked() )
    	fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idcliente"))+"</td>";
    if(mver_idalmacen->isChecked() )
    	fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idalmacen"))+"</td>";

            /// Calculamos el total del presupuesto y lo presentamos.
            cursor2 *cur1 = companyact->cargacursor("SELECT calctotalpedcli("+cur->valor("idpedidocliente")+") AS total, calcbimppedcli("+cur->valor("idpedidocliente")+") AS base, calcimpuestospedcli("+cur->valor("idpedidocliente")+") AS impuestos");
    if(mver_totalpedidocliente->isChecked() )
    	fitxersortidatxt += "<td>"+XMLProtect(cur1->valor("total"))+"</td>";
    if(mver_totalbaseimp->isChecked() )
    	fitxersortidatxt += "<td>"+XMLProtect(cur1->valor("base"))+"</td>";
    if(mver_totalimpuestos->isChecked() )
    	fitxersortidatxt += "<td>"+XMLProtect(cur1->valor("impuestos"))+"</td>";
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
    invocaPDF("pedidoscliente");

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
        PedidoClienteView *bud = new PedidoClienteView(companyact,0,theApp->translate("Edicion de Presupuestos", "company"));
	companyact->m_pWorkspace->addWindow(bud);
        bud->cargaPedidoCliente(m_idpedidocliente);
        bud->show();
    } else {
        close();
    }// end if
}

