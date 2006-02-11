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
   clavewebproveedor character varying(100)
);
*/
 
#include "provedit.h"
#include "company.h"
#include "division.h"
#include <QLineEdit>
#include <QMessageBox>
#include <Q3Table>

#include <funcaux.h>

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
_depura("provedit::provedit",0);
   companyact = comp;
   idprovider = "0";
   
	m_provproveedor->setcompany(companyact);
	m_provproveedor->setProvincia("");


	/// Desabilitamos los tabs que aun no se usan
    masdf->setTabEnabled(5,FALSE);
    masdf->setTabEnabled(6,FALSE);
    masdf->setTabEnabled(7,FALSE);


	/// Cargamos el listado de pedidos del proveedor y dejamos presentable.
    _depura("inicializamos el listado de pedidos\n",0);
    m_listpedidosprov->setcompany(companyact);
    m_listpedidosprov->hideConfiguracion();
    m_listpedidosprov->hideBusqueda();

    m_albaranesprov->setcompany(companyact);
    m_albaranesprov->hideConfiguracion();
    m_albaranesprov->hideBusqueda();

    m_listfacturasprov->setcompany(companyact);
    m_listfacturasprov->hideConfiguracion();
    m_listfacturasprov->hideBusqueda();

    m_listpagosprov->setcompany(companyact);
    m_listpagosprov->hideConfiguracion();
    m_listpagosprov->hideBusqueda();

   // Arreglamos la tabla de divisionoes.
   m_divisiones->setNumRows( 0 );
   m_divisiones->setNumCols( 0 );
   m_divisiones->setSelectionMode( Q3Table::SingleRow );
   m_divisiones->setSorting( TRUE );
   m_divisiones->setSelectionMode( Q3Table::SingleRow );
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
   if (companyact->meteWindow(caption(),this)) return;
   s_releaseModificado();
_depura("END provedit::provedit",0);
}// end provedit

provedit::~provedit() {
	companyact->sacaWindow(this);
}// end ~provedit


/************************************************************************
* Esta funci� carga un proveedor de la base de datos y lo presenta.    *
* Si el parametro pasado no es un identificador v�ido entonces se pone *
* la ventana de edici� en modo de inserci�                            *
*************************************************************************/
int provedit::chargeprovider(QString idprov) {
	_depura("provedit::chargeprovider",0);
	int error = 0;
   idprovider = idprov;
   fprintf(stderr,"chargeprovider activado \n");
   if (idprovider != "0") {
      QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor="+idprov;
      cursor2 *cur= companyact->cargacursor(SQLQuery);
      if (!cur->eof()) {
         idprovider = idprov;
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
	m_provproveedor->setProvincia(cur->valor("provproveedor"));
	m_comentproveedor->setText(cur->valor("comentproveedor"));
         
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

	/// Inicializamos el listado
	_depura("Vamos a cargar el proveedor",0);
            m_listpedidosprov->setidproveedor(cur->valor("idproveedor"));
            m_listpedidosprov->presenta();
            m_albaranesprov->setidproveedor(cur->valor("idproveedor"));
            m_albaranesprov->presenta();
            m_listfacturasprov->setidproveedor(cur->valor("idproveedor"));
            m_listfacturasprov->presenta();
            m_listpagosprov->setidproveedor(cur->valor("idproveedor"));
            m_listpagosprov->presenta();

            /// Cambiamos el titulo de la ventana para que salga reflejado donde toca.
            setCaption("Proveedor "+cur->valor("nomproveedor"));
            error = companyact->meteWindow(caption(),this);

      } else {
         idprovider="0";
      }// end if
      delete cur;
   }// end if

	if (error) return 1;

   s_releaseModificado();
	_depura("END provedit::chargeprovider",0);
	return 0;
}// end chargeprovider


/************************************************************************
* Esta funci� se ejecuta cuando se ha pulsado sobre el bot� de nuevo  *
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
      m_comentproveedor->setText("");
      s_releaseModificado();
}// end boton_nuevo

/*************************************************************************
* Esta funcion es la respuesta a la pulsacion del boton de guardar       *
* Comprueba si es una insercion o una modificacion y hace los pasos      *
* pertinentes                                                            *
**************************************************************************/
void provedit::s_saveProvider() {
   if (idprovider != "0") {
      QString SQLQuery = "UPDATE proveedor SET urlproveedor='"+companyact->sanearCadena(m_urlproveedor->text())+"'";
      SQLQuery += " , nomproveedor='"+companyact->sanearCadena(m_nomproveedor->text())+"'";
      SQLQuery += " , nomaltproveedor='"+companyact->sanearCadena(m_nomaltproveedor->text())+"'";
      SQLQuery += " , cifproveedor='"+companyact->sanearCadena(m_cifproveedor->text())+"'";
      SQLQuery += " , codicliproveedor='"+companyact->sanearCadena(m_codicliproveedor->text())+"'";
      SQLQuery += " , cbancproveedor='"+companyact->sanearCadena(m_cbancproveedor->text())+"'";
      SQLQuery += " , dirproveedor='"+companyact->sanearCadena(m_dirproveedor->text())+"'";
      SQLQuery += " , poblproveedor='"+companyact->sanearCadena(m_poblproveedor->text())+"'";
      SQLQuery += " , cpproveedor='"+companyact->sanearCadena(m_cpproveedor->text())+"'";
      SQLQuery += " , telproveedor='"+companyact->sanearCadena(m_telproveedor->text())+"'";
      SQLQuery += " , faxproveedor='"+companyact->sanearCadena(m_faxproveedor->text())+"'";
      SQLQuery += " , emailproveedor='"+companyact->sanearCadena(m_emailproveedor->text())+"'";
      SQLQuery += " , provproveedor='"+companyact->sanearCadena(m_provproveedor->currentText())+"'";
      SQLQuery += " , comentproveedor='"+companyact->sanearCadena(m_comentproveedor->text())+"'";
      SQLQuery += " WHERE idproveedor ="+idprovider;
      companyact->begin();
      int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
      companyact->commit();
   } else {
      QString SQLQuery = " INSERT INTO proveedor (nomproveedor, nomaltproveedor, cifproveedor, codicliproveedor, cbancproveedor, dirproveedor, poblproveedor, cpproveedor, telproveedor, faxproveedor, urlproveedor, emailproveedor, provproveedor, comentproveedor)";
      SQLQuery += " VALUES (";
      SQLQuery += "'"+companyact->sanearCadena(m_nomproveedor->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_nomaltproveedor->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_cifproveedor->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_codicliproveedor->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_cbancproveedor->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_dirproveedor->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_poblproveedor->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_cpproveedor->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_telproveedor->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_faxproveedor->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_urlproveedor->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_emailproveedor->text())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_provproveedor->currentText())+"'";
      SQLQuery += ",'"+companyact->sanearCadena(m_comentproveedor->text())+"'";
      SQLQuery += ")";
      companyact->begin();
      int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
      cursor2 * cur = companyact->cargacursor("SELECT max(idproveedor) AS id FROM proveedor");
      idprovider = cur->valor("id");
      delete cur;
      companyact->commit();
   }// end if
   s_releaseModificado();
}// end accept

/************************************************************************
* Esta funci� se ejecuta cuando se ha pulsado sobre el bot� de borrar *
*************************************************************************/
void provedit::boton_borrar() {
   if (idprovider != "0") {
      if ( QMessageBox::Yes == QMessageBox::question(this,"Borrar Proveedor","Esta a punto de borrar un proveedor, Estos datos pueden dar problemas.",QMessageBox::Yes, QMessageBox::No)) {
         QString SQLQuery="DELETE FROM proveedor WHERE idproveedor="+idprovider;
         companyact->begin();
         int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
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

