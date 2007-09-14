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

#include "cliente.h"
#include "company.h"
#include "configuracion.h"
#include "plugins.h"


/** Inicializa todos los componentes.
    Prepara el DBRecord para trabajar con la tabla cliente. */
Cliente::Cliente(Company *comp, QWidget *parent) : FichaBf(comp, parent) {
    _depura("Cliente::Cliente", 0);
    setTitleName(tr("Cliente"));
    setDBTableName("cliente");
    setDBCampoId("idcliente");
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("Cliente", "ID cliente"));
    addDBCampo("nomcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Nombre del cliente"));
    addDBCampo("nomaltcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Nombre alternativo del cliente"));
    addDBCampo("cifcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "C.I.F. del cliente"));
    addDBCampo("bancocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Numero cuenta corriente"));
    addDBCampo("dircliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Direccion"));
    addDBCampo("poblcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Poblacion"));
    addDBCampo("idprovincia", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Cliente", "Provincia"));
    addDBCampo("cpcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Codigo postal"));
    addDBCampo("telcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Numero de telefono"));
    addDBCampo("teltrabcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Numero de telefono en el trabajo"));
    addDBCampo("movilcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Numero de telefono movil"));
    addDBCampo("faxcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Numero de fax"));
    addDBCampo("mailcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Direccion electronica"));
    addDBCampo("urlcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Identificador de presupuesto"));
    addDBCampo("faltacliente", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Cliente", "Fecha de alta del cliente"));
    addDBCampo("fbajacliente", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Cliente", "Fecha de baja del cliente"));
    addDBCampo("comentcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Comentarios"));
    addDBCampo("inactivocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Cliente inactivo"));
    addDBCampo("regimenfiscalcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Regimen fiscal"));
    addDBCampo("codcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Codigo"));
    addDBCampo("corpcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Empresa"));
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Cliente", "Forma de pago"));
    addDBCampo("recargoeqcliente", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("Cliente", "Recargo de Equivalencia"));
    _depura("END Cliente::Cliente", 0);
}


/** No requiere de acciones adicionales. */
Cliente::~Cliente() {
    _depura("Cliente::~Cliente", 0);
    _depura("END Cliente::~Cliente", 0);
}


void Cliente::pintaidcliente(QString) {
    _depura("Cliente::pintaidcliente", 0);
    _depura("END Cliente::pintaidcliente", 0);
}


void Cliente::pintanomcliente(QString) {
    _depura("Cliente::pintanomcliente", 0);
    _depura("END Cliente::pintanomcliente", 0);
}


void Cliente::pintanomaltcliente(QString) {
    _depura("Cliente::pintanomaltcliente", 0);
    _depura("END Cliente::pintanomaltcliente", 0);
}


void Cliente::pintacifcliente(QString) {
    _depura("Cliente::pintacifcliente", 0);
    _depura("END Cliente::pintacifcliente", 0);
}


void Cliente::pintabancocliente(QString) {
    _depura("Cliente::pintabancocliente", 0);
    _depura("END Cliente::pintabancocliente", 0);
}


void Cliente::pintadircliente(QString) {
    _depura("Cliente::pintadircliente", 0);
    _depura("END Cliente::pintadircliente", 0);
}


void Cliente::pintapoblcliente(QString) {
    _depura("Cliente::pintapoblcliente", 0);
    _depura("END Cliente::pintapoblcliente", 0);
}


void Cliente::pintacpcliente(QString) {
    _depura("Cliente::pintacpcliente", 0);
    _depura("END Cliente::pintacpcliente", 0);
}


void Cliente::pintatelcliente(QString) {
    _depura("Cliente::pintatelcliente", 0);
    _depura("END Cliente::pintatelcliente", 0);
}


void Cliente::pintateltrabcliente(QString) {
    _depura("Cliente::pintateltrabcliente", 0);
    _depura("END Cliente::pintateltrabcliente", 0);
}


void Cliente::pintamovilcliente(QString) {
    _depura("Cliente::pintamovilcliente", 0);
    _depura("END Cliente::pintamovilcliente", 0);
}


void Cliente::pintafaxcliente(QString) {
    _depura("Cliente::pintafaxcliente", 0);
    _depura("END Cliente::pintafaxcliente", 0);
}


void Cliente::pintamailcliente(QString) {
    _depura("Cliente::pintamailcliente", 0);
    _depura("END Cliente::pintamailcliente", 0);
}


void Cliente::pintaurlcliente(QString) {
    _depura("Cliente::pintaurlcliente", 0);
    _depura("END Cliente::pintaurlcliente", 0);
}


void Cliente::pintafaltacliente(QString) {
    _depura("Cliente::pintafaltacliente", 0);
    _depura("END Cliente::pintafaltacliente", 0);
}


void Cliente::pintafbajacliente(QString) {
    _depura("Cliente::pintafbajacliente", 0);
    _depura("END Cliente::pintafbajacliente", 0);
}


void Cliente::pintacomentcliente(QString) {
    _depura("Cliente::pintacomentcliente", 0);
    _depura("END Cliente::pintacomentcliente", 0);
}


void Cliente::pintainactivocliente(QString) {
    _depura("Cliente::pintainactivocliente", 0);
    _depura("END Cliente::pintainactivocliente", 0);
}


void Cliente::pintaIdProvincia(QString) {
    _depura("Cliente::pintaIdProvincia", 0);
    _depura("END Cliente::pintaIdProvincia", 0);
}


void Cliente::pintacodcliente(QString) {
    _depura("Cliente::pintacodcliente", 0);
    _depura("END Cliente::pintacodcliente", 0);
}


void Cliente::pintacorpcliente(QString) {
    _depura("Cliente::pintacorpcliente", 0);
    _depura("END Cliente::pintacorpcliente", 0);
}


void Cliente::pintaidforma_pago(QString) {
    _depura("Cliente::pintaidforma_pago", 0);
    _depura("END Cliente::pintaidforma_pago", 0);
}


void Cliente::pintarecargoeqcliente(QString) {
    _depura("Cliente::pintarecargoeqcliente", 0);
    _depura("END Cliente::pintarecargoeqcliente", 0);
}


void Cliente::pintaregimenfiscalcliente(QString) {
    _depura("Cliente::pintaregimenfiscalcliente", 0);
    _depura("END Cliente::pintaregimenfiscalcliente", 0);
}


/** Metodo que permite borrar un cliente.
    Hace un delete sobre la base de datos del cliente que esta viendose.
*/
/// \TODO: Deberia meterse dentro de un bloque de depuracion try{} catch{}.
int Cliente::borrar() {
    _depura("Cliente::borrar", 0);
    if (DBvalue("idcliente") != "") {
        empresaBase()->begin();
        int error = empresaBase()->ejecuta("DELETE FROM cliente WHERE idcliente = " + DBvalue("idcliente"));
        if (error) {
            empresaBase()->rollback();
            return -1;
        } // end if
        empresaBase()->commit();
	empresaBase()->refreshClientes();
    } // end if
    return 0;
    _depura("END Cliente::borrar", 0);
}




/** Se encarga de pintar un cliente.
    PAra ello coge todos los valores del DBRecord y llama a los metodos pintar que deben
    estar reimplementados en la clase de visualizacion. */
void Cliente::pintar() {
    _depura("Cliente::pintaCliente", 0);

    /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
    int res = g_plugins->lanza("Cliente_pintaCliente", this);
    if (res != 0)
        return;

    pintaidcliente(DBvalue("idcliente"));
    pintanomcliente(DBvalue("nomcliente"));
    pintanomaltcliente(DBvalue("nomaltcliente"));
    pintacifcliente(DBvalue("cifcliente"));
    pintabancocliente(DBvalue("bancocliente"));
    pintadircliente(DBvalue("dircliente"));
    pintapoblcliente(DBvalue("poblcliente"));
    pintacpcliente(DBvalue("cpcliente"));
    pintatelcliente(DBvalue("telcliente"));
    pintateltrabcliente(DBvalue("teltrabcliente"));
    pintamovilcliente(DBvalue("movilcliente"));
    pintafaxcliente(DBvalue("faxcliente"));
    pintamailcliente(DBvalue("mailcliente"));
    pintaurlcliente(DBvalue("urlcliente"));
    pintafaltacliente(DBvalue("faltacliente"));
    pintafbajacliente(DBvalue("fbajacliente"));
    pintacomentcliente(DBvalue("comentcliente"));
    pintainactivocliente(DBvalue("inactivocliente"));
    pintaIdProvincia(DBvalue("idprovincia"));
    pintacodcliente(DBvalue("codcliente"));
    pintacorpcliente(DBvalue("corpcliente"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintarecargoeqcliente(DBvalue("recargoeqcliente"));
    pintaregimenfiscalcliente(DBvalue("regimenfiscalcliente"));
    _depura("END Cliente::pintaCliente", 0);
}


/** Metodo para realizar la carga de un cliente.
    Hace el select en la base de datos y delega la responsibilidad de llentar los campos a
    DBRecord.
*/
/// \TODO:    Justo depues de la carga deberia hacerse un pintado.
/// \TODO:    Deberia estar en un bloque de depuracion try{} catch{}
int Cliente::cargar(QString idcliente) {
    _depura("Cliente::cargar", 0);
    QString query = "SELECT * FROM cliente WHERE idcliente = " + idcliente;
    cursor2 *cur = empresaBase()->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;
    _depura("END Cliente::cargar", 0);
    return 0;
}


/** Almacena en la base de datos los valores del registro.
    Delega en DBRecord la composicion del query y la ejecucion del mismo.
    Sin embargo crea la transaccion de borrado y la finaliza.
    Tambien trata las posibles excepciones.

    Si todo va bien devuelve 0.
    Si se produce algun error devuelve -1.
*/
/// \TODO: Deberia generar una excepcion que deberia ser tratada en la parte visual.
int Cliente::guardar() {
    _depura("Cliente::guardar", 0);
    QString id;
    empresaBase()->begin();
    try {
        DBsave(id);
        setDBvalue("idcliente", id);
        empresaBase()->commit();
        _depura("END Cliente::guardar", 0);
        return 0;
    } catch (...) {
        _depura("error al guardar el cliente", 1);
        empresaBase()->rollback();
        return -1;
    } // end try
}

