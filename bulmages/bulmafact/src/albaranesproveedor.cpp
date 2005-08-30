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

// Implementación del listado de albaranps.
/*
-- Entendemos que un albaranp es una relación de materiales y trabajos cuantificada que
-- hacemos a petición de un cliente determinado
-- Numero
-- Data: Data d'emisió del presupost.
-- PersContacte: Nom de persona de contacte (si cal).
-- TelfContacte: Teléfon.
-- Venciment: Data máxima de validesa del presupost.
-- Comentaris
--  Pressupost a clients.
CREATE TABLE albaranp (
   idalbaranp serial PRIMARY KEY,
   numalbaranp integer,
   falbaranp date,
   contactalbaranp character varying(90),
   telalbaranp character varying(20),
   vencalbaranp date,
   comentalbaranp character varying(3000),
   idusuari integer,
   
   idcliente integer REFERENCES cliente(idcliente)
);
*/
#include "albaranesproveedor.h"
#include "albaranproveedorview.h"
#include "company.h"
#include "budget.h"
#include "qtable1.h"
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qcheckbox.h>
#include <qfile.h>
#include <qcheckbox.h>

#include "configuracion.h"


#define COL_IDALBARANP 0
#define COL_NUMALBARANP 1
#define COL_DESCALBARANP 2
#define COL_REFALBARANP 3
#define COL_FECHAALBARANP 4
#define COL_LOGINUSUARIO 5
#define COL_COMENTALBARANP 6
#define COL_PROCESADOALBARANP 7
#define COL_IDPROVEEDOR 8
#define COL_IDFORMA_PAGO 9
#define COL_IDALMACEN 10
#define COL_NOMPROVEEDOR 11
#define COL_NOMALMACEN 12
#define COL_DESCFORMA_PAGO 13



void AlbaranesProveedor::s_configurar() {

    if(mver_idalbaranp->isChecked() )
        m_list->showColumn(COL_IDALBARANP);
    else
        m_list->hideColumn(COL_IDALBARANP);

    if(mver_numalbaranp->isChecked() )
        m_list->showColumn(COL_NUMALBARANP);
    else
        m_list->hideColumn(COL_NUMALBARANP);

    if(mver_descalbaranp->isChecked() )
        m_list->showColumn(COL_DESCALBARANP);
    else
        m_list->hideColumn(COL_DESCALBARANP);
	
    if(mver_refalbaranp->isChecked() )
        m_list->showColumn(COL_REFALBARANP);
    else
        m_list->hideColumn(COL_REFALBARANP);

    if(mver_fechaalbaranp->isChecked() )
        m_list->showColumn(COL_FECHAALBARANP);
    else
        m_list->hideColumn(COL_FECHAALBARANP);

    if(mver_loginusuario->isChecked() )
        m_list->showColumn(COL_LOGINUSUARIO);
    else
        m_list->hideColumn(COL_LOGINUSUARIO);

    if(mver_comentalbaranp->isChecked() )
        m_list->showColumn(COL_COMENTALBARANP);
    else
        m_list->hideColumn(COL_COMENTALBARANP);

    if(mver_procesadoalbaranp->isChecked() )
        m_list->showColumn(COL_PROCESADOALBARANP);
    else
        m_list->hideColumn(COL_PROCESADOALBARANP);
	
    if(mver_idproveedor->isChecked() )
        m_list->showColumn(COL_IDPROVEEDOR);
    else
        m_list->hideColumn(COL_IDPROVEEDOR);
	
    if(mver_idforma_pago->isChecked() )
        m_list->showColumn(COL_IDFORMA_PAGO);
    else
        m_list->hideColumn(COL_IDFORMA_PAGO);

    if(mver_idalmacen->isChecked() )
        m_list->showColumn(COL_IDALMACEN);
    else
        m_list->hideColumn(COL_IDALMACEN);

    if(mver_nomproveedor->isChecked() )
        m_list->showColumn(COL_NOMPROVEEDOR);
    else
        m_list->hideColumn(COL_NOMPROVEEDOR);

    if(mver_nomalmacen->isChecked() )
        m_list->showColumn(COL_NOMALMACEN);
    else
        m_list->hideColumn(COL_NOMALMACEN);
	
    if(mver_descforma_pago->isChecked() )
        m_list->showColumn(COL_DESCFORMA_PAGO);
    else
        m_list->hideColumn(COL_DESCFORMA_PAGO);

    if (confpr->valor(CONF_MOSTRAR_ALMACEN)!="YES") {
        m_list->hideColumn(COL_NOMALMACEN);
    }// end if
}


AlbaranesProveedor::AlbaranesProveedor(QWidget *parent, const char *name, int flag)
: AlbaranesProveedorBase(parent, name, flag) {
    companyact = NULL;
    m_modo=0;
    m_idalbaranp="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}// end providerslist

AlbaranesProveedor::AlbaranesProveedor(company *comp, QWidget *parent, const char *name, int flag)
: AlbaranesProveedorBase(parent, name, flag) {
    companyact = comp;
    m_proveedor->setcompany(comp);
    m_articulo->setcompany(comp);
    inicializa();
    m_modo=0;
    m_idalbaranp="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}// end providerslist

AlbaranesProveedor::~AlbaranesProveedor() {
    companyact->refreshAlbaranesProveedor();
    companyact->sacaWindow(this);
}// end ~providerslist

void AlbaranesProveedor::inicializa() {
    fprintf(stderr,"AlbaranesProveedor::inicializa()\n");
    m_list->setNumRows( 0 );
    m_list->setNumCols( 0 );
    m_list->setSelectionMode( QTable::SingleRow );
    m_list->setSorting( TRUE );
    m_list->setSelectionMode( QTable::SingleRow );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(14);
       
    m_list->horizontalHeader()->setLabel( COL_IDALBARANP, tr( "COL_IDALBARANP" ) );
    m_list->horizontalHeader()->setLabel( COL_NUMALBARANP, tr( "COL_NUMALBARANP" ) );
    m_list->horizontalHeader()->setLabel( COL_DESCALBARANP, tr( "COL_DESCALBARANP" ) );
    m_list->horizontalHeader()->setLabel( COL_REFALBARANP, tr( "COL_REFALBARANP" ) );
    m_list->horizontalHeader()->setLabel( COL_FECHAALBARANP, tr( "COL_FECHAALBARANP" ) );
    m_list->horizontalHeader()->setLabel( COL_LOGINUSUARIO, tr( "COL_LOGINUSUARIO" ) );
    m_list->horizontalHeader()->setLabel( COL_COMENTALBARANP, tr( "COL_COMENTALBARANP" ) );
    m_list->horizontalHeader()->setLabel( COL_PROCESADOALBARANP, tr( "COL_PROCESADOALBARANP" ) );
    m_list->horizontalHeader()->setLabel( COL_IDPROVEEDOR, tr( "COL_IDPROVEEDOR" ) );
    m_list->horizontalHeader()->setLabel( COL_IDFORMA_PAGO, tr("COL_IDFORMA_PAGO") );
    m_list->horizontalHeader()->setLabel( COL_IDALMACEN, tr("COL_IDALMACEN") );
    m_list->horizontalHeader()->setLabel( COL_NOMPROVEEDOR, tr("COL_NOMPROVEEDOR") );
    m_list->horizontalHeader()->setLabel( COL_NOMALMACEN, tr("COL_NOMALMACEN") );
    m_list->horizontalHeader()->setLabel( COL_DESCFORMA_PAGO, tr("COL_DESCFORMA_PAGO") );
    
    m_list->setColumnWidth(COL_IDALBARANP,75);
    m_list->setColumnWidth(COL_NUMALBARANP,75);
    m_list->setColumnWidth(COL_DESCALBARANP,100);
    m_list->setColumnWidth(COL_REFALBARANP,100);
    m_list->setColumnWidth(COL_FECHAALBARANP,200);
    m_list->setColumnWidth(COL_LOGINUSUARIO,150);
    m_list->setColumnWidth(COL_COMENTALBARANP,300);
    m_list->setColumnWidth(COL_PROCESADOALBARANP, 300);
    m_list->setColumnWidth(COL_IDPROVEEDOR,100);
    m_list->setColumnWidth(COL_IDFORMA_PAGO,75);
    m_list->setColumnWidth(COL_IDALMACEN,75);
    m_list->setColumnWidth(COL_NOMPROVEEDOR,75);
    m_list->setColumnWidth(COL_NOMALMACEN,200);
    m_list->setColumnWidth(COL_DESCFORMA_PAGO,75);



    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuración que es global.
    m_list->setPaletteBackgroundColor("#EEFFFF");
    m_list->setReadOnly(TRUE);



    if (companyact != NULL ) {
        cursor2 * cur= companyact->cargacursor("SELECT * FROM albaranp LEFT JOIN proveedor ON albaranp.idproveedor = proveedor.idproveedor LEFT JOIN almacen ON   albaranp.idalmacen=almacen.idalmacen WHERE 1=1 "+generaFiltro());
        m_list->setNumRows( cur->numregistros() );
        int i=0;
        while (!cur->eof()) {	
            m_list->setText(i,COL_IDALBARANP,cur->valor("idalbaranp"));
            m_list->setText(i,COL_NUMALBARANP,cur->valor("numalbaranp"));
            m_list->setText(i,COL_DESCALBARANP,cur->valor("descalbaranp"));
            m_list->setText(i,COL_REFALBARANP,cur->valor("refalbaranp"));
            m_list->setText(i,COL_FECHAALBARANP,cur->valor("fechaalbaranp"));
            m_list->setText(i,COL_LOGINUSUARIO,cur->valor("loginusuario"));
            m_list->setText(i,COL_COMENTALBARANP,cur->valor("comentalbaranp"));
            m_list->setText(i,COL_PROCESADOALBARANP,cur->valor("procesadoalbaranp"));
            m_list->setText(i,COL_IDPROVEEDOR,cur->valor("idproveedor"));
            m_list->setText(i,COL_IDFORMA_PAGO,cur->valor("idforma_pago"));
            m_list->setText(i,COL_IDALMACEN,cur->valor("idalmacen"));
            m_list->setText(i,COL_NOMPROVEEDOR,cur->valor("nomproveedor"));
            m_list->setText(i,COL_NOMALMACEN,cur->valor("nomalmacen"));
            m_list->setText(i,COL_DESCFORMA_PAGO,cur->valor("descforma_pago"));
            i++;
            cur->siguienteregistro();
        }// end while
        delete cur;
    }// end if
    s_configurar();
    fprintf(stderr,"end AlbaranesProveedor::inicializa()\n");
}// end inicializa



QString AlbaranesProveedor::generaFiltro() {
    /// Tratamiento de los filtros.
    fprintf(stderr,"Tratamos el filtro \n");
    QString filtro="";
    if (m_filtro->text() != "") {
        filtro = " AND ( descalbaranp LIKE '%"+m_filtro->text()+"%' ";
        filtro +=" OR nomproveedor LIKE '%"+m_filtro->text()+"%') ";
    } else {
        filtro = "";
    }// end if
    if (m_proveedor->idproveedor() != "") {
        filtro += " AND albaranp.idproveedor="+m_proveedor->idproveedor();
    }// end if
    if (!m_procesados->isChecked() ) {
        filtro += " AND NOT procesadoalbaranp";
    }// end if
    if (m_articulo->idarticulo() != "") {
        filtro += " AND idalbaranp IN (SELECT DISTINCT idalbaranp FROM lalbaranp WHERE idarticulo='"+m_articulo->idarticulo()+"')";
    }// end if
    filtro += " ORDER BY idalbaranp";
    return (filtro);
}// end generaFiltro



void AlbaranesProveedor::s_editar() {
    int a = m_list->currentRow();
    m_idalbaranp = m_list->text(a,COL_IDALBARANP);
    if (m_modo ==0 && m_idalbaranp != "") {
        AlbaranProveedorView *bud = new AlbaranProveedorView(companyact,companyact->m_pWorkspace,theApp->translate("Edición de Albaranes de Proveedores", "company"));
        bud->cargaAlbaranProveedor(m_idalbaranp);
        bud->show();
    } else {
        close();
    }// end if
}

void AlbaranesProveedor::doubleclicked(int a, int , int , const QPoint &) {
    m_idalbaranp = m_list->text(a,COL_IDALBARANP);
    if (m_modo ==0 && m_idalbaranp != "") {
        AlbaranProveedorView *bud = new AlbaranProveedorView(companyact,companyact->m_pWorkspace,theApp->translate("Edición de Presupuestos", "company"));
        bud->cargaAlbaranProveedor(m_idalbaranp);
        bud->show();
    } else {
        close();
    }// end if
}


void AlbaranesProveedor::s_contextMenu(int, int, int button, const QPoint &poin) {
    qDebug("button = %d", button);
    if (button == 2) {
        QPopupMenu *popup;
        popup = new QPopupMenu;
        popup->insertItem(tr("Borrar Albaran Proveedor"),101);
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


void AlbaranesProveedor::newBudget() {
    fprintf(stderr,"Iniciamos el boton_crear\n");
    AlbaranProveedorView *bud = new AlbaranProveedorView(companyact,companyact->m_pWorkspace,theApp->translate("Edición de Presupuestos", "company"));
    bud->show();
    bud->pintaAlbaranProveedor();
}// end boton_crear


void AlbaranesProveedor::imprimir() {
    /// Copiamos el archivo
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"albaranesproveedor.rml";
    archivo = "cp "+archivo+" /tmp/albaranesproveedor.rml";
    system (archivo.ascii());

    /// Copiamos el logo
    archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";
    archivo = "cp "+archivo+" /tmp/logo.jpg";
    system (archivo.ascii());

    QFile file;
    file.setName( "/tmp/albaranesproveedor.rml" );
    file.open( IO_ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;
    // L�ea de totales del albaranp

    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"10cm, 2cm, 2cm, 3cm\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "	<td>Descripicon</td>";
    fitxersortidatxt += "	<td>Referencia</td>";
    fitxersortidatxt += "	<td>Proveedor</td>";
    fitxersortidatxt += "	<td>Contacto</td>";
    fitxersortidatxt += "</tr>";

    QString SQLQuery = "SELECT * FROM albaranp LEFT JOIN proveedor ON albaranp.idproveedor = proveedor.idproveedor LEFT JOIN almacen ON   albaranp.idalmacen=almacen.idalmacen WHERE 1=1 "+generaFiltro();
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    while(!cur->eof()) {
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "<td>"+cur->valor("descalbaranp")+"</td>";
        fitxersortidatxt += "<td>"+cur->valor("refalbaranp")+"</td>";
        fitxersortidatxt += "<td>"+cur->valor("idproveedor")+"</td>";
        fitxersortidatxt += "<td>"+cur->valor("contactalbaranp")+"</td>";
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
    system("trml2pdf.py /tmp/albaranesproveedor.rml > /tmp/albaranesproveedor.pdf");
    system("kpdf /tmp/albaranesproveedor.pdf &");

}// end imprimir


void AlbaranesProveedor::s_removeBudget() {
    fprintf(stderr,"Iniciamos el boton_borrar\n");
    int a = m_list->currentRow();
    m_idalbaranp = m_list->text(a,COL_IDALBARANP);
    if (m_modo ==0 && m_idalbaranp != "") {
        AlbaranProveedorView *bud = new AlbaranProveedorView(companyact,companyact->m_pWorkspace,theApp->translate("Edición de Albaranes de Proveedores", "company"));
        bud->cargaAlbaranProveedor(m_idalbaranp);
        bud->s_deleteAlbaranProveedor();
	bud->close();
    }// end if
    inicializa();
}// end boton_borrar
