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


Pago::Pago(company *comp) : DBRecord(comp) {
    companyact = comp;
    setDBTableName("pago");
    setDBCampoId("idpago");
    addDBCampo("idpago", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("Pago", "Id pago"));
    addDBCampo("idproveedor", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("Pago", "Id proveedor"));
    addDBCampo("previsionpago", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("Pago", "Previcion de pago"));
    addDBCampo("fechapago", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Pago", "Fecha de pago"));
    addDBCampo("refpago", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Pago", "Referencia de pago"));
    addDBCampo("cantpago", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate("Pago", "Cantidad"));
    addDBCampo("comentpago", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Pago", "Comentario del pago"));
}


Pago::~Pago() {}


void Pago::borraPago() {
    if (DBvalue("idpago") != "") {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM pago WHERE idpago = " + DBvalue("idpago"));
        if (error) {
            companyact->rollback();
            return;
        } // end if
        companyact->commit();
        vaciar();
        pintar();
    } // end if
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
}


/// Esta funcion carga un pago.
int Pago::cargar(QString idpago) {
    QString query = "SELECT * FROM pago WHERE idPago = " + idpago;
    cursor2 * cur = companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;
    pintar();
    return 0;
}


void Pago::guardaPago() {
    try {
        QString id;
        companyact->begin();
        DBsave(id);
        setidpago(id);
        companyact->commit();
    } catch (...) {
        mensajeInfo("Error inesperado al guardar");
        companyact->rollback();
    } // end try
}

