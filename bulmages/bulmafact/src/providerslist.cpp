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
#include "funcaux.h"

#include <QLineEdit>
#include <QMessageBox>
#include <QCheckBox>
#include <QFile>
#include <Q3FileDialog>
//Added by qt3to4:
#include <QTextStream>

#include "company.h"
#include "provedit.h"
#include "pgimportfiles.h"

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

void providerslist::guardaconfig() {
    QString aux = "";
    mver_idproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_nomproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_nomaltproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_cifproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_codicliproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_cbancproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_comentproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_dirproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_poblproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_cpproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_telproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_faxproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_emailproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_urlproveedor->isChecked() ? aux += "1,":aux+="0,";
    mver_clavewebproveedor->isChecked() ? aux += "1,":aux+="0,";

    QFile file( confpr->valor(CONF_DIR_USER)+"confproviderslist.cfn" );
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

void providerslist::cargaconfig() {
    QFile file( confpr->valor(CONF_DIR_USER)+"confproviderslist.cfn" );
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

    mver_idproveedor->setChecked(line.at(0)=='1');
    mver_nomproveedor->setChecked(line.at(2)=='1');
    mver_nomaltproveedor->setChecked(line.at(4)=='1');
    mver_cifproveedor->setChecked(line.at(6)=='1');
    mver_codicliproveedor->setChecked(line.at(8)=='1');
    mver_cbancproveedor->setChecked(line.at(10)=='1');
    mver_dirproveedor->setChecked(line.at(12)=='1');
    mver_poblproveedor->setChecked(line.at(14)=='1');
    mver_cpproveedor->setChecked(line.at(16)=='1');
    mver_telproveedor->setChecked(line.at(18)=='1');
    mver_faxproveedor->setChecked(line.at(20)=='1');
    mver_urlproveedor->setChecked(line.at(22)=='1');
    mver_clavewebproveedor->setChecked(line.at(24)=='1');
}// end cargaconfig



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

providerslist::providerslist(company *comp, QWidget *parent, const char *name, Qt::WFlags flag, edmode editmode)
        : providerslistbase(parent, name, flag) , pgimportfiles(comp) {
    cargaconfig();
    s_configurar();
    companyact = comp;
    hideBusqueda();
    hideConfiguracion();
    m_modo=editmode;
    m_idprovider="";
    m_cifprovider="";
    m_nomprovider="";
    if (m_modo == EditMode)
        companyact->meteWindow(caption(),this);

    m_list->setNumRows( 0 );
    m_list->setNumCols( 0 );
    m_list->setSelectionMode( Q3Table::SingleRow );
    m_list->setSorting( TRUE );
    m_list->setSelectionMode( Q3Table::SingleRow );
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
    presenta();

}// end providerslist

providerslist::~providerslist() {
    if (m_modo == EditMode)
        companyact->sacaWindow(this);
    guardaconfig();
}// end ~providerslist

void providerslist::presenta() {
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
}// end presenta



void providerslist::doubleclicked(int a, int , int , const QPoint &) {
	_depura("providerslist::doubleclicked()",0);
    m_idprovider = m_list->text(a,COL_IDPROVEEDOR);
    m_cifprovider = m_list->text(a,COL_CIFPROVEEDOR);
    m_nomprovider = m_list->text(a,COL_NOMPROVEEDOR);
    if (m_modo ==0 ) {
        QString idprov = m_list->text(a, COL_IDPROVEEDOR);
        provedit *prov = new provedit(companyact,0,theApp->translate("Edicion de Proveedores", "company"));
        if (prov->chargeprovider(idprov)) {
            return;
	}
        companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        close();
    }// end if
	_depura("END providerslist::doubleclicked()",0);
}




void providerslist::contextMenu(int , int , const QPoint &) {}// end contextMenuRequested


void providerslist::newprovider() {
    fprintf(stderr,"Iniciamos el boton_crear\n");
    provedit *prov = new provedit(companyact,0,theApp->translate("Edicion de Proveedores", "company"));
    companyact->m_pWorkspace->addWindow(prov);
    prov->show();
}// end boton_crear


void providerslist::s_findProvider() {
    presenta();
}// end s_findProvider


void providerslist::s_editProvider() {
    int a = m_list->currentRow();
	if (a >=0 ) 
    	doubleclicked(a,0,0, QPoint());
	else
	_depura("Debe seleccionar una linea",2);
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
        presenta();
    }
}// end s_removeProvider


/** SLOT que se ejecuta al pulsar sobre el bot� de imprimir en la ventana de proveedores
  * La funci� llama a rtkview para generar el listado predefinido en reports/providerslist.rtk
  */
void providerslist::s_printProviders() {
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"proveedores.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"proveedores.rml";
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
    if(mver_idproveedor->isChecked() )
        fitxersortidatxt += "	<td>Id.</td>";
    if(mver_nomproveedor->isChecked() )
        fitxersortidatxt += "	<td>Nombre</td>";
    if(mver_nomaltproveedor->isChecked() )
        fitxersortidatxt += "	<td>Nom. Alt</td>";
    if(mver_cifproveedor->isChecked() )
        fitxersortidatxt += "	<td>CIF</td>";
    if(mver_codicliproveedor->isChecked() )
        fitxersortidatxt += "	<td>Cod. Cliente</td>";
    if(mver_cbancproveedor->isChecked() )
        fitxersortidatxt += "	<td>Banco</td>";
    if(mver_comentproveedor->isChecked() )
        fitxersortidatxt += "	<td>Coment</td>";
    if(mver_dirproveedor->isChecked() )
        fitxersortidatxt += "	<td>Direccion</td>";
    if(mver_poblproveedor->isChecked() )
        fitxersortidatxt += "	<td>Poblacion</td>";
    if(mver_cpproveedor->isChecked() )
        fitxersortidatxt += "	<td>CP</td>";
    if(mver_telproveedor->isChecked() )
        fitxersortidatxt += "	<td>Tel.</td>";
    if(mver_faxproveedor->isChecked() )
        fitxersortidatxt += "	<td>Fax</td>";
    if(mver_emailproveedor->isChecked() )
        fitxersortidatxt += "	<td>E-Mail</td>";
    if(mver_urlproveedor->isChecked() )
        fitxersortidatxt += "	<td>URL</td>";
    if(mver_clavewebproveedor->isChecked() )
        fitxersortidatxt += "	<td>Clave Web</td>";
    /// -----------------------------------------------
    fitxersortidatxt += "</tr>";

    cursor2 * cur= companyact->cargacursor("SELECT * FROM proveedor WHERE nomproveedor LIKE'%"+m_filtro->text()+"%'");
    while(!cur->eof()) {
        fitxersortidatxt += "<tr>";
        /// -----------------------------------------------
        if(mver_idproveedor->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idproveedor"))+"</td>";
        if(mver_nomproveedor->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("nomproveedor"))+"</td>";
        if(mver_nomaltproveedor->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("nomaltproveedor"))+"</td>";
        if(mver_cifproveedor->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("cifproveedor"))+"</td>";
        if(mver_codicliproveedor->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("codicliproveedor"))+"</td>";
        if(mver_cbancproveedor->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("cbancproveedor"))+"</td>";
        if(mver_comentproveedor->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("comentproveedor"))+"</td>";
        if(mver_dirproveedor->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("dirproveedor"))+"</td>";
        if(mver_poblproveedor->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("poblproveedor"))+"</td>";
        if(mver_cpproveedor->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("cpproveedor"))+"</td>";
        if(mver_telproveedor->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("telproveedor"))+"</td>";
        if(mver_faxproveedor->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("faxproveedor"))+"</td>";
        if(mver_emailproveedor->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("emailproveedor"))+"</td>";
        if(mver_urlproveedor->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("urlproveedor"))+"</td>";
        if(mver_clavewebproveedor->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("clavewebproveedor"))+"</td>";
        /// -----------------------------------------------
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


    invocaPDF("proveedores");
}// end s_printProviders.



void providerslist::s_exportar() {
    QFile filexml (Q3FileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER),"Proveedores (*.xml)", this, "select file", "Elija el Archivo"));
    if(filexml.open(QIODevice::WriteOnly)) {
        bulmafact2XML(filexml, IMPORT_PROVEEDORES);
        filexml.close();
    } else {
        _depura("ERROR AL ABRIR ARCHIVO\n",2);
    }// end if
}//


void providerslist::s_importar() {
    QFile filexml (Q3FileDialog::getOpenFileName(confpr->valor(CONF_DIR_USER),"Proveedores (*.xml)", this, "select file", "Elija el Archivo"));
    if (filexml.open(QIODevice::ReadOnly))  {
        XML2BulmaFact(filexml, IMPORT_PROVEEDORES);
        filexml.close();
        presenta();
    }  else  {
        _depura("ERROR AL ABRIR ARCHIVO\n",2);
    }// end if
}


