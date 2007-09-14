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

#include "albaranproveedor.h"


/** Constructor de la clase. Prepara el DBRecord para trabajar con la tabla albaranp
*/
AlbaranProveedor::AlbaranProveedor(Company *comp, QWidget *parent)
        : FichaBf(comp, parent) {
    _depura("AlbaranProveedor::AlbaranProveedor", 0);
    setTitleName(tr("Albaran Proveedor"));
    setDBTableName("albaranp");
    setDBCampoId("idalbaranp");
    addDBCampo("idalbaranp", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("AlbaranProveedor", "Id albaran proveedor"));
    addDBCampo("numalbaranp", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("AlbaranProveedor", "Numero albaran proveedor"));
    addDBCampo("fechaalbaranp", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("AlbaranProveedor", "Fecha albaran proveedor"));
    addDBCampo("comentalbaranp", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranProveedor", "Comentario albaran proveedor"));
    addDBCampo("idproveedor", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("AlbaranProveedor", "Id proveedor"));
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("AlbaranProveedor", "Id forma de pago"));
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("AlbaranProveedor", "Id almacen"));
    addDBCampo("refalbaranp", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranProveedor", "Referencia albaran proveedor"));
    addDBCampo("descalbaranp", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranProveedor", "Descripcion albaran proveedor"));
    _depura("END AlbaranProveedor::AlbaranProveedor", 0);
}


/** Destructor de la clase, no necesita funcionalidades adicionales.
*/
AlbaranProveedor::~AlbaranProveedor() {
    _depura("AlbaranProveedor::~AlbaranProveedor", 0);
    _depura("END AlbaranProveedor::~AlbaranProveedor", 0);
}


void AlbaranProveedor::pintaidalbaranp(QString) {
    _depura("AlbaranProveedor::pintaidalbaranp", 0);
}


void AlbaranProveedor::pintanumalbaranp(QString) {
    _depura("AlbaranProveedor::pintanumalbaranp", 0);
}


void AlbaranProveedor::pintafechaalbaranp(QString) {
    _depura("AlbaranProveedor::pintafechaalbaranp", 0);
}


void AlbaranProveedor::pintacomentalbaranp(QString) {
    _depura("AlbaranProveedor::pintacomentalbaranp", 0);
}


void AlbaranProveedor::pintaidproveedor(QString) {
    _depura("AlbaranProveedor::pintaidproveedor", 0);
}


void AlbaranProveedor::pintaidforma_pago(QString) {
    _depura("AlbaranProveedor::pintaidforma_pago", 0);
}


void AlbaranProveedor::pintaidalmacen(QString) {
    _depura("AlbaranProveedor::pintaidalmacen", 0);
}


void AlbaranProveedor::pintadescalbaranp(QString) {
    _depura("AlbaranProveedor::pintadescalbaranp", 0);
}


void AlbaranProveedor::pintarefalbaranp(QString) {
    _depura("AlbaranProveedor::pintarefalbaranp", 0);
}


void AlbaranProveedor::setidalbaranp(QString val) {
    setDBvalue("idalbaranp", val);
    m_listalineas->setColumnValue("idalbaranp", val);
    m_listadescuentos->setColumnValue("idalbaranp", val);
}


void AlbaranProveedor::setnumalbaranp(QString val) {
    setDBvalue("numalbaranp", val);
}


void AlbaranProveedor::setfechaalbaranp(QString val) {
    setDBvalue("fechaalbaranp", val);
}


void AlbaranProveedor::setloginusuario(QString val) {
    setDBvalue("loginusuario", val);
}


void AlbaranProveedor::setcomentalbaranp(QString val) {
    setDBvalue("comentalbaranp", val);
}


void AlbaranProveedor::setidproveedor(QString val) {
    setDBvalue("idproveedor", val);
}


void AlbaranProveedor::setidforma_pago(QString val) {
    setDBvalue("idforma_pago", val);
}


void AlbaranProveedor::setidalmacen(QString val) {
    setDBvalue("idalmacen", val);
}


void AlbaranProveedor::setrefalbaranp(QString val) {
    setDBvalue("refalbaranp", val);
}


void AlbaranProveedor::setdescalbaranp(QString val) {
    setDBvalue("descalbaranp", val);
}


/** Se encarga del borrado en la base de datos de una albaran de proveedor.
    Si se produce algun error devuelve una excepcion.
    Primero llama al borrado de las lineas y luego a los descuentos.
    Por ultimo llama al borrado del registro correspondiente en albaranp.
*/
int AlbaranProveedor::borrar() {
    _depura("AlbaranProveedor::borrar", 0);
    try {
        if (DBvalue("idalbaranp") != "")  {
            empresaBase()->begin();
            m_listalineas->borrar();
            m_listadescuentos->borrar();
            empresaBase()->ejecuta("DELETE FROM albaranp WHERE idalbaranp = " + DBvalue("idalbaranp"));
            empresaBase()->commit();
        } // end if
        _depura("END AlbaranProveedor::borrar", 0);
        return 0;
    } catch (...) {
        mensajeInfo( "Error al borrar el Albaran de Proveedor");
        empresaBase()->rollback();
        return -1;
    } // end catch
}


/** Vacia los datos que puedan haber quedado en la clase DBRecord.
*/
void AlbaranProveedor::vaciaAlbaranProveedor()  {
    _depura("AlbaranProveedor::vaciaAlbaranProveedor", 0);
    DBclear();
    _depura("END AlbaranProveedor::vaciaAlbaranProveedor", 0);
}


/** Se encarga de hacerel pintado de la ficha del albaran.
    Para ello coge todos los campos e invoca a los metodos pinta... que deben estar
    implementados en la clase encargada de la visualizacion.
    Tambien pinta los totales.
*/
void AlbaranProveedor::pintar()  {
    _depura("AlbaranProveedor::pintar", 0);
    pintaidalbaranp(DBvalue("idalbaranp"));
    pintanumalbaranp(DBvalue("numalbaranp"));
    pintafechaalbaranp(DBvalue("fechaalbaranp"));
    pintacomentalbaranp(DBvalue("comentalbaranp"));
    pintaidproveedor(DBvalue("idproveedor"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintaidalmacen(DBvalue("idalmacen"));
    pintarefalbaranp(DBvalue("refalbaranp"));
    pintadescalbaranp(DBvalue("descalbaranp"));
    /// Pintamos los totales.
    calculaypintatotales();
    _depura("END AlbaranProveedor::pintar", 0);
}


/// Esta funcion carga un AlbaranProveedor.
/// Hace el query adecuado, carga el registro a traves de DBRecord.
/// Hace la carga de las lineas y de los descuentos.
/// Invoca al pintado.
int AlbaranProveedor::cargar(QString idbudget) {
    _depura("AlbaranProveedor::cargar", 0);
    QString query = "SELECT * FROM albaranp WHERE idalbaranp =" + idbudget;
    cursor2 * cur = empresaBase()->cargacursor(query);

    if (!cur->eof())
        DBload(cur);

    delete cur;
    pintar();
    m_listalineas->cargar(idbudget);
    m_listadescuentos->cargar(idbudget);
    calculaypintatotales();
    _depura("END AlbaranProveedor::cargar", 0);
    return 0;
}


/** Guarda el albaran de proveedor en la base de datos.
    Para ello hace el guardado de DBRecord y luego guarda las lineas y los descuentos.
    Una vez hecho el guardado se hace una carga para recuperar posibles datos que haya
    introducido la base de datos como la referencia.

    Si algo falla devuelve una excepcion -1.
*/
int AlbaranProveedor::guardar() {
    _depura("AlbaranProveedor::guardar", 0);
    QString id;
    try {
        empresaBase()->begin();
        DBsave(id);
        setidalbaranp(id);
        m_listalineas->guardar();
        m_listadescuentos->guardar();
        empresaBase()->commit();

    /// Hacemos una carga para recuperar la referencia
    cargar(id);

        _depura("END AlbaranProveedor::guardar", 0);
        return 0;
    } catch(...) {
        _depura("AlbaranProveedor::guardar error al guardar", 2);
        empresaBase()->rollback();
        throw -1;
    } // end catch
}

