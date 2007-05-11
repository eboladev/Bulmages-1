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

#include <QFile>
#include <QTextStream>

#include "incidenciacomercial.h"
#include "company.h"
#include "configuracion.h"
#include "plugins.h"


IncidenciaComercial::IncidenciaComercial(company *comp, QWidget *parent) : FichaBf(comp, parent) {
    setDBTableName("incidenciacomercial");
    setDBCampoId("idincidenciacomercial");
    addDBCampo("idincidenciacomercial", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("IncidenciaComercial", "Identificador"));
    addDBCampo("fechaincidenciacomercial", DBCampo::DBdate, DBCampo::DBNotNull, QApplication::translate("IncidenciaComercial", "Fecha"));
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("IncidenciaComercial", "Familia"));
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("IncidenciaComercial", "Trabajador"));
    addDBCampo("comentincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("IncidenciaComercial", "Comentarios"));
    addDBCampo("estadoincidenciacomercial", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("IncidenciaComercial", "Estado"));
    addDBCampo("horaincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("IncidenciaComercial", "Hora"));
    addDBCampo("refincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("IncidenciaComercial", "Referencia"));
}


IncidenciaComercial::~IncidenciaComercial() {}


void IncidenciaComercial::pintar() {
    _depura("IncidenciaComercial::pintar", 0);
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
    int res = g_plugins->lanza("IncidenciaComercial_pintar", this);
    if (res != 0)
        return;
    _depura("END IncidenciaComercial::pintar", 0);
}

