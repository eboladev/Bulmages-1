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
   clavewebproveedor character varying(100),
   iddivision integer NOT NULL REFERENCES division(iddivision)
);
*/
 
#include "provedit.h"
#include "company.h"
#include <qlineedit.h>

provedit::provedit(company *comp, QWidget *parent, const char *name)
 : provedit_base(parent, name) {
   companyact = comp;
   idprovider = "";
}// end provedit

provedit::~provedit() {
}// end ~provedit


/*********************************************************************
* Esta función carga un proveedor de la base de datos y lo presenta. *
**********************************************************************/
void provedit::chargeprovider(QString idprov) {
   QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor="+idprov;
   companyact->begin();
   cursor2 *cur= companyact->cargacursor(SQLQuery, "unquery");
   companyact->commit();
   if (!cur->eof()) {
      m_idproveedor->setText(cur->valor("idproveedor"));
      m_nomproveedor->setText(cur->valor("nomproveedor"));
      m_nomaltproveedor->setText(cur->valor("nomaltproveedor"));
      m_cifproveedor->setText(cur->valor("cifproveedor"));
      m_codicliproveedor->setText(cur->valor("codicliproveedor"));
      m_cbancproveedor->setText(cur->valor("cbancproveedor"));
//      m_comentproveedor->setText(cur->valor("comentproveedor"));
      m_dirproveedor->setText(cur->valor("dirproveedor"));
      m_poblproveedor->setText(cur->valor("poblproveedor"));
      m_cpproveedor->setText(cur->valor("cpproveedor"));
      m_telproveedor->setText(cur->valor("telproveedor"));
      m_faxproveedor->setText(cur->valor("faxproveedor"));
      m_emailproveedor->setText(cur->valor("emailproveedor"));
      m_urlproveedor->setText(cur->valor("urlproveedor"));
//      m_clavewebproveedor->setText(cur->valor("clavewebproveedor"));
      m_iddivision->setText(cur->valor("iddivision"));    
   }// end if
   delete cur;
}// end chargeprovider


