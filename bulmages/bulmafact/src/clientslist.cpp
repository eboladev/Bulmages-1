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
#define COL_IDRECARGO 15

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
        for (int i = 0; i < m_clientList->numCols(); i++) {
            m_clientList->showColumn(i);
            stream << m_clientList->columnWidth(i) << "\n";
        }// end for
        file.close();
    }// end if
}// end guardaconfig()

void ClientsList::cargaconfig() {
    QFile file( confpr->valor(CONF_DIR_USER)+"confclientslist.cfn" );
    QString line;
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        line = stream.readLine(); // line of text excluding '\n'
        for (int i = 0; i < m_clientList->numCols(); i++) {
            QString linea = stream.readLine();
            m_clientList->setColumnWidth(i, linea.toInt());
        }// end for
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



void ClientsList::s_configurar() {
    if(mver_idcliente->isChecked() )
        m_clientList->showColumn(COL_IDCLIENTE);
    else
        m_clientList->hideColumn(COL_IDCLIENTE);

    if(mver_nomcliente->isChecked() )
        m_clientList->showColumn(COL_NOMCLIENTE);
    else
        m_clientList->hideColumn(COL_NOMCLIENTE);

    if(mver_nomaltcliente->isChecked() )
        m_clientList->showColumn(COL_NOMALTCLIENTE);
    else
        m_clientList->hideColumn(COL_NOMALTCLIENTE);

    if(mver_cifcliente->isChecked() )
        m_clientList->showColumn(COL_CIFCLIENTE);
    else
        m_clientList->hideColumn(COL_CIFCLIENTE);

    if(mver_bancocliente->isChecked() )
        m_clientList->showColumn(COL_BANCOCLIENTE);
    else
        m_clientList->hideColumn(COL_BANCOCLIENTE);

    if(mver_dircliente->isChecked() )
        m_clientList->showColumn(COL_DIRCLIENTE);
    else
        m_clientList->hideColumn(COL_DIRCLIENTE);

    if(mver_poblcliente->isChecked() )
        m_clientList->showColumn(COL_POBLCLIENTE);
    else
        m_clientList->hideColumn(COL_POBLCLIENTE);

    if(mver_cpcliente->isChecked() )
        m_clientList->showColumn(COL_CPCLIENTE);
    else
        m_clientList->hideColumn(COL_CPCLIENTE);

    if(mver_telcliente->isChecked() )
        m_clientList->showColumn(COL_TELCLIENTE);
    else
        m_clientList->hideColumn(COL_TELCLIENTE);

    if(mver_faxcliente->isChecked() )
        m_clientList->showColumn(COL_FAXCLIENTE);
    else
        m_clientList->hideColumn(COL_FAXCLIENTE);

    if(mver_mailcliente->isChecked() )
        m_clientList->showColumn(COL_MAILCLIENTE);
    else
        m_clientList->hideColumn(COL_MAILCLIENTE);

    if(mver_urlcliente->isChecked() )
        m_clientList->showColumn(COL_URLCLIENTE);
    else
        m_clientList->hideColumn(COL_URLCLIENTE);

    if(mver_faltacliente->isChecked() )
        m_clientList->showColumn(COL_FALTACLIENTE);
    else
        m_clientList->hideColumn(COL_FALTACLIENTE);

    if(mver_fbajacliente->isChecked() )
        m_clientList->showColumn(COL_FBAJACLIENTE);
    else
        m_clientList->hideColumn(COL_FBAJACLIENTE);

    if(mver_comentcliente->isChecked() )
        m_clientList->showColumn(COL_COMENTCLIENTE);
    else
        m_clientList->hideColumn(COL_COMENTCLIENTE);

    if(mver_idrecargo->isChecked() )
        m_clientList->showColumn(COL_IDRECARGO);
    else
        m_clientList->hideColumn(COL_IDRECARGO);

}// end s_configurar

ClientsList::ClientsList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag, edmode editmode)
        : ClientsListBase(parent, name, flag) , pgimportfiles(comp) {
    cargaconfig();

    s_configurar();
    companyact = comp;
    m_idclient="";
    m_cifclient="";
    m_nomclient="";
    m_mode = editmode;
    hideBusqueda();
    hideConfiguracion();



    m_clientList->setNumRows( 0 );
    m_clientList->setSelectionMode( Q3Table::SingleRow );
    m_clientList->setSorting( TRUE );
    m_clientList->setColumnMovingEnabled( TRUE );
    m_clientList->setNumCols(16);
    m_clientList->horizontalHeader()->setLabel( COL_IDCLIENTE, tr( "Código" ) );
    m_clientList->horizontalHeader()->setLabel( COL_NOMCLIENTE, tr( "Nombre Fiscal" ) );
    m_clientList->horizontalHeader()->setLabel( COL_NOMALTCLIENTE, tr( "Nombre Comercial" ) );
    m_clientList->horizontalHeader()->setLabel( COL_CIFCLIENTE, tr( "CIF/NIF" ) );
    m_clientList->horizontalHeader()->setLabel( COL_BANCOCLIENTE, tr( "Cuenta Bancaria" ) );
    m_clientList->horizontalHeader()->setLabel( COL_DIRCLIENTE, tr( "Domicilio" ) );
    m_clientList->horizontalHeader()->setLabel( COL_POBLCLIENTE, tr("Població") );
    m_clientList->horizontalHeader()->setLabel( COL_CPCLIENTE, tr("C.P.") );
    m_clientList->horizontalHeader()->setLabel( COL_TELCLIENTE, tr("Teléfono") );
    m_clientList->horizontalHeader()->setLabel( COL_FAXCLIENTE, tr("Fax") );
    m_clientList->horizontalHeader()->setLabel( COL_MAILCLIENTE, tr("Correo Electrónico") );
    m_clientList->horizontalHeader()->setLabel( COL_URLCLIENTE, tr("Página Web") );
    m_clientList->horizontalHeader()->setLabel( COL_FALTACLIENTE, tr("Fecha de Alta") );
    m_clientList->horizontalHeader()->setLabel( COL_FBAJACLIENTE, tr("Fecha de Baja") );
    m_clientList->horizontalHeader()->setLabel( COL_COMENTCLIENTE, tr("Observaciones") );
    m_clientList->horizontalHeader()->setLabel( COL_IDRECARGO, tr("Cod. Recargo Eq.") );

    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuración que es global.
    m_clientList->setPaletteBackgroundColor("#DDDDFF");
    m_clientList->setReadOnly(TRUE);

    presenta();
    /// Si estamos en el modo edicion metemos la ventana en el lugar apropiado.
    if (m_mode==EditMode)
        companyact->meteWindow(caption(),this);


}// end clientslist

ClientsList::~ClientsList() {
    if (m_mode==EditMode)
        companyact->sacaWindow(this);

    guardaconfig();
}// end ~clientslist


/**
  * Iniciamos los clientes.
  * Hacemos la consulta a la base de datos y presentamos el listado.
  */
void ClientsList::presenta() {
    cursor2 * cur= companyact->cargacursor("SELECT * FROM cliente  WHERE nomcliente LIKE '%"+m_findClient->text()+"%' ORDER BY nomcliente");
    m_clientList->setNumRows( cur->numregistros() );
    int i=0;
    while (!cur->eof()) {
        m_clientList->setText(i,COL_IDCLIENTE,cur->valor("idcliente"));
        m_clientList->setText(i,COL_NOMCLIENTE,cur->valor("nomcliente"));
        m_clientList->setText(i,COL_NOMALTCLIENTE,cur->valor("nomaltcliente"));
        m_clientList->setText(i,COL_CIFCLIENTE,cur->valor("cifcliente"));
        m_clientList->setText(i,COL_BANCOCLIENTE,cur->valor("bancocliente"));
        m_clientList->setText(i,COL_DIRCLIENTE,cur->valor("dircliente"));
        m_clientList->setText(i,COL_POBLCLIENTE,cur->valor("poblcliente"));
        m_clientList->setText(i,COL_CPCLIENTE,cur->valor("cpcliente"));
        m_clientList->setText(i,COL_TELCLIENTE,cur->valor("telcliente"));
        m_clientList->setText(i,COL_FAXCLIENTE,cur->valor("faxcliente"));
        m_clientList->setText(i,COL_MAILCLIENTE,cur->valor("mailcliente"));
        m_clientList->setText(i,COL_URLCLIENTE,cur->valor("urlcliente"));
        m_clientList->setText(i,COL_FALTACLIENTE,cur->valor("faltacliente"));
        m_clientList->setText(i,COL_FBAJACLIENTE,cur->valor("fbajacliente"));
        m_clientList->setText(i,COL_COMENTCLIENTE,cur->valor("comentcliente"));
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
}// end presenta


void ClientsList::m_clientList_clicked(int a, int , int , const QPoint &) {
    m_idclient = m_clientList->text(a,COL_IDCLIENTE);
}// end m_clientList_clicked


void ClientsList::editClient() {
    if (m_clientList->currentRow() < 0) {
        _depura("Debe seleccionar un elemento",2);
        return;
    }// end if
    m_clientList_doubleClicked(m_clientList->currentRow(), 0, 0, QPoint());

}// end editClient

/**
   *  Se ha hecho un doble click sobre la tabla de Clientes.
  */
void ClientsList::m_clientList_doubleClicked(int a, int , int , const QPoint &) {
    m_idclient = m_clientList->text(a,COL_IDCLIENTE);
    m_cifclient = m_clientList->text(a,COL_CIFCLIENTE);
    m_nomclient = m_clientList->text(a, COL_NOMCLIENTE);
    if (m_mode == EDIT_MODE ) {
//        ClientEdit *cli = new ClientEdit(companyact,0,theApp->translate("Edicion de Clientes", "company"));
	ClienteView *cli = companyact->newClienteView();
        if(cli->loadClient(m_idclient))
            return;
        companyact->m_pWorkspace->addWindow(cli);
        cli->show();
    } else {
        close();
    }// end if
}// end doubleClicked


void ClientsList::s_findClients() {
    presenta();
}// end s_findClients


void ClientsList::s_printClients() {
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

    cursor2 * cur= companyact->cargacursor("SELECT * FROM cliente  WHERE nomcliente LIKE '%"+m_findClient->text()+"%' ORDER BY nomcliente");
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


void ClientsList::s_borrarCliente() {
    m_idclient = m_clientList->text(m_clientList->currentRow(),COL_IDCLIENTE);
    QString SQLQuery = "DELETE FROM cliente WHERE idcliente = "+m_idclient;
    int error = companyact->ejecuta(SQLQuery);
    if (error) {
        return;
    }// end if
    presenta();
}// end s_borrarCliente


void ClientsList::s_exportar() {
    QFile filexml (Q3FileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER),"Clientes (*.xml)", this, "select file", "Elija el Archivo"));
    if(filexml.open(QIODevice::WriteOnly)) {
        bulmafact2XML(filexml, IMPORT_CLIENTES);
        filexml.close();
    } else {
        _depura("ERROR AL ABRIR ARCHIVO\n",2);
    }// end if
}//


void ClientsList::s_importar() {
    QFile filexml (Q3FileDialog::getOpenFileName(confpr->valor(CONF_DIR_USER),"Clientes (*.xml)", this, "select file", "Elija el Archivo"));
    if (filexml.open(QIODevice::ReadOnly))  {
        XML2BulmaFact(filexml, IMPORT_CLIENTES);
        filexml.close();
        presenta();
    }  else  {
        _depura("ERROR AL ABRIR ARCHIVO\n",2);
    }// end if
}

