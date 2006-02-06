//
// C++ Implementation: ListLinAsiento1
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listlinasiento1.h"
#include "empresa.h"


ListLinAsiento1::ListLinAsiento1(empresa *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idasiento="";
}// end ListLinAsiento1

ListLinAsiento1::ListLinAsiento1() {
    	   _depura("Constructor de ListLinAsiento1\n",0);
           companyact=NULL;
           m_lista.setAutoDelete(TRUE);
           mdb_idasiento="";
}// end ListLinAsiento1

ListLinAsiento1::~ListLinAsiento1() {}


LinAsiento1 *ListLinAsiento1::linpos(int pos) {
	return (m_lista.at(pos));
}// end linFactura


// Carga l�eas de Factura
void ListLinAsiento1::cargaListLinAsiento1(QString idbudget) {
    _depura("ListLinAsiento1::cargaListLinAsiento1()\n",0);
    vaciar();
    mdb_idasiento = idbudget;
    cursor2 * cur= companyact->cargacursor("SELECT * FROM borrador, cuenta WHERE idasiento="+idbudget+" AND cuenta.idcuenta=borrador.idcuenta ORDER BY idborrador");
    while (!cur->eof())   {
        /// Creamos un elemento del tipo LinAsiento1 y lo agregamos a la lista.
        LinAsiento1 *lin = new LinAsiento1(companyact,cur);
        m_lista.append(lin);
        cur->siguienteregistro();
    }// end while
    delete cur;
    fprintf(stderr,"Fin de ListLinAsiento1::cargaListLinAsiento1\n");
}// end chargeBudgetLines


void ListLinAsiento1::guardaListLinAsiento1() {
    LinAsiento1 *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardaLinAsiento1();
        i++;
    }// end for
}// en guardaListLinAsiento1


 
void ListLinAsiento1::vaciar() {
    mdb_idasiento = "";
    m_lista.clear();
}// end guardaListLinAsiento1


void ListLinAsiento1::borrar() {
    if (mdb_idasiento != "")  {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM borrador WHERE idborrador="+mdb_idasiento);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
}// end borrar


void ListLinAsiento1::borraLinAsiento1(int pos) {
    LinAsiento1 *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListLinAsiento1();
}// end borraLinAsiento1
