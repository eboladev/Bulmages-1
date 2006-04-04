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

#include <QMessageBox>
#include <Q3PopupMenu>
#include <QFile>
//Added by qt3to4:
#include <QTextStream>

#include "facturasplist.h"
#include "company.h"
#include "qtable1.h"
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
        mui_list->showColumn(COL_REFFACTURAP);
    else
        mui_list->hideColumn(COL_REFFACTURAP);

    if(mver_idfacturap->isChecked() )
        mui_list->showColumn(COL_IDFACTURAP);
    else
        mui_list->hideColumn(COL_IDFACTURAP);

    if(mver_numfacturap->isChecked() )
        mui_list->showColumn(COL_NUMFACTURAP);
    else
        mui_list->hideColumn(COL_NUMFACTURAP);

    if(mver_nomcliente->isChecked() )
        mui_list->showColumn(COL_NOMCLIENTE);
    else
        mui_list->hideColumn(COL_NOMCLIENTE);

    if(mver_ffacturap->isChecked() )
        mui_list->showColumn(COL_FFACTURAP);
    else
        mui_list->hideColumn(COL_FFACTURAP);

    if(mver_contactfacturap->isChecked() )
        mui_list->showColumn(COL_CONTACTFACTURAP);
    else
        mui_list->hideColumn(COL_CONTACTFACTURAP);

    if(mver_telfacturap->isChecked() )
        mui_list->showColumn(COL_TELFACTURAP);
    else
        mui_list->hideColumn(COL_TELFACTURAP);

    if(mver_comentfacturap->isChecked() )
        mui_list->showColumn(COL_COMENTFACTURAP);
    else
        mui_list->hideColumn(COL_COMENTFACTURAP);

    if(mver_idusuari->isChecked() )
        mui_list->showColumn(COL_IDUSUARI);
    else
        mui_list->hideColumn(COL_IDUSUARI);

    if(mver_idcliente->isChecked() )
        mui_list->showColumn(COL_IDCLIENTE);
    else
        mui_list->hideColumn(COL_IDCLIENTE);

    if(mver_totalfacturaproveedor->isChecked() )
        mui_list->showColumn(COL_TOTALFACTURAPROVEEDOR);
    else
        mui_list->hideColumn(COL_TOTALFACTURAPROVEEDOR);

    if(mver_totalbaseimp->isChecked() )
        mui_list->showColumn(COL_TOTALBASEIMP);
    else
        mui_list->hideColumn(COL_TOTALBASEIMP);

    if(mver_totalimpuestos->isChecked() )
        mui_list->showColumn(COL_TOTALIMPUESTOS);
    else
        mui_list->hideColumn(COL_TOTALIMPUESTOS);

}// end s_configurar

FacturasProveedorList::FacturasProveedorList(QWidget *parent, const char *name, Qt::WFlags flag)
        : QWidget(parent, name, flag) {
    setupUi(this);
    m_companyact = NULL;
    m_modo=0;
    mdb_idfacturap="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
    cargaconfig();
    s_configurar();
}// end providerslist

FacturasProveedorList::FacturasProveedorList(company *comp, QWidget *parent, const char *name)
        : QWidget(parent, name) {
    setupUi(this);
    m_companyact = comp;
    m_proveedor->setcompany(m_companyact);
    m_articulo->setcompany(m_companyact);
    cargaconfig();
    s_configurar();
    presenta();
    m_modo=0;
    mdb_idfacturap="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}


FacturasProveedorList::~FacturasProveedorList() {
    guardaconfig();
}



void FacturasProveedorList::presenta() {
    cursor2 * cur= m_companyact->cargacursor("SELECT *, calctotalfacpro(idfacturap) AS total, calcbimpfacpro(idfacturap) AS base, calcimpuestosfacpro(idfacturap) AS impuestos  FROM facturap LEFT JOIN proveedor ON facturap.idproveedor=proveedor.idproveedor WHERE 1=1  "+generaFiltro());
    mui_list->cargar(cur);
    delete cur;


    /// Hacemos el calculo del total.
    cur = m_companyact->cargacursor("SELECT SUM(calctotalfacpro(idfacturap)) AS total FROM facturap LEFT JOIN proveedor ON facturap.idproveedor=proveedor.idproveedor WHERE 1=1  "+generaFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;

    s_configurar();
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



void FacturasProveedorList::editar(int  row) {
    _depura("FacturasProveedorList::editar",0);
    mdb_idfacturap = mui_list->DBvalue(QString("idfacturap"),row);
    if (m_modo ==0 ) {
        FacturaProveedorView *prov = m_companyact->newFacturaProveedorView();
        if (prov->cargar(mdb_idfacturap)) {
            return;
        }
        m_companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(mdb_idfacturap));
        // close();
    }// end if
    _depura("END FacturasProveedorList::editar",0);
}

void FacturasProveedorList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >=0 )
        editar(a);
    else
        _depura("Debe seleccionar una linea",2);
}


void FacturasProveedorList::on_mui_borrar_clicked() {
    mdb_idfacturap = mui_list->DBvalue("idfacturap");
    if (mdb_idfacturap != "") {
        FacturaProveedorView *bud = m_companyact->newFacturaProveedorView();
        bud->cargar(mdb_idfacturap);
        bud->on_mui_borrar_clicked();
        delete bud;
    }// end if
    presenta();
}


void FacturasProveedorList::on_mui_imprimir_clicked() {
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

    cursor2 * cur= m_companyact->cargacursor("SELECT * FROM facturap LEFT JOIN proveedor ON facturap.idproveedor=proveedor.idproveedor WHERE 1=1  "+generaFiltro());
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
        cursor2 *cur1 = m_companyact->cargacursor("SELECT calctotalfacpro("+cur->valor("idfacturap")+") AS total, calcbimpfacpro("+cur->valor("idfacturap")+") AS base, calcimpuestosfacpro("+cur->valor("idfacturap")+") AS impuestos");
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





/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================

FacturasProveedorListSubform::FacturasProveedorListSubform(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("facturap");
    setDBCampoId("idfacturap");
    addSHeader("reffacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "reffacturap");
    addSHeader("idfacturap", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, "idfacturap");
    addSHeader("numfacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "numfacturap");
    addSHeader("nomproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomproveedor");
    addSHeader("ffacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "ffacturap");
    addSHeader("contactfacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "contactfacturap");
    addSHeader("telfacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "telfacturap");
    addSHeader("comentfacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "comentfacturap");
    addSHeader("idusuari", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idusuari");
    addSHeader("idproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idproveedor");
    addSHeader("total", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "total");
    addSHeader("base", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "base");
    addSHeader("impuestos", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "impuestos");
    setinsercion(FALSE);
};


