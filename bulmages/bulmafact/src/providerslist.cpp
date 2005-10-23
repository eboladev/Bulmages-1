/***************************************************************************
 *   Copyright (C) 2004 by J. M. Torres Rigo                               *
 *   joanmi@bulma.net                                                      *
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

/*
-- Los proveedores son los que nos suminstran articulos y/o servicios.
-- COMPROVACIONS D'INTEGRITAT>Gen�iques:
-- 1 Article t�1 sol prove�or principal.
-- 1 Article t�1 sol prove�or referent.
-- CAMPOS
-- ======
-- Codi: Clau artificial.
-- Nom: Nom comercial o fiscal.
-- Nom_alternatiu: Nom comercial o fiscal.
-- CIF: Codi d'Identificaci�Fiscal.
-- CodiCli: Codi de client amb que ens facturen. �il per a identificar-nos.
-- C_Banc
-- Comentaris
-- Adre�: Adre�.
-- Poblaci� Poblaci�
-- CProv: Codi de provincia (dos primers d�its del codi postal).
-- sCP: Tres darrers d�its del codi postal.
-- Telf: Tel�on.
-- Fax: Fax.
-- Email: eMail.
-- Url: Url.
-- CompteWeb: Dades de login si disposen de tenda o tarifes en l�ia
CREATE TABLE proveedor (
  idproveedor serial PRIMARY KEY,
  nomproveedor character varying(200),
  nomaltproveedor character varying(200),
  cifproveedor character varying(12),
  codicliproveedor character varying(30),
  cbancproveedor character varying(20),
  comentproveedor character varying(2000),
  dirproveedor character varying(50),
  poblproveedor character varying(50),
  cpproveedor character varying(9) NOT NULL,
  telproveedor character varying(12),
  faxproveedor character varying(12),
  emailproveedor character varying(100),
  urlproveedor character varying(100),
  clavewebproveedor character varying(100),
);
*/


#include "providerslist.h"
#include "qtable1.h"
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qcheckbox.h>

#include "company.h"
#include "provedit.h"

#define COL_IDPROVEEDOR 0
#define COL_NOMPROVEEDOR 1
#define COL_NOMALTPROVEEDOR 2
#define COL_CIFPROVEEDOR 3
#define COL_CODICLIPROVEEDOR 4
#define COL_CBANCPROVEEDOR 5
#define COL_COMENTPROVEEDOR 6
#define COL_DIRPROVEEDOR 7
#define COL_POBLPROVEEDOR 8
#define COL_CPPROVEEDOR 9
#define COL_TELPROVEEDOR 10
#define COL_FAXPROVEEDOR 11
#define COL_EMAILPROVEEDOR 12
#define COL_URLPROVEEDOR 13
#define COL_CLAVEWEBPROVEEDOR 14

void providerslist::s_configurar() {
    if(mver_idproveedor->isChecked() )
        m_list->showColumn(COL_IDPROVEEDOR);
    else
        m_list->hideColumn(COL_IDPROVEEDOR);

    if(mver_nomproveedor->isChecked() )
        m_list->showColumn(COL_NOMPROVEEDOR);
    else
        m_list->hideColumn(COL_NOMPROVEEDOR);

    if(mver_nomaltproveedor->isChecked() )
        m_list->showColumn(COL_NOMALTPROVEEDOR);
    else
        m_list->hideColumn(COL_NOMALTPROVEEDOR);

    if(mver_cifproveedor->isChecked() )
        m_list->showColumn(COL_CIFPROVEEDOR);
    else
        m_list->hideColumn(COL_CIFPROVEEDOR);

    if(mver_codicliproveedor->isChecked() )
        m_list->showColumn(COL_CODICLIPROVEEDOR);
    else
        m_list->hideColumn(COL_CODICLIPROVEEDOR);

    if(mver_cbancproveedor->isChecked() )
        m_list->showColumn(COL_CBANCPROVEEDOR);
    else
        m_list->hideColumn(COL_CBANCPROVEEDOR);

    if(mver_comentproveedor->isChecked() )
        m_list->showColumn(COL_COMENTPROVEEDOR);
    else
        m_list->hideColumn(COL_COMENTPROVEEDOR);

    if(mver_dirproveedor->isChecked() )
        m_list->showColumn(COL_DIRPROVEEDOR);
    else
        m_list->hideColumn(COL_DIRPROVEEDOR);

    if(mver_poblproveedor->isChecked() )
        m_list->showColumn(COL_POBLPROVEEDOR);
    else
        m_list->hideColumn(COL_POBLPROVEEDOR);

    if(mver_cpproveedor->isChecked() )
        m_list->showColumn(COL_CPPROVEEDOR);
    else
        m_list->hideColumn(COL_CPPROVEEDOR);

    if(mver_telproveedor->isChecked() )
        m_list->showColumn(COL_TELPROVEEDOR);
    else
        m_list->hideColumn(COL_TELPROVEEDOR);

    if(mver_faxproveedor->isChecked() )
        m_list->showColumn(COL_FAXPROVEEDOR);
    else
        m_list->hideColumn(COL_FAXPROVEEDOR);

    if(mver_emailproveedor->isChecked() )
        m_list->showColumn(COL_EMAILPROVEEDOR);
    else
        m_list->hideColumn(COL_EMAILPROVEEDOR);

    if(mver_urlproveedor->isChecked() )
        m_list->showColumn(COL_URLPROVEEDOR);
    else
        m_list->hideColumn(COL_URLPROVEEDOR);

    if(mver_clavewebproveedor->isChecked() )
        m_list->showColumn(COL_CLAVEWEBPROVEEDOR);
    else
        m_list->hideColumn(COL_CLAVEWEBPROVEEDOR);
}// end s_configurar

providerslist::providerslist(company *comp, QWidget *parent, const char *name, int flag)
        : providerslistbase(parent, name, flag) {
    companyact = comp;
    hideBusqueda();
    hideConfiguracion();
    m_modo=0;
    m_idprovider="";
    m_cifprovider="";
    m_nomprovider="";
    companyact->meteWindow(caption(),this);

    m_list->setNumRows( 0 );
    m_list->setNumCols( 0 );
    m_list->setSelectionMode( QTable::SingleRow );
    m_list->setSorting( TRUE );
    m_list->setSelectionMode( QTable::SingleRow );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(15);
    m_list->horizontalHeader()->setLabel( COL_IDPROVEEDOR, tr( "Codigo" ) );
    m_list->horizontalHeader()->setLabel( COL_NOMPROVEEDOR, tr( "Nombre Fiscal" ) );
    m_list->horizontalHeader()->setLabel( COL_NOMALTPROVEEDOR, tr( "Nombre Comercial" ) );
    m_list->horizontalHeader()->setLabel( COL_CIFPROVEEDOR, tr( "CIF/NIF" ) );
    m_list->horizontalHeader()->setLabel( COL_CODICLIPROVEEDOR, tr( "Codigo de cliente" ) );
    m_list->horizontalHeader()->setLabel( COL_CBANCPROVEEDOR, tr( "Cuenta Bancaria" ) );
    m_list->horizontalHeader()->setLabel( COL_COMENTPROVEEDOR, tr("Observaciones") );
    m_list->horizontalHeader()->setLabel( COL_DIRPROVEEDOR, tr("Domicilio") );
    m_list->horizontalHeader()->setLabel( COL_POBLPROVEEDOR, tr("Poblacion�") );
    m_list->horizontalHeader()->setLabel( COL_CPPROVEEDOR, tr("C.P.") );
    m_list->horizontalHeader()->setLabel( COL_TELPROVEEDOR, tr("N Telefono") );
    m_list->horizontalHeader()->setLabel( COL_FAXPROVEEDOR, tr("N Fax") );
    m_list->horizontalHeader()->setLabel( COL_EMAILPROVEEDOR, tr("Correo Electronico") );
    m_list->horizontalHeader()->setLabel( COL_URLPROVEEDOR, tr("Pagina Web") );
    m_list->horizontalHeader()->setLabel( COL_CLAVEWEBPROVEEDOR, tr("Clave propia web proveedor") );
    m_list->setColumnWidth(COL_IDPROVEEDOR,75);
    m_list->setColumnWidth(COL_NOMPROVEEDOR,300);
    m_list->setColumnWidth(COL_NOMALTPROVEEDOR,300);
    m_list->setColumnWidth(COL_CIFPROVEEDOR,75);
    m_list->setColumnWidth(COL_CODICLIPROVEEDOR,75);
    m_list->setColumnWidth(COL_CBANCPROVEEDOR,100);
    m_list->setColumnWidth(COL_COMENTPROVEEDOR,1000);
    m_list->setColumnWidth(COL_DIRPROVEEDOR,300);
    m_list->setColumnWidth(COL_POBLPROVEEDOR,200);
    m_list->setColumnWidth(COL_CPPROVEEDOR,75);
    m_list->setColumnWidth(COL_TELPROVEEDOR,75);
    m_list->setColumnWidth(COL_FAXPROVEEDOR,100);
    m_list->setColumnWidth(COL_EMAILPROVEEDOR,300);
    m_list->setColumnWidth(COL_URLPROVEEDOR,300);
    m_list->setColumnWidth(COL_CLAVEWEBPROVEEDOR,300);
    inicializa();
    s_configurar();
}// end providerslist

providerslist::~providerslist() {
    companyact->sacaWindow(this);
}// end ~providerslist

void providerslist::inicializa() {
    //   listado->setPaletteBackgroundColor(QColor(150,230,230));
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor(confpr->valor(CONF_BG_BALANCE).ascii());
    m_list->setReadOnly(TRUE);
    m_list->setSorting(FALSE);
    cursor2 * cur= companyact->cargacursor("SELECT * FROM proveedor WHERE nomproveedor LIKE'%"+m_filtro->text()+"%'");
    m_list->setNumRows( cur->numregistros() );
    int i=0;
    while (!cur->eof()) {
        m_list->setText(i,COL_IDPROVEEDOR,cur->valor("idproveedor"));
        m_list->setText(i,COL_NOMPROVEEDOR,cur->valor("nomproveedor"));
        m_list->setText(i,COL_NOMALTPROVEEDOR,cur->valor("nomaltproveedor"));
        m_list->setText(i,COL_CIFPROVEEDOR,cur->valor("cifproveedor"));
        m_list->setText(i,COL_CODICLIPROVEEDOR,cur->valor("codicliproveedor"));
        m_list->setText(i,COL_CBANCPROVEEDOR,cur->valor("cbancproveedor"));
        m_list->setText(i,COL_COMENTPROVEEDOR,cur->valor("comentproveedor"));
        m_list->setText(i,COL_DIRPROVEEDOR,cur->valor("dirproveedor"));
        m_list->setText(i,COL_POBLPROVEEDOR,cur->valor("poblproveedor"));
        m_list->setText(i,COL_CPPROVEEDOR,cur->valor("cpproveedor"));
        m_list->setText(i,COL_TELPROVEEDOR,cur->valor("telproveedor"));
        m_list->setText(i,COL_FAXPROVEEDOR,cur->valor("faxproveedor"));
        m_list->setText(i,COL_EMAILPROVEEDOR,cur->valor("emailproveedor"));
        m_list->setText(i,COL_URLPROVEEDOR,cur->valor("urlproveedor"));
        m_list->setText(i,COL_CLAVEWEBPROVEEDOR,cur->valor("clavewebproveedor"));
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
}// end inicializa



void providerslist::doubleclicked(int a, int b, int c, const QPoint &) {
    m_idprovider = m_list->text(a,COL_IDPROVEEDOR);
    m_cifprovider = m_list->text(a,COL_CIFPROVEEDOR);
    m_nomprovider = m_list->text(a,COL_NOMPROVEEDOR);
    if (m_modo ==0 ) {
        QString idprov = m_list->text(a, COL_IDPROVEEDOR);
        fprintf(stderr, "parm a: %d  parm b: %d  parm c %d \n", a, b, c);
        provedit *prov = new provedit(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Proveedores", "company"));
        prov->chargeprovider(idprov);
        prov->show();
    } else {
        close();
    }// end if
}




void providerslist::contextMenu(int , int , const QPoint &) {
    /*
       QString idprov = m_list->text(a, COL_IDPROVEEDOR);
       provedit *prov = new provedit(companyact,0,theApp->translate("Edicion de Proveedores", "company"));
       prov->chargeprovider(idprov);
       prov->exec();
       delete prov;
       inicializa();
    */
}// end contextMenuRequested


void providerslist::newprovider() {
    fprintf(stderr,"Iniciamos el boton_crear\n");
    provedit *prov = new provedit(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Proveedores", "company"));
    prov->show();
}// end boton_crear


void providerslist::s_findProvider() {
    inicializa();
}// end s_findProvider


void providerslist::s_editProvider() {
    QPoint a;
    doubleclicked(m_list->currentRow(), 0,0, a);
}// end s_editProvider


/** SLOT que responde a la pulsaci� de borrar un determinado proveedor
  * Dicha funci� avisa de la perdida de datos y si se decide continuar
  * Se procede a borrar el proveedor
  */
void providerslist::s_removeProvider() {
    fprintf(stderr, "removeOrder button activated");
    if (QMessageBox::warning( this, "BulmaFact - Proveedores",
                              "Seguro que desea borrar el proveedor?", "Si", "No") == 0) {
        int row = m_list->currentRow();
        QString idProvider = m_list->text(row,COL_IDPROVEEDOR);
        QString SQLQuery = "DELETE FROM proveedor WHERE idproveedor ="+idProvider;
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
        if (error) {
            companyact->rollback();
            return;
        }// end if
        companyact->commit();
        inicializa();
    }
}// end s_removeProvider


/** SLOT que se ejecuta al pulsar sobre el bot� de imprimir en la ventana de proveedores
  * La funci� llama a rtkview para generar el listado predefinido en reports/providerslist.rtk
  */
void providerslist::s_printProviders() {
    fprintf(stderr,"Impresion� del listado\n");
    /// Mediante comandos de sistema reemplazamos lo que necesitamos para obtener un fichero deseable.
    QString cadena;
    // ACORDARSE DE CAMBIAR LAS RUTAS POR LAS DEL ARCHIVO DE CONFIGURACION.
    cadena = "cp "+confpr->valor(CONF_DIR_REPORTS)+"bulma-styles.xml   /tmp/bulma-styles.xml" ;
    system (cadena.ascii());
    cadena = "cp "+confpr->valor(CONF_DIR_REPORTS)+"providerslist.rtk   /tmp/providerslist.rtk" ;
    system (cadena.ascii());
    cadena = "rtkview --input-sql-driver QPSQL7 --input-sql-database ";
    cadena += companyact->nameDB()+" ";
    cadena += "/tmp/providerslist.rtk &";
    fprintf(stderr,"%s\n",cadena.ascii());
    system (cadena.ascii());
}// end s_printProviders.



