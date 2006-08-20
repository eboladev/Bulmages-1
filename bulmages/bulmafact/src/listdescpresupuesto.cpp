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

#include "listdescpresupuesto.h"
#include "company.h"
#include "descpresupuesto.h"
#include "funcaux.h"


ListDescuentoPresupuesto::ListDescuentoPresupuesto(company *comp) {
    companyact = comp;
    mdb_idpresupuesto = "";
}


ListDescuentoPresupuesto::ListDescuentoPresupuesto() {
    _depura("Constructor de ListDescuentoPresupuesto\n", 2);
    companyact = NULL;
    mdb_idpresupuesto = "";
}


ListDescuentoPresupuesto::~ListDescuentoPresupuesto() {}


void ListDescuentoPresupuesto::nuevalinea(QString concept, QString propor) {
    DescuentoPresupuesto *lin = new DescuentoPresupuesto(companyact,
                                "",
                                concept,
                                propor,
                                mdb_idpresupuesto);
    m_lista.append(lin);
}


DescuentoPresupuesto *ListDescuentoPresupuesto::linpos(int pos) {
    return (m_lista.at(pos));
}


/// Carga lineas de presupuesto.
int ListDescuentoPresupuesto::cargaDescuentos(QString idbudget) {
    int error = 0;
    vaciar();
    _depura("ListDescuentoPresupuesto::chargeBudgetLines\n", 0);
    mdb_idpresupuesto = idbudget;
    _depura("Hacemos la carga del cursor\n", 2);
    cursor2 * cur = companyact->cargacursor("SELECT * FROM dpresupuesto WHERE idpresupuesto=" + idbudget, "unquery");
    if (cur->error())
        error = 1;
    int i = 0;
    while (!cur->eof()) {
        /// Creamos un elemento del tipo DescuentoPresupuesto y lo agregamos a la lista.
        DescuentoPresupuesto *lin = new DescuentoPresupuesto(companyact,
                                    cur->valor("iddpresupuesto"),
                                    cur->valor("conceptdpresupuesto"),
                                    cur->valor("proporciondpresupuesto"),
                                    cur->valor("idpresupuesto")
                                                            );
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    } // end while
    delete cur;
    /// Tratamiento de excepciones.
    if (error) {
        _depura("Error en la carga de la linea de presupuesto\n", 0);
        return 1;
    } // end if
    _depura("Fin de ListDescuentoPresupuesto::chargeBudgetLines\n", 0);
    return 0;
}


void ListDescuentoPresupuesto::guardaListDescuentoPresupuesto() {
    for (int i = 0; i < m_lista.size(); ++i) {
        m_lista.at(i)->guardaDescuentoPresupuesto();
    } // end for
}


void ListDescuentoPresupuesto::vaciar() {
    mdb_idpresupuesto = "";
    m_lista.clear();
}


int ListDescuentoPresupuesto::borrar() {
    if (mdb_idpresupuesto != "")  {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM dpresupuesto WHERE idpresupuesto=" + mdb_idpresupuesto);
        if (error) {
            companyact->rollback();
            return -1;
        } // end if
        companyact->commit();
    } // end if
    return 0;
}


int ListDescuentoPresupuesto::borraDescuentoPresupuesto(int pos) {
    DescuentoPresupuesto *linea;
    linea = m_lista.at(pos);
    int err = linea->borrar();
    if (err)
        return err;
    delete linea;
    m_lista.removeAt(pos);
    pintaListDescuentoPresupuesto();
    return 0;
}

