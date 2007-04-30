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

#include "articulo.h"
#include "company.h"
#include "configuracion.h"
#include "plugins.h"

/** Se encarga de inicializar la clase.
    Prepara el DBRecord para trabajar con la tabla articulo.
    Agrega los campos necesarios para toda la ficha.
*/
Articulo::Articulo(company *comp, QWidget *parent, Qt::WFlags f ) : FichaBf(comp, parent, f) {
  _depura("Articulo::Articulo", 0);
  setDBTableName("articulo");
  setDBCampoId("idarticulo");
  addDBCampo("idarticulo", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("Articulo", "Identificador"));
  addDBCampo("codarticulo", DBCampo::DBvarchar, DBCampo::DBNotNull, QApplication::translate("Articulo", "Codigo"));
  addDBCampo("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNotNull, QApplication::translate("Articulo", "Nombre del articulo"));
  addDBCampo("abrevarticulo", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Articulo", "Abreviacion"));
  addDBCampo("obserarticulo", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Articulo", "Observaciones"));
  addDBCampo("presentablearticulo", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("Articulo", "Incluir en presentaciones"));
  addDBCampo("controlstockarticulo", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("Articulo", "Incluir en control de stock"));
  addDBCampo("idtipo_articulo", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Articulo", "Tipo de articulo"));
  addDBCampo("idtipo_iva", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Articulo", "Tipo de I.V.A."));
  addDBCampo("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, QApplication::translate("Articulo", "Codigo completo"));
  addDBCampo("idfamilia", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("Articulo", "Familia"));
  addDBCampo("stockarticulo", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Articulo", "Stock"));
  addDBCampo("inactivoarticulo", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("Articulo", "Inactivo"));
  addDBCampo("pvparticulo", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate("Articulo", "P.V.P. base"));
  addDBCampo("pesoundarticulo", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate("Articulo", "Peso Unidad"));
  addDBCampo("volumenundarticulo", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate("Articulo", "Volumen Unidad"));
  _depura("END Articulo::Articulo", 0);
}

/** No precisa de ninguna accion especial */
Articulo::~Articulo() {
    _depura("Articulo::~Articulo", 0);
    _depura("END Articulo::~Articulo", 0);
}


