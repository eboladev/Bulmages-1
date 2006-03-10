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
/*
-- Entendemos que un presupuesto es una relaci� de materiales y trabajos cuantificada que
-- hacemos a petici� de un proveedor determinado
-- Numero
-- Data: Data d'emisi�del presupost.
-- PersContacte: Nom de persona de contacte (si cal).
-- TelfContacte: Tel�on.
-- Venciment: Data m�ima de validesa del presupost.
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
   
   idproveedor integer REFERENCES proveedor(idproveedor)
);
*/
#include "pagoslist.h"
#include "company.h"
#include "pagoview.h"
#include "qtable1.h"
#include "funcaux.h"

#include <QMessageBox>
#include <Q3PopupMenu>
#include <QCheckBox>
#include <QFile>
#include <QCheckBox>
#include <QMenu>
//Added by qt3to4:
#include <QTextStream>

#include "configuracion.h"

#define COL_IDPAGO 0
#define COL_IDPROVEEDOR 1
#define COL_FECHAPAGO 2
#define COL_CANTPAGO 3
#define COL_REFPAGO 4
#define COL_PREVISIONPAGO 5
#define COL_COMENTPAGO 6

void PagosList::guardaconfig() {
    QString aux = "";
    mver_idpago->isChecked() ? aux += "1,":aux+="0,";
    mver_idproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_fechapago->isChecked() ? aux += "1,":aux+="0,";
    mver_cantpago->isChecked() ? aux += "1,":aux+="0,";
    mver_refpago->isChecked() ? aux += "1,":aux+="0,";
    mver_previsionpago->isChecked() ? aux += "1,":aux+="0,";
    mver_comentpago->isChecked() ? aux += "1,":aux+="0,";

    QFile file( confpr->valor(CONF_DIR_USER)+"confpagoslist.cfn" );
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

void PagosList::cargaconfig() {
    QFile file( confpr->valor(CONF_DIR_USER)+"confpagoslist.cfn" );
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

    mver_idpago->setChecked(line.at(0)=='1');
    mver_idproveedor->setChecked(line.at(2)=='1');
    mver_fechapago->setChecked(line.at(4)=='1');
    mver_cantpago->setChecked(line.at(6)=='1');
    mver_refpago->setChecked(line.at(8)=='1');
    mver_previsionpago->setChecked(line.at(10)=='1');
    mver_comentpago->setChecked(line.at(12)=='1');
}// end cargaconfig


void PagosList::s_configurar() {
	_depura("PagosList::s_configurar",0);
    if(mver_idpago->isChecked() )
        m_list->showColumn(COL_IDPAGO);
    else
        m_list->hideColumn(COL_IDPAGO);

    if(mver_idproveedor->isChecked() )
        m_list->showColumn(COL_IDPROVEEDOR);
    else
        m_list->hideColumn(COL_IDPROVEEDOR);

    if(mver_fechapago->isChecked() )
        m_list->showColumn(COL_FECHAPAGO);
    else
        m_list->hideColumn(COL_FECHAPAGO);

    if(mver_cantpago->isChecked() )
        m_list->showColumn(COL_CANTPAGO);
    else
        m_list->hideColumn(COL_CANTPAGO);

    if(mver_refpago->isChecked() )
        m_list->showColumn(COL_REFPAGO);
    else
        m_list->hideColumn(COL_REFPAGO);

    if(mver_previsionpago->isChecked() )
        m_list->showColumn(COL_PREVISIONPAGO);
    else
        m_list->hideColumn(COL_PREVISIONPAGO);

    if(mver_comentpago->isChecked() )
        m_list->showColumn(COL_COMENTPAGO);
    else
        m_list->hideColumn(COL_COMENTPAGO);

}


PagosList::PagosList(QWidget *parent, const char *name, Qt::WFlags flag)
        : QWidget(parent, name, flag) {

    companyact = NULL;

    setupUi(this);
    m_modo=0;
    m_idpago="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
    inicializa();
    cargaconfig();
}// end providerslist


PagosList::PagosList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag)
        : QWidget(parent, name, flag) {
    companyact = comp;

    setupUi(this);

    m_proveedor->setcompany(comp);

    inicializa();
    cargaconfig();
    presenta();
    m_modo=0;
    m_idpago="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}// end providerslist



PagosList::~PagosList() {
    companyact->sacaWindow(this);
    guardaconfig();
}// end ~providerslist



void PagosList::inicializa() {
    fprintf(stderr,"PagosList::inicializa()\n");
    m_list->setNumRows( 0 );
    m_list->setNumCols( 0 );
    m_list->setSelectionMode( Q3Table::SingleRow );
    m_list->setSorting( TRUE );
    m_list->setSelectionMode( Q3Table::SingleRow );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(7);

    m_list->horizontalHeader()->setLabel( COL_IDPAGO, tr( "COL_IDPAGO" ) );
    m_list->horizontalHeader()->setLabel( COL_IDPROVEEDOR, tr( "COL_IDPROVEEDOR" ) );
    m_list->horizontalHeader()->setLabel( COL_FECHAPAGO, tr( "COL_FECHAPAGO" ) );
    m_list->horizontalHeader()->setLabel( COL_CANTPAGO, tr( "COL_CANTPAGO" ) );
    m_list->horizontalHeader()->setLabel( COL_REFPAGO, tr( "COL_REFPAGO" ) );
    m_list->horizontalHeader()->setLabel( COL_PREVISIONPAGO, tr( "COL_PREVISIONPAGO" ) );
    m_list->horizontalHeader()->setLabel( COL_COMENTPAGO, tr( "COL_COMENTPAGO" ) );

    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor("#EEFFFF");
    m_list->setReadOnly(TRUE);



    if (companyact != NULL ) {
        cursor2 * cur= companyact->cargacursor("SELECT * FROM pago where 1=1"+generaFiltro());
        m_list->setNumRows( cur->numregistros() );
        int i=0;
        while (!cur->eof()) {
            m_list->setText(i,COL_IDPAGO,cur->valor("idpago"));
            m_list->setText(i,COL_IDPROVEEDOR,cur->valor("idproveedor"));
            m_list->setText(i,COL_FECHAPAGO,cur->valor("fechapago"));
            m_list->setText(i,COL_CANTPAGO,cur->valor("cantpago"));
            m_list->setText(i,COL_REFPAGO,cur->valor("refpago"));
            m_list->setText(i,COL_PREVISIONPAGO,cur->valor("previsionpago"));
            m_list->setText(i,COL_COMENTPAGO,cur->valor("comentpago"));
            i++;
            cur->siguienteregistro();
        }// end while
        delete cur;

        /// Hacemos el calculo del total.
        cur = companyact->cargacursor("SELECT SUM(cantpago) AS total FROM pago where 1=1"+generaFiltro());
        m_total->setText(cur->valor("total"));
        delete cur;

    }// end if

    fprintf(stderr,"end PagosList::inicializa()\n");
}// end inicializa



void PagosList::presenta() {
    _depura("PagosList::presenta()\n",0);
	s_filtrar();
	

    if (companyact != NULL ) {
        cursor2 * cur= companyact->cargacursor("SELECT * FROM pago where 1=1"+generaFiltro());
        m_list->setNumRows( cur->numregistros() );
        int i=0;
        while (!cur->eof()) {
            m_list->setText(i,COL_IDPAGO,cur->valor("idpago"));
            m_list->setText(i,COL_IDPROVEEDOR,cur->valor("idproveedor"));
            m_list->setText(i,COL_FECHAPAGO,cur->valor("fechapago"));
            m_list->setText(i,COL_CANTPAGO,cur->valor("cantpago"));
            m_list->setText(i,COL_REFPAGO,cur->valor("refpago"));
            m_list->setText(i,COL_PREVISIONPAGO,cur->valor("previsionpago"));
            m_list->setText(i,COL_COMENTPAGO,cur->valor("comentpago"));
            i++;
            cur->siguienteregistro();
        }// end while
        delete cur;

        /// Hacemos el calculo del total.
        cur = companyact->cargacursor("SELECT SUM(cantpago) AS total FROM pago where 1=1"+generaFiltro());
        m_total->setText(cur->valor("total"));
        delete cur;

    }// end if
	s_configurar();

    _depura("END PagosList::presenta()\n",0);
}// end presenta



QString PagosList::generaFiltro() {
    /// Tratamiento de los filtros.
    fprintf(stderr,"Tratamos el filtro \n");
    QString filtro="";
    if (m_filtro->text() != "") {
        filtro = " AND ( descpago LIKE '%"+m_filtro->text()+"%' ";
    } else {
        filtro = "";
    }// end if
    if (m_proveedor->idproveedor() != "") {
        filtro += " AND pago.idproveedor="+m_proveedor->idproveedor();
    }// end if
    if (!m_procesados->isChecked() ) {
        filtro += " AND NOT previsionpago";
    }// end if

    if (m_fechain->text() != "")
        filtro += " AND fechapago >= '"+m_fechain->text()+"' ";

    if (m_fechafin->text() != "")
        filtro += " AND fechapago <= '"+m_fechafin->text()+"' ";

    return (filtro);
}// end generaFiltro



void PagosList::on_mui_editar_clicked() {
    int a = m_list->currentRow();
	if (a >=0 ) 
    	on_m_list_doubleClicked(a,0,0, QPoint());
	else
	_depura("Debe seleccionar una linea",2);
}

void PagosList::on_m_list_doubleClicked(int a, int , int , const QPoint &) {
    m_idpago = m_list->text(a,COL_IDPAGO);
    if (m_modo ==0 && m_idpago != "") {
        PagoView *bud = new PagoView(companyact,NULL,theApp->translate("Edicion de Pagos", "company"));
        bud->cargaPago(m_idpago);
        bud->show();
    } else {
        close();
    }// end if
}


void PagosList::on_m_list_contextMenuRequested(int, int, const QPoint &poin) {
        QMenu *popup = new QMenu(this);
	QAction *del = popup->addAction(tr("Borrar Pago"));
        QAction *opcion = popup->exec(QCursor::pos());
        if (opcion == del) 
            on_mui_borrar_clicked();
        delete popup;
}// end contextmenu


void PagosList::on_mui_crear_clicked() {
    fprintf(stderr,"Iniciamos el boton_crear\n");
    PagoView *bud = new PagoView(companyact,NULL,theApp->translate("Edicion de Pagos", "company"));
    bud->show();
    bud->setidproveedor(m_proveedor->idproveedor());
    bud->pintaPago();
}// end boton_crear


void PagosList::imprimir() {
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"pagos.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"pagos.rml";
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
    if(mver_idpago->isChecked() )
        fitxersortidatxt += "	<td>Id.</td>";
    if(mver_idproveedor->isChecked() )
        fitxersortidatxt += "	<td>Id. Proveedor</td>";
    if(mver_fechapago->isChecked() )
        fitxersortidatxt += "	<td>Fecha</td>";
    if(mver_cantpago->isChecked() )
        fitxersortidatxt += "	<td>Cantidad</td>";
    if(mver_refpago->isChecked() )
        fitxersortidatxt += "	<td>Referencia</td>";
    if(mver_previsionpago->isChecked() )
        fitxersortidatxt += "	<td>Prevision</td>";
    if(mver_comentpago->isChecked() )
        fitxersortidatxt += "	<td>Comentarios</td>";
    /// ------------------------------------------------
    fitxersortidatxt += "</tr>";

    cursor2 * cur= companyact->cargacursor("SELECT * FROM pago where 1=1"+generaFiltro());
    while(!cur->eof()) {
        fitxersortidatxt += "<tr>";
        /// -----------------------------
        if(mver_idpago->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idpago"))+"</td>";
        if(mver_idproveedor->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idproveedor"))+"</td>";
        if(mver_fechapago->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("fechapago"))+"</td>";
        if(mver_cantpago->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("cantpago"))+"</td>";
        if(mver_refpago->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("refpago"))+"</td>";
        if(mver_previsionpago->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("previsionpago"))+"</td>";
        if(mver_comentpago->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("comentpago"))+"</td>";
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
    _depura("Vamos a imprimir e listado de pagos",0);
    invocaPDF("pagos");
}// end imprimir


void PagosList::on_mui_borrar_clicked() {
    int a = m_list->currentRow();
    m_idpago = m_list->text(a,COL_IDPAGO);
    if (m_modo ==0 && m_idpago != "") {
        PagoView *bud = new PagoView(companyact,NULL,theApp->translate("Edicion de Presupuestos", "company"));
        bud->cargaPago(m_idpago);
        bud->borraPago();
    }// end if
    presenta();
}// end boton_borrar
