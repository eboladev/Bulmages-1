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

#include <QMessageBox>
#include <Q3PopupMenu>
#include <QFile>
#include <QTextStream>

#include "facturaslist.h"
#include "company.h"
#include "qtable1.h"
#include "busquedacliente.h"
#include "busquedaarticulo.h"
#include "busquedafecha.h"
#include "configuracion.h"
#include "facturaview.h"
#include "funcaux.h"
#include "plugins.h"


FacturasList::FacturasList(QWidget *parent, const char *name, Qt::WFlags flag,edmode editmodo)
        : QWidget(parent, name, flag) {
	setupUi(this);
    m_companyact = NULL;
    m_modo=editmodo;
    mdb_idfactura="";
    if (m_modo == EditMode)
        meteWindow(caption(),this);
    hideBusqueda();
}// end providerslist

FacturasList::FacturasList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag,edmode editmodo)
        : QWidget(parent, name, flag) {
	setupUi(this);
    m_companyact = comp;
    m_cliente->setcompany(m_companyact);
    m_articulo->setcompany(m_companyact);
    mui_list->setcompany(m_companyact);
    presenta();
    m_modo=editmodo;
    mdb_idfactura="";
    if (m_modo == EditMode)
        meteWindow(caption(),this);
    hideBusqueda();
}


FacturasList::~FacturasList() {
    if (m_modo == EditMode)
        m_companyact->sacaWindow(this);
}




void FacturasList::presenta() {
    _depura("FacturasList::presenta()",1);

    cursor2 * cur= m_companyact->cargacursor("SELECT *, calctotalfactura(idfactura) AS total, calcbimpfactura(idfactura) AS base, calcimpuestosfactura(idfactura) AS impuestos FROM factura LEFT JOIN cliente ON factura.idcliente=cliente.idcliente LEFT JOIN  almacen ON  factura.idalmacen=almacen.idalmacen WHERE 1=1  "+generaFiltro());
	mui_list->cargar(cur);
	delete cur;



    /// Hacemos el calculo del total.
    cur = m_companyact->cargacursor("SELECT SUM(calctotalfactura(idfactura)) AS total FROM factura LEFT JOIN cliente ON factura.idcliente=cliente.idcliente LEFT JOIN  almacen ON  factura.idalmacen=almacen.idalmacen WHERE 1=1  "+generaFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;

    _depura("END FacturasList::presenta()",1);
}// end presenta



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



void FacturasList::editar(int  row) {
    _depura("FacturasList::editar",0);
    mdb_idfactura = mui_list->DBvalue(QString("idfactura"),row);
    if (m_modo ==0 ) {
        FacturaView *prov = m_companyact->newFacturaView();
        if (prov->cargar(mdb_idfactura)) {
            return;
        }
        m_companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(mdb_idfactura));
        // close();
    }// end if
    _depura("END FacturasList::editar",0);
}

void FacturasList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
	if (a >=0 ) 
    	editar(a);
	else
	_depura("Debe seleccionar una linea",2);
}// end s_edit


void FacturasList::on_mui_imprimir_clicked() {

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

    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += mui_list->imprimir();
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]",fitxersortidatxt);

    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }// end if

    invocaPDF("facturas");
}





/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================


FacturasListSubform::FacturasListSubform(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("factura");
    setDBCampoId("idfactura");
    addSHeader("reffactura", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, "reffactura");
    addSHeader("idfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idfactura");
    addSHeader("codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "codigoalmacen");
    addSHeader("numfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "numfactura");
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomcliente");
    addSHeader("ffactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "ffactura");
    addSHeader("contactfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "contactfactura");
    addSHeader("telfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "telfactura");
    addSHeader("comentfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "comentfactura");
    addSHeader("idtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idtrabajador");
    addSHeader("idcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idcliente");
    addSHeader("idalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idalmacen");
    addSHeader("codigoserie_factura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "codigoserie_factura");
    addSHeader("total", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "total");
    addSHeader("base", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "base");
    addSHeader("impuestos", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "impuestos");
    setinsercion(FALSE);
};








