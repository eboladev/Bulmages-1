/***************************************************************************
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

// Implementacion del listado de albaranes.
/*
-- COMPROVACIONS D'INTEGRITAT>Generiques:
-- Tots els albarans d'una factura corresponen al mateix client.
-- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
-- Numero
-- Data
-- Albara a clients.
CREATE TABLE albaran (
   numalbaran integer PRIMARY KEY,
   fechaalbaran date,
   idusuario integer,
 
--   idpresupuesto integer REFERENCES presupuesto(idpresupuesto),
   idcliente integer REFERENCES cliente(idcliente),
   idforma_pago integer REFERENCES forma_pago(idforma_pago),
   numfactura integer REFERENCES factura(numfactura),
   numnofactura integer REFERENCES nofactura(numnofactura)
);
 
*/
#include <QMessageBox>
#include <QFile>
#include <QCheckBox>
#include <QTextStream>


#include "albaranclientelist.h"
#include "company.h"
#include "albaranclienteview.h"
#include "qtable1.h"
#include "funcaux.h"

#define COL_REFALBARAN 0
#define COL_CODIGOALMACEN 1
#define COL_NUMALBARAN 2
#define COL_FECHAALBARAN 3
#define COL_NOMCLIENTE 4
#define COL_IDFORMA_PAGO 5
#define COL_DESCFORMA_PAGO 6
#define COL_IDUSUARIO 7
#define COL_IDCLIENTE 8
#define COL_IDALBARAN 9
#define COL_COMENTALBARAN 10
#define COL_IDALMACEN 11
#define COL_TOTALALBARAN 12
#define COL_TOTALBASEIMP 13
#define COL_TOTALIMPUESTOS 14

void AlbaranClienteList::guardaconfig() {
    QString aux = "";
    mver_refalbaran->isChecked() ? aux += "1,":aux+="0,";
    mver_codigoalmacen->isChecked() ? aux += "1,":aux+="0,";
    mver_numalbaran->isChecked() ? aux += "1,":aux+="0,";
    mver_fechaalbaran->isChecked() ? aux += "1,":aux+="0,";
    mver_nomcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_idforma_pago->isChecked() ? aux += "1,":aux+="0,";
    mver_descforma_pago->isChecked() ? aux += "1,":aux+="0,";
    mver_numfactura->isChecked() ? aux += "1,":aux+="0,";
    mver_numnofactura->isChecked() ? aux += "1,":aux+="0,";
    mver_idusuario->isChecked() ? aux += "1,":aux+="0,";
    mver_idcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_idalbaran->isChecked() ? aux += "1,":aux+="0,";
    mver_comentalbaran->isChecked() ? aux += "1,":aux+="0,";
    mver_idalmacen->isChecked() ? aux += "1,":aux+="0,";
    mver_totalalbaran->isChecked() ? aux += "1,":aux+="0,";
    mver_totalbaseimp->isChecked() ? aux += "1,":aux+="0,";
    mver_totalimpuestos->isChecked() ? aux += "1,":aux+="0,";

    QFile file( confpr->valor(CONF_DIR_USER)+"confclientdelivnoteslist.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << aux << "\n";
        file.close();
    }// end if
}// end guardaconfig()

void AlbaranClienteList::cargaconfig() {
    QFile file( confpr->valor(CONF_DIR_USER)+"confclientdelivnoteslist.cfn" );
    QString line;
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        line = stream.readLine(); // line of text excluding '\n'
        file.close();
    } else
        return;

    mver_refalbaran->setChecked(line.at(0)=='1');
    mver_codigoalmacen->setChecked(line.at(2)=='1');
    mver_numalbaran->setChecked(line.at(4)=='1');
    mver_fechaalbaran->setChecked(line.at(6)=='1');
    mver_nomcliente->setChecked(line.at(8)=='1');
    mver_idforma_pago->setChecked(line.at(10)=='1');
    mver_descforma_pago->setChecked(line.at(12)=='1');
    mver_numfactura->setChecked(line.at(14)=='1');
    mver_numnofactura->setChecked(line.at(16)=='1');
    mver_idusuario->setChecked(line.at(18)=='1');
    mver_idcliente->setChecked(line.at(20)=='1');
    mver_idalbaran->setChecked(line.at(22)=='1');
    mver_comentalbaran->setChecked(line.at(24)=='1');
    mver_idalmacen->setChecked(line.at(26)=='1');
    mver_totalalbaran->setChecked(line.at(28)=='1');
    mver_totalbaseimp->setChecked(line.at(30)=='1');
    mver_totalimpuestos->setChecked(line.at(32)=='1');
}// end cargaconfig




void AlbaranClienteList::configurar() {

    if(mver_refalbaran->isChecked() )
        mui_list->showColumn(COL_REFALBARAN);
    else
        mui_list->hideColumn(COL_REFALBARAN);

    if(mver_codigoalmacen->isChecked() )
        mui_list->showColumn(COL_CODIGOALMACEN);
    else
        mui_list->hideColumn(COL_CODIGOALMACEN);

    if(mver_numalbaran->isChecked() )
        mui_list->showColumn(COL_NUMALBARAN);
    else
        mui_list->hideColumn(COL_NUMALBARAN);

    if(mver_fechaalbaran->isChecked() )
        mui_list->showColumn(COL_FECHAALBARAN);
    else
        mui_list->hideColumn(COL_FECHAALBARAN);

    if(mver_nomcliente->isChecked() )
        mui_list->showColumn(COL_NOMCLIENTE);
    else
        mui_list->hideColumn(COL_NOMCLIENTE);

    if(mver_idforma_pago->isChecked() )
        mui_list->showColumn(COL_IDFORMA_PAGO);
    else
        mui_list->hideColumn(COL_IDFORMA_PAGO);

    if(mver_descforma_pago->isChecked() )
        mui_list->showColumn(COL_DESCFORMA_PAGO);
    else
        mui_list->hideColumn(COL_DESCFORMA_PAGO);

    if(mver_idusuario->isChecked() )
        mui_list->showColumn(COL_IDUSUARIO);
    else
        mui_list->hideColumn(COL_IDUSUARIO);

    if(mver_idcliente->isChecked() )
        mui_list->showColumn(COL_IDCLIENTE);
    else
        mui_list->hideColumn(COL_IDCLIENTE);

    if(mver_idalbaran->isChecked() )
        mui_list->showColumn(COL_IDALBARAN);
    else
        mui_list->hideColumn(COL_IDALBARAN);

    if(mver_comentalbaran->isChecked() )
        mui_list->showColumn(COL_COMENTALBARAN);
    else
        mui_list->hideColumn(COL_COMENTALBARAN);

    if(mver_idalmacen->isChecked() )
        mui_list->showColumn(COL_IDALMACEN);
    else
        mui_list->hideColumn(COL_IDALMACEN);

    if(mver_totalalbaran->isChecked() )
        mui_list->showColumn(COL_TOTALALBARAN);
    else
        mui_list->hideColumn(COL_TOTALALBARAN);

    if(mver_totalbaseimp->isChecked() )
        mui_list->showColumn(COL_TOTALBASEIMP);
    else
        mui_list->hideColumn(COL_TOTALBASEIMP);

    if(mver_totalimpuestos->isChecked() )
        mui_list->showColumn(COL_TOTALIMPUESTOS);
    else
        mui_list->hideColumn(COL_TOTALIMPUESTOS);
}// end s_configurar


AlbaranClienteList::AlbaranClienteList(QWidget *parent, const char *name, Qt::WFlags flag, edmode editmodo)
        : QWidget(parent, name, flag) {
	setupUi(this);
    m_companyact = NULL;
    cargaconfig();
    configurar();
    m_modo=editmodo;
    mdb_idalbaran="";
    if (m_modo == EditMode)
        meteWindow(caption(),this);
    hideBusqueda();
}// end providerslist


AlbaranClienteList::AlbaranClienteList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag, edmode editmodo)
        : QWidget (parent, name, flag) {
	setupUi(this);
    m_companyact = comp;
    m_cliente->setcompany(comp);
    m_articulo->setcompany(comp);
	mui_list->setcompany(comp);
    cargaconfig();
	presenta();
    m_modo=editmodo;
    mdb_idalbaran="";
    if (m_modo == EditMode)
        m_companyact->meteWindow(caption(), this);
    hideBusqueda();
    hideConfiguracion();
}// end AlbaranClienteList

AlbaranClienteList::~AlbaranClienteList() {
    if (m_modo == EditMode)
        m_companyact->sacaWindow(this);
    guardaconfig();
}// end ~providerslist



void AlbaranClienteList::presenta() {
    _depura("AlbaranClienteList::presenta\n");


    cursor2 * cur= m_companyact->cargacursor("SELECT *, calctotalalbaran(idalbaran) AS total, calcbimpalbaran(idalbaran) AS base, calcimpuestosalbaran(idalbaran) AS impuestos FROM albaran LEFT JOIN  cliente ON albaran.idcliente=cliente.idcliente LEFT JOIN almacen ON albaran.idalmacen=almacen.idalmacen LEFT JOIN forma_pago ON albaran.idforma_pago = forma_pago.idforma_pago WHERE 1=1  "+generarFiltro());
	mui_list->cargar(cur);
	delete cur;


    /// Hacemos el calculo del total.
    cur = m_companyact->cargacursor("SELECT SUM(calctotalalbaran(idalbaran)) AS total FROM albaran LEFT JOIN cliente ON albaran.idcliente=cliente.idcliente LEFT JOIN almacen ON almacen.idalmacen=albaran.idalmacen where 1=1 "+generarFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;
	configurar();

    _depura("End AlbaranClienteList::presenta");
}// end presenta





void AlbaranClienteList::editar(int  row) {
    _depura("AlbaranClienteList::editar",0);
    mdb_idalbaran = mui_list->DBvalue(QString("idalbaran"),row);
    if (m_modo ==0 ) {
        AlbaranClienteView *prov = m_companyact->newAlbaranClienteView();
        if (prov->cargar(mdb_idalbaran))
            return;
        m_companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(mdb_idalbaran));
        // close();
    }// end if
    _depura("END AlbaranClienteList::editar",0);
}

void AlbaranClienteList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
	if (a >=0 ) 
    	editar(a);
	else
	_depura("Debe seleccionar una linea",2);
}



void AlbaranClienteList::on_mui_borrar_clicked() {
    fprintf(stderr,"Iniciamos el boton_borrar\n");
    mdb_idalbaran = mui_list->DBvalue(QString("idalbaran"));
    if (m_modo ==0 ) {
        AlbaranClienteView *prov = m_companyact->newAlbaranClienteView();
        if (prov->cargar(mdb_idalbaran)) {
            return;
        }
        prov->borrar();
    }// end if
    presenta();
}// end boton_borrar


void AlbaranClienteList::imprimir() {
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"albaranescliente.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"albaranescliente.rml";
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
    /// -----------------------------------------------
    if(mver_refalbaran->isChecked() )
        fitxersortidatxt += "	<td>Referencia</td>";
    if(mver_codigoalmacen->isChecked() )
        fitxersortidatxt += "	<td>Cod.Almacen</td>";
    if(mver_numalbaran->isChecked() )
        fitxersortidatxt += "	<td>Num.</td>";
    if(mver_fechaalbaran->isChecked() )
        fitxersortidatxt += "	<td>Fecha</td>";
    if(mver_nomcliente->isChecked() )
        fitxersortidatxt += "	<td>Cliente</td>";
    if(mver_idforma_pago->isChecked() )
        fitxersortidatxt += "	<td>Id. F. Pago</td>";
    if(mver_descforma_pago->isChecked() )
        fitxersortidatxt += "	<td>F. Pago</td>";
    if(mver_numfactura->isChecked() )
        fitxersortidatxt += "	<td>Num. Factura</td>";
    if(mver_numnofactura->isChecked() )
        fitxersortidatxt += "	<td>N. No Factura</td>";
    if(mver_idusuario->isChecked() )
        fitxersortidatxt += "	<td>Id. Usuario</td>";
    if(mver_idcliente->isChecked() )
        fitxersortidatxt += "	<td>Id. Cliente</td>";
    if(mver_idalbaran->isChecked() )
        fitxersortidatxt += "	<td>Id. Albaran</td>";
    if(mver_comentalbaran->isChecked() )
        fitxersortidatxt += "	<td>Comentarios</td>";
    if(mver_idalmacen->isChecked() )
        fitxersortidatxt += "	<td>Id. Almacen</td>";
    if(mver_totalalbaran->isChecked() )
        fitxersortidatxt += "	<td>Total</td>";
    if(mver_totalbaseimp->isChecked() )
        fitxersortidatxt += "	<td>Base Imp.</td>";
    if(mver_totalimpuestos->isChecked() )
        fitxersortidatxt += "	<td>Impuestos</td>";
    /// -----------------------------------------------
    fitxersortidatxt += "</tr>";

    QString SQLQuery = "SELECT * FROM albaran, cliente, almacen where albaran.idcliente=cliente.idcliente AND albaran.idalmacen=almacen.idalmacen"+generarFiltro();
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    while(!cur->eof()) {
        fitxersortidatxt += "<tr>";
        /// -----------------------------------
        if(mver_refalbaran->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("refalbaran"))+"</td>";
        if(mver_codigoalmacen->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("codigoalmacen"))+"</td>";
        if(mver_numalbaran->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("numalbaran"))+"</td>";
        if(mver_fechaalbaran->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("fechaalbaran"))+"</td>";
        if(mver_nomcliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("nomcliente"))+"</td>";
        if(mver_idforma_pago->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idforma_pago"))+"</td>";
        if(mver_descforma_pago->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("descforma_pago"))+"</td>";
        if(mver_numfactura->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("numfactura"))+"</td>";
        if(mver_numnofactura->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("numnofactura"))+"</td>";
        if(mver_idusuario->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idusuario"))+"</td>";
        if(mver_idcliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idcliente"))+"</td>";
        if(mver_idalbaran->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idalbaran"))+"</td>";
        if(mver_comentalbaran->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("comentalbaran"))+"</td>";
        if(mver_idalmacen->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idalmacen"))+"</td>";

        /// Calculamos el total del presupuesto y lo presentamos.
        cursor2 *cur1 = m_companyact->cargacursor("SELECT calctotalalbaran("+cur->valor("idalbaran")+") AS total, calcbimpalbaran("+cur->valor("idalbaran")+") AS base, calcimpuestosalbaran("+cur->valor("idalbaran")+") AS impuestos");
        if(mver_totalalbaran->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur1->valor("total"))+"</td>";
        if(mver_totalbaseimp->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur1->valor("base"))+"</td>";
        if(mver_totalimpuestos->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur1->valor("impuestos"))+"</td>";
        delete cur1;
        /// -----------------------------------
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
    invocaPDF("albaranescliente");

}// end imprimir


QString AlbaranClienteList::generarFiltro() {
    /// Tratamiento de los filtros.
    fprintf(stderr,"Tratamos el filtro \n");
    QString filtro="";

    if (m_filtro->text() != "") {
        filtro = " AND ( descalbaran LIKE '%"+m_filtro->text()+"%' ";
        filtro +=" OR nomcliente LIKE '%"+m_filtro->text()+"%') ";
    } else {
        filtro = "";
    }// end if

    if (m_cliente->idcliente() != "")
        filtro += " AND albaran.idcliente='"+m_cliente->idcliente()+"'";

    if (m_articulo->idarticulo() != "")
        filtro += " AND idalbaran IN (SELECT DISTINCT idalbaran FROM lalbaran WHERE idarticulo='"+m_articulo->idarticulo()+"')";

    if (!m_procesados->isChecked() )
        filtro += " AND NOT procesadoalbaran";

    if (m_fechain->text() != "")
        filtro += " AND fechaalbaran >= '"+m_fechain->text()+"' ";

    if (m_fechafin->text() != "")
        filtro += " AND fechaalbaran <= '"+m_fechafin->text()+"' ";

    return (filtro);
}// end generaFiltro






/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
AlbaranClienteListSubform::AlbaranClienteListSubform(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("albaran");
    setDBCampoId("idalbaran");

    addSHeader("refalbaran", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, "refalbaran");
    addSHeader("codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "codigoalmacen");
    addSHeader("numalbaran", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "numalbaran");
    addSHeader("fechaalbaran", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "fechaalbaran");
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomcliente");
    addSHeader("idforma_pago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idforma_pago");
    addSHeader("descforma_pago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "descforma_pago");
    addSHeader("idusuari", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idusuari");
    addSHeader("idcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idcliente");
    addSHeader("idalbaran", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idalbaran");
    addSHeader("comentalbaran", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "comentalbaran");
    addSHeader("idalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idalmacen");
    addSHeader("total", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "total");
    addSHeader("base", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "base");
    addSHeader("impuestos", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "impuestos");
    setinsercion(FALSE);
};








