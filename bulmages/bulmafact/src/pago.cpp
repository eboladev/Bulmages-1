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

#include "pago.h"
#include "company.h"
#include "configuracion.h"


Pago::Pago(Company *comp, QWidget *parent) : FichaBf(comp, parent) {
    setDBTableName("pago");
    setDBCampoId("idpago");
    addDBCampo("idpago", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("Pago", "Id pago"));
    addDBCampo("idproveedor", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("Pago", "Id proveedor"));
    addDBCampo("previsionpago", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("Pago", "Previcion de pago"));
    addDBCampo("fechapago", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Pago", "Fecha de pago"));
    addDBCampo("refpago", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Pago", "Referencia de pago"));
    addDBCampo("cantpago", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate("Pago", "Cantidad"));
    addDBCampo("comentpago", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Pago", "Comentario del pago"));
    addDBCampo("idbanco", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Pago", "Banco"));
}


Pago::~Pago() {}


int Pago::borrar() {
    _depura("Pago::borrar", 0);
    if (DBvalue("idpago") != "") {
        empresaBase()->begin();
        int error = empresaBase()->ejecuta("DELETE FROM pago WHERE idpago = " + DBvalue("idpago"));
        if (error) {
            empresaBase()->rollback();
            return -1;
        } // end if
        empresaBase()->commit();
        vaciar();
        pintar();
    } // end if
    _depura("END Pago::borrar", 0);
    return 0;
}


void Pago::vaciar() {
    DBclear();
}


void Pago::pintar() {
    pintaidpago(DBvalue("idpago"));
    pintaidproveedor(DBvalue("idproveedor"));
    pintafechapago(DBvalue("fechapago"));
    pintacantpago(DBvalue("cantpago"));
    pintarefpago(DBvalue("refpago"));
    pintaprevisionpago(DBvalue("previsionpago"));
    pintacomentpago(DBvalue("comentpago"));
    pintaidbanco(DBvalue("idbanco"));
}


/// Esta funcion carga un pago.
int Pago::cargar(QString idpago) {
    _depura("Pago::cargar", 0);
    QString query = "SELECT * FROM pago WHERE idpago = " + idpago;
    cursor2 * cur = empresaBase()->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;
    pintar();
    _depura("Pago::cargar", 0);
    return 0;
}


int Pago::guardar() {
    _depura("Pago::guardar", 0);
    try {
        QString id;
        empresaBase()->begin();
        DBsave(id);
        setidpago(id);
        empresaBase()->commit();

	/// Hacemos una carga para que se actualizen datos como la referencia
	cargar(id);

        _depura("END Pago::guardar", 0);
        return 0;
    } catch (...) {
        mensajeInfo("Error inesperado al guardar");
        empresaBase()->rollback();
	return -1;
    } // end try
}


    void Pago::setidpago(QString val) {
	_depura("Pago::setidpago", 0);
        setDBvalue("idpago", val);
	_depura("END Pago::setidpago", 0);
    }
    void Pago::setidproveedor(QString val) {
	_depura("Pago::setidproveedor", 0);
        setDBvalue("idproveedor", val);
	_depura("END Pago::setidproveedor", 0);
    }
    void Pago::setfechapago(QString val) {
	_depura("Pago::setfechapago", 0);
        setDBvalue("fechapago", val);
	_depura("END Pago::setfechapago", 0);
    }
    void Pago::setcantpago(QString val) {
	_depura("Pago::setcantpago", 0);
        setDBvalue("cantpago", val);
	_depura("END Pago::setcantpago", 0);
    }
    void Pago::setrefpago(QString val) {
	_depura("Pago::setrefpago", 0);
        setDBvalue("refpago", val);
	_depura("END Pago::setrefpago", 0);
    }
    void Pago::setprevisionpago(QString val) {
	_depura("Pago::setprevisionpago", 0);
        setDBvalue("previsionpago", val);
	_depura("END Pago::setprevisionpago", 0);
    }
    void Pago::setcomentpago(QString val) {
	_depura("Pago::setcomentpago", 0);
        setDBvalue("comentpago", val);
	_depura("END Pago::setcomentpago", 0);
    }
    void Pago::setidbanco(QString val) {
	_depura("Pago::setidbanco", 0);
        setDBvalue("idbanco", val);
	_depura("END Pago::setidbanco", 0);
    }
