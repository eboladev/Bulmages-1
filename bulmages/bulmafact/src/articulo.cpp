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
    addDBCampo("idarticulo", DBCampo::DBint, DBCampo::DBPrimaryKey, tr("Identificador"));
    addDBCampo("codarticulo", DBCampo::DBvarchar, DBCampo::DBNotNull, tr("Codigo"));
    addDBCampo("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNotNull, tr("Nombre"));
    addDBCampo("abrevarticulo", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Abreviacion"));
    addDBCampo("obserarticulo", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Observaciones"));
    addDBCampo("presentablearticulo", DBCampo::DBboolean, DBCampo::DBNothing, tr("Incluir en presentaciones"));
    addDBCampo("controlstockarticulo", DBCampo::DBboolean, DBCampo::DBNothing, tr("Incluir en control de stock"));
    addDBCampo("idtipo_articulo", DBCampo::DBint, DBCampo::DBNothing, tr("Tipo de articulo"));
    addDBCampo("idtipo_iva", DBCampo::DBint, DBCampo::DBNothing, tr("Tipo de I.V.A."));
    addDBCampo("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, tr("Codigo completo"));
    addDBCampo("idfamilia", DBCampo::DBint, DBCampo::DBNotNull, tr("Familia"));
    addDBCampo("stockarticulo", DBCampo::DBint, DBCampo::DBNothing, tr("Stock"));
    addDBCampo("inactivoarticulo", DBCampo::DBboolean, DBCampo::DBNothing, tr("Inactivo"));
    addDBCampo("pvparticulo", DBCampo::DBnumeric, DBCampo::DBNotNull, tr("P.V.P. base"));
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




