/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#include "descpresupuesto.h"


void DescuentoPresupuesto::definetabla() {
    setDBTableName("dpresupuesto");
    setDBCampoId("iddpresupuesto");
    addDBCampo("iddpresupuesto", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Linea Presupuesto");
    addDBCampo("conceptdpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, "Descripcion");
    addDBCampo("idpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("proporciondpresupuesto", DBCampo::DBnumeric, DBCampo::DBNotNull, "Proporcion");
}


DescuentoPresupuesto::DescuentoPresupuesto(company *comp) : DBRecord(comp) {
    companyact = comp;
    definetabla();
}


DescuentoPresupuesto::DescuentoPresupuesto(company *comp, QString id) : DBRecord(comp) {
    companyact = comp;
    definetabla();
    QString SQLQuery = "SELECT * FROM dpresupuesto WHERE  iddpresupuesto=" + id;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        DBload(cur);
    } else {
        vaciaDescuentoPresupuesto();
    } // end if
}


DescuentoPresupuesto::DescuentoPresupuesto(company *comp, QString a, QString b, QString c, QString d)
        : DBRecord(comp) {
    companyact = comp;
    definetabla();
    setDBvalue("iddpresupuesto", a);
    setDBvalue("conceptdpresupuesto", b);
    setDBvalue("proporciondpresupuesto", c);
    setDBvalue("idpresupuesto", d);
}


DescuentoPresupuesto::~DescuentoPresupuesto() {}


void DescuentoPresupuesto::vaciaDescuentoPresupuesto() {
    DBclear();
}


void DescuentoPresupuesto::guardaDescuentoPresupuesto() {
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return;
    } // end if
    setDBvalue("iddpresupuesto", id);
    companyact->commit();
}

