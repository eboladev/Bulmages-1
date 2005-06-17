//
// C++ Implementation: ListLinAlbaranCliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listlinalbarancliente.h"
#include "company.h"
#include "linalbarancliente.h"


ListLinAlbaranCliente::ListLinAlbaranCliente(company *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idalbaran="";
}// end ListLinAlbaranCliente

ListLinAlbaranCliente::ListLinAlbaranCliente() {
    	   fprintf(stderr,"Constructor de ListLinAlbaranCliente\n");
           companyact=NULL;
           m_lista.setAutoDelete(TRUE);
           mdb_idalbaran="";
}// end ListLinAlbaranCliente

ListLinAlbaranCliente::~ListLinAlbaranCliente() {}


void ListLinAlbaranCliente::nuevalinea(QString desclalbaran, QString cantlalbaran, QString pvplalbaran, QString descontlalbaran, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString iva) {
        LinAlbaranCliente *lin = new LinAlbaranCliente(companyact,
                              "",
                              desclalbaran,
                              cantlalbaran,
                              pvplalbaran,
                              descontlalbaran,
                              idarticulo,
                              codigocompletoarticulo,
                              nomarticulo,
                              mdb_idalbaran,
			      iva
			      );
	m_lista.append(lin);
}// end nuevalinea

LinAlbaranCliente *ListLinAlbaranCliente::linpos(int pos) {
	return (m_lista.at(pos));
}// end linpos


// Carga l�eas de Factura
void ListLinAlbaranCliente::cargaListLinAlbaranCliente(QString idbudget) {
    vaciar();
    fprintf(stderr,"ListLinAlbaranCliente::cargaListLinAlbaranCliente(%s)\n", idbudget.ascii());
    mdb_idalbaran = idbudget;
    fprintf(stderr,"Hacemos la carga del cursor\n");
    cursor2 * cur= companyact->cargacursor("SELECT * FROM lalbaran, articulo WHERE idalbaran="+mdb_idalbaran+" AND articulo.idarticulo=lalbaran.idarticulo");
    int i=0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo LinAlbaran y lo agregamos a la lista.
        LinAlbaranCliente *lin = new LinAlbaranCliente(companyact,
                              cur->valor("numlalbaran"),
                              cur->valor("desclalbaran"),
                              cur->valor("cantlalbaran"),
                              cur->valor("pvplalbaran"),
                              cur->valor("descontlalbaran"),
                              cur->valor("idarticulo"),
                              cur->valor("codigocompletoarticulo"),
                              cur->valor("nomarticulo"),
			      cur->valor("idalbaran"),
			      cur->valor("ivalalbaran"));
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
    fprintf(stderr,"Fin de ListLinAlbaranCliente::cargaListLinAlbaranCliente\n");
}// end chargeBudgetLines


void ListLinAlbaranCliente::guardaListLinAlbaranCliente() {
    LinAlbaranCliente *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardaLinAlbaranCliente();
        i++;
    }// end for
}// en guardaListLinAlbaranCliente

 
void ListLinAlbaranCliente::vaciar() {
    mdb_idalbaran = "";
    m_lista.clear();
}// end guardaListLinAlbaranCliente


void ListLinAlbaranCliente::borrar() {
    if (mdb_idalbaran != "")  {
        companyact->begin();
        companyact->ejecuta("DELETE FROM lalbaran WHERE idalbaran="+mdb_idalbaran);
        companyact->commit();
    }// end if
}// end borrar


void ListLinAlbaranCliente::borraLinAlbaranCliente(int pos) {
    LinAlbaranCliente *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListLinAlbaranCliente();
}// end borraLinAlbaran
