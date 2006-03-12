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

// Implementación del listado de presupuestos.
/*
-- Entendemos que un presupuesto es una relación de materiales y trabajos cuantificada que
-- hacemos a petición de un cliente determinado
-- Numero
-- Data: Data d'emisió del presupost.
-- PersContacte: Nom de persona de contacte (si cal).
-- TelfContacte: Teléfon.
-- Venciment: Data máxima de validesa del presupost.
-- Comentaris
--  Pressupost a clients.
CREATE TABLE presupuesto (
   idpresupuesto serial PRIMARY KEY,
   numpresupuesto integer,
   fpresupuesto date,
   contactpresupuesto character varying(90),
   telpresupuesto character varying(20),
   vencpresupuesto date,
   comentpresupuesto character varying(3000),
   idusuari integer,
   
   idcliente integer REFERENCES cliente(idcliente)
);
*/


#include <QMessageBox>
#include <QCheckBox>
#include <QFile>
#include <QCheckBox>
#include <QTextStream>
#include <QMenu>

#include "configuracion.h"
#include "cobroslist.h"
#include "company.h"
#include "cobroview.h"
#include "qtable1.h"
#include "funcaux.h"


#define COL_IDCOBRO 0
#define COL_IDCLIENTE 1
#define COL_FECHACOBRO 2
#define COL_CANTCOBRO 3
#define COL_REFCOBRO 4
#define COL_PREVISIONCOBRO 5
#define COL_COMENTCOBRO 6



void CobrosList::guardaconfig() {
    QString aux = "";
    mver_idcobro->isChecked() ? aux += "1,":aux+="0,";
    mver_idcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_fechacobro->isChecked() ? aux += "1,":aux+="0,";
    mver_cantcobro->isChecked() ? aux += "1,":aux+="0,";
    mver_refcobro->isChecked() ? aux += "1,":aux+="0,";
    mver_previsioncobro->isChecked() ? aux += "1,":aux+="0,";
    mver_comentcobro->isChecked() ? aux += "1,":aux+="0,";

    QFile file( confpr->valor(CONF_DIR_USER)+"confcobroslist.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << aux << "\n";
        for (int i = 0; i < mui_list->columnCount(); i++) {
            mui_list->showColumn(i);
            stream << mui_list->columnWidth(i) << "\n";
        }// end for
        file.close();
    }// end if
}

void CobrosList::cargaconfig() {
    QFile file( confpr->valor(CONF_DIR_USER)+"confcobroslist.cfn" );
    QString line;
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        line = stream.readLine(); // line of text excluding '\n'
        for (int i = 0; i < mui_list->columnCount(); i++) {
            QString linea = stream.readLine();
            mui_list->setColumnWidth(i, linea.toInt());
        }// end for
        file.close();
    } else
        return;

    mver_idcobro->setChecked(line.at(0)=='1');
    mver_idcliente->setChecked(line.at(2)=='1');
    mver_fechacobro->setChecked(line.at(4)=='1');
    mver_cantcobro->setChecked(line.at(6)=='1');
    mver_refcobro->setChecked(line.at(8)=='1');
    mver_previsioncobro->setChecked(line.at(10)=='1');
    mver_comentcobro->setChecked(line.at(12)=='1');
}// end cargaconfig




void CobrosList::s_configurar() {
    if(mver_idcobro->isChecked() )
        mui_list->showColumn(COL_IDCOBRO);
    else
        mui_list->hideColumn(COL_IDCOBRO);

    if(mver_idcliente->isChecked() )
        mui_list->showColumn(COL_IDCLIENTE);
    else
        mui_list->hideColumn(COL_IDCLIENTE);

    if(mver_fechacobro->isChecked() )
        mui_list->showColumn(COL_FECHACOBRO);
    else
        mui_list->hideColumn(COL_FECHACOBRO);

    if(mver_cantcobro->isChecked() )
        mui_list->showColumn(COL_CANTCOBRO);
    else
        mui_list->hideColumn(COL_CANTCOBRO);

    if(mver_refcobro->isChecked() )
        mui_list->showColumn(COL_REFCOBRO);
    else
        mui_list->hideColumn(COL_REFCOBRO);

    if(mver_previsioncobro->isChecked() )
        mui_list->showColumn(COL_PREVISIONCOBRO);
    else
        mui_list->hideColumn(COL_PREVISIONCOBRO);

    if(mver_comentcobro->isChecked() )
        mui_list->showColumn(COL_COMENTCOBRO);
    else
        mui_list->hideColumn(COL_COMENTCOBRO);
}


CobrosList::CobrosList(QWidget *parent, const char *name, Qt::WFlags flag) : QWidget (parent, name, flag) {
    setupUi(this);
    inicializa();
    cargaconfig();
    s_configurar();
    m_companyact = NULL;
    m_modo=0;
    mdb_idcobro="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}

CobrosList::CobrosList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag)   : QWidget(parent, name, flag) {
    m_companyact = comp;
    setupUi(this);
    m_cliente->setcompany(comp);
    inicializa();
    cargaconfig();
    s_configurar();
    presenta();
    m_modo=0;
    mdb_idcobro="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}

CobrosList::~CobrosList() {
    m_companyact->sacaWindow(this);
    guardaconfig();
}

void CobrosList::inicializa() {
    _depura("CobrosList::inicializa()\n",0);
    mui_list->setRowCount(0);
    mui_list->setColumnCount(7);
    QStringList headers;
    headers << tr( "COL_IDCOBRO" ) << tr( "COL_IDCLIENTE" ) << tr( "COL_FECHACOBRO" ) << tr( "COL_CANTCOBRO" ) << tr( "COL_REFCOBRO" ) << tr( "COL_PREVISIONCOBRO" ) << tr( "COL_COMENTCOBRO" );
    mui_list->setHorizontalHeaderLabels (headers);
    _depura("end CobrosList::inicializa()\n",0);
}



void CobrosList::presenta() {
    _depura("CobrosList::presenta()\n",0);
	inicializa();
    if (m_companyact != NULL ) {
        cursor2 * cur= m_companyact->cargacursor("SELECT * FROM cobro where 1=1"+generaFiltro());
        mui_list->setRowCount( cur->numregistros() );
        int i=0;
        while (!cur->eof()) {
            mui_list->setText(i,COL_IDCOBRO,cur->valor("idcobro"));
            mui_list->setText(i,COL_IDCLIENTE,cur->valor("idcliente"));
            mui_list->setText(i,COL_FECHACOBRO,cur->valor("fechacobro"));
            mui_list->setText(i,COL_CANTCOBRO,cur->valor("cantcobro"));
            mui_list->setText(i,COL_REFCOBRO,cur->valor("refcobro"));
            mui_list->setText(i,COL_PREVISIONCOBRO,cur->valor("previsioncobro"));
            mui_list->setText(i,COL_COMENTCOBRO,cur->valor("comentcobro"));
            i++;
            cur->siguienteregistro();
        }// end while
        delete cur;
        /// Hacemos el calculo del total.
        cur = m_companyact->cargacursor("SELECT SUM(cantcobro) AS total FROM cobro where 1=1"+generaFiltro());
        m_total->setText(cur->valor("total"));
        delete cur;
    }// end if
	s_configurar();
    _depura("END CobrosList::presenta()\n",0);
}



QString CobrosList::generaFiltro() {
    /// Tratamiento de los filtros.
    fprintf(stderr,"Tratamos el filtro \n");
    QString filtro="";
    if (m_filtro->text() != "") {
        filtro = " AND ( desccobro LIKE '%"+m_filtro->text()+"%' ";
    } else {
        filtro = "";
    }// end if
    if (m_cliente->idcliente() != "") {
        filtro += " AND cobro.idcliente="+m_cliente->idcliente();
    }// end if
    if (!m_procesados->isChecked() ) {
        filtro += " AND NOT previsioncobro";
    }// end if

    if (m_fechain->text() != "")
        filtro += " AND fechacobro >= '"+m_fechain->text()+"' ";

    if (m_fechafin->text() != "")
        filtro += " AND fechacobro <= '"+m_fechafin->text()+"' ";

    //    filtro += " ORDER BY idcobro";
    return (filtro);
}



void CobrosList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >=0 )
        on_mui_list_cellDoubleClicked(a,0);
    else
        _depura("Debe seleccionar una linea",2);
}







void CobrosList::on_mui_crear_clicked() {
    _depura("CobrosList::on_mui_crear_clicked",0);
    CobroView *bud = new CobroView(m_companyact,NULL,theApp->translate("Edicion de Cobros", "company"));
    bud->show();
    bud->setidcliente(m_cliente->idcliente());
    bud->pintar();
}


void CobrosList::imprimir() {
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"cobros.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"cobros.rml";
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
    /// ------------------------------------------------
    if(mver_idcobro->isChecked() )
        fitxersortidatxt += "	<td>Id.</td>";
    if(mver_idcliente->isChecked() )
        fitxersortidatxt += "	<td>Id. cliente</td>";
    if(mver_fechacobro->isChecked() )
        fitxersortidatxt += "	<td>Fecha</td>";
    if(mver_cantcobro->isChecked() )
        fitxersortidatxt += "	<td>Cantidad</td>";
    if(mver_refcobro->isChecked() )
        fitxersortidatxt += "	<td>Referencia</td>";
    if(mver_previsioncobro->isChecked() )
        fitxersortidatxt += "	<td>Prevision</td>";
    if(mver_comentcobro->isChecked() )
        fitxersortidatxt += "	<td>Comentarios</td>";
    /// ------------------------------------------------
    fitxersortidatxt += "</tr>";

    cursor2 * cur= m_companyact->cargacursor("SELECT * FROM cobro where 1=1"+generaFiltro());
    while(!cur->eof()) {
        fitxersortidatxt += "<tr>";
        /// -----------------------------
        if(mver_idcobro->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idcobro"))+"</td>";
        if(mver_idcliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idcliente"))+"</td>";
        if(mver_fechacobro->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("fechacobro"))+"</td>";
        if(mver_cantcobro->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("cantcobro"))+"</td>";
        if(mver_refcobro->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("refcobro"))+"</td>";
        if(mver_previsioncobro->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("previsioncobro"))+"</td>";
        if(mver_comentcobro->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("comentcobro"))+"</td>";
        /// -----------------------------
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
    // Crea el pdf  y lo muestra.
    _depura("Vamos a imprimir e listado de cobros",0);
    invocaPDF("cobros");
}


void CobrosList::on_mui_borrar_clicked() {
    int a = mui_list->currentRow();
    mdb_idcobro = mui_list->item(a,COL_IDCOBRO)->text();
    if (m_modo ==0 && mdb_idcobro != "") {
        CobroView *bud = new CobroView(m_companyact,NULL,theApp->translate("Edicion de Presupuestos", "company"));
        bud->cargar(mdb_idcobro);
        bud->borrar();
    }// end if
    presenta();
}


void CobrosList::on_mui_list_cellDoubleClicked(int a, int ) {
    mdb_idcobro = mui_list->item(a,COL_IDCOBRO)->text();
    if (m_modo ==0 && mdb_idcobro != "") {
        CobroView *bud = new CobroView(m_companyact,NULL,theApp->translate("Edicion de Cobros", "company"));
        bud->cargar(mdb_idcobro);
        bud->show();
    } else {
        close();
    }// end if
}

void CobrosList::on_mui_list_customContextMenuRequested(const QPoint &) {
    _depura("PagosList::on_mui_list_customContextMenuRequested",0);
    int a = mui_list->currentRow();
    if ( a < 0)
        return;
    QMenu *popup = new QMenu(this);
    QAction *edit = popup->addAction(tr("Editar Cobro"));
    QAction *del = popup->addAction(tr("Borrar Cobro"));
    QAction *opcion = popup->exec(QCursor::pos());
    if (opcion == del)
        on_mui_borrar_clicked();
    if (opcion == edit)
	on_mui_editar_clicked();
    delete popup;
}



