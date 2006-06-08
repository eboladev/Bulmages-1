//
// C++ Implementation: RutaComercial
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "rutacomercial.h"
#include "company.h"
#include "configuracion.h"
#include "plugins.h"

#include <QFile>
#include <QTextStream>



RutaComercial::RutaComercial(company *comp) : DBRecord(comp) {
    m_companyact=comp;
    setDBTableName("rutacomercial");
    setDBCampoId("idrutacomercial");
    addDBCampo("idrutacomercial", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador");
    addDBCampo("fecharutacomercial", DBCampo::DBdate, DBCampo::DBNotNull, "Fecha");
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, "Familia");
    addDBCampo("comentariosrutacomercial", DBCampo::DBvarchar, DBCampo::DBNothing, "Comentarios");
    addDBCampo("horarutacomercial", DBCampo::DBvarchar, DBCampo::DBNothing, "Hora");
    addDBCampo("refrutacomercial",  DBCampo::DBvarchar, DBCampo::DBNothing, "Referencia");
}

RutaComercial::~RutaComercial() {}




void RutaComercial::pintar() {
    _depura("RutaComercial::pintar",0);
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto
    int res = g_plugins->lanza("RutaComercial_pintar", this);
    if (res != 0)
        return;
	_depura("funcion no sobrecargada",2);
    _depura("END RutaComercial::pintar",0);
}

/*
int RutaComercial::guardar() {
	_depura("RutaComercial::guardar",0);
	/// Todo el guardado es una transaccion.
	QString id;
	m_companyact->begin();
	int error = DBsave(id);
	if (error)  {
		m_companyact->rollback();
		return -1;
	}
	setDBvalue("idrutacomercial",id);
	m_companyact->commit();
	_depura("END RutaComercial::guardar",0);
	return 0;
}
*/

