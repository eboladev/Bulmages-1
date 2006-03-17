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

#include <QLineEdit>
#include <QMessageBox>
#include <QCheckBox>
#include <QFile>
#include <Q3FileDialog>
#include <QTextStream>

#include "providerslist.h"
#include "qtable1.h"
#include "funcaux.h"
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

void ProveedorList::guardaconfig() {
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

    QFile file( confpr->valor(CONF_DIR_USER)+"confProveedorList.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << aux << "\n";
        for (int i = 0; i < mui_list->columnCount(); i++) {
            mui_list->showColumn(i);
            stream << mui_list->columnWidth(i) << "\n";
        }// end for
        file.close();
    }// end if
}// end guardaconfig()

void ProveedorList::cargaconfig() {
    QFile file( confpr->valor(CONF_DIR_USER)+"confProveedorList.cfn" );
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



void ProveedorList::s_configurar() {
    if(mver_idproveedor->isChecked() )
        mui_list->showColumn(COL_IDPROVEEDOR);
    else
        mui_list->hideColumn(COL_IDPROVEEDOR);

    if(mver_nomproveedor->isChecked() )
        mui_list->showColumn(COL_NOMPROVEEDOR);
    else
        mui_list->hideColumn(COL_NOMPROVEEDOR);

    if(mver_nomaltproveedor->isChecked() )
        mui_list->showColumn(COL_NOMALTPROVEEDOR);
    else
        mui_list->hideColumn(COL_NOMALTPROVEEDOR);

    if(mver_cifproveedor->isChecked() )
        mui_list->showColumn(COL_CIFPROVEEDOR);
    else
        mui_list->hideColumn(COL_CIFPROVEEDOR);

    if(mver_codicliproveedor->isChecked() )
        mui_list->showColumn(COL_CODICLIPROVEEDOR);
    else
        mui_list->hideColumn(COL_CODICLIPROVEEDOR);

    if(mver_cbancproveedor->isChecked() )
        mui_list->showColumn(COL_CBANCPROVEEDOR);
    else
        mui_list->hideColumn(COL_CBANCPROVEEDOR);

    if(mver_comentproveedor->isChecked() )
        mui_list->showColumn(COL_COMENTPROVEEDOR);
    else
        mui_list->hideColumn(COL_COMENTPROVEEDOR);

    if(mver_dirproveedor->isChecked() )
        mui_list->showColumn(COL_DIRPROVEEDOR);
    else
        mui_list->hideColumn(COL_DIRPROVEEDOR);

    if(mver_poblproveedor->isChecked() )
        mui_list->showColumn(COL_POBLPROVEEDOR);
    else
        mui_list->hideColumn(COL_POBLPROVEEDOR);

    if(mver_cpproveedor->isChecked() )
        mui_list->showColumn(COL_CPPROVEEDOR);
    else
        mui_list->hideColumn(COL_CPPROVEEDOR);

    if(mver_telproveedor->isChecked() )
        mui_list->showColumn(COL_TELPROVEEDOR);
    else
        mui_list->hideColumn(COL_TELPROVEEDOR);

    if(mver_faxproveedor->isChecked() )
        mui_list->showColumn(COL_FAXPROVEEDOR);
    else
        mui_list->hideColumn(COL_FAXPROVEEDOR);

    if(mver_emailproveedor->isChecked() )
        mui_list->showColumn(COL_EMAILPROVEEDOR);
    else
        mui_list->hideColumn(COL_EMAILPROVEEDOR);

    if(mver_urlproveedor->isChecked() )
        mui_list->showColumn(COL_URLPROVEEDOR);
    else
        mui_list->hideColumn(COL_URLPROVEEDOR);

    if(mver_clavewebproveedor->isChecked() )
        mui_list->showColumn(COL_CLAVEWEBPROVEEDOR);
    else
        mui_list->hideColumn(COL_CLAVEWEBPROVEEDOR);
}// end s_configurar


ProveedorList::ProveedorList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag, edmode editmode)
        : QWidget (parent, name, flag) , pgimportfiles(comp) {
    setupUi(this);
    cargaconfig();
    m_companyact = comp;
    mui_list->setcompany(comp);
    hideBusqueda();
    hideConfiguracion();
    m_modo=editmode;
    m_idprovider="";
    m_cifprovider="";
    m_nomprovider="";
    if (m_modo == EditMode)
        m_companyact->meteWindow(caption(),this);
    presenta();

}// end ProveedorList

ProveedorList::~ProveedorList() {
    if (m_modo == EditMode)
        m_companyact->sacaWindow(this);
    guardaconfig();
}// end ~ProveedorList

void ProveedorList::presenta() {
    cursor2 * cur= m_companyact->cargacursor("SELECT * FROM proveedor WHERE nomproveedor LIKE'%"+m_filtro->text()+"%'");
    mui_list->cargar(cur);
    delete cur;
    s_configurar();
}// end presenta





void ProveedorList::on_mui_crear_clicked() {
    fprintf(stderr,"Iniciamos el boton_crear\n");
    provedit *prov = new provedit(m_companyact,0,theApp->translate("Edicion de Proveedores", "company"));
    m_companyact->m_pWorkspace->addWindow(prov);
    prov->show();
}// end boton_crear


void ProveedorList::s_findProvider() {
    presenta();
}// end s_findProvider


void ProveedorList::editar(int  row) {
    _depura("ProveedorList::editar",0);
    m_idprovider = mui_list->DBvalue(QString("idproveedor"),row);
    m_cifprovider = mui_list->DBvalue(QString("cifproveedor"),row);
    m_nomprovider = mui_list->DBvalue(QString("nomproveedor"),row);
    if (m_modo ==0 ) {
        provedit *prov = new provedit(m_companyact,0,theApp->translate("Edicion de Proveedores", "company"));
        if (prov->chargeprovider(mui_list->DBvalue(QString("idproveedor"),row))) {
            return;
        }
        m_companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(m_idprovider));
        // close();
    }// end if
    _depura("END ProveedorList::editar",0);
}

void ProveedorList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >=0 )
        editar(a);
    else
        _depura("Debe seleccionar una linea",2);
}// end s_editProvider


/** SLOT que responde a la pulsaci� de borrar un determinado proveedor
  * Dicha funci� avisa de la perdida de datos y si se decide continuar
  * Se procede a borrar el proveedor
  */
void ProveedorList::on_mui_borrar_clicked() {
    _depura("ProveedorList::on_mui_borrar_clicked",0);
    int row = mui_list->currentRow();
    provedit *prov = new provedit(m_companyact, 0);
    if (prov->chargeprovider(mui_list->DBvalue(QString("idproveedor"),row))) {
        return;
    }
    prov->boton_borrar();
    delete prov;
    _depura("END ProveedorList::on_mui_borrar_clicked",0);
}// end s_removeProvider


/** SLOT que se ejecuta al pulsar sobre el bot� de imprimir en la ventana de proveedores
  * La funci� llama a rtkview para generar el listado predefinido en reports/ProveedorList.rtk
  */
void ProveedorList::on_mui_imprimir_clicked() {
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

    cursor2 * cur= m_companyact->cargacursor("SELECT * FROM proveedor WHERE nomproveedor LIKE'%"+m_filtro->text()+"%'");
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



void ProveedorList::on_mui_exportar_clicked() {
    QFile filexml (Q3FileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER),"Proveedores (*.xml)", this, "select file", "Elija el Archivo"));
    if(filexml.open(QIODevice::WriteOnly)) {
        bulmafact2XML(filexml, IMPORT_PROVEEDORES);
        filexml.close();
    } else {
        _depura("ERROR AL ABRIR ARCHIVO\n",2);
    }// end if
}//


void ProveedorList::on_mui_importar_clicked() {
    QFile filexml (Q3FileDialog::getOpenFileName(confpr->valor(CONF_DIR_USER),"Proveedores (*.xml)", this, "select file", "Elija el Archivo"));
    if (filexml.open(QIODevice::ReadOnly))  {
        XML2BulmaFact(filexml, IMPORT_PROVEEDORES);
        filexml.close();
        presenta();
    }  else  {
        _depura("ERROR AL ABRIR ARCHIVO\n",2);
    }// end if
}



/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================

ProveedorListSubform::ProveedorListSubform(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("proveedor");
    setDBCampoId("idproveedor");
    addSHeader("idproveedor", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, "idproveedor");
    addSHeader("nomproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomproveedor");
    addSHeader("nomaltproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomaltproveedor");
    addSHeader("cifproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "cifproveedor");
    addSHeader("codicliproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "codicliproveedor");
    addSHeader("cbancproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "cbancproveedor");
    addSHeader("comentproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "comentproveedor");
    addSHeader("dirproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "dirproveedor");
    addSHeader("poblproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "poblproveedor");
    addSHeader("cpproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "cpproveedor");
    addSHeader("telproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "telproveedor");
    addSHeader("faxproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "faxproveedor");
    addSHeader("emailproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "emailproveedor");
    addSHeader("urlproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "urlproveedor");
    addSHeader("clavewebproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "clavewebproveedor");
    setinsercion(FALSE);
};


