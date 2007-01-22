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

#include <QTextStream>
#include <QMessageBox>
#include <QFile>

#include "asiento1.h"
#include "fixed.h"
#include "funcaux.h"
#include "plugins.h"
#include "empresa.h"


Asiento1::Asiento1(empresa *comp) : DBRecord (comp) {
    _depura("Asiento1::Asiento1(empresa *)", 0);
    m_companyact = comp;
    setDBTableName("asiento");
    setDBCampoId("idasiento");
    addDBCampo("idasiento", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("Asiento1", "Id asiento"));
    addDBCampo("descripcion", DBCampo::DBvarchar, DBCampo::DBNoSave, QApplication::translate("Asiento1", "Descripcion del asiento"));
    addDBCampo("fecha", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Asiento1", "Fecha del asiento"));
    addDBCampo("comentariosasiento", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Asiento1", "Comentarios del asiento"));
    addDBCampo("ordenasiento", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("Asiento1", "Orden de asiento"));
    addDBCampo("clase", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Asiento1", "Tipo de asiento"));
    listalineas = NULL;
}


Asiento1::~Asiento1() {}


void Asiento1::borraAsiento1() {
    _depura("Asiento1::borraAsiento1", 0);
    int error;
    if (DBvalue("idasiento") != "") {
        switch (QMessageBox::warning(0,
                                     QApplication::translate("Asiento1", "Borrar asiento"),
                                     QApplication::translate("Asiento1", "Se va a borrar el asiento. Esta seguro?"),
                                     QMessageBox::Ok ,
                                     QMessageBox::Cancel )) {
        case QMessageBox::Ok: /// Retry clicked or Enter pressed.
            listalineas->borrar();
            m_companyact->begin();
            error = m_companyact->ejecuta("DELETE FROM apunte WHERE idasiento = " + DBvalue("idasiento"));
            error += m_companyact->ejecuta("DELETE FROM asiento WHERE idasiento = " + DBvalue("idasiento"));
            if (error) {
                m_companyact->rollback();
                return;
            } // end if
            m_companyact->commit();
            vaciaAsiento1();
            break;
        case QMessageBox::Cancel: /// Abort clicked or Escape pressed.
            break;
        } // end switch
    } // end if
    _depura("END Asiento1::borraAsiento1", 0);

}


void Asiento1::vaciaAsiento1() {
    _depura("Asiento1::vaciaAsiento1", 0);
    DBclear();
    listalineas->inicializar();
}


/** Se encarga del pintado del asiento.
*/
/// \TODO: Debe ser pintar()
void Asiento1::pintar() {
    _depura("Asiento1::pintar", 0);
    pintaidasiento(idasiento());
    pintadescripcion(DBvalue("descripcion"));
    pintafecha(DBvalue("fecha"));
    pintacomentariosasiento(DBvalue("comentariosasiento"));
    pintaordenasiento(DBvalue("ordenasiento"));
    pintaclase(DBvalue("clase"));
    /// Pintamos los totales.
    calculaypintatotales(idasiento());
    trataestadoAsiento1();
    _depura("END Asiento1::pintar", 0);
}


/// Esta funci&oacute; carga un asiento.
int Asiento1::cargar(QString idasiento) {
    _depura("Asiento1::cargar", 0, idasiento);
    QString query = "SELECT * FROM asiento WHERE idasiento = " + idasiento;
    cursor2 *cur = m_companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;
    trataestadoAsiento1();
    listalineas->cargar(idasiento);
    pintar();
    _depura("END Asiento1::cargar", 0, idasiento);
    return 0;
}


Fixed Asiento1::totaldebe(QString idasiento) {
    return listalineas->totaldebe(idasiento);
}


Fixed Asiento1::totalhaber(QString idbudget) {
    return listalineas->totalhaber(idbudget);
}


void Asiento1::abreAsiento1() {
    _depura("Asiento1::abreAsiento1", 0);
    if (estadoAsiento1() != ASCerrado)
        return;
    QString id = DBvalue("idasiento");
    if (id == "") {
        _depura("No hay asiento");
        return;
    }
    m_companyact->abreasiento(id.toInt());
    trataestadoAsiento1();
}


void Asiento1::cierraAsiento1() {
    _depura("Asiento1::cierraAsiento1", 0);
    if (estadoAsiento1() != ASAbierto)
        return;
    if (guardar())
        return;
    QString id = DBvalue("idasiento");
    if (id == "") {
        _depura("No hay asiento");
        return;
    }
    m_companyact->cierraasiento(id.toInt());
    QString idasiento = DBvalue("idasiento");
    vaciaAsiento1();
    cargar(idasiento);
    _depura("END Asiento1::cierraasiento1", 0);
}


Asiento1::estadoasiento  Asiento1::estadoAsiento1() {
    if (DBvalue("idasiento") == "")
        return ASVacio;

    QString SQLQuery = "SELECT count(idborrador) AS cuenta FROM borrador WHERE idasiento = " + DBvalue("idasiento");
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    QString numborr = cur->valor("cuenta");
    delete cur;

    SQLQuery = "SELECT count(idapunte) AS cuenta FROM apunte WHERE idasiento = " + DBvalue("idasiento");
    cur = m_companyact->cargacursor(SQLQuery);
    QString numap = cur->valor("cuenta");
    delete cur;

    if (numborr == "0")
        return ASVacio;
    if (numap != "0")
        return ASCerrado;
    return ASAbierto;
}


int Asiento1::guardar() {
    _depura("Asiento1::guardar", 0);
    QString id;
    m_companyact->begin();
    try {
        DBsave(id);
        setidasiento(id);
        listalineas->guardar();
        m_companyact->commit();
        /// Disparamos los plugins
        int res = g_plugins->lanza("Asiento1_guardaAsiento1_post", this);
        if (res != 0)
            return 0;
        _depura("END Asiento1::guardar",0);

        if (estadoAsiento1() == ASCerrado)
            m_companyact->cierraasiento(id.toInt());
        return 0;
    } catch (...) {
        _depura("Error guardando, se cancela la operacion", 1);
        m_companyact->rollback();
        return -1;
    } // end try
}

