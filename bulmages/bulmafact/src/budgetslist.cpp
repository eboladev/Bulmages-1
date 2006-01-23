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
#include "budgetslist.h"
#include "company.h"
#include "budget.h"
#include "qtable1.h"
#include "funcaux.h"

#include <qmessagebox.h>
#include <q3popupmenu.h>
#include <qcheckbox.h>
#include <qfile.h>
#include <qcheckbox.h>
//Added by qt3to4:
#include <QTextStream>

#include "configuracion.h"
#include "funcaux.h"

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

void BudgetsList::guardaconfig() {
    _depura("BudgetsList::guardaconfig",0);
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
        for (int i = 0; i < m_list->numCols(); i++) {
            stream << m_list->columnWidth(i) << "\n";
        }// end for
        file.close();
    }// end if
}// end guardaconfig()

void BudgetsList::cargaconfig() {
    _depura("BudgetsList::cargaconfig",0);
    QFile file( confpr->valor(CONF_DIR_USER)+"confbudgetslist.cfn" );
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


void BudgetsList::s_configurar() {

    if(mver_idpresupuesto->isChecked() )
        m_list->showColumn(COL_IDPRESUPUESTO);
    else
        m_list->hideColumn(COL_IDPRESUPUESTO);

    if(mver_codigoalmacen->isChecked() )
        m_list->showColumn(COL_CODIGOALMACEN);
    else
        m_list->hideColumn(COL_CODIGOALMACEN);

    if(mver_refpresupuesto->isChecked() )
        m_list->showColumn(COL_REFPRESUPUESTO);
    else
        m_list->hideColumn(COL_REFPRESUPUESTO);

    if(mver_nomcliente->isChecked() )
        m_list->showColumn(COL_NOMCLIENTE);
    else
        m_list->hideColumn(COL_NOMCLIENTE);

    if(mver_descpresupuesto->isChecked() )
        m_list->showColumn(COL_DESCPRESUPUESTO);
    else
        m_list->hideColumn(COL_DESCPRESUPUESTO);

    if(mver_fechapresupuesto->isChecked() )
        m_list->showColumn(COL_FPRESUPUESTO);
    else
        m_list->hideColumn(COL_FPRESUPUESTO);

    if(mver_vencpresupuesto->isChecked() )
        m_list->showColumn(COL_VENCPRESUPUESTO);
    else
        m_list->hideColumn(COL_VENCPRESUPUESTO);

    if(mver_contactpresupuesto->isChecked() )
        m_list->showColumn(COL_CONTACTPRESUPUESTO);
    else
        m_list->hideColumn(COL_CONTACTPRESUPUESTO);

    if(mver_numpresupuesto->isChecked() )
        m_list->showColumn(COL_NUMPRESUPUESTO);
    else
        m_list->hideColumn(COL_NUMPRESUPUESTO);

    if(mver_telpresupuesto->isChecked() )
        m_list->showColumn(COL_TELPRESUPUESTO);
    else
        m_list->hideColumn(COL_TELPRESUPUESTO);

    if(mver_idcliente->isChecked() )
        m_list->showColumn(COL_IDCLIENTE);
    else
        m_list->hideColumn(COL_IDCLIENTE);

    if(mver_idusuario->isChecked() )
        m_list->showColumn(COL_IDUSUARI);
    else
        m_list->hideColumn(COL_IDUSUARI);

    if(mver_idalmacen->isChecked() )
        m_list->showColumn(COL_IDALMACEN);
    else
        m_list->hideColumn(COL_IDALMACEN);


    if(mver_totalpresupuesto->isChecked() )
        m_list->showColumn(COL_TOTALPRESUPUESTO);
    else
        m_list->hideColumn(COL_TOTALPRESUPUESTO);

    if(mver_totalbaseimp->isChecked() )
        m_list->showColumn(COL_TOTALBASEIMP);
    else
        m_list->hideColumn(COL_TOTALBASEIMP);


    if(mver_totalimpuestos->isChecked() )
        m_list->showColumn(COL_TOTALIMPUESTOS);
    else
        m_list->hideColumn(COL_TOTALIMPUESTOS);

    m_list->hideColumn(COL_COMENTPRESUPUESTO);
}// end s_configurar


BudgetsList::BudgetsList(QWidget *parent, const char *name, Qt::WFlags flag)
        : BudgetsListBase(parent, name, flag) {
    companyact = NULL;
    m_modo=0;
    m_idpresupuesto="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
    cargaconfig();
}// end BudgetsList

BudgetsList::BudgetsList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag)
        : BudgetsListBase(parent, name, flag) {
    companyact = comp;
    m_cliente->setcompany(comp);
    m_articulo->setcompany(comp);
    inicializa();
    cargaconfig();
    s_configurar();
    m_modo=0;
    m_idpresupuesto="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}// end BudgetsList

BudgetsList::~BudgetsList() {
    _depura("BudgetsList::~BudgetsList",0);
    guardaconfig();
    companyact->sacaWindow(this);
}// end ~providerslist

void BudgetsList::inicializa() {
    _depura("BudgetsList::inicializa()\n");
    m_list->setNumRows( 0 );
    m_list->setSorting( TRUE );
    m_list->setSelectionMode( Q3Table::SingleRow );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(17);
    m_list->horizontalHeader()->setLabel( COL_IDPRESUPUESTO, tr( "Id. Presupuesto" ) );
    m_list->horizontalHeader()->setLabel( COL_NOMCLIENTE, tr( "Cliente" ) );
    m_list->horizontalHeader()->setLabel( COL_CODIGOALMACEN, tr( "Almacén" ) );
    m_list->horizontalHeader()->setLabel( COL_NUMPRESUPUESTO, tr( "Num. Presupuesto" ) );
    m_list->horizontalHeader()->setLabel( COL_FPRESUPUESTO, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_VENCPRESUPUESTO, tr( "Vencimiento" ) );
    m_list->horizontalHeader()->setLabel( COL_CONTACTPRESUPUESTO, tr( "Persona Contacto" ) );
    m_list->horizontalHeader()->setLabel( COL_TELPRESUPUESTO, tr( "Teléfono" ) );
    m_list->horizontalHeader()->setLabel( COL_COMENTPRESUPUESTO, tr( "Comentarios" ) );
    m_list->horizontalHeader()->setLabel( COL_IDUSUARI, tr("Id. Usuario") );
    m_list->horizontalHeader()->setLabel( COL_IDCLIENTE, tr("Id. Cliente") );
    m_list->horizontalHeader()->setLabel( COL_IDALMACEN, tr("Id. Almacén") );
    m_list->horizontalHeader()->setLabel( COL_DESCPRESUPUESTO, tr("Descripción") );
    m_list->horizontalHeader()->setLabel( COL_REFPRESUPUESTO, tr("Referencia.") );
    m_list->horizontalHeader()->setLabel( COL_TOTALPRESUPUESTO, tr("Total") );
    m_list->horizontalHeader()->setLabel( COL_TOTALBASEIMP, tr("Base Imponible") );
    m_list->horizontalHeader()->setLabel( COL_TOTALIMPUESTOS, tr("Impuestos.") );

    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor(confpr->valor(CONF_BG_LISTPRESUPUESTOS));
    m_list->setReadOnly(TRUE);



    if (companyact != NULL ) {
        cursor2 * cur= companyact->cargacursor("SELECT * FROM presupuesto LEFT JOIN cliente ON presupuesto.idcliente=cliente.idcliente LEFT JOIN almacen ON almacen.idalmacen = presupuesto.idalmacen where 1=1 "+generaFiltro());
        m_list->setNumRows( cur->numregistros() );
        int i=0;
        while (!cur->eof()) {
            m_list->setText(i,COL_IDPRESUPUESTO,cur->valor("idpresupuesto"));
            m_list->setText(i,COL_NUMPRESUPUESTO,cur->valor("numpresupuesto"));
            m_list->setText(i,COL_FPRESUPUESTO,cur->valor("fpresupuesto"));
            m_list->setText(i,COL_VENCPRESUPUESTO,cur->valor("vencpresupuesto"));
            m_list->setText(i,COL_CONTACTPRESUPUESTO,cur->valor("contactpresupuesto"));
            m_list->setText(i,COL_TELPRESUPUESTO,cur->valor("telpresupuesto"));
            m_list->setText(i,COL_COMENTPRESUPUESTO,cur->valor("comentpresupuesto"));
            m_list->setText(i,COL_IDUSUARI,cur->valor("idusuari"));
            m_list->setText(i,COL_IDCLIENTE,cur->valor("idcliente"));
            m_list->setText(i,COL_IDALMACEN,cur->valor("idalmacen"));
            m_list->setText(i,COL_NOMCLIENTE,cur->valor("nomcliente"));
            m_list->setText(i,COL_CODIGOALMACEN,cur->valor("codigoalmacen"));
            m_list->setText(i,COL_REFPRESUPUESTO,cur->valor("refpresupuesto"));
            m_list->setText(i,COL_DESCPRESUPUESTO,cur->valor("descpresupuesto"));

            /// Calculamos el total del presupuesto y lo presentamos.
            cursor2 *cur1 = companyact->cargacursor("SELECT calctotalpres("+cur->valor("idpresupuesto")+") AS total, calcbimppres("+cur->valor("idpresupuesto")+") AS base, calcimpuestospres("+cur->valor("idpresupuesto")+") AS impuestos");
            m_list->setText(i,COL_TOTALPRESUPUESTO,cur1->valor("total"));
            m_list->setText(i,COL_TOTALBASEIMP, cur1->valor("base"));
            m_list->setText(i,COL_TOTALIMPUESTOS, cur1->valor("impuestos"));
            delete cur1;

            i++;
            cur->siguienteregistro();
        }// end while
        delete cur;


        /// Hacemos el calculo del total.
        cur = companyact->cargacursor("SELECT SUM(calctotalpres(idpresupuesto)) AS total FROM presupuesto LEFT JOIN cliente ON presupuesto.idcliente=cliente.idcliente LEFT JOIN almacen ON almacen.idalmacen = presupuesto.idalmacen where 1=1 "+generaFiltro());
        m_total->setText(cur->valor("total"));
        delete cur;
    }// end if


    _depura("end BudgetsList::inicializa()\n");
}// end inicializa



QString BudgetsList::generaFiltro() {
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



void BudgetsList::s_editar() {
    int a = m_list->currentRow();
    m_idpresupuesto = m_list->text(a,COL_IDPRESUPUESTO);
    if (m_modo ==0 && m_idpresupuesto != "") {
        Budget *bud = new Budget(companyact,0,theApp->translate("Edicion de Presupuestos", "company"));
        companyact->m_pWorkspace->addWindow(bud);
        bud->chargeBudget(m_idpresupuesto);
        bud->show();
    } else {
        close();
    }// end if
}

void BudgetsList::doubleclicked(int a, int , int , const QPoint &) {
    m_idpresupuesto = m_list->text(a,COL_IDPRESUPUESTO);
    if (m_modo ==0 && m_idpresupuesto != "") {
        Budget *bud = new Budget(companyact, 0,theApp->translate("Edicion de Presupuestos", "company"));
        companyact->m_pWorkspace->addWindow(bud);
        bud->chargeBudget(m_idpresupuesto);
        bud->show();
    } else {
        close();
    }// end if
}


void BudgetsList::s_contextMenu(int, int, int button, const QPoint &poin) {
    qDebug("button = %d", button);
    if (button == 2) {
        Q3PopupMenu *popup;
        popup = new Q3PopupMenu;
        popup->insertItem(tr("Borrar Presupuesto"),101);
        //popup->insertSeparator();
        int opcion = popup->exec(m_list->mapToGlobal(poin));
        switch(opcion) {
        case 101:
            s_removeBudget();
            break;
        }// end switch
        delete popup;
    }// end if
}// end contextmenu


void BudgetsList::imprimir() {
	_depura("BudgetsList::imprimir",0);
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
    cursor2 *cur = companyact->cargacursor(SQLQuery);
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
        cursor2 *cur1 = companyact->cargacursor("SELECT calctotalpres("+cur->valor("idpresupuesto")+") AS total, calcbimppres("+cur->valor("idpresupuesto")+") AS base, calcimpuestospres("+cur->valor("idpresupuesto")+") AS impuestos");
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


void BudgetsList::s_removeBudget() {
    fprintf(stderr,"Iniciamos el boton_borrar\n");
    if (m_list->currentRow() >= 0) {
        if (QMessageBox::warning( this, "BulmaFact - Presupuestos", "Desea borrar el presupuesto seleccionado", "Si", "No") == 0) {
            companyact->begin();
            QString SQLQuery = "DELETE FROM lpresupuesto WHERE idpresupuesto ="+m_list->text(m_list->currentRow(),COL_IDPRESUPUESTO);
            if (companyact->ejecuta(SQLQuery)==0) {
                QString SQLQuery = "DELETE FROM dpresupuesto WHERE idpresupuesto ="+m_list->text(m_list->currentRow(),COL_IDPRESUPUESTO);
                if (companyact->ejecuta(SQLQuery)==0) {
                    QString SQLQuery = "DELETE FROM presupuesto WHERE idpresupuesto ="+m_list->text(m_list->currentRow(),COL_IDPRESUPUESTO);
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
