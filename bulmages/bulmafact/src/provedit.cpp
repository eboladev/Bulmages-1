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
      m_cifproveedor->setText(cur->valor("cifproveedor"));
      m_idproveedor->setText(cur->valor("idproveedor"));
      m_nomproveedor->setText(cur->valor("nomproveedor"));
      m_nomaltproveedor->setText(cur->valor("nomaltproveedor"));
   }// end if
   delete cur;
}// end chargeprovider


