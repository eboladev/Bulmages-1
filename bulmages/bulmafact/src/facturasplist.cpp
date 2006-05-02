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
#include <QTextStream>

#include "facturasplist.h"
#include "company.h"
#include "qtable1.h"
#include "busquedaproveedor.h"
#include "busquedaarticulo.h"
#include "configuracion.h"
#include "facturapview.h"

FacturasProveedorList::FacturasProveedorList(QWidget *parent, const char *name, Qt::WFlags flag)
        : QWidget(parent, name, flag) {
    setupUi(this);
    m_companyact = NULL;
    m_modo=0;
    mdb_idfacturap="";
    meteWindow(caption(),this);
    hideBusqueda();
}

FacturasProveedorList::FacturasProveedorList(company *comp, QWidget *parent, const char *name)
        : QWidget(parent, name) {
    setupUi(this);
    m_companyact = comp;
    m_proveedor->setcompany(m_companyact);
    m_articulo->setcompany(m_companyact);
    presenta();
    m_modo=0;
    mdb_idfacturap="";
    meteWindow(caption(),this);
    hideBusqueda();
}


FacturasProveedorList::~FacturasProveedorList() {
}



void FacturasProveedorList::presenta() {
    cursor2 * cur= m_companyact->cargacursor("SELECT *, calctotalfacpro(idfacturap) AS total, calcbimpfacpro(idfacturap) AS base, calcimpuestosfacpro(idfacturap) AS impuestos  FROM facturap LEFT JOIN proveedor ON facturap.idproveedor=proveedor.idproveedor WHERE 1=1  "+generaFiltro());
    mui_list->cargar(cur);
    delete cur;


    /// Hacemos el calculo del total.
    cur = m_companyact->cargacursor("SELECT SUM(calctotalfacpro(idfacturap)) AS total FROM facturap LEFT JOIN proveedor ON facturap.idproveedor=proveedor.idproveedor WHERE 1=1  "+generaFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;
}



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

    return (filtro);
}



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
    // Linea de totales del presupuesto
    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += mui_list->imprimir();
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


