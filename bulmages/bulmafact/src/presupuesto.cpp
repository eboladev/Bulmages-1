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

#include "presupuesto.h"
#include "company.h"
#include "configuracion.h"
#include "fixed.h"
#include "funcaux.h"
#include "plugins.h"


Presupuesto::Presupuesto(Company *comp, QWidget *parent) : FichaBf(comp, parent) {
    _depura("Presupuesto::Presupuesto", 0);
    setDBTableName("presupuesto");
    setDBCampoId("idpresupuesto");
    addDBCampo("idpresupuesto", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("Presupuesto", "ID Presupuesto"));
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("Presupuesto", "ID cliente"));
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("Presupuesto", "ID almacen"));
    addDBCampo("numpresupuesto", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Presupuesto", "Numero de Presupuesto"));
    addDBCampo("fpresupuesto", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Presupuesto", "Fecha de creacion"));
    addDBCampo("vencpresupuesto", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Presupuesto", "Fecha de vencimiento"));
    addDBCampo("contactpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Presupuesto", "Persona de contacto"));
    addDBCampo("telpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Presupuesto", "Numero de telefono"));
    addDBCampo("comentpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Presupuesto", "Comentarios"));
    addDBCampo("procesadopresupuesto", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("Presupuesto", "Presupuesto procesado"));
    addDBCampo("descpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Presupuesto", "Descuento"));
    addDBCampo("refpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Presupuesto", "Referencia"));
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Presupuesto", "ID forma de pago"));
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Presupuesto", "ID trabajador"));
    _depura("END Presupuesto::Presupuesto", 0);

}


Presupuesto::~Presupuesto() {
    _depura("Presupuesto::~Presupuesto", 0);
    _depura("END Presupuesto::~Presupuesto", 0);
}


int Presupuesto::borrar() {
    _depura("Presupuesto::borrar", 0);
    if (DBvalue("idpresupuesto") != "") {
        empresaBase()->begin();
        /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
        g_plugins->lanza("Presupuesto_borrar", this);
        m_listalineas->borrar();
        m_listadescuentos->borrar();
        int error = empresaBase()->ejecuta("DELETE FROM presupuesto WHERE idpresupuesto = " + DBvalue("idpresupuesto"));
        if (error) {
            empresaBase()->rollback();
            return -1;
        } // end if
        empresaBase()->commit();
    } // end if
    _depura("END Presupuesto::borrar", 0);
    return 0;
}


void Presupuesto::vaciaPresupuesto() {
    _depura("Presupuesto::vaciaPresupuesto", 0);
    DBclear();
    _depura("END Presupuesto::vaciaPresupuesto", 0);
}


void Presupuesto::pintaPresupuesto() {
    _depura("Presupuesto::pintaPresupuesto", 0);
    pintaidcliente(DBvalue("idcliente"));
    pintaIdAlmacen(DBvalue("idalmacen"));
    pintaNumPresupuesto(DBvalue("numpresupuesto"));
    pintaFPresupuesto(DBvalue("fpresupuesto"));
    pintaVencPresupuesto(DBvalue("vencpresupuesto"));
    pintaContractPresupuesto(DBvalue("contactpresupuesto"));
    pintaTelPresupuesto(DBvalue("telpresupuesto"));
    pintaComentPresupuesto(DBvalue("comentpresupuesto"));
    pintaprocesadoPresupuesto(DBvalue("procesadopresupuesto"));
    pintadescPresupuesto(DBvalue("descpresupuesto"));
    pintarefPresupuesto(DBvalue("refpresupuesto"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintaidalmacen(DBvalue("idalmacen"));
    pintaidtrabajador(DBvalue("idtrabajador"));
    calculaypintatotales();
    _depura("END Presupuesto::pintaPresupuesto", 0);
}


/// Esta funcion carga un Presupuesto.
int Presupuesto::cargar(QString idbudget) {
    _depura("Presupuesto::cargar", 0);
    int error = 0;
    QString query = "SELECT * FROM presupuesto WHERE idpresupuesto = " + idbudget;
    cursor2 * cur= empresaBase()->cargacursor(query);
    if (cur->error())
        error = 1;
    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;


    /// Tratamiento de excepciones.
    if (error) {
        _depura("Error en la carga del Presupuesto\n", 2);
        return 1;
    } // end if

    pintaPresupuesto();

    m_listalineas->cargar(idbudget);
    m_listadescuentos->cargar(idbudget);

    calculaypintatotales();

    _depura("END Presupuesto::cargar", 0);
    return 0;
}


int Presupuesto::guardar() {
    _depura("Presupuesto::guardar", 0);
    QString id;
    empresaBase()->begin();
    try {
        DBsave(id);
        setidPresupuesto(id);
        m_listalineas->guardar();
        m_listadescuentos->guardar();
        /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
        g_plugins->lanza("Presupuesto_guardar_Post", this);
        empresaBase()->commit();
        /// Hacemos una carga para recuperar el numero y la referencia.
        cargar(id);
        _depura("END Presupuesto::guardar", 0);
        return 0;
    } catch (...) {
        _depura("Error guardando. Se cancela la operacion", 0);
        empresaBase()->rollback();
        throw -1;
    } // end try
}


QString Presupuesto::detalleArticulos() {
    _depura("Presupuesto::detalleArticulos", 0);
    QString texto = "";
    cursor2 *cur = empresaBase()->cargacursor("SELECT * FROM lpresupuesto LEFT JOIN articulo ON lpresupuesto.idarticulo = articulo.idarticulo WHERE presentablearticulo AND idpresupuesto=" + DBvalue("idpresupuesto"));
    int i = 0;
    while (!cur->eof()) {
        i = !i;
        if (i) {
            texto += "<blockTable style=\"tabladetalle1\" colWidths=\"5cm, 8cm\" rowHeights=\"5.5cm\">\n";
        } else {
            texto += "<blockTable style=\"tabladetalle2\" colWidths=\"8cm, 5cm\" rowHeights=\"5.5cm\">\n";
        } // end if
        texto += "<tr>\n";
        if (i) {
            texto += "<td><h1>" + cur->valor("nomarticulo") + "</h1>";
            texto += "<para><pre>" + cur->valor("obserarticulo") + "</pre></para></td>\n";
        } // end if
        QString file = confpr->valor(CONF_DIR_IMG_ARTICLES) + cur->valor("codigocompletoarticulo") + ".jpg";
        QFile f(file);
        if (f.exists()) {
            texto += "    <td><illustration x=\"0\" y=\"0\" height=\"5cm\">\n"
                     "        <image file=\"" + confpr->valor(CONF_DIR_IMG_ARTICLES) + cur->valor("codigocompletoarticulo") + ".jpg\" x=\"0\" y=\"0\" height=\"5cm\"/>\n"
                     "        </illustration></td>\n";
        } else {
            texto += "<td></td>\n";
        } // end if
        if (!i) {
            texto += "<td><h1>" + cur->valor("nomarticulo") + "</h1>";
            texto += "<para><pre>" + cur->valor("obserarticulo") + "</pre></para></td>\n";
        } // end if
        texto += "</tr>\n";
        texto += "</blockTable>";
        cur->siguienteregistro();
    } // end while
    delete cur;
    _depura("END Presupuesto::detalleArticulos", 0);
    return texto;
}




