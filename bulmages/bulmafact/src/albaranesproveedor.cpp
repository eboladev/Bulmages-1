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
#include <q3popupmenu.h>
#include <qcheckbox.h>
#include <qfile.h>
#include <qcheckbox.h>
//Added by qt3to4:
#include <QTextStream>

#include "configuracion.h"
#include "funcaux.h"

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
#define COL_TOTALALBARANPROVEEDOR 14
#define COL_TOTALBASEIMP 15
#define COL_TOTALIMPUESTOS 16

void AlbaranesProveedor::guardaconfig() {
    QString aux = "";
    mver_idalbaranp->isChecked() ? aux += "1,":aux+="0,";
    mver_numalbaranp->isChecked() ? aux += "1,":aux+="0,";
    mver_descalbaranp->isChecked() ? aux += "1,":aux+="0,";
    mver_refalbaranp->isChecked() ? aux += "1,":aux+="0,";
    mver_fechaalbaranp->isChecked() ? aux += "1,":aux+="0,";
    mver_loginusuario->isChecked() ? aux += "1,":aux+="0,";
    mver_comentalbaranp->isChecked() ? aux += "1,":aux+="0,";
    mver_procesadoalbaranp->isChecked() ? aux += "1,":aux+="0,";
    mver_idproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_idforma_pago->isChecked() ? aux += "1,":aux+="0,";
    mver_idalmacen->isChecked() ? aux += "1,":aux+="0,";
    mver_nomproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_nomalmacen->isChecked() ? aux += "1,":aux+="0,";
    mver_descforma_pago->isChecked() ? aux += "1,":aux+="0,";
    mver_totalalbaranproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_totalbaseimp->isChecked() ? aux += "1,":aux+="0,";
    mver_totalimpuestos->isChecked() ? aux += "1,":aux+="0,";

    QFile file( confpr->valor(CONF_DIR_USER)+"confalbaranesproveedor.cfn" );
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

void AlbaranesProveedor::cargaconfig() {
    QFile file( confpr->valor(CONF_DIR_USER)+"confalbaranesproveedor.cfn" );
    QString line;
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        line = stream.readLine(); // line of text excluding '\n'
for (int i = 0; i < m_list->numCols(); i++)
{
    QString linea = stream.readLine();
    m_list->setColumnWidth(i, linea.toInt());
}// end for
        file.close();
    } else
        return;

    mver_idalbaranp->setChecked(line.at(0)=='1');
    mver_numalbaranp->setChecked(line.at(2)=='1');
    mver_descalbaranp->setChecked(line.at(4)=='1');
    mver_refalbaranp->setChecked(line.at(6)=='1');
    mver_fechaalbaranp->setChecked(line.at(8)=='1');
    mver_loginusuario->setChecked(line.at(10)=='1');
    mver_comentalbaranp->setChecked(line.at(12)=='1');
    mver_procesadoalbaranp->setChecked(line.at(14)=='1');
    mver_idproveedor->setChecked(line.at(16)=='1');
    mver_idforma_pago->setChecked(line.at(18)=='1');
    mver_idalmacen->setChecked(line.at(20)=='1');
    mver_nomproveedor->setChecked(line.at(22)=='1');
    mver_nomalmacen->setChecked(line.at(24)=='1');
    mver_descforma_pago->setChecked(line.at(26)=='1');
    mver_totalalbaranproveedor->setChecked(line.at(28)=='1');
    mver_totalbaseimp->setChecked(line.at(30)=='1');
    mver_totalimpuestos->setChecked(line.at(32)=='1');
}// end cargaconfig


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

    if(mver_totalalbaranproveedor->isChecked() )
        m_list->showColumn(COL_TOTALALBARANPROVEEDOR);
    else
        m_list->hideColumn(COL_TOTALALBARANPROVEEDOR);

    if(mver_totalbaseimp->isChecked() )
        m_list->showColumn(COL_TOTALBASEIMP);
    else
        m_list->hideColumn(COL_TOTALBASEIMP);

    if(mver_totalimpuestos->isChecked() )
        m_list->showColumn(COL_TOTALIMPUESTOS);
    else
        m_list->hideColumn(COL_TOTALIMPUESTOS);

    if (confpr->valor(CONF_MOSTRAR_ALMACEN)!="YES") {
        m_list->hideColumn(COL_NOMALMACEN);
    }// end if

}


AlbaranesProveedor::AlbaranesProveedor(QWidget *parent, const char *name, Qt::WFlags flag)
: AlbaranesProveedorBase(parent, name, flag) {
	inicializa();
	cargaconfig();
	s_configurar();
    companyact = NULL;
    m_modo=0;
    m_idalbaranp="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}// end providerslist

AlbaranesProveedor::AlbaranesProveedor(company *comp, QWidget *parent, const char *name, Qt::WFlags flag)
: AlbaranesProveedorBase(parent, name, flag) {

    companyact = comp;
    m_proveedor->setcompany(comp);
    m_articulo->setcompany(comp);
    inicializa();
	cargaconfig();
	s_configurar();
	presenta();
    m_modo=0;
    m_idalbaranp="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}// end providerslist

AlbaranesProveedor::~AlbaranesProveedor() {
    companyact->refreshAlbaranesProveedor();
    companyact->sacaWindow(this);
	guardaconfig();
}// end ~providerslist

void AlbaranesProveedor::inicializa() {
    _depura("AlbaranesProveedor::inicializa()\n",1);
    m_list->setNumRows( 0 );
    m_list->setNumCols( 0 );
    m_list->setSelectionMode( Q3Table::SingleRow );
    m_list->setSorting( TRUE );
    m_list->setSelectionMode( Q3Table::SingleRow );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(17);
       
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
    m_list->horizontalHeader()->setLabel( COL_TOTALALBARANPROVEEDOR, tr("Total") );
    m_list->horizontalHeader()->setLabel( COL_TOTALBASEIMP, tr("Base Imponible") );
    m_list->horizontalHeader()->setLabel( COL_TOTALIMPUESTOS, tr("Impuestos") );

    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuración que es global.
    m_list->setPaletteBackgroundColor("#EEFFFF");
    m_list->setReadOnly(TRUE);

    _depura("END AlbaranesProveedor::inicializa()\n",0);
}// end inicializa


void AlbaranesProveedor::presenta() {
    _depura("AlbaranesProveedor::presenta()\n",1);
    if (companyact != NULL ) {
        cursor2 * cur= companyact->cargacursor("SELECT * FROM albaranp LEFT JOIN proveedor ON albaranp.idproveedor = proveedor.idproveedor LEFT JOIN almacen ON   albaranp.idalmacen=almacen.idalmacen LEFT JOIN forma_pago ON albaranp.idforma_pago = forma_pago.idforma_pago WHERE 1=1 "+generaFiltro());
        m_list->setNumRows( cur->numregistros() );
        int i=0;
        while (!cur->eof()) {	
            m_list->setText(i,COL_IDALBARANP,cur->valor("idalbaranp"));
            m_list->setText(i,COL_NUMALBARANP,cur->valor("numalbaranp"));
            m_list->setText(i,COL_DESCALBARANP,cur->valor("descalbaranp"));
            m_list->setText(i,COL_REFALBARANP,cur->valor("refalbaranp"));
            m_list->setText(i,COL_FECHAALBARANP,cur->valor("fechaalbaranp"));
//            m_list->setText(i,COL_LOGINUSUARIO,cur->valor("loginusuario"));
            m_list->setText(i,COL_COMENTALBARANP,cur->valor("comentalbaranp"));
            m_list->setText(i,COL_PROCESADOALBARANP,cur->valor("procesadoalbaranp"));
            m_list->setText(i,COL_IDPROVEEDOR,cur->valor("idproveedor"));
            m_list->setText(i,COL_IDFORMA_PAGO,cur->valor("idforma_pago"));
            m_list->setText(i,COL_IDALMACEN,cur->valor("idalmacen"));
            m_list->setText(i,COL_NOMPROVEEDOR,cur->valor("nomproveedor"));
            m_list->setText(i,COL_NOMALMACEN,cur->valor("nomalmacen"));
            m_list->setText(i,COL_DESCFORMA_PAGO,cur->valor("descforma_pago"));

            /// Calculamos el total del presupuesto y lo presentamos.
            cursor2 *cur1 = companyact->cargacursor("SELECT calctotalalbpro("+cur->valor("idalbaranp")+") AS total, calcbimpalbpro("+cur->valor("idalbaranp")+") AS base, calcimpuestosalbpro("+cur->valor("idalbaranp")+") AS impuestos");
            m_list->setText(i,COL_TOTALALBARANPROVEEDOR,cur1->valor("total"));
            m_list->setText(i,COL_TOTALBASEIMP, cur1->valor("base"));
            m_list->setText(i,COL_TOTALIMPUESTOS, cur1->valor("impuestos"));
            delete cur1;

            i++;
            cur->siguienteregistro();
        }// end while
        delete cur;


	/// Hacemos el calculo del total.
	cur = companyact->cargacursor("SELECT SUM(calctotalalbpro(idalbaranp)) AS total FROM albaranp LEFT JOIN proveedor ON albaranp.idproveedor = proveedor.idproveedor LEFT JOIN almacen ON   albaranp.idalmacen=almacen.idalmacen WHERE 1=1 "+generaFiltro());
	m_total->setText(cur->valor("total"));
	delete cur;

    }// end if

    _depura("END AlbaranesProveedor::presenta()\n",0);
}// end presenta



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

    if (m_fechain->text() != "")
	filtro += " AND fechaalbaranp >= '"+m_fechain->text()+"' ";

    if (m_fechafin->text() != "") 
	filtro += " AND fechaalbaranp <= '"+m_fechafin->text()+"' ";


//    filtro += " ORDER BY idalbaranp";
    return (filtro);
}// end generaFiltro



void AlbaranesProveedor::s_editar() {
    int a = m_list->currentRow();
	if (a >=0 ) 
    	doubleclicked(a,0,0, QPoint());
	else
	_depura("Debe seleccionar una linea",2);
}

void AlbaranesProveedor::doubleclicked(int a, int , int , const QPoint &) {
    m_idalbaranp = m_list->text(a,COL_IDALBARANP);
    if (m_modo ==0 && m_idalbaranp != "") {
        AlbaranProveedorView *bud = new AlbaranProveedorView(companyact,0,theApp->translate("Edición de Presupuestos", "company"));
	companyact->m_pWorkspace->addWindow(bud);
        bud->cargaAlbaranProveedor(m_idalbaranp);
        bud->show();
    } else {
        close();
    }// end if
}


void AlbaranesProveedor::s_contextMenu(int, int, int button, const QPoint &poin) {
    qDebug("button = %d", button);
    if (button == 2) {
        Q3PopupMenu *popup;
        popup = new Q3PopupMenu;
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


void AlbaranesProveedor::imprimir() {
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"albaranesproveedor.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"albaranesproveedor.rml";
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
/// ---------------------------------------------------------------------
    if(mver_idalbaranp->isChecked() )
	    fitxersortidatxt += "	<td>Id.</td>";
    if(mver_numalbaranp->isChecked() )
	    fitxersortidatxt += "	<td>Num. Albaran</td>";
    if(mver_descalbaranp->isChecked() )
	    fitxersortidatxt += "	<td>Descripcion</td>";
    if(mver_refalbaranp->isChecked() )
	    fitxersortidatxt += "	<td>Referencia</td>";
    if(mver_fechaalbaranp->isChecked() )
	    fitxersortidatxt += "	<td>Fecha</td>";
    if(mver_loginusuario->isChecked() )
	    fitxersortidatxt += "	<td>Usuario</td>";
    if(mver_comentalbaranp->isChecked() )
	    fitxersortidatxt += "	<td>Comentarios</td>";
    if(mver_procesadoalbaranp->isChecked() )
	    fitxersortidatxt += "	<td>Procesado</td>";
    if(mver_idproveedor->isChecked() )
	    fitxersortidatxt += "	<td>Id. Proveedor</td>";
    if(mver_idforma_pago->isChecked() )
	    fitxersortidatxt += "	<td>Id. F. Pago</td>";
    if(mver_idalmacen->isChecked() )
	    fitxersortidatxt += "	<td>Id. Almacen</td>";
    if(mver_nomproveedor->isChecked() )
	    fitxersortidatxt += "	<td>Proveedor</td>";
    if(mver_nomalmacen->isChecked() )
	    fitxersortidatxt += "	<td>Almacen</td>";
    if(mver_descforma_pago->isChecked() )
	    fitxersortidatxt += "	<td>F. Pago</td>";
    if(mver_totalalbaranproveedor->isChecked() )
	    fitxersortidatxt += "	<td>Total</td>";
    if(mver_totalbaseimp->isChecked() )
	    fitxersortidatxt += "	<td>Base Imp.</td>";
    if(mver_totalimpuestos->isChecked() )
	    fitxersortidatxt += "	<td>Impuestos</td>";
/// ---------------------------------------------------------------------
    fitxersortidatxt += "</tr>";

        cursor2 * cur= companyact->cargacursor("SELECT * FROM albaranp LEFT JOIN proveedor ON albaranp.idproveedor = proveedor.idproveedor LEFT JOIN almacen ON   albaranp.idalmacen=almacen.idalmacen LEFT JOIN forma_pago ON albaranp.idforma_pago = forma_pago.idforma_pago WHERE 1=1 "+generaFiltro());
    while(!cur->eof()) {
        fitxersortidatxt += "<tr>";
/// ----------------------------------------------------------
    if(mver_idalbaranp->isChecked() )
        fitxersortidatxt += "<td>"+cur->valor("idalbaranp")+"</td>";
    if(mver_numalbaranp->isChecked() )
        fitxersortidatxt += "<td>"+cur->valor("numalbaranp")+"</td>";
    if(mver_descalbaranp->isChecked() )
        fitxersortidatxt += "<td>"+cur->valor("descalbaranp")+"</td>";
    if(mver_refalbaranp->isChecked() )
        fitxersortidatxt += "<td>"+cur->valor("refalbaranp")+"</td>";
    if(mver_fechaalbaranp->isChecked() )
        fitxersortidatxt += "<td>"+cur->valor("fechaalbaranp")+"</td>";
    if(mver_loginusuario->isChecked() )
        fitxersortidatxt += "<td>"+cur->valor("idtrabajador")+"</td>";
    if(mver_comentalbaranp->isChecked() )
        fitxersortidatxt += "<td>"+cur->valor("comentalbaranp")+"</td>";
    if(mver_procesadoalbaranp->isChecked() )
        fitxersortidatxt += "<td>"+cur->valor("procesadoalbaranp")+"</td>";
    if(mver_idproveedor->isChecked() )
        fitxersortidatxt += "<td>"+cur->valor("idproveedor")+"</td>";
    if(mver_idforma_pago->isChecked() )
        fitxersortidatxt += "<td>"+cur->valor("idforma_pago")+"</td>";
    if(mver_idalmacen->isChecked() )
        fitxersortidatxt += "<td>"+cur->valor("idalmacen")+"</td>";
    if(mver_nomproveedor->isChecked() )
        fitxersortidatxt += "<td>"+cur->valor("nomproveedor")+"</td>";
    if(mver_nomalmacen->isChecked() )
        fitxersortidatxt += "<td>"+cur->valor("nomalmacen")+"</td>";
    if(mver_descforma_pago->isChecked() )
        fitxersortidatxt += "<td>"+cur->valor("descforma_pago")+"</td>";

            /// Calculamos el total del presupuesto y lo presentamos.
            cursor2 *cur1 = companyact->cargacursor("SELECT calctotalalbpro("+cur->valor("idalbaranp")+") AS total, calcbimpalbpro("+cur->valor("idalbaranp")+") AS base, calcimpuestosalbpro("+cur->valor("idalbaranp")+") AS impuestos");
    if(mver_totalalbaranproveedor->isChecked() )
        fitxersortidatxt += "<td>"+cur1->valor("total")+"</td>";
    if(mver_totalbaseimp->isChecked() )
        fitxersortidatxt += "<td>"+cur1->valor("base")+"</td>";
    if(mver_totalimpuestos->isChecked() )
        fitxersortidatxt += "<td>"+cur1->valor("impuestos")+"</td>";
            delete cur1;
/// ----------------------------------------------------------
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
    invocaPDF("albaranesproveedor");

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
    presenta();
}// end boton_borrar
