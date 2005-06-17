//
// C++ Implementation: ListDescuentoAlbaranCliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listdescalbarancliente.h"
#include "company.h"
#include "descalbarancliente.h"


ListDescuentoAlbaranCliente::ListDescuentoAlbaranCliente(company *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idalbaran="";
}// end ListDescuentoAlbaranCliente

ListDescuentoAlbaranCliente::ListDescuentoAlbaranCliente() {
    	   fprintf(stderr,"Constructor de ListDescuentoAlbaranCliente\n");
           companyact=NULL;
           m_lista.setAutoDelete(TRUE);
           mdb_idalbaran="";
}// end ListDescuentoAlbaranCliente

ListDescuentoAlbaranCliente::~ListDescuentoAlbaranCliente() {}


void ListDescuentoAlbaranCliente::nuevalinea(QString concept, QString propor) {
        DescuentoAlbaranCliente *lin = new DescuentoAlbaranCliente(companyact,
                              "",
                              concept,
                              propor,
                              mdb_idalbaran);
	m_lista.append(lin);
}// end nuevalinea

DescuentoAlbaranCliente *ListDescuentoAlbaranCliente::linpos(int pos) {
	return (m_lista.at(pos));
}// end linpos


// Carga l�eas de albaran
void ListDescuentoAlbaranCliente::cargaDescuentos(QString idbudget) {
    vaciar();
    fprintf(stderr,"ListDescuentoAlbaranCliente::chargeBudgetLines\n");
    mdb_idalbaran = idbudget;
    fprintf(stderr,"Hacemos la carga del cursor\n");
    cursor2 * cur= companyact->cargacursor("SELECT * FROM dalbaran WHERE idalbaran="+idbudget,"unquery");
    int i=0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo DescuentoAlbaranCliente y lo agregamos a la lista.
        DescuentoAlbaranCliente *lin = new DescuentoAlbaranCliente(companyact,
                              cur->valor("iddalbaran"),
                              cur->valor("conceptdalbaran"),
                              cur->valor("proporciondalbaran"),
                              cur->valor("idalbaran")
                   );
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
    fprintf(stderr,"Fin de ListDescuentoAlbaranCliente::chargeBudgetLines\n");
}// end chargeBudgetLines


void ListDescuentoAlbaranCliente::guardaListDescuentoAlbaranCliente() {
    DescuentoAlbaranCliente *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardaDescuentoAlbaranCliente();
        i++;
    }// end for
}// en guardaListDescuentoAlbaranCliente


 
void ListDescuentoAlbaranCliente::vaciar() {
    mdb_idalbaran = "";
    m_lista.clear();
}// end guardaListDescuentoAlbaranCliente


void ListDescuentoAlbaranCliente::borrar() {
    if (mdb_idalbaran != "")  {
        companyact->begin();
        companyact->ejecuta("DELETE FROM dalbaran WHERE idalbaran="+mdb_idalbaran);
        companyact->commit();
    }// end if
}// end borrar


void ListDescuentoAlbaranCliente::borraDescuentoAlbaranCliente(int pos) {
    DescuentoAlbaranCliente *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListDescuentoAlbaranCliente();
}// end borraDescuentoAlbaranCliente
