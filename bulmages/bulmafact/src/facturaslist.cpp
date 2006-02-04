//
// C++ Implementation: facturaslist
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "facturaslist.h"
#include "company.h"
#include "qtable1.h"
#include <qmessagebox.h>
#include <q3popupmenu.h>
#include <qfile.h>
//Added by qt3to4:
#include <QTextStream>
#include "busquedacliente.h"
#include "busquedaarticulo.h"
#include "busquedafecha.h"
#include "configuracion.h"
#include "facturaview.h"
#include "funcaux.h"
#include "plugins.h"

#define COL_REFFACTURA 0
#define COL_IDFACTURA 1
#define COL_CODIGOALMACEN 2
#define COL_NUMFACTURA 3
#define COL_NOMCLIENTE 4
#define COL_FFACTURA 5
#define COL_CONTACTFACTURA 6
#define COL_TELFACTURA 7
#define COL_COMENTFACTURA 8
#define COL_IDUSUARI 9
#define COL_IDCLIENTE 10
#define COL_IDALMACEN 11
#define COL_IDSERIE_FACTURA 12
#define COL_TOTALFACTURA 13
#define COL_TOTALBASEIMP 14
#define COL_TOTALIMPUESTOS 15

void FacturasList::guardaconfig() {
    QString aux = "";
    mver_reffactura->isChecked() ? aux += "1,":aux+="0,";
    mver_idfactura->isChecked() ? aux += "1,":aux+="0,";
    mver_codigoalmacen->isChecked() ? aux += "1,":aux+="0,";
    mver_numfactura->isChecked() ? aux += "1,":aux+="0,";
    mver_nomcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_ffactura->isChecked() ? aux += "1,":aux+="0,";
    mver_contactfactura->isChecked() ? aux += "1,":aux+="0,";
    mver_telfactura->isChecked() ? aux += "1,":aux+="0,";
    mver_comentfactura->isChecked() ? aux += "1,":aux+="0,";
    mver_idusuari->isChecked() ? aux += "1,":aux+="0,";
    mver_idcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_idalmacen->isChecked() ? aux += "1,":aux+="0,";
    mver_idserie_factura->isChecked() ? aux += "1,":aux+="0,";
    mver_totalfactura->isChecked() ? aux += "1,":aux+="0,";
    mver_totalbaseimp->isChecked() ? aux += "1,":aux+="0,";
    mver_totalimpuestos->isChecked() ? aux += "1,":aux+="0,";

    QFile file( confpr->valor(CONF_DIR_USER)+"conffacturaslist.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << aux << "\n";
        for (int i = 0; i < m_list->numCols(); i++) {
            m_list->showColumn(i);
            stream << m_list->columnWidth(i) << "\n";
        }// end for
        file.close();
    }// end if
}// end guardaconfig()

void FacturasList::cargaconfig() {
    QFile file( confpr->valor(CONF_DIR_USER)+"conffacturaslist.cfn" );
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

    mver_reffactura->setChecked(line.at(0)=='1');
    mver_idfactura->setChecked(line.at(2)=='1');
    mver_codigoalmacen->setChecked(line.at(4)=='1');
    mver_numfactura->setChecked(line.at(6)=='1');
    mver_nomcliente->setChecked(line.at(8)=='1');
    mver_ffactura->setChecked(line.at(10)=='1');
    mver_contactfactura->setChecked(line.at(12)=='1');
    mver_telfactura->setChecked(line.at(14)=='1');
    mver_comentfactura->setChecked(line.at(16)=='1');
    mver_idusuari->setChecked(line.at(18)=='1');
    mver_idcliente->setChecked(line.at(20)=='1');
    mver_idalmacen->setChecked(line.at(22)=='1');
    mver_idserie_factura->setChecked(line.at(24)=='1');
    mver_totalfactura->setChecked(line.at(26)=='1');
    mver_totalbaseimp->setChecked(line.at(28)=='1');
    mver_totalimpuestos->setChecked(line.at(30)=='1');
}// end cargaconfig


void FacturasList::s_configurar() {
    if(mver_reffactura->isChecked() )
        m_list->showColumn(COL_REFFACTURA);
    else
        m_list->hideColumn(COL_REFFACTURA);

    if(mver_idfactura->isChecked() )
        m_list->showColumn(COL_IDFACTURA);
    else
        m_list->hideColumn(COL_IDFACTURA);

    if(mver_codigoalmacen->isChecked() )
        m_list->showColumn(COL_CODIGOALMACEN);
    else
        m_list->hideColumn(COL_CODIGOALMACEN);

    if(mver_numfactura->isChecked() )
        m_list->showColumn(COL_NUMFACTURA);
    else
        m_list->hideColumn(COL_NUMFACTURA);

    if(mver_nomcliente->isChecked() )
        m_list->showColumn(COL_NOMCLIENTE);
    else
        m_list->hideColumn(COL_NOMCLIENTE);

    if(mver_ffactura->isChecked() )
        m_list->showColumn(COL_FFACTURA);
    else
        m_list->hideColumn(COL_FFACTURA);

    if(mver_contactfactura->isChecked() )
        m_list->showColumn(COL_CONTACTFACTURA);
    else
        m_list->hideColumn(COL_CONTACTFACTURA);

    if(mver_telfactura->isChecked() )
        m_list->showColumn(COL_TELFACTURA);
    else
        m_list->hideColumn(COL_TELFACTURA);

    if(mver_comentfactura->isChecked() )
        m_list->showColumn(COL_COMENTFACTURA);
    else
        m_list->hideColumn(COL_COMENTFACTURA);

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

    if(mver_idserie_factura->isChecked() )
        m_list->showColumn(COL_IDSERIE_FACTURA);
    else
        m_list->hideColumn(COL_IDSERIE_FACTURA);

    if(mver_totalfactura->isChecked() )
        m_list->showColumn(COL_TOTALFACTURA);
    else
        m_list->hideColumn(COL_TOTALFACTURA);

    if(mver_totalbaseimp->isChecked() )
        m_list->showColumn(COL_TOTALBASEIMP);
    else
        m_list->hideColumn(COL_TOTALBASEIMP);

    if(mver_totalimpuestos->isChecked() )
        m_list->showColumn(COL_TOTALIMPUESTOS);
    else
        m_list->hideColumn(COL_TOTALIMPUESTOS);


}// end s_configurar

FacturasList::FacturasList(QWidget *parent, const char *name, Qt::WFlags flag)
        : FacturasListBase(parent, name, flag) {
    companyact = NULL;
    m_modo=0;
    m_idfactura="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
    inicializa();
    cargaconfig();
    s_configurar();
}// end providerslist

FacturasList::FacturasList(company *comp, QWidget *parent, const char *name)
        : FacturasListBase(parent, name) {
    companyact = comp;
    m_cliente->setcompany(companyact);
    m_articulo->setcompany(companyact);

    inicializa();
    cargaconfig();
    s_configurar();
    presenta();
    m_modo=0;
    m_idfactura="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}


FacturasList::~FacturasList() {
    companyact->sacaWindow(this);
    guardaconfig();
}


void FacturasList::inicializa() {
    _depura("FacturasList::inicializa()",1);
    m_list->setNumRows( 0 );
    m_list->setNumCols( 0 );
    m_list->setSelectionMode( Q3Table::SingleRow );
    m_list->setSorting( TRUE );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(16);
    m_list->horizontalHeader()->setLabel( COL_REFFACTURA, tr( "COL_REFFACTURA" ) );
    m_list->horizontalHeader()->setLabel( COL_IDFACTURA, tr( "COL_IDFACTURA" ) );
    m_list->horizontalHeader()->setLabel( COL_NOMCLIENTE, tr( "Cliente" ) );
    m_list->horizontalHeader()->setLabel( COL_CODIGOALMACEN, tr( "Almacén" ) );
    m_list->horizontalHeader()->setLabel( COL_NUMFACTURA, tr( "N Presupuesto" ) );
    m_list->horizontalHeader()->setLabel( COL_FFACTURA, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_IDSERIE_FACTURA, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_CONTACTFACTURA, tr( "Persona Contacto" ) );
    m_list->horizontalHeader()->setLabel( COL_TELFACTURA, tr( "Teléfono" ) );
    m_list->horizontalHeader()->setLabel( COL_COMENTFACTURA, tr( "Comentarios" ) );
    m_list->horizontalHeader()->setLabel( COL_IDUSUARI, tr("COL_IDUSUARI") );
    m_list->horizontalHeader()->setLabel( COL_IDCLIENTE, tr("COL_IDCLIENTE") );
    m_list->horizontalHeader()->setLabel( COL_IDALMACEN, tr("COL_IDALMACEN") );
    m_list->horizontalHeader()->setLabel( COL_TOTALFACTURA, tr("Total") );
    m_list->horizontalHeader()->setLabel( COL_TOTALBASEIMP, tr("Base Imponible") );
    m_list->horizontalHeader()->setLabel( COL_TOTALIMPUESTOS, tr("Impuestos") );

    if (confpr->valor(CONF_MOSTRAR_ALMACEN)!="YES") {
        m_list->hideColumn(COL_CODIGOALMACEN);
    }// end if
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor(confpr->valor(CONF_BG_LISTFACTURASCLIENTE));
    m_list->setReadOnly(TRUE);
    _depura("END FacturasList::inicializa()",1);
}// end inicializa


void FacturasList::presenta() {
    _depura("FacturasList::presenta()",1);

    cursor2 * cur= companyact->cargacursor("SELECT * FROM factura LEFT JOIN cliente ON factura.idcliente=cliente.idcliente LEFT JOIN  almacen ON  factura.idalmacen=almacen.idalmacen WHERE 1=1  "+generaFiltro());
    m_list->setNumRows( cur->numregistros() );
    int i=0;
    while (!cur->eof()) {
        m_list->setText(i,COL_REFFACTURA,cur->valor("reffactura"));
        m_list->setText(i,COL_IDFACTURA,cur->valor("idfactura"));
        m_list->setText(i,COL_NUMFACTURA,cur->valor("numfactura"));
        m_list->setText(i,COL_FFACTURA,cur->valor("ffactura"));
        m_list->setText(i,COL_CONTACTFACTURA,cur->valor("contactfactura"));
        m_list->setText(i,COL_TELFACTURA,cur->valor("telfactura"));
        m_list->setText(i,COL_COMENTFACTURA,cur->valor("comentfactura"));
        m_list->setText(i,COL_IDUSUARI,cur->valor("idusuari"));
        m_list->setText(i,COL_IDCLIENTE,cur->valor("idcliente"));
        m_list->setText(i,COL_IDALMACEN,cur->valor("idalmacen"));
        m_list->setText(i,COL_NOMCLIENTE,cur->valor("nomcliente"));
        m_list->setText(i,COL_CODIGOALMACEN,cur->valor("codigoalmacen"));
        m_list->setText(i,COL_IDSERIE_FACTURA,cur->valor("codigoserie_factura"));

        /// Calculamos el total del presupuesto y lo presentamos.
        cursor2 *cur1 = companyact->cargacursor("SELECT calctotalfactura("+cur->valor("idfactura")+") AS total, calcbimpfactura("+cur->valor("idfactura")+") AS base, calcimpuestosfactura("+cur->valor("idfactura")+") AS impuestos");
        m_list->setText(i,COL_TOTALFACTURA,cur1->valor("total"));
        m_list->setText(i,COL_TOTALBASEIMP, cur1->valor("base"));
        m_list->setText(i,COL_TOTALIMPUESTOS, cur1->valor("impuestos"));
        delete cur1;

        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;

    /// Hacemos el calculo del total.
    cur = companyact->cargacursor("SELECT SUM(calctotalfactura(idfactura)) AS total FROM factura LEFT JOIN cliente ON factura.idcliente=cliente.idcliente LEFT JOIN  almacen ON  factura.idalmacen=almacen.idalmacen WHERE 1=1  "+generaFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;

    _depura("END FacturasList::presenta()",1);
}// end inicializa



QString FacturasList::generaFiltro() {
    /// Tratamiento de los filtros.
    fprintf(stderr,"Tratamos el filtro \n");
    QString filtro="";
    if (m_filtro->text() != "") {
        filtro = " AND ( descfactura LIKE '%"+m_filtro->text()+"%' ";
        filtro +=" OR nomcliente LIKE '%"+m_filtro->text()+"%') ";
    } else {
        filtro = "";
    }// end if
    if (m_cliente->idcliente() != "") {
        filtro += " AND factura.idcliente="+m_cliente->idcliente();
    }// end if
    if (!m_procesada->isChecked() ) {
        filtro += " AND NOT procesadafactura";
    }// end if
    if (m_articulo->idarticulo() != "") {
        filtro += " AND idfactura IN (SELECT DISTINCT idfactura FROM lfactura WHERE idarticulo='"+m_articulo->idarticulo()+"')";
    }// end if
    if (m_fechain->text() != "") {
        filtro += " AND ffactura >= '"+m_fechain->text()+"' ";
    }// end if
    if (m_fechafin->text() != "") {
        filtro += " AND ffactura <= '"+m_fechafin->text()+"' ";
    }// end if
    return (filtro);
}// end generaFiltro


void FacturasList::doubleclicked(int a, int , int , const QPoint &) {
    m_idfactura = m_list->text(a,COL_IDFACTURA);
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto
    int res = g_plugins->lanza("FacturasList_doubleclicked", this);
    if (res != 0)
        return;
    if (m_modo ==0 && m_idfactura != "") {
        FacturaView *bud = companyact->newFacturaView();
        if (bud->cargaFactura(m_idfactura))
            return;
        companyact->m_pWorkspace->addWindow(bud);
        bud->show();
    } else {
        close();
    }// end if
}


void FacturasList::s_edit() {
    int a = m_list->currentRow();
	if (a >=0 ) 
    	doubleclicked(a,0,0, QPoint());
	else
	_depura("Debe seleccionar una linea",2);
}// end s_edit


void FacturasList::s_print() {

    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"facturas.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"facturas.rml";
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
    // L�ea de totales del presupuesto

    //    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"10cm, 2cm, 2cm, 3cm\" repeatRows=\"1\">";

    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    if(mver_reffactura->isChecked() )
        fitxersortidatxt += "	<td>Referencia</td>";
    if(mver_idfactura->isChecked() )
        fitxersortidatxt += "	<td>Id. Factura</td>";
    if(mver_codigoalmacen->isChecked() )
        fitxersortidatxt += "	<td>Almacen</td>";
    if(mver_numfactura->isChecked() )
        fitxersortidatxt += "	<td>Num</td>";
    if(mver_nomcliente->isChecked() )
        fitxersortidatxt += "	<td>Cliente</td>";
    if(mver_ffactura->isChecked() )
        fitxersortidatxt += "	<td>Fecha</td>";
    if(mver_contactfactura->isChecked() )
        fitxersortidatxt += "	<td>Contacto</td>";
    if(mver_telfactura->isChecked() )
        fitxersortidatxt += "	<td>Tel</td>";
    if(mver_comentfactura->isChecked() )
        fitxersortidatxt += "	<td>Coment</td>";
    if(mver_idusuari->isChecked() )
        fitxersortidatxt += "	<td>Id. Usuario</td>";
    if(mver_idcliente->isChecked() )
        fitxersortidatxt += "	<td>Id. Cliente</td>";
    if(mver_idalmacen->isChecked() )
        fitxersortidatxt += "	<td>Id. Almacen</td>";
    if(mver_idserie_factura->isChecked() )
        fitxersortidatxt += "	<td>Serie</td>";
    if(mver_totalfactura->isChecked() )
        fitxersortidatxt += "	<td>Total</td>";
    if(mver_totalbaseimp->isChecked() )
        fitxersortidatxt += "	<td>Base</td>";
    if(mver_totalimpuestos->isChecked() )
        fitxersortidatxt += "	<td>Total Impuestos</td>";

    fitxersortidatxt += "</tr>";

    cursor2 * cur= companyact->cargacursor("SELECT * FROM factura LEFT JOIN cliente ON factura.idcliente=cliente.idcliente LEFT JOIN  almacen ON  factura.idalmacen=almacen.idalmacen WHERE 1=1  "+generaFiltro());
    while(!cur->eof()) {
        fitxersortidatxt += "<tr>";
        if(mver_reffactura->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("reffactura"))+"</td>";
        if(mver_idfactura->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idfactura"))+"</td>";
        if(mver_codigoalmacen->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("codigoalmacen"))+"</td>";
        if(mver_numfactura->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("numfactura"))+"</td>";
        if(mver_nomcliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("nomcliente"))+"</td>";
        if(mver_ffactura->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("ffactura"))+"</td>";
        if(mver_contactfactura->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("contactfactura"))+"</td>";
        if(mver_telfactura->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("telfactura"))+"</td>";
        if(mver_comentfactura->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("comentfactura"))+"</td>";
        if(mver_idusuari->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("descfactura"))+"</td>";
        if(mver_idcliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idcliente"))+"</td>";
        if(mver_idalmacen->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idalmacen"))+"</td>";
        if(mver_idserie_factura->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("codigoserie_factura"))+"</td>";

        /// Calculamos el total del presupuesto y lo presentamos.
        cursor2 *cur1 = companyact->cargacursor("SELECT calctotalfactura("+cur->valor("idfactura")+") AS total, calcbimpfactura("+cur->valor("idfactura")+") AS base, calcimpuestosfactura("+cur->valor("idfactura")+") AS impuestos");
        if(mver_totalfactura->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur1->valor("total"))+"</td>";
        if(mver_totalbaseimp->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur1->valor("base"))+"</td>";
        if(mver_totalimpuestos->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur1->valor("impuestos"))+"</td>";
        delete cur1;

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

    invocaPDF("facturas");
}// end imprimir

