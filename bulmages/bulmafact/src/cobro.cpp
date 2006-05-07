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
    addDBCampo("idcobro", DBCampo::DBint, DBCampo::DBPrimaryKey, tr("ID cobro"));
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, tr("ID cliente"));
    addDBCampo("previsioncobro", DBCampo::DBboolean, DBCampo::DBNothing, tr("Prevision de cobro"));
    addDBCampo("fechacobro", DBCampo::DBdate, DBCampo::DBNothing, tr("Fecha de cobro"));
    addDBCampo("refcobro", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Referencia del cobro"));
    addDBCampo("cantcobro", DBCampo::DBnumeric, DBCampo::DBNotNull, tr("Cantidad"));
    addDBCampo("comentcobro", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Comentarios"));
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
}// end pintaCobro






