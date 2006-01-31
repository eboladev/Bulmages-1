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

// Implementación del listado de albaranes.
/*
-- COMPROVACIONS D'INTEGRITAT>Genèriques:
-- Tots els albarans d'una factura corresponen al mateix client.
-- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
-- Numero
-- Data
-- Albará a clients.
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
#include "clientdelivnoteslist.h"
#include "company.h"
#include "albaranclienteview.h"
#include "qtable1.h"
#include <qmessagebox.h>
#include <q3popupmenu.h>
#include <qfile.h>
#include <qcheckbox.h>
//Added by qt3to4:
#include <QTextStream>
#include "funcaux.h"

#define COL_REFALBARAN 0
#define COL_CODIGOALMACEN 1
#define COL_NUMALBARAN 2
#define COL_FECHAALBARAN 3
#define COL_NOMCLIENTE 4
#define COL_IDFORMA_PAGO 5
#define COL_DESCFORMA_PAGO 6
#define COL_NUMFACTURA 7
#define COL_NUMNOFACTURA 8
#define COL_IDUSUARIO 9
#define COL_IDCLIENTE 10
#define COL_IDALBARAN 11
#define COL_COMENTALBARAN 12
#define COL_IDALMACEN 13
#define COL_TOTALALBARAN 14
#define COL_TOTALBASEIMP 15
#define COL_TOTALIMPUESTOS 16

void ClientDelivNotesList::guardaconfig() {
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
    //	mver_comentpresupuesto->isChecked() ? aux += "1,":aux+="0,";
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
        for (int i = 0; i < m_list->numCols(); i++) {
            m_list->showColumn(i);
            stream << m_list->columnWidth(i) << "\n";
        }// end for
        file.close();
    }// end if
}// end guardaconfig()

void ClientDelivNotesList::cargaconfig() {
    QFile file( confpr->valor(CONF_DIR_USER)+"confclientdelivnoteslist.cfn" );
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




void ClientDelivNotesList::s_configurar() {

    if(mver_refalbaran->isChecked() )
        m_list->showColumn(COL_REFALBARAN);
    else
        m_list->hideColumn(COL_REFALBARAN);

    if(mver_codigoalmacen->isChecked() )
        m_list->showColumn(COL_CODIGOALMACEN);
    else
        m_list->hideColumn(COL_CODIGOALMACEN);

    if(mver_numalbaran->isChecked() )
        m_list->showColumn(COL_NUMALBARAN);
    else
        m_list->hideColumn(COL_NUMALBARAN);

    if(mver_fechaalbaran->isChecked() )
        m_list->showColumn(COL_FECHAALBARAN);
    else
        m_list->hideColumn(COL_FECHAALBARAN);

    if(mver_nomcliente->isChecked() )
        m_list->showColumn(COL_NOMCLIENTE);
    else
        m_list->hideColumn(COL_NOMCLIENTE);

    if(mver_idforma_pago->isChecked() )
        m_list->showColumn(COL_IDFORMA_PAGO);
    else
        m_list->hideColumn(COL_IDFORMA_PAGO);

    if(mver_descforma_pago->isChecked() )
        m_list->showColumn(COL_DESCFORMA_PAGO);
    else
        m_list->hideColumn(COL_DESCFORMA_PAGO);

    if(mver_numfactura->isChecked() )
        m_list->showColumn(COL_NUMFACTURA);
    else
        m_list->hideColumn(COL_NUMFACTURA);

    if(mver_numnofactura->isChecked() )
        m_list->showColumn(COL_NUMNOFACTURA);
    else
        m_list->hideColumn(COL_NUMNOFACTURA);

    if(mver_idusuario->isChecked() )
        m_list->showColumn(COL_IDUSUARIO);
    else
        m_list->hideColumn(COL_IDUSUARIO);

    if(mver_idcliente->isChecked() )
        m_list->showColumn(COL_IDCLIENTE);
    else
        m_list->hideColumn(COL_IDCLIENTE);

    if(mver_idalbaran->isChecked() )
        m_list->showColumn(COL_IDALBARAN);
    else
        m_list->hideColumn(COL_IDALBARAN);

    if(mver_comentalbaran->isChecked() )
        m_list->showColumn(COL_COMENTALBARAN);
    else
        m_list->hideColumn(COL_COMENTALBARAN);

    if(mver_idalmacen->isChecked() )
        m_list->showColumn(COL_IDALMACEN);
    else
        m_list->hideColumn(COL_IDALMACEN);

    if(mver_totalalbaran->isChecked() )
        m_list->showColumn(COL_TOTALALBARAN);
    else
        m_list->hideColumn(COL_TOTALALBARAN);

    if(mver_totalbaseimp->isChecked() )
        m_list->showColumn(COL_TOTALBASEIMP);
    else
        m_list->hideColumn(COL_TOTALBASEIMP);

    if(mver_totalimpuestos->isChecked() )
        m_list->showColumn(COL_TOTALIMPUESTOS);
    else
        m_list->hideColumn(COL_TOTALIMPUESTOS);


}// end s_configurar

ClientDelivNotesList::ClientDelivNotesList(QWidget *parent, const char *name, Qt::WFlags flag, edmode editmodo)
        : ClientDelivNotesListBase(parent, name, flag) {
    companyact = NULL;
	inicializa();
    cargaconfig();
    s_configurar();
    m_modo=editmodo;
    m_idclidelivnote="";
    if (m_modo == EditMode)
        meteWindow(caption(),this);
    hideBusqueda();
}// end providerslist

ClientDelivNotesList::ClientDelivNotesList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag, edmode editmodo)
        : ClientDelivNotesListBase(parent, name, flag) {
    companyact = comp;
    m_cliente->setcompany(comp);
    m_articulo->setcompany(comp);

    inicializa();
    cargaconfig();
	s_configurar();
	presenta();
    m_modo=editmodo;
    m_idclidelivnote="";
    if (m_modo == EditMode)
        companyact->meteWindow(caption(), this);
    hideBusqueda();
    hideConfiguracion();
}// end ClientDelivNotesList

ClientDelivNotesList::~ClientDelivNotesList() {
    if (m_modo == EditMode)
        companyact->sacaWindow(this);
    guardaconfig();
}// end ~providerslist



void ClientDelivNotesList::inicializa() {
    _depura("ClientDelivNotesList::inicializa\n");
    m_list->setNumRows( 0 );
    m_list->setNumCols( 0 );
    m_list->setSelectionMode( Q3Table::SingleRow );
    m_list->setSorting( TRUE );
    m_list->setSelectionMode( Q3Table::SingleRow );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(17);
    m_list->horizontalHeader()->setLabel( COL_REFALBARAN, tr( "Referencia" ) );
    m_list->horizontalHeader()->setLabel( COL_CODIGOALMACEN, tr( "Almacén" ) );
    m_list->horizontalHeader()->setLabel( COL_NOMCLIENTE, tr( "Cliente" ) );
    m_list->horizontalHeader()->setLabel( COL_NUMALBARAN, tr( "N Albaran" ) );
    m_list->horizontalHeader()->setLabel( COL_FECHAALBARAN, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_IDFORMA_PAGO, tr( "COL_IDFORMA_PAGO" ) );
    m_list->horizontalHeader()->setLabel( COL_NUMFACTURA, tr( "N Factura" ) );
    m_list->horizontalHeader()->setLabel( COL_NUMNOFACTURA, tr( "N No Fra." ) );
    m_list->horizontalHeader()->setLabel( COL_IDUSUARIO, tr("COL_IDUSUARIO") );
    m_list->horizontalHeader()->setLabel( COL_IDCLIENTE, tr("COL_IDCLIENTE") );
    m_list->horizontalHeader()->setLabel( COL_IDALBARAN, tr("COL_IDALBARAN") );
    m_list->horizontalHeader()->setLabel( COL_COMENTALBARAN, tr("Comentario") );
    m_list->horizontalHeader()->setLabel( COL_DESCFORMA_PAGO, tr("Forma de Pago") );
    m_list->horizontalHeader()->setLabel( COL_TOTALALBARAN, tr("Total") );
    m_list->horizontalHeader()->setLabel( COL_TOTALBASEIMP, tr("Base Imponible") );
    m_list->horizontalHeader()->setLabel( COL_TOTALIMPUESTOS, tr("Impuestos") );

    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor(confpr->valor(CONF_BG_LISTALBARANESCLIENTE));
    m_list->setReadOnly(TRUE);

    _depura("End ClientDelivNotesList::inicializa");
}// end inicializa


void ClientDelivNotesList::presenta() {
    _depura("ClientDelivNotesList::presenta\n");
    cursor2 * cur= companyact->cargacursor("SELECT * FROM albaran LEFT JOIN cliente ON albaran.idcliente=cliente.idcliente LEFT JOIN almacen ON  almacen.idalmacen=albaran.idalmacen LEFT JOIN forma_pago ON albaran.idforma_pago = forma_pago.idforma_pago where 1=1 "+generarFiltro());
    m_list->setNumRows( cur->numregistros() );
    int i=0;
    while (!cur->eof()) {
        m_list->setText(i,COL_REFALBARAN,cur->valor("refalbaran"));
        m_list->setText(i,COL_NUMALBARAN,cur->valor("numalbaran"));
        m_list->setText(i,COL_FECHAALBARAN,cur->valor("fechaalbaran"));
        m_list->setText(i,COL_IDFORMA_PAGO,cur->valor("idforma_pago"));
        m_list->setText(i,COL_NUMFACTURA,cur->valor("idfactura"));
        m_list->setText(i,COL_NUMNOFACTURA,cur->valor("idnofactura"));
        m_list->setText(i,COL_IDUSUARIO,cur->valor("idtrabajador"));
        m_list->setText(i,COL_IDCLIENTE,cur->valor("idcliente"));
        m_list->setText(i,COL_NOMCLIENTE,cur->valor("nomcliente"));
        m_list->setText(i,COL_IDALBARAN,cur->valor("idalbaran"));
        m_list->setText(i,COL_COMENTALBARAN,cur->valor("comentalbaran"));
        m_list->setText(i,COL_IDALMACEN,cur->valor("idalmacen"));
        m_list->setText(i,COL_CODIGOALMACEN,cur->valor("codigoalmacen"));
        m_list->setText(i,COL_DESCFORMA_PAGO,cur->valor("descforma_pago"));

        _depura("Pintamos los totales");
        /// Calculamos el total del presupuesto y lo presentamos.
        cursor2 *cur1 = companyact->cargacursor("SELECT calctotalalbaran("+cur->valor("idalbaran")+") AS total, calcbimpalbaran("+cur->valor("idalbaran")+") AS base, calcimpuestosalbaran("+cur->valor("idalbaran")+") AS impuestos");
        m_list->setText(i,COL_TOTALALBARAN,cur1->valor("total"));
        m_list->setText(i,COL_TOTALBASEIMP, cur1->valor("base"));
        m_list->setText(i,COL_TOTALIMPUESTOS, cur1->valor("impuestos"));
        delete cur1;


        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;

    /// Hacemos el calculo del total.
    cur = companyact->cargacursor("SELECT SUM(calctotalalbaran(idalbaran)) AS total FROM albaran LEFT JOIN cliente ON albaran.idcliente=cliente.idcliente LEFT JOIN almacen ON almacen.idalmacen=albaran.idalmacen where 1=1 "+generarFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;

    _depura("End ClientDelivNotesList::presenta");
}// end presenta



void ClientDelivNotesList::s_doubleclicked(int a, int , int , const QPoint &) {
    m_idclidelivnote = m_list->text(a,COL_IDALBARAN);
    if (m_modo ==0 && m_idclidelivnote != "") {
        fprintf(stderr,"ClientDelivNotesList::s_doubleclicked\n");
        AlbaranClienteView *cDelivNote = new AlbaranClienteView(companyact,0,theApp->translate("Edicion de Albaran de Cliente", "company"));

        if (cDelivNote->cargaAlbaranCliente(m_idclidelivnote))
            return;

        companyact->m_pWorkspace->addWindow(cDelivNote);
        cDelivNote->show();
    } else {
        close();
    }// end if
}


void ClientDelivNotesList::s_edit() {
    int a = m_list->currentRow();
    if (a >= 0) {
        m_idclidelivnote = m_list->text(a,COL_IDALBARAN);
        if (m_idclidelivnote != "") {
            fprintf(stderr,"ClientDelivNotesList::s_doubleclicked\n");
            AlbaranClienteView *cDelivNote = new AlbaranClienteView(companyact,0,theApp->translate("Edicion de Albaran de Cliente", "company"));
            if (cDelivNote->cargaAlbaranCliente(m_idclidelivnote))
                return;

            companyact->m_pWorkspace->addWindow(cDelivNote);
            cDelivNote->show();
        }// end if
    }// end if
}


void ClientDelivNotesList::s_contextMenu(int, int, int button, const QPoint &poin) {
    qDebug("button = %d", button);
    if (button == 2) {
        Q3PopupMenu *popup;
        popup = new Q3PopupMenu;
        popup->insertItem(tr("Eliminar"),101);
        //popup->insertSeparator();
        int opcion = popup->exec(m_list->mapToGlobal(poin));
        switch(opcion) {
        case 101:
            s_removeClientDelivNote();
            break;
        }// end switch
        delete popup;
    }
}// end contextmenu

void ClientDelivNotesList::s_removeClientDelivNote() {
    fprintf(stderr,"Iniciamos el boton_borrar\n");
    if (m_list->currentRow() >= 0) {
        if (QMessageBox::warning( this, tr("BulmaFact - Albaranes"), tr("Desea borrar el albaran seleccionado"),tr("Si"), tr("No"),0,0,1) == 0) {
            companyact->begin();
            QString SQLQuery = "DELETE FROM lalbaran WHERE idalbaran ="+m_list->text(m_list->currentRow(),COL_IDALBARAN);
            if (companyact->ejecuta(SQLQuery)==0) {
                QString SQLQuery = "DELETE FROM dalbaran WHERE idalbaran ="+m_list->text(m_list->currentRow(),COL_IDALBARAN);
                if (companyact->ejecuta(SQLQuery)==0) {
                    QString SQLQuery = "DELETE FROM albaran WHERE idalbaran ="+m_list->text(m_list->currentRow(),COL_IDALBARAN);
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
    presenta();
}// end boton_borrar


void ClientDelivNotesList::imprimir() {
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
    cursor2 *cur = companyact->cargacursor(SQLQuery);
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
        cursor2 *cur1 = companyact->cargacursor("SELECT calctotalalbaran("+cur->valor("idalbaran")+") AS total, calcbimpalbaran("+cur->valor("idalbaran")+") AS base, calcimpuestosalbaran("+cur->valor("idalbaran")+") AS impuestos");
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


QString ClientDelivNotesList::generarFiltro() {
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
