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
-- COMPROVACIONS D'INTEGRITAT>Genèriques:
-- 1 Article té 1 sol proveïdor principal.
-- 1 Article té 1 sol proveïdor referent.
-- CAMPOS
-- ======
-- Codi: Clau artificial.
-- Nom: Nom comercial o fiscal.
-- Nom_alternatiu: Nom comercial o fiscal.
-- CIF: Codi d'Identificació Fiscal.
-- CodiCli: Codi de client amb que ens facturen. Útil per a identificar-nos.
-- C_Banc
-- Comentaris
-- Adreça: Adreça.
-- Població: Població.
-- CProv: Codi de provincia (dos primers dígits del codi postal).
-- sCP: Tres darrers dígits del codi postal.
-- Telf: Telèfon.
-- Fax: Fax.
-- Email: eMail.
-- Url: Url.
-- CompteWeb: Dades de login si disposen de tenda o tarifes en línia
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
   clavewebproveedor character varying(100)
);
*/
 
#include "provedit.h"
#include "company.h"
#include "division.h"
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qtable.h>

#define COL_DIVISION_IDDIVISION 0
#define COL_DIVISION_DESCDIVISION 1
#define COL_DIVISION_CONTACTODIVISION 2
#define COL_DIVISION_COMENTDIVISION 3
#define COL_DIVISION_TELDIVISION 4
#define COL_DIVISION_FAXDIVISION 5
#define COL_DIVISION_MAILDIVISION 6
#define COL_DIVISION_IDPROVEEDOR 7


provedit::provedit(company *comp, QWidget *parent, const char *name)
 : provedit_base(parent, name, Qt::WDestructiveClose) {
   companyact = comp;
   idprovider = "0";
   
   
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
   companyact->meteWindow(caption(),this);
   s_releaseModificado();

}// end provedit

provedit::~provedit() {
	companyact->sacaWindow(this);
}// end ~provedit


/************************************************************************
* Esta función carga un proveedor de la base de datos y lo presenta.    *
* Si el parametro pasado no es un identificador válido entonces se pone *
* la ventana de edición en modo de inserción                            *
*************************************************************************/
void provedit::chargeprovider(QString idprov) {
   idprovider = idprov;
   fprintf(stderr,"chargeprovider activado \n");
   if (idprovider != "0") {
      QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor="+idprov;
      companyact->begin();
      cursor2 *cur= companyact->cargacursor(SQLQuery, "unquery");
      companyact->commit();
      if (!cur->eof()) {
         idprovider = idprov;
         m_idproveedor->setText(cur->valor("idproveedor"));
         m_nomproveedor->setText(cur->valor("nomproveedor"));
         m_nomaltproveedor->setText(cur->valor("nomaltproveedor"));
         m_cifproveedor->setText(cur->valor("cifproveedor"));
         m_codicliproveedor->setText(cur->valor("codicliproveedor"));
         m_cbancproveedor->setText(cur->valor("cbancproveedor"));
         m_dirproveedor->setText(cur->valor("dirproveedor"));
         m_poblproveedor->setText(cur->valor("poblproveedor"));
         m_cpproveedor->setText(cur->valor("cpproveedor"));
         m_telproveedor->setText(cur->valor("telproveedor"));
         m_faxproveedor->setText(cur->valor("faxproveedor"));
         m_emailproveedor->setText(cur->valor("emailproveedor"));
         m_urlproveedor->setText(cur->valor("urlproveedor"));
         
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
      } else {
         idprovider="0";
      }// end if
      delete cur;
   }// end if
   s_releaseModificado();
}// end chargeprovider

/************************************************************************
* Esta función se ejecuta cuando se ha pulsado sobre el botón de nuevo  *
*************************************************************************/
void provedit::boton_nuevo() {
   if (m_modificado  ) {
    	    if ( QMessageBox::warning( this, "Guardar Proveedor",
		"Desea guardar los cambios.",
		QMessageBox::Ok ,
		QMessageBox::Cancel ) == QMessageBox::Ok)
		s_saveProvider();	   
   }// end if

      idprovider = "0";
      m_idproveedor->setText("");
      m_nomproveedor->setText("");
      m_nomaltproveedor->setText("");
      m_cifproveedor->setText("");
      m_codicliproveedor->setText("");
      m_cbancproveedor->setText("");
      m_dirproveedor->setText("");
      m_poblproveedor->setText("");
      m_cpproveedor->setText("");
      m_telproveedor->setText("");
      m_faxproveedor->setText("");
      m_emailproveedor->setText("");
      m_urlproveedor->setText("");
      s_releaseModificado();
}// end boton_nuevo

/*************************************************************************
* Esta función es la respuesta a la pulsación del boton de guardar       *
* Comprueba si es una inserción o una modificación y hace los pasos      *
* pertinentes                                                            *
**************************************************************************/
void provedit::s_saveProvider() {
   if (idprovider != "0") {
      QString SQLQuery = "UPDATE proveedor SET urlproveedor='"+m_urlproveedor->text()+"'";
      SQLQuery += " , nomproveedor='"+m_nomproveedor->text()+"'";
      SQLQuery += " , nomaltproveedor='"+m_nomaltproveedor->text()+"'";
      SQLQuery += " , cifproveedor='"+m_cifproveedor->text()+"'";
      SQLQuery += " , codicliproveedor='"+m_codicliproveedor->text()+"'";
      SQLQuery += " , cbancproveedor='"+m_cbancproveedor->text()+"'";
      SQLQuery += " , dirproveedor='"+m_dirproveedor->text()+"'";
      SQLQuery += " , poblproveedor='"+m_poblproveedor->text()+"'";
      SQLQuery += " , cpproveedor='"+m_cpproveedor->text()+"'";
      SQLQuery += " , telproveedor='"+m_telproveedor->text()+"'";
      SQLQuery += " , faxproveedor='"+m_faxproveedor->text()+"'";
      SQLQuery += " , emailproveedor='"+m_emailproveedor->text()+"'";
      SQLQuery += " WHERE idproveedor ="+idprovider;
      companyact->begin();
      companyact->ejecuta(SQLQuery);
      companyact->commit();
   } else {
      QString SQLQuery = " INSERT INTO proveedor (nomproveedor, nomaltproveedor, cifproveedor, codicliproveedor, cbancproveedor, dirproveedor, poblproveedor, cpproveedor, telproveedor, faxproveedor, urlproveedor, emailproveedor)";
      SQLQuery += " VALUES (";
      SQLQuery += "'"+m_nomproveedor->text()+"'";
      SQLQuery += ",'"+m_nomaltproveedor->text()+"'";
      SQLQuery += ",'"+m_cifproveedor->text()+"'";
      SQLQuery += ",'"+m_codicliproveedor->text()+"'";
      SQLQuery += ",'"+m_cbancproveedor->text()+"'";
      SQLQuery += ",'"+m_dirproveedor->text()+"'";
      SQLQuery += ",'"+m_poblproveedor->text()+"'";
      SQLQuery += ",'"+m_cpproveedor->text()+"'";
      SQLQuery += ",'"+m_telproveedor->text()+"'";
      SQLQuery += ",'"+m_faxproveedor->text()+"'";
      SQLQuery += ",'"+m_urlproveedor->text()+"'";
      SQLQuery += ",'"+m_emailproveedor->text()+"'";
      SQLQuery += ")";
      companyact->begin();
      companyact->ejecuta(SQLQuery);
      companyact->commit();
   }// end if
   s_releaseModificado();
}// end accept

/************************************************************************
* Esta función se ejecuta cuando se ha pulsado sobre el botón de borrar *
*************************************************************************/
void provedit::boton_borrar() {
   if (idprovider != "0") {
      if ( QMessageBox::Yes == QMessageBox::question(this,"Borrar Proveedor","Esta a punto de borrar un proveedor, Estos datos pueden dar problemas.",QMessageBox::Yes, QMessageBox::No)) {
         QString SQLQuery="DELETE FROM proveedor WHERE idproveedor="+idprovider;
         companyact->begin();
         companyact->ejecuta(SQLQuery);
         companyact->commit();
         close();
      }// end if
   }// end if
}// end boton_borrar

void provedit::boton_newdivision() {
   division *driv = new division(companyact, 0,"hola");
   driv->exec();
   s_setModificado();
}// end boton_newdivision


void provedit::close() {
   if (m_modificado  ) {
    	    if ( QMessageBox::warning( this, "Guardar Proveedor",
		"Desea guardar los cambios.",
		QMessageBox::Ok ,
		QMessageBox::Cancel ) == QMessageBox::Ok)
		s_saveProvider();	   
   }// end if
   QWidget::close();
   delete this;
}// end close

