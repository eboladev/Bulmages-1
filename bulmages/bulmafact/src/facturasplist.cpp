//
// C++ Implementation: facturapslist
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "facturasplist.h"
#include "company.h"
#include "qtable1.h"
#include <QMessageBox>
#include <Q3PopupMenu>
#include <QFile>
//Added by qt3to4:
#include <QTextStream>
#include "busquedaproveedor.h"
#include "busquedaarticulo.h"
#include "configuracion.h"
#include "facturapview.h"

#define COL_REFFACTURAP 0
#define COL_IDFACTURAP 1
#define COL_NUMFACTURAP 2
#define COL_NOMCLIENTE 3
#define COL_FFACTURAP 4
#define COL_CONTACTFACTURAP 5
#define COL_TELFACTURAP 6
#define COL_COMENTFACTURAP 7
#define COL_IDUSUARI 8
#define COL_IDCLIENTE 9
#define COL_TOTALFACTURAPROVEEDOR 10
#define COL_TOTALBASEIMP 11
#define COL_TOTALIMPUESTOS 12

void FacturasProveedorList::guardaconfig() {
    _depura("FacturasProveedorList::guardaconfig",0);
    QString aux = "";
    mver_reffacturap->isChecked() ? aux += "1,":aux+="0,";
    mver_idfacturap->isChecked() ? aux += "1,":aux+="0,";
    mver_numfacturap->isChecked() ? aux += "1,":aux+="0,";
    mver_nomcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_ffacturap->isChecked() ? aux += "1,":aux+="0,";
    mver_contactfacturap->isChecked() ? aux += "1,":aux+="0,";
    mver_telfacturap->isChecked() ? aux += "1,":aux+="0,";
    mver_comentfacturap->isChecked() ? aux += "1,":aux+="0,";
    mver_idusuari->isChecked() ? aux += "1,":aux+="0,";
    mver_idcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_totalfacturaproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_totalbaseimp->isChecked() ? aux += "1,":aux+="0,";
    mver_totalimpuestos->isChecked() ? aux += "1,":aux+="0,";

    QFile file( confpr->valor(CONF_DIR_USER)+"conffacturasproveedorlist.cfn" );
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

void FacturasProveedorList::cargaconfig() {
    _depura("FacturasProveedorList::cargaconfig",0);
    QFile file( confpr->valor(CONF_DIR_USER)+"conffacturasproveedorlist.cfn" );
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

    mver_reffacturap->setChecked(line.at(0)=='1');
    mver_idfacturap->setChecked(line.at(2)=='1');
    mver_numfacturap->setChecked(line.at(4)=='1');
    mver_nomcliente->setChecked(line.at(6)=='1');
    mver_ffacturap->setChecked(line.at(8)=='1');
    mver_contactfacturap->setChecked(line.at(10)=='1');
    mver_telfacturap->setChecked(line.at(12)=='1');
    mver_comentfacturap->setChecked(line.at(14)=='1');
    mver_idusuari->setChecked(line.at(16)=='1');
    mver_idcliente->setChecked(line.at(18)=='1');
    mver_totalfacturaproveedor->setChecked(line.at(20)=='1');
    mver_totalbaseimp->setChecked(line.at(22)=='1');
    mver_totalimpuestos->setChecked(line.at(24)=='1');
}// end cargaconfig


void FacturasProveedorList::s_configurar() {
    _depura ("FacturasProveedorList::s_configurar",0);
    if(mver_reffacturap->isChecked() )
        m_list->showColumn(COL_REFFACTURAP);
    else
        m_list->hideColumn(COL_REFFACTURAP);

    if(mver_idfacturap->isChecked() )
        m_list->showColumn(COL_IDFACTURAP);
    else
        m_list->hideColumn(COL_IDFACTURAP);

    if(mver_numfacturap->isChecked() )
        m_list->showColumn(COL_NUMFACTURAP);
    else
        m_list->hideColumn(COL_NUMFACTURAP);

    if(mver_nomcliente->isChecked() )
        m_list->showColumn(COL_NOMCLIENTE);
    else
        m_list->hideColumn(COL_NOMCLIENTE);

    if(mver_ffacturap->isChecked() )
        m_list->showColumn(COL_FFACTURAP);
    else
        m_list->hideColumn(COL_FFACTURAP);

    if(mver_contactfacturap->isChecked() )
        m_list->showColumn(COL_CONTACTFACTURAP);
    else
        m_list->hideColumn(COL_CONTACTFACTURAP);

    if(mver_telfacturap->isChecked() )
        m_list->showColumn(COL_TELFACTURAP);
    else
        m_list->hideColumn(COL_TELFACTURAP);

    if(mver_comentfacturap->isChecked() )
        m_list->showColumn(COL_COMENTFACTURAP);
    else
        m_list->hideColumn(COL_COMENTFACTURAP);

    if(mver_idusuari->isChecked() )
        m_list->showColumn(COL_IDUSUARI);
    else
        m_list->hideColumn(COL_IDUSUARI);

    if(mver_idcliente->isChecked() )
        m_list->showColumn(COL_IDCLIENTE);
    else
        m_list->hideColumn(COL_IDCLIENTE);

    if(mver_totalfacturaproveedor->isChecked() )
        m_list->showColumn(COL_TOTALFACTURAPROVEEDOR);
    else
        m_list->hideColumn(COL_TOTALFACTURAPROVEEDOR);

    if(mver_totalbaseimp->isChecked() )
        m_list->showColumn(COL_TOTALBASEIMP);
    else
        m_list->hideColumn(COL_TOTALBASEIMP);

    if(mver_totalimpuestos->isChecked() )
        m_list->showColumn(COL_TOTALIMPUESTOS);
    else
        m_list->hideColumn(COL_TOTALIMPUESTOS);

}// end s_configurar

FacturasProveedorList::FacturasProveedorList(QWidget *parent, const char *name, Qt::WFlags flag)
        : FacturasProveedorListBase(parent, name, flag) {
    companyact = NULL;
    m_modo=0;
    m_idfacturap="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
    inicializa();
    cargaconfig();
    s_configurar();
}// end providerslist

FacturasProveedorList::FacturasProveedorList(company *comp, QWidget *parent, const char *name)
        : FacturasProveedorListBase(parent, name) {
    companyact = comp;
    m_proveedor->setcompany(companyact);
    m_articulo->setcompany(companyact);
    inicializa();
    cargaconfig();
    s_configurar();
    presenta();
    m_modo=0;
    m_idfacturap="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}


FacturasProveedorList::~FacturasProveedorList() {
    guardaconfig();
}


void FacturasProveedorList::inicializa() {
    m_list->setNumRows( 0 );
    m_list->setNumCols( 0 );
    m_list->setSelectionMode( Q3Table::SingleRow );
    m_list->setSorting( TRUE );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(13);
    m_list->horizontalHeader()->setLabel( COL_REFFACTURAP, tr( "Referencia" ) );
    m_list->horizontalHeader()->setLabel( COL_IDFACTURAP, tr( "COL_IDFACTURAP" ) );
    m_list->horizontalHeader()->setLabel( COL_NOMCLIENTE, tr( "Proveedor" ) );
    m_list->horizontalHeader()->setLabel( COL_NUMFACTURAP, tr( "N Factura" ) );
    m_list->horizontalHeader()->setLabel( COL_FFACTURAP, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_CONTACTFACTURAP, tr( "Persona Contacto" ) );
    m_list->horizontalHeader()->setLabel( COL_TELFACTURAP, tr( "Teléfono" ) );
    m_list->horizontalHeader()->setLabel( COL_COMENTFACTURAP, tr( "Comentarios" ) );
    m_list->horizontalHeader()->setLabel( COL_IDUSUARI, tr("COL_IDUSUARI") );
    m_list->horizontalHeader()->setLabel( COL_IDCLIENTE, tr("COL_IDCLIENTE") );
    m_list->horizontalHeader()->setLabel( COL_TOTALFACTURAPROVEEDOR, tr("Total") );
    m_list->horizontalHeader()->setLabel( COL_TOTALBASEIMP, tr("Base Imponible") );
    m_list->horizontalHeader()->setLabel( COL_TOTALIMPUESTOS, tr("Impuestos") );

    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuración que es global.
    m_list->setPaletteBackgroundColor("#EEFFFF");
    m_list->setReadOnly(TRUE);
}// end inicializa


void FacturasProveedorList::presenta() {

    cursor2 * cur= companyact->cargacursor("SELECT * FROM facturap LEFT JOIN proveedor ON facturap.idproveedor=proveedor.idproveedor WHERE 1=1  "+generaFiltro());
    m_list->setNumRows( cur->numregistros() );
    int i=0;
    while (!cur->eof()) {
        m_list->setText(i,COL_REFFACTURAP,cur->valor("reffacturap"));
        m_list->setText(i,COL_IDFACTURAP,cur->valor("idfacturap"));
        m_list->setText(i,COL_NUMFACTURAP,cur->valor("numfacturap"));
        m_list->setText(i,COL_FFACTURAP,cur->valor("ffacturap"));
        m_list->setText(i,COL_CONTACTFACTURAP,cur->valor("contactfacturap"));
        m_list->setText(i,COL_TELFACTURAP,cur->valor("telfacturap"));
        m_list->setText(i,COL_COMENTFACTURAP,cur->valor("comentfacturap"));
        m_list->setText(i,COL_IDUSUARI,cur->valor("idusuari"));
        m_list->setText(i,COL_IDCLIENTE,cur->valor("idproveedor"));
        m_list->setText(i,COL_NOMCLIENTE,cur->valor("nomproveedor"));

        /// Calculamos el total del presupuesto y lo presentamos.
        cursor2 *cur1 = companyact->cargacursor("SELECT calctotalfacpro("+cur->valor("idfacturap")+") AS total, calcbimpfacpro("+cur->valor("idfacturap")+") AS base, calcimpuestosfacpro("+cur->valor("idfacturap")+") AS impuestos");
        m_list->setText(i,COL_TOTALFACTURAPROVEEDOR,cur1->valor("total"));
        m_list->setText(i,COL_TOTALBASEIMP, cur1->valor("base"));
        m_list->setText(i,COL_TOTALIMPUESTOS, cur1->valor("impuestos"));
        delete cur1;
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;


    /// Hacemos el calculo del total.
    cur = companyact->cargacursor("SELECT SUM(calctotalfacpro(idfacturap)) AS total FROM facturap LEFT JOIN proveedor ON facturap.idproveedor=proveedor.idproveedor WHERE 1=1  "+generaFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;

}// end presenta





QString FacturasProveedorList::generaFiltro() {
    /// Tratamiento de los filtros.
    fprintf(stderr,"Tratamos el filtro \n");
    QString filtro="";
    if (m_filtro->text() != "") {
        filtro = " AND ( descfacturap LIKE '%"+m_filtro->text()+"%' ";
        filtro +=" OR nomproveedor LIKE '%"+m_filtro->text()+"%') ";
    } else {
        filtro = "";
    }// end if
    if (m_proveedor->idproveedor() != "") {
        filtro += " AND facturap.idproveedor="+m_proveedor->idproveedor();
    }// end if
    if (!m_procesada->isChecked() ) {
        filtro += " AND NOT procesadafacturap";
    }// end if
    if (m_articulo->idarticulo() != "") {
        filtro += " AND idfacturap IN (SELECT DISTINCT idfacturap FROM lfacturap WHERE idarticulo='"+m_articulo->idarticulo()+"')";
    }// end if

    if (m_fechain->text() != "")
        filtro += " AND ffacturap >= '"+m_fechain->text()+"' ";

    if (m_fechafin->text() != "")
        filtro += " AND ffacturap <= '"+m_fechafin->text()+"' ";

    //    filtro += " ORDER BY numfacturap";
    return (filtro);
}// end generaFiltro

void FacturasProveedorList::doubleclicked(int a, int , int , const QPoint &) {
    m_idfacturap = m_list->text(a,COL_IDFACTURAP);
    if (m_modo ==0 && m_idfacturap != "") {
        FacturaProveedorView *bud = companyact->newFacturaProveedorView();
        if(bud->cargaFacturaProveedor(m_idfacturap))
            return;
        companyact->m_pWorkspace->addWindow(bud);
        bud->show();
    } else {
        close();
    }// end if
}


void FacturasProveedorList::s_editarFacturaProveedor() {
    int a = m_list->currentRow();
	if (a >=0 ) 
    	doubleclicked(a,0,0, QPoint());
	else
	_depura("Debe seleccionar una linea",2);
}


void FacturasProveedorList::s_borrarFacturaProveedor() {
    m_idfacturap = m_list->text(m_list->currentRow(),COL_IDFACTURAP);
    if (m_idfacturap != "") {
        FacturaProveedorView *bud = companyact->newFacturaProveedorView();
        bud->cargaFacturaProveedor(m_idfacturap);
        bud->borraFacturaProveedor();
        delete bud;
    }// end if
    presenta();
}


void FacturasProveedorList::s_imprimir() {
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"facturasproveedor.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"facturasproveedor.rml";
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

    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    /// ---------------------------------------------------------------------
    if(mver_reffacturap->isChecked() )
        fitxersortidatxt += "	<td>Referencia</td>";
    if(mver_idfacturap->isChecked() )
        fitxersortidatxt += "	<td>Id.</td>";
    if(mver_numfacturap->isChecked() )
        fitxersortidatxt += "	<td>Num</td>";
    if(mver_nomcliente->isChecked() )
        fitxersortidatxt += "	<td>Cliente</td>";
    if(mver_ffacturap->isChecked() )
        fitxersortidatxt += "	<td>Fecha</td>";
    if(mver_contactfacturap->isChecked() )
        fitxersortidatxt += "	<td>Contacto</td>";
    if(mver_telfacturap->isChecked() )
        fitxersortidatxt += "	<td>Tel</td>";
    if(mver_comentfacturap->isChecked() )
        fitxersortidatxt += "	<td>Coment</td>";
    if(mver_idusuari->isChecked() )
        fitxersortidatxt += "	<td>Id. Usuario</td>";
    if(mver_idcliente->isChecked() )
        fitxersortidatxt += "	<td>Id. Cliente</td>";
    if(mver_totalfacturaproveedor->isChecked() )
        fitxersortidatxt += "	<td>Total</td>";
    if(mver_totalbaseimp->isChecked() )
        fitxersortidatxt += "	<td>Base Imp.</td>";
    if(mver_totalimpuestos->isChecked() )
        fitxersortidatxt += "	<td>Impuestos</td>";
    /// ---------------------------------------------------------------------
    fitxersortidatxt += "</tr>";

    cursor2 * cur= companyact->cargacursor("SELECT * FROM facturap LEFT JOIN proveedor ON facturap.idproveedor=proveedor.idproveedor WHERE 1=1  "+generaFiltro());
    m_list->setNumRows( cur->numregistros() );
    while(!cur->eof()) {
        fitxersortidatxt += "<tr>";
        /// ----------------------------------------------------------
        if(mver_reffacturap->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("reffacturap")+"</td>";
        if(mver_idfacturap->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("idfacturap")+"</td>";
        if(mver_numfacturap->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("numfacturap")+"</td>";
        if(mver_nomcliente->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("nomproveedor")+"</td>";
        if(mver_ffacturap->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("ffacturap")+"</td>";
        if(mver_contactfacturap->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("contactfacturap")+"</td>";
        if(mver_telfacturap->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("telfacturap")+"</td>";
        if(mver_comentfacturap->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("comentfacturap")+"</td>";
        if(mver_idusuari->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("idusuari")+"</td>";
        if(mver_idcliente->isChecked() )
            fitxersortidatxt += "<td>"+cur->valor("idproveedor")+"</td>";

        /// Calculamos el total del presupuesto y lo presentamos.
        cursor2 *cur1 = companyact->cargacursor("SELECT calctotalfacpro("+cur->valor("idfacturap")+") AS total, calcbimpfacpro("+cur->valor("idfacturap")+") AS base, calcimpuestosfacpro("+cur->valor("idfacturap")+") AS impuestos");
        if(mver_totalfacturaproveedor->isChecked() )
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
    invocaPDF("facturasproveedor");

}// end imprimir

