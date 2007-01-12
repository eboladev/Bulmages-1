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
    Prepara el DBRecord para trabajar con la tabla cliente.
*/
Cliente::Cliente(company *comp) : DBRecord(comp) {
    _depura("Cliente::Cliente", 0);
    m_companyact = comp;
    setDBTableName("cliente");
    setDBCampoId("idcliente");
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("Cliente", "ID cliente"));
    addDBCampo("nomcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Nombre del cliente"));
    addDBCampo("nomaltcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Nombre alternativo del cliente"));
    addDBCampo("cifcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "C.I.F. del cliente"));
    addDBCampo("bancocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Numero cuenta corriente"));
    addDBCampo("dircliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Direccion"));
    addDBCampo("poblcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Poblacion"));
    addDBCampo("provcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Provincia"));
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
    addDBCampo("codcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Codigo"));
    addDBCampo("corpcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Cliente", "Empresa"));
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Cliente", "Forma_Pago"));
    addDBCampo("recargoeqcliente", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("Cliente", "Recargo de Equivalencia"));
    _depura("END Cliente::Cliente", 0);
}


/** No requiere de acciones adicionales.
*/
Cliente::~Cliente() {
    _depura("Cliente::~Cliente", 0);
    _depura("END Cliente::~Cliente", 0);
}


/** Metodo que permite borrar un cliente. 
    Hace un delete sobre la base de datos del cliente que esta viendose.
*/
/// \TODO: Deberia meterse dentro de un bloque de depuracion try{} catch{}.
void Cliente::borraCliente() {
    _depura("Cliente::borraCliente", 0);
    if (DBvalue("idcliente") != "") {
        m_companyact->begin();
        int error = m_companyact->ejecuta("DELETE FROM cliente WHERE idcliente = " + DBvalue("idcliente"));
        if (error) {
            m_companyact->rollback();
            return;
        } // end if
        m_companyact->commit();
        vaciaCliente();
    } // end if
    _depura("END Cliente::borraCliente", 0);
}


/** Vacia los valores que pueda contener DBRecord para asegurarnos que no hay 
    Basura en las variables.
*/
void Cliente::vaciaCliente() {
    _depura("Cliente::vaciaCliente", 0);
    DBclear();
    _depura("END Cliente::vaciaCliente", 0);
}


/** Se encarga de pintar un cliente.
    PAra ello coge todos los valores del DBRecord y llama a los metodos pintar que deben
    estar reimplementados en la clase de visualizacion.
*/
void Cliente::pintaCliente() {
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
    pintaprovcliente(DBvalue("provcliente"));
    pintacodcliente(DBvalue("codcliente"));
    pintacorpcliente(DBvalue("corpcliente"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintarecargoeqcliente(DBvalue("recargoeqcliente"));

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
    cursor2 *cur = m_companyact->cargacursor(query);
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
    m_companyact->begin();
    try {
        DBsave(id);
	setDBvalue("idcliente", id);
        m_companyact->commit();
        _depura("END Cliente::guardar", 0);
        return 0;
    } catch (...) {
        _depura("error al guardar el cliente", 1);
        m_companyact->rollback();
        return -1;
    } // end try
}

