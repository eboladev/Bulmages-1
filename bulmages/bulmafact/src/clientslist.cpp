/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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
-- El cliente siempre tiene la razón, bueno, o por lo menos eso cree.
--Codi: Clau artificial.
--Nom: Nom comercial o fiscal.
--Nom_alternatiu: Nom comercial o fiscal.
--CIF: Codi d'Identificación Fiscal.
--C_Banc: Compte Bancari.
--Adr: Adreça.
--Pobl: Població
--CProv: Codi de provincia (dos primers d�its del codi postal).
--sCP: Tres darrers dígits del codi postal.
--Telf: Teléfon.
--Fax: Fax.
--Email: eMail.
--Url: Url.
--Data_alta
--Data_Baixa
---Comentaris
CREATE TABLE cliente (
  idcliente serial PRIMARY KEY,
  nomcliente character varying(100),
  nomaltcliente character varying(300),
  cifcliente character varying(200),
  bancocliente character varying(35),
  dircliente character varying(100),
  poblcliente character varying(40),
  cpcliente character varying(10),
  telcliente character varying(20),
  faxcliente character varying(20),
  mailcliente character varying(100),
  urlcliente character varying(150),
  faltacliente date DEFAULT NOW(),
  fbajacliente date,
  comentcliente character varying(2000),
  
  idrecargo integer NOT NULL REFERENCES recargo(idrecargo)
);
*/

#include "clientslist.h"
#include "clienteview.h"
#include "company.h"
#include <QMessageBox>
#include "qtable1.h"
#include "funcaux.h"
#include <QLineEdit>
//Added by qt3to4:
#include <QTextStream>
#include "pgimportfiles.h"
#include <Q3FileDialog>
#include <QCheckBox>

#define COL_IDCLIENTE 0
#define COL_NOMCLIENTE 1
#define COL_NOMALTCLIENTE 2
#define COL_CIFCLIENTE 3
#define COL_BANCOCLIENTE 4
#define COL_DIRCLIENTE 5
#define COL_POBLCLIENTE 6
#define COL_CPCLIENTE 7
#define COL_TELCLIENTE 8
#define COL_FAXCLIENTE 9
#define COL_MAILCLIENTE 10
#define COL_URLCLIENTE 11
#define COL_FALTACLIENTE 12
#define COL_FBAJACLIENTE 13
#define COL_COMENTCLIENTE 14


#define EDIT_MODE 0

void ClientsList::guardaconfig() {
    QString aux = "";
    mver_idcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_nomcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_nomaltcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_cifcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_bancocliente->isChecked() ? aux += "1,":aux+="0,";
    mver_dircliente->isChecked() ? aux += "1,":aux+="0,";
    mver_poblcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_cpcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_telcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_faxcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_mailcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_urlcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_faltacliente->isChecked() ? aux += "1,":aux+="0,";
    mver_fbajacliente->isChecked() ? aux += "1,":aux+="0,";
    mver_comentcliente->isChecked() ? aux += "1,":aux+="0,";
    mver_idrecargo->isChecked() ? aux += "1,":aux+="0,";

    QFile file( confpr->valor(CONF_DIR_USER)+"confclientslist.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << aux << "\n";
        file.close();
    }// end if
}// end guardaconfig()

void ClientsList::cargaconfig() {
	_depura("ClientsList::cargaconfig",0);
    QFile file( confpr->valor(CONF_DIR_USER)+"confclientslist.cfn" );
    QString line;
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        line = stream.readLine(); // line of text excluding '\n'
        file.close();
    } else
        return;

    mver_idcliente->setChecked(line.at(0)=='1');
    mver_nomcliente->setChecked(line.at(2)=='1');
    mver_nomaltcliente->setChecked(line.at(4)=='1');
    mver_cifcliente->setChecked(line.at(6)=='1');
    mver_bancocliente->setChecked(line.at(8)=='1');
    mver_dircliente->setChecked(line.at(10)=='1');
    mver_poblcliente->setChecked(line.at(12)=='1');
    mver_cpcliente->setChecked(line.at(14)=='1');
    mver_telcliente->setChecked(line.at(16)=='1');
    mver_faxcliente->setChecked(line.at(18)=='1');
    mver_mailcliente->setChecked(line.at(20)=='1');
    mver_urlcliente->setChecked(line.at(22)=='1');
    mver_faltacliente->setChecked(line.at(24)=='1');
    mver_fbajacliente->setChecked(line.at(26)=='1');
    mver_comentcliente->setChecked(line.at(28)=='1');
    mver_idrecargo->setChecked(line.at(30)=='1');
}// end cargaconfig



void ClientsList::configurar() {
	_depura("ClientsList::configurar",0);
    if(mver_idcliente->isChecked() )
        mui_list->showColumn(COL_IDCLIENTE);
    else
        mui_list->hideColumn(COL_IDCLIENTE);

    if(mver_nomcliente->isChecked() )
        mui_list->showColumn(COL_NOMCLIENTE);
    else
        mui_list->hideColumn(COL_NOMCLIENTE);

    if(mver_nomaltcliente->isChecked() )
        mui_list->showColumn(COL_NOMALTCLIENTE);
    else
        mui_list->hideColumn(COL_NOMALTCLIENTE);

    if(mver_cifcliente->isChecked() )
        mui_list->showColumn(COL_CIFCLIENTE);
    else
        mui_list->hideColumn(COL_CIFCLIENTE);

    if(mver_bancocliente->isChecked() )
        mui_list->showColumn(COL_BANCOCLIENTE);
    else
        mui_list->hideColumn(COL_BANCOCLIENTE);

    if(mver_dircliente->isChecked() )
        mui_list->showColumn(COL_DIRCLIENTE);
    else
        mui_list->hideColumn(COL_DIRCLIENTE);

    if(mver_poblcliente->isChecked() )
        mui_list->showColumn(COL_POBLCLIENTE);
    else
        mui_list->hideColumn(COL_POBLCLIENTE);

    if(mver_cpcliente->isChecked() )
        mui_list->showColumn(COL_CPCLIENTE);
    else
        mui_list->hideColumn(COL_CPCLIENTE);

    if(mver_telcliente->isChecked() )
        mui_list->showColumn(COL_TELCLIENTE);
    else
        mui_list->hideColumn(COL_TELCLIENTE);

    if(mver_faxcliente->isChecked() )
        mui_list->showColumn(COL_FAXCLIENTE);
    else
        mui_list->hideColumn(COL_FAXCLIENTE);

    if(mver_mailcliente->isChecked() )
        mui_list->showColumn(COL_MAILCLIENTE);
    else
        mui_list->hideColumn(COL_MAILCLIENTE);

    if(mver_urlcliente->isChecked() )
        mui_list->showColumn(COL_URLCLIENTE);
    else
        mui_list->hideColumn(COL_URLCLIENTE);

    if(mver_faltacliente->isChecked() )
        mui_list->showColumn(COL_FALTACLIENTE);
    else
        mui_list->hideColumn(COL_FALTACLIENTE);

    if(mver_fbajacliente->isChecked() )
        mui_list->showColumn(COL_FBAJACLIENTE);
    else
        mui_list->hideColumn(COL_FBAJACLIENTE);

    if(mver_comentcliente->isChecked() )
        mui_list->showColumn(COL_COMENTCLIENTE);
    else
        mui_list->hideColumn(COL_COMENTCLIENTE);

	_depura("END ClientsList::configurar",0);
}// end s_configurar

ClientsList::ClientsList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag, edmode editmode)
        : QWidget(parent, name, flag) , pgimportfiles(comp) {
    setupUi(this);
    m_companyact = comp;
    mui_list->setcompany(comp);
    mdb_idcliente="";
    mdb_cifcliente="";
    mdb_nomcliente="";
    m_modo = editmode;
    hideBusqueda();
    hideConfiguracion();
    cargaconfig();
    presenta();
    /// Si estamos en el modo edicion metemos la ventana en el lugar apropiado.
    if (m_modo==EditMode)
        m_companyact->meteWindow(caption(),this);
}// end clientslist


ClientsList::~ClientsList() {
    if (m_modo==EditMode)
        m_companyact->sacaWindow(this);
    guardaconfig();
}// end ~clientslist


/**
  * Iniciamos los clientes.
  * Hacemos la consulta a la base de datos y presentamos el listado.
  */
void ClientsList::presenta() {
	_depura("ClientsList::presenta",0);
    cursor2 * cur= m_companyact->cargacursor("SELECT * FROM cliente  WHERE nomcliente LIKE '%"+m_findClient->text()+"%' ORDER BY nomcliente");
    mui_list->cargar(cur);
    delete cur;
    configurar();
	_depura("END ClientsList::presenta",0);

}// end presenta




void ClientsList::editar(int  row) {
    _depura("ClientsList::editar",0);
    mdb_idcliente = mui_list->DBvalue("idcliente");
    mdb_cifcliente = mui_list->DBvalue("cifcliente",row);
    mdb_nomcliente = mui_list->DBvalue("nomcliente",row);
    if (m_modo ==0 ) {
        ClienteView *prov = new ClienteView(m_companyact,0,theApp->translate("Edicion de Pedidos a Cliente", "company"));
        if (prov->cargar(mdb_idcliente)) {
            return;
        }
        m_companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(mdb_idcliente));
        // close();
    }// end if
    _depura("END ClientsList::editar",0);
}

void ClientsList::on_mui_editar_clicked() {
    if (mui_list->currentRow() < 0) {
        _depura("Debe seleccionar un elemento",2);
        return;
    }// end if
    editar(mui_list->currentRow());

}// end editClient






void ClientsList::on_mui_imprimir_clicked() {
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"clientes.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"clientes.rml";
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
    if(mver_idcliente->isChecked() )
        fitxersortidatxt += "	<td>Id. Cliente</td>";
    if(mver_nomcliente->isChecked() )
        fitxersortidatxt += "	<td>Nombre</td>";
    if(mver_nomaltcliente->isChecked() )
        fitxersortidatxt += "	<td>Nombre Alt.</td>";
    if(mver_cifcliente->isChecked() )
        fitxersortidatxt += "	<td>CIF</td>";
    if(mver_bancocliente->isChecked() )
        fitxersortidatxt += "	<td>Banco</td>";
    if(mver_dircliente->isChecked() )
        fitxersortidatxt += "	<td>Direccion</td>";
    if(mver_poblcliente->isChecked() )
        fitxersortidatxt += "	<td>Poblacion</td>";
    if(mver_cpcliente->isChecked() )
        fitxersortidatxt += "	<td>CP</td>";
    if(mver_telcliente->isChecked() )
        fitxersortidatxt += "	<td>Tel.</td>";
    if(mver_faxcliente->isChecked() )
        fitxersortidatxt += "	<td>Fax</td>";
    if(mver_mailcliente->isChecked() )
        fitxersortidatxt += "	<td>E-Mail</td>";
    if(mver_urlcliente->isChecked() )
        fitxersortidatxt += "	<td>URL</td>";
    if(mver_faltacliente->isChecked() )
        fitxersortidatxt += "	<td>F. Alta</td>";
    if(mver_fbajacliente->isChecked() )
        fitxersortidatxt += "	<td>F. Baja</td>";
    if(mver_comentcliente->isChecked() )
        fitxersortidatxt += "	<td>Comentarios</td>";
    if(mver_idrecargo->isChecked() )
        fitxersortidatxt += "	<td>Id. Recargo</td>";
    /// ------------------------------------------------
    fitxersortidatxt += "</tr>";

    cursor2 * cur= m_companyact->cargacursor("SELECT * FROM cliente  WHERE nomcliente LIKE '%"+m_findClient->text()+"%' ORDER BY nomcliente");
    while(!cur->eof()) {
        fitxersortidatxt += "<tr>";
        /// -----------------------------
        if(mver_idcliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idcliente"))+"</td>";
        if(mver_nomcliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("nomcliente"))+"</td>";
        if(mver_nomaltcliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("nomaltcliente"))+"</td>";
        if(mver_cifcliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("cifcliente"))+"</td>";
        if(mver_bancocliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("bancocliente"))+"</td>";
        if(mver_dircliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("dircliente"))+"</td>";
        if(mver_poblcliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("poblcliente"))+"</td>";
        if(mver_cpcliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("cpcliente"))+"</td>";
        if(mver_telcliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("telcliente"))+"</td>";
        if(mver_faxcliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("faxcliente"))+"</td>";
        if(mver_mailcliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("mailcliente"))+"</td>";
        if(mver_urlcliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("urlcliente"))+"</td>";
        if(mver_faltacliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("faltacliente"))+"</td>";
        if(mver_fbajacliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("fbajacliente"))+"</td>";
        if(mver_comentcliente->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("comentcliente"))+"</td>";
        if(mver_idrecargo->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idrecargo"))+"</td>";
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
    _depura("Vamos a imprimir e listado de clientes",0);
    invocaPDF("clientes");
}// end s_printClients


void ClientsList::on_mui_borrar_clicked() {
    mdb_idcliente = mui_list->item(mui_list->currentRow(),COL_IDCLIENTE)->text();
    QString SQLQuery = "DELETE FROM cliente WHERE idcliente = "+mdb_idcliente;
    int error = m_companyact->ejecuta(SQLQuery);
    if (error) {
        return;
    }// end if
    presenta();
}// end s_borrarCliente


void ClientsList::on_mui_exportar_clicked() {
    QFile filexml (Q3FileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER),"Clientes (*.xml)", this, "select file", "Elija el Archivo"));
    if(filexml.open(QIODevice::WriteOnly)) {
        bulmafact2XML(filexml, IMPORT_CLIENTES);
        filexml.close();
    } else {
        _depura("ERROR AL ABRIR ARCHIVO\n",2);
    }// end if
}//


void ClientsList::on_mui_importar_clicked() {
    QFile filexml (Q3FileDialog::getOpenFileName(confpr->valor(CONF_DIR_USER),"Clientes (*.xml)", this, "select file", "Elija el Archivo"));
    if (filexml.open(QIODevice::ReadOnly))  {
        XML2BulmaFact(filexml, IMPORT_CLIENTES);
        filexml.close();
        presenta();
    }  else  {
        _depura("ERROR AL ABRIR ARCHIVO\n",2);
    }// end if
}




/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================


ClienteListSubform::ClienteListSubform(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("cliente");
    setDBCampoId("idcliente");
    addSHeader("idcliente", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, "idcliente");
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomcliente");
    addSHeader("nomaltcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomaltcliente");
    addSHeader("cifcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "cifcliente");
    addSHeader("bancocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "bancocliente");
    addSHeader("dircliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "dircliente");
    addSHeader("poblcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "poblcliente");
    addSHeader("cpcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "cpcliente");
    addSHeader("telcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "telcliente");
    addSHeader("faxcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "faxcliente");
    addSHeader("mailcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "mailcliente");
    addSHeader("urlcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "urlcliente");
    addSHeader("faltacliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "faltacliente");
    addSHeader("fbajacliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "fbajacliente");
    addSHeader("comentcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "comentcliente");
    setinsercion(FALSE);
};







