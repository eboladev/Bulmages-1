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

#include "comparticulolist.h"
#include "company.h"
#include "comparticulo.h"


ListCompArticulo::ListCompArticulo(company *comp) {
    companyact = comp;
    mdb_idarticulo = "";
}


ListCompArticulo::ListCompArticulo() {
    _depura("Constructor de ListCompArticulo\n", 0);
    companyact = NULL;
    mdb_idarticulo = "";
}


ListCompArticulo::~ListCompArticulo() {}


void ListCompArticulo::nuevalinea(QString idcomponente, QString cantcomparticulo, QString codigocompleto, QString nombre) {
    CompArticulo *lin = new CompArticulo(companyact, mdb_idarticulo, idcomponente, cantcomparticulo, codigocompleto, nombre);
    m_lista.append(lin);
}


CompArticulo *ListCompArticulo::linpos(int pos) {
    return (m_lista.at(pos));
}


/// Carga lineas de presupuesto.
void ListCompArticulo::cargar(QString idarticulo) {
    vaciar();
    _depura("ListCompArticulo::cargaListCompArticulo\n", 0);
    mdb_idarticulo = idarticulo;
    cursor2 * cur= companyact->cargacursor("SELECT * FROM comparticulo, articulo WHERE comparticulo.idarticulo=" + mdb_idarticulo + " AND articulo.idarticulo=comparticulo.idcomponente");
    int i = 0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo CompArticulo y lo agregamos a la lista.
        CompArticulo *lin = new CompArticulo(companyact,
                                             cur->valor("idarticulo"),
                                             cur->valor("idcomponente"),
                                             cur->valor("cantcomparticulo"),
                                             cur->valor("codigocompletoarticulo"),
                                             cur->valor("nomarticulo"));
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    } // end while
    delete cur;
    _depura("END ListCompArticulo::cargar");
}


void ListCompArticulo::guardaListCompArticulo() {
    for (int i = 0; i < m_lista.size(); ++i) {
        m_lista.at(i)->guardaCompArticulo();
    } // end for
}


void ListCompArticulo::vaciar() {
    mdb_idarticulo = "";
    m_lista.clear();
}


void ListCompArticulo::borrar() {
    if (mdb_idarticulo != "")  {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM comparticulo WHERE idarticulo=" + mdb_idarticulo);
        if (error) {
            companyact->rollback();
            return;
        } // end if
        companyact->commit();
    } // end if
}


void ListCompArticulo::borrar(int pos) {
    CompArticulo *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    delete linea;
    m_lista.removeAt(pos);
    pintar();
}

