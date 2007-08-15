/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#include "contrato.h"
#include "company.h"
#include "funcaux.h"
#include "plugins.h"


/** PReparamos la clase DBRecord para funcionar con la tabla contrato.
*/
Contrato::Contrato(Company *comp, QWidget *parent) : FichaBf(comp, parent) {
    _depura("Contrato::Contrato", 0);
    setDBTableName("contrato");
    setDBCampoId("idcontrato");
    addDBCampo("idcontrato",  DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("Contrato", "Id contrato"));
    addDBCampo("idcliente",   DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("Contrato", "Id cliente"));
    addDBCampo("refcontrato", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Contrato", "Referencia contrato"));
    addDBCampo("descontrato", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Contrato", "Descripcion contrato"));
    addDBCampo("nomcontrato", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Contrato", "Descripcion contrato"));
    addDBCampo("loccontrato", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Contrato", "Descripcion contrato"));
    addDBCampo("periodicidadcontrato", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Contrato", "Descripcion contrato"));
    addDBCampo("fincontrato",  DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Contrato", "Descripcion contrato"));
    addDBCampo("ffincontrato", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Contrato", "Descripcion contrato"));
    _depura("END Contrato::Contrato", 0);
}


/** No requiere de acciones especiales en el destructor.
*/
Contrato::~Contrato() {
    _depura("Contrato::~Contrato", 0);
    _depura("END Contrato::~Contrato", 0);
}


/** Se encarga del borrado de una contrato.
    Para ello primero borra las lineas y descuentos de esta. y luego
    Borra la ficha de la contrato.
    Sit odo va bien devuelve 0, sino devuelve -1.
*/
/// \TODO: Deberia contener un bloque try{} catch{}
int Contrato::borrar() {
    _depura("Contrato::borrar", 0);
    if (DBvalue("idcontrato") != "") {
        empresaBase()->begin();
        int error = m_listalineas->borrar();
        if (error) {
            empresaBase()->rollback();
            return -1;
        } // end if
        error = DBRecord::borrar();
        if (error) {
            empresaBase()->rollback();
            return -1;
        } // end if
        empresaBase()->commit();
    } // end if
    _depura("END Contrato::borrar", 0);
    return 0;
}


/** Vacia la memoria que pudiera tener DBRecord
*/
void Contrato::vaciaContrato() {
    _depura("Contrato::vaciaContrato", 0);
    DBclear();
    _depura("END Contrato::vaciaContrato", 0);
}


/** Pinta una contrato.
    Para ello utiliza los metodos sobrecargables pintaXXX de esta forma se podria generar una segunda interficie reutilizando esta clase.
*/
/// \TODO: Deberia llamarse pintar.
void Contrato::pintar() {
    _depura("pintaContrato", 0);
    pintaidcliente(DBvalue("idcliente"));
    pintafincontrato(DBvalue("fincontrato"));
    pintaffincontrato(DBvalue("ffincontrato"));
    pintaperiodicidadcontrato(DBvalue("periodicidadcontrato"));
    pintarefcontrato(DBvalue("refcontrato"));
    pintadescontrato(DBvalue("descontrato"));
    pintanomcontrato(DBvalue("nomcontrato"));
    pintaloccontrato(DBvalue("loccontrato"));
    /// Pintamoslas lineas
    m_listalineas->pintar();
    _depura("END pintaContrato", 0);
}


/// Esta funcion carga un contrato.
/** Carga una contrato. 
    Crea el query de carga y delega el llenodo de registros a DBRecord.
    Una vez cargado DBRecord hace una llamada a la carga de lineas y descuentos.
    Tras el cargado se hace un pintado para que se vea bien todo.
    Si todo va bien devuelve 0.
*/
/// \TODO: Deberia utilizar un bloque try{} catch{}
int Contrato::cargar(QString idbudget) {
    _depura("Contrato::cargar", 0);
    inicialize();
    QString query = "SELECT * FROM contrato WHERE idcontrato = " + idbudget;
    cursor2 * cur= empresaBase()->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;
    m_listalineas->cargar(idbudget);
    pintar();
    _depura("END Contrato::cargar", 0);
    return 0;
}


/** Se encarga del guardado de una contrato.
    Primero busca el numero de contrato que hace falta.
    Luego guarda el registro.
    Despues guarda las lineas y descuentos.
    Al finalizar el guardado hace un cargar para repintar todos los elementos.
    Si todo va bien devuelve 0
    Si se produce un error genera una excepcion -1
*/
int Contrato::guardar() {
    _depura("Contrato::guardar", 0);
    QString fecha;
    try {
        /// Calculamos el proximo numero de contrato para poder insertarlo en caso de que este sea nulo.
        QString id;
        empresaBase()->begin();
        DBsave(id);
        setDBvalue("idcontrato", id);
	m_listalineas->setColumnValue("idcontrato", id);
	m_listalineas->guardar();
        empresaBase()->commit();

	/// Hacemos una carga para recuperar datos como la referencia
	cargar(id);

        _depura("END Contrato::guardar", 0);
        return 0;
    } catch (...) {
        _depura("Contrato::guardar() se produjo un error guardando la contrato", 0);
        empresaBase()->rollback();
        throw  -1;
    } // end try
}


/** Devuelve el puntero a la clase company con la que esta trabajando la contrato
**/
Company * Contrato::_company() {
	_depura("Contrato::_company", 0);
	_depura("END Contrato::_company", 0);
        return empresaBase();
}






/** Funciones para ser derivadas en las clases de pintado
**/
    void Contrato::pintaidcliente(QString) {}
    void Contrato::pintarefcontrato(QString) {}
    void Contrato::pintafincontrato(QString ) {}
    void Contrato::pintadescontrato(QString) {}
    void Contrato::pintanomcontrato(QString) {}
    void Contrato::pintaffincontrato(QString) {}
    void Contrato::pintaloccontrato(QString) {}
    void Contrato::pintaperiodicidadcontrato(QString) {}
    
    void Contrato::inicialize() {}
    
    QString Contrato::calculateValues() {
        return "";
    }



