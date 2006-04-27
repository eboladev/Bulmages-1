//
// C++ Implementation: Articulo
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "articulo.h"
#include "company.h"
#include "configuracion.h"
#include "plugins.h"

#include <QFile>
#include <QTextStream>



Articulo::Articulo(company *comp) : DBRecord(comp) {
    m_companyact=comp;
    setDBTableName("articulo");
    setDBCampoId("idarticulo");
    addDBCampo("idarticulo", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador");
    addDBCampo("codarticulo", DBCampo::DBvarchar, DBCampo::DBNotNull, "Codigo");
    addDBCampo("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNotNull, "Nombre");
    addDBCampo("abrevarticulo", DBCampo::DBvarchar, DBCampo::DBNothing, "Abreviacion");
    addDBCampo("obserarticulo", DBCampo::DBvarchar, DBCampo::DBNothing, "Observaciones");
    addDBCampo("presentablearticulo", DBCampo::DBboolean, DBCampo::DBNothing, "Incluir en presentaciones");
    addDBCampo("controlstockarticulo", DBCampo::DBboolean, DBCampo::DBNothing, "Incluir en control de stock");
    addDBCampo("idtipo_articulo", DBCampo::DBint, DBCampo::DBNothing, "Tipo de articulo");
    addDBCampo("idtipo_iva", DBCampo::DBint, DBCampo::DBNothing, "Tipo de I.V.A.");
    addDBCampo("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, "Codigo completo");
    addDBCampo("idfamilia", DBCampo::DBint, DBCampo::DBNotNull, "Familia");
    addDBCampo("stockarticulo", DBCampo::DBint, DBCampo::DBNothing, "Stock");
    addDBCampo("inactivoarticulo", DBCampo::DBboolean, DBCampo::DBNothing, "Inactivo");
    addDBCampo("pvparticulo", DBCampo::DBnumeric, DBCampo::DBNotNull, "P.V.P. base");
}

Articulo::~Articulo() {}




void Articulo::pintar() {
    _depura("Articulo::pintar",0);
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto
    int res = g_plugins->lanza("Articulo_pintar", this);
    if (res != 0)
        return;
    pintaidarticulo(DBvalue("idarticulo"));
    pintacodarticulo (DBvalue("codarticulo"));
    pintanomarticulo (DBvalue("nomarticulo"));
    pintaabrevarticulo (DBvalue("abrevarticulo"));
    pintaobservarticulo (DBvalue("observarticulo"));
    pintapresentablearticulo (DBvalue("presentablearticulo"));
    pintacontrolstockarticulo (DBvalue("controlstockarticulo"));
    pintaidtipo_articulo (DBvalue("idtipo_articulo"));
    pintaidtipo_iva (DBvalue("idtipo_iva"));
    pintacodigocompletoarticulo (DBvalue("codigocompletoarticulo"));
    pintaidfamilia (DBvalue("idfamilia"));
    pintastockarticulo (DBvalue("stockarticulo"));
    pintainactivoarticulo (DBvalue("inactivoarticulo"));
    pintapvparticulo (DBvalue("pvparticulo"));
    _depura("END Articulo::pintar",0);
}// end pintaArticulo




