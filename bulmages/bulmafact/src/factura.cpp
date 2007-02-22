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
    m_companyact = comp;
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
        m_companyact->begin();
        int error = m_listalineas->borrar();
        if (error) {
            m_companyact->rollback();
            return -1;
        } // end if
        error = m_listadescuentos->borrar();
        if (error) {
            m_companyact->rollback();
            return -1;
        } // end if
        error = DBRecord::borrar();
        if (error) {
            m_companyact->rollback();
            return -1;
        } // end if
        m_companyact->commit();
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
    _depura("pintaFactura", 0);
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
    _depura("END pintaFactura", 0);
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
    cursor2 * cur= m_companyact->cargacursor(query);
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
            cursor2 *cur = m_companyact->cargacursor(SQLQueryn);
            if (!cur->eof())
                setDBvalue("numfactura", cur->valor("num"));
            pintaNumFactura(DBvalue("numfactura"));
            delete cur;
        } // end if
        QString id;
        m_companyact->begin();
        DBsave(id);
        setidfactura(id);
        m_listalineas->guardar();
        m_listadescuentos->guardar();
        m_companyact->commit();

	/// Hacemos una carga para recuperar datos como la referencia
	cargar(id);

        _depura("END Factura::guardar", 0);
        return 0;
    } catch (...) {
        _depura("Factura::guardar() se produjo un error guardando la factura", 0);
        m_companyact->rollback();
        throw  -1;
    } // end try
}

