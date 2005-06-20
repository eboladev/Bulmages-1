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

// Implementaciï¿½ del listado de presupuestos.
#include "budgetslist.h"
#include "company.h"
#include "budget.h"
#include <qtable.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qcheckbox.h>
#include <qfile.h>
#include <qcheckbox.h>

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

    m_list->hideColumn(COL_COMENTPRESUPUESTO);

}


BudgetsList::BudgetsList(QWidget *parent, const char *name, int flag)
: BudgetsListBase(parent, name, flag) {
    companyact = NULL;
    m_modo=0;
    m_idpresupuesto="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}// end providerslist

BudgetsList::BudgetsList(company *comp, QWidget *parent, const char *name, int flag)
: BudgetsListBase(parent, name, flag) {
    companyact = comp;
    m_cliente->setcompany(comp);
    m_articulo->setcompany(comp);
    inicializa();
    m_modo=0;
    m_idpresupuesto="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}// end providerslist

BudgetsList::~BudgetsList() {
    companyact->sacaWindow(this);
}// end ~providerslist

void BudgetsList::inicializa() {
    fprintf(stderr,"BudgetsList::inicializa()\n");
    m_list->setNumRows( 0 );
    m_list->setSorting( FALSE );
    m_list->setSelectionMode( QTable::SingleRow );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(14);
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
    m_list->setColumnWidth(COL_IDPRESUPUESTO,75);
    m_list->setColumnWidth(COL_NUMPRESUPUESTO,75);
    m_list->setColumnWidth(COL_FPRESUPUESTO,100);
    m_list->setColumnWidth(COL_VENCPRESUPUESTO,100);
    m_list->setColumnWidth(COL_CONTACTPRESUPUESTO,200);
    m_list->setColumnWidth(COL_TELPRESUPUESTO,150);
    m_list->setColumnWidth(COL_COMENTPRESUPUESTO,300);
    m_list->setColumnWidth(COL_DESCPRESUPUESTO, 300);
    m_list->setColumnWidth(COL_REFPRESUPUESTO,100);
    m_list->setColumnWidth(COL_IDUSUARI,75);
    m_list->setColumnWidth(COL_IDCLIENTE,75);
    m_list->setColumnWidth(COL_IDALMACEN,75);
    m_list->setColumnWidth(COL_NOMCLIENTE,200);
    m_list->setColumnWidth(COL_CODIGOALMACEN,75);

    if (confpr->valor(CONF_MOSTRAR_ALMACEN)!="YES") {
        m_list->hideColumn(COL_CODIGOALMACEN);
    }// end if

    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor(confpr->valor(CONF_BG_LISTPRESUPUESTOS));
    fprintf(stderr,"---------> Color del presupuesto\n\n\n%s\n\n\n",confpr->valor(CONF_BG_LISTPRESUPUESTOS).ascii());
    m_list->setReadOnly(TRUE);



    if (companyact != NULL ) {
        cursor2 * cur= companyact->cargacursor("SELECT * FROM presupuesto, cliente, almacen where presupuesto.idcliente=cliente.idcliente AND presupuesto.idalmacen=almacen.idalmacen "+generaFiltro());
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
            i++;
            cur->siguienteregistro();
        }// end while
        delete cur;
    }// end if
    s_configurar();
    fprintf(stderr,"end BudgetsList::inicializa()\n");
}// end inicializa



QString BudgetsList::generaFiltro() {
    /// Tratamiento de los filtros.
    fprintf(stderr,"Tratamos el filtro \n");
    
    QString orden[] = {"fpresupuesto","vencpresupuesto","cifcliente","nomcliente","refpresupuesto","numpresupuesto"};
    
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
    filtro += " ORDER BY "+orden[m_orden->currentItem()];
    return (filtro);
}// end generaFiltro



void BudgetsList::s_editar() {
    int a = m_list->currentRow();
    m_idpresupuesto = m_list->text(a,COL_IDPRESUPUESTO);
    if (m_modo ==0 && m_idpresupuesto != "") {
        Budget *bud = new Budget(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Presupuestos", "company"));
        bud->chargeBudget(m_idpresupuesto);
        bud->show();
    } else {
        close();
    }// end if
}

void BudgetsList::doubleclicked(int a, int , int , const QPoint &) {
    m_idpresupuesto = m_list->text(a,COL_IDPRESUPUESTO);
    if (m_modo ==0 && m_idpresupuesto != "") {
        Budget *bud = new Budget(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Presupuestos", "company"));
        bud->chargeBudget(m_idpresupuesto);
        bud->show();
    } else {
        close();
    }// end if
}


void BudgetsList::s_contextMenu(int, int, int button, const QPoint &poin) {
    qDebug("button = %d", button);
    if (button == 2) {
        QPopupMenu *popup;
        popup = new QPopupMenu;
        popup->insertItem(tr("Borrar Presupuesto"),101);
        //popup->insertSeparator();
        int opcion = popup->exec(m_list->mapToGlobal(poin));
        switch(opcion) {
        case 101:
            s_removeBudget();
            break;
        }// end switch
        delete popup;
    }
}// end contextmenu


void BudgetsList::newBudget() {
    fprintf(stderr,"Iniciamos el boton_crear\n");
    Budget *bud = new Budget(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Presupuestos", "company"));
    bud->show();
}// end boton_crear


void BudgetsList::imprimir() {
    /// Copiamos el archivo
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"presupuestos.rml";
    archivo = "cp "+archivo+" /tmp/presupuestos.rml";
    system (archivo.ascii());

    /// Copiamos el logo
    archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";
    archivo = "cp "+archivo+" /tmp/logo.jpg";
    system (archivo.ascii());

    QFile file;
    file.setName( "/tmp/presupuestos.rml" );
    file.open( IO_ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;
    // Lï¿½ea de totales del presupuesto

    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"10cm, 2cm, 2cm, 3cm\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "	<td>Descripicon</td>";
    fitxersortidatxt += "	<td>Referencia</td>";
    fitxersortidatxt += "	<td>Cliente</td>";
    fitxersortidatxt += "	<td>Contacto</td>";
    fitxersortidatxt += "</tr>";

    QString SQLQuery = "SELECT * FROM presupuesto, cliente, almacen where presupuesto.idcliente=cliente.idcliente AND presupuesto.idalmacen=almacen.idalmacen "+generaFiltro();
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    while(!cur->eof()) {
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "<td>"+cur->valor("descpresupuesto")+"</td>";
        fitxersortidatxt += "<td>"+cur->valor("refpresupuesto")+"</td>";
        fitxersortidatxt += "<td>"+cur->valor("idcliente")+"</td>";
        fitxersortidatxt += "<td>"+cur->valor("contactpresupuesto")+"</td>";
        fitxersortidatxt += "</tr>";
        cur->siguienteregistro();
    }// end if
    delete cur;
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]",fitxersortidatxt);

    if ( file.open( IO_WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }
    system("trml2pdf.py /tmp/presupuestos.rml > /tmp/pressupost.pdf");
    system("kpdf /tmp/pressupost.pdf &");

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
