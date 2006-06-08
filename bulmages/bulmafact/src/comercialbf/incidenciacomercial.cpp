//
// C++ Implementation: IncidenciaComercial
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//



#include "incidenciacomercial.h"
#include "company.h"
#include "configuracion.h"
#include "plugins.h"

#include <QFile>
#include <QTextStream>



IncidenciaComercial::IncidenciaComercial(company *comp) : DBRecord(comp) {
    m_companyact=comp;
    setDBTableName("incidenciacomercial");
    setDBCampoId("idincidenciacomercial");
    addDBCampo("idincidenciacomercial", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador");
    addDBCampo("fechaincidenciacomercial", DBCampo::DBdate, DBCampo::DBNotNull, "Fecha");
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, "Familia");
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNotNull, "Trabajador");
    addDBCampo("comentincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNothing, "Comentarios");
    addDBCampo("estadoincidenciacomercial", DBCampo::DBint, DBCampo::DBNotNull, "Estado");
    addDBCampo("horaincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNothing, "Hora");
    addDBCampo("refincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNothing, "Referencia");
}

IncidenciaComercial::~IncidenciaComercial() {}


void IncidenciaComercial::pintar() {
    _depura("IncidenciaComercial::pintar",0);
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto
    int res = g_plugins->lanza("IncidenciaComercial_pintar", this);
    if (res != 0)
        return;
    _depura("END IncidenciaComercial::pintar",0);
}




