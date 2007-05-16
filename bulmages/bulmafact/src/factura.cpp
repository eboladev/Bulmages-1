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

#include "factura.h"
#include "company.h"
#include "funcaux.h"
#include "plugins.h"


/** PReparamos la clase DBRecord para funcionar con la tabla factura.
*/
Factura::Factura(company *comp, QWidget *parent) : FichaBf(comp, parent) {
    _depura("Factura::Factura", 0);
    setDBTableName("factura");
    setDBCampoId("idfactura");
    addDBCampo("idfactura", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("Factura", "Id factura"));
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("Factura", "Id cliente"));
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("Factura", "Id almacen"));
    addDBCampo("numfactura", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Factura", "Numero factura"));
    addDBCampo("ffactura", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Factura", "Ffactura"));
    addDBCampo("procesadafactura", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("Factura", "Procesada factura"));
    addDBCampo("codigoserie_factura", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Factura", "Codigo serie factura"));
    addDBCampo("comentfactura", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Factura", "Comentario factura"));
    addDBCampo("reffactura", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Factura", "Referencia factura"));
    addDBCampo("descfactura", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Factura", "Descripcion factura"));
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Factura", "Id trabajador"));
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Factura", "Id formad de pago"));
    _depura("END Factura::Factura", 0);
}


/** No requiere de acciones especiales en el destructor.
*/
Factura::~Factura() {
    _depura("Factura::~Factura", 0);
    _depura("END Factura::~Factura", 0);
}


/** Se encarga del borrado de una factura.
    Para ello primero borra las lineas y descuentos de esta. y luego
    Borra la ficha de la factura.
    Sit odo va bien devuelve 0, sino devuelve -1.
*/
/// \TODO: Deberia contener un bloque try{} catch{}
int Factura::borrar() {
    _depura("Factura::borrar", 0);
    if (DBvalue("idfactura") != "") {
        empresaBase()->begin();
        int error = m_listalineas->borrar();
        if (error) {
            empresaBase()->rollback();
            return -1;
        } // end if
        error = m_listadescuentos->borrar();
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
    _depura("END Factura::borrar", 0);
    return 0;
}


/** Vacia la memoria que pudiera tener DBRecord
*/
void Factura::vaciaFactura() {
    _depura("Factura::vaciaFactura", 0);
    DBclear();
    _depura("END Factura::vaciaFactura", 0);
}


/** Pinta una factura.
    Para ello utiliza los metodos sobrecargables pintaXXX de esta forma se podria generar una segunda interficie reutilizando esta clase.
*/
/// \TODO: Deberia llamarse pintar.
void Factura::pintaFactura() {
    _depura("Factura::pintaFactura", 0);
    pintaidcliente(DBvalue("idcliente"));
    pintaidalmacen(DBvalue("idalmacen"));
    pintaNumFactura(DBvalue("numfactura"));
    pintafechafactura(DBvalue("ffactura"));
    pintareffactura(DBvalue("reffactura"));
    pintaComentFactura(DBvalue("comentfactura"));
    pintaprocesadafactura(DBvalue("procesadafactura"));
    pintacodigoserie_factura(DBvalue("codigoserie_factura"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintadescfactura(DBvalue("descfactura"));
    /// Pinta el subformulario de detalle del factura.
    calculaypintatotales();
    _depura("END Factura::pintaFactura", 0);
}


/// Esta funcion carga un factura.
/** Carga una factura.
    Crea el query de carga y delega el llenodo de registros a DBRecord.
    Una vez cargado DBRecord hace una llamada a la carga de lineas y descuentos.
    Tras el cargado se hace un pintado para que se vea bien todo.
    Si todo va bien devuelve 0.
*/
/// \TODO: Deberia utilizar un bloque try{} catch{}
int Factura::cargar(QString idbudget) {
    _depura("Factura::cargar", 0);
    inicialize();
    QString query = "SELECT * FROM factura WHERE idfactura = " + idbudget;
    cursor2 * cur= empresaBase()->cargacursor(query);

    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;
    pintaFactura();
    m_listalineas->cargar(idbudget);
    m_listadescuentos->cargar(idbudget);
    calculaypintatotales();
    _depura("END Factura::cargar", 0);
    return 0;
}


/** Se encarga del guardado de una factura.
    Primero busca el numero de factura que hace falta.
    Luego guarda el registro.
    Despues guarda las lineas y descuentos.
    Al finalizar el guardado hace un cargar para repintar todos los elementos.
    Si todo va bien devuelve 0
    Si se produce un error genera una excepcion -1
*/
int Factura::guardar() {
    _depura("Factura::guardar", 0);
    QString fecha;
    try {
        /// Calculamos el proximo numero de factura para poder insertarlo en caso de que este sea nulo.
        if (DBvalue("numfactura") == "") {
            QString SQLQueryn = "SELECT MAX(numfactura) + 1 AS num FROM factura WHERE codigoserie_factura = '" + DBvalue("codigoserie_factura") + "'";
            cursor2 *cur = empresaBase()->cargacursor(SQLQueryn);
            if (!cur->eof())
                setDBvalue("numfactura", cur->valor("num"));
            pintaNumFactura(DBvalue("numfactura"));
            delete cur;
        } // end if
        QString id;
        empresaBase()->begin();
        DBsave(id);
        setidfactura(id);
        m_listalineas->guardar();
        m_listadescuentos->guardar();
        empresaBase()->commit();

    /// Hacemos una carga para recuperar datos como la referencia
    cargar(id);

        _depura("END Factura::guardar", 0);
        return 0;
    } catch (...) {
        _depura("Factura::guardar() se produjo un error guardando la factura", 0);
        empresaBase()->rollback();
        throw  -1;
    } // end try
}


/** Devuelve el puntero a la clase company con la que esta trabajando la factura
**/
    company * Factura::_company() {
    _depura("Factura::_company", 0);
    _depura("END Factura::_company", 0);
        return empresaBase();
    }

/** Devuelve la serie de factura que tiene establecida la factura
**/
    QString Factura::codigoserie_factura() {
    _depura("Factura::codigoserie_factura", 0);
    _depura("END Factura::codigoserie_factura", 0);
        return DBvalue("codigoserie_factura");
    }

/** Devuelve el identificador de cliente que tiene establecido la factura
**/
    QString Factura::idcliente() {
    _depura("Factura::idcliente", 0);
    _depura("END Factura::idcliente", 0);
        return DBvalue("idcliente");
    }

/** Devuelve el identificador del almacen que tiene establecido la factura
**/
    QString Factura::idalmacen() {
    _depura("Factura::idalmacen", 0);
    _depura("END Factura::idalmacen", 0);
        return DBvalue("idalmacen");
    }

/** Devuelve el numero de factura que tiene establecida la factura
**/
    QString Factura::numfactura() {
    _depura("Factura::numfactura", 0);
    _depura("END Factura::numfactura", 0);
        return DBvalue("numfactura");
    }

/** Devuelve la fecha de factura que tiene establecida la factura
**/
    QString Factura::ffactura() {
    _depura("Factura::ffactura", 0);
    _depura("END Factura::ffactura", 0);
        return DBvalue("ffactura");
    }
/** Devuelve si la factura esta procesada o no
**/
    QString Factura::procesadafactura() {
    _depura("Factura::procesadafactura", 0);
    _depura("END Factura::procesadafactura", 0);
        return DBvalue("procesadafactura");
    }

/** Devuelve los comentarios de la factura
**/
    QString Factura::comentfactura() {
    _depura("Factura::comentfactura", 0);
    _depura("END Factura::comentfactura", 0);
        return DBvalue("comentfactura");
    }

/** Devuelve la referencia de la factura
**/
    QString Factura::reffactura() {
    _depura("Factura::reffactura", 0);
    _depura("END Factura::reffactura", 0);
        return DBvalue("reffactura");
    }

/** Devuelve la descripcion de la factura
**/
    QString Factura::descfactura() {
    _depura("Factura::descfactura", 0);
    _depura("END Factura::descfactura", 0);
        return DBvalue("descfactura");
    }

/** Devuevle el identificador de factura
**/
    QString Factura::idfactura() {
    _depura("Factura::idfactura", 0);
    _depura("END Factura::idfactura", 0);
        return DBvalue("idfactura");
    }

/** Devuelve el identificador de forma de pago establecido en la factura
**/
    QString Factura::idforma_pago() {
    _depura("Factura::idforma_pago", 0);
    _depura("END Factura::idforma_pago", 0);
        return DBvalue("idforma_pago");
    }

/** Establece el identificador de cliente de la factura
**/
    void Factura::setidcliente(QString val) {
    _depura("Factura::setidcliente", 0);
        setDBvalue("idcliente", val);
    _depura("END Factura::setidcliente", 0);
    }

/** Establece el identificador de almacen de la factura
**/
    void Factura::setidalmacen(QString val) {
    _depura("Factura::setidalmacen", 0);
        setDBvalue("idalmacen", val);
    _depura("END Factura::setidalmacen", 0);
    }

/** Establece la referencia de la factura
**/
    void Factura::setreffactura(QString val) {
    _depura("Factura::setreffactura", 0);
        setDBvalue("reffactura", val);
    _depura("END Factura::setreffactura", 0);
    }

/** Establece el numero de factura
**/
    void Factura::setNumFactura(QString val) {
    _depura("Factura::setNumFactura", 0);
        setDBvalue("numfactura", val);
    _depura("END Factura::setNumFactura", 0);
    }

/** Establece la fecha de la factura
**/
    void Factura::setfechafactura(QString val) {
    _depura("Factura::setfechafactura", 0);
        setDBvalue("ffactura", val);
    _depura("END Factura::setfechafactura", 0);
    }

/** Establece la descripcion de la factura
**/
    void Factura::setdescfactura(QString val) {
    _depura("Factura::setdescfactura", 0);
        setDBvalue("descfactura", val);
    _depura("END Factura::setdescfactura", 0);
    }

/** Establece cual es la serie de factura
**/
    void Factura::setcodigoserie_factura(QString val) {
    _depura("Factura::setcodigoserie_factura", 0);
        setDBvalue("codigoserie_factura", val);
    _depura("END Factura::setcodigoserie_factura", 0);
    }

/** Establece cual es el comentario de la factura
**/
    void Factura::setcomentfactura(QString val) {
    _depura("Factura::setcomentfactura", 0);
        setDBvalue("comentfactura", val);
    _depura("END Factura::setcomentfactura", 0);
    }

/** Establece cual es el identificador de factura
**/
    void Factura::setidfactura(QString val) {
    _depura("Factura::setidfactura", 0);
        setDBvalue("idfactura", val);
        m_listalineas->setColumnValue( "idfactura", val);
        m_listadescuentos->setColumnValue( "idfactura", val);
    _depura("END Factura::setidfactura", 0);
    }

/** Establece cual es la forma de pago de la factura
**/
    void Factura::setidforma_pago(QString val) {
    _depura("Factura::setidforma_pago", 0);
        setDBvalue("idforma_pago", val);
    _depura("END Factura::setidforma_pago", 0);
    }

/** Establece cual es el estado de procesada de la factura
**/
    void Factura::setprocesadafactura(QString val) {
    _depura("Factura::setprocesadafactura", 0);
        setDBvalue("procesadafactura", val);
    _depura("END Factura::setprocesadafactura", 0);
    }

/** Funciones para ser derivadas en las clases de pintado
**/
    void Factura::pintaidcliente(QString) {}

    void Factura::pintareffactura(QString) {}

    void Factura::pintaNumFactura(QString ) {}

    void Factura::pintafechafactura(QString ) {}

    void Factura::pintadescfactura(QString) {}

    void Factura::pintacodigoserie_factura(QString) {}

    void Factura::pintaComentFactura(QString) {}

    void Factura::pintaidalmacen(QString ) {}

    void Factura::pintaidforma_pago(QString) {}

    void Factura::pintaprocesadafactura(QString) {}

     void Factura::cargaFacturaDescuentas(QString) {}

     void Factura::calculateImports() {}

    void Factura::inicialize() {}

    QString Factura::calculateValues() {
        return "";
    }



