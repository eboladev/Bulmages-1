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
#include <QMap>

#include "fixed.h"
#include "funcaux.h"
#include "albarancliente.h"


/** Constructor de la clase que inicializa la clase, prepara la clase DBRecord
    para que trabaje en la base de datos con la tabla albaran y procese adecuadamente
    todos los campos de dicha tabla.    
*/    
AlbaranCliente::AlbaranCliente(company *comp, QWidget *parent) : FichaBf (comp, parent)  {
    _depura("AlbaranCliente::AlbaranCliente", 0);
    setDBTableName("albaran");
    setDBCampoId("idalbaran");
    addDBCampo("idalbaran", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("AlbaranCliente", "Id albaran"));
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("AlbaranCliente", "Id cliente"));
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("AlbaranCliente", "Id almacen"));
    addDBCampo("numalbaran", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Numero de albaran"));
    addDBCampo("fechaalbaran", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Fecha albaran"));
    addDBCampo("contactalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Contacto albaran"));
    addDBCampo("telalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Telefono"));
    addDBCampo("comentalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Comentario"));
    addDBCampo("comentprivalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Comentario priv albaran"));
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Id forma de pago"));
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Id trabajador"));
    addDBCampo("procesadoalbaran", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Procesado albaran"));
    addDBCampo("descalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Descripcion albaran"));
    addDBCampo("refalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Referencia albaran"));
    _depura("END AlbaranCliente::AlbaranCliente", 0);
}


/// Destructor de la clase.
AlbaranCliente::~AlbaranCliente() {
    _depura("AlbaranCliente::~AlbaranCliente", 0);
    _depura("END AlbaranCliente::~AlbaranCliente", 0);
}

/** Metodo de borrar un albaran. Se encarga de mandar a la base de datos
    la instruccion necesaria para el borrado de un albaran y controlar
    los posibles errores que se produzcan.
    Tambi&eacute;n borra todas las lineas y las lineas de descuento que se refieren
    a el albaran que se pretende borrar.
*/
/// \todo: Este metodo deberia poderse delegar en DBRecord, o por lo menos la parte del borrado del registro.
/// \todo: Hace falta meter el metodo dentro del sistema de excepciones try catch.
int AlbaranCliente::borrar() {
    _depura("AlbaranCliente::borrar", 0);
    if (DBvalue("idalbaran") != "")  {
        m_companyact->begin();
        m_listalineas->borrar();
        m_listadescuentos->borrar();
        int error = m_companyact->ejecuta("DELETE FROM albaran WHERE idalbaran = " + DBvalue("idalbaran"));
        if (error)  {
            m_companyact->rollback();
            return -1;
        }
        m_companyact->commit();
    }
    _depura("END AlbaranCliente::borrar", 0);
    return 0;
}

/// Hace un vaciado de los elementos que se corresponden con la base de datos.
/// Es &uacute;til para sistemas de recarga de la pantalla.
void AlbaranCliente::vaciaAlbaranCliente() {
    _depura("AlbaranCliente::vaciaAlbaranCliente", 0);
    DBclear();
    _depura("END AlbaranCliente::vaciaAlbaranCliente", 0);
}

/** Se encarga de sincronizar los elementos de la base de datos con los de la pantalla
    Invocando este m&eacute;todo conseguimos que la pantalla muestre lo que hay en la base
    de datos. No invoca el pintado de las lineas de albar&aacute;n ni descuentos de albar&aacute;n.
    Sin embarco si que provoca el calculo y pintado de los totales.
*/
void AlbaranCliente::pintar() {
    _depura("AlbaranCliente::pintar", 0);
    pintaIdAlbaran(DBvalue("idalbaran"));
    pintaNumAlbaran(DBvalue("numalbaran"));
    pintafechaalbaran(DBvalue("fechaalbaran"));
    pintaComentAlbaran(DBvalue("comentalbaran"));
    pintaComentPrivAlbaran(DBvalue("comentprivalbaran"));
    pintaidcliente(DBvalue("idcliente"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintaidalmacen(DBvalue("idalmacen"));
    pintarefalbaran(DBvalue("refalbaran"));
    pintadescalbaran(DBvalue("descalbaran"));
    pintaidtrabajador(DBvalue("idtrabajador"));
    pintacontactalbaran(DBvalue("contactalbaran"));
    pintatelalbaran(DBvalue("telalbaran"));
    pintaprocesadoalbaran(DBvalue("procesadoalbaran"));
    /// Pinta el subformulario de detalle del AlbaranCliente.
    /// Pintamos los totales.
    calculaypintatotales();
    _depura("END AlbaranCliente::pintar", 0);
}


/** Este m&eacute;todo carga un AlbaranCliente. Tambi&eacute;n carga las lineas
    de albar&aacute;n y los descuentos de albar&aacute;n.
    Tras la carga tambi&eacute;n invoca un repintado del albaran para que se vea
    correctamente la pantalla.
*/
/// \TODO: Falta introducir este m&eacute;todo dentro de sentencias try catch.
int AlbaranCliente::cargar(QString idalbaran)  {
    _depura("AlbaranCliente::cargar", 0);
    QString query = "SELECT * FROM albaran WHERE idalbaran = " + idalbaran;
    cursor2 *cur = m_companyact->cargacursor(query);
    if (!cur->eof())
        DBload(cur);
    delete cur;
    m_listalineas->cargar(idalbaran);
    m_listadescuentos->cargar(idalbaran);
    pintar();
    _depura("Fin AlbaranCliente::cargar", 0);
    return 0;
}

/** Este m&eacute;todo se encarga de hacer el guardado del albar&aacute; en la
    base de datos. Una vez guardado se guardan las lineas de albar&aacute;n y los
    descuentos de albar&aacute;n. Tras hacer el guardado se hace una carga del albaran
    para recuperar los datos que haya podido escribir la base de datos automaticamente.
    Dichos datos son la referencia y el n&uacute;mero de albar&aacute;n.
    
    Si todo funciona bien este m&eacute;todo devuelve 0. Si se produce algun error
    se genera una excepcion -1.
*/    
int AlbaranCliente::guardar() {
    _depura("AlbaranCliente::guardar", 0);
    /// Todo el guardado es una transaccion.
    m_companyact->begin();
    try {
        QString id;
        DBsave(id);
        setidalbaran(id);
        m_listalineas->guardar();
        m_listadescuentos->guardar();
        m_companyact->commit();
        /// Hacemos una carga para recuperar datos como la referencia y el numero de albaran
        cargar(id);

        _depura("END AlbaranCliente::guardar", 0);
        return 0;
    } catch (...) {
        _depura("AlbaranCliente::guardar error al guardar albaran cliente", 0);
        m_companyact->rollback();
        throw -1;
    } // end try
}


