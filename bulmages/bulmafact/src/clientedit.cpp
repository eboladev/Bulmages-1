/***************************************************************************
 *   Copyright (C) 2004 by Marcos Mezo                                     *
 *   mmezo@selexco.net                                                     *
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
-- client table
---------------+-------------------------+----------------------------------------------------------------
 idcliente     | integer                 | not null default nextval('public.cliente_idcliente_seq'::text)
 nomcliente    | character varying(100)  |
 nomaltcliente | character varying(300)  |
 cifcliente    | character varying(200)  |
 bancocliente  | character varying(35)   |
 dircliente    | character varying(100)  |
 poblcliente   | character varying(40)   |
 cpcliente     | character varying(10)   |
 telcliente    | character varying(20)   |
 faxcliente    | character varying(20)   |
 mailcliente   | character varying(100)  |
 urlcliente    | character varying(150)  |
 faltacliente  | date                    | default now()
 fbajacliente  | date                    |
 comentcliente | character varying(2000) |
 idrecargo     | integer                 | not null
Índices:
    "cliente_pkey" llave primaria, btree (idcliente)
Restricciones de llave foránea:
    "$1" FOREIGN KEY (idrecargo) REFERENCES recargo(idrecargo)

*/
 
#include "clientedit.h"
#include "company.h"
#include "division.h"
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qtable.h>
#include <qtoolbutton.h>

#define COL_DIVISION_IDDIVISION 0
#define COL_DIVISION_DESCDIVISION 1
#define COL_DIVISION_CONTACTODIVISION 2
#define COL_DIVISION_COMENTDIVISION 3
#define COL_DIVISION_TELDIVISION 4
#define COL_DIVISION_FAXDIVISION 5
#define COL_DIVISION_MAILDIVISION 6
#define COL_DIVISION_IDPROVEEDOR 7

#define NEW_CLIENT "0"
ClientEdit::ClientEdit(company *comp, QWidget *parent, const char *name)
 : ClientEditBase(parent, name) {
   companyact = comp;
   clientId = NEW_CLIENT;
   //clientId = "0";
   
   // Arreglamos la tabla de divisionoes.
   m_divisiones->setNumRows( 0 );
   m_divisiones->setNumCols( 0 );
   m_divisiones->setSelectionMode( QTable::SingleRow );
   m_divisiones->setSorting( TRUE );
   m_divisiones->setSelectionMode( QTable::SingleRow );
   m_divisiones->setColumnMovingEnabled( TRUE );
   m_divisiones->setNumCols(8);
   m_divisiones->horizontalHeader()->setLabel( COL_DIVISION_IDDIVISION, tr( "Id. Division" ) );
   m_divisiones->horizontalHeader()->setLabel( COL_DIVISION_DESCDIVISION, tr( "Descripción" ) );
   m_divisiones->horizontalHeader()->setLabel( COL_DIVISION_CONTACTODIVISION, tr( "Contacto" ) );
   m_divisiones->horizontalHeader()->setLabel( COL_DIVISION_COMENTDIVISION, tr( "Comentarios" ) );
   m_divisiones->horizontalHeader()->setLabel( COL_DIVISION_TELDIVISION, tr( "Telefono" ) );
   m_divisiones->horizontalHeader()->setLabel( COL_DIVISION_FAXDIVISION, tr( "Fax" ) );
   m_divisiones->horizontalHeader()->setLabel( COL_DIVISION_MAILDIVISION, tr( "E-mail" ) );
   m_divisiones->horizontalHeader()->setLabel( COL_DIVISION_IDPROVEEDOR, tr( "idproveedor" ) );
   
   m_divisiones->setColumnWidth(COL_DIVISION_IDDIVISION,100);
   m_divisiones->setColumnWidth(COL_DIVISION_DESCDIVISION,300);
   m_divisiones->setColumnWidth(COL_DIVISION_CONTACTODIVISION,100);
   m_divisiones->setColumnWidth(COL_DIVISION_COMENTDIVISION,100);
   m_divisiones->setColumnWidth(COL_DIVISION_TELDIVISION,100);
   m_divisiones->setColumnWidth(COL_DIVISION_FAXDIVISION,100);
   m_divisiones->setColumnWidth(COL_DIVISION_MAILDIVISION,100);
   m_divisiones->setColumnWidth(COL_DIVISION_IDPROVEEDOR,100);
   
   emptyForm();
   companyact->meteWindow(caption(),this);
}// end ClientEdit

ClientEdit::~ClientEdit() {
	companyact->sacaWindow(this);
}// end ~ClientEdit

/**
* loadClient
*
* Given a valid client ID this function loads the client into the
* form so that we can edit it.
* 
* Otherwise it empties the form and sets it so that we can add
* a new client
*/
void ClientEdit::loadClient(QString client) {
   clientId = client;
   
   if (clientId != "0") {
      QString SQLQuery = "SELECT * FROM cliente WHERE idcliente="+clientId;
      companyact->begin();
      cursor2 *cur= companyact->cargacursor(SQLQuery, "unquery");
      companyact->commit();
      if (!cur->eof()) {
         m_clientId->setText(cur->valor("idcliente"));
         m_clientName->setText(cur->valor("nomcliente"));
         m_altClientName->setText(cur->valor("nomaltcliente"));
         m_clientCif->setText(cur->valor("cifcliente"));
         m_clientBankAccount->setText(cur->valor("bancocliente"));
         m_clientAddress->setText(cur->valor("dircliente"));
         m_clientCity->setText(cur->valor("poblcliente"));
         m_clientZipCode->setText(cur->valor("cpcliente"));
         m_clientPhone->setText(cur->valor("telcliente"));
         m_clientFax->setText(cur->valor("faxcliente"));
         m_clientEmail->setText(cur->valor("mailcliente"));
         m_clientUrl->setText(cur->valor("urlcliente"));
         
         /*
         
         // Providers division loading
         // Cargamos las divisiones del proveedor.
         QString SQLQuery1 = "SELECT * FROM division WHERE idproveedor="+idprov;
         companyact->begin();
         cursor2 *cur1 = companyact->cargacursor(SQLQuery1, "cargadivisiones");
         companyact->commit();
         m_divisiones->setNumRows(cur1->numregistros());
         int i=0;
         while (!cur1->eof()) {
            m_divisiones->setText(i,COL_DIVISION_IDDIVISION,cur1->valor("iddivision"));
            m_divisiones->setText(i,COL_DIVISION_DESCDIVISION,cur1->valor("descdivision"));
            m_divisiones->setText(i,COL_DIVISION_CONTACTODIVISION,cur1->valor("contactodivision"));
            m_divisiones->setText(i,COL_DIVISION_COMENTDIVISION,cur1->valor("comentdivision"));
            m_divisiones->setText(i,COL_DIVISION_TELDIVISION,cur1->valor("teldivision"));
            m_divisiones->setText(i,COL_DIVISION_FAXDIVISION,cur1->valor("faxdivision"));
            m_divisiones->setText(i,COL_DIVISION_MAILDIVISION,cur1->valor("maildivision"));
            m_divisiones->setText(i++,COL_DIVISION_IDPROVEEDOR,cur1->valor("idproveedor"));
            cur1->siguienteregistro();
         }// end while
         delete cur1;
         
         */
      } else {
          emptyForm();
      }// end if
      delete cur;

      setModified(false);      
   }// end if
}// end loadClient


/**
* Toggles the status of the onscreen client modified/not modified.
* Also performs the necesary gui actions in the window caption to reflect it.
*/
void ClientEdit::setModified(bool formContentsChanged) {   
   if (formContentsChanged) {
      modified = true;
      saveButton->setEnabled(true);
      setCaption(tr("Cliente (modificado)"));
   } else {
      modified = false;
      saveButton->setEnabled(false);
      setCaption(tr("Cliente"));
   }
}// end setModified


/**
* Empties the form.
*
*/
void ClientEdit::emptyForm() {
   
      deleteButton->setEnabled(false);
      
      clientId = "0";
      
      m_clientId->setText( tr("Cliente nuevo"));
      m_clientName->setText("");
      m_altClientName->setText("");
      m_clientCif->setText("");
      m_clientBankAccount->setText("");
      m_clientAddress->setText("");
      m_clientCity->setText("");
      m_clientZipCode->setText("");
      m_clientPhone->setText("");
      m_clientFax->setText("");
      m_clientEmail->setText("");
      m_clientUrl->setText("");
      
      setModified(false);
}// end newClient


/**
* saveClient
*
* This function saves the current client. It checks
* if it is a new client that needs to be added or if
* it is an existing one that has to be modified
*/
void ClientEdit::saveClient() {
   QString SQLQuery; 
   
   if (clientId != "0") {
      SQLQuery = "UPDATE cliente SET ";
      SQLQuery += " urlcliente='"+companyact->sanearCadena(m_clientUrl->text())+"'";
      SQLQuery += " , nomcliente='"+companyact->sanearCadena(m_clientName->text())+"'";
      SQLQuery += " , nomaltcliente='"+companyact->sanearCadena(m_altClientName->text())+"'";
      SQLQuery += " , cifcliente='"+companyact->sanearCadena(m_clientCif->text())+"'";
      SQLQuery += " , bancocliente='"+companyact->sanearCadena(m_clientBankAccount->text())+"'";
      SQLQuery += " , dircliente='"+companyact->sanearCadena(m_clientAddress->text())+"'";
      SQLQuery += " , poblcliente='"+companyact->sanearCadena(m_clientCity->text())+"'";
      SQLQuery += " , cpcliente='"+companyact->sanearCadena(m_clientZipCode->text())+"'";
      SQLQuery += " , telcliente='"+companyact->sanearCadena(m_clientPhone->text())+"'";
      SQLQuery += " , faxcliente='"+companyact->sanearCadena(m_clientFax->text())+"'";
      SQLQuery += " , mailcliente='"+companyact->sanearCadena(m_clientEmail->text())+"'";
      SQLQuery += " WHERE idcliente ="+clientId;
   } else {
      SQLQuery = " INSERT INTO cliente (nomcliente, nomaltcliente, cifcliente, bancocliente, dircliente, poblcliente, cpcliente, telcliente, faxcliente, urlcliente, mailcliente)";
      SQLQuery += " VALUES (";
      SQLQuery += "'"+companyact->sanearCadena(m_clientName->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_altClientName->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_clientCif->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_clientBankAccount->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_clientAddress->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_clientCity->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_clientZipCode->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_clientPhone->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_clientFax->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_clientUrl->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_clientEmail->text())+"'";
      SQLQuery += ")";
   }// end if
   
   companyact->begin();
   companyact->ejecuta(SQLQuery);
   companyact->commit(); 
   
   setModified(false);  
}// end accept


/**
* For now this function cleans the form and sets it so that we can add
* a new client
*
* In the future it should really delete the client, or better yet
* mark it as deleted on an appropiate field in the DB
*/
void ClientEdit::deleteClient() {
   QMessageBox::information(this,tr("Edición de clientes"),
         tr("Qué peligro tienes, majete!\n"
            "En vez de salir este mensaje tendría que borrar el cliente."),
         QMessageBox::Yes | QMessageBox::Default,
         QMessageBox::No,
         QMessageBox::Cancel | QMessageBox::Escape);
   emptyForm();
}// end deleteClient()


void ClientEdit::saveButton_clicked() {
   saveClient();
   setModified(false);
}

void ClientEdit::deleteButton_clicked() {
   int ret = QMessageBox::warning(this,tr("Edición de clientes"),
                  tr("Está a punto de borrar un cliente.\n"
                     "¿Está seguro de que quiere borrarlo?"),
                  QMessageBox::Yes | QMessageBox::Default,
                  QMessageBox::No,
                  QMessageBox::Cancel | QMessageBox::Escape);
   if (ret == QMessageBox::Yes) {
      deleteClient();
      close();
   } else if (ret == QMessageBox::No) 
      close();
}


void ClientEdit::cancelButton_clicked() {
   if (modified) {
      int ret = QMessageBox::warning(this,tr("Edición de clientes"),
                     tr("El cliente ha sido modificado.\n"
                        "¿Quiere guardar los cambios?"),
                     QMessageBox::Yes | QMessageBox::Default,
                     QMessageBox::No,
                     QMessageBox::Cancel | QMessageBox::Escape);
      if (ret == QMessageBox::Yes) {
         saveClient();
         close();
      } else if (ret == QMessageBox::No) 
         close();
   } else {
      close();
   } 
   
}

void ClientEdit::formModified() {
   setModified(true);
}// end boton_newdivision
