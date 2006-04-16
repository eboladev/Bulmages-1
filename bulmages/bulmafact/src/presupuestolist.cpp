/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

// Implementaci� del listado de presupuestos.
#include <QMessageBox>
#include <Q3PopupMenu>
#include <QCheckBox>
#include <QFile>
#include <QCheckBox>
#include <QTextStream>

#include "presupuestolist.h"
#include "company.h"
#include "presupuestoview.h"
#include "qtable1.h"
#include "funcaux.h"
#include "plugins.h"
#include "configuracion.h"

#define COL_IDPRESUPUESTO 0
#define COL_CODIGOALMACEN 1
#define COL_NUMPRESUPUESTO 2
#define COL_REFPRESUPUESTO 3
#define COL_NOMCLIENTE 4
#define COL_DESCPRESUPUESTO 5
#define COL_FPRESUPUESTO 6
#define COL_VENCPRESUPUESTO 7
#define COL_CONTACTPRESUPUESTO 8
#define COL_TELPRESUPUESTO 9
#define COL_COMENTPRESUPUESTO 10
#define COL_IDUSUARI 11
#define COL_IDCLIENTE 12
#define COL_IDALMACEN 13
#define COL_TOTALPRESUPUESTO 14
#define COL_TOTALBASEIMP 15
#define COL_TOTALIMPUESTOS 16

void PresupuestoList::guardaconfig() {
    _depura("PresupuestoList::guardaconfig",0);
    QString aux = "";
    mver_idpresupuesto->isChecked() ? aux += "1,":aux+="0,";
    mver_codigoalmacen->isChecked() ? aux += "1,":aux+="0,";
    mver_numpresupuesto->isChecked() ? aux += "1,":aux+="0,";
    mver_refpresupuesto->isChecked() ? aux += "1,":aux+="0,";
    mver_nomcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_descpresupuesto->isChecked() ? aux += "1,":aux+="0,";
    mver_fechapresupuesto->isChecked() ? aux += "1,":aux+="0,";
    mver_vencpresupuesto->isChecked() ? aux += "1,":aux+="0,";
    mver_contactpresupuesto->isChecked() ? aux += "1,":aux+="0,";
    mver_telpresupuesto->isChecked() ? aux += "1,":aux+="0,";
    mver_idusuario->isChecked() ? aux += "1,":aux+="0,";
    mver_idalmacen->isChecked() ? aux += "1,":aux+="0,";
    mver_totalpresupuesto->isChecked() ? aux += "1,":aux+="0,";
    mver_totalbaseimp->isChecked() ? aux += "1,":aux+="0,";
    mver_totalimpuestos->isChecked() ? aux += "1,":aux+="0,";

    QFile file( confpr->valor(CONF_DIR_USER)+"confbudgetslist.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << aux << "\n";
        file.close();
    }// end if
}// end guardaconfig()

void PresupuestoList::cargaconfig() {
    _depura("PresupuestoList::cargaconfig",0);
    QFile file( confpr->valor(CONF_DIR_USER)+"confbudgetslist.cfn" );
    QString line;
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        line = stream.readLine(); // line of text excluding '\n'
        file.close();
    } else
        return;

    mver_idpresupuesto->setChecked(line.at(0)=='1');
    mver_codigoalmacen->setChecked(line.at(2)=='1');
    mver_numpresupuesto->setChecked(line.at(4)=='1');
    mver_refpresupuesto->setChecked(line.at(6)=='1');
    mver_nomcliente->setChecked(line.at(8)=='1');
    mver_descpresupuesto->setChecked(line.at(10)=='1');
    mver_fechapresupuesto->setChecked(line.at(12)=='1');
    mver_vencpresupuesto->setChecked(line.at(14)=='1');
    mver_contactpresupuesto->setChecked(line.at(16)=='1');
    mver_telpresupuesto->setChecked(line.at(18)=='1');
    mver_idusuario->setChecked(line.at(20)=='1');
    mver_idalmacen->setChecked(line.at(22)=='1');
    mver_totalpresupuesto->setChecked(line.at(24)=='1');
    mver_totalbaseimp->setChecked(line.at(26)=='1');
    mver_totalimpuestos->setChecked(line.at(28)=='1');
}// end cargaconfig


void PresupuestoList::s_configurar() {

    if(mver_idpresupuesto->isChecked() )
        mui_list->showColumn(COL_IDPRESUPUESTO);
    else
        mui_list->hideColumn(COL_IDPRESUPUESTO);

    if(mver_codigoalmacen->isChecked() )
        mui_list->showColumn(COL_CODIGOALMACEN);
    else
        mui_list->hideColumn(COL_CODIGOALMACEN);

    if(mver_refpresupuesto->isChecked() )
        mui_list->showColumn(COL_REFPRESUPUESTO);
    else
        mui_list->hideColumn(COL_REFPRESUPUESTO);

    if(mver_nomcliente->isChecked() )
        mui_list->showColumn(COL_NOMCLIENTE);
    else
        mui_list->hideColumn(COL_NOMCLIENTE);

    if(mver_descpresupuesto->isChecked() )
        mui_list->showColumn(COL_DESCPRESUPUESTO);
    else
        mui_list->hideColumn(COL_DESCPRESUPUESTO);

    if(mver_fechapresupuesto->isChecked() )
        mui_list->showColumn(COL_FPRESUPUESTO);
    else
        mui_list->hideColumn(COL_FPRESUPUESTO);

    if(mver_vencpresupuesto->isChecked() )
        mui_list->showColumn(COL_VENCPRESUPUESTO);
    else
        mui_list->hideColumn(COL_VENCPRESUPUESTO);

    if(mver_contactpresupuesto->isChecked() )
        mui_list->showColumn(COL_CONTACTPRESUPUESTO);
    else
        mui_list->hideColumn(COL_CONTACTPRESUPUESTO);

    if(mver_numpresupuesto->isChecked() )
        mui_list->showColumn(COL_NUMPRESUPUESTO);
    else
        mui_list->hideColumn(COL_NUMPRESUPUESTO);

    if(mver_telpresupuesto->isChecked() )
        mui_list->showColumn(COL_TELPRESUPUESTO);
    else
        mui_list->hideColumn(COL_TELPRESUPUESTO);

    if(mver_idcliente->isChecked() )
        mui_list->showColumn(COL_IDCLIENTE);
    else
        mui_list->hideColumn(COL_IDCLIENTE);

    if(mver_idusuario->isChecked() )
        mui_list->showColumn(COL_IDUSUARI);
    else
        mui_list->hideColumn(COL_IDUSUARI);

    if(mver_idalmacen->isChecked() )
        mui_list->showColumn(COL_IDALMACEN);
    else
        mui_list->hideColumn(COL_IDALMACEN);


    if(mver_totalpresupuesto->isChecked() )
        mui_list->showColumn(COL_TOTALPRESUPUESTO);
    else
        mui_list->hideColumn(COL_TOTALPRESUPUESTO);

    if(mver_totalbaseimp->isChecked() )
        mui_list->showColumn(COL_TOTALBASEIMP);
    else
        mui_list->hideColumn(COL_TOTALBASEIMP);


    if(mver_totalimpuestos->isChecked() )
        mui_list->showColumn(COL_TOTALIMPUESTOS);
    else
        mui_list->hideColumn(COL_TOTALIMPUESTOS);

    mui_list->hideColumn(COL_COMENTPRESUPUESTO);
}// end s_configurar


PresupuestoList::PresupuestoList(QWidget *parent, const char *name, Qt::WFlags flag)
        : QWidget(parent, name, flag) {
    setupUi(this);
    m_companyact = NULL;
    m_modo=0;
    m_idpresupuesto="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
    cargaconfig();
}// end PresupuestoList

PresupuestoList::PresupuestoList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag)
        : QWidget(parent, name, flag) {
    setupUi(this);
    m_companyact = comp;
    m_cliente->setcompany(comp);
    m_articulo->setcompany(comp);
    presenta();
    m_modo=0;
    m_idpresupuesto="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}// end PresupuestoList

PresupuestoList::~PresupuestoList() {
    _depura("PresupuestoList::~PresupuestoList",0);
    guardaconfig();
    m_companyact->sacaWindow(this);
}

void PresupuestoList::presenta() {
    _depura("PresupuestoList::presenta()\n");

    /// Hacemos el listado y lo presentamos.
    cursor2 * cur= m_companyact->cargacursor("SELECT *, totalpresupuesto AS total, bimppresupuesto AS base, imppresupuesto AS impuestos FROM presupuesto LEFT JOIN  cliente ON presupuesto.idcliente=cliente.idcliente LEFT JOIN almacen ON presupuesto.idalmacen=almacen.idalmacen WHERE 1=1  "+generaFiltro());
	mui_list->cargar(cur);
	delete cur;

	/// Hacemos el calculo del total.
	cur = m_companyact->cargacursor("SELECT SUM(totalpresupuesto) AS total FROM presupuesto LEFT JOIN cliente ON presupuesto.idcliente=cliente.idcliente LEFT JOIN almacen ON presupuesto.idalmacen=almacen.idalmacen WHERE 1=1"+generaFiltro());
	m_total->setText(cur->valor("total"));
	delete cur;

    s_configurar();


    _depura("end PresupuestoList::presenta()\n");
}// end presenta



QString PresupuestoList::generaFiltro() {
    /// Tratamiento de los filtros.
    QString filtro="";
    if (m_filtro->text() != "") {
        filtro = " AND ( descpresupuesto LIKE '%"+m_filtro->text()+"%' ";
        filtro +=" OR nomcliente LIKE '%"+m_filtro->text()+"%') ";
    } else {
        filtro = "";
    }// end if
    if (m_cliente->idcliente() != "") {
        filtro += " AND presupuesto.idcliente="+m_cliente->idcliente();
    }// end if
    if (!m_procesados->isChecked() ) {
        filtro += " AND NOT procesadopresupuesto";
    }// end if
    if (m_articulo->idarticulo() != "") {
        filtro += " AND idpresupuesto IN (SELECT DISTINCT idpresupuesto FROM lpresupuesto WHERE idarticulo='"+m_articulo->idarticulo()+"')";
    }// end if


    if (m_fechain->text() != "")
        filtro += " AND fpresupuesto >= '"+m_fechain->text()+"' ";

    if (m_fechafin->text() != "")
        filtro += " AND fpresupuesto <= '"+m_fechafin->text()+"' ";


    return (filtro);
}// end generaFiltro


void PresupuestoList::editar(int  row) {
    _depura("PresupuestoList::editar",0);
    m_idpresupuesto = mui_list->DBvalue(QString("idpresupuesto"),row);
    if (m_modo ==0 ) {
        PresupuestoView *prov = m_companyact->newBudget();
        if (prov->cargar(m_idpresupuesto)) {
            return;
        }
        m_companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(m_idpresupuesto));
    }// end if
    _depura("END PresupuestoList::editar",0);
}


void PresupuestoList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
	if (a >=0 ) 
    	editar(a);
	else
	_depura("Debe seleccionar una linea",2);
}




void PresupuestoList::imprimir() {
    _depura("PresupuestoList::imprimir",0);
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"presupuestos.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"presupuestos.rml";
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
    if(mver_idpresupuesto->isChecked() )
        fitxersortidatxt += "	<td>Id. </td>";
    if(mver_codigoalmacen->isChecked() )
        fitxersortidatxt += "	<td>Cod. Almacen</td>";
    if(mver_refpresupuesto->isChecked() )
        fitxersortidatxt += "	<td>Referencia</td>";
    if(mver_nomcliente->isChecked() )
        fitxersortidatxt += "	<td>Cliente</td>";
    if(mver_descpresupuesto->isChecked() )
        fitxersortidatxt += "	<td>Descripicon</td>";
    if(mver_fechapresupuesto->isChecked() )
        fitxersortidatxt += "	<td>Fecha</td>";
    if(mver_vencpresupuesto->isChecked() )
        fitxersortidatxt += "	<td>Vencimiento</td>";
    if(mver_contactpresupuesto->isChecked() )
        fitxersortidatxt += "	<td>Contacto</td>";
    if(mver_numpresupuesto->isChecked() )
        fitxersortidatxt += "	<td>Num.</td>";
    if(mver_telpresupuesto->isChecked() )
        fitxersortidatxt += "	<td>Tel.</td>";
    if(mver_idcliente->isChecked() )
        fitxersortidatxt += "	<td>Id. Cliente</td>";
    if(mver_idusuario->isChecked() )
        fitxersortidatxt += "	<td>Id. Usuario</td>";
    if(mver_idalmacen->isChecked() )
        fitxersortidatxt += "	<td>Id. Almacen</td>";
    if(mver_totalpresupuesto->isChecked() )
        fitxersortidatxt += "	<td>Total</td>";
    if(mver_totalbaseimp->isChecked() )
        fitxersortidatxt += "	<td>Base Imp.</td>";
    if(mver_totalimpuestos->isChecked() )
        fitxersortidatxt += "	<td>Impuestos</td>";
    fitxersortidatxt += "</tr>";

    QString SQLQuery = "SELECT * FROM presupuesto, cliente, almacen where presupuesto.idcliente=cliente.idcliente AND presupuesto.idalmacen=almacen.idalmacen "+generaFiltro();
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    while(!cur->eof()) {
        fitxersortidatxt += "<tr>";
        if(mver_idpresupuesto->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idpresupuesto"))+"</td>";
        if(mver_codigoalmacen->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("codigoalmacen"))+"</td>";
        if(mver_refpresupuesto->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("refpresupuesto"))+"</td>";
        if(mver_nomcliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("nomcliente"))+"</td>";
        if(mver_descpresupuesto->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("descpresupuesto"))+"</td>";
        if(mver_fechapresupuesto->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("fpresupuesto"))+"</td>";
        if(mver_vencpresupuesto->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("vencpresupuesto"))+"</td>";
        if(mver_contactpresupuesto->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("contactpresupuesto"))+"</td>";
        if(mver_numpresupuesto->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("numpresupuesto"))+"</td>";
        if(mver_telpresupuesto->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("telpresupuesto"))+"</td>";
        if(mver_idcliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idcliente"))+"</td>";
        if(mver_idusuario->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idusuario"))+"</td>";
        if(mver_idalmacen->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idalmacen"))+"</td>";

        /// Calculamos el total del presupuesto y lo presentamos.
        cursor2 *cur1 = m_companyact->cargacursor("SELECT calctotalpres("+cur->valor("idpresupuesto")+") AS total, calcbimppres("+cur->valor("idpresupuesto")+") AS base, calcimpuestospres("+cur->valor("idpresupuesto")+") AS impuestos");
        if(mver_totalpresupuesto->isChecked() )
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

    invocaPDF("presupuestos");

}// end imprimir


void PresupuestoList::on_mui_borrar_clicked() {
    _depura("PresupuestoList::on_mui_borrar_clicked",0);
	int row = mui_list->currentRow();
    m_idpresupuesto = mui_list->DBvalue(QString("idpresupuesto"),row);
    if (m_modo ==0 ) {
        PresupuestoView *prov = m_companyact->newBudget();
        if (prov->cargar(m_idpresupuesto)) {
            return;
        }
	prov->borrar();
	delete prov;
        // close();
    }// end if
    _depura("END PresupuestoList::on_mui_borrar_clicked",0);
    presenta();
}// end boton_borrar





/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================

PresupuestoListSubForm::PresupuestoListSubForm(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("presupuesto");
    setDBCampoId("idpresupuesto");
    addSHeader("idpresupuesto", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, "idpedidocliente");
    addSHeader("codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "codigoalmacen");
    addSHeader("numpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "numpedidocliente");
    addSHeader("refpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "refpedidocliente");
    addSHeader("descpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "descpedidocliente");
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomcliente");
    addSHeader("fpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "fechapedidocliente");
    addSHeader("contactpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "contactpedidocliente");
    addSHeader("telpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "telpedidocliente");
    addSHeader("comentpresupuesto", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "comentpedidocliente");
    addSHeader("idusuari", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idusuari");
    addSHeader("idcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idcliente");
    addSHeader("idalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idalmacen");
    addSHeader("total", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "totalpedidocliente");
    addSHeader("base", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "totalbaseimp");
    addSHeader("impuestos", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "totalimpuestos");
    setinsercion(FALSE);
};


