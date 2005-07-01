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
-- Data: Data d'emisiï¿½del presupost.
-- PersContacte: Nom de persona de contacte (si cal).
-- TelfContacte: Telï¿½on.
-- Venciment: Data mï¿½ima de validesa del presupost.
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
#include "cobroslist.h"
#include "company.h"
#include "cobroview.h"
#include <qtable.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qcheckbox.h>
#include <qfile.h>
#include <qcheckbox.h>

#include "configuracion.h"

#define COL_IDCOBRO 0
#define COL_IDCLIENTE 1
#define COL_FECHACOBRO 2
#define COL_CANTCOBRO 3
#define COL_REFCOBRO 4
#define COL_PREVISIONCOBRO 5
#define COL_COMENTCOBRO 6
    
    




void CobrosList::s_configurar() {
}


CobrosList::CobrosList(QWidget *parent, const char *name, int flag)
: CobrosListBase(parent, name, flag) {
    companyact = NULL;
    m_modo=0;
    m_idcobro="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}// end providerslist

CobrosList::CobrosList(company *comp, QWidget *parent, const char *name, int flag)
: CobrosListBase(parent, name, flag) {
    companyact = comp;
    m_cliente->setcompany(comp);
    inicializa();
    m_modo=0;
    m_idcobro="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}// end providerslist

CobrosList::~CobrosList() {
    companyact->sacaWindow(this);
}// end ~providerslist

void CobrosList::inicializa() {
    fprintf(stderr,"CobrosList::inicializa()\n");
    m_list->setNumRows( 0 );
    m_list->setNumCols( 0 );
    m_list->setSelectionMode( QTable::SingleRow );
    m_list->setSorting( FALSE );
    m_list->setSelectionMode( QTable::SingleRow );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(7);
    
    m_list->horizontalHeader()->setLabel( COL_IDCOBRO, tr( "COL_IDCOBRO" ) );
    m_list->horizontalHeader()->setLabel( COL_IDCLIENTE, tr( "COL_IDCLIENTE" ) );
    m_list->horizontalHeader()->setLabel( COL_FECHACOBRO, tr( "COL_FECHACOBRO" ) );
    m_list->horizontalHeader()->setLabel( COL_CANTCOBRO, tr( "COL_CANTCOBRO" ) );
    m_list->horizontalHeader()->setLabel( COL_REFCOBRO, tr( "COL_REFCOBRO" ) );
    m_list->horizontalHeader()->setLabel( COL_PREVISIONCOBRO, tr( "COL_PREVISIONCOBRO" ) );
    m_list->horizontalHeader()->setLabel( COL_COMENTCOBRO, tr( "COL_COMENTCOBRO" ) );
    
    m_list->setColumnWidth(COL_IDCOBRO,75);
    m_list->setColumnWidth(COL_IDCLIENTE,75);
    m_list->setColumnWidth(COL_FECHACOBRO,100);
    m_list->setColumnWidth(COL_CANTCOBRO,100);
    m_list->setColumnWidth(COL_REFCOBRO,200);
    m_list->setColumnWidth(COL_PREVISIONCOBRO,150);
    m_list->setColumnWidth(COL_COMENTCOBRO,300);

    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor("#EEFFFF");
    m_list->setReadOnly(TRUE);



    if (companyact != NULL ) {
        cursor2 * cur= companyact->cargacursor("SELECT * FROM cobro where 1=1"+generaFiltro());
        m_list->setNumRows( cur->numregistros() );
        int i=0;
        while (!cur->eof()) {
            m_list->setText(i,COL_IDCOBRO,cur->valor("idcobro"));
            m_list->setText(i,COL_IDCLIENTE,cur->valor("idcliente"));
            m_list->setText(i,COL_FECHACOBRO,cur->valor("fechacobro"));
            m_list->setText(i,COL_CANTCOBRO,cur->valor("cantcobro"));
            m_list->setText(i,COL_REFCOBRO,cur->valor("refcobro"));
            m_list->setText(i,COL_PREVISIONCOBRO,cur->valor("previsioncobro"));
            m_list->setText(i,COL_COMENTCOBRO,cur->valor("comentcobro"));
            i++;
            cur->siguienteregistro();
        }// end while
        delete cur;
    }// end if
    s_configurar();
    fprintf(stderr,"end CobrosList::inicializa()\n");
}// end inicializa



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
    filtro += " ORDER BY idcobro";
    return (filtro);
}// end generaFiltro



void CobrosList::s_editar() {
    int a = m_list->currentRow();
    m_idcobro = m_list->text(a,COL_IDCOBRO);
    if (m_modo ==0 && m_idcobro != "") {
        CobroView *bud = new CobroView(companyact,NULL,theApp->translate("Edicion de Presupuestos", "company"));
        bud->cargaCobro(m_idcobro);
        bud->show();
    } else {
        close();
    }// end if
}

void CobrosList::doubleclicked(int a, int , int , const QPoint &) {
    m_idcobro = m_list->text(a,COL_IDCOBRO);
    if (m_modo ==0 && m_idcobro != "") {
        CobroView *bud = new CobroView(companyact,NULL,theApp->translate("Edicion de Cobros", "company"));
        bud->cargaCobro(m_idcobro);
        bud->show();
    } else {
        close();
    }// end if
}


void CobrosList::s_contextMenu(int, int, int button, const QPoint &poin) {
    qDebug("button = %d", button);
    if (button == 2) {
        QPopupMenu *popup;
        popup = new QPopupMenu;
        popup->insertItem(tr("Borrar Cobro"),101);
        //popup->insertSeparator();
        int opcion = popup->exec(m_list->mapToGlobal(poin));
        switch(opcion) {
        case 101:
            s_borrarCobro();
            break;
        }// end switch
        delete popup;
    }
}// end contextmenu


void CobrosList::s_nuevoCobro() {
    fprintf(stderr,"Iniciamos el boton_crear\n");
    CobroView *bud = new CobroView(companyact,NULL,theApp->translate("Edicion de Cobros", "company"));
    bud->show();
    bud->setidcliente(m_cliente->idcliente());
    bud->pintaCobro();
}// end boton_crear


void CobrosList::imprimir() {
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
    // Línea de totales del presupuesto

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


void CobrosList::s_borrarCobro() {
    int a = m_list->currentRow();
    m_idcobro = m_list->text(a,COL_IDCOBRO);
    if (m_modo ==0 && m_idcobro != "") {
        CobroView *bud = new CobroView(companyact,NULL,theApp->translate("Edicion de Presupuestos", "company"));
        bud->cargaCobro(m_idcobro);
        bud->borraCobro();
    }// end if
    inicializa();
}// end boton_borrar
