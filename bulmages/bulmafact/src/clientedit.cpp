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
�dices:
    "cliente_pkey" llave primaria, btree (idcliente)
Restricciones de llave for�ea:
    "$1" FOREIGN KEY (idrecargo) REFERENCES recargo(idrecargo)
 
*/

#include "clientedit.h"
#include "company.h"
#include "division.h"
#include <qlineedit.h>
#include <qmessagebox.h>
#include <q3table.h>
#include <qtoolbutton.h>
#include <qwidget.h>
#include <qcombobox.h>

#include <QCloseEvent>

#include "budgetslist.h"
#include "pedidosclientelist.h"
#include "clientdelivnoteslist.h"
#include "cobroslist.h"
#include "facturaslist.h"
#include "busquedaprovincia.h"
#include "funcaux.h"


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
        : ClientEditBase(parent, name, Qt::WDestructiveClose) ,dialogChanges(this) {
    companyact = comp;

	m_provcliente->setcompany(companyact);
	m_provcliente->setProvincia("");

	/// Desabilitamos los tabs que aun no se usan
    tab->setTabEnabled(6,FALSE);
    tab->setTabEnabled(7,FALSE);
    tab->setTabEnabled(8,FALSE);


	/// Inicializamos las pantallas auxiliares a esta
    m_listpresupuestos->setcompany(companyact);
	m_listpresupuestos->hideConfiguracion();
    m_listpedidos->setcompany(companyact);
	m_listpedidos->hideConfiguracion();
    m_listalbaranes->setcompany(companyact);
	m_listalbaranes->hideConfiguracion();
    m_listfacturas->setcompany(companyact);
	m_listfacturas->hideConfiguracion();
    m_listcobros->setcompany(companyact);
	m_listcobros->hideConfiguracion();
    clientId = NEW_CLIENT;
    //clientId = "0";




    // Arreglamos la tabla de divisionoes.
    m_divisiones->setNumRows( 0 );
    m_divisiones->setNumCols( 0 );
    m_divisiones->setSelectionMode( Q3Table::SingleRow );
    m_divisiones->setSorting( TRUE );
    m_divisiones->setSelectionMode( Q3Table::SingleRow );
    m_divisiones->setColumnMovingEnabled( TRUE );
    m_divisiones->setNumCols(8);
    m_divisiones->horizontalHeader()->setLabel( COL_DIVISION_IDDIVISION, tr( "Id. Division" ) );
    m_divisiones->horizontalHeader()->setLabel( COL_DIVISION_DESCDIVISION, tr( "Descripci�" ) );
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

    companyact->meteWindow(caption(),this);

    dialogChanges_cargaInicial();
    fprintf(stderr,"Vamos a tratar el listado de presupuestos\n");
}// end ClientEdit

ClientEdit::~ClientEdit() {
    companyact->sacaWindow(this);
    companyact->refreshClientes();
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
        cursor2 *cur= companyact->cargacursor(SQLQuery);
        if (!cur->eof()) {
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
            m_clientComments->setText(cur->valor("comentcliente"));

            /// Ponemos la provincia correspondiente al cliente.
	    m_provcliente->setProvincia(cur->valor("provcliente"));

            /// Hacemos que el listado de presupuestos de un cliente se inicialize.
            m_listpresupuestos->setidcliente(cur->valor("idcliente"));
            m_listpresupuestos->inicializa();
            m_listpedidos->setidcliente(cur->valor("idcliente"));
            m_listpedidos->presenta();
            m_listalbaranes->setidcliente(cur->valor("idcliente"));
            m_listalbaranes->presenta();
            m_listfacturas->setidcliente(cur->valor("idcliente"));
            m_listfacturas->presenta();
            m_listcobros->setidcliente(cur->valor("idcliente"));
            m_listcobros->presenta();

            /// Cambiamos el titulo de la ventana para que salga reflejado donde toca.
            setCaption("Cliente "+cur->valor("nomcliente"));
            companyact->meteWindow(caption(),this);

        }// end if
        delete cur;

        dialogChanges_cargaInicial();
    }// end if
}// end loadClient



/**
* Empties the form.
*
*/
void ClientEdit::emptyForm() {
    clientId = "0";
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
    m_clientComments->setText("");
    m_provcliente->setCurrentItem(0);
    dialogChanges_cargaInicial();
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
        SQLQuery += " , provcliente='"+companyact->sanearCadena(m_provcliente->currentText())+"'";
        SQLQuery += " , comentcliente='"+companyact->sanearCadena(m_clientComments->text())+"'";


        SQLQuery += " WHERE idcliente ="+clientId;
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    } else {
        SQLQuery = " INSERT INTO cliente (nomcliente, nomaltcliente, cifcliente, bancocliente, dircliente, poblcliente, cpcliente, telcliente, faxcliente, urlcliente, mailcliente, provcliente, comentcliente)";
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
        SQLQuery += ",'"+companyact->sanearCadena(m_provcliente->currentText())+"'";
        SQLQuery += ",'"+companyact->sanearCadena(m_clientComments->text())+"'";
        SQLQuery += ")";
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        cursor2 *cur = companyact->cargacursor("SELECT max(idcliente) AS id FROM cliente");
        clientId = cur->valor("id");
        delete cur;
        companyact->commit();
    }// end if
    dialogChanges_cargaInicial();
}// end accept


/**
* For now this function cleans the form and sets it so that we can add
* a new client
*
* In the future it should really delete the client, or better yet
* mark it as deleted on an appropiate field in the DB
*/
void ClientEdit::deleteClient() {
    QString SQLQuery = "DELETE FROM cliente WHERE idcliente="+clientId;
    int error = companyact->ejecuta(SQLQuery);
	if (error) return;
    clientId = "";
    emptyForm();
}// end deleteClient()


void ClientEdit::saveButton_clicked() {
    saveClient();
}

void ClientEdit::deleteButton_clicked() {
    int ret = QMessageBox::warning(this,tr("Edici� de clientes"),
                                   tr("Est�a punto de borrar un cliente.\n"
                                      "Est�seguro de que quiere borrarlo?"),
                                   QMessageBox::Yes | QMessageBox::Default,
                                   QMessageBox::No,
                                   QMessageBox::Cancel | QMessageBox::Escape);
    if (ret == QMessageBox::Yes) {
        deleteClient();
    }// end if
}

/*
void ClientEdit::cancelButton_clicked() {
    if (m_modified) {
        int ret = QMessageBox::warning(this,tr("Edici� de clientes"),
                                       tr("El cliente ha sido modificado.\n"
                                          "Quiere guardar los cambios?"),
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
*/


void ClientEdit::closeEvent( QCloseEvent *e) {
	_depura("closeEvent",0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning( this, "Guardar Cliente",
                                   "Desea guardar los cambios.","Si","No","Cancelar",0,2);
	if (val == 0) 
            saveClient();
	if (val == 2)
	    e->ignore();
    }// end if	
}
