//
// C++ Implementation: ListIva
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listiva.h"
#include "empresa.h"
#include "funcaux.h"

ListIva::ListIva(empresa *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idregistroiva="";
}// end ListIva


ListIva::ListIva() {
    _depura("Constructor de ListIva\n",0);
    companyact=NULL;
    m_lista.setAutoDelete(TRUE);
    mdb_idregistroiva="";
}// end ListIva


ListIva::~ListIva() {}



Iva *ListIva::linpos(int pos) {
    return (m_lista.at(pos));
}// end linpos


// Carga lineas de presupuesto
int ListIva::cargaListIva(QString idregistroiva) {
    _depura("ListIva::cargaListIva\n",0);
    int error=0;
    vaciar();
    mdb_idregistroiva = idregistroiva;

    cursor2 * cur= companyact->cargacursor("SELECT * FROM  tipoiva LEFT JOIN (SELECT * FROM iva WHERE idregistroiva="+idregistroiva+" ) AS t1 ON t1.idtipoiva = tipoiva.idtipoiva LEFT JOIN cuenta on tipoiva.idcuenta = cuenta.idcuenta  ORDER BY codigo");
    if (cur->error())
        error=1;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo linpresupuesto y lo agregamos a la lista.
        Iva *lin = new Iva(companyact, cur);
        m_lista.append(lin);
        cur->siguienteregistro();
    }// end while
    delete cur;

    /// Tratamiento de excepciones
    if (error) {
        _depura("Error en la carga de Iva\n",0);
        return 1;
    }// end if

    _depura("END ListIva::cargaListIva\n",0);
    return 0;
}// end chargeBudgetLines


void ListIva::guardaListIva() {
	_depura("ListIva::guardaListIva",0);
    Iva *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
	if (linea->baseiva() != "")
	    linea->guardaIva();
        i++;
    }// end for
	_depura("END ListIva::guardaListIva",0);
}// en guardaListIva

Fixed ListIva::calculabase() {
    Fixed base("0.00");
    Iva *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        base = base + Fixed(linea->baseiva());
        i++;
    }// end for
    return base;
}// end calculabase

Fixed ListIva::calculaiva() {
    Fixed iva("0.00");
    Iva *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        iva = iva + Fixed(linea->ivaiva());
        i++;
    }// end for
    return iva;
}// end calculabase


void ListIva::vaciar() {
    mdb_idregistroiva = "";
    m_lista.clear();
}// end guardaListIva


void ListIva::borrar() {
    if (mdb_idregistroiva != "")  {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM iva WHERE idregistroiva="+mdb_idregistroiva);
        if (error) {
            companyact->rollback();
            return;
        }// end if
        companyact->commit();
    }// end if
}// end borrar


void ListIva::borraIva(int pos) {
    Iva *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListIva();
}// end borralinpresupuesto
