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

/*
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
*/


#define EDIT_MODE 0

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
    presenta();
    /// Si estamos en el modo edicion metemos la ventana en el lugar apropiado.
    if (m_modo==EditMode)
        m_companyact->meteWindow(caption(),this);
}


ClientsList::~ClientsList() {
    if (m_modo==EditMode)
        m_companyact->sacaWindow(this);
}

/**
  * Iniciamos los clientes.
  * Hacemos la consulta a la base de datos y presentamos el listado.
  */
void ClientsList::presenta() {
	_depura("ClientsList::presenta",0);
    cursor2 * cur= m_companyact->cargacursor("SELECT * FROM cliente  WHERE nomcliente LIKE '%"+m_findClient->text()+"%' ORDER BY nomcliente");
    mui_list->cargar(cur);
    delete cur;
//    configurar();
	_depura("END ClientsList::presenta",0);

}




void ClientsList::editar(int  row) {
    _depura("ClientsList::editar",0);
    mdb_idcliente = mui_list->DBvalue("idcliente");
    mdb_cifcliente = mui_list->DBvalue("cifcliente",row);
    mdb_nomcliente = mui_list->DBvalue("nomcliente",row);
    if (m_modo ==0 ) {
        ClienteView *prov = m_companyact->newClienteView();
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
    fitxersortidatxt += mui_list->imprimir();
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]",fitxersortidatxt);

    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }// end if

    // Crea el pdf  y lo muestra.
    _depura("Vamos a imprimir e listado de clientes",0);
    invocaPDF("clientes");
}// end s_printClients


void ClientsList::on_mui_borrar_clicked() {
    mdb_idcliente = mui_list->DBvalue("idcliente");
    QString SQLQuery = "DELETE FROM cliente WHERE idcliente = "+mdb_idcliente;
    int error = m_companyact->ejecuta(SQLQuery);
    if (error)
        return;
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







