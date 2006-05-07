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

#include "cobro.h"
#include "company.h"
#include "configuracion.h"
#include <QFile>
#include <QTextStream>



Cobro::Cobro(company *comp) : DBRecord(comp) {
    companyact=comp;

    setDBTableName("cobro");
    setDBCampoId("idcobro");
    addDBCampo("idcobro", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("ID cobro", "cobro"));
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("ID cliente", "cobro"));
    addDBCampo("previsioncobro", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("Prevision de cobro", "cobro"));
    addDBCampo("fechacobro", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Fecha de cobro", "cobro"));
    addDBCampo("refcobro", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Referencia del cobro", "cobro"));
    addDBCampo("cantcobro", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate("Cantidad", "cobro"));
    addDBCampo("comentcobro", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Comentarios", "cobro"));
}

Cobro::~Cobro() {}




void Cobro::pintar() {
    pintaidcobro(DBvalue("idcobro"));
    pintaidcliente(DBvalue("idcliente"));
    pintafechacobro(DBvalue("fechacobro"));
    pintacantcobro(DBvalue("cantcobro"));
    pintarefcobro(DBvalue("refcobro"));
    pintaprevisioncobro(DBvalue("previsioncobro"));
    pintacomentcobro(DBvalue("comentcobro"));
}






