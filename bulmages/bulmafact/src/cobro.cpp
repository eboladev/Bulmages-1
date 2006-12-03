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

#include "cobro.h"
#include "company.h"
#include "configuracion.h"


/** Inicializa el DBRecord para que trabaje con la tabla cobro.
*/
Cobro::Cobro(company *comp) : DBRecord(comp) {
    _depura("Cobro::Cobro", 0);
    companyact = comp;
    setDBTableName("cobro");
    setDBCampoId("idcobro");
    addDBCampo("idcobro", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("Cobro", "ID cobro"));
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("Cobro", "ID cliente"));
    addDBCampo("previsioncobro", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("Cobro", "Prevision de cobro"));
    addDBCampo("fechacobro", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Cobro", "Fecha de cobro"));
    addDBCampo("refcobro", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cobro", "Referencia del cobro"));
    addDBCampo("cantcobro", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate("Cobro", "Cantidad"));
    addDBCampo("comentcobro", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cobro", "Comentarios"));
    _depura("END Cobro::Cobro", 0);
}

/** No requiere de acciones especiales en el constructor.
*/
Cobro::~Cobro() {
    _depura("Cobro::~Cobro", 0);
    _depura("END Cobro::~Cobro", 0);
}


/** Este mapa se encarga de pintar el cobro a traves de la pantalla de presentacion.
    Para ello utiliza los metodos virtuales pintarXXX() que deben estar implementados
    en la clase de visualizacion.
*/
void Cobro::pintar() {
    _depura("Cobro::pintar", 0);
    pintaidcobro(DBvalue("idcobro"));
    pintaidcliente(DBvalue("idcliente"));
    pintafechacobro(DBvalue("fechacobro"));
    pintacantcobro(DBvalue("cantcobro"));
    pintarefcobro(DBvalue("refcobro"));
    pintaprevisioncobro(DBvalue("previsioncobro"));
    pintacomentcobro(DBvalue("comentcobro"));
    _depura("END Cobro::pintar", 0);
}


/** Este metodo se encarga de guardar el cobro en la Base de datos.
    Crea una transaccion e invoca al metodo DBsave de DBRecord.
    Una vez guardado se hace una carga para incorporar los elementos que haya podido
    introducir la base de datos (p. ej: La referencia del cobro).
    
    Si se produce un error genera un mensaje de error y devuelve -1.
    Si todo va bien devuelve 0.
*/
int Cobro::guardar() {
    _depura("Cobro::guardar", 0);
    try {
        QString id;
        companyact->begin();
        DBsave(id);
        setidcobro(id);
        companyact->commit();

	/// Hacemos una carga para que se actualizen datos como la referencia.
	cargar(id);

        _depura("END Cobro::guardar", 0);
        return 0;
    } catch (...) {
        mensajeInfo("Error inesperado al guardar");
        companyact->rollback();
	return -1;
    } // end try
}

