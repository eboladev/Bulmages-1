//
// C++ Implementation: ListLTarifa
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listltarifa.h"
#include "company.h"
#include "funcaux.h"

ListLTarifa::ListLTarifa(company *comp) {
    m_companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idtarifa="";
    mdb_idalmacen="";
   mdb_idarticulo="";
}// end ListLTarifa


ListLTarifa::ListLTarifa() {
    _depura("Constructor de ListLTarifa\n",0);
    m_companyact=NULL;
    m_lista.setAutoDelete(TRUE);
    mdb_idtarifa="";
    mdb_idalmacen="";
   mdb_idarticulo="";
}// end ListLTarifa


ListLTarifa::~ListLTarifa() {}



LTarifa *ListLTarifa::linpos(int pos) {
    return (m_lista.at(pos));
}// end linpos


// Carga lineas de presupuesto
int ListLTarifa::cargarParaArticulo(QString idarticulo) {
    _depura("ListLTarifa::cargaListLTarifa\n",0);
    int error=0;

	if (m_companyact == NULL) return -1;

    vaciar();
    mdb_idarticulo = idarticulo;
    QString SQLQuery = "SELECT * FROM almacen, (SELECT * FROM articulo WHERE idarticulo ="+mdb_idarticulo+") AS t2 , tarifa ";
	    SQLQuery+= "LEFT JOIN (SELECT * FROM ltarifa WHERE idarticulo="+mdb_idarticulo+") as t1 ON t1.idtarifa=tarifa.idtarifa ";
	    SQLQuery+= "";

    cursor2 * cur= m_companyact->cargacursor(SQLQuery);

    while (!cur->eof())   {
	_depura("hay elementos");
        /// Creamos un elemento del tipo linpresupuesto y lo agregamos a la lista.
        LTarifa *lin = new LTarifa(m_companyact, cur);
        m_lista.append(lin);
        cur->siguienteregistro();
    }// end while
    delete cur;

    /// Tratamiento de excepciones
    if (error) {
        _depura("Error en la carga de LTarifa\n",0);
        return 1;
    }// end if

    _depura("END ListLTarifa::cargar\n",0);
    return 0;
}// end chargeBudgetLines


void ListLTarifa::guardar() {
	_depura("ListLTarifa::guardaListLTarifa",0);
    LTarifa *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
	if (linea->pvpltarifa() != "")
	    linea->guardar();
        i++;
    }// end for
	_depura("END ListLTarifa::guardaListLTarifa",0);
}// en guardaListLTarifa



void ListLTarifa::vaciar() {
    mdb_idtarifa="";
    mdb_idalmacen="";
   mdb_idarticulo="";
    m_lista.clear();
}// end guardaListLTarifa

/*
void ListLTarifa::borrar() {
    if (mdb_idregistroiva != "")  {
        m_companyact->begin();
        int error = m_companyact->ejecuta("DELETE FROM iva WHERE idregistroiva="+mdb_idregistroiva);
        if (error) {
            m_companyact->rollback();
            return;
        }// end if
        m_companyact->commit();
    }// end if
}// end borrar
*/

void ListLTarifa::borrar(int pos) {
    LTarifa *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintar();
}// end borralinpresupuesto
