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

#include "listlinpresupuesto.h"
#include "company.h"
#include "linpresupuesto.h"
#include "funcaux.h"

/*

listlinpresupuesto::listlinpresupuesto(company *comp) {
    companyact = comp;
    mdb_idpresupuesto = "";
}


listlinpresupuesto::listlinpresupuesto() {
    _depura("Constructor de listlinpresupuesto\n", 2);
    companyact = NULL;
    mdb_idpresupuesto = "";
}


listlinpresupuesto::~listlinpresupuesto() {}


void listlinpresupuesto::nuevalinea(QString desc, QString cantl, QString pvpl, QString descl, QString idart, QString codart, QString nomart, QString ivapres) {
    linpresupuesto *lin = new linpresupuesto(companyact,
                          "", desc, cantl, pvpl, descl, mdb_idpresupuesto,
                          idart, codart, nomart, ivapres);
    m_lista.append(lin);
}


linpresupuesto *listlinpresupuesto::linpos(int pos) {
    return (m_lista.at(pos));
}


/// Carga l&iacute;neas de presupuesto.
int listlinpresupuesto::chargeBudgetLines(QString idbudget) {
    int error = 0;
    vaciar();
    _depura("listlinpresupuesto::chargeBudgetLines\n", 0);
    mdb_idpresupuesto = idbudget;

    cursor2 * cur = companyact->cargacursor("SELECT * FROM lpresupuesto, articulo WHERE idpresupuesto = " + idbudget + " AND articulo.idarticulo = lpresupuesto.idarticulo ORDER BY idlpresupuesto");
    if (cur->error())
        error=1;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo linpresupuesto y lo agregamos a la lista.
        linpresupuesto *lin = new linpresupuesto(companyact, cur);
        m_lista.append(lin);
        cur->siguienteregistro();
    } // end while
    delete cur;

    /// Tratamiento de excepciones.
    if (error) {
        _depura("Error en la carga de la linea de presupuesto\n",0);
        return 1;
    } // end if

    _depura("Fin de listlinpresupuesto::chargeBudgetLines\n",0);
    return 0;
}


void listlinpresupuesto::guardalistlinpresupuesto() {
    for (int i = 0; i < m_lista.size(); ++i) {
        m_lista.at(i)->guardalinpresupuesto();
    } // end for
}


float listlinpresupuesto::calculabase() {
    float base = 0;
    for (int i = 0; i < m_lista.size(); ++i) {
        base += m_lista.at(i)->calculabase();
    } // end for
    return base;
}


float listlinpresupuesto::calculaiva() {
    float iva = 0;
    for (int i = 0; i < m_lista.size(); ++i) {
        iva += m_lista.at(i)->calculaiva();
    } // end for
    return iva;
}


void listlinpresupuesto::vaciar() {
    mdb_idpresupuesto = "";
    m_lista.clear();
}


void listlinpresupuesto::borrar() {
    if (mdb_idpresupuesto != "")  {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM lpresupuesto WHERE idpresupuesto = " + mdb_idpresupuesto);
        if (error) {
            companyact->rollback();
            return;
        } // end if
        companyact->commit();
    } // end if
}


void listlinpresupuesto::borralinpresupuesto(int pos) {
    linpresupuesto *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    delete linea;
    m_lista.removeAt(pos);
    pintalistlinpresupuesto();
}


*/
