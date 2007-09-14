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

#include "pedidocliente.h"
#include "company.h"
#include "fixed.h"
#include "funcaux.h"


PedidoCliente::PedidoCliente(Company *comp, QWidget *parent) : FichaBf(comp, parent) {
    _depura("PedidoCliente::PedidoCliente", 0);
    setTitleName(tr("Pedido Cliente"));
    setDBTableName("pedidocliente");
    setDBCampoId("idpedidocliente");
    addDBCampo("idpedidocliente", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("PedidoCliente", "Identificador"));
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("PedidoCliente", "Cliente"));
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("PedidoCliente", "Almacen"));
    addDBCampo("numpedidocliente", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Numero pedido cliente"));
    addDBCampo("fechapedidocliente", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Identificador presupuesto"));
    addDBCampo("descpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Fecha"));
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Forma pago"));
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Trabajador"));
    addDBCampo("contactpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Contacto"));
    addDBCampo("telpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Telefono"));
    addDBCampo("comentpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Comentarios"));
    addDBCampo("procesadopedidocliente", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Procesado"));
    addDBCampo("refpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("PedidoCliente", "Referencia"));
    _depura("END PedidoCliente::PedidoCliente", 0);
}


PedidoCliente::~PedidoCliente() {
    _depura("PedidoCliente::~PedidoCliente", 0);
    _depura("END PedidoCliente::~PedidoCliente", 0);
}


int PedidoCliente::borrar() {
    _depura("PedidoCliente::borrar", 0);
    if (DBvalue("idpedidocliente") != "") {
        empresaBase()->begin();
        m_listalineas->borrar();
        m_listadescuentos->borrar();
        int error = empresaBase()->ejecuta("DELETE FROM pedidocliente WHERE idpedidocliente = " + DBvalue("idpedidocliente"));
        if (error) {
            empresaBase()->rollback();
            return -1;
        } // end if
        empresaBase()->commit();
    } // end if
    _depura("PedidoCliente::borrar", 0);
    return 0;
}


void PedidoCliente::vaciaPedidoCliente() {
    _depura("PedidoCliente::vaciaPedidoCliente", 0);
    DBclear();
    _depura("END PedidoCliente::vaciaPedidoCliente", 0);
}


void PedidoCliente::pintar() {
    _depura("PedidoCliente::pintar", 0);
    pintaidcliente(DBvalue("idcliente"));
    pintaidalmacen(DBvalue("idalmacen"));
    pintaidpedidocliente(DBvalue("idpedidocliente"));
    pintanumpedidocliente(DBvalue("numpedidocliente"));
    pintafechapedidocliente(DBvalue("fechapedidocliente"));
    pintadescpedidocliente(DBvalue("descpedidocliente"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintacomentpedidocliente(DBvalue("comentpedidocliente"));
    pintarefpedidocliente(DBvalue("refpedidocliente"));
    pintaprocesadopedidocliente(DBvalue("procesadopedidocliente"));
    pintacontactpedidocliente(DBvalue("contactpedidocliente"));
    pintatelpedidocliente(DBvalue("telpedidocliente"));
    pintaidtrabajador(DBvalue("idtrabajador"));
    calculaypintatotales();
    _depura("END PedidoCliente::pintar", 0);
}


/// Esta funcion carga un PedidoCliente.
int PedidoCliente::cargar(QString idbudget) {
    _depura("PedidoCliente::cargar", 0);
    QString query = "SELECT * FROM pedidocliente WHERE idpedidocliente = " + idbudget;
    cursor2 *cur = empresaBase()->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;
    m_listalineas->cargar(idbudget);
    m_listadescuentos->cargar(idbudget);
    pintar();
    _depura("END PedidoCliente::cargar", 0);
    return 0;
}

/// Guardamos el pedido cliente.
int PedidoCliente::guardar() {
    _depura("PedidoCliente::guardar", 0);
    QString id;
    empresaBase()->begin();
    try {
        DBsave(id);
        setidpedidocliente(id);
        m_listalineas->guardar();
        m_listadescuentos->guardar();
        empresaBase()->commit();
        /// Hacemos una carga para recuperar los campos Referencia y num.
        cargar(id);
        _depura("END PedidoCliente::guardar", 0);
        return 0;
    } catch (...) {
        _depura("PedidoCliente::guardar se produjo un error al guardar, cancelamos la operacion", 0);
        empresaBase()->rollback();
        throw -1;
    } // end try
}

